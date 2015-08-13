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

#ifndef GEOMETRY_TRANSFORMATION_INCLUDED
#define GEOMETRY_TRANSFORMATION_INCLUDED

#include "HVector.h"

namespace Geometry {

class Transformation
	{
	/* Elements: */
	private:
	float matrix[4][4]; // The 2D array of matrix elements
	
	/* Constructors and destructors: */
	public:
	static const Transformation identity; // The identity transformation (represented by the identity matrix)
	Transformation(void) // Creates uninitialized transformation
		{
		};
	explicit Transformation(float sDiagonal); // Creates diagonal matrix using given scalar
	template <class SourceScalarParam>
	static Transformation fromRowMajor(const SourceScalarParam array[16]) // Creates transformation from C-style array in row-major order
		{
		Transformation result;
		for(int i=0;i<=3;++i)
			for(int j=0;j<=3;++j)
				result.matrix[i][j]=array[i*4+j];
		return result;
		};
	template <class SourceScalarParam>
	static Transformation fromColumnMajor(const SourceScalarParam array[16]) // Creates transformation from C-style array in column-major order
		{
		Transformation result;
		for(int i=0;i<=3;++i)
			for(int j=0;j<=3;++j)
				result.matrix[i][j]=array[i+j*4];
		return result;
		};
	
	/* Methods: */
	const float* getMatrix(void) const // Returns the transformation's matrix as a row-major C array
		{
		return &matrix[0][0];
		};
	float* getMatrix(void) // Same, as modifiable array
		{
		return &matrix[0][0];
		};
	float operator()(int row,int column) const // Returns a matrix entry
		{
		return matrix[row][column];
		};
	float& operator()(int row,int column) // Same, as modifiable l-value
		{
		return matrix[row][column];
		};
	
	/* Coordinate frame methods: */
	HVector getBasisVector(int index) const // Returns index-th basis vector of the transformation's coordinate frame
		{
		HVector result;
		for(int i=0;i<=3;++i)
			result[i]=matrix[i][index];
		return result;
		};
	HVector getOrigin(void) const // Returns the origin of the transformation's coordinate frame
		{
		HVector result;
		for(int i=0;i<=3;++i)
			result[i]=matrix[i][3];
		return result;
		};
	Transformation& setBasisVector(int index,const HVector& newBasisVector); // Sets index-th basis vector of the transformation's coordinate frame
	Transformation& setOrigin(const HVector& newOrigin); // Sets the origin of the transformation's coordinate frame
	
	/* Transformation space operations: */
	Transformation& operator*=(const Transformation& other); // Concatenates other transformation from the right
	Transformation& leftMultiply(const Transformation& other); // Concatenates other transformation from the left
	friend Transformation operator*(const Transformation& t1,const Transformation& t2); // Concatenates two transformations
	Transformation invert(void) const; // Returns the inverse of the transformation
	Transformation& doInvert(void); // Same, but modifies transformation itself
	
	/* Transformation operations: */
	HVector transform(const HVector& v) const // Transforms a homogeneous vector by multiplying it with the transformation's matrix
		{
		HVector result;
		for(int i=0;i<=3;++i)
			{
			result[i]=0.0f;
			for(int j=0;j<=3;++j)
				result[i]+=matrix[i][j]*v[j];
			}
		return result;
		};
	HVector inverseTransform(const HVector& v) const; // Transforms a homogeneous vector with the inverse of the transformation
	};

/********************************************
Operations on objects of type Transformation:
********************************************/

Transformation operator*(const Transformation& t1,const Transformation& t2); // Concatenates two transformations

inline HVector operator*(const Transformation& t,const HVector v) // Transforms the vector by the transformation
	{
	return t.transform(v);
	}

inline HVector operator/(const HVector& v,const Transformation& t) // Transforms the vector by the inverse of the transformation
	{
	return t.inverseTransform(v);
	}

}

#endif
