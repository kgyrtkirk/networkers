#include	"glprintf.h"
#include	<stdarg.h>
#include	<stdio.h>
#include	<string.h>
#include	"util/log.h"

#include <GL/glx.h>
#include "SDL.h"
#include	<plib/fnt.h>

fntRenderer texout ;
fntTexFont  default_font;


void	glprintf( float x,float y,float size, const char *fmt, ... )
{
#define	MAX_STR	512
	char	str[MAX_STR];
	int	n;
	va_list ap;

	va_start(ap, fmt);
	n = vsnprintf(str, MAX_STR, fmt, ap);
	va_end(ap);
	
	if(n>=MAX_STR)
	{
		LOG(ERR,"ignoring string printout -- too long %d > %d",n,MAX_STR);
		return;
	}

	texout . setPointSize ( size ) ;
	
	texout . begin () ;
		texout . start2f ( x,y ) ;
		texout . puts ( str ) ;
// 		texout . start2f ( 50.0f, 50.0f ) ;
// 		texout . puts ( "World" ) ;
	texout . end () ;
	
}

void	GLLog::add(const char*str,...)
{
	char	*s=(char*)malloc(MAX_STR);
	
	va_list ap;     /* Pointer to our list of elements */
	va_start( ap, str );
		vsnprintf( s, MAX_STR, str, ap );
	va_end( ap );
	
	int	i=0;
	if(lines[0])
		free(lines[0]);
	for(i=0;i<GLLOG_MAX_LINES-1;i++)
		lines[i]=lines[i+1];
	lines[i]=s;

}

void	GLLog::render()
{
	int	i=0;
	float	H=.03f;
	for(i=0;i<GLLOG_MAX_LINES;i++)
		glprintf(0.0f,-H*i,H,lines[i]);

}
GLLog	glLog;



void	buildFont()
{
	texout . setFont      (&default_font) ;
	default_font.load( "fonts/default.txf" ) ;
 /* Declarations */


   /* Load some fonts */

	
// 	fntTexFont  Courier    ( "courier.txf" ) ;

   /* Select a font and pointsize to render with... */



   /* Print "Hello" and "World" */


}


// GLuint	glprintf_font=0;
// 
// #define	base	glprintf_font
// 
// // void	glprintf(const char *fmt, ...)
// // {
// // 	glprintf_service.printf(
// // }
// 
// 
// void		deleteFont()
// {
// 	if(base)
// 		glDeleteLists( base, 96 );
// 	base=0;
// }
// 
// void	buildFont()
// {
// 	LOG(MSG,"GLPrintfService -- loading font");
// 	Display *dpy;          /* Our current X display */
// 	XFontStruct *fontInfo; /* Our font info */
// 	
// 	/* Sotrage for 96 characters */
// 	base = glGenLists( 96 );
// 	
// 	/* Get our current display long enough to get the fonts */
// 	dpy = XOpenDisplay( NULL );
// 	
// 	/* Get the font information */
// 	fontInfo = XLoadQueryFont( dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1" );
// 	
// 	/* If the above font didn't exist try one that should */
// 	if ( fontInfo == NULL )
// 		{
// 			fontInfo = XLoadQueryFont( dpy, "fixed" );
// 			/* If that font doesn't exist, something is wrong */
// 			if ( fontInfo == NULL )
// 				{
// 				LOG(ERR,"GLPrintfService -- no X font available");
// 				fprintf( stderr, "no X font available?\n" );
// 				exit(1);
// 				}
// 		}
// 	
// 	/* generate the list */
// 	glXUseXFont( fontInfo->fid, 32, 96, base );
// 	
// 	/* Recover some memory */
// 	XFreeFont( dpy, fontInfo );
// 	
// 	/* close the display now that we're done with it */
// 	XCloseDisplay( dpy );
// 	
// 	return;
// }
// 
// void	glprintf( const char *fmt, ... )
// {
//     char text[256]; /* Holds our string */
//     va_list ap;     /* Pointer to our list of elements */
// 
//     /* If there's no text, do nothing */
//     if ( fmt == NULL )
//         return;
// 
// // 	if(base==0)
// // 	{
// // 		buildFont();
// // 	}
//     /* Parses The String For Variables */
//     va_start( ap, fmt );
//       /* Converts Symbols To Actual Numbers */
//       vsprintf( text, fmt, ap );
//     va_end( ap );
// 
//     /* Pushes the Display List Bits */
//     glPushAttrib( GL_LIST_BIT );
// 
//     /* Sets base character to 32 */
//     glListBase( base - 32 );
// 
//     /* Draws the text */
//     glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );
// 
//     /* Pops the Display List Bits */
//     glPopAttrib( );
// }













// #include <stdio.h>
// #include <stdarg.h>
// #include <math.h>
// #include <GL/glx.h>
// #include <GL/gl.h>
// #include <GL/glu.h>
// 
// 
// #include <X11/extensions/xf86vmode.h>
// #include <X11/keysym.h>
// 
// 
// 
// 
// void	GLPrintfService::printf(const char *fmt, ...)
// {
//     va_list ap;     /* our argument pointer */
//     char text[256];
//     if (fmt == NULL)    /* if there is no string to draw do nothing */
//         return;
//     va_start(ap, fmt);  /* make ap point to first unnamed arg */
//     /* FIXME: we *should* do boundschecking or something to prevent buffer
//      * overflows/segmentations faults
//      */
//     vsprintf(text, fmt, ap);
//     va_end(ap);
//     glPushAttrib(GL_LIST_BIT);
//     glListBase(base - 32);
//     glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
//     glPopAttrib();
// }
// 
// void	GLPrintfService::deleteFont()
// {
// 	if(base)
// 		glDeleteLists(base, 96);
// }
// 
// 
// void	GLPrintfService::buildFont()
// {
//     XFontStruct *font;
// 
//     base = glGenLists(96);      /* storage for 96 characters */
//     /* load a font with a specific name in "Host Portable Character Encoding" */
//     font = XLoadQueryFont(GLWin.dpy,
//         "-*-helvetica-bold-r-normal--24-*-*-*-p-*-iso8859-1");
//     if (font == NULL)
//     {
//         /* this really *should* be available on every X Window System...*/
//         font = XLoadQueryFont(GLWin.dpy, "fixed");
//         if (font == NULL)
//         {
//             printf("Problems loading fonts :-(\n");
//             exit(1);
//         }
//     }
//     /* build 96 display lists out of our font starting at char 32 */
//     glXUseXFont(font->fid, 32, 96, base);
//     /* free our XFontStruct since we have our display lists */
//     XFreeFont(GLWin.dpy, font);
// }

