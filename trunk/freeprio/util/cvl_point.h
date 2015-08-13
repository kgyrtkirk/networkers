#ifndef	CVL_POINT_H
#define	CVL_POINT_H
#include	<cv.h>
#include	<highgui.h>
#include	"lemon/dim2.h"

/*	a good point to start with? :)
*/

typedef	lemon::dim2::Point<float>	lemonPoint;

class	Point	:	public	lemonPoint
{
public:
	Point()
	{
		x=0;
		y=0;
	}
	Point(const lemonPoint&p)
	{
		x=p.x;
		y=p.y;
	}
	Point(const CvPoint&p)
	{
		x=p.x;
		y=p.y;
	}

	operator	CvPoint()
	{
		CvPoint	p;
		return	cvPoint((int)x,(int)y);
	}
	Point&	operator=(const lemonPoint&lp)
	{
		x=lp.x;
		y=lp.y;
		return	*this;
		
	}
// 	void		rot180()
// 	{
// 		x=-x;
// 		y=-y;
// 	}
// 	Point&	rot90(const Point&p)	
// 	{
// 		Point	q;
// 		q.x= p.y;
// 		q.y=-p.x;
// 		return	q;
// 	}

	float	dot(const lemonPoint&p)		const
	{
		return	p.x*x+p.y*y;
	}
	float	dot(const Point&p)		const
	{
		return	p.x*x+p.y*y;
	}
	float	length()				const
	{
		return	sqrtf(normSquare());
	}
	
	float	dist(const Point&p)		const
	{
		Point	q=*this;
		q-=p;
		return	q.length();
	}
	float	dist(const lemonPoint&p)		const
	{
		Point	q=*this;
		q-=p;
		return	q.length();
	}
	float	dist2(const Point&p)	const
	{
		Point	q=*this;
		q-=p;
		return	q.normSquare();
	}
	float	normalize()
	{
		float	l=length();
		x/=l;
		y/=l;
		return	l;
	}
	Point	translate_to(const Point&x,const Point&y)	const
	{
		Point	q;
		q.x=dot(x);
		q.y=dot(y);
		return	q;
	}
};

#endif
