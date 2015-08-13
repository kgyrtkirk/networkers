#ifndef	EX_TYPES
#define	EX_TYPES
#include	"port/portability.h"

class	cmp_uint8
{
public:
	uint8	val;
	inline	int	cmp(const cmp_uint8*op)	const	{	if(val>op->val)	return	1;
										if(val<op->val)	return	-1;
										return	0;
									}
	inline	static uint8	 swap(uint8 v)  {return v;}
};

class	cmp_uint16
{
public:
	uint16	val;
	cmp_uint16()		{	}
	cmp_uint16(int iu)	{	val=iu;}
	inline	int	cmp(const cmp_uint16*op)	const	{	if(val>op->val)	return	1;
										if(val<op->val)	return	-1;
										return	0;
									}
	inline	uint16	static swap(const uint16 ival)		{	
										return	((ival&0xff)<<8) | ((ival&0xff00)>>8);
									}
};

union	uX32
{
	uint8	c[4];
	uint32	word;
};
class	cmp_uint32
{
public:
	uint32	val;
	cmp_uint32()		{	}
	cmp_uint32(int iu)	{	val=iu;}
	inline	int	cmp(const cmp_uint32*op)	const	{	if(val>op->val)	return	1;
										if(val<op->val)	return	-1;
										return	0;
									}
	inline	uint32 static swap(const uint32 v)	{	uX32	q;
									q.word=v;
									uint8	c;
									c=q.c[0];q.c[0]=q.c[3];q.c[3]=c;
									c=q.c[1];q.c[1]=q.c[2];q.c[2]=c;
									return	q.word;
								}
};

class	cmp_uint64
{
public:
	uint64	val;
	cmp_uint64()		{	}
	cmp_uint64(int iu)	{	val=iu;}
	inline	int	cmp(const cmp_uint64*op)	const	{	if(val>op->val)	return	1;
										if(val<op->val)	return	-1;
										return	0;
									}
	inline	uint64 static swap(const uint64 v)	{	uX32	q;
									q.word=v;
									uint8	c;
									c=q.c[0];q.c[0]=q.c[3];q.c[3]=c;
									c=q.c[1];q.c[1]=q.c[2];q.c[2]=c;
									return	q.word;
								}
};


bool	operator==(const cmp_uint16&a,const cmp_uint16&b);
bool	operator==(const cmp_uint32&a,const cmp_uint32&b);

#endif
