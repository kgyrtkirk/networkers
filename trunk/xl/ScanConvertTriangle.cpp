#define SCANCONVERTTRIANGLE_IMPLEMENTATION

#include <math.h>

#include "ScanConvertTriangle.h"

namespace {

/*
Helper structure to perform interpolation along triangle edges
*/

template <int numIcsParam>
struct ActiveEdge
	{
	/* Embedded classes: */
	public:
	static const int numIcs=numIcsParam;
	typedef Vertex2D<numIcsParam> Vertex;
	
	/* Elements: */
	int dx; // Increment of x coordinate when interpolating along an edge
	int x; // Current x coordinate
	int icds[numIcs]; // Increments of interpolated components when interpolating along an edge
	int ics[numIcs]; // Current values of interpolated components
	
	/* Methods: */
	void startEdge(const Vertex& v1,const Vertex& v2,int y) // Starts interpolating along a new edge
		{
		#if 1 // Floatiest version
		/* Calculate y distance from v1 to v2 and from v1 to current scan line: */
		float vy=v2.pos[1]-v1.pos[1];
		float dy=float(y)/65536.0f-v1.pos[1];
		
		/* Calculate edge interpolation increment and initial value of x coordinate: */
		float dxf=(v2.pos[0]-v1.pos[0])/vy;
		dx=int(dxf*65536.0f);
		x=int((v1.pos[0]+dy*dxf)*65536.0f);
		#endif
		#if 0 // Somewhat less floaty
		/* Calculate y distance from v1 to v2 and from v1 to current scan line: */
		float vy=v2.pos[1]-v1.pos[1];
		int dy=y-int(v1.pos[1]*65536.0f);
		
		/* Calculate edge interpolation increment and initial value of x coordinate: */
		float dxf=(v2.pos[0]-v1.pos[0])/vy;
		dx=int(dxf*65536.0f);
		x=int(v1.pos[0]*65536.0f)+((long(dy)*long(dx))>>16);
		#endif
		#if 0 // Least floaty version
		/* Calculate y distance from v1 to v2 and from v1 to current scan line: */
		int y1=int(v1.pos[1]*65536.0f);
		int vy=int(v2.pos[1]*65536.0f)-y1;
		int dy=y-y1;
		
		/* Calculate edge interpolation increment and initial value of x coordinate: */
		int x1=int(v1.pos[0]*65536.0f);
		int dx=(long(int(v2.pos[0]*65536.0f)-x1)<<16)/vy;
		x=x1+((long(dy)*long(dx))>>16);
		#endif
		};
	void startEdgeWithIcs(const Vertex& v1,const Vertex& v2,int y) // Starts interpolating along a new edge; sets up additional value interpolation
		{
		#if 1 // Floatiest version
		/* Calculate y distance from v1 to v2 and from v1 to current scan line: */
		float vy=v2.pos[1]-v1.pos[1];
		float dy=float(y)/65536.0f-v1.pos[1];
		
		/* Calculate edge interpolation increment and initial value of x coordinate: */
		float dxf=(v2.pos[0]-v1.pos[0])/vy;
		dx=int(dxf*65536.0f);
		x=int((v1.pos[0]+dy*dxf)*65536.0f);
		
		/* Calculate edge interpolation increments and initial values of interpolated components: */
		for(int i=0;i<numIcs;++i)
			{
			float icdf=(v2.ics[i]-v1.ics[i])/vy;
			icds[i]=int(icdf*65536.0f);
			ics[i]=int((v1.ics[i]+dy*icdf)*65536.0f);
			}
		#endif
		#if 0 // Somewhat less floaty
		/* Calculate y distance from v1 to v2 and from v1 to current scan line: */
		float vy=v2.pos[1]-v1.pos[1];
		int dy=y-int(v1.pos[1]*65536.0f);
		
		/* Calculate edge interpolation increment and initial value of x coordinate: */
		float dxf=(v2.pos[0]-v1.pos[0])/vy;
		dx=int(dxf*65536.0f);
		x=int(v1.pos[0]*65536.0f)+((long(dy)*long(dx))>>16);
		
		/* Calculate edge interpolation increments and initial values of interpolated components: */
		for(int i=0;i<numIcs;++i)
			{
			float icdf=(v2.ics[i]-v1.ics[i])/vy;
			icds[i]=int(icdf*65536.0f);
			ics[i]=int(v1.ics[i]*65536.0f)+((dy*icds[i])>>16);
			}
		#endif
		#if 0 // Least floaty version
		/* Calculate y distance from v1 to v2 and from v1 to current scan line: */
		int y1=int(v1.pos[1]*65536.0f);
		int vy=int(v2.pos[1]*65536.0f)-y1;
		int dy=y-y1;
		
		/* Calculate edge interpolation increment and initial value of x coordinate: */
		int x1=int(v1.pos[0]*65536.0f);
		int dx=(long(int(v2.pos[0]*65536.0f)-x1)<<16)/vy;
		x=x1+((long(dy)*long(dx))>>16);
		
		/* Calculate edge interpolation increments and initial values of interpolated components: */
		for(int i=0;i<numIcs;++i)
			{
			int ics1=int(v1.ics[i]*65536.0f);
			icds[i]=(long(int(v2.ics[i]*65536.0f)-ics1)<<16)/vy;
			ics[i]=ics1+((long(dy)*long(icds[i]))>>16);
			}
		#endif
		};
	void advanceEdge(void) // Advances interpolation along the same edge
		{
		/* Advance x coordinate: */
		x+=dx;
		};
	void advanceEdgeWithIcs(void) // Advances interpolation along the same edge; interpolates additional values
		{
		/* Advance x coordinate: */
		x+=dx;
		
		/* Advance all interpolated components: */
		for(int i=0;i<numIcs;++i)
			ics[i]+=icds[i];
		};
	};

/*
Helper structure to perform interpolation along scan lines
*/

template <int numIcsParam>
struct Fragment
	{
	/* Embedded classes: */
	public:
	static const int numIcs=numIcsParam;
	typedef Vertex2D<numIcsParam> Vertex;
	typedef ActiveEdge<numIcsParam> xActiveEdge;
	
	/* Elements: */
	float normal; // Normal vector component determining orientation of triangle
	int icds[numIcs]; // Increments of interpolated components when interpolating along a scan line
	int ics[numIcs]; // Current values of interpolated components
	
	/* Constructors and destructors: */
	Fragment(const Vertex& v1,const Vertex& v2,const Vertex& v3)
		:normal((v2.pos[0]-v1.pos[0])*(v3.pos[1]-v1.pos[1])-(v2.pos[1]-v1.pos[1])*(v3.pos[0]-v1.pos[0]))
		{
		/* Check for degenerate triangle: */
		if(normal==0.0f)
			return;
		
		/* Calculate scan line interpolation increments: */
		for(int i=0;i<numIcs;++i)
			icds[i]=int((-((v2.pos[1]-v1.pos[1])*(v3.ics[i]-v1.ics[i])-(v2.ics[i]-v1.ics[i])*(v3.pos[1]-v1.pos[1]))/normal)*65536.0f);
		};
	
	/* Methods: */
	void startScanline(const xActiveEdge& ae,int x) // Starts interpolating along a new scan line
		{
		int dx=x-ae.x;
		
		/* Calculate initial values of interpolated components: */
		for(int i=0;i<numIcs;++i)
			ics[i]=ae.ics[i]+((dx*icds[i])>>16);
		};
	void advanceScanline(void) // Advances interpolation along the same scan line
		{
		/* Advance all interpolated components: */
		for(int i=0;i<numIcs;++i)
			ics[i]+=icds[i];
		};
	};

/*
Helper function to swap values
*/

template <class ValueParam>
inline void swap(ValueParam& v1,ValueParam& v2)
	{
	ValueParam temp=v1;
	v1=v2;
	v2=temp;
	}

}

template <int numIcsParam,class PlotPixelFunctorParam>
void scanConvertTriangle(const Vertex2D<numIcsParam>* v1,const Vertex2D<numIcsParam>* v2,const Vertex2D<numIcsParam>* v3,const PlotPixelFunctorParam& plotPixelFunctor)
	{
	/* Local classes: */
//	static const int numIcs=numIcsParam;
	typedef Vertex2D<numIcsParam> Vertex;
	typedef Fragment<numIcsParam> Fragment;
	typedef ActiveEdge<numIcsParam> xActiveEdge;
	
	/* Calculate a fragment to interpolate the given triangle: */
	Fragment f(*v1,*v2,*v3);
	
	/* Cull degenerate or back-facing triangles: */
	if(f.normal<=0.0f)
		return;
	
	/* Sort vertices according to their y coordinates: */
	if(v1->pos[1]>v2->pos[1])
		swap(v1,v2);
	if(v2->pos[1]>v3->pos[1])
		swap(v2,v3);
	if(v1->pos[1]>v2->pos[1])
		swap(v1,v2);
	int v1y=int(v1->pos[1]*65536.0f);
	int v2y=int(v2->pos[1]*65536.0f);
	int v3y=int(v3->pos[1]*65536.0f);
	
	/*
	Process all scan lines intersecting the triangle:
	*/
	
	xActiveEdge aes[2];
	
	/* Calculate the first intersected scan line: */
	int y=((v1y+32768)&~0xffff)+32768;
	int row=y>>16;
	
	if(y<=v2y)
		{
		/*
		Scan convert the top part of the triangle:
		*/
		
		/* Calculate the triangle's orientation and initialize the two edges starting at v1: */
		bool v2OnLeft=(v2->pos[0]-v1->pos[0])*(v3->pos[1]-v1->pos[1])<=(v3->pos[0]-v1->pos[0])*(v2->pos[1]-v1->pos[1]);
		if(v2OnLeft)
			{
			aes[0].startEdgeWithIcs(*v1,*v2,y);
			aes[1].startEdge(*v1,*v3,y);
			}
		else
			{
			aes[0].startEdgeWithIcs(*v1,*v3,y);
			aes[1].startEdge(*v1,*v2,y);
			}
		
		/* Process all scan lines intersecting the top part of the triangle: */
		do
			{
			/*
			Interpolate the current scan line:
			*/
			
			/* Calculate the first intersected raster column: */
			int x=((aes[0].x+32768)&~0xffff)+32768;
			int col=x>>16;
			
			/* Start interpolating the scan line: */
			f.startScanline(aes[0],x);
			
			while(x<=aes[1].x)
				{
				/* Plot the current pixel: */
				plotPixelFunctor(col,row,f.ics);
				
				/* Go to the next raster column: */
				x+=65536;
				++col;
				f.advanceScanline();
				}
			
			/* Go to the next scan line: */
			y+=65536;
			++row;
			aes[0].advanceEdgeWithIcs();
			aes[1].advanceEdge();
			}
		while(y<=v2y);
		
		if(y<=v3y)
			{
			/*
			Prepare scan converting the bottom part of the triangle:
			*/
			
			/* Initialize the edge starting at v2: */
			if(v2OnLeft)
				aes[0].startEdgeWithIcs(*v2,*v3,y);
			else
				aes[1].startEdge(*v2,*v3,y);
			}
		}
	else if(y<=v3y)
		{
		/*
		Prepare scan converting the bottom part of the triangle:
		*/
		
		bool v1OnLeft=(v3->pos[0]-v1->pos[0])*(v3->pos[1]-v2->pos[1])>=(v3->pos[0]-v2->pos[0])*(v3->pos[1]-v1->pos[1]);
		if(v1OnLeft)
			{
			aes[0].startEdgeWithIcs(*v1,*v3,y);
			aes[1].startEdge(*v2,*v3,y);
			}
		else
			{
			aes[0].startEdgeWithIcs(*v2,*v3,y);
			aes[1].startEdge(*v1,*v3,y);
			}
		}
	
	/*
	Scan convert the bottom part of the triangle:
	*/
	
	/* Process all scan lines intersecting the bottom part of the triangle: */
	while(y<=v3y)
		{
		/*
		Interpolate the current scan line:
		*/
		
		/* Calculate the first intersected raster column: */
		int x=((aes[0].x+32768)&~0xffff)+32768;
		int col=x>>16;
		
		/* Start interpolating the scan line: */
		f.startScanline(aes[0],x);
		
		while(x<=aes[1].x)
			{
			/* Plot the current pixel: */
			plotPixelFunctor(col,row,f.ics);

			/* Go to the next raster column: */
			x+=65536;
			++col;
			f.advanceScanline();
			}
		
		/* Go to the next scan line: */
		y+=65536;
		++row;
		aes[0].advanceEdgeWithIcs();
		aes[1].advanceEdge();
		}
	}
