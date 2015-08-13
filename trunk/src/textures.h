#ifndef	TEXTURES_H
#define	TEXTURES_H
#include	<GL/gl.h>

enum	eTextureNames
{
	#define	TX(NAME,PATH)	TX_##NAME,
	#include	"textures.def.h"
	#undef	TX
	TEXTURES_CNT
};

extern	GLuint	texture_cache[];

void	loadTextures();

#define	TEXTURE(NAME)	texture_cache[NAME]

#endif
