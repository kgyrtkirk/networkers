/***********************************************************************
GLMatrix - Helper functions to set OpenGL matrices from objects of class
Transformation.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef GLMATRIX_INCLUDED
#define GLMATRIX_INCLUDED

#include <GL/gl.h>

#include "Transformation.h"

inline void glMultMatrix(const Geometry::Transformation& t)
	{
	/* Copy the transformation into a column-major array of entries: */
	GLfloat matrix[16];
	GLfloat* mPtr=matrix;
	for(int j=0;j<4;++j)
		for(int i=0;i<4;++i,++mPtr)
			*mPtr=t(i,j);
	
	/* Multiply the current matrix with the transformation matrix: */
	glMultMatrixf(matrix);
	}

#endif
