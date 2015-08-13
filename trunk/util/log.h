#ifndef	ARS_LOG_H
#define	ARS_LOG_H

#define	MAXSTR	256

#include	<assert.h>
#include	<errno.h>
#include	<string.h>
//#include	<pcap.h>
#include	"port/portability.h"

#define	xinet_ntoa(ADDR)	inet_ntoa(*(in_addr*)&ADDR)
#define	xether_ntoa(ADDR)	ether_ntoa((ether_addr*)ADDR)

#define	NLOG_LEVELS	8

#ifdef	LOG_INTERNAL
#define	DECLARE_STATIC(A,B...)	A = B;
#else
#define	DECLARE_STATIC(A,B...)	extern		A;
#endif

// #pragma	poison	strncpy

extern	int	log_level;
//extern	char	errbuf[PCAP_ERRBUF_SIZE];

// CSI n [;k] m
// \x1b

//	yes, i do love ANSI X3.64 ;)
#define	ANSI_ESC	"\x1B[1;"
#define	ANSI_COLOR(IDX)	ANSI_ESC #IDX "m"

//	cursor control
#define	_ANSI_CRSR_UP(N)	ANSI_ESC #N "A"
#define	_ANSI_CRSR_DOWN(N)	ANSI_ESC #N "B"
#define	_ANSI_CRSR_LEFT(N)	ANSI_ESC #N "D"
#define	_ANSI_CRSR_RIGHT(N)	ANSI_ESC #N "C"

#define	ANSI_CRSR_UP1		_ANSI_CRSR_UP(1)
#define	ANSI_CRSR_DOWN1	_ANSI_CRSR_DOWN(1)
#define	ANSI_CRSR_LEFT1	_ANSI_CRSR_LEFT(1)
#define	ANSI_CRSR_RIGHT1	_ANSI_CRSR_RIGHT(1)

#define	DUP(A)	A	A
#define	ANSI_CRSR_UP2		DUP(ANSI_CRSR_UP1)
#define	ANSI_CRSR_DOWN2	DUP(ANSI_CRSR_DOWN1)
#define	ANSI_CRSR_LEFT2	DUP(ANSI_CRSR_LEFT1)
#define	ANSI_CRSR_RIGHT2	DUP(ANSI_CRSR_RIGHT1)
#define	ANSI_CRSR_UP4		DUP(ANSI_CRSR_UP2)
#define	ANSI_CRSR_DOWN4	DUP(ANSI_CRSR_DOWN2)
#define	ANSI_CRSR_LEFT4	DUP(ANSI_CRSR_LEFT2)
#define	ANSI_CRSR_RIGHT4	DUP(ANSI_CRSR_RIGHT2)

#define	ANSI_CRSR_RIGHT8	DUP(ANSI_CRSR_RIGHT4)


// #define	ANSI_CURSOR_POS()

#define	ANSI_CLR(MODE)		ANSI_ESC #MODE "K"

#define	ARESET	ANSI_COLOR(0)
#define	ARED		ANSI_COLOR(31)
#define	AYEL		ANSI_COLOR(33)
#define	AGRE		ANSI_COLOR(32)
#define	ACYA		ANSI_COLOR(36)


#define	HAVE_ANSI_SUPPORT

DECLARE_STATIC(	const	char*log_levels_hr0[NLOG_LEVELS],	{"CRI",	"ERR",	"WRN",	"MSG",	"D01",	"D02",	"D03",	"D04"	}	);

#ifdef	HAVE_ANSI_SUPPORT
DECLARE_STATIC( const	char*log_levels_hr[NLOG_LEVELS],	{AYEL "CRI" ARESET,	ARED "ERR" ARESET,	ACYA "WRN" ARESET,	/*AGRE*/ "MSG" /*ARESET*/,	
"D01",	"D02",	"D03",	"D04"	});
#define	CLR_LINE_PREFIX	ANSI_CLR(2) "\r"
#else
#define	CLR_LINE_PREFIX	"\r"

DECLARE_STATIC(	const	char*log_levels_hr[NLOG_LEVELS],	{"CRI",	"ERR",	"WRN",	"MSG",	"D01",	"D02",	"D03",	"D04"	}	)

#endif
enum	log_levels{
CRI,		ERR,		WRN,		MSG,		D01,		D02,		D03,		D04		};

const char*	log_time();

#define	LOG(LEVEL,FORMAT,B...)																	\
								{															\
									assert(LEVEL>=0 && LEVEL<NLOG_LEVELS);							\
									if(LEVEL < log_level)										\
									printf("%s[%s] %s %s:%d	" FORMAT "\n",		CLR_LINE_PREFIX,		\
											log_levels_hr[LEVEL],								\
												log_time(),									\
												__FILE__,__LINE__,								\
														##B);								\
								}


#define	SLOG(LEVEL,FORMAT,B...)																	\
								{															\
									assert(LEVEL>=0 && LEVEL<NLOG_LEVELS);							\
									if(LEVEL < log_level)										\
									printf("%s[%s] %s %s:%d	" FORMAT "",		CLR_LINE_PREFIX,		\
											log_levels_hr[LEVEL],								\
											log_time(),										\
												__FILE__,__LINE__,								\
														##B);								\
								}


//	error handling basics - for custom expressions.

							
#define	XCHKX(EXPR,	FAILURE_ACTION,LEVEL,	STR,B...)				\
							if(!(EXPR))							\
							{								\
								LOG(LEVEL, STR " (" #EXPR ")",##B)	\
								FAILURE_ACTION					\
							}

#define	XCHK(EXPR,	LEVEL,	STR,B...)				\
		XCHKX(EXPR,	;,LEVEL,STR,##B)

#define	XCHKL(EXPR,	FAILURE_LABEL,	LEVEL,	STR,B...)	\
		XCHKX(EXPR,	goto FAILURE_LABEL;,LEVEL,STR,##B)
							
#define	XCHKR(EXPR,	RETURN_VALUE,	LEVEL,	STR,B...)	\
		XCHKX(EXPR,	return RETURN_VALUE;,LEVEL,STR,##B)

//	handle posix.1 'errno' style errors the same way..
#define	XERRNOX(EXPR,	FAILURE_ACTION,LEVEL,	STR,B...)						\
							if(errno)	{LOG(CRI,"errno is already set!");errno=0;}		\
							{EXPR;}									\
							if(errno)									\
							{										\
								LOG(LEVEL, STR "\n *** errno(%d:%s)\n *** expr (" #EXPR ")",	\
										##B,	errno,strerror(errno)		\
															)		\
								errno=0;								\
								FAILURE_ACTION							\
							}

#define	XERRNO(EXPR,	LEVEL,	STR,B...)	\
		XERRNOX(EXPR,	;,LEVEL,STR,##B)

#define	XERRNOL(EXPR,	FAILURE_LABEL,	LEVEL,	STR,B...)	\
		XERRNOX(EXPR,	goto FAILURE_LABEL;,LEVEL,STR,##B)
							
#define	XERRNOR(EXPR,	RETURN_VALUE,	LEVEL,	STR,B...)	\
		XERRNOX(EXPR,	return RETURN_VALUE;,LEVEL,STR,##B)
		
//	handle pcap errors
#define	XPCAPX(EXPR,	FAILURE_ACTION,LEVEL,	STR,B...)						\
							{EXPR;}									\
							if(errno)									\
							{										\
								LOG(LEVEL, STR "\n *** libpcap errbuf(%s)\n *** expr (" #EXPR ")",	\
										##B,	errbuf		\
															)		\
								errno=0;								\
								FAILURE_ACTION							\
							}

#define	XPCAPL(EXPR,	FAILURE_LABEL,	LEVEL,	STR,B...)	\
		XPCAPX(EXPR,	goto FAILURE_LABEL;,LEVEL,STR,##B)
							
#define	XPCAPR(EXPR,	RETURN_VALUE,	LEVEL,	STR,B...)	\
		XPCAPX(EXPR,	return RETURN_VALUE;,LEVEL,STR,##B)


//	handle mysql errors
#define	XMYSQLX(EXPR,	FAILURE_ACTION,LEVEL,	STR,B...)						\
							{EXPR;}									\
							if(mysql_errno(&dbase))						\
							{										\
								LOG(LEVEL, STR "\n *** mysqlerror: %s\n *** expr (" #EXPR ")",	\
										##B,	mysql_error(&dbase)			\
															)		\
								errno=0;								\
								FAILURE_ACTION							\
							}

#define	XMYSQLL(EXPR,	FAILURE_LABEL,	LEVEL,	STR,B...)	\
		XMYSQLX(EXPR,	goto FAILURE_LABEL;,LEVEL,STR,##B)
							
#define	XMYSQLR(EXPR,	RETURN_VALUE,	LEVEL,	STR,B...)	\
		XMYSQLX(EXPR,	return RETURN_VALUE;,LEVEL,STR,##B)


#define	DEF_INTERFACE_ROUTINE		DEF_INTERFACE_ROUTINE_X()
#define	DEF_INTERFACE_ROUTINE_R0		DEF_INTERFACE_ROUTINE_X(return 0)
#define	DEF_INTERFACE_ROUTINE_R1		DEF_INTERFACE_ROUTINE_X(return 1)
#define	DEF_INTERFACE_ROUTINE_X(RET)	{	LOG(CRI,"interface routine triggered:%s",__PRETTY_FUNCTION__);RET;	}


#define	IFREE(A)	if(A)		\
				{			\
					free(A);	\
					A=0;		\
				}

//#define	XCHKR(EXPR,				STR,	...)
// 							if(!(EXPR))
// 							{
// 							{	LOG(D01,"failure: call: >" #COND "< result:%s",streerror(errno));		
// 								errno=0;		/*clear error*/									
// 								goto LABEL;													
// 							}
// 
// //	fast and verbose error handling thru a small define ;)
// #define	XERRX(COND,FAIL_EXPR)	{	if(!(COND))													
// 							{	LOG(D01,"failure: call: >" #COND "< result:%s",streerror(errno));		
// 								errno=0;		/*clear error*/									
// 								goto LABEL;													
// 							}
// 
// //	execute/check errno[failure->goto label]
// #define	XCHK(EXPR,LABEL)	{	EXPR;	
// 							if(errno)	
// 							{	LOG(D01,"failed system call: >" #EXPR "< result:%s",streerror(errno));	
// 								errno=0;		/*clear error*/									
// 								goto LABEL;													
// 							}



#endif
