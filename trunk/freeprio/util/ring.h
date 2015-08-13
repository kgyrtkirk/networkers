#include	"port/portability.h"
#include	<string.h>
#include	"log.h"

//!\file	ring.h	a very simple ring implementation

//!	operation modes
enum	eRingOperationMode{
	RING_FIFO,	//!<	standard first-in first-out
	RING_LIFO		//!<	stack-alike last-in first-out
};

//QUESTION	add template parameter for logging? :)
//			...it would be very straithforward

template<class T,size_t N,eRingOperationMode MODE>
class	cRing
{
public:
	cRing();
	int	push(T*);
	T*	pop();
	int	length()	const;
protected:
	int	first;
	int	last;
	int	nbuffer;
	T	*buffer[N];
};

template<class T,size_t N,eRingOperationMode MODE>
cRing<T,N,MODE>::cRing()
{
	first=last=nbuffer=0;
	setzero(buffer,T*,N);
}

template<class T,size_t N,eRingOperationMode MODE>
int	cRing<T,N,MODE>::push(T*ptr)
{
	if(nbuffer >= N)
	{
		LOG(ERR,"ring is full[%d]: ",nbuffer);
		return	FAIL;
	}
	if(++last>=N)
		last=0;
	buffer[last]=ptr;
	nbuffer++;
	return	0;
}

template<class T,size_t N,eRingOperationMode MODE>
T*	cRing<T,N,MODE>::pop()
{
	if( nbuffer <= 0 )
		return	0;
	int	i;
	switch(MODE)
	{
		case	RING_FIFO:
				i=first++;
				if(first==N)
					first=0;
				nbuffer--;
				return	buffer[i];
			break;
		case	RING_LIFO:
				last--;
				return	buffer[nbuffer--];
			break;
		default:
				LOG(ERR,"this line _should_ be optimized out by the compiler...");
			break;
	}
}

template<class T,size_t N,eRingOperationMode MODE>
int	cRing<T,N,MODE>::length()	const
{
	return	nbuffer;
}
