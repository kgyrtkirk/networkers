/***********************************************************************
GLGetMaterial - Helper functions to read OpenGL material properties into
objects of class Material.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef GLGETMATERIAL_INCLUDED
#define GLGETMATERIAL_INCLUDED

#include <GL/gl.h>

#include "Material.h"

inline Material glGetMaterial(GLenum face)
	{
	Material result;
	
	/* Read material ambient and diffuse components: */
	glGetMaterialfv(face,GL_AMBIENT,result.ambient.getComponents());
	glGetMaterialfv(face,GL_DIFFUSE,result.diffuse.getComponents());
	
	/* Read material specular components: */
	glGetMaterialfv(face,GL_SPECULAR,result.specular.getComponents());
	glGetMaterialfv(face,GL_SHININESS,&result.shininess);
	
	/* Read material emissive components: */
	glGetMaterialfv(face,GL_EMISSION,result.emission.getComponents());
	
	return result;
	}

#endif
