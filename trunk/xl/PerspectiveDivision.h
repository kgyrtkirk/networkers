/***********************************************************************
PerspectiveDivision - The perspective division stage of a 3D graphics
pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef PERSPECTIVEDIVISION_INCLUDED
#define PERSPECTIVEDIVISION_INCLUDED

#include "Vertex.h"
#include "Vertex2D.h"

/* Forward declarations: */
class Rasterization;

class PerspectiveDivision
	{
	/* Embedded classes: */
	public:
	typedef Vertex<3> InputVertex; // Input vertices have color
	typedef Vertex2D<5> OutputVertex; // Output vertices have inverse weight, depth, and color
	
	/* Elements: */
	private:
	Rasterization* rasterization; // Pointer to the rasterization pipeline stage
	float viewportScale[2],viewportOffset[2]; // Scaling factor and offset for the viewport transformation
	
	/* Private methods: */
	OutputVertex transformVertex(const InputVertex& iv) const; // Transforms a vertex
	
	/* Constructors and destructors: */
	public:
	PerspectiveDivision(Rasterization* sRasterization); // Creates a perspective division pipeline stage
	~PerspectiveDivision(void); // Destroys the pipeline stage
	
	/* Methods: */
	void start(void); // Starts rendering; updates the settings of the pipeline stage from the current OpenGL context
	void renderTriangle(const InputVertex& iv1,const InputVertex& iv2,const InputVertex& iv3) const; // Sends a triangle down the pipeline
	void finish(void); // Finishes rendering
	};

#endif
