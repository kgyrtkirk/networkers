#ifndef	SET_H
#define	SET_H
#include	"port/portability.h"
#include	"util/log.h"

template<class T,size_t N>
class	cSet
{
public:
	T		*c[N];
	size_t	n;
	cSet()	:	n(0)	{};
	void	add(T*t)
	{
		if(n>=N)
		{
// 			LOG(CRI,"set overloaded: |S|>%d",n);
			return;
		}
		if(contains(t))
			return;
		c[n++]=t;
	}
	void	empty()
	{
		n=0;
	}
	void	remove(T*t)
	{
		for(size_t i=0;i<n;i++)
			if(c[i]==t)
			{
				for(size_t j=i;j<n-1;j++)
					c[j]=c[j+1];
				n--;
				return;
			}
	}
	bool	contains(T*t)
	{
		for(size_t i=0;i<n;i++)
			if(t==c[i])
				return	1;
		return	0;
	}
	cSet<T,N>		operator+(cSet<T,N>&b)
	{
		cSet	q(*this);
		for(size_t j=0;j<b.n;j++)
			q.add(b.c[j]);
		return	q;
//		cSet	*q=new cSet(*this);
//		for(size_t j=0;j<b.n;j++)
//			q->add(b.c[j]);
//		return	*q;
	}
	cSet<T,N>&	operator+=(T*t)
	{
		add(t);
		return	*this;
	}
	cSet<T,N>		operator-(cSet<T,N>&b)
	{
		cSet	q(*this);
		for(size_t j=0;j<b.n;j++)
			q.remove(b.c[j]);
		return	q;
//		cSet	*q=new cSet(*this);
//		for(size_t j=0;j<b.n;j++)
//			q->remove(b.c[j]);
//		return	*q;
	}
	cSet<T,N>		operator-(T*t)
	{
		cSet	q(*this);//=new cSet(*this);
		q.remove(t);
		return	q;
//		cSet	*q=new cSet(*this);
//		q->remove(t);
//		return	*q;
	}
	cSet<T,N>&	operator-=(T*t)
	{
		remove(t);
		return	*this;
	}
	T*	rand()
	{
		if(n<=0)
			return	0;
		return	c[::rand()%n];
	}
	bool	full()
	{
		return	n==N;
	}
	int	size()
	{
		return	n;
	}
};

template<class T,size_t N>
class	cSetCursor
{
	cSet<T,N>		&set;
	size_t		i;
public:
	cSetCursor(cSet<T,N>&_set)	:	set(_set),	i(0)	{}
	T*	next()	{	return	(i<set.n)?set.c[i++]:0;	}
	void	reset()	{	i=0;	}
};

class	Detectable;
typedef	cSet< Detectable,32 >		DSet;
typedef	cSetCursor<Detectable,32>	DCursor;

void	set_test();

// template<class T>
// T*	rand(cList<T>&src,T*e)
// {
// 	
// 
// }
// 
// 
// template<class T>
// T*	rand(cList<T>&src)
// {
// 	return	rand(src,0);
// }

#endif	
