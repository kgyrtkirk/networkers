/***********************************************************************
Lighting - The lighting stage of a 3D graphics pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef LIGHTING_INCLUDED
#define LIGHTING_INCLUDED

#include <stddef.h>

#include "Vertex.h"
#include "Color.h"
#include "Light.h"
#include "Material.h"

/* Forward declarations: */
class ProjectionTransform;

class Lighting
	{
	/* Embedded classes: */
	public:
	typedef Vertex<6> InputVertex; // Input vertices have color + normal vectors
	typedef Vertex<3> OutputVertex; // Output vertices have color
	
	/* Elements: */
	private:
	ProjectionTransform* projectionTransform; // Pointer to the projection transformation stage of the pipeline
	Color ambientColor; // Ambient light color
	size_t numLights; // Number of currently active light sources
	Light* lights; // Array of currently active light sources
	int colorMaterialFace; // Current color material face bit mask (0x1 front, 0x2 back)
	int colorMaterialParameter; // Current color material parameter (0 if disabled)
	Material front; // Material property for front faces
	Material back; // Material property for back faces
	
	/* Private methods: */
	OutputVertex lightVertex(const InputVertex& iv) const; // Lights a vertex
	
	/* Constructors and destructors: */
	public:
	Lighting(ProjectionTransform* sProjectionTransform); // Creates a viewing transformation pipeline stage
	~Lighting(void); // Destroys the pipeline stage
	
	/* Methods: */
	void start(void); // Starts rendering; updates the settings of the pipeline stage from the current OpenGL context
	void renderTriangle(const InputVertex& iv1,const InputVertex& iv2,const InputVertex& iv3) const; // Sends a triangle down the pipeline
	void finish(void); // Finishes rendering
	};

#endif
