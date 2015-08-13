#include	"networker.h"
#include	"radar.h"
#include	"xneg.h"
#include	"flow.h"

HVector	tower_flow(HVector d,int opt,bool opt_phase);


void	NetWorker::tower_step(DSet&dl)
{
	MCursor		mc(&msg_queue);
	Message		*msg;
	scan_upd_recvd=false;
	Detectable	*restore_link=0;
	Detectable	*lo_candidate=0;
// 	int		d=1;
	
	
	//	length optimalization - LO node:
	if( (radar.timestamp % LO_ROUND) == 0	&&
		state  == S_TOWER				&&
		lo_node(serial) 				)
	{	//
		curr_length=		0;
		max_known_length=	0;
		lo_parent=		0;
		lo_depth=			0;
		lo_source_id=		serial;
		
		Detectable	*d;
		::DCursor		dc(K);
		while((d=dc.next()))
			send_message(LENGTH_OPT,	d);
	}
	
	//	message processing
	while( (msg=mc.next()) )
	{
		if(msg->head.timestamp==radar.timestamp)
			continue;
		switch(msg->type)
		{
			case	SCAN_UPDATE_REQ:
// 					if(K.size()==1 && !leader)
// 					{
// 						Detectable	*d=K.rand();	
// 						dir=(msg->getPos()-d->pos).normalize();
// 					}
// 					else
					dir=(msg->getPos()-pos).normalize();
					if(isnan(dir[0]) || K.size()<1)
					{
						dir=Vec2D(1.0f,0);
					}
// 					{
// 					
// 						Vec2D	s;
// 						s=dir;
// 						s.rot90();
// 						
// 						Detectable	*d;
// 						Detectable	*max;
// 						Vec2D		z,r;
// 						::DCursor		dc(K);
// 						
// 						max=0;
// 						while(d=dc.next())
// 						{
// 							z=d->pos - msg->getPos();
// 							r=pos-d->pos;
// 							r.rot90();
// 							if(	z.dot(s) > 0.0f &&
// 								r.dot(s) > 0.0f	)
// 							{
// 								max=d;
// 								break;
// 							}
// 						}
// 						
// 						if(max)
// 						{
// 							dir=r.normalize();
// 						}
// 
// 					}
					
// 					printf("SU:	%d	%d\n",serial,	msg->head.sender->serial);
					send_message(SCAN_UPDATE,	msg->head.sender);
					scan_upd_recvd=true;
				break;
			case	INTERLOCK:
					K+=msg->head.sender;
					lo_parent=0;
				break;
			case	HEARTBEAT:
				break;
			case	LINK_ERROR:
					GLOG(D03,"serial:%d	is recv LE	%d	@@@@@@@@@@@@@@@@@",serial,msg->head.src_id);
					if(K.contains(msg->head.sender))
						K-=msg->head.sender;
					else
						GLOG(ERR,"LE[tower not found]	%d	@	%d	%d",msg->head.sender->serial,serial,knows(msg->head.sender));
					if(!K.size())
						restore_link=msg->head.sender;
					if(K.size()==1)
						counter=0;
					if(O.contains(msg->head.sender))
						O-=msg->head.sender;
					lo_parent=0;
				break;
			case	LINK_INFO:
					debug0=msg->head.sender;
					debug1=msg->li.tower;
#define	CLEAR_TIME	128
					{
						Detectable	*r=msg->head.sender;
						Detectable	*t=msg->li.tower;
						if(counter>CLEAR_TIME && K.size()>1)
						if(
// 							(t->pos - pos).mag() < ( (r->pos - pos).mag() + (r->pos - t->pos).mag() ) &&
							(t->pos - pos).mag()/tx_power < 1.0f && rand()%2)
						{
							if(		 K.contains(msg->head.sender)	&&
									!K.contains(msg->li.tower)	)
							{
								GLOG(D03,"LE	%d	->	%d",serial,msg->head.sender->serial);
								send_message(LINK_ERROR,	msg->head.sender);
								K-=msg->head.sender;
								K+=msg->li.tower;
								send_message(INTERLOCK,	msg->li.tower);
								GLOG(D03,"IL	%d	->	%d",serial,msg->li.tower->serial);
								counter=0;
							}
						}
					}
				break;
			case	LENGTH_OPT:
					//	FIXME:	make sure we are connected --- why?
					//	if we already have a parent, then ignore this message
					if(K.contains(msg->head.sender) && !lo_parent )
					{
						max_known_length=	msg->lo.max_length;
						curr_length=		msg->lo.curr_length+1;
						lo_depth=			msg->lo.lo_depth;
						lo_source_id=		msg->lo.source_id;
						
						/* roll the aggregate */
						if(K.size()>2)
							curr_length=0;
						if(curr_length > max_known_length)
							max_known_length=curr_length;
						lo_parent=	msg->head.sender;
						
						{	/* send lo message to everybody execept our parent */
							Detectable	*d;
							::DCursor		dc(K);
							while((d=dc.next()))
								if(d!=lo_parent)
									send_message(LENGTH_OPT,	d);
						}
					}
				break;
			case	LEADER:
					leader=true;
				break;
			case	SCAN_INFO:
					if(	(//(msg->head.state == S_RECON )		||
// 						( msg->head.state == S_TOWER && xneg4(msg->head.sender->serial,radar.timestamp/PHASE_CNT) > xneg4(serial,radar.timestamp/PHASE_CNT) ) )		&&
						( msg->head.state == S_TOWER ))		&&
//												false)	&&
						msg->si.authorative						&&
						counter > 0							&&
						lo_parent								&&	//	we are an intermediate node
						msg->si.lo_source_id == lo_source_id		&&	//	in the same group
						msg->si.lo_depth  > 0					&&	//	we are not the source
						msg->si.max_length  < max_known_length		&&	//	we need to have the measurement node outer of this circle
// 						curr_length > 	msg->si.curr_length			&&	
// 															&&	//	we need to have the measurement node outer of this circle
						msg->si.curr_length < max_known_length		&&	//	and it's better!
						msg->si.lo_depth	< lo_depth			&&	//	don't do any studpid thing
						K.contains(lo_parent)					&&
						!K.contains(msg->head.sender)				&&
						msg->si.curr_length >=	0				)	//	we are still connected to lo_parent
					{
						write_log("-L-	%5d	#%3d	#%3d	%s	%08f	%08f	%d",		radar.timestamp,
																	serial,
																	msg->head.sender->serial,
																"lo_candidate",
																	pos[0],pos[1],
																	K.size());

						LOG(MSG,"	SILO	%d|%d	ld:%d<%d	ml:%d<%d	cl:%d",
						serial,msg->head.sender->serial,
								msg->si.lo_depth,lo_depth,
										msg->si.max_length,max_known_length,
												msg->si.curr_length);
						lo_candidate=msg->head.sender;
					}
				break;
// 			case	SCAN_INFO:
// 					if(msg->head.state==S_TOWER && serial < msg->head.src_id && tie_to_tower)
// 					{
// 						::DCursor		dc(K);
// 						Detectable	*d;
// // 						TowerEntry	*te;
// // 						TECursor		tec(&tower_entries);
// 						float		switch_alpha=1.1f;
// 						while((d=dc.next()))
// 						{
// 							if((d->pos - pos).mag() > switch_alpha * (msg->head.sender->pos - pos).mag())
// 							{
// //#define	TIE_IN
// #ifdef	TIE_IN
// 								send_message(LINK_ERROR,	d);
// 								d=	msg->head.sender;
// // 								te->dist=!msg->head.state-1;
// 								send_message(INTERLOCK,	d);
// #endif
// 								break;
// 							}
// 						}
// 					}
// // 					if(K.size()==0 && msg->head.state==S_TOWER)
// // 					{
// // 						K+=msg->head.sender;
// // 						send_message(INTERLOCK,	msg->head.sender);
// // 					}
// 					break;
			case	MEASURE_DEPTH:
				{
// 					TowerEntry*	te=K.search((cmp_uint32*)&msg->head.sender);
					if(K.contains(msg->head.sender))
					{
					
						O+=msg->head.sender;
						weight+=msg->md.depth+1;
						
// 						te->dist=msg->md.depth;
// 						TowerEntry*	lte;
// 						int	c=0;
// 						int	dist=0;
// 						TECursor		tec(&tower_entries);
// 						while(te=tec.next())
// 							if(te->dist>=0)
// 							{
// 								dist+=te->dist+1;
// 								c++;
// 							}
// 							else
// 								lte=te;
						if(O.size()+1==K.size())
							send_message(MEASURE_DEPTH,	(K-O).rand(),	weight);
						if(O.size() == K.size())
						{
							::DCursor	dc(K);
							Detectable	*d;
							while((d=dc.next()))
								if(d!=msg->head.sender)
									send_message(MEASURE_DEPTH,	d,	weight);
						}
					}
				}
				break;
			default:
				break;
		}
		mc.del_current();
		delete	msg;
	}
	if(scan_upd_recvd && !K.size() && restore_link && !leader)
	{
		K+=restore_link;
		send_message(INTERLOCK,	restore_link);
	}

	freeprio=1;
	int	phase=radar.timestamp % PHASE_CNT;
	{
		Detectable	*d;
		int	xa=xneg4(serial,		radar.timestamp/PHASE_CNT);
// 		if(serial==7)
// 			LOG(CRI,"	fpd	%d	->	%d	%d",serial,xa,radar.timestamp%33);
		::DCursor		dc(K);
		
		while((d=dc.next()))
		{
// 			if(serial==7)
// 				LOG(CRI,"	fpd#	%d	->	%d",d->serial,xneg4(d->serial,	radar.timestamp));
			if(xneg4(d->serial,	radar.timestamp/PHASE_CNT)<xa)
				break;
		}
		if(d)
			freeprio=0;
// 		if(serial==7)
// 			LOG(CRI,"	fpR	%d	---->	%d",serial,freeprio);
	}	
	
	
	if( phase==PHASE_TOWER_LINK_INFO && freeprio && K.size()>=2 )
//	if( K.size()>=2 && counter > 10 &&  ((counter+irand) % C_recon)>2 && (((counter+irand) % C_recon)%3)>1 && radar.timestamp%2 )
	{
		Detectable	*tb;
		Detectable	*tc;
		if(freeprio)
		{
			tb=K.rand();
			DSet		Q=(K-tb);
			Q-=lo_parent;
			
//			::DCursor		dc(Q);
			while((tc=Q.rand()))
			{
			
				if(tc->getState() && (tc->pos - tb->pos).mag()<tx_power)
					break;
				Q-=tc;
			}
		}
		
		if(freeprio && tb && tc && tb->getState() && tc->getState() )
		{
//			int		xa,xb,xc;
// 			xa=xneg(serial,	radar.timestamp%((2*CEIL_LOGN)+1),	CEIL_LOGN);
// 			xb=xneg(tb->serial,	radar.timestamp%((2*CEIL_LOGN)+1),	CEIL_LOGN);
// 			xc=xneg(tc->serial,	radar.timestamp%((2*CEIL_LOGN)+1),	CEIL_LOGN);
			
// 			if(	xb > xa && xa < xc	// tb->dist!=0 && tc->dist!=0 &&  
// 				)

			{
	// 			if(serial==9 || serial == 10)
	//  			LOG(MSG,	"handing over:	%d	%d	%d",serial,tb->d->serial,tc->d->serial);
	// 			send_message(HANDOVER,	tb->d,	tc->d,	tc->dist);
	// 			send_message(HANDOVER,	tc->d,	tb->d,	tb->dist);
				if(rand()%2)
					send_message(LINK_INFO,	tb,	tc,	0);
				else
					send_message(LINK_INFO,	tc,	tb,	0);
	// 			new_state=S_RECON;
				return;
			}

		}
	}
//balatonalmadi
	if( K.size() <2 && !leader && freeprio && phase==PHASE_TOWER_RELEASE )
	{
		if(!scan_upd_recvd && counter>10)
		{
			Detectable	*d;
			::DCursor		dc(K);
			
			Vec2D	m(0,0);
			d=dc.next();
			if(d)
				m=d->pos-pos;

			if(m.mag()<tx_power*d_tower_release)
			{
				::DCursor		dc(K);
				d=dc.next();
				tower_connection=1;
				if(d)
				{
					send_message(LINK_ERROR,	d);
					known=d;
				}
				else
					known=restore_link;
				tie_to_tower=1;

	// 			if(serial==8 || serial==49)
					GLOG(D03,"serial:%d	is RECON		@@@@@@@@@@@@@@@@@",serial);
// 					GLOG(MSG,"serial:%d	is RECON		%p	%d",serial,known,K.size());
// 					GLOG(MSG,"serial:%d	is RECON		%p	%p	%d",serial,known,d,K.size());
				new_state=S_RECON;
				return;
			}
		}
	}
	
	if(	lo_candidate				&&
		freeprio					&&
		phase==PHASE_TOWER_LENGTH_OPT	&&
//		counter > 10				&&
		K.contains(lo_parent)		&& !scan_upd_recvd
		)
	{
		LOG(MSG,"LO optimalization	%d	[%d]	{%d}",serial,lo_candidate->serial,lo_parent->serial);
		send_message(LINK_ERROR,	lo_parent);
		send_message(INTERLOCK,	lo_candidate);
		K-=lo_parent;
		K+=lo_candidate;
// 		lo_parent=lo_candidate;
		lo_parent=0;
	}
	
// 	if( leader && K.size()==1 && !scan_upd_recvd )
// 	{
// 		Detectable	*d;
// 		d=K.rand();
// 		if(d->getState())
// 		{
// 			send_message(LEADER,	K.rand());
// 			leader=false;
// 			return;
// 		}
// 	}

// 	if(leader && tower_entries.getsize()>0)
// 	{
// 		LOG(MSG,"tes	%d",tower_entries.getsize());
// 	}

//	int	k=radar.get_fraction_size(0);
	
//	if(radar.timestamp > k*tx_power/(v_max))
//	{
// 		if( ((counter+irand) % C_recon)==0 && (K.size()>1 || leader))
		if( ((K.size()>1 && counter > TOWER_COOLDOWN )|| leader) && phase == PHASE_TOWER_SCAN_INFO)
	// 	if( (counter % C_recon)==0 )
		{
			Detectable	*d=(dl-K-this).rand();
			if(d)
				send_message(SCAN_INFO,	d);
		}
// 	}
// 	else
// 	{
// 		if(radar.timestamp == k*TX_POWER/(v_max))
// 		{
// 			::DCursor		dc(K);
// 			Detectable	*d;
// 			while((d=dc.next()))
// 				send_message(SCAN_INFO,	d);
// 		}
// 	}

	
	if( (radar.timestamp % C_opt) == 0)
	{
		weight=0;
		O.empty();
// 		TowerEntry	*te;
// 		TECursor		tec(&tower_entries);
// 		while(te=tec.next())
// 			if(te->dist>0)
// 				te->dist=-1;
		if(leader)
			LOG(MSG,"spring-net");
	}
	
	if( (radar.timestamp % C_opt) == C_spring )
	{
		if(leader)
			LOG(MSG,"optimizing");
// 		TowerEntry	*te;
// 		TECursor		tec(&tower_entries);
		::DCursor		dc(K);
		Detectable	*d;
		while((d=dc.next()))
		{
			if(d->getState()==0)
				O+=d;
		}
		if(O.size()+1 == K.size())
		{
			LOG(MSG,"	@cp: %d",	serial);
//			O+=d;
//			if(K.size()==2)
			{
				dc.reset();
				while((d=dc.next()))
					if(d->getState()!=0)
						send_message(MEASURE_DEPTH,	d,	1);
			}
		}
	}
	
	//	move!
	if(K.size()>=1 && !scan_upd_recvd )
	{
			float	d=1.0/(K.size());//*K.size());
			
			Vec2D	dest;
			Vec2D	q;

			if(O.size()==0)
				dest=tower_dest_pos(pos,K,K,tx_power*d_tower);
			else
				dest=tower_dest_pos(pos,K,O,tx_power*d_tower);
			
			if((dest-pos).mag() > v_max*v_tower)
				dest=pos+(dest-pos).normalize() * v_max*v_tower;
			
			pos=(dest*d+pos*(1.0-d));
			
			
	}
}


//	@deprecated
HVector	tower_flow(HVector d,int opt,bool opt_phase)
{
	HVector	n=d.normalize();
	
	float	x=d.mag();
	float	f;
	
	if(opt_phase)
	{
		if( opt )
// 			f=	x-d_tower;
			f=	pow(MAX(0,(x-d_tower)*1/(1-d_tower)),2);
// 			f=	-(MIN((d_tower-1)*x/d_tower/d_tower,0)+MAX(0,(x-d_tower)*1/(1-d_tower)));
		else	
			f=	pow(x,2);
			
	}
	else
		f=	pow(x, 2);

	
// 	for i=[1:size(x,1)]
// 	for j=[1:size(x,1)]

// 	q=sqrt(ux(i,j)^2+uy(i,j)^2);
	
	
// 	f=x-d_tower;
// 		ux(i,j)/=q;
// 		uy(i,j)/=q;
// 		
// %  		if(q>1)
// %  		end
// 			ux(i,j)*=s-q;
// 			uy(i,j)*=s-q;
	

		
// 		f=	x;
// 	f=	x;
	if(f<0.0)
		f=0.0;
	if(f>1.0)
		f=1.0;
//	n*=f;
	
	if(x<0.01f)
		return	HVector(0,0,0,0);
//	if(n>
	return	n*f;
}


