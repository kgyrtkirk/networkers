#include	"com_point.h"
#include	"textures.h"
#include	"message.h"
#include	"radar.h"
#include	"networker.h"

void	ComPoint::step()
{
	MCursor	mc(&msg_queue);
	Message	*msg;
	bool		lock=0;
	//	message processing
#ifdef	COM_POINT_JUMP
	time_remaining--;
#endif
	while( (msg=mc.next()) )
	{
		if(msg->head.timestamp==radar.timestamp )
			continue;
		switch(msg->type)
		{
			case	SCAN_INFO:
//					msg->head.sender
					if( 	!lock	&&
						(!known || (known && msg->head.state==S_TOWER && (msg->head.sender->pos - pos).mag() < (known->pos - pos).mag()))
						&& 	time_remaining>0 )
// 					if( !known && time_remaining > 0)
					{
						if(known)
							known->msg_queue.add(new Message(	LINK_ERROR,
														this,
														known));
						lock=1;
						known=msg->head.sender;
						msg->head.sender->msg_queue.add(new Message(INTERLOCK,
															this,
															msg->head.sender));
					}
					
// 					dir=(msg->getPos()-pos).normalize();
				break;
			case	INTERLOCK:
					if( known )
						known->msg_queue.add(new Message(	LINK_ERROR,
													this,
													known));
					known=msg->head.sender;
				break;
			case	LINK_ERROR:
					if( msg->head.sender == known )
						known=0;
					else	LOG(ERR,"LE but no conn");
				break;
		}
		mc.del_current();
		delete	msg;
	}
#ifdef	COM_POINT_JUMP
	if(time_remaining==0 && known)
	{
		if( known )
			known->msg_queue.add(new Message(	LINK_ERROR,
										this,
										known));
	}
	if(time_remaining == -2 )
	{
		known=0;
		pos=radar.rand_point();
	}
	if(time_remaining == -3 )
		set_time();
		
#endif

}
void	ComPoint::set_time()
{
	time_remaining=rand()%COM_POINT_T_RANGE+COM_POINT_T_MIN;
}

void	ComPoint::seen()
{
	see=30;
}

void	ComPoint::render()
{
#define	SIZE	16

	see--;
	if(see<0)
		see=0;
	
	if(!known)
	{
		drawQuad(	TX_COM_POINT,	16.0f,
				pos[0]-SIZE,pos[1]-SIZE,
				pos[0]+SIZE,pos[1]+SIZE);
	}else
		drawQuad(	TX_NODE_LEADER,	16.0f,
				pos[0]-SIZE,pos[1]-SIZE,
				pos[0]+SIZE,pos[1]+SIZE);

	
}
int	ComPoint::getState()
{
	return	0;
}

void	ComPoint::info(DSet&m0,DSet&m1,char*str)
{
	char	*p=str;
	p+=sprintf(p,"compoint	%d\n",serial);
#ifdef	COM_POINT_JUMP
 	p+=sprintf(p,"time_r:	%d\n",time_remaining);
#endif
	if(known)
	{
		p+=sprintf(p,"known:	%d\n",known->serial);
		if(known);
			m1.add(known);
	}
	else
		p+=sprintf(p,"known:	N/A\n");
}


ComPoint::~ComPoint()
{

}
