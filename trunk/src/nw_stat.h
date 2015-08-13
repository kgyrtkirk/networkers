#ifndef	NW_STAT_H
#define	NW_STAT_H
#include	"port/portability.h"
#include	"opengl.h"
#include	"colors.h"
#include	"util/list.h"
#include	"textures.h"
#include	"glprintf.h"

template<size_t N>
class	Graph{
public:
	
	float	value[N];
	float	min;
	float	max;
	float	avg;
	const	char*name;

	Graph(const char*_name)	:name(_name)
	{
			memset(value,0,sizeof(value));
			min=avg=max=0.0f;
	}
	void		add(float v)
	{
		uint32	i;
		for(i=0;i<N-1;i++)
			value[i]=value[i+1];
		value[i]=v;
	}
	void		update()
	{
		min= 9000.0f;
		max=-9000.0f;
		avg=0.0f;
		for(uint32 i=0;i<N;i++)
		{
			avg+=value[i];
			if(min>value[i])
				min=value[i];
			if(max<value[i])
				max=value[i];
		}
		avg/=N;
	}
	void	render()
	{
// 		glColor4f(1.0f,1.0f,1.0f,1.0f);
		
		glBegin(GL_LINE_STRIP);
		for(uint32 i=0;i<N;i++)
			glVertex2f(i,value[i]);
		glEnd();
	}
	void	texts()
	{
		glprintf(0,0,0.05f,"%s\n%06.2f\n%06.2f\n%06.2f\n%06.2f",name,min,avg,max,value[N-1]);
		
	}
	const	int	cmp(const	Graph*) const
	{
		return	0;
	}
	static	size_t	getN()	
	{
		return	N;
	}
};

template<class G>
class	GDisplay
{
	cList<G>		li;
	const char*	name;
public:
	GDisplay(const char*_name)	:	name(_name)	{}
	void	add(G*g)
	{
		li.add(g);
	}
	void	render(float width)
	{
		glPushMatrix();
		glScalef(width,1.0f,1.0f);
		glColor4f(1.0f,1.0f,1.0f,0.5f);
		drawQuad(TX_BG,		-0.00001,0,0,
								1,1,
										1.0f,1.0f	);

		glDisable(GL_TEXTURE_2D);
		float	/*min,*/max;
		cListCursor<G>	cr(&li);
		G		*g;
		max=0;
		while((g=cr.next()))
		{
			g->update();
			if(g->max>max)
				max=g->max;
		}
		if(max<1)
			max=1.0;
		glScalef(1.0f/G::getN(),1.0/max,1.0f);
// 		LOG(MSG,"%f",max);
		
		cr.reset();
		int		i=0;
		while((g=cr.next()))
		{
			glColor4fv(&_colors[i++][0]);
			g->render();
		}
		glEnable(GL_TEXTURE_2D);
		glPopMatrix();
		
		cr.reset();
		glPushMatrix();
		glTranslatef(0.1f,0.8f,0);
		i=0;
		while((g=cr.next()))
		{
			glColor4fv(&_colors[i++][0]);
// 			glTranslatef(0,-0.1f,0);
			g->texts();
			
			glTranslatef(0.3f,0,0);
			
		}
		
		glPopMatrix();
		glColor4f(1.0f,1.0f,1.0f,0.5f);
		glprintf(0.1f,0.2f,0.1,name);
		
		
	}
	
};

#define	GRAPH_SIZE	4096

class	NetWorkerStat
{
public:

	Graph<GRAPH_SIZE>			gr_tower;
	Graph<GRAPH_SIZE>			gr_recon;
	
	Graph<GRAPH_SIZE>			gr_cp_known;
	Graph<GRAPH_SIZE>			gr_cp_free;
	
	
	GDisplay< Graph<GRAPH_SIZE> >	gd_networkers;
	GDisplay< Graph<GRAPH_SIZE> >	gd_compoints;

	NetWorkerStat()	:	
						gr_tower("tower"),		gr_recon("recon"),
						gr_cp_known("known"),	gr_cp_free("free"),
						gd_networkers("cnt"),	gd_compoints("compoint")
	{
		gd_networkers.add(&gr_tower);
		gd_networkers.add(&gr_recon);
		
		gd_compoints.add(&gr_cp_known);
		gd_compoints.add(&gr_cp_free);
	}

	void	render()
	{
		gd_networkers.render(1.75f);
		glTranslatef(0,-1.1f,0);
		gd_compoints.render(1.75f);
// 		

	}
	
};

extern	NetWorkerStat	nws;

#endif
