/***********************************************************************
ViewingTransform - The viewing transformation stage of a 3D graphics
pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#include <GL/gl.h>

#include "HVector.h"
#include "GLGetMatrix.h"
#include "Lighting.h"

#include "ViewingTransform.h"

/*********************************
Methods of class ViewingTransform:
*********************************/

ViewingTransform::OutputVertex ViewingTransform::transformVertex(const ViewingTransform::InputVertex& iv) const
	{
	OutputVertex ov;
	
	/* Transform the vertex position: */
	ov.pos=modelview.transform(iv.pos);
	
	/* Copy the vertex color: */
	for(int i=0;i<3;++i)
		ov.properties[i]=iv.properties[i];
	
	/* Transform the normal vector: */
	Geometry::HVector normal=normalTransform.transform(Geometry::HVector(iv.properties[3],iv.properties[4],iv.properties[5],0.0f));
	//Geometry::HVector normal=modelview.transform(Geometry::HVector(iv.properties[3],iv.properties[4],iv.properties[5],0.0f));
	for(int i=0;i<3;++i)
		ov.properties[3+i]=normal[i];
	
	return ov;
	}

ViewingTransform::ViewingTransform(Lighting* sLighting)
	:lighting(sLighting)
	{
	/* Nothing to do... */
	}

ViewingTransform::~ViewingTransform(void)
	{
	/* Nothing to do... */
	}

void ViewingTransform::start(void)
	{
	/* Read the current modelview matrix from OpenGL: */
	modelview=glGetModelviewMatrix();
	
	/* Compute the normal vector transformation matrix: */
	for(int i=0;i<4;++i)
		for(int j=0;j<4;++j)
			normalTransform(i,j)=modelview(j,i);
	normalTransform.doInvert();
	}

void ViewingTransform::renderTriangle(const ViewingTransform::InputVertex& iv1,const ViewingTransform::InputVertex& iv2,const ViewingTransform::InputVertex& iv3) const
	{
	/* Transform the three vertices and pass them on to the next pipeline stage: */
	lighting->renderTriangle(transformVertex(iv1),transformVertex(iv2),transformVertex(iv3));
	}

void ViewingTransform::finish(void)
	{
	/* Nothing to do... */
	}
