/***********************************************************************
StandardTransformations - Helper functions to generate "standard"
transformations such as translations, rotations, scalings, etc., as
objects of class Transformation.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#include "StandardTransformations.h"

namespace Geometry {

Transformation translate(float tx,float ty,float tz)
	{
	Transformation result=Transformation::identity;
	
	result(0,3)=tx;
	result(1,3)=ty;
	result(2,3)=tz;
	
	return result;
	}

Transformation rotateX(float angle)
	{
	Transformation result=Transformation::identity;
	
	float c=cosf(angle);
	float s=sinf(angle);
	
	result(1,1)=c;
	result(2,1)=s;
	result(1,2)=-s;
	result(2,2)=c;
	
	return result;
	}

Transformation rotateY(float angle)
	{
	Transformation result=Transformation::identity;
	
	float c=cosf(angle);
	float s=sinf(angle);
	
	result(0,0)=c;
	result(2,0)=-s;
	result(0,2)=s;
	result(2,2)=c;
	
	return result;
	}

Transformation rotateZ(float angle)
	{
	Transformation result=Transformation::identity;
	
	float c=cosf(angle);
	float s=sinf(angle);
	
	result(0,0)=c;
	result(1,0)=s;
	result(0,1)=-s;
	result(1,1)=c;
	
	return result;
	}

Transformation rotateAxis(float angle,float ax,float ay,float az)
	{
	Transformation result=Transformation::identity;
	
	float axisLen=sqrtf(ax*ax+ay*ay+az*az);
	if(axisLen!=0.0f)
		{
		ax/=axisLen;
		ay/=axisLen;
		az/=axisLen;
		
		float c=cosf(angle);
		float s=sinf(angle);
		
		result(0,0)=(1.0f-c)*(ax*ax)+c;
		result(0,1)=(1.0f-c)*(ax*ay)-az*s;
		result(0,2)=(1.0f-c)*(ax*az)+ay*s;
		result(1,0)=(1.0f-c)*(ay*ax)+az*s;
		result(1,1)=(1.0f-c)*(ay*ay)+c;
		result(1,2)=(1.0f-c)*(ay*az)-ax*s;
		result(2,0)=(1.0f-c)*(az*ax)-ay*s;
		result(2,1)=(1.0f-c)*(az*ay)+ax*s;
		result(2,2)=(1.0f-c)*(az*az)+c;
		}
	
	return result;
	}

Transformation rotate(float ax,float ay,float az)
	{
	Transformation result=Transformation::identity;
	
	float axisLen=sqrtf(ax*ax+ay*ay+az*az);
	if(axisLen!=0.0f)
		{
		ax/=axisLen;
		ay/=axisLen;
		az/=axisLen;
		
		float c=cosf(axisLen);
		float s=sinf(axisLen);
		
		result(0,0)=(1.0f-c)*(ax*ax)+c;
		result(0,1)=(1.0f-c)*(ax*ay)-az*s;
		result(0,2)=(1.0f-c)*(ax*az)+ay*s;
		result(1,0)=(1.0f-c)*(ay*ax)+az*s;
		result(1,1)=(1.0f-c)*(ay*ay)+c;
		result(1,2)=(1.0f-c)*(ay*az)-ax*s;
		result(2,0)=(1.0f-c)*(az*ax)-ay*s;
		result(2,1)=(1.0f-c)*(az*ay)+ax*s;
		result(2,2)=(1.0f-c)*(az*az)+c;
		}
	
	return result;
	}

Transformation scale(float s)
	{
	Transformation result=Transformation::identity;
	
	result(0,0)=s;
	result(1,1)=s;
	result(2,2)=s;
	
	return result;
	}

Transformation scale(float sx,float sy,float sz)
	{
	Transformation result=Transformation::identity;
	
	result(0,0)=sx;
	result(1,1)=sy;
	result(2,2)=sz;
	
	return result;
	}

}
