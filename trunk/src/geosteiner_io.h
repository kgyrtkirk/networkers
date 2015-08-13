#ifndef	GEOSTEINER_IO_H
#define	GEOSTEINER_IO_H
#include	"detectable.h"

class	cGeoSteinerIO{
public:
	cGeoSteinerIO()	:	running(false)	{}
	void	clear();
	void	render();
	void	addPoint(HVector pos);
	void	calculate();
	bool	running;
protected:
	float		checksum;	//	sum(p_x+p_y)
	struct	GPoint{
		GPoint(HVector _p)	:	p(_p)	{}
		
		HVector	p;
		const int cmp(const GPoint*)const {return 0;}
	};
	cList<GPoint>	points;
	struct GeoEdge{
		HVector	a,b;
		const int	cmp(const GeoEdge*) const	{return	0;}
	};
	cList<GeoEdge>	edges;
};

void	geosteiner_io();


#endif
