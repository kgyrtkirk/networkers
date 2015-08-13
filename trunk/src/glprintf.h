#ifndef	GLPRINTF_H
#define	GLPRINTF_H

#include	"opengl.h"
#include	<string.h>


void	buildFont();

// extern	GLPrintfService	glprintf_service;

// void	glprintf(int dir,const char *fmt, ...)
// #define	glprintf	glprintf_service.printf
// void	glprintf(const char *fmt, ...);
void	glprintf( float x,float y,float size, const char *fmt, ... );

class	GLLog
{
public:
#define	GLLOG_MAX_LINES	90

	char	*lines[GLLOG_MAX_LINES];
// 	int	nlines;
	GLLog()	{	memset(lines,0,sizeof(lines));};
	void	add(const char*str,...);
	void	render();
};

extern	GLLog	glLog;

#endif

