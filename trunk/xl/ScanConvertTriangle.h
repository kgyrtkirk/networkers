#ifndef SCANCONVERTTRIANGLE_INCLUDED
#define SCANCONVERTTRIANGLE_INCLUDED

#include "Vertex2D.h"

/*
Function to scan convert triangles with arbitrary numbers of interpolated components
*/

template <int numIcsParam,class PlotPixelFunctorParam>
void scanConvertTriangle(const Vertex2D<numIcsParam>* v1,const Vertex2D<numIcsParam>* v2,const Vertex2D<numIcsParam>* v3,const PlotPixelFunctorParam& plotPixelFunctor);

#ifndef SCANCONVERTTRIANGLE_IMPLEMENTATION
#include "ScanConvertTriangle.cpp"
#endif

#endif
