#include	"nw_info.h"
#include	"opengl.h"
#include	"textures.h"

#include	"colors.h"

NetWorkerInfo	nwi;



void		NetWorkerInfo::render()
{
	DSet	mark0;
	DSet	mark1;
// 	char				str[8192];

	if(!subject)
		return;
		
	glPushMatrix();
	
	glTranslatef(subject->pos[0],subject->pos[1],16.0f);
	#define	SIZE	12
		drawQuad(	TX_SELECT,	1.0f,
			-SIZE, SIZE,
			 SIZE,-SIZE);

	glPopMatrix();


	subject->info(mark0,mark1,str);
	
	{
	
		DCursor		dc(mark0);
		Detectable		*d;
		int	i=0;
		while((d=dc.next()))
		{
			glPushMatrix();
			
			glTranslatef(d->pos[0],d->pos[1],16.0f);
			glColor4fv(_colors[i++%NCOLORS]);

			glRotatef(d->rotor*500/3.0,0,0,1);
				drawQuad(	TX_MARK,	1.5f,
					-SIZE, SIZE,
					SIZE,-SIZE);
			glPopMatrix();
		}
	}
			glColor4f(1.0f,1.0f,1.0f,1.0f);
	
	{
		DCursor		dc(mark1);
		Detectable		*d;
		while((d=dc.next()))
		{
			glPushMatrix();
			
			glTranslatef(d->pos[0],d->pos[1],16.0f);
			glRotatef(d->rotor*500/3.0,0,0,1);
				drawQuad(	TX_CONNECT,	2.0f,
					-SIZE, SIZE,
					SIZE,-SIZE);
			glPopMatrix();
		}
	}

	
// 	glPopMatrix();

}

void		NetWorkerInfo::text_render()
{
	glprintf(0.0f,0.0f,.05f,str);
}

