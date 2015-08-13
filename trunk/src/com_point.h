#include	"portability.h"
#include	<string.h>
#include	"opengl.h"
#include	"detectable.h"

#define	NUM_GROUPS	2

// #define	COM_POINT_JUMP
#define	COM_POINT_T_RANGE	1024
#define	COM_POINT_T_MIN	256
// #define	COM_POINT_T_RANGE	16384
// #define	COM_POINT_T_MIN	1024

class	ComPoint	:	public	Detectable
{
public:
	int			connected_to[NUM_GROUPS];
	int			see;
	int			time_remaining;
	Detectable	*known;
//
	ComPoint(double x,double y)	:	Detectable(x,y)	{	SETZERO(connected_to);
													known=0;	
													set_time();
													see=0;
												}
	virtual	~ComPoint();
	void	render();
	void	step();
	void	seen();
	int	getState();
	bool	knows(Detectable*d)		{return	d==known;	}
	int	getType()	const	{	return	T_COMPOINT;	}
	void	info(DSet&m0,DSet&m1,char*str);
	void	set_time();

};
