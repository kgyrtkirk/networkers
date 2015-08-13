/***********************************************************************
PerspectiveDivision - The perspective division stage of a 3D graphics
pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#include <GL/gl.h>

#include "Rasterization.h"

#include "PerspectiveDivision.h"

/************************************
Methods of class PerspectiveDivision:
************************************/

PerspectiveDivision::OutputVertex PerspectiveDivision::transformVertex(const PerspectiveDivision::InputVertex& iv) const
	{
	OutputVertex ov;
	
	/* Calculate (x, y) in window coordinates: */
	ov.pos[0]=(iv.pos[0]*viewportScale[0])/iv.pos[3]+viewportOffset[0];
	ov.pos[1]=(iv.pos[1]*viewportScale[1])/iv.pos[3]+viewportOffset[1];
	
	/* Calculate inverse weight: */
	ov.ics[0]=1.0f/iv.pos[3];
	
	/* Calculate z in window coordinates: */
	ov.ics[1]=(1.0f-iv.pos[2]/iv.pos[3])*0.5f; // Map far plane to 0, near plane to 1
	
	/* Copy color values: */
	for(int i=0;i<3;++i)
		ov.ics[i+2]=iv.properties[i];
	
	return ov;
	}

PerspectiveDivision::PerspectiveDivision(Rasterization* sRasterization)
	:rasterization(sRasterization)
	{
	/* Nothing to do... */
	}

PerspectiveDivision::~PerspectiveDivision(void)
	{
	/* Nothing to do... */
	}

void PerspectiveDivision::start(void)
	{
	/* Read viewport size from OpenGL: */
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	
	/* Calculate the viewport transformation: */
	viewportScale[0]=float(viewport[2])*0.5f;
	viewportOffset[0]=float(viewport[0])+viewportScale[0];
	viewportScale[1]=float(viewport[3])*0.5f;
	viewportOffset[1]=float(viewport[1])+viewportScale[1];
	}

void PerspectiveDivision::renderTriangle(const PerspectiveDivision::InputVertex& iv1,const PerspectiveDivision::InputVertex& iv2,const PerspectiveDivision::InputVertex& iv3) const
	{
	/* Transform the three vertices and pass them on to the next pipeline stage: */
	rasterization->renderTriangle(transformVertex(iv1),transformVertex(iv2),transformVertex(iv3));
	}

void PerspectiveDivision::finish(void)
	{
	/* Nothing to do... */
	}
