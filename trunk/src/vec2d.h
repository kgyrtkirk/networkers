#ifndef VEC2D_H
#define VEC2D_H

#include <math.h>


class Vec2D
{
	private:
	float __skip[200];
	float components[2];
	float __skip1[200];

	public:
	static const Vec2D zero; // The zero vector (illegal vector with all zero components)
	static const Vec2D origin; // The zero point (origin of local coordinate system)
	Vec2D(void) // No initialization
			{
				memset(__skip,0,sizeof(__skip));
				memset(__skip1,0,sizeof(__skip));
			};
	explicit Vec2D(float sXyz) // Fills the component array with a single value and homogeneous weight
			{
			for(int i=0;i<2;++i)
				components[i]=sXyz;
			memset(__skip,0,sizeof(__skip));
			memset(__skip1,0,sizeof(__skip));
// 			components[3]=sW;
			};
	Vec2D(float sX,float sY)
			{
			components[0]=sX;
			components[1]=sY;
			memset(__skip,0,sizeof(__skip));
			memset(__skip1,0,sizeof(__skip));
			};
	template <class SourceScalarParam>
	explicit Vec2D(const SourceScalarParam array[2]) // Construction from C-style array
			{
			for(int i=0;i<=2;++i)
				components[i]=array[i];
			memset(__skip,0,sizeof(__skip));
			memset(__skip1,0,sizeof(__skip));
			}
// 	static Vec2D 	fromVector(float sX,float sY) // Constructs homogeneous vector from 3D vector
// 			{
// 			return HVector(sX,sY); // Weight is zero for vectors
// 			};
// 	static Vec2D	fromPoint(float sX,float sY) // Constructs homogeneous vector from 3D point
// 			{
// 			return HVector(sX,sY,sZ,1.0f); // Weight is one for points
// 			};

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
	Vec2D	operator+(void) const // Unary plus operator; returns copy of vector
			{
			return *this;
			};
	Vec2D operator-(void) const // Negation operator
			{
			return Vec2D(-components[0],-components[1]);
			};
	Vec2D& operator+=(const Vec2D& other) // Addition assignment
			{
			for(int i=0;i<2;++i)
				components[i]+=other.components[i];
			return *this;
			};
	Vec2D& operator-=(const Vec2D& other) // Subtraction assignment
			{
			for(int i=0;i<2;++i)
				components[i]-=other.components[i];
			return *this;
			};
	Vec2D& operator*=(float scalar) // Scalar multiplication assignment
			{
			for(int i=0;i<2;++i)
				components[i]*=scalar;
			return *this;
			};
	Vec2D& operator/=(float scalar) // Scalar division assignment
			{
			for(int i=0;i<2;++i)
				components[i]/=scalar;
			return *this;
			};
	float dot(const Vec2D& other) const // Calculates dot product between this and other vector
			{
			float result=0.0f;
			for(int i=0;i<=2;++i)
				result+=components[i]*other.components[i];
			return result;
			};
	
	void	rot90()
			{
				float	t;
				t=components[0];
				components[0]=components[1];
				components[1]=-t;
			};

// 	/* Projective space operations: */
// 	bool isVector(void) const // Tests whether a homogenuous vector represents a vector
// 			{
// 			return components[3]==0.0f;
// 			};
// 	bool isPoint(void) const // Tests whether a homogenuous vector represents a point
// 			{
// 			return components[3]!=0.0f;
// 			};

	/* Methods for vectors (when weight is zero, result undefined otherwise): */
	float mag(void) const // Returns the magnitude (length) of a vector
			{
			double mag2=0.0;
			for(int i=0;i<2;++i)
				mag2+=double(components[i])*double(components[i]);
			return float(sqrt(mag2));
			};
	Vec2D	normalize(void) const // Returns the vector scaled to unit length
			{
			double mag2=0.0;
			for(int i=0;i<2;++i)
				mag2+=double(components[i])*double(components[i]);
			mag2=sqrt(mag2);
			return Vec2D(float(components[0]/mag2),float(components[1]/mag2));
			};
	Vec2D& doNormalize(void) // Same, but modifies vector itself
			{
			double mag2=0.0;
			for(int i=0;i<2;++i)
				mag2+=double(components[i])*double(components[i]);
			mag2=sqrt(mag2);
			for(int i=0;i<2;++i)
				components[i]/=mag2;
			return *this;
			};
// 	Vec2D	cross(const Vec2D& other) const // Calculates cross product between this and other vector
// 			{
// 			HVector result;
// 			result.components[0]=components[1]*other.components[2]-components[2]*other.components[1];
// 			result.components[1]=components[2]*other.components[0]-components[0]*other.components[2];
// 			result.components[2]=components[0]*other.components[1]-components[1]*other.components[0];
// 			result.components[3]=0.0f;
// 			return result;
// 			};

	/* Methods for points: */
// 	Vec2D	project(void) const // Projects a vector into affine space by dividing by the homogenuous weight; undefined result if weight is zero
// 			{
// 			return HVector(components[0]/components[3],components[1]/components[3],components[2]/components[3],1.0f);
// 			};
// 	Vec2D& doProject(void) // Same, but modifies vector itself
// 			{
// 			for(int i=0;i<2;++i)
// 				components[i]/=components[3];
// 			components[3]=1.0f;
// 			return *this;
// 			};
	};

/**************************************
Operations on objects of class HVector:
**************************************/

inline Vec2D	operator+(const Vec2D& v1,const Vec2D& v2) // Adds two vectors
	{
	Vec2D	result=v1;
	return result+=v2;
	}

inline Vec2D	operator-(const Vec2D& v1,const Vec2D& v2) // Subtracts two vectors
	{
	Vec2D	result=v1;
	return result-=v2;
	}

inline Vec2D	operator*(const Vec2D& v,float scalar) // Multiplies a vector and a scalar
	{
	Vec2D	result=v;
	return result*=scalar;
	}

inline Vec2D	operator/(const Vec2D& v,float scalar) // Divides a vector by a scalar
	{
	Vec2D	result=v;
	return result/=scalar;
	}

inline float operator*(const Vec2D& v1,const Vec2D& v2) // Calculates the dot product of two vectors
	{
	return v1.dot(v2);
	}


#endif