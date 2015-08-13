/***********************************************************************
GLGetMatrix - Helper functions to read OpenGL matrices into objects of
class Transformation.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef GLGETMATRIX_INCLUDED
#define GLGETMATRIX_INCLUDED

#include <GL/gl.h>

#include "Transformation.h"

inline Geometry::Transformation glGetMatrix(GLenum pname) // Returns any matrix (not safe to use directly)
	{
	GLfloat matrix[16];
	glGetFloatv(pname,matrix);
	return Geometry::Transformation::fromColumnMajor(matrix);
	}

inline Geometry::Transformation glGetModelviewMatrix(void) // Returns the current modelview matrix
	{
	return glGetMatrix(GL_MODELVIEW_MATRIX);
	}

inline Geometry::Transformation glGetProjectionMatrix(void) // Returns the current projection matrix
	{
	return glGetMatrix(GL_PROJECTION_MATRIX);
	}

#endif
