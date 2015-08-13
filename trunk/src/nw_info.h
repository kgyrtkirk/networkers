#ifndef	NW_INFO_H
#define	NW_INFO_H
#include	"networker.h"
#include	"radar.h"

class	NetWorkerInfo
{
public:
	Detectable	*subject;
	char				str[8192];

	NetWorkerInfo()	:	subject(0)	{};
	const	Detectable*	getSelected()	{return	subject;	}
	void		render();
	void		text_render();
	void		select(Detectable*d)		{subject=d;	}
};

extern	NetWorkerInfo	nwi;

#endif
