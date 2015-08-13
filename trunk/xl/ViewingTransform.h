/***********************************************************************
ViewingTransform - The viewing transformation stage of a 3D graphics
pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef VIEWINGTRANSFORM_INCLUDED
#define VIEWINGTRANSFORM_INCLUDED

#include "Transformation.h"
#include "Vertex.h"

/* Forward declarations: */
class Lighting;

class ViewingTransform
	{
	/* Embedded classes: */
	public:
	typedef Vertex<6> InputVertex; // Input vertices have color + normal vectors
	typedef Vertex<6> OutputVertex; // Output vertices have color + normal vectors
	
	/* Elements: */
	private:
	Lighting* lighting; // Pointer to the lighting stage of the pipeline
	Geometry::Transformation modelview; // The modelview matrix
	Geometry::Transformation normalTransform; // The modelview transformation for normal vectors
	
	/* Private methods: */
	OutputVertex transformVertex(const InputVertex& iv) const; // Transforms a vertex
	
	/* Constructors and destructors: */
	public:
	ViewingTransform(Lighting* sLighting); // Creates a viewing transformation pipeline stage
	~ViewingTransform(void); // Destroys the pipeline stage
	
	/* Methods: */
	void start(void); // Starts rendering; updates the settings of the pipeline stage from the current OpenGL context
	void renderTriangle(const InputVertex& iv1,const InputVertex& iv2,const InputVertex& iv3) const; // Sends a triangle down the pipeline
	void finish(void); // Finishes rendering
	};

#endif
