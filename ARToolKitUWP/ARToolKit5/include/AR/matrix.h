/*
 *	matrix.h
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
 *  Copyright 2002-2015 ARToolworks, Inc.
 *
 *  Author(s): Hirokazu Kato, Philip Lamb
 *
 */

#ifndef AR_MATRIX_H
#define AR_MATRIX_H

#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif


/* === matrix definition ===

  <---- clm --->
  [ 10  20  30 ] ^
  [ 20  10  15 ] |
  [ 12  23  13 ] row
  [ 20  10  15 ] |
  [ 13  14  15 ] v

=========================== */

typedef struct {
	ARdouble *m;
	int      row;
	int      clm;
} ARMat;

#ifdef ARDOUBLE_IS_FLOAT
#  define ARMatf ARMat
#else
typedef struct {
	float   *m;
	int      row;
	int      clm;
} ARMatf;
#endif
    
typedef struct {
    ARdouble *v;
    int      clm;
} ARVec;


/* 0 origin */
#define ARELEM0(mat,r,c) ((mat)->m[(r)*((mat)->clm)+(c)])
/* 1 origin */
#define ARELEM1(mat,row,clm) ARELEM0(mat,row-1,clm-1)



ARMat  *arMatrixAlloc(int row, int clm);
int    arMatrixFree(ARMat *m);
#ifdef ARDOUBLE_IS_FLOAT
#  define arMatrixAllocf arMatrixAlloc
#  define arMatrixFreef arMatrixFree
#else
ARMatf *arMatrixAllocf(int row, int clm);
int    arMatrixFreef(ARMatf *m);
#endif

int    arMatrixDup(ARMat *dest, ARMat *source);
ARMat  *arMatrixAllocDup(ARMat *source);

int    arMatrixUnit(ARMat *unit);
ARMat  *arMatrixAllocUnit(int dim);

int    arMatrixMul(ARMat *dest, ARMat *a, ARMat *b);
ARMat  *arMatrixAllocMul(ARMat *a, ARMat *b);

#ifdef ARDOUBLE_IS_FLOAT
#  define arMatrixMulf arMatrixMul
#  define arMatrixAllocMulf arMatrixAllocMul
#else
int    arMatrixMulf(ARMatf *dest, ARMatf *a, ARMatf *b);
ARMatf *arMatrixAllocMulf(ARMatf *a, ARMatf *b);
#endif

int    arMatrixTrans(ARMat *dest, ARMat *source); // Transpose source, place result in dest.
ARMat  *arMatrixAllocTrans(ARMat *source);        // Transpose source, place result in newly allocated matrix.

#ifdef ARDOUBLE_IS_FLOAT
#  define arMatrixTransf arMatrixTrans
#  define arMatrixAllocTransf arMatrixAllocTrans
#else
int    arMatrixTransf(ARMatf *dest, ARMatf *source);
ARMatf *arMatrixAllocTransf(ARMatf *source);
#endif

int    arMatrixInv(ARMat *dest, ARMat *source);
int    arMatrixSelfInv(ARMat *m);
ARMat  *arMatrixAllocInv(ARMat *source);

#ifdef ARDOUBLE_IS_FLOAT
#  define arMatrixSelfInvf arMatrixSelfInv
#else
int    arMatrixSelfInvf(ARMatf *m);
#endif

ARdouble arMatrixDet(ARMat *m);

int    arMatrixPCA( ARMat *input, ARMat *evec, ARVec *ev, ARVec *mean );
int    arMatrixPCA2( ARMat *input, ARMat *evec, ARVec *ev );

int    arMatrixDisp(ARMat *m);


ARVec  *arVecAlloc( int clm );
int    arVecFree( ARVec *v );
int    arVecDisp( ARVec *v );
ARdouble arVecHousehold( ARVec *x );
ARdouble arVecInnerproduct( ARVec *x, ARVec *y );
int    arVecTridiagonalize( ARMat *a, ARVec *d, ARVec *e );


#ifdef __cplusplus
}
#endif
#endif
