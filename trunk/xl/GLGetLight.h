/***********************************************************************
GLGetLight - Helper functions to read OpenGL light source properties
into objects of class Light.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef GLGETLIGHT_INCLUDED
#define GLGETLIGHT_INCLUDED

#include <GL/gl.h>

#include "Light.h"

inline Light glGetLight(GLenum light)
	{
	Light result;
	
	/* Read light color components: */
	glGetLightfv(light,GL_AMBIENT,result.ambient.getComponents());
	glGetLightfv(light,GL_DIFFUSE,result.diffuse.getComponents());
	glGetLightfv(light,GL_SPECULAR,result.specular.getComponents());
	
	/* Read light position: */
	glGetLightfv(light,GL_POSITION,result.position.getComponents());
	
	/* Read light spot direction: */
	glGetLightfv(light,GL_SPOT_DIRECTION,result.spotDirection.getComponents()); // Reads only first three components
	result.spotDirection[3]=0.0f;  
	
	/* Read light spot exponent and cut-off angle: */
	glGetLightfv(light,GL_SPOT_EXPONENT,&result.spotExponent);
	glGetLightfv(light,GL_SPOT_CUTOFF,&result.spotCutoff);
	
	/* Read light attenuation coefficients: */
	glGetLightfv(light,GL_CONSTANT_ATTENUATION,&result.constantAttenuation);
	glGetLightfv(light,GL_LINEAR_ATTENUATION,&result.linearAttenuation);
	glGetLightfv(light,GL_QUADRATIC_ATTENUATION,&result.quadraticAttenuation);
	
	return result;
	}

#endif
