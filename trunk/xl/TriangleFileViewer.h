/***********************************************************************
TriangleFileViewer - Subclass of the examiner viewer class to render
lists of triangles read from a triangle description file.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef TRIANGLEFILEVIEWER_INCLUDED
#define TRIANGLEFILEVIEWER_INCLUDED

#include <vector>

#include "Light.h"
#include "TriangleFileParser.h"
#include "ExaminerViewer.h"

/* Forward declarations: */
class ViewingTransform;
class Lighting;
class ProjectionTransform;
class Clipping;
class PerspectiveDivision;
class Rasterization;

class TriangleFileViewer:public ExaminerViewer
	{
	/* Elements: */
	private:
	VertexList vertices; // List of vertices forming triangles
	Light lightSource; // An additional light source
	
	/* The 3D graphics pipeline (bottom-up): */
	Rasterization* rasterization;
	PerspectiveDivision* perspectiveDivision;
	Clipping* clipping;
	ProjectionTransform* projectionTransform;
	Lighting* lighting;
	ViewingTransform* viewingTransform;
	
	/* Rendering mode: */
	bool softwareRendering; // Flag if the triangles are to be rendered using the 3D graphics pipeline
	
	/* Event handling methods from ExaminerViewer: */
	protected:
	virtual int keyboard(int key,const char* text,int textLength,int modifierState);
	
	/* Constructors and destructors: */
	public:
	TriangleFileViewer(int w,int h,const char* label =0);
	TriangleFileViewer(int x,int y,int w,int h,const char* label =0);
	virtual ~TriangleFileViewer(void);
	
	/* Methods from ExaminerViewer: */
	virtual void initScene(void);
	virtual void renderScene(void);
	
	/* New methods: */
	void loadTriangleFile(const char* triangleFileName); // Loads a triangle file
	void setLightPosition(double x,double y,double z); // Sets position of the additional light source
	};

#endif
