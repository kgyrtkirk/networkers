#include	"set.h"

void	set_test()
{
	cSet<int,8>	a,b;
	int			qq[]={1,2,6,8,9,4};
	a.add(&qq[0]);
	a.add(&qq[1]);
	a.add(qq);
	b.add(&qq[3]);
	
	b.remove(qq);
	
	LOG(MSG,"x	%d",	(a+b).size());
	

}
