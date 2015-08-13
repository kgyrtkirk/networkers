/***********************************************************************
HVector - Class for 3-dimensional homogenuous vectors. (Simplified
version of HVector class from the Templatized Geometry Library.)
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

#ifndef GEOMETRY_HVECTOR_INCLUDED
#define GEOMETRY_HVECTOR_INCLUDED

#include <math.h>

namespace Geometry {

class HVector
	{
	/* Elements: */
	private:
	float components[4]; // Array of vector components in order (x, y, z, w)
	
	/* Constructors and destructors: */
	public:
	static const HVector zero; // The zero vector (illegal vector with all zero components)
	static const HVector origin; // The zero point (origin of local coordinate system)
	HVector(void) // No initialization
		{
		};
	explicit HVector(float sXyz,float sW) // Fills the component array with a single value and homogeneous weight
		{
		for(int i=0;i<3;++i)
			components[i]=sXyz;
		components[3]=sW;
		};
	HVector(float sX,float sY,float sZ,float sW) // Component-wise constructor
		{
		components[0]=sX;
		components[1]=sY;
		components[2]=sZ;
		components[3]=sW;
		};
	template <class SourceScalarParam>
	explicit HVector(const SourceScalarParam array[4]) // Construction from C-style array
		{
		for(int i=0;i<=3;++i)
			components[i]=array[i];
		};
	static HVector fromVector(float sX,float sY,float sZ) // Constructs homogeneous vector from 3D vector
		{
		return HVector(sX,sY,sZ,0.0f); // Weight is zero for vectors
		};
	static HVector fromPoint(float sX,float sY,float sZ) // Constructs homogeneous vector from 3D point
		{
		return HVector(sX,sY,sZ,1.0f); // Weight is one for points
		};
	
	/* Methods: */
	const float* getComponents(void) const // Returns the array of vector components
		{
		return components;
		};
	float* getComponents(void) // Same, as modifiable array
		{
		return components;
		};
	float operator[](int index) const // Returns one of the vector's components
		{
		return components[index];
		};
	float& operator[](int index) // Same, as modifiable l-value
		{
		return components[index];
		};
	
	/* Vector space operations: */
	HVector operator+(void) const // Unary plus operator; returns copy of vector
		{
		return *this;
		};
	HVector operator-(void) const // Negation operator
		{
		return HVector(-components[0],-components[1],-components[2],-components[3]);
		};
	HVector& operator+=(const HVector& other) // Addition assignment
		{
		for(int i=0;i<=3;++i)
			components[i]+=other.components[i];
		return *this;
		};
	HVector& operator-=(const HVector& other) // Subtraction assignment
		{
		for(int i=0;i<=3;++i)
			components[i]-=other.components[i];
		return *this;
		};
	HVector& operator*=(float scalar) // Scalar multiplication assignment
		{
		for(int i=0;i<=3;++i)
			components[i]*=scalar;
		return *this;
		};
	HVector& operator/=(float scalar) // Scalar division assignment
		{
		for(int i=0;i<=3;++i)
			components[i]/=scalar;
		return *this;
		};
	float dot(const HVector& other) const // Calculates dot product between this and other vector
		{
		float result=0.0f;
		for(int i=0;i<=3;++i)
			result+=components[i]*other.components[i];
		return result;
		};
	
	/* Projective space operations: */
	bool isVector(void) const // Tests whether a homogenuous vector represents a vector
		{
		return components[3]==0.0f;
		};
	bool isPoint(void) const // Tests whether a homogenuous vector represents a point
		{
		return components[3]!=0.0f;
		};
	
	/* Methods for vectors (when weight is zero, result undefined otherwise): */
	float mag(void) const // Returns the magnitude (length) of a vector
		{
		double mag2=0.0;
		for(int i=0;i<3;++i)
			mag2+=double(components[i])*double(components[i]);
		return float(sqrt(mag2));
		};
	HVector normalize(void) const // Returns the vector scaled to unit length
		{
		double mag2=0.0;
		for(int i=0;i<3;++i)
			mag2+=double(components[i])*double(components[i]);
		mag2=sqrt(mag2);
		return HVector(float(components[0]/mag2),float(components[1]/mag2),float(components[2]/mag2),0.0f);
		};
	HVector& doNormalize(void) // Same, but modifies vector itself
		{
		double mag2=0.0;
		for(int i=0;i<3;++i)
			mag2+=double(components[i])*double(components[i]);
		mag2=sqrt(mag2);
		for(int i=0;i<3;++i)
			components[i]/=mag2;
		return *this;
		};
	HVector cross(const HVector& other) const // Calculates cross product between this and other vector
		{
		HVector result;
		result.components[0]=components[1]*other.components[2]-components[2]*other.components[1];
		result.components[1]=components[2]*other.components[0]-components[0]*other.components[2];
		result.components[2]=components[0]*other.components[1]-components[1]*other.components[0];
		result.components[3]=0.0f;
		return result;
		};
	
	/* Methods for points: */
	HVector project(void) const // Projects a vector into affine space by dividing by the homogenuous weight; undefined result if weight is zero
		{
		return HVector(components[0]/components[3],components[1]/components[3],components[2]/components[3],1.0f);
		};
	HVector& doProject(void) // Same, but modifies vector itself
		{
		for(int i=0;i<3;++i)
			components[i]/=components[3];
		components[3]=1.0f;
		return *this;
		};
	};

/**************************************
Operations on objects of class HVector:
**************************************/

inline HVector operator+(const HVector& v1,const HVector& v2) // Adds two vectors
	{
	HVector result=v1;
	return result+=v2;
	}

inline HVector operator-(const HVector& v1,const HVector& v2) // Subtracts two vectors
	{
	HVector result=v1;
	return result-=v2;
	}

inline HVector operator*(const HVector& v,float scalar) // Multiplies a vector and a scalar
	{
	HVector result=v;
	return result*=scalar;
	}

inline HVector operator/(const HVector& v,float scalar) // Divides a vector by a scalar
	{
	HVector result=v;
	return result/=scalar;
	}

inline float operator*(const HVector& v1,const HVector& v2) // Calculates the dot product of two vectors
	{
	return v1.dot(v2);
	}

}

#endif
