/***********************************************************************
Vertex - Class to represent three-dimensional vertices with associated
color values and normal vectors.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef VERTEX_INCLUDED
#define VERTEX_INCLUDED

#include "HVector.h"

template <int numPropertiesParam>
struct Vertex
	{
	/* Embedded classes: */
	static const int numProperties=numPropertiesParam; // Number of associated properties of a vertex
	
	/* Elements: */
	public:
	Geometry::HVector pos; // 3D position of vertex as homogeneous vector
	float properties[numProperties]; // Associated properties of vertex
	};

#endif
