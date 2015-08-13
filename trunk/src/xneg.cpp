#include	"util/log.h"

int	xneg(int i,int q,int l)
{
	if(q>l)
	{
		q-=l;
		i^=(1<<l)-1;
	}
	return	i^((1<<q)-1);
}

//	level 4 for 9 bit
	const	uint8	xn4_9_src[]={0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,1,0,0,1};
	uint64			xn4_9=0;

//	7	--	3,4,10,14
int	xneg4(uint32 i,uint32 q)
{
	if(xn4_9==0)
	{
		LOG(MSG,"xneg:	%d",sizeof(xn4_9_src));
		for(uint32 i=0;i<sizeof(xn4_9_src);i++)
		{
			xn4_9<<=1;
			xn4_9|=xn4_9_src[i];
		}
	}
	int	r=q%33;
	if(r!=32)
		i^=((xn4_9<<r)|(xn4_9>>(sizeof(xn4_9_src)-r)))&0xffff;
	return	i;
}

int	xtest()
{
	int	i,j,k,t;
	#define	L	8
	#define	N	(1<<L)
	int	c[2*L];
	memset(c,0,sizeof(c));
	
	for(i=0;i<N;i++)
	for(j=0;j<N;j++)
	{
		if(i==j)
			continue;
		for(k=0;k<N;k++)
		{
			if(k==i || k==j)
				continue;
			for(t=0;t<2;t++)
			{
				int	xi,xj,xk;
				xi=xneg(i,t,L);
				xj=xneg(j,t,L);
				xk=xneg(k,t,L);
				if( xi > xj && xj < xk )
				{
// 					printf("%d	|	%d	%d	%d	-	%d	%d	%d		-	%d\n",t,i,j,k,xi,xj,xk,t);
					c[t]++;
					break;
				}
			}
			assert(t<2*L);
		}
	}
	for(t=0;t<2*L;t++)
		LOG(MSG,"	%d	%d",t,c[t]);
	return	0;	
}
