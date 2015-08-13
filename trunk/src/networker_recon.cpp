#include	"networker.h"
#include	"radar.h"

//
//			 X	-	me
//		/	 |
//	    /	 |  p
//	   /		\|/
//	 |/		 x	-	known
//	d - advertised direction

Vec2D	scanner_flow(Vec2D	p,Vec2D	d)
{
	Vec2D	r(d[1],-d[0]);
	float	x,y;
	
	r=r.normalize();
	d=d.normalize();
	
	x=r.dot(p);
	y=d.dot(p);
#define	s	d_scan

// 	HVector	u(-x,-y+(1-fabsf(x)) * s /(fabsf(y-s)+1),0,0);
	Vec2D	u(-x,-y+s);
	Vec2D	t(y,0);
	
	float	m=MAX(0, 1 - 1.0f/(1.0f-s) * sqrtf(x*x+(y-s)*(y-s)));
//  	u[0]+=0.1f;
	u=u.normalize();
	
	Vec2D	res=t*m*v_recon+u*(1-m);
	
	return	r*res[0]+d*res[1];
}

#define	SENDER		msg->head.sender
#define	S(X)			msg->head.state
#define	ID(X)		msg->head.src_id
#define	DEPTH(X)		msg->md.depth
#define	CHAIN_LEN(X)	msg->sur.chain_len

void	NetWorker::recon_step(DSet&dl)
{
	
	MCursor		mc(&msg_queue);
// 	MList		junk;
	Message		*msg;
	int			sur_len=9999;
	int			breakdown=0;
	Detectable	*sur_src=0;
//	int	_last_chain_len=chain_len;
	chain_len=1;
	
	freeprio=1;

	
	if(known && (known->pos - pos).mag() > tx_power)
	{
		GLOG(D01,"lost:%d	%f",serial,(known->pos - pos).mag()/tx_power);
	}
	//	message processing
	while( (msg=mc.next()) )
	{
		if(msg->head.timestamp==radar.timestamp)
			continue;
		switch(msg->type)
		{
			case	INTERLOCK:
					if(tie_to_tower)
					{
						new_state=S_TOWER;
						K+=msg->head.sender;
// 						tower_entries.add(new TowerEntry(msg->head.sender,!msg->head.state));
						if(known)
						{
							send_message(INTERLOCK,		known);
							K+=known;
// 							tower_entries.add(new TowerEntry(known,0));
						}
// 						K.sort();
					}
					else
						send_message(LINK_ERROR,		msg->head.sender);
						
				break;	
			case	SCAN_INFO:
// #define	AUTONOM_RECONS

#ifdef	AUTONOM_RECONS

					if(	( S(R) == S_TOWER && (!known || (known && tower_connection && (known->pos -pos).mag()>(msg->head.sender->pos-pos).mag() ) || (rand()%150)==0 ))	||
						( (S(R) == S_RECON || S(R) == S_SWEEP) && (counter>10 || S(R)==S_SWEEP) && ID(R) < serial && (!known || known->serial < ID(R) || tower_connection) && (SENDER->pos-pos).mag()/tx_power<d_handover ) )
// 						( S(R) == S_RECON && ID(R) < serial && (rand()%2) && (!known || known->serial < ID(R) || tower_connection) && (SENDER->pos-pos).mag()/tx_power<d_handover ) )
						{
							tower_connection=(S(R)==S_TOWER);
							known=msg->head.sender;
						}
#else
					if(	( S(R) == S_TOWER && (!known || (known && tower_connection && (known->pos -pos).mag()>(msg->head.sender->pos-pos).mag() )))	||
						( (S(R) == S_RECON || S(R) == S_SWEEP) && (counter>10 || S(R)==S_SWEEP) && ID(R) < serial && (!known || known->serial < ID(R) || tower_connection) && (SENDER->pos-pos).mag()/tx_power<d_handover ) )
						{
							tower_connection=(S(R)==S_TOWER);
							known=msg->head.sender;
						}
#endif						
// 					if(amok && !tower_connection)
// 					{
// 						known=0;
// 						tower_connection=false;
// 					}
// 					if(serial==2)
// 						LOG(MSG,"tt	%d	%d	[%d]",serial,tie_to_tower,msg->head.src_id);
				break;
			case	SCAN_UPDATE:
// 					dir=(msg->getDir().normalize()*0.9+dir*0.1).normalize();
// 					dir[0]=1;
// 					dir=((msg->getDir().normalize()));//*0.9+dir*0.1).normalize();
// 					dir=((msg->getDir().normalize())*0.9+dir*0.1).normalize();
					{
						
					
						Vec2D	nd=msg->getDir().normalize();
						Vec2D	od=dir.normalize();
						
						if(acos(nd.dot(od)) > ALPHA)
						{
							float	na=atan2(nd[1],nd[0]);
							float	oa=atan2(od[1],od[0]);
							
							float	a;
							
							if(na-oa > M_PI)
								oa+=2*M_PI;
							if(oa-na > M_PI)
								na+=2*M_PI;
							
							if(na>oa)
								a= ALPHA;
							else	a=-ALPHA;
							
							float	sa=sinf(a+na);
							float	ca=cosf(a+na);
							
							dir=Vec2D(ca,sa);
							
						}
						else
							dir=nd;
							
						dir=nd;
						
						if(isnan(dir[0]) || isnan(dir[1])||
							isinf(dir[0]) || isinf(dir[1]))
							{
								dir[0]=1.0f;
								dir[1]=0.0f;
							}
						
// 						float	na=atan2(nd[1],nd[0]);
// 						float	oa=atan2(od[1],od[0]);
// 						if(na-oa > M_PI)
// 							oa+=2*M_PI;
// 						if(oa-na > M_PI)
// 							na+=2*M_PI;
// 						
// 						float	a=na*0.2+oa*0.8;
// 						float	sa=sinf(a);
// 						float	ca=cosf(a);
						
// 						dir=Vec2D(ca,sa);
						

// 						printf("	>	%f	%f	%f	%f\n",dir[0],dir[1],nd[0],nd[1]);
// 						dir=nd;
// 						dir=nd;
					}
					
					
// 					dir=((msg->getDir().normalize())*0.9+dir*0.1).normalize();
// 					dir=(msg->getDir().normalize());//*0.9+dir*0.1).normalize();
					
					tower_connection=(S(R)==S_TOWER);
					tie_to_tower=		msg->su.tie_to_tower;
					curr_length=		msg->su.advertised_dist;
					max_known_length=	msg->su.max_length;
					lo_depth=			msg->su.lo_depth;
					lo_source_id=		msg->su.lo_source_id;
// 					if(serial==2)
// 						LOG(MSG,"tt	%d	%d",serial,tie_to_tower);
// 					dir=(dir+HVector(dir[1],-dir[0],0,0)*0.1/(1+chain_len)).normalize();
				break;
			case	SCAN_UPDATE_REQ:
					
// 					if(CHAIN_LEN(R) < sur_len  )
// 					{
						if(sur_src)
							breakdown=1;
						sur_src=					msg->head.sender;
// 						sur_len =CHAIN_LEN(R);
// 					}
					
					chain_len=MAX(chain_len,CHAIN_LEN(R)+1);
					send_message(SCAN_UPDATE,	msg->head.sender);
// 					/(pos-known->pos).mag()
				break;
			default:
				break;
		}
// 		junk.add(msg);
		mc.del_current();
		delete	msg;
	}
// 	MCursor	jcr(&junk);
// 	while(msg=jcr.next())
// 	{
// 		msg_queue.del(msg);
// 		delete	msg;
// 	}
	//	step
	if(amok)
	{
		send_message(INTERLOCK,	known);
		new_state=S_TOWER;
		return;
	}
	//1323
// 	263	40	266
// 	257
// 	203
	
	int	phase=radar.timestamp%PHASE_CNT;
// 	if( !known || true ) //|| counter > C_recon)//((counter+irand) % C_recon)==0 )
// 	if( phase==PHASE_RECON_SCAN_INFO && (!known || counter > 10))//((counter+irand) % C_recon)==0 )
	if( phase==PHASE_RECON_SCAN_INFO && (!known || counter > 10))//((counter+irand) % C_recon)==0 )
	{ 
// #define	CRIPPLE_RECONS

#ifndef	CRIPPLE_RECONS
		Detectable	*d;
// 		if(sur_src)
// 			d=(dl-known-sur_src).rand();
// 		else
			d=(dl-known).rand();
		
		if(d && phase==PHASE_RECON_SCAN_INFO )
			send_message(SCAN_INFO,	d);
#endif
	}
	
// #define	EXPERIMENTAL
#ifdef	EXPERIMENTAL
	if( known )
	{
		Vec2D	m;
		Vec2D	desired;
		Vec2D	target;
		Vec2D	r;
		float	error=0.2f;
		float	s=0.7;
		r[0]= dir[1];
		r[1]=-dir[0];
// 		if(isnan(r[0]) || isnan(r[1]) || isinf(r[0])||isinf(r[1]))
// 			return;

		if(sur_src)
		{
			desired=	pos+dir*TX_POWER*s;
// 			float	u,v;
			Vec2D	er;
			er[0]=r.dot((sur_src->pos-pos));
			er[1]=dir.dot((sur_src->pos-pos));
			
			er[0]+=v_max*sin(ALPHA);
			er[1]+=v_max*cos(ALPHA);
		
			float	e0=
			error=((sur_src->pos-pos).mag()/TX_POWER-s)/(1.0f-s);
			float	e1=
				error=(sur_src->pos-desired).mag()/TX_POWER/(1.0f-s);
				
			error=MAX(e0,e1);
// 			error=(sur_src->pos-desired).mag()/TX_POWER/(1.0f-s);
// 			if((sur_src->pos-pos).dot(dir)<0.0)
// 				error=0.0;
			if(error<0.01f || isnan(error) || isinf(error))
				error=0.01f;
			if(error>0.99f)
				error=0.99f;
		}
		
		if(breakdown)
			error=1.0f;
// 		else
// 		{
// 			float	s=0.8;
// 			desired=	pos-dir*TX_POWER*s;
// 			error=(known->pos-desired).mag()/TX_POWER/(1.0f-s);
// 			error/=2;
// 			if(error<0.0f)
// 				error=0;
// 			if(error>0.99f)
// 				error=0.99f;
// 		}
// 			error=0.0f;
		
// 		if(known->getState()==S_TOWER)
// 			target=pos+dir*TX_POWER*s/10+r*(1.0f-error)*v_max;
// 		else
			target=known->pos+dir*TX_POWER*s+r*(1.0f-error)*v_max;
		

			
			if((pos-target).mag()<v_max*(1.0f-error))
				pos=target;
			else
			{
// 				printf("m:	%d	%f	%f\n",	serial,(target-pos).mag(),error);
				m=(target-pos).normalize()*v_max*(1.0f-error);
				pos+=m;
				
			}
			
// 			if(serial==5 || serial == 7)
// 			{
// // 				printf("m:	%d	%f	%f\n",	serial,(target-pos).mag(),error);
// 				printf("m:	%d	%f	%f\n",	serial,(target-pos).mag(),error);
// 				if(sur_src)
// 				printf("d:	%d	%f	%f\n",	serial,desired[0],desired[1]);
// 				printf("t:	%d	%f	%f\n",	serial,target[0],target[1]);
// // 				printf("M:	%d	%f	%f	%f\n",	serial,m[0],m[1],v_max);
// 			}
			
// 		}
// 		else
// 		{
// 			m=scanner_flow((pos - known->pos)/tx_power,dir.normalize());
// 			pos+=m*v_max*abs(curr_length+1)/abs(curr_length+chain_len+1);
// 		}
		send_message(SCAN_UPDATE_REQ,		known);
	}
		
	return;
#endif

	if( known )
	{
// 		LOG(MSG,"%f	%f",known->pos[0],known->pos[1]);
		Vec2D	m;
// #define	MAGNET
#ifdef	MAGNET
 		if(sur_src && curr_length > 2)
 		{
 			m=(sur_src->pos+known->pos)/2;
// 			m=known->pos+(sur_src->pos-known->pos).normalize()*d_scan;
//  			m=m-pos;
//  			m+=dir;
//  			if(m.mag() > v_max*d_scan)
//  				m=m.normalize()*v_max*d_scan;
			Vec2D	opos=pos;
			if( (pos-known->pos).mag() > d_scan || (pos-sur_src->pos).mag() > d_scan)
			{	// looks like spring forces are to powerful, fall-back to proven method
				m=scanner_flow((pos - known->pos)/tx_power,dir.normalize());
				pos+=m*v_max*abs(curr_length+1)/abs(curr_length+chain_len+2);
// 				assert(!isnan(pos[0]));
			}
			else
			{	// we have a green light, full speed!
				m=m-pos;
				if(m.mag() > v_max)
					m=m.normalize()*v_max;
				pos+=m;
// 				assert(!isnan(pos[0]));
			}
				if(isnan(pos[0]))
				{
					printf("NAN:	%f	ID:	%d	S:%d\n",known->pos[0],known->serial,known->getState());
					printf("OP:	%f,%f\n",opos[0],opos[1]);
					printf("P:	%f,%f\n",pos[0],pos[1]);
					printf("XA:	%d,%d\n",curr_length,chain_len);
					printf("M0:	%f,%f\n",m[0],m[1]);
					m=scanner_flow((pos - known->pos)/tx_power,dir.normalize());
					printf("M1:	%f,%f\n",m[0],m[1]);
// 					printf("NAN:	%f,%f\n",dir[0],dir[1]);
// 					printf("NAN:	%f,%f\n",m[0],m[1]);
					assert(0);
				}
//  			pos+=m;
 		}
		else
#endif
		{
		
			m=scanner_flow((pos - known->pos)/tx_power,dir.normalize());
// 		if(m.mag()>1.0f)
// 			LOG(MSG,"%f",m.mag());
// 		LOG(MSG,"D		%d",chain_len);
		//	we are the leader of the chain...
// 			pos+=m*(v_max);//*abs(curr_length+2)/abs(curr_length+chain_len+2);
// 			pos+=m*v_max*abs(curr_length+1)/abs(curr_length+chain_len+1);
			pos+=m*v_max*abs(curr_length+1)/abs(curr_length+chain_len+1);
// 			assert(!isnan(pos[0]));
// 			pos+=m*v_max/sqrt(chain_len);
		}

// 		pos+=m*v_max/((float)chain_len+1);
		
// 		if(serial==30)
// 	 		LOG(MSG,"%f	%f",known->pos[0],known->pos[1]);
		send_message(SCAN_UPDATE_REQ,		known);
	}
	
}
#undef	SENDER
#undef	S
#undef	ID
#undef	DEPTH
