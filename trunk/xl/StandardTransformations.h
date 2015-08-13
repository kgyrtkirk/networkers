/***********************************************************************
StandardTransformations - Helper functions to generate "standard"
transformations such as translations, rotations, scalings, etc., as
objects of class Transformation.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef STANDARDTRANSFORMATIONS_INCLUDED
#define STANDARDTRANSFORMATIONS_INCLUDED

#include "Transformation.h"

namespace Geometry {

Transformation translate(float tx,float ty,float tz); // Returns translation by given vector
Transformation rotateX(float angle); // Returns rotation around x axis by given angle in radians
Transformation rotateY(float angle); // Returns rotation around y axis by given angle in radians
Transformation rotateZ(float angle); // Returns rotation around z axis by given angle in radians
Transformation rotateAxis(float angle,float ax,float ay,float az); // Returns rotation around given axis by given angle in radians
Transformation rotate(float ax,float ay,float az); // Returns rotation around given axis, by angle defined by length of axis in radians
Transformation scale(float s); // Returns uniform scaling by the given factor
Transformation scale(float sx,float sy,float sz); // Returns non-uniform scaling by the given factors

}

#endif
