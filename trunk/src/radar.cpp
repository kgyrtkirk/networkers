#include	"detectable.h"
#include	"radar.h"
#include	"com_point.h"
#include	"networker.h"
#include	"set.h"
#include	"nw_stat.h"

Radar	radar;


void	Radar::render()
{
	cListCursor<Detectable>	cr(&objects);
	Detectable*			ent;
	
	steiner.render();
	
	while((ent=cr.next()))
		ent->render();
}

int	Radar::get_fraction_size(int idx)
{
	return	fraction_size[idx];
}

void	Radar::step()
{
	cListCursor<Detectable>	cr(&objects);
	Detectable*			ent;
	int	nw_tower=0;
	int	nw_recon=0;
	int	cp_known=0;
	int	cp_free=0;
	
	prepare();
	
	
	timestamp++;
	
	steiner.clear();

	while((ent=cr.next()))
	{
		switch(ent->getType())
		{
			case	T_COMPOINT:
					if(((ComPoint*)ent)->known)
					{
						steiner.addPoint(HVector(ent->pos[0],ent->pos[1],0,0));
// 						steiner.addPoint(ent->pos);
						cp_known++;
					}
					else	cp_free++;
				break;
			case	T_NETWORKER:
					switch(ent->getState())
					{
						case	S_TOWER:		nw_tower++;	break;
						case	S_RECON:		nw_recon++;	break;
					}
				break;
			default:
			
				break;
		}
		
		ent->step();
	}
// printf("--step:	%d	%d\n",timestamp,nw_recon);
// nw_tower=20;
// 		LOG(MSG,"nwr%d",nw_recon);
		

	nws.gr_tower.add(nw_tower);
	nws.gr_recon.add(nw_recon);

	nws.gr_cp_known.add(cp_known);
	nws.gr_cp_free.add(cp_free);
	
	steiner.calculate();
	
}

HVector&	Radar::rand_point()
{
	float	x=(MAP_MAX-MAP_MIN)*drand48()+MAP_MIN;
	float	y=(MAP_MAX-MAP_MIN)*drand48()+MAP_MIN;
	HVector	*v=new HVector(x,y,0,0);
	return	*v;
}



void	Radar::createComPoint()
{
	int				l=20;
	DSet	c;
	prepare();
	while(l--)
	{
		float	x=(MAP_MAX-MAP_MIN)*drand48()+MAP_MIN;
		float	y=(MAP_MAX-MAP_MIN)*drand48()+MAP_MIN;
		
		c=collect(x,y,100.0);
		if(!c.size())
		{
			objects.add(new ComPoint(x,y));
			return;
		}
	}

// 	ComPoint	*cp=new ComPoint();
}

void	Radar::createRobot(int fraction,int count)
{
	int				l=20;
	prepare();
	DSet	c;
	while(l--)
	{
		float	x=(MAP_MAX-MAP_MIN)*drand48()+MAP_MIN;
		float	y=(MAP_MAX-MAP_MIN)*drand48()+MAP_MIN;
		x/=7;
		y/=7;
		c=collect(x,y,100.0);
// 		x=0.0f;
// 		y=0.0f;
		if(!c.size() || l==0)
		{
#ifdef	_JUNK
			
//			int		q=(int)sqrt(count);
			float	sa=sin(l*M_PI_2/count)*l;
			for(l=0;l<count;l++)
// 				objects.add(new NetWorker(l+1,x+(l%q)*25,y+(l/q)*25));
				objects.add(new NetWorker(l+1,x+sa*cos(count/(l+1)),y+sa*sin(count/(l+1))));
			return;
#else
			fraction_size[fraction]+=count;
			for(l=0;l<count;l++)
				objects.add(new NetWorker(l+1,x,y));
			return;
#endif

		}
	}

// 	ComPoint	*cp=new ComPoint();
}


DSet		Radar::collect(float x,float y,float radius)
{
	DSet		list;//=new DSet();
	cListCursor<Detectable>	cr(&objects);
// 	cXCursor	 			cr2(&objects_x);
	Detectable*		ent;
	Vec2D	p(x,y);
// 	float	radius2=radius*radius;
	float	R=radius*1.1f;
//  	cmp_flt32	f;
// 	cmp_flt32	s;
// 	s.a=x-R;
// 	cr2.reset(s);
	
	while(!list.full() && (ent=cr.next()))
	{
		if(	ent->pos[0]+R > x	&&
			(ent->pos-p).mag() < radius )
				list.add(ent);
				
		if(	ent->pos[0] > x+R )
			break;
	}
// 	static	int	q=400;
// 	if(--q==0)	
// 		exit(0);
	return	list;
}

void	Radar::prepare()
{
// 	uint32	l;
	objects.sort();
// 	l=objects.getsize();
}


Detectable*	Radar::look(Vec2D&pos)
{
	cListCursor<Detectable>	cr(&objects);
	Detectable			*ent,*t=0;
	float				d;
	while((ent=cr.next()))
		if(!t || (ent->pos-pos).mag() < d)
		{
			t=ent;
			d=(ent->pos-pos).mag();
		}
	return	t;
	
	
}

Radar::~Radar()
{
	cListCursor<Detectable>	cr(&objects);
	Detectable*			ent;
	
	while((ent=cr.next()))
		delete ent;
		
//	cr.();
}
