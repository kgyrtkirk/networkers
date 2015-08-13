
#include "port/portability.h"
#include "log.h"
#include	<stdio.h>
#include	<stdlib.h>
#include "list.h"
#include "as/datasource.h"
#include "sentry/arpentry.h"
#include "as/as_file.h"

int log_level=0xff;
struct	test_struct
{
	int		a;
	float	b;
};

int	tprint(test_struct*a,int i)
{
	LOG(MSG,"[%d]	a.i	%d",i,a->a);
}
struct	cmp_uint32
{
	uint32	a;
	int	cmp(struct cmp_uint32*b)	{return	a-b->a;}
}	asd;

  	cList<sPRecord>									li;
  	cOffsetList<sPRecord,cmp_uint32,OFFSETOF(sPRecord,ip)>		li2;

int	tchk(sPRecord*a,int i)
{
//	LOG(MSG,"[%d]	%08x %08x 	%s	%x %x %x %x %x %x",i,a,a->ip,xinet_ntoa(a->ip),a->mac[0],a->mac[1],a->mac[2],a->mac[3],a->mac[4],a->mac[5]);
	asd.a=a->ip;
	sPRecord*ptr;
	ptr=li2.search(&asd);
	LOG(MSG,"[%d]	%08x	%08x	%08x	%s %s",i,a,ptr,a->ip,ptr?"found":"N/A",(ptr==a)?"ok":"fail");
}
	static	int	lip=0;

int	tprint2(sPRecord*a,int i)
{
	
//	LOG(MSG,"[%d]	%08x %08x 	%s	%x %x %x %x %x %x",i,a,a->ip,xinet_ntoa(a->ip),a->mac[0],a->mac[1],a->mac[2],a->mac[3],a->mac[4],a->mac[5]);
	LOG(MSG,"[%d]	%s	%08x %08x %s	%d	%x %x %x %x %x %x",i,(a->ip>lip)?"OK":"FAIL",a,a->ip,xinet_ntoa(a->ip),a->ip,
												a->mac[0],a->mac[1],a->mac[2],a->mac[3],a->mac[4],a->mac[5]
													);
	lip=a->ip;
}

int	main()
{
#define	TLEN	32

// 	cList<test_struct>	li;
// 	test_struct	a[TLEN];
// 	test_struct	b[TLEN];
// 	int	i;
// 	for(i=0;i<TLEN;i++)
// 	{
// 		a[i].a=800+i;
// 		li.add(a+i);
// 	}
// 	li.iterate(0,(list_iterator*)&tprint);
// 	
// 	for(i=0;i<TLEN;i++)
// 	{
// 		LOG(ERR,"%d",i);
// //		a[i].a=i;
// 		li.del(a+i);
// 		if(i%3)
// 			i++;
// 	}
// // 	li.vacuum(1);
// 	li.iterate(0,(list_iterator*)&tprint);
// 	for(i=0;i<TLEN;i++)
// 	{
// 		b[i].a=500+i;
// 		li.add(b+i);
// 	}
// 	li.iterate(0,(list_iterator*)&tprint);


	printf("asd	%d	%d\n",__builtin_offsetof(sPRecord,ip),sizeof(sPRecord));
// 	cList<sPRecord>		li;
	li.add_key(&li2);
// 	cDataSource<sPRecord>	ds;
//	ds.scanner=new cTabbedFileScanner("asd.list");
//	strcpy(((cBasicFileScanner*)ds.scanner)->file,"asd.list");
//	ds.target=&li;
	
	cRandomDataSource<sPRecord>	ds(67,&li);

//	ds.init();
	ds.sync();
	
	

//	li.sort();
//	asd.a=0x0beb4298;
//	printf("%08x\n",li.search(&asd));
 	li.iterate(0,(list_iterator*)&tprint2);
 	li.iterate(0,(list_iterator*)&tchk);
 	li.iterate(0,(list_iterator*)&tprint2);
 	lip=0;
 	li2.iterate(0,(list_iterator*)&tprint2);
	
}
