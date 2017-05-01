/*
 *  arPattLoad.c
 *  ARToolKit5
 *
 *  This file is part of ARToolKit.
 *
 *  ARToolKit is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ARToolKit is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with ARToolKit.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, the copyright holders of this library give you
 *  permission to link this library with independent modules to produce an
 *  executable, regardless of the license terms of these independent modules, and to
 *  copy and distribute the resulting executable under terms of your choice,
 *  provided that you also meet, for each linked independent module, the terms and
 *  conditions of the license of that module. An independent module is a module
 *  which is neither derived from nor based on this library. If you modify this
 *  library, you may extend this exception to your version of the library, but you
 *  are not obligated to do so. If you do not wish to do so, delete this exception
 *  statement from your version.
 *
 *  Copyright 2015 Daqri, LLC.
 *  Copyright 2003-2015 ARToolworks, Inc.
 *
 *  Author(s): Hirokazu Kato, Philip Lamb
 *
 */
/*******************************************************
 *
 * Author: Hirokazu Kato
 *
 *         kato@sys.im.hiroshima-cu.ac.jp
 *
 * Revision: 3.0
 * Date: 03/08/13
 *
 *******************************************************/

#include <stdio.h>
#include <math.h>
#include <AR/ar.h>
#include <string.h>

int arPattLoadFromBuffer(ARPattHandle *pattHandle, const char *buffer) {
    
	char   *bufCopy;
    int     patno;
    int     h, i1, i2, i3;
    int     i, j, l, m;
	char   *buffPtr;
	const char *delims = " \t\n\r";
    
    if (!pattHandle) {
        ARLOGe("Error: NULL pattHandle.\n");
        return (-1);
    }
    if (!buffer) {
        ARLOGe("Error: can't load pattern from NULL buffer.\n");
        return (-1);
    }

    for( i = 0; i < pattHandle->patt_num_max; i++ ) {
        if(pattHandle->pattf[i] == 0) break;
    }
    if( i == pattHandle->patt_num_max ) return -1;
    patno = i;

    if (!(bufCopy = strdup(buffer))) { // Make a mutable copy.
        ARLOGe("Error: out of memory.\n");
        return (-1);
    }
	buffPtr = strtok(bufCopy, delims);

    for( h=0; h<4; h++ ) {
        l = 0;
        for( i3 = 0; i3 < 3; i3++ ) { // Three colours B G R
            for( i2 = 0; i2 < pattHandle->pattSize; i2++ ) { // Rows
                for( i1 = 0; i1 < pattHandle->pattSize; i1++ ) { // Columns

					/* Switch file scanning to buffer reading */

                    /* if( fscanf(fp, "%d", &j) != 1 ) {
                        ARLOGe("Pattern Data read error!!\n");
                        return -1;
                    }
					*/

					if (buffPtr == NULL) {
						ARLOGe("Pattern Data read error!!\n");
                        free(bufCopy);
                        return -1;
					}

					j = atoi(buffPtr);
					buffPtr = strtok(NULL, delims);

                    j = 255-j;
                    pattHandle->patt[patno*4 + h][(i2*pattHandle->pattSize+i1)*3+i3] = j;
                    if( i3 == 0 ) pattHandle->pattBW[patno*4 + h][i2*pattHandle->pattSize+i1]  = j;
                    else          pattHandle->pattBW[patno*4 + h][i2*pattHandle->pattSize+i1] += j;
                    if( i3 == 2 ) pattHandle->pattBW[patno*4 + h][i2*pattHandle->pattSize+i1] /= 3;
                    l += j;
                }
            }
        }
        l /= (pattHandle->pattSize*pattHandle->pattSize*3);

        m = 0;
        for( i = 0; i < pattHandle->pattSize*pattHandle->pattSize*3; i++ ) {
            pattHandle->patt[patno*4 + h][i] -= l;
            m += (pattHandle->patt[patno*4 + h][i]*pattHandle->patt[patno*4 + h][i]);
        }
        pattHandle->pattpow[patno*4 + h] = sqrt((ARdouble)m);
        if( pattHandle->pattpow[patno*4 + h] == 0.0 ) pattHandle->pattpow[patno*4 + h] = 0.0000001;

        m = 0;
        for( i = 0; i < pattHandle->pattSize*pattHandle->pattSize; i++ ) {
            pattHandle->pattBW[patno*4 + h][i] -= l;
            m += (pattHandle->pattBW[patno*4 + h][i]*pattHandle->pattBW[patno*4 + h][i]);
        }
        pattHandle->pattpowBW[patno*4 + h] = sqrt((ARdouble)m);
        if( pattHandle->pattpowBW[patno*4 + h] == 0.0 ) pattHandle->pattpowBW[patno*4 + h] = 0.0000001;
    }

    free(bufCopy);

    pattHandle->pattf[patno] = 1;
    pattHandle->patt_num++;

    return( patno );
}

/* Old behaviour: load pattern data by loading the specified file and reading its contents.
 * New behaviour: load file contents and pass to arPattLoadFromBuffer.
 */

int arPattLoad( ARPattHandle *pattHandle, const char *filename )
{
    FILE   *fp;
	int     patno;
    size_t  ret;

	/* Old variables */
    /*	
    int     h, i1, i2, i3;
    int     i, j, l, m;
	*/

	/* New variables */
	long pos = 0;
	char* bytes = NULL;

	/* Open file */
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		ARLOGe("Error opening pattern file '%s' for reading.\n", filename);
        return (-1);
	}

	/* Determine number of bytes in file */
	fseek(fp, 0L, SEEK_END);
	pos = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	//ARLOGd("Pattern file is %ld bytes\n", pos);
    
	/* Allocate buffer */
	bytes = (char *)malloc(pos + 1);
	if (!bytes) {
		ARLOGe("Out of memory!!\n");
        fclose(fp);
		return (-1);
	}

	/* Read pattern into buffer and close file */
	ret = fread(bytes, pos, 1, fp);
	fclose(fp);
    if (ret < 1) {
        ARLOGe("Error reading pattern file '%s'.\n", filename);
        free(bytes);
        return (-1);
    }

	/* Terminate string */
	bytes[pos] = '\0';

	/* Load pattern from buffer */
	patno = arPattLoadFromBuffer(pattHandle, bytes);

	/* Free allocated buffer */
	free(bytes);
	
	return( patno );
}

int arPattFree( ARPattHandle *pattHandle, int patno )
{
    if( pattHandle->pattf[patno] == 0 ) return -1;

    pattHandle->pattf[patno] = 0;
    pattHandle->patt_num--;

    return 1;
}

int arPattActivate( ARPattHandle *pattHandle, int patno )
{
    if( pattHandle->pattf[patno] == 0 ) return -1;

    pattHandle->pattf[patno] = 1;

    return 1;
}

int arPattDeactivate( ARPattHandle *pattHandle, int patno )
{
    if( pattHandle->pattf[patno] == 0 ) return -1;

    pattHandle->pattf[patno] = 2;

    return 1;
}
