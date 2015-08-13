/***********************************************************************
Lighting - The lighting stage of a 3D graphics pipeline.
Copyright (c) 2005 Oliver Kreylos
***********************************************************************/

#include <math.h>
#include <GL/gl.h>

#include "HVector.h"
#include "GLGetLight.h"
#include "GLGetMaterial.h"
#include "ProjectionTransform.h"

#include "Lighting.h"

/*************************
Methods of class Lighting:
*************************/

Lighting::OutputVertex Lighting::lightVertex(const Lighting::InputVertex& iv) const
	{
	/* Calculate vertex position, normal vector, and eye vector: */
	Geometry::HVector pos=iv.pos.project();
	Geometry::HVector normal(iv.properties[3],iv.properties[4],iv.properties[5],0.0f);
	normal.doNormalize();
	Geometry::HVector eye=Geometry::HVector::origin-pos;
	eye.doNormalize();
	
	/* Determine whether to use front- or back-face material: */
	float cosGamma=eye*normal;
	bool frontFace=cosGamma>=0.0f;
	Material material;
	int faceMask;
	if(frontFace)
		{
		material=front;
		faceMask=0x1;
		}
	else
		{
		material=back;
		faceMask=0x2;
		normal=-normal;
		}
	
	/* Apply color material: */
	if(colorMaterialParameter!=0&&(colorMaterialFace&faceMask)!=0)
		{
		Color vertexColor;
		for(int i=0;i<3;++i)
			vertexColor[i]=iv.properties[i];
		vertexColor[3]=1.0f;
		switch(colorMaterialParameter)
			{
			case GL_EMISSION:
				material.emission=vertexColor;
				break;
			
			case GL_AMBIENT:
				material.ambient=vertexColor;
				break;
			
			case GL_DIFFUSE:
				material.diffuse=vertexColor;
				break;
			
			case GL_AMBIENT_AND_DIFFUSE:
				material.ambient=vertexColor;
				material.diffuse=vertexColor;
				break;
			
			case GL_SPECULAR:
				material.specular=vertexColor;
				break;
			}
		}

	/* Accumulate results of illuminating the vertex with all light sources: */
	Color ambient=ambientColor;
	Color diffuse=Color::zero;
	Color specular=Color::zero;
	for(size_t i=0;i<numLights;++i)
		{
		/* Determine the normalized direction to the light source and the total attenuation factor: */
		Geometry::HVector light;
		float attenuation;
		if(lights[i].position.isPoint()) // Point or spot light source
			{
			light=lights[i].position.project()-pos;
			float lightLen=light.mag();
			light/=lightLen;
			attenuation=1.0f/((lights[i].quadraticAttenuation*lightLen+lights[i].linearAttenuation)*lightLen+lights[i].constantAttenuation);
			if(lights[i].spotCutoff!=180.0f)
				{
				float cosAngle=-(light*lights[i].spotDirection);
				if(cosAngle>=cosf(lights[i].spotCutoff*float(M_PI)/180.0f)) // Inside the valid cone
					attenuation*=powf(cosAngle,lights[i].spotExponent);
				else // Not inside the valid cone
					attenuation=0.0f;
				}
			}
		else // Directional light source (no attenuation)
			{
			light=lights[i].position;
			light.doNormalize();
			attenuation=1.0f;
			}
		
		/* Accumulate ambient component: */
		ambient+=lights[i].ambient*attenuation;
		
		/* Accumulate diffuse component: */
		float cosAlpha=light*normal;
		if(cosAlpha>0.0f)
			diffuse+=lights[i].diffuse*(cosAlpha*attenuation);
		
		/* Accumulate specular component: */
		float cosBeta=2.0f*cosGamma*cosAlpha-eye*light;
		if(cosBeta>=0.0f)
			specular+=lights[i].specular*(powf(cosBeta,material.shininess)*attenuation);
		}
	
	/* Calculate final vertex color: */
	Color color=material.emission;
	color+=ambient*material.ambient;
	color+=diffuse*material.diffuse;
	color+=specular*material.specular;
	color.doClamp();
	
	/* Create output vertex: */
	OutputVertex ov;
	ov.pos=iv.pos;
	for(int i=0;i<3;++i)
		ov.properties[i]=color[i];
	return ov;
	}

Lighting::Lighting(ProjectionTransform* sProjectionTransform)
	:projectionTransform(sProjectionTransform),
	 numLights(0),lights(0)
	{
	/* Nothing to do... */
	}

Lighting::~Lighting(void)
	{
	delete[] lights;
	}

void Lighting::start(void)
	{
	/* Get the ambient light color from OpenGL: */
	glGetFloatv(GL_LIGHT_MODEL_AMBIENT,ambientColor.getComponents());
	
	/* Read the number of active light sources from OpenGL: */
	GLint maxLights;
	glGetIntegerv(GL_MAX_LIGHTS,&maxLights);
	size_t newNumLights=0;
	for(GLint i=0;i<maxLights;++i)
		{
		GLboolean active;
		glGetBooleanv(GL_LIGHT0+i,&active);
		if(active)
			++newNumLights;
		}
	if(numLights!=newNumLights)
		{
		delete[] lights;
		numLights=newNumLights;
		lights=new Light[numLights];
		}
	
	/* Read properties of all active light sources: */
	Light* lPtr=lights;
	for(GLint i=0;i<maxLights;++i)
		{
		GLboolean active;
		glGetBooleanv(GL_LIGHT0+i,&active);
		if(active)
			{
			*lPtr=glGetLight(GL_LIGHT0+i);
			++lPtr;
			}
		}
	
	/* Read the color material face and mode: */
	GLint cmf;
	glGetIntegerv(GL_COLOR_MATERIAL_FACE,&cmf);
	switch(cmf)
		{
		case GL_FRONT:
			colorMaterialFace=0x1;
			break;
		
		case GL_BACK:
			colorMaterialFace=0x2;
			break;
		
		case GL_FRONT_AND_BACK:
			colorMaterialFace=0x3;
			break;
		
		default:
			colorMaterialFace=0x0;
		}
	GLboolean cme;
	glGetBooleanv(GL_COLOR_MATERIAL,&cme);
	if(cme)
		glGetIntegerv(GL_COLOR_MATERIAL_PARAMETER,&colorMaterialParameter);
	else
		colorMaterialParameter=0;
	
	/* Read the material properties: */
	front=glGetMaterial(GL_FRONT);
	back=glGetMaterial(GL_BACK);
	}

void Lighting::renderTriangle(const Lighting::InputVertex& iv1,const Lighting::InputVertex& iv2,const Lighting::InputVertex& iv3) const
	{
	/* Light the three vertices and pass them on to the next pipeline stage: */
	projectionTransform->renderTriangle(lightVertex(iv1),lightVertex(iv2),lightVertex(iv3));
	}

void Lighting::finish(void)
	{
	/* Nothing to do... */
	}
