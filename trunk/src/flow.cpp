#include	"flow.h"
#include	"set.h"
#include	"log.h"
#include	"nw_info.h"


//
//	calculates the dead_zone of p
//		min_{n\in N} range-(p-n).mag()
float	flow_deadzone(Vec2D p,DSet neigh,float range)
{
	DCursor			dc(neigh);
	Detectable		*d;
	float			dist;
	float			v;
	v=1.0f;
	while(d=dc.next())
	{
		dist=	(d->pos-p).mag()/range;
		if(dist>1.0f)
			dist=0.0f;
		else	dist=1.0f-dist;

	//		v= \min_{n \in K} \max(0,1-d(p,n) )
		if( v > dist )
			v=dist;
	}
	
	return	v;
}

#define	MAX_POINTS	8

Vec2D	sh_avg(Vec2D &c,DSet neigh)
{
	DCursor			dc(neigh);
	Detectable		*d;
	Vec2D			r(0,0);
	float			w;
	float			W;
	
	Vec2D			points[MAX_POINTS];
	int				npoints;
	
	assert(neigh.size() < MAX_POINTS);
	
	if(neigh.size()<1)		//	
		return	c;

	//	flatten the set into a vector, and order them.
	//		...	create a temporary shape from them
	npoints=0;
	while(d=dc.next())
	{
		points[npoints++]=d->pos;
		c+=d->pos;
	}
	
	if(npoints>3)	//	only need ordering in case of |V| > 3
	{
		float	angles[MAX_POINTS];
		for(int i=0;i<npoints;i++)
		{
			Vec2D	x=(points[i]-c);
			angles[i]=atan2(x[0],x[1]);
		}
		//dirty little sort
		for(int i=0;i<npoints;i++)
		for(int j=i+1;j<npoints;j++)
		{
			if(angles[i] > angles[j])
			{
				float	t=angles[i];
				Vec2D	v=points[i];
				points[i]=points[j];
				angles[i]=angles[j];
				points[j]=v;
				angles[j]=t;
			}
		}
	}
	
	W=0;
	r=Vec2D(0,0);
	//	weighting
	for(int i=0;i<npoints;i++)
	{
		w=	(points[i] - points[(i+1)%npoints]).mag()	+
			(points[i] - points[(i+npoints-1)%npoints]).mag()	;
		r+=points[i]*w;
		W+=w;
	}
	
	if(W>0.0f)
		return	r/W;
	else	return	points[0];	//	exceptional, all lengths are 0, so returning the 0. point is correct
}

Vec2D	tower_dest_pos(Vec2D	p,DSet neigh,DSet opt_on,float range)
{
// 	float	q;
// 	q=flow_quotient(p,neigh,range);
	DSet				q=neigh-opt_on;
	DCursor			dc(q);
	Detectable		*d;
	Vec2D			r(0,0);
	Vec2D			k;
	int				c=0;
	float	dz=	flow_deadzone(p,opt_on,range);
	Vec2D	n=	sh_avg(p,neigh);
	
	while(d=dc.next())
	{
// 	LOG(CRI,	"X%d	p:%08f %08f",
// 			c,
// 				d->pos[0],d->pos[1]);
		r+=d->pos;
		c++;
	}
	if(c)
		r/=c;
	else	r=n;
	
// 	LOG(CRI,	"TD	p:%08f %08f r:%08f	%08f	n:%08f	%08f	c:%d	%f",	
// 						p[0],p[1],
// 						r[0],r[1],
// 						n[0],n[1],
// 						c,dz);
	k=(r-n);
	if(c && k.mag()>0.01f)
	{
		k=k.normalize();
		r=k*dz*range+n;
	}
	else	r=n;
// 	r=n;
	
	return	r;
}


// HVector	tower_flow(HVector d,int opt,bool opt_phase)
// {
// 	HVector	n=d.normalize();
// 	
// 	float	x=d.mag();
// 	float	f;
// 	
// 	if(opt_phase)
// 	{
// 		if( opt )
// 			f=	pow(MAX(0,(x-d_tower)*1/(1-d_tower)),2);
// // 			f=	-(MIN((d_tower-1)*x/d_tower/d_tower,0)+MAX(0,(x-d_tower)*1/(1-d_tower)));
// 		else	
// 			f=	pow(x,2);
// 			
// 	}
// 	else
// 		f=	pow(x, 1);
// 		
// // 		f=	x;
// // 	f=	x;
// 	if(f<0.0)
// 		f=0.0;
// 	if(f>1.0)
// 		f=1.0;
// //	n*=f;
// 	
// 	if(x<0.01f)
// 		return	HVector(0,0,0,0);
// //	if(n>
// 	return	n*f;
// }
