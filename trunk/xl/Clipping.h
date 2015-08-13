/***********************************************************************
Clipping - The clipping stage of a 3D graphics pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef CLIPPING_INCLUDED
#define CLIPPING_INCLUDED

#include "HVector.h"
#include "Vertex.h"

/* Forward declarations: */
class PerspectiveDivision;

class Clipping
	{
	/* Embedded classes: */
	public:
	typedef Vertex<3> InputVertex; // Input vertices have color
	typedef Vertex<3> OutputVertex; // Output vertices have color
	
	/* Elements: */
	private:
	PerspectiveDivision* perspectiveDivision; // Pointer to the perspective division pipeline stage
	
	/* Private methods: */
	int clipPolygon(const Geometry::HVector& clipPlane,int numIvs,const InputVertex ivs[],InputVertex ovs[]) const; // Clips a polygon against a clipping plane
	
	/* Constructors and destructors: */
	public:
	Clipping(PerspectiveDivision* sPerspectiveDivision); // Creates a clipping pipeline stage
	~Clipping(void); // Destroys the pipeline stage
	
	/* Methods: */
	void start(void); // Starts rendering; updates the settings of the pipeline stage from the current OpenGL context
	void renderTriangle(const InputVertex& iv1,const InputVertex& iv2,const InputVertex& iv3) const; // Sends a triangle down the pipeline
	void finish(void); // Finishes rendering
	};

#endif
