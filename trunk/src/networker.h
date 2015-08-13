#ifndef	NETWORKER_H
#define	NETWORKER_H
#include	"portability.h"
#include	<string.h>
#include	"opengl.h"
#include	"detectable.h"
#include	"util/list.h"
#include	"util/cmps.h"

#define	TX_POWER			40
#define	CEIL_LOGN			10

#define	LO_ROUND			(PHASE_CNT*32)


#define	ALPHA				0.05

#define	PHASE_CNT				4
#define	PHASE_TOWER_LINK_INFO	0
#define	PHASE_TOWER_RELEASE 	0

#define	PHASE_RECON_SCAN_INFO 	2
#define	PHASE_TOWER_SCAN_INFO	2
#define	PHASE_TOWER_LENGTH_OPT	3

#define	TOWER_COOLDOWN			128


// #define	CRIPPLE_RECONS
//	returns true if node is an LO node in this round
bool	lo_node(int);


enum	eState
{
	X_LEADER,
	S_INIT,
	S_RECON,
	S_TOWER,
	S_SWEEP,
// 	S_REPAIR,
};

/* global parameters */
extern	int		C_recon;
extern	int		C_opt;
extern	int		C_spring;
extern	float	v_max;

extern	float	v_recon;
extern	float	v_tower;
extern	float	d_scan;
extern	float	d_tower;
extern	float	d_tower_release;
extern	float	d_handover;	

void	write_log(const char*format,...);


class	NetWorker	:	public	Detectable
{
public:
	
	int			group_id;
// 	int			serial;
	float		noder;		//	0-1 random
// 	float		rotor;
// 	float		range;	
	int			counter;		//	program counter
	int			chain_len;
	eState		new_state;
	int			irand;
	eState		state;
	bool			leader;
	bool			tower_connection;
// 	bool			scan_info_recvd;
	bool			neg_ok;
	bool			tie_to_tower;
	bool			amok;
	bool			freeprio;
	Vec2D		dir;
	
//--	length optimization related
	int			curr_length;		//!<	current measurement
	int			max_known_length;	//!<	max_known_length
	int			lo_depth;
	Detectable	*lo_parent;		//!<	this nodes parent in the graph of 'lo_node()' rooted graph
	int			lo_source_id;
//--------------------------
	float		v_recon_max;
	bool			scan_upd_recvd;
	
	Detectable	*known;
	struct		TowerEntry{
		TowerEntry(Detectable*_d,int _cp)
		{
			d=_d;
			if(_cp)
				dist=0;
			else	dist=-1;
		}
		Detectable	*d;
		uint32		dist;
		const int	cmp(const TowerEntry*o)	const
		{
			return	o->d>d;
		}
	};
// 	typedef	cOffsetList<Detectable>		DSet;
	typedef	cListCursor<TowerEntry>		TECursor;

	Detectable	*debug0;
	Detectable	*debug1;

	uint32	res[33];

// 	cList<TowerEntry>									tower_entries;
// 	cOffsetList<TowerEntry,cmp_uint32,OFFSETOF(TowerEntry,d)>	K;
	DSet		K;
	DSet		O;
	int	weight;
	
	#define	MAX_CON	4
	
// 	Connection	con[MAX_CON];
	
// 	int		connected_to[NUM_GROUPS];
//	
//
	NetWorker(int _serial,double x,double y)	:	Detectable(x,y,TX_POWER),	dir(1,0)	//,	K(&entries)
	{	/*LOG(MSG,"cre%d",serial);*/
// 		tower_entries.add_key(&K);
		noder=drand48();
		irand=rand();
		state=S_INIT;
		serial=_serial;
		leader=false;
		known=0;
		neg_ok=0;
		dir[0]=drand48()-0.5f;
		dir[1]=drand48()-0.5f;
		dir=dir.normalize();
		assert(serial < (1<<CEIL_LOGN));
		pos[0]=x;
		pos[1]=y;
		debug0=debug1=0;
		freeprio=0;
		range_error=0;

	}
	virtual	~NetWorker();
	void	render();
	void	step();
	int	getState()	{	return	state;	}
	
	void	recon_step(DSet&dl);
	void	tower_step(DSet&dl);
	void	sweep_step(DSet&dl);
	
	void	set_sweep_dest();
	
	void	send_message(int type,	Detectable*dst,	int depth=0);
	void	send_message(int type,	Detectable*dst,	Detectable*tower,	int	dist);
	void	info(DSet&m0,DSet&m1,char*str);
	bool	knows(Detectable*d);
	int	getType()	const	{	return	T_NETWORKER;	}

	int	getSweepK(int i);
};


#define	GLOG(LEVEL,FORMAT,B...)		if(LEVEL < log_level)											\
								{															\
									glLog.add("%s@%03d#%03d	<%d>	" FORMAT ,						\
											log_levels_hr0[LEVEL],								\
																							\
												__LINE__,	radar.timestamp,serial,					\
														##B);								\
									LOG(LEVEL,FORMAT,##B);										\
								}

#define	MAX(A,B)	((A)>(B))?(A):(B)
#define	MIN(A,B)	((A)<(B))?(A):(B)

#endif
