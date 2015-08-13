#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>

#define	SIGN(A)	(A?((A>0)?1:-1):0)

#define	OFFSETOF(STRUCT,MEMBER)	__builtin_offsetof(STRUCT,MEMBER)
// #define	OFFSETOF(STRUCT,MEMBER)	reinterpret_cast<const int>(&((((STRUCT*)0x0))->MEMBER))
// //#define	OFFSETOF(STRUCT,MEMBER)	&((reinterpret_cast<STRUCT*>0x0)->MEMBER)


int			bitcount (unsigned int n);
unsigned int	rev32(register unsigned int x);

#define	xmalloc(TYPE,COUNT)			((TYPE*)malloc(sizeof(TYPE)*(COUNT)))
#define	xrealloc(PTR,TYPE,COUNT)		((TYPE*)realloc(PTR,sizeof(TYPE)*(COUNT)))
#define	setzero(PTR,TYPE,NUM)		memset(PTR,0,NUM);


#define	FAIL	1
#define	OK	0

#define	uint64	unsigned long long
#define	uint32	unsigned int
#define	uint16	unsigned short int
#define	uint8	unsigned char

#define	int64	long long
#define	int32	int
#define	int16	short int
#define	int8		char

#define	flt24	float
#define	flt32	float

#define	SETZERO(A)	memset(A,0,sizeof(A))

#ifdef  CXX_PACKET
#define _F(A)   A
#define PACKED  __attribute__((__packed__))
#else
#define _F(A)
#ifndef PACKED
#define PACKED
#endif
#endif
