/***********************************************************************
Rasterization - The rasterization stage of a 3D graphics pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#include <string.h>
#include <GL/gl.h>

#include "ScanConvertTriangle.h"

#include "Rasterization.h"

/*******************************************************************************
Pixel plotter class to plot colored pixels with z-buffer hidden surface removal:
*******************************************************************************/

/* Set this to non-zero to make the rasterizer go really slow: */
#define SLOOOW 0

class RGBDepthPixelPlotter
	{
	/* Elements: */
	private:
	int bufferSize[2]; // Width and height of z-buffer for display window
	mutable int* buffer; // Pointer to z-buffer for display window
	
	/* Constructors and destructors: */
	public:
	RGBDepthPixelPlotter(const int sBufferSize[2],int* sBuffer)
		:buffer(sBuffer)
		{
		/* Copy z-buffer settings: */
		for(int i=0;i<2;++i)
			bufferSize[i]=sBufferSize[i];
		
		/* Start rendering pixels: */
		#if !SLOOOW
		glBegin(GL_POINTS);
		#endif
		};
	~RGBDepthPixelPlotter(void)
		{
		/* Stop rendering pixels: */
		#if !SLOOOW
		glEnd();
		#endif
		};
	
	/* Methods: */
	void operator()(int x,int y,const float ics[5]) const
		{
		/* Perform depth test: */
		int* bufPtr=&buffer[y*bufferSize[0]+x];
		int depth=int(ics[0]*65536.0f);
		if(*bufPtr<depth) // "Closer" depth is bigger
			{
			/* Fill the pixel: */
			#if SLOOOW
			glBegin(GL_POINTS);
			#endif
			glColor3fv(ics+2);
			glVertex2s(x,y);
			*bufPtr=depth;
			#if SLOOOW
			glEnd();
			#endif
			}
		#if SLOOOW
		glFinish();
		for(int i=0;i<50000;++i)
			;
		#endif
		};
	void operator()(int x,int y,const int ics[5]) const
		{
		/* Perform depth test: */
		int* bufPtr=&buffer[y*bufferSize[0]+x];
		if(*bufPtr<ics[0]) // "Closer" depth is bigger
			{
			/* Fill the pixel: */
			#if SLOOOW
			glBegin(GL_POINTS);
			#endif
			glColor3ub(ics[2]>>8,ics[3]>>8,ics[4]>>8);
			glVertex2s(x,y);
			*bufPtr=ics[0];
			#if SLOOOW
			glEnd();
			#endif
			}
		#if SLOOOW
		glFinish();
		for(int i=0;i<50000;++i)
			;
		#endif
		};
	};

/******************************
Methods of class Rasterization:
******************************/

Rasterization::Rasterization(void)
	:buffer(0),pixelPlotter(0)
	{
	/* Initialize z-buffer: */
	bufferSize[0]=bufferSize[1]=0;
	}

Rasterization::~Rasterization(void)
	{
	/* Destroy z-buffer: */
	delete[] buffer;
	}

void Rasterization::start(const int windowSize[2])
	{
	/* Adjust the z-buffer size: */
	if(bufferSize[0]!=windowSize[0]||bufferSize[1]!=windowSize[1])
		{
		delete[] buffer;
		for(int i=0;i<2;++i)
			bufferSize[i]=windowSize[i];
		buffer=new int[bufferSize[0]*bufferSize[1]];
		}
	
	/* Initialize the z-buffer: */
	memset(buffer,0,sizeof(int)*bufferSize[0]*bufferSize[1]);
	
	/* Set up OpenGL for pixel plotting: */
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0,double(windowSize[0]),0.0,double(windowSize[1]),0.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(0.5,0.5,0.0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDrawBuffer(GL_FRONT_AND_BACK);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* Create a pixel plotter: */
	pixelPlotter=new RGBDepthPixelPlotter(bufferSize,buffer);
	}

void Rasterization::renderTriangle(const Rasterization::InputVertex& iv1,const Rasterization::InputVertex& iv2,const Rasterization::InputVertex& iv3) const
	{
	/* Scan convert the triangle using the z-buffer pixel plotter: */
	scanConvertTriangle(&iv1,&iv2,&iv3,*pixelPlotter);
	}

void Rasterization::finish(void)
	{
	/* Destroy the pixel plotter: */
	delete pixelPlotter;
	
	/* Reset OpenGL settings: */
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	}
