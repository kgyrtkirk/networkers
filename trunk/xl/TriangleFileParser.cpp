/***********************************************************************
TriangleFileParser - Parser to create a list of vertices with properties
by parsing a triangle input file. Adapted version of triangle files v2.0
(for project 2).
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdexcept>

#include "TriangleFileParser.h"

namespace {

/*******************************************************
Helper method to compare an extracted token to a string:
*******************************************************/

inline bool cmpToken(const char* token,size_t tokenLength,const char* pattern)
	{
	return tokenLength==strlen(pattern)&&memcmp(token,pattern,tokenLength)==0;
	}

}

VertexList parseTriangleFile(const char* inputFileName)
	{
	/* Open input file: */
	FILE* inputFile=fopen(inputFileName,"rt");
	if(inputFile==0)
		throw std::runtime_error("parseTriangleFile: Unable to open input file");
	
	/* Parse the input file: */
	enum FileState 
		{
		START,INPOLYGON
		};
	
	FileState fileState=START;
	int lineNumber=0;
	int polygonVertexIndex;
	float currentColor[3];
	float currentNormal[3];
	VertexList vertices;
	while(!feof(inputFile))
		{
		/* Read next line from file: */
		char lineBuffer[256];
		lineBuffer[0]='\0';
		fgets(lineBuffer,sizeof(lineBuffer),inputFile);
		++lineNumber;
		
		/* Skip whitespace at beginning of line: */
		char* token;
		for(token=lineBuffer;*token!='\0'&&isspace(*token);++token)
			;
		
		/* Ignore blank lines: */
		if(*token=='\0')
			continue;
		
		/* Extract the first token at beginning of line: */
		char* tokenEnd;
		for(tokenEnd=token;*tokenEnd!='\0'&&!isspace(*tokenEnd);++tokenEnd)
			;
		size_t tokenLength=tokenEnd-token;
		
		/* Process the line: */
		switch(fileState)
			{
			case START:
				if(cmpToken(token,tokenLength,"beginPolygon"))
					{
					/* Start parsing a polygon: */
					polygonVertexIndex=0;
					fileState=INPOLYGON;
					}
				else if(cmpToken(token,tokenLength,"color"))
					{
					/* Parse the color value for the next vertex: */
					if(sscanf(token+tokenLength,"%f, %f, %f",&currentColor[0],&currentColor[1],&currentColor[2])!=3)
						fprintf(stderr,"Wrong number of components in color in line %d\n",lineNumber);
					}
				else if(cmpToken(token,tokenLength,"normal"))
					{
					/* Parse the normal vector for the next vertex: */
					if(sscanf(token+tokenLength,"%f, %f, %f",&currentNormal[0],&currentNormal[1],&currentNormal[2])!=3)
						fprintf(stderr,"Wrong number of components in normal vector in line %d\n",lineNumber);
					}
				else
					{
					/* Signal an error: */
					fprintf(stderr,"Unexpected line %d in input file ignored\n",lineNumber);
					}
				break;
			
			case INPOLYGON:
				if(cmpToken(token,tokenLength,"color"))
					{
					/* Parse the color value for the next vertex: */
					if(sscanf(token+tokenLength,"%f, %f, %f",&currentColor[0],&currentColor[1],&currentColor[2])!=3)
						fprintf(stderr,"Wrong number of components in color in line %d\n",lineNumber);
					}
				else if(cmpToken(token,tokenLength,"normal"))
					{
					/* Parse the normal vector for the next vertex: */
					if(sscanf(token+tokenLength,"%f, %f, %f",&currentNormal[0],&currentNormal[1],&currentNormal[2])!=3)
						fprintf(stderr,"Wrong number of components in normal vector in line %d\n",lineNumber);
					}
				else if(cmpToken(token,tokenLength,"vertex"))
					{
					/* Parse the coordinates for the next vertex: */
					float vertexCoords[3];
					if(sscanf(token+tokenLength,"%f, %f, %f",&vertexCoords[0],&vertexCoords[1],&vertexCoords[2])!=3)
						fprintf(stderr,"Wrong number of components in vertex in line %d\n",lineNumber);
					else
						{
						/* Create the next vertex: */
						Vertex<6> v;
						v.pos=Geometry::HVector::fromPoint(vertexCoords[0],vertexCoords[1],vertexCoords[2]);
						for(int i=0;i<3;++i)
							v.properties[i]=currentColor[i];
						for(int i=0;i<3;++i)
							v.properties[3+i]=currentNormal[i];
						vertices.push_back(v);
						++polygonVertexIndex;
						}
					}
				else if(cmpToken(token,tokenLength,"end"))
					{
					/* Check if the created polygon is a triangle: */
					if(polygonVertexIndex!=3)
						{
						/* Print an error message: */
						fprintf(stderr,"Polygon in input file line %d is not a triangle\n",lineNumber);
						
						/* Remove the vertices from the result list: */
						for(int i=0;i<polygonVertexIndex;++i)
							vertices.pop_back();
						}
					
					/* Stop parsing a polygon: */
					fileState=START;
					}
				else
					{
					/* Signal an error: */
					fprintf(stderr,"Unexpected line %d in input file ignored\n",lineNumber);
					}
				break;
			}
		}
	fclose(inputFile);
	
	/* Return the result vertex list: */
	return vertices;
	}
