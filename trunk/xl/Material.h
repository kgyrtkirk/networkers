/*
Material - Structure to represent the properties of an OpenGL material.
Copyright (c) 2005 Oliver Kreylos
*/

#ifndef MATERIAL_INCLUDED
#define MATERIAL_INCLUDED

#include "Color.h"

struct Material
	{
	/* Elements: */
	public:
	Color ambient; // Ambient color component
	Color diffuse; // Diffuse color component
	Color specular; // Specular color component
	float shininess; // Specular lighting exponent
	Color emission; // Emissive color component
	
	/* Constructors and destructors: */
	Material(void); // Constructs default material
	Material(const Color& sAmbientDiffuse); // Constructs diffuse material
	Material(const Color& sAmbientDiffuse,
	         const Color& sSpecular,
	         float sShininess); // Constructs specular material
	Material(const Color& sAmbient,
	         const Color& sDiffuse,
	         const Color& sSpecular,
	         float sShininess); // Constructs specular material with separate ambient color
	Material(const Color& sAmbientDiffuse,
	         const Color& sSpecular,
	         float sShininess,
	         const Color& sEmission); // Constructs specular and emissive material
	Material(const Color& sAmbient,
	         const Color& sDiffuse,
	         const Color& sSpecular,
	         float sShininess,
	         const Color& sEmission); // Full initialization
	};

#endif
