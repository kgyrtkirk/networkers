/***********************************************************************
ProjectionTransform - The projection transformation stage of a 3D
graphics pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef PROJECTIONTRANSFORM_INCLUDED
#define PROJECTIONTRANSFORM_INCLUDED

#include "Transformation.h"
#include "Vertex.h"

/* Forward declarations: */
class Clipping;

class ProjectionTransform
	{
	/* Embedded classes: */
	public:
	typedef Vertex<3> InputVertex; // Input vertices have color
	typedef Vertex<3> OutputVertex; // Output vertices have color
	
	/* Elements: */
	private:
	Clipping* clipping; // Pointer to the clipping stage of the pipeline
	Geometry::Transformation projection; // The projection matrix
	
	/* Private methods: */
	OutputVertex transformVertex(const InputVertex& iv) const; // Transforms a vertex
	
	/* Constructors and destructors: */
	public:
	ProjectionTransform(Clipping* sClipping); // Creates a projection transformation pipeline stage
	~ProjectionTransform(void); // Destroys the pipeline stage
	
	/* Methods: */
	void start(void); // Starts rendering; updates the settings of the pipeline stage from the current OpenGL context
	void renderTriangle(const InputVertex& iv1,const InputVertex& iv2,const InputVertex& iv3) const; // Sends a triangle down the pipeline
	void finish(void); // Finishes rendering
	};

#endif
