#ifndef	RADAR_H
#define	RADAR_H
#include	"util/list.h"
#include	"util/cmps.h"
#include	"detectable.h"
#include	"set.h"
#include	"geosteiner_io.h"

#define	MAP_MIN	-1000.0f
#define	MAP_MAX	 1000.0f

// typedef	cList< Detectable >	DSet;

#define	MAX_FRACTIONS	4
/*
struct	xDetectable
{
	float	tree_x;
};
class	Q	:	public	xDetectable
{
	virtual	~Q()			{}
	virtual	void	asd()		{printf("a");}
	virtual	void	xasd()		{printf("a");}
};*/

class	Radar
{
public:
	int					timestamp;
	int					fraction_size[MAX_FRACTIONS];
	cList<Detectable>		objects;
	cGeoSteinerIO			steiner;
// 	struct	IntEntry
// 	{
// 		Detectable	*d;
// 		float		x;
// 	}		int_arr;
	
	
//	cOffsetList<Detectable,>	objects;
// 	cOffsetList<Detectable,cmp_flt32,offsetof(Detectable,tree_x)>			objects_x;
// 	typedef	cIndexCursor<Detectable,cmp_flt32,offsetof(Detectable,tree_x)>	cXCursor;

	Radar()		:	timestamp(0)	{	SETZERO(fraction_size);	/*objects.add_key(&objects_x);*//*;int_arr=0;*/	}
	~Radar();
	
// 	cList<Helpers>		helpers;
	void	createComPoint();
	void	createRobot(int fraction,int count);
	void	render();
	void	step();
	void	prepare();
	
	HVector&rand_point();
	Detectable*	look(Vec2D&pos);
	DSet			collect(float x,float y,float radius);
	int			get_fraction_size(int idx);
};

extern	Radar	radar;
#endif
