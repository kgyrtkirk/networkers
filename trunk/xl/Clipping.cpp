/***********************************************************************
Clipping - The clipping stage of a 3D graphics pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#include "PerspectiveDivision.h"

#include "Clipping.h"

/*************************
Methods of class Clipping:
*************************/

int Clipping::clipPolygon(const Geometry::HVector& clipPlane,int numIvs,const Clipping::InputVertex ivs[],Clipping::InputVertex ovs[]) const
	{
	/* Calculate the signed distance of the last input vertex: */
	const InputVertex* v1=&ivs[numIvs-1];
	float d1=clipPlane*v1->pos;
	
	/* Test all edges against the clipping plane and create the output polygon: */
	int numOvs=0;
	for(int i=0;i<numIvs;++i)
		{
		/* Test the current edge against the clipping plane: */
		const InputVertex* v2=&ivs[i];
		float d2=clipPlane*v2->pos;
		
		/* Check if the current edge strictly intersects the clipping plane: */
		if(d1*d2<0.0f)
			{
			/* Add the intersection point to the output polygon: */
			float w1=(d2-0.0f)/(d2-d1);
			float w2=(0.0f-d1)/(d2-d1);
			for(int i=0;i<4;++i)
				ovs[numOvs].pos[i]=v1->pos[i]*w1+v2->pos[i]*w2;
			for(int i=0;i<3;++i)
				ovs[numOvs].properties[i]=v1->properties[i]*w1+v2->properties[i]*w2;
			++numOvs;
			}
		
		/* Check if the current edge end point is on the inside of the clipping plane: */
		if(d2>=0.0f)
			{
			/* Add the current edge end point to the output polygon: */
			ovs[numOvs]=*v2;
			++numOvs;
			}
		
		/* Go to the next edge: */
		v1=v2;
		d1=d2;
		}
	
	/* Return the output polygon: */
	return numOvs;
	}

Clipping::Clipping(PerspectiveDivision* sPerspectiveDivision)
	:perspectiveDivision(sPerspectiveDivision)
	{
	/* Nothing to do... */
	}

Clipping::~Clipping(void)
	{
	/* Nothing to do... */
	}

void Clipping::start(void)
	{
	/* Nothing to do... */
	}

void Clipping::renderTriangle(const Clipping::InputVertex& iv1,const Clipping::InputVertex& iv2,const Clipping::InputVertex& iv3) const
	{
	/* Create a temporary clipping polygon: */
	int numVertices=3;
	InputVertex vertices[2][9]; // 9 is a loose upper bound on the number of vertices in the clipped polygon
	vertices[0][0]=iv1;
	vertices[0][1]=iv2;
	vertices[0][2]=iv3;
	
	/* Clip the triangle against all six clipping planes: */
	static const Geometry::HVector clipPlanes[6]=
		{
		Geometry::HVector( 1.0f, 0.0f, 0.0f,1.0f),Geometry::HVector(-1.0f, 0.0f, 0.0f,1.0f), // Negative and positive x
		Geometry::HVector( 0.0f, 1.0f, 0.0f,1.0f),Geometry::HVector( 0.0f,-1.0f, 0.0f,1.0f), // Negative and positive y
		Geometry::HVector( 0.0f, 0.0f, 1.0f,1.0f),Geometry::HVector( 0.0f, 0.0f,-1.0f,1.0f)  // Negative and positive z
		};
	for(int i=0;i<6;++i)
		{
		numVertices=clipPolygon(clipPlanes[i],numVertices,vertices[i&1],vertices[1-(i&1)]);
		if(numVertices<3)
			return;
		}
	
	/* Pass all resulting triangles on to the next pipeline stage: */
	for(int i=1;i<numVertices-1;++i)
		perspectiveDivision->renderTriangle(vertices[0][0],vertices[0][i],vertices[0][i+1]);
	}

void Clipping::finish(void)
	{
	/* Nothing to do... */
	}
