/*
Material - Structure to represent the properties of an OpenGL material.
Copyright (c) 2005 Oliver Kreylos
*/

#include "Material.h"

/*************************
Methods of class Material:
*************************/

Material::Material(void)
	:ambient(0.2f,0.2f,0.2f,1.0f),
	 diffuse(0.8f,0.8f,0.8f,1.0f),
	 specular(0.0f,0.0f,0.0f,1.0f),
	 shininess(0.0f),
	 emission(0.0f,0.0f,0.0f,1.0f)
	{
	}

Material::Material(const Color& sAmbientDiffuse)
	:ambient(sAmbientDiffuse),
	 diffuse(sAmbientDiffuse),
	 specular(0.0f,0.0f,0.0f,1.0f),
	 shininess(0.0f),
	 emission(0.0f,0.0f,0.0f,1.0f)
	{
	}

Material::Material(const Color& sAmbientDiffuse,
                   const Color& sSpecular,
                   float sShininess)
	:ambient(sAmbientDiffuse),
	 diffuse(sAmbientDiffuse),
	 specular(sSpecular),
	 shininess(sShininess),
	 emission(0.0f,0.0f,0.0f,1.0f)
	{
	}

Material::Material(const Color& sAmbient,
                   const Color& sDiffuse,
                   const Color& sSpecular,
                   float sShininess)
	:ambient(sAmbient),
	 diffuse(sDiffuse),
	 specular(sSpecular),
	 shininess(sShininess),
	 emission(0.0f,0.0f,0.0f,1.0f)
	{
	}

Material::Material(const Color& sAmbientDiffuse,
                   const Color& sSpecular,
                   float sShininess,
                   const Color& sEmission)
	:ambient(sAmbientDiffuse),
	 diffuse(sAmbientDiffuse),
	 specular(sSpecular),
	 shininess(sShininess),
	 emission(sEmission)
	{
	}

Material::Material(const Color& sAmbient,
                   const Color& sDiffuse,
                   const Color& sSpecular,
                   float sShininess,
                   const Color& sEmission)
	:ambient(sAmbient),
	 diffuse(sDiffuse),
	 specular(sSpecular),
	 shininess(sShininess),
	 emission(sEmission)
	{
	}
