/*
Color - Class to represent RGB colors with opacity components.
Copyright (c) 2005 Oliver Kreylos
*/

#ifndef COLOR_INCLUDED
#define COLOR_INCLUDED

class Color
	{
	/* Elements: */
	private:
	float components[4]; // Array of color components in order (red, green, blue, alpha)
	
	/* Constructors and destructors: */
	public:
	static const Color zero; // The zero color (color with all zero components)
	static const Color black; // The black color (all zero components with full opacity)
	static const Color white; // The white color (all one components with full opacity)
	Color(void) // No initialization
		{
		};
	explicit Color(float sRgb,float sAlpha =1.0f) // Fills the component array with a single value and opacity
		{
		for(int i=0;i<3;++i)
			components[i]=sRgb;
		components[3]=sAlpha;
		};
	Color(float sR,float sG,float sB,float sA =1.0f) // Component-wise constructor
		{
		components[0]=sR;
		components[1]=sG;
		components[2]=sB;
		components[3]=sA;
		};
	template <class SourceScalarParam>
	explicit Color(const SourceScalarParam array[4]) // Construction from C-style array
		{
		for(int i=0;i<4;++i)
			components[i]=array[i];
		};
	
	/* Methods: */
	const float* getComponents(void) const // Returns the array of color components
		{
		return components;
		};
	float* getComponents(void) // Same, as modifiable array
		{
		return components;
		};
	float operator[](int index) const // Returns one of the color's components
		{
		return components[index];
		};
	float& operator[](int index) // Same, as modifiable l-value
		{
		return components[index];
		};
	
	/* Color space operations: */
	Color operator+(void) const // Unary plus operator; returns copy of color
		{
		return *this;
		};
	Color& operator+=(const Color& other) // Addition assignment
		{
		for(int i=0;i<4;++i)
			components[i]+=other.components[i];
		return *this;
		};
	Color& operator*=(float scalar) // Scalar multiplication assignment
		{
		for(int i=0;i<4;++i)
			components[i]*=scalar;
		return *this;
		};
	Color& operator/=(float scalar) // Scalar division assignment
		{
		for(int i=0;i<4;++i)
			components[i]/=scalar;
		return *this;
		};
	Color& operator*=(const Color& other) // Filter assignment (component-wise multiplication assignment)
		{
		for(int i=0;i<4;++i)
			components[i]*=other.components[i];
		return *this;
		};
	Color clamp(void) const; // Clamps all color components to [0, 1]
	Color& doClamp(void); // Same, but modifies color itself
	};

/************************************
Operations on objects of class Color:
************************************/

inline Color operator+(const Color& c1,const Color& c2) // Adds two colors
	{
	Color result=c1;
	return result+=c2;
	}

inline Color operator*(const Color& c,float scalar) // Multiplies a color and a scalar
	{
	Color result=c;
	return result*=scalar;
	}

inline Color operator/(const Color& c,float scalar) // Divides a color by a scalar
	{
	Color result=c;
	return result/=scalar;
	}

inline Color operator*(const Color& c1,const Color& c2) // Filters two colors (multiplies component-wise)
	{
	Color result=c1;
	return result*=c2;
	}

#endif
