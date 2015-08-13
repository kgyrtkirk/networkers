#ifndef	DETECTABLE_H
#define	DETECTABLE_H
#include	"opengl.h"
#include	"glprintf.h"
#include	"util/log.h"
#include	"util/list.h"
#include	"set.h"
#include	"vec2d.h"

enum	eDetectableType
{
	T_DETECTABLE,
	T_COMPOINT,
	T_NETWORKER,
};

union	Message;
class	Detectable;

class	TrackedDetectable
{
public:
	HVector		pos;			//!<	last known position
	int			serial;		//!<	0 if Communication point
	int			dist;		//!<	distance reported distance

	Detectable	&detectable;	//!<	only used for drawing

	TrackedDetectable(Detectable&d)	:	detectable(d)	{}
// 	CLink		&clink;
};


class	Detectable
{
public:
	typedef	cList< Detectable >		DList;
	typedef	cListCursor<Detectable>	DCursor;
	typedef	cList<Message>			MList;
	typedef	cListCursor<Message>	MCursor;

	
	Vec2D		pos;
	float		tree_x;		//	!<
	int			serial;
	float		tx_power;
	float		rotor;
	cList<Message>	msg_queue;
	int			range_error;
	
	Detectable(flt32 x,flt32 y)				:	pos(x,y)	{	tx_power=0.0f;		serial=0xbeef;	rotor=drand48();	};
	Detectable(flt32 x,flt32 y,flt32 _tx_power)	:	pos(x,y)	{	tx_power=_tx_power;	serial=0xbeef;	rotor=drand48();	};
	virtual	~Detectable()			{}
	int cmp(const Detectable*b)	const
	{
		#define	a	pos[0]
				if(a==b->a)	return	0;
				if(a<b->a)	return	-1;
				else			return	1;	
		#undef	a
	}
	virtual	void		render()		{	LOG(ERR,"detectable	dummy render");	}
	virtual	void		step()		{	LOG(ERR,"detectable	dummy step");		}
	virtual	int		getState()	{	LOG(ERR,"detectable	getState");	return	0;	}
	virtual	void		seen()		{}
	virtual	void		info(DSet&m0,DSet&m1,char*str)		{sprintf(str,"detectable	%d",serial);	}
	virtual	bool		knows(Detectable*d)		{return	0;	}
	virtual	int		getType()	const	{return	T_DETECTABLE;	}

// 	virtual	float	flow_quotient(Detectable&p,DSet neigh);

// 	virtual	void	send_message(int type,	Detectable*dst);


};
#endif
#include	"message.h"
