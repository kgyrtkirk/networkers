#ifndef	CMPS_H
#define	CMPS_H

//	i know..this isn't the beutiest..but for now..it would suffice..
struct	cmp_uint32
{
	uint32	a;
#ifdef	WITH_CMP_CONSTRUCTOR
	cmp_uint32(const void*ptr)
	{
		a=*((const uint32*)&ptr);
	}
#endif
	int	cmp(const cmp_uint32*b)	const
							{		if(a==b->a) return	0;
									if(a<b->a)return	-1;
									else		return	1;	}
};

struct	cmp_flt32
{
	float	a;
#ifdef	WITH_CMP_CONSTRUCTOR
	cmp_flt32(const void*ptr)
	{
		a=*((float*)&ptr);
	}
#endif
	int	cmp(const cmp_flt32*b)	const
							{		if(a==b->a) return	0;
									if(a<b->a)return	-1;
									else		return	1;	}
};

#define	cmp_ptr	cmp_uint32


#endif
