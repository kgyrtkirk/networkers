#include	<bitset>		/*part of the stlport library*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>
#include	"port/portability.h"

using namespace std;

// #undef	uint64
// #define	uint64	unsigned long long
// #define	N	6
// #define	M	12

unsigned	int mirror32(unsigned int n)
{
	int	m=n;
	m=((m&0x55555555) << 1)	| ((m&(0x55555555<<1)) >> 1);
	m=((m&0x33333333) << 2)	| ((m&(0x33333333<<2)) >> 2);
	m=((m&0x0f0f0f0f) << 4)	| ((m&(0x0f0f0f0f<<4)) >> 4);
	m=((m&0x00ff00ff) << 8)	| ((m&(0x00ff00ff<<8)) >> 8);
	m=((m&0x0000ffff) << 16)	| ((m&(0x0000ffff<<16)) >> 16);

	return	m;
}

//	K	-	level of freedom
//	u	-	the mask we are testing
//	n 	-	number length in bits
//	s	-	mask length in bits

#define	MAX_BITS		128

template<size_t K>
inline	int	is_freeprio(bitset<MAX_BITS>&u,uint32 s,uint32 n)
{
	uint32	idx[K];
	uint32	t;
	uint8	ed_idx;
	uint32	g,i;
	uint32	curr_idx=0;
	
	bitset<MAX_BITS>	z;
	z.set();
	z>>=MAX_BITS-s;				//	mask for 's' bits
	
	t=K-1;
	for(i=0;i<K;i++)
		idx[i]=i;			//	initialize search
	
	u=u&z;
	while(1)
	{
		bitset<MAX_BITS>	r;
		bitset<MAX_BITS>	parts[K][2];
		
		for(g=0;g<K;g++)
		{	//	create idx[g] for 1 and 0 too!
			parts[g][0]=		((u>>idx[g])	|	(u<<(s-idx[g])))&z;
			parts[g][1]=		parts[g][0]^z;
			
			
// 			printf("G	%d		%s\n",g,parts[g][0].to_string().c_str()+MAX_BITS-s);

		}
			
		for(g=0;g<(1<<K);g++)
		{
// 			r^=r;	//	r=0
// 			r=~r;	//	set all bits
			r.set();

			for(i=0;i<K;i++)
				r&=parts[i][!(g&(1<<i))];
			if( r.none() )
				return	0;
		}
		
		
		while(t && (idx[t]>=s-(K-t) || idx[t]-idx[0]>=n-(K-t)))
			t--;
		
		if(t==0 && idx[t] >=s-(K))
			break;
		idx[t]++;
		while(t<K-1)
		{
			idx[t+1]=idx[t]+1;
			t++;
		}
	}
	return	1;
}

	
template<size_t K>
inline	int	is_freeprio(uint64 u0,uint32 s,uint32 n)
{
	bitset<MAX_BITS>	u(u0);
	return	is_freeprio<K>(u,s,n);
}
	
	int	tx_test();
	
enum	fp_modes
{
	M_MIRROR=1,
	M_STD,
};
	
template<size_t K,enum fp_modes FP_MODE>
inline	int	fp_search(int	M,int N,int depth)
{
	static	uint32	bits[MAX_BITS/2];
	size_t	END_DEPTH;
	size_t	RECURSION_BIT_DEPTH;
	int		r=0;
	
	switch(FP_MODE)
	{
		case	M_MIRROR:	END_DEPTH=M/4;
					RECURSION_BIT_DEPTH=M/2;	break;
		case	M_STD:	END_DEPTH=M/2;
					RECURSION_BIT_DEPTH=M;	break;
	}
	
// 	if(depth > END_DEPTH - K && depth <=END_DEPTH)
	if(depth == END_DEPTH)
	{
		bitset<MAX_BITS>	word;
		word.reset();
		
		switch(FP_MODE)
		{
			case	M_MIRROR:
					{
						bitset<MAX_BITS>	w2;
						bitset<MAX_BITS>	mask;
						for(int i=0;i<depth;i++)
							word.set(bits[i]);
						w2=~word;
						w2<<=RECURSION_BIT_DEPTH;
						w2=word|w2;
						
// 						w2=~word;
						
						if(is_freeprio<K>(w2,M,N))
						{
							printf("X	X	%d|%d		%s\n",N,M,w2.to_string().c_str()+MAX_BITS-M);
							r|=1;
						}
						
// 						mask
						mask.set();
						mask>>=MAX_BITS-END_DEPTH;				//	mask for 's' bits
						
						word=	(( word >> END_DEPTH) << (3 * END_DEPTH))	|
								((~word >> END_DEPTH) << (2 * END_DEPTH))	|
								(( word &  mask) << (1 * END_DEPTH))	|
								((~word &  mask) << (1 * END_DEPTH))	;

						if(is_freeprio<K>(word,M,N))
						{
							printf("X	X	%d|%d		%s\n",N,M,word.to_string().c_str()+MAX_BITS-M);
							r|=1;
						}

// 			printf("TT	%d|%d		%s\n",N,M,word.to_string().c_str()+MAX_BITS-M);
// 			if(M!=16)
// 						assert(0);
					}
					
// 01000001100101101111
// 10111110011010010000
// 00000101101101001111
// 11111010010010110000

				break;
			case	M_STD:
					for(int i=0;i<depth;i++)
						word.set(M-1-bits[i]);
					if(is_freeprio<K>(word,M,N))
					{
						printf("X	X	%d|%d		%s\n",N,M,word.to_string().c_str()+MAX_BITS-M);
						r|=1;
					}
				break;
			default:	assert(0);
		}
			
	}
	if(depth==0)
		memset(bits,0,sizeof(bits));
	
// 	1101001011110000
	
	uint32	st,ed,b;
	if(depth)
	{
		st=bits[depth-1]+1;
		ed=st+K;
		if(depth==1)
			st=bits[depth-1]+2;
		if(2*depth+2*K-2<ed)
		{
			ed=2*depth+2*K-2;
		}
		if(2*depth-2>st)
			st=2*depth-2;
// 		if(depth>=K && bits[depth-K]+2*K-3>st)
// 			st=bits[depth-K]+2*K-3;
// 		if(depth>=K && bits[depth-K]+2*K-3>st)
// 			st=bits[depth-K]+2*K-3;
// 		if(depth>=K && bits[depth-K]+2*K-3>st)
// 			st=bits[depth-K]+2*K-3;
// 		if(4*depth+K+1<ed)
// 		{
// 			printf(">%d",depth);
// // 			st=depth/2+K;
// 			ed=4*depth+K+1;
// 		}



	}
	else
	{
// 			printf("	INIT\n");
		st=K;
		ed=K+1;
	}
	
	for(b=st;b<ed && b<RECURSION_BIT_DEPTH;b++)
	{
		bits[depth]=b;
		r|=fp_search<K,FP_MODE>(M,N,depth+1);
	}
	return	r;
}
	
int	main()
{
	int	i;
//  	int	c[24][24];
	uint64	n;
	int	N;
	int	M=2;
#define	N_MAX	99
#define	M_MAX	MAX_BITS


// #define	M_MAX	40
	bitset<128>	a;
// 	string("11") >> a;
// 	printf("asd%lld\n",a.to_ulong());
	a.set(127);
	a>>=1;
	
// 	for(i=0;i<128;i++)
// 		if(a[i])
// 			printf("1");
// 		else	printf("0");
	
// 	printf("%s\n",a.to_string().c_str());
// 	cout << 1;
// 	return	0;

	uint64	r=0xffffffffffffffffll;
// 	memset(&r,0xff,sizeof(r));
// 	r/=5;
	r/=(1ll<<40ll)+1;

// 	tx_test();
// 	return	0;
	
	assert(sizeof(uint64)==8);
	printf("%d	%llx\n",sizeof(uint64),r);
//	K		mask
//	2	6	0 0 1 0 1 1
//	3	19	0 0 0 0 1 0 1 0 1 1 1 1 0 0 1 1 0 1 1
//	3	19	0 0 0 0 1 0 1 0 1 1 1 1 0 0 1 0 0 1 1


#define	K		3
//	M=1<<K;
	M=20;
	for(N=K;N<N_MAX && M<M_MAX;N++)
	{
		int	found=0;
		while(!found && M<M_MAX)
		{
// 			if((M%8) != 0 && !found)
// 			{
// 				M++;
// 				continue;
// 			}

			uint64	xl=1;
			xl<<=(M-K);
			printf("-->M:	%d		%x	",M,((1<<K)-1));
			printf("\n");
			if((M-K)>0)
				found=fp_search<K,M_STD>(M,N,0);
			if(!found)
				M++;
		}
	}
	printf("asd\n");
	return	0;
}
