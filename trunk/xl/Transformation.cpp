/***********************************************************************
Transformation - Class for 3-dimensional projective transformations
represented as 4x4 matrices. (Simplified version of
ProjectiveTransformation class from the Templatized Geometry Library.)
Copyright (c) 2005 Oliver Kreylos

This file is part of the Templatized Geometry Library (TGL).

The Templatized Geometry Library is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The Templatized Geometry Library is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with the Templatized Geometry Library; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA
***********************************************************************/

#include <math.h>

#include "Transformation.h"

namespace Geometry {

namespace {

/**************************************************************************************************
Helper method to solve systems of linear equations using Gaussian elimination with column pivoting:
**************************************************************************************************/

template <int numRowsParam,int numColumnsParam>
inline
void
gaussElimination(
	double matrix[numRowsParam][numColumnsParam])
	{
	/* Perform Gaussian elimination with column pivoting on the extended matrix: */
	for(int step=0;step<numRowsParam-1;++step)
		{
		/* Find the column pivot: */
		double pivot=fabs(matrix[step][step]);
		int pivotRow=step;
		for(int i=step+1;i<numRowsParam;++i)
			{
			double val=fabs(matrix[i][step]);
			if(pivot<val)
				{
				pivot=val;
				pivotRow=i;
				}
			}
		
		/* Swap current and pivot rows if necessary: */
		if(pivotRow!=step)
			{
			/* Swap rows step and pivotRow: */
			for(int j=0;j<numColumnsParam;++j)
				{
				double temp=matrix[step][j];
				matrix[step][j]=matrix[pivotRow][j];
				matrix[pivotRow][j]=temp;
				}
			}
		
		/* Combine all rows with the current row: */
		for(int i=step+1;i<numRowsParam;++i)
			{
			/* Combine rows i and step: */
			double factor=-matrix[i][step]/matrix[step][step];
			for(int j=step+1;j<numColumnsParam;++j)
				matrix[i][j]+=matrix[step][j]*factor;
			}
		}
	}

}

/***************************************
Static elements of class Transformation:
***************************************/

const Transformation Transformation::identity=Transformation(1.0f);

/*******************************
Methods of class Transformation:
*******************************/

Transformation::Transformation(float sDiagonal)
	{
	/* Set the matrix to all zeros first: */
	for(int i=0;i<=3;++i)
		for(int j=0;j<=3;++j)
			matrix[i][j]=0.0f;
	
	/* Set the diagonal elements: */
	for(int i=0;i<=3;++i)
		matrix[i][i]=sDiagonal;
	}

Transformation& Transformation::setBasisVector(int index,const HVector& newBasisVector)
	{
	for(int i=0;i<=3;++i)
		matrix[i][index]=newBasisVector[i];
	return *this;
	}

Transformation& Transformation::setOrigin(const HVector& newOrigin)
	{
	for(int i=0;i<=3;++i)
		matrix[i][3]=newOrigin[i];
	return *this;
	}

Transformation& Transformation::operator*=(const Transformation& other)
	{
	/* Calculate the matrix product into a temporary matrix: */
	float temp[4][4];
	for(int i=0;i<=3;++i)
		for(int j=0;j<=3;++j)
			{
			temp[i][j]=0.0f;
			for(int k=0;k<=3;++k)
				temp[i][j]+=matrix[i][k]*other.matrix[k][j];
			}
	
	/* Set the matrix from the temporary matrix: */
	for(int i=0;i<=3;++i)
		for(int j=0;j<=3;++j)
			matrix[i][j]=temp[i][j];
	return *this;
	}

Transformation& Transformation::leftMultiply(const Transformation& other)
	{
	/* Calculate the matrix product into a temporary matrix: */
	float temp[4][4];
	for(int i=0;i<=3;++i)
		for(int j=0;j<=3;++j)
			{
			temp[i][j]=0.0f;
			for(int k=0;k<=3;++k)
				temp[i][j]+=other.matrix[i][k]*matrix[k][j];
			}
	
	/* Set the matrix from the temporary matrix: */
	for(int i=0;i<=3;++i)
		for(int j=0;j<=3;++j)
			matrix[i][j]=temp[i][j];
	return *this;
	}

Transformation operator*(const Transformation& t1,const Transformation& t2)
	{
	/* Calculate the matrix product directly into the result transformation: */
	Transformation result;
	for(int i=0;i<=3;++i)
		for(int j=0;j<=3;++j)
			{
			result.matrix[i][j]=0.0f;
			for(int k=0;k<=3;++k)
				result.matrix[i][j]+=t1.matrix[i][k]*t2.matrix[k][j];
			}
	return result;
	}

Transformation Transformation::invert(void) const
	{
	/* Create the extended matrix: */
	double temp[4][8];
	for(int i=0;i<=3;++i)
		for(int j=0;j<=3;++j)
			{
			temp[i][j]=double(matrix[i][j]);
			temp[i][4+j]=i==j?1.0:0.0;
			}
	
	/* Perform Gaussian elimination with column pivoting on the extended matrix: */
	gaussElimination<4,8>(temp);
	
	/* Calculate the result matrix: */
	Transformation result;
	for(int i=3;i>=0;--i)
		for(int j=0;j<=3;++j)
			{
			for(int k=i+1;k<=3;++k)
				temp[i][4+j]-=temp[i][k]*temp[k][4+j];
			temp[i][4+j]/=temp[i][i];
			result.matrix[i][j]=float(temp[i][4+j]);
			}
	return result;
	}

Transformation& Transformation::doInvert(void)
	{
	/* Create the extended matrix: */
	double temp[4][8];
	for(int i=0;i<=3;++i)
		for(int j=0;j<=3;++j)
			{
			temp[i][j]=double(matrix[i][j]);
			temp[i][4+j]=i==j?1.0:0.0;
			}
	
	/* Perform Gaussian elimination with column pivoting on the extended matrix: */
	gaussElimination<4,8>(temp);
	
	/* Calculate the result matrix: */
	for(int i=3;i>=0;--i)
		for(int j=0;j<=3;++j)
			{
			for(int k=i+1;k<=3;++k)
				temp[i][4+j]-=temp[i][k]*temp[k][4+j];
			temp[i][4+j]/=temp[i][i];
			matrix[i][j]=float(temp[i][4+j]);
			}
	return *this;
	}

HVector Transformation::inverseTransform(const HVector& v) const
	{
	/* Create the extended matrix: */
	double temp[4][5];
	for(int i=0;i<=3;++i)
		{
		for(int j=0;j<=3;++j)
			temp[i][j]=double(matrix[i][j]);
		temp[i][4]=double(v[i]);
		}
	
	/* Perform Gaussian elimination with column pivoting on the extended matrix: */
	gaussElimination<4,5>(temp);
	
	/* Calculate the result vector: */
	HVector result;
	for(int i=3;i>=0;--i)
		{
		for(int j=i+1;j<=3;++j)
			temp[i][4]-=temp[i][j]*temp[j][4];
		temp[i][4]/=temp[i][i];
		result[i]=float(temp[i][4]);
		}
	return result;
	}

}
