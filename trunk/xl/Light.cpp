/*
Light - Structure to represent the properties of an OpenGL light source.
Copyright (c) 2005 Oliver Kreylos
*/

#include "Light.h"

/**********************
Methods of class Light:
**********************/

Light::Light(void)
	:ambient(0.0f,0.0f,0.0f,1.0f),
	 diffuse(1.0f,1.0f,1.0f,1.0f),
	 specular(1.0f,1.0f,1.0f,1.0f),
	 position(0.0f,0.0f,1.0f,0.0f),
	 spotDirection(0.0f,0.0f,-1.0f,0.0f),
	 spotCutoff(180.0f),
	 spotExponent(0.0f),
	 constantAttenuation(1.0f),
	 linearAttenuation(0.0f),
	 quadraticAttenuation(0.0f)
	{
	}

Light::Light(const Color& sColor,
             const Geometry::HVector& sPosition,
             float sConstantAttenuation,
             float sLinearAttenuation,
             float sQuadraticAttenuation)
	:ambient(0.0f,0.0f,0.0f,1.0f),
	 diffuse(sColor),
	 specular(sColor),
	 position(sPosition),
	 spotDirection(0.0f,0.0f,-1.0f,0.0f),
	 spotCutoff(180.0f),
	 spotExponent(0.0f),
	 constantAttenuation(sConstantAttenuation),
	 linearAttenuation(sLinearAttenuation),
	 quadraticAttenuation(sQuadraticAttenuation)
	{
	}

Light::Light(const Color& sColor,
             const Geometry::HVector& sPosition,
             const Geometry::HVector& sSpotDirection,
             float sSpotCutoff,
             float sSpotExponent,
             float sConstantAttenuation,
             float sLinearAttenuation,
             float sQuadraticAttenuation)
	:ambient(0.0f,0.0f,0.0f,1.0f),
	 diffuse(sColor),
	 specular(sColor),
	 position(sPosition),
	 spotDirection(sSpotDirection),
	 spotCutoff(sSpotCutoff),
	 spotExponent(sSpotExponent),
	 constantAttenuation(sConstantAttenuation),
	 linearAttenuation(sLinearAttenuation),
	 quadraticAttenuation(sQuadraticAttenuation)
	{
	}

Light::Light(const Color& sAmbient,
             const Color& sDiffuse,
             const Color& sSpecular,
             const Geometry::HVector& sPosition,
             const Geometry::HVector& sSpotDirection,
             float sSpotCutoff,
             float sSpotExponent,
             float sConstantAttenuation,
             float sLinearAttenuation,
             float sQuadraticAttenuation)
	:ambient(sAmbient),
	 diffuse(sDiffuse),
	 specular(sSpecular),
	 position(sPosition),
	 spotDirection(sSpotDirection),
	 spotCutoff(sSpotCutoff),
	 spotExponent(sSpotExponent),
	 constantAttenuation(sConstantAttenuation),
	 linearAttenuation(sLinearAttenuation),
	 quadraticAttenuation(sQuadraticAttenuation)
	{
	}
