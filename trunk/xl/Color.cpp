/*
Color - Class to represent RGB colors with opacity components.
Copyright (c) 2005 Oliver Kreylos
*/

#include "Color.h"

/******************************
Static elements of class Color:
******************************/

const Color Color::zero(0.0f,0.0f);
const Color Color::black(0.0f,1.0f);
const Color Color::white(1.0f,1.0f);

/**********************
Methods of class Color:
**********************/

Color Color::clamp(void) const
	{
	Color result;
	for(int i=0;i<4;++i)
		{
		if(components[i]<=0.01f)
			result.components[i]=0.01f;
		else if(components[i]>=0.99f)
			result.components[i]=0.99f;
		else
			result.components[i]=components[i];
		}
	return result;
	}

Color& Color::doClamp(void)
	{
	for(int i=0;i<4;++i)
		{
		if(components[i]<=0.01f)
			components[i]=0.01f;
		else if(components[i]>=0.99f)
			components[i]=0.99f;
		}
	return *this;
	}
