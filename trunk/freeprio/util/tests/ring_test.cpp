#include	<stdio.h>
#include	<stdlib.h>
#include	"ring.h"
#include	<time.h>

#define	TL	8
int	main()
{
	cRing<int,TL,RING_FIFO>	fifo;
	cRing<int,TL,RING_LIFO>	lifo;
	int	a[100];
	int	i;
	int	popi;
	int	pushi;
	popi=0;
	pushi=0;
	srand(time(0));

	for(i=0;i<100;i++)
		a[i]=i+1;

	while(popi<100)
	{
		XCHK(pushi-popi == fifo.length(),	ERR,	"pu:%d pi:%d rl:%d",pushi,popi,fifo.length());
		if(rand()%2)
		{	//	push
			if(pushi>=100)
				continue;
			printf("+");
			if(fifo.length()==TL)
			{
				assert(fifo.push(&a[pushi]));
				a[pushi]=-1;
			}
			else
				assert(!fifo.push(&a[pushi]));
			pushi++;
		}
		else
		{
			int	*p;
			printf("-");
			p=fifo.pop();
			while(a[popi]==-1)	//	ignore not pushed elements
				popi++;
			if(fifo.length()==0)
				XCHK(!p,	ERR,	"p(%p:%d)	a[%d](%p:%d)",p,*p,popi,&a[popi],a[popi])
			else
				XCHK(p==&a[popi],	ERR,	"p(%p:%d)	a[%d](%p:%d)",p,*p,popi,&a[popi],a[popi]);
			popi++;
		}
	}
	
	for(i=0;i<100;i++)
		a[i]=i+1;
// 	while(popi<100)
// 	{
// 		if(rand()%2)
// 		{
// 			
// 		}
// 	}
	assert(	!lifo.push(&a[0])	);
	assert(	!lifo.push(&a[1])	);
	assert(	!lifo.push(&a[2])	);
	
	assert(	3==lifo.length()	);
	int	*pp=lifo.pop();
//	printf("%p	%d\n",pp,*pp);
	assert(	&a[2]==pp			);
	assert(	&a[1]==lifo.pop()	);
	assert(	&a[0]==lifo.pop()	);
	
	assert(	!lifo.length()	);

	return	0;
}

