#ifndef	MESSAGE_H
#define	MESSAGE_H

#include	"vec2d.h"

//	sender is part of the message, because it is needed to make drawing easier

void	drawMessage(int tx,	Vec2D src,Vec2D dst);

class	Detectable;

enum		MessageTypes{
#define	MSGT(A)	A,
#include	"message_types.h"
#undef	MSGT
		NMESSAGE_TYPES
// 	MSG_TYPE_RESERVED,
// 	SCAN_INFO,
// 	SCAN_UPDATE_REQ,
// 	SCAN_UPDATE,
// 	INTERLOCK,
// 	LINK_ERROR,
// 	MEASURE_DEPTH,
// 	HEARTBEAT,
// 	HANDOVER,
// 	LINK_INFO,
// 	LEADER,
};

extern	const	char*	message_types[];

union	Message
{
	int		type;
	struct	__header	{
			int			type;
			int			timestamp;
			int			src_id;
			int			dst_id;
			int			state;
			Detectable	*sender;
			float		pos[2];
	}	head;
	struct	__scan_info	{
			__header		__hold_the_line;
			int			authorative;
			float		query_pos[2];
			int			curr_length;
			int			max_length;
			int			lo_depth;
			int			lo_source_id;
// 			int			id;
	}	si;
	struct	__scan_update_req	{
			__header		__hold_the_line;
			int			chain_len;
			float		max_speed;
	}	sur;
	struct	__scan_update		{
			__header		__hold_the_line;
			float	dir[2];
			bool		tie_to_tower;
			int		advertised_dist;
			int		max_length;
			int		lo_depth;
			int		lo_source_id;
	}	su;
	struct	__interlock		{
			__header		__hold_the_line;
			//?
	}	il;
	struct	__link_error		{
			__header		__hold_the_line;
			//?
	}	le;
	struct	__measure_depth		{
			__header		__hold_the_line;
			int			depth;
	}	md;
	struct	__length_opt			{
			__header		__hold_the_line;
			int			curr_length;
			int			max_length;
			int			lo_depth;
			int			source_id;
	}	lo;
	struct	__heartbeat			{
			__header		__hold_the_line;
			int		radius;
			int		scan_id;
	}	hb;
	struct	__handover			{
			__header		__hold_the_line;
			Detectable	*tower;
			int			com_point;
	}	ho,li;
	
//	----------------------	//
	Message(int	_type,Detectable*src,Detectable*dst);
	Vec2D	getPos() 
	{
//		return	*(new HVector(head.pos[0],head.pos[1],0,0));
//		return	*(new HVector(head.pos[0],head.pos[1],0,0));
		return	Vec2D(head.pos[0],head.pos[1]);
	};
	Vec2D	getDir() 
	{
//		return	*(new HVector(su.dir[0],su.dir[1],0,0));
		return	Vec2D(su.dir[0],su.dir[1]);
	};
	int cmp(const Message*)	const
	{
		return	0;
	}
};
#undef	DEF

#endif
