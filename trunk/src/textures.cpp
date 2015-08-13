#include	"port/portability.h"
#include	"util/log.h"
#include	"textures.h"
#include	"opengl.h"
#include	"SDL.h"
#include	<GL/glu.h>
#include	<GL/glpng.h>
#ifdef __TGA
#include	"misc/Texture.h"
#endif

const	char*const	textures_src[TEXTURES_CNT]	=
					{
#define	TX(NAME,PATH)	PATH,
#include	"textures.def.h"
#undef	TX
					};

GLuint	texture_cache[TEXTURES_CNT];

void	loadTextures()
{
// 	glGenTextures( TEXTURES_CNT, texture_cache );

	int	i;
	for(i=0;i<TEXTURES_CNT;i++)
	{
		LOG(D01,"loading	tx%d	--	>%s<",i,textures_src[i]);
#ifdef	__BMP	
		SDL_Surface *TextureImage;

		TextureImage=SDL_LoadBMP( textures_src[i] );
		
		glBindTexture( GL_TEXTURE_2D, texture_cache[i] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, TextureImage->w, TextureImage->h,
			GL_BGR, GL_UNSIGNED_BYTE, TextureImage->pixels );
#else
#ifdef	__TGA
		Texture	texture[2];
		int loop=0;
		LoadTGA(&texture[0], textures_src[i]);
		
				/* Typical Texture Generation Using Data From The TGA */
// 		glGenTextures(1, &texture[loop].texID);                         /* Create The Texture */
		glBindTexture( GL_TEXTURE_2D, texture_cache[i] );
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[loop].width, texture[loop].height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture[loop].imageData);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		if (texture[loop].imageData)                                            /* If Texture Image Exists */
		{
				free(texture[loop].imageData);                                  /* Free The Texture Image Memory */
		}
#else

		pngInfo info;
		int		id;//PNG_ALPHA
		id=texture_cache[i] = pngBind(textures_src[i], PNG_NOMIPMAP, PNG_ALPHA, &info, GL_REPEAT, GL_NEAREST, GL_NEAREST);
		
		if (id != 0) {
			LOG(MSG,"	tx@%d	:	s:%i	%i	d:%i a:%i	>%s<	",id,info.Width, info.Height, info.Depth, info.Alpha,textures_src[i]);
//			LOG(D02,"Size=%i,%i Depth=%i Alpha=%i", );
		}
		else {
			LOG(CRI,"Can't load >%s<",textures_src[i]);
			exit(1);
		}
		

// 		pngInfo info;
// 		if (pngLoad(textures_src[i], PNG_NOMIPMAP, PNG_SOLID, &info))
// 		{
// 			LOG(MSG,"Loaded %s with resounding success",textures_src[i]);
// 			LOG(MSG,"Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);
// 		}
// 		else
// 		{
// 			LOG(MSG,"Loaded %s with resounding success",textures_src[i]);
// 			LOG(MSG,"Can't load >%s<",textures_src[i]);
// 			exit(1);
// 		}

		
#endif
#endif

	}
}

