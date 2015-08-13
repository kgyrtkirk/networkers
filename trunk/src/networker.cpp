#include	"networker.h"
#include	"opengl.h"
#include	"textures.h"
#include	"radar.h"
#include	"message.h"
#include	"xneg.h"
#include	"nw_info.h"
#include	"vis.h"


int		C_recon=			5;
int		C_opt=			1024;
int		C_spring=			C_opt/2;
float	v_max=			3.149;

float	v_recon=			0.9;
float	v_tower=			0.3;
float	d_scan=			0.7;
float	d_tower=			0.8;
float	d_tower_release=	0.05;
float	d_handover=		0.9;


extern	int	mode;

NetWorker::~NetWorker()
{

}

int	lo_idx(int a)
{
	return	((a+radar.timestamp/LO_ROUND)*13) % 1024;
}

bool	lo_node(int a)
{
	int	u;
	u=lo_idx(a);
	
	//	returns true if u = 2^k
	
	return	!( u & (u-1));

// 	int	q=radar.timestamp/LO_ROUND;
// // 	return	111;
// //	return	q;
// 	q=rev32(q);
// 	return	q>>=22;	/*low 10 bits only*/
}

int	lo_node_color(int a)
{
	int	u;
	u=lo_idx(a);
	return	bitcount(u-1);
}


void	NetWorker::step()
{
	DSet		dl;
	dl=radar.collect(pos[0],pos[1],tx_power);
	dl-=this;

	write_log("LOC	%5d	%3d	%08f	%08f",		radar.timestamp,
							serial,
							pos[0],pos[1]);

	if( (radar.timestamp % LO_ROUND) == 0 )
	{
		lo_parent=		0;
		lo_depth=			-1;
		curr_length=		-1;
		max_known_length=	-1;
		lo_source_id=		0;
	}

	range_error=0;

	switch(state)
	{
		case	S_INIT:
				if(serial==1)
				{
					new_state=S_TOWER;
					leader=true;
				}
				else
					new_state=S_SWEEP;
				known=0;
				tie_to_tower=0;
				counter=0;
			break;
		case	S_RECON:
				recon_step(dl);
			break;
		case	S_TOWER:
				tower_step(dl);
			break;
		case	S_SWEEP:
				sweep_step(dl);
			break;
		case	X_LEADER:
				assert(0 /*can't happen*/);
			break;
	}
	if(new_state!=state)
	{
// 		if(radar.timestamp && new_state==S_RECON && (serial==47 || serial==75 || serial==76 || serial==72 || serial==79))
// 			mode=0;
		lo_parent=0;
		lo_depth=			-1;
		lo_source_id=		0;
		curr_length=		-1;
		max_known_length=	-1;

		if(new_state!=S_TOWER)
		{
			K.empty();
			O.empty();
			max_known_length=0xffff;
		}
		else	GLOG(D01,"	=> tower %d	@%d",K.size(),serial);

		if(new_state==S_SWEEP)
		{
			dir=Vec2D(1.0f,0.0f);
			set_sweep_dest();
		}
		if(new_state == S_TOWER )
		{
			counter=-100;
			tower_connection=0;
			tie_to_tower=0;
			
		}
		else
			counter=0;
//		known=0;
		amok=0;
		state=new_state;
		counter=0;
		weight=0;
		
//		tie_to_tower=false;
	}
	
	counter++;

	rotor+=diff/10;
//	delete	dl;
}

void	NetWorker::send_message(int type,	Detectable*dst,	Detectable*tower,	int	dist)
{
	Message	*m=new Message(type,	this,	dst);
	switch(type)
	{
		case	LINK_INFO:
		case	HANDOVER:
				m->ho.tower=		tower;
				m->ho.com_point=	0;
			break;

		default:
			GLOG(CRI,"can't happen!");
	}
	dst->msg_queue.add(m);
}

void	NetWorker::send_message(int type,	Detectable*dst,int	depth)
{
	//	this will fill out the header
	assert(type<NMESSAGE_TYPES);
	Message	*m=new Message(type,	this,	dst);

	write_log("MSG	%5d	#%3d	#%3d	%s	%08f	%08f	%d",		radar.timestamp,
												serial,
												dst->serial,
												message_types[type],
												pos[0],pos[1],
												K.size());
	switch(type)
	{
		case	SCAN_INFO:
				m->si.authorative=	freeprio;
				m->si.query_pos[0]=dst->pos[0];
				m->si.query_pos[1]=dst->pos[1];
				m->si.curr_length=		curr_length+1;
				m->si.max_length=		max_known_length;
				m->si.lo_depth=		(lo_depth>=0 ? lo_depth+1 : -1 );
				m->si.lo_source_id=		lo_source_id;
			break;
		case	SCAN_UPDATE_REQ:
				m->sur.chain_len=chain_len;
			break;
		case	SCAN_UPDATE:
				m->su.dir[0]= dir[0];
				m->su.dir[1]= dir[1];
// 				m->su.tie_to_tower=		(state==S_TOWER || (tie_to_tower && known && known->serial < serial));
				m->su.tie_to_tower=		(state==S_TOWER || tie_to_tower);
				m->su.advertised_dist=	(state==S_TOWER ? 0 : curr_length+1 );
				m->su.max_length=		max_known_length;
				m->su.lo_depth=		(lo_depth>=0 ? lo_depth+1 : -1 );
				m->su.lo_source_id=		lo_source_id;
			break;
		case	MEASURE_DEPTH:
				m->md.depth=depth;
			break;
		case	LENGTH_OPT:
// 				LOG(MSG,"	---	%d",max_known_length);
				m->lo.max_length=	max_known_length;
				m->lo.curr_length=	curr_length;
				m->lo.lo_depth=	(lo_depth>=0	?	lo_depth+1		:	-1		);
				m->lo.source_id=	lo_source_id;
			break;
		default:
			break;
	}
	dst->msg_queue.add(m);
}



void	drawMessage(int tx,	Vec2D	src,Vec2D dst)
{
	Vec2D	v=dst-src;
	Vec2D	q=v.normalize();
	
	glPushMatrix();
	glRotatef(-atan2(v[0],v[1])*180/M_PI,0,0,1);
	
	drawQuad(	tx,		3.0f,
			-2,			0,
			 2,		 	v.mag()	);
	glPopMatrix();

}

void	NetWorker::render()
{
#define	SIZE	12
	
	glPushMatrix();
	
	glTranslatef(pos[0],pos[1],16.0f);
	
// 	glScalef(1000.0f,1000.0f,1000.0f);
	
	int	tx=TX_NODE_TOWER;
	switch(state*!leader)
	{
		case	X_LEADER:	tx=TX_NODE_LEADER;	break;
		case	S_TOWER:	tx=TX_NODE_TOWER;	break;
		default:	//S_SCAN
					tx=TX_NODE_SCAN;	break;
	}
	glprintf(SIZE/2,SIZE/2,SIZE/2,	"#%d\ns%d",serial,state);

//   glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	if(range_error)
	{
			drawQuad(	TX_PRIORITY,	-1.0f,
					-4*SIZE,	 4*SIZE,
					 4*SIZE,	-4*SIZE);
	}
	
	if(state == S_TOWER)
	{
		if(freeprio)
		{
			drawQuad(	TX_PRIORITY,	-1.0f,
					-SIZE,	 SIZE,
					 SIZE,	-SIZE);
		}

		if(lo_source_id)
		{
			int	cid=lo_node_color(lo_source_id) ;
			glColor3fv(	colors[cid]);
			if(curr_length == max_known_length)
				drawQuad(	TX_LENGTH_OPT_SELECT,	-1.0f,
						-SIZE*2,	 SIZE*2,
						SIZE*2,	-SIZE*2);
			else
				drawQuad(	TX_LENGTH_OPT,	-1.0f,
						-SIZE*2,	 SIZE*2,
						SIZE*2,	-SIZE*2);
		}
		if(K.size() - O.size() > 0)
			glColor4f(1.0f,0.0f,0.0f,1.0f);
		if(K.size() - O.size() == 1)
			glColor4f(1.0f,1.0f,0.0f,1.0f);
		if(K.size() - O.size() == 0)
			glColor4f(0.0f,1.0f,0.0f,1.0f);
		
	}
	
// 	if(O.size() == 0)
// 		glColor4f(1.0f,1.0f,1.0f,1.0f);
		
	
// 	if(	scan_upd_recvd)
		drawQuad(	tx,	-2.0f,
			-SIZE, SIZE,
			 SIZE,-SIZE);
	
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	MCursor	mc(&msg_queue);
	Message	*msg;
// 	int		d=1;
	//	message processing
	while( (msg=mc.next()))
	{
// 		if(msg->head.timestamp!=radar.timestamp)
// 			LOG(MSG,"x	%d	%d",radar.timestamp,msg->head.timestamp);
		drawMessage(TX_MESSAGE,pos,msg->getPos());
	}


	{
		::DCursor		dc(K);
		Detectable	*d;
// 		TowerEntry	*te;
// 		TECursor		tec(&tower_entries);
		glColor4f(1.0f,1.0f,1.0f,0.5f);
// 		while(te=tec.next())
		while((d=dc.next()))
			if(serial < d->serial )
				drawMessage(TX_LINK,pos,d->pos);
// 			send_message(LINK_ERROR,	te->d);
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	}

// 	{
// 		::DCursor		dc(O);
// 		Detectable	*d;
// // 		TowerEntry	*te;
// // 		TECursor		tec(&tower_entries);
// 		glColor4f(1.0f,1.0f,1.0f,0.5f);
// // 		while(te=tec.next())
// 		while((d=dc.next()))
// 			if(serial < d->serial )
// 				drawMessage(TX_DEPTH,pos,d->pos);
// // 			send_message(LINK_ERROR,	te->d);
// 		glColor4f(1.0f,1.0f,1.0f,1.0f);
// 	}

// 	if(known && state==S_RECON)
// 	{
// 		drawMessage(TX_LINK,pos,known->pos);
// // 		glColor3f(1.0f,0.0f,1.0f);
// 		drawMessage(TX_LINK,pos,pos+dir*10);
// 	}
		
// 	if(!neg_ok)
		glRotatef(rotor*360,0,0,1);
	
	if(nwi.getSelected()==this)
		glColor4f(1.0f,1.0f,0.0f,1.0f);
	else
		glColor4f(1.0f,0.0f,1.0f,0.25f);


	
	drawQuad(	TX_NODE_RANGE,	3.0f,
			-tx_power,-tx_power,
			tx_power,	 tx_power	);

		glRotatef(rotor*360,0,0,1);
	drawQuad(       TX_NODE_RANGE1,  3.0f,
				0,              -tx_power/2,
				tx_power,        tx_power/2     );




	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glPopMatrix();
  glEnable(GL_DEPTH_TEST);
// 	LOG(MSG,"cre%d	%f	%f",serial,pos[0],pos[1]);	
}


void	NetWorker::info(DSet&m0,DSet&m1,char*str)
{
	char	*p=str;
	p+=sprintf(p,"state: %d\n",state);
	p+=sprintf(p,"serial:%d\n",serial);
	p+=sprintf(p,"#p:		% 5.2f,% 5.2f\n",pos[0],pos[1]);
	p+=sprintf(p,"ts:		%d\n",radar.timestamp);
	p+=sprintf(p,"tO:		%d\n",radar.timestamp%C_spring);
	p+=sprintf(p,"tO:		%s\n",((radar.timestamp%C_opt)<C_spring)?"spring":"opt");
	p+=sprintf(p,"lo-curr:	%d\n",curr_length);
	p+=sprintf(p,"lo-max:	%d\n",max_known_length);
	p+=sprintf(p,"lo-dep:	%d\n",lo_depth);
	p+=sprintf(p,"lo-node?	%d\n",lo_node(serial));
	p+=sprintf(p,"lo-src:	%d\n",lo_source_id);
	p+=sprintf(p,"tie:		%d\n",tie_to_tower);
	p+=sprintf(p,"tie:		%d\n",tower_connection);
	
	switch(state)
	{
		case	S_TOWER:
				p+=sprintf(p,"#conn:	%d\n",K.size());
				p+=sprintf(p,"-conn-check-\n");
				{
					::DCursor		dc(K);
					Detectable	*d;
					while((d=dc.next()))
					{
						p+=sprintf(p," ** %d ** %s\n",d->serial,d->knows(this)?"ok":"FAIL");
						m1.add(d);
					}
				}
			break;
		case	S_RECON:
				p+=sprintf(p,"#conn:	%p\n",known);
				m1.add(known);
				p+=sprintf(p,"#d:		% 6.2f,% 6.2f\n",dir[0],dir[1]);
				p+=sprintf(p,"chain len: %d\n",chain_len);
			break;
		default:
				/* can't happen */
			break;
	}
	m0.add(debug0);
	m0.add(debug1);
// 	LOG(MSG,"str:%s",str);
// 	strcpy(str,"asd");
}
bool	NetWorker::knows(Detectable*d)
{
	switch(state)
	{
		case	S_TOWER:
			if(K.contains(d))
				return	1;
		default:
			break;
			
	}
	return	0;
}
