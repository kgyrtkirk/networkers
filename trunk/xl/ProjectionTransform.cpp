/***********************************************************************
ProjectionTransform - The projection transformation stage of a 3D
graphics pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#include <GL/gl.h>

#include "HVector.h"
#include "GLGetMatrix.h"
#include "Clipping.h"

#include "ProjectionTransform.h"

/************************************
Methods of class ProjectionTransform:
************************************/

ProjectionTransform::OutputVertex ProjectionTransform::transformVertex(const ProjectionTransform::InputVertex& iv) const
	{
	OutputVertex ov;
	ov.pos=projection.transform(iv.pos);
	for(int i=0;i<3;++i)
		ov.properties[i]=iv.properties[i];
	return ov;
	}

ProjectionTransform::ProjectionTransform(Clipping* sClipping)
	:clipping(sClipping)
	{
	/* Nothing to do... */
	}

ProjectionTransform::~ProjectionTransform(void)
	{
	/* Nothing to do... */
	}

void ProjectionTransform::start(void)
	{
	/* Read the current projection matrix from OpenGL: */
	projection=glGetProjectionMatrix();
	}

void ProjectionTransform::renderTriangle(const ProjectionTransform::InputVertex& iv1,const ProjectionTransform::InputVertex& iv2,const ProjectionTransform::InputVertex& iv3) const
	{
	/* Transform the three vertices and pass them on to the next pipeline stage: */
	clipping->renderTriangle(transformVertex(iv1),transformVertex(iv2),transformVertex(iv3));
	}

void ProjectionTransform::finish(void)
	{
	/* Nothing to do... */
	}
