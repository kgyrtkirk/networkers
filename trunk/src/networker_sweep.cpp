#include	"networker.h"
#include	"radar.h"


int	NetWorker::getSweepK(int i)
{
	//	casting to int is equiv. to floor
	return	(int)((sqrt(1+8*i)+1)/2);
//	return	(int)((sqrt(1+8*i)+1)/2);
}

void	NetWorker::set_sweep_dest()
{
// #define	LINE_SWEEP
#ifdef	LINE_SWEEP
	dir=		pos+
			Vec2D(1,0)*(serial-1)*tx_power*0.5f;
// 			v*l;

#else

	if(serial==1)
	{
		dir=pos;
		return;
	}
	float	m=tx_power*0.5f;
	
	float	alpha=(serial%3)*2*M_PI/3;	//	0,120,240
	
	#define	D	6
	Vec2D	u=Vec2D(	sin(alpha-M_PI/D),	cos(alpha-M_PI/D)	)*m;
	Vec2D	v=Vec2D(	sin(alpha+M_PI/D),	cos(alpha+M_PI/D)	)*m;
	
	int	i=(serial-2)/3;
	int	k=getSweepK(i);
	int	l=i-k*(k-1)/2;
	
	k=k-l;	//	-->	k'+l= k
	
	dir=		pos+
			u*k+
			v*l;
#endif
}

void	NetWorker::sweep_step(DSet&dl)
{
	MCursor		mc(&msg_queue);
	Message		*msg;

	while( (msg=mc.next()) )
	{
		mc.del_current();
		delete	msg;
	}

	int	n=radar.get_fraction_size(0);
	int	k=getSweepK(n)+1;
	
// 	int	mk=getSweepK(serial)+1;
// 	if(radar.timestamp > mk*tx_power*d_scan/(sqrt(3)*v_max))
// 	{
	if(radar.timestamp > k*tx_power*d_scan/(sqrt(3)*v_max))
	{
		::DCursor		dc(dl);
		Detectable	*d;
		while((d=dc.next()))
			send_message(SCAN_INFO,	d);
		new_state=S_RECON;
// 		tie_to_tower=1;
	}

	Vec2D	d=dir-pos;
	if(d.mag() > v_max)
		d=d.normalize()*v_max;
	pos+=d;
}

