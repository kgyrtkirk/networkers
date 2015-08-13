#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

// #define	N	6
// #define	M	12

unsigned	int mirror32(unsigned int n);
// {
// 	int	m=n;
// 	m=((m&0x55555555) << 1)	| ((m&(0x55555555<<1)) >> 1);
// 	m=((m&0x33333333) << 2)	| ((m&(0x33333333<<2)) >> 2);
// 	m=((m&0x0f0f0f0f) << 4)	| ((m&(0x0f0f0f0f<<4)) >> 4);
// 	m=((m&0x00ff00ff) << 8)	| ((m&(0x00ff00ff<<8)) >> 8);
// 	m=((m&0x0000ffff) << 16)	| ((m&(0x0000ffff<<16)) >> 16);
// 
// 	return	m;
// }

int	old_main()
{
	int	i;
 	int	c[24][24];
	unsigned int	n;
	int	N;
	int	M=3;
	for(N=3;N<12 && M<24;N++)
	{
		int	found=0;
		while(!found && M<24)
		{
			for(n=0;n<1<<M;n++)
			{
				
				int	z=((1<<M)-1);
				memset(c,0,sizeof(c));
				int	u,v,w;
				int	ok=1;
				for(u=0;	u<M-2	&&	ok	;u++)
				for(v=u+1;v<M-1	&&	ok	&&	v-u<N-1	;v++)
				{
				for(w=v+1;w<M		&&	ok	&&	w-u<N	;w++)
				{
					int	g;
					unsigned	int	k0=((n>>u)|(n<<(M-u)))&z;
					unsigned	int	k1=((n>>v)|(n<<(M-v)))&z;
					unsigned	int	k2=((n>>w)|(n<<(M-w)))&z;
					
					for(g=0;g<8;g++)
					{
						if(! (	((g&1)?k0^z:k0)	&
								((g&2)?k1^z:k1)	&
								((g&4)?k2^z:k2)	)	)
		// 				if(! (	(((g&1)?~n:n)>>u)	&
		// 						(((g&2)?~n:n)>>v)	&
		// 						(((g&4)?~n:n)>>w)	)	)
							ok=0;
						else
							c[v-u][w-u]++;
					}
		// 			printf("	%d|%d	%x	%x\n",N,M,u,v);
				}
		// 			printf("	%d|%d	%x	%x\n",N,M,u,v);
				}
				if(ok)
				{
					int	i;
					for(i=0;i<M;i++)
						if((((n>>i)|(n<<(M-i)))&z) < n)
							break;
					unsigned int	m=mirror32(n) >> (32-M);
					if(i==M)
						for(i=0;i<M;i++)
							if((((m>>i)|(m<<(M-i)))&z) < n)
								break;

					if(i==M)
					{
						printf("	X	%d|%d	%x	%d	",N,M,n,n);
						for(i=0;i<M;i++)
							printf("%d ",!!(n&(1<<(M-i-1))));

						printf("\n");
						for(i=0;i<N*N;i++)
						{
							printf("%d ",c[i/N][i%N]);
							if((i%N)==N-1)
								printf("\n");
						}

// 						printf("	|	");
// 						for(i=0;i<M;i++)
// 							printf("%d ",!!(m&(1<<(M-i-1))));

						printf("\n");
						found=1;
					}
				}
			}
			if(!found)
				M++;
		}
	}
	printf("asd\n");
	return	0;
}
