/*
Light - Structure to represent the properties of an OpenGL light source.
Copyright (c) 2005 Oliver Kreylos
*/

#ifndef LIGHT_INCLUDED
#define LIGHT_INCLUDED

#include "HVector.h"
#include "Color.h"

struct Light
	{
	/* Elements: */
	public:
	Color ambient; // Ambient color component
	Color diffuse; // Diffuse color component
	Color specular; // Specular color component
	Geometry::HVector position; // Light source position
	Geometry::HVector spotDirection; // Spot light direction
	float spotCutoff; // Spot light cutoff angle
	float spotExponent; // Spot light attenuation exponent
	float constantAttenuation; // Point light constant attenuation coefficient
	float linearAttenuation; // Point light linear attenuation coefficient
	float quadraticAttenuation; // Point light quadratic attenuation coefficient
	
	/* Constructors and destructors: */
	Light(void); // Constructs default light source
	Light(const Color& sColor,
	      const Geometry::HVector& sPosition,
	      float sConstantAttenuation =1.0f,
	      float sLinearAttenuation =0.0f,
	      float sQuadraticAttenuation =0.0f); // Sets a monochromatic point or directional light source
	Light(const Color& sColor,
	      const Geometry::HVector& sPosition,
	      const Geometry::HVector& sSpotDirection,
	      float sSpotCutoff,
	      float sSpotExponent =0.0f,
	      float sConstantAttenuation =1.0f,
	      float sLinearAttenuation =0.0f,
	      float sQuadraticAttenuation =0.0f); // Sets a monochromatic spot light source
	Light(const Color& sAmbient,
	      const Color& sDiffuse,
	      const Color& sSpecular,
	      const Geometry::HVector& sPosition,
	      const Geometry::HVector& sSpotDirection,
	      float sSpotCutoff,
	      float sSpotExponent =0.0f,
	      float sConstantAttenuation =1.0f,
	      float sLinearAttenuation =0.0f,
	      float sQuadraticAttenuation =0.0f); // Full initialization
	};

#endif
