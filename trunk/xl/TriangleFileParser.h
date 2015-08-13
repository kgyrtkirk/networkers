/***********************************************************************
TriangleFileParser - Parser to create a list of vertices with properties
by parsing a triangle input file. Adapted version of triangle files v2.0
(for project 2).
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#ifndef TRIANGLEFILEPARSER_INCLUDED
#define TRIANGLEFILEPARSER_INCLUDED

#include <vector>

#include "Vertex.h"

typedef std::vector<Vertex<6> > VertexList;

VertexList parseTriangleFile(const char* inputFileName);

#endif
