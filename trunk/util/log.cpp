#define	LOG_INTERNAL
#include	"log.h"
#include	<time.h>

#ifdef	NEED_PCAP
#include <pcap.h>
//	for pcap errors
char	errbuf[PCAP_ERRBUF_SIZE];
#endif

//int	log_level;


#define	TIMEBUFSIZE	64
char	timebuf[TIMEBUFSIZE];

const char*	log_time()
{
	time_t	now;
	struct	tm	loc_t;
	time(&now);
	localtime_r(&now,&loc_t);
	strftime(timebuf,TIMEBUFSIZE,"%F %T",&loc_t);
	return	timebuf;
}
