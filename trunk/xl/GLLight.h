/***********************************************************************
GLLight - Helper functions to set OpenGL light source properties from
objects of class Light.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef GLLIGHT_INCLUDED
#define GLLIGHT_INCLUDED

#include <GL/gl.h>

#include "Light.h"

inline void glLight(GLenum light,const Light& lightProperties)
	{
	/* Set light color components: */
	glLightfv(light,GL_AMBIENT,lightProperties.ambient.getComponents());
	glLightfv(light,GL_DIFFUSE,lightProperties.diffuse.getComponents());
	glLightfv(light,GL_SPECULAR,lightProperties.specular.getComponents());
	
	/* Set light position: */
	glLightfv(light,GL_POSITION,lightProperties.position.getComponents());
	
	/* Set light spot direction: */
	glLightfv(light,GL_SPOT_DIRECTION,lightProperties.spotDirection.getComponents()); // Sets only first three components
	
	/* Set light spot exponent and cut-off angle: */
	glLightf(light,GL_SPOT_EXPONENT,lightProperties.spotExponent);
	glLightf(light,GL_SPOT_CUTOFF,lightProperties.spotCutoff);
	
	/* Set light attenuation coefficients: */
	glLightf(light,GL_CONSTANT_ATTENUATION,lightProperties.constantAttenuation);
	glLightf(light,GL_LINEAR_ATTENUATION,lightProperties.linearAttenuation);
	glLightf(light,GL_QUADRATIC_ATTENUATION,lightProperties.quadraticAttenuation);
	}

#endif
