/***********************************************************************
GLMaterial - Helper functions to set OpenGL material properties from
objects of class Material.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef GLMATERIAL_INCLUDED
#define GLMATERIAL_INCLUDED

#include <GL/gl.h>

#include "Material.h"

inline void glMaterial(GLenum face,const Material& materialProperties)
	{
	/* Set material ambient and diffuse components: */
	glMaterialfv(face,GL_AMBIENT,materialProperties.ambient.getComponents());
	glMaterialfv(face,GL_DIFFUSE,materialProperties.diffuse.getComponents());
	
	/* Set material specular components: */
	glMaterialfv(face,GL_SPECULAR,materialProperties.specular.getComponents());
	glMaterialf(face,GL_SHININESS,materialProperties.shininess);
	
	/* Set material emissive components: */
	glMaterialfv(face,GL_EMISSION,materialProperties.emission.getComponents());
	}

#endif
