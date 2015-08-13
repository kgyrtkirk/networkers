#include	"radar.h"
#include	"networker.h"

const	char*	message_types[]=
{
#define	MSGT(A)	#A,
#include	"message_types.h"
#undef	MSGT
};


Message::Message(int	_type,Detectable*src,Detectable*dst)
{
	if((src->pos - dst->pos).mag() > TX_POWER)
		src->range_error=1;
	type=_type;
	head.timestamp=radar.timestamp;
	head.src_id=	src->serial;
	head.dst_id=	dst?dst->serial:0;
	head.state=	src->getState();
	head.sender=	src;
	head.pos[0]=	src->pos[0];
	head.pos[1]=	src->pos[1];
}

