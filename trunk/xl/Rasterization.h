/***********************************************************************
Rasterization - The rasterization stage of a 3D graphics pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef RASTERIZATION_INCLUDED
#define RASTERIZATION_INCLUDED

#include "Vertex2D.h"

/* Forward declarations: */
class RGBDepthPixelPlotter;

class Rasterization
	{
	/* Embedded classes: */
	public:
	typedef Vertex2D<5> InputVertex; // Input vertices have inverse weight, depth, and color
	
	/* Elements: */
	private:
	int bufferSize[2]; // Current size of the OpenGL display window (and associated z-buffer)
	mutable int* buffer; // Pointer to the z-buffer
	RGBDepthPixelPlotter* pixelPlotter; // Pointer to pixel plotter
	
	/* Constructors and destructors: */
	public:
	Rasterization(void); // Creates a rasterization pipeline stage
	~Rasterization(void); // Destroys the pipeline stage
	
	/* Methods: */
	void start(const int windowSize[2]); // Starts rendering; updates the settings of the pipeline stage from the current OpenGL context
	void renderTriangle(const InputVertex& iv1,const InputVertex& iv2,const InputVertex& iv3) const; // Sends a triangle down the pipeline
	void finish(void); // Finishes rendering
	};

#endif
