#ifndef	LIST_H
#define	LIST_H

#define	LIST_EXPAND_STEP_SIZE	32
#include	"log.h"

#include <math.h>


//TODO	rename cList to something else...	cSet ?
//TODO	rename cOffsetList to cIndex
//TODO	remove those long comments? :)


typedef int	list_iterator(void*e,int idx);

template<class XT>
class	cListCursor;

template<class XT,class XE>
class	cSelectorCursor;

template<class T,class E,size_t O>
class	cIndexCursor;


template<class T>
class	cList
{
public:
	cList();
	virtual	~cList();
	int		add(T*e);
	T*		del(T*e);
	void		vacuum(int full);									//!<	deprecated? sort is called instead of vacuum - should be rename to 'sort' or sort to vacuum, and move current sorter to coffsetlist, or to an intermediate: cList
	virtual	void		sort();									//!<	implicates a vacuum alike effect
	void		iterate(int iterator_flags,list_iterator*call);			//!<	deprecate	this shit! - arpsentry is using it
	void		free_contents();									//!<	call free for every element in list + empty
	void		delete_contents();									//!<	call delete for every element in list + empty
	void		add_key(cList<T>*_list);								//!<	adds \param _list as a key..all add/rem operations are forwarded - doubles storage, but the encapsulated list can be ordered
	int		exact_search(const T*e);								//!<	return storage index...useless for real clients
	T*		search(const T*e);									//!<	content search	O(n) - not recommended ;)
	const int&getsize()			const	{	return	ncontent;}	
	
	void		dump();
	void		empty();
//	i can't make this work...
// 	template<class E,int offset>
// 	T*		offset_search_function(E*e);
// 	int		idx_search(const E*e)	{}

protected:
	T*		del_at(int idx);

	T**		content;		//!<	ptrs to the encapsulated data
	int		ncontent;		//!<	cache used ptr num
	int		mcontent;		//!<	total allocated ptrs
	int		first_free;	//!<	maybe 'free'
	enum	list_flags
	{
		list_ordered=	0x1
	};
	int		flags;		//!<	flags: ordered

private:
	cList<T>	**nested;		//!<	support 1 nested list(key) - for now
	int		nnested;		//!<	support 1 nested list(key) - for now
	
	
	template<class XT,class E,int offset>
	friend	XT*	offset_search_function(cList<XT>*list,const E*e);
	template<class XT,class E,int offset>
	friend	int	idx_offset_search_function(cList<XT>*list,const E*e);
	
	template<class XT,class E,int offset>
	friend	int	idx_offset_search_function(cList<XT>*list,const E*e,int lo,int hi);

	
	friend	class	cListCursor<T>;
};

template<class	T>
class	iListCursor
{
public:
	virtual	T*	next()=0;
	virtual	void	reset()=0;
};
template<class T>
class	cListCursor
{
public:
	cListCursor(cList<T>*_list)	:	list(_list)
	{
		XCHK(_list > (void*)0x1000,	CRI,	"i tell you! something really sucks...i'm gone fishin', good luck buddy!");
		assert(_list > (void*)0x1000);
		reset();
	}
	T*	next();
	void	reset();
	void	del_current();
	int	getidx()	const;
protected:
	cList<T>	*list;
	T*		curr;
	int		curr_idx;
};

template<class T,class E>
class	cSelectorCursor	:	public	cListCursor<T>
{
public:
	cSelectorCursor(cList<T>*_list,E*_crit)	:	cListCursor<T>(_list),crit(_crit)	{	cListCursor<T>::reset();	}
	T*	next();

protected:
	cList<T>	*list;
	T*		curr;
	E*		crit;
	int		curr_idx;
};

template<class T>
int	cListCursor<T>::getidx()	const
{
	return	curr_idx;
}

template<class T>
T*	cListCursor<T>::next()
{
	for(curr_idx++;curr_idx<list->mcontent && !(list->content[curr_idx]);curr_idx++);
	if(curr_idx>=list->mcontent)
		curr=0;
	else	curr=list->content[curr_idx];
	return	curr;
}

template<class T,class E>
T*	cSelectorCursor<T,E>::next()
{
	while(cListCursor<T>::next() && !crit->match(cListCursor<T>::curr));
	return	cListCursor<T>::curr;
}



template<class T>
void	cListCursor<T>::reset()
{
	curr_idx=-1;
	curr=0;
}

template<class T>
void	cListCursor<T>::del_current()
{
	list->del_at(curr_idx);
	curr=0;
}

template<class T,class E,int offset>
T*	offset_search_function(cList<T>*list,const E*e);


// class	iOfssetList	: public	cList<T>
// {
// 	int	idx_search(const E*e);
// }


template<class T,class E,int offset>
class	cOffsetList	: public	cList<T>
{
protected:
	struct sLogHash{
		E	key_val;
		int	idx;
	}	*lhash;
	int	nlhash;
public:
	cOffsetList()	:	cList<T>()	{nlhash=0;lhash=0;}
	T*	search(const E*e);
	int	idx_search(const E*e);
	const	E&	idx_val(const size_t idx)	const;
// 	cListCursor<T>*	
	void	sort();
	void	enable_lhash(size_t size);
// 	void	lhash_experiment();
	
	friend	class	cIndexCursor<T,E,offset>;

};

// template<class T,class E,int offset>
// void	cOffsetList::lhash_experiment()
// {
// 	
// }


#define	PAGE_SIZE	4096
template<class T,class E,int offset>
void	cOffsetList<T,E,offset>::enable_lhash(size_t size)
{
	if(size==0)
	{
		size=(int)(2*logf((float)((cList<T>::ncontent*sizeof(T))/PAGE_SIZE)/log(2)));
	}
	IFREE(lhash);
	nlhash=size;
	if(size<2)	//	leave disabled
		return;
	lhash=xmalloc(sLogHash,size);
	int	i;
//	int	bl=cList<T>::ncontent/(size-1);
	int	idx;
	for(i=0;i<size;i++)
	{
		if(i==size-1)
			idx=cList<T>::ncontent-1;
		else	idx=(cList<T>::ncontent*i/(size-1));	//	this is expensive, but it's worth to be precise in this case
		lhash[i].key_val=idx_val(idx);
		lhash[i].idx=idx;
	}
}

template<class T,class E,int offset>
const E&	cOffsetList<T,E,offset>::idx_val(const size_t i) const
{
#define	OMAP(V)	((E*)(((char*)V)+offset))
#define	CI	(*(this->content+i))
	return	*(OMAP(CI));
#undef	CI
#undef	OMAP
}

template<class T>
void		cList<T>::add_key(cList<T>*_list)
{
	XCHK(_list,	CRI,	"check calling function, it tries to supply a NULL list as my key");
	assert(_list);
	nested=xrealloc(nested,cList<T>*,nnested+1);
	nested[nnested++]=_list;
}

template<class T>
void		cList<T>::empty()
{
	memset(content,0,mcontent*sizeof(T*));
	first_free=ncontent=0;
	int	i;
	for(i=0;i<nnested;i++)
		nested[i]->empty();
// 	_list->nested=this;
}

template<class T>
void		cList<T>::free_contents()
{
	cListCursor<T>	c(this);
	T			*e;
	while(e=c.next())
		free(e);
	empty();
}

template<class T>
void		cList<T>::delete_contents()
{
	cListCursor<T>	c(this);
	T			*e;
	while(e=c.next())
		delete e;
	empty();
}

template<class T>
cList<T>::cList()
{
	content=0;
	ncontent=0;
	mcontent=0;
	first_free=0;
	nested=0;
	nnested=0;
}
template<class T>
cList<T>::~cList()
{
	IFREE(content);
	IFREE(nested);
}

template<class T>
int	cList<T>::add(T*e)
{

	if(ncontent==mcontent && (ncontent % LIST_EXPAND_STEP_SIZE) == 0)
	{
		content=(T**)realloc(content,sizeof(T*)*(mcontent+LIST_EXPAND_STEP_SIZE));
		memset(content+mcontent,0,sizeof(T*)*LIST_EXPAND_STEP_SIZE);
		mcontent+=LIST_EXPAND_STEP_SIZE;
	}
	if(content[first_free])
	{
		int	i=first_free;
		do{
			i++;
			i%=mcontent;
			if(!content[i])
				break;
		}while(i!=first_free);
		XCHKR(i!=first_free,	-1,	CRI,	"list invariant is broken");
		first_free=i;
	}
	content[first_free]=e;
	ncontent++;
	if(nested)
		for(int i=0;i<nnested;i++)
			nested[i]->add(e);

	flags&=	~list_ordered;
	return	ncontent;
}

#define	ROUNDUP(VAL,BOUNDARY)	(((VAL)/BOUNDARY)*BOUNDARY)

template<class T>
T*	cList<T>::del(T*e)
{
	int	idx;
	XCHKR(	(idx=exact_search(e))	>=0,
			0,	ERR,	"can't remove, not in list!");
	
	flags&=~list_ordered;
	
	if(nested)
		for(int i=0;i<nnested;i++)
			nested[i]->del(e);
	return	del_at(idx);
//	return	rem;
}

template<class T>
T*	cList<T>::del_at(int idx)
{
	T*	rem;
	rem=content[idx];
	content[idx]=0;
	ncontent--;
	first_free=idx;
	return	rem;
}

template<class T>
int	cList<T>::exact_search(const T*e)	
{
	int	i;
	for(i=0;i<mcontent;i++)
		if(content[i]==e)
			return	i;
	return	-1;	
}

template<class T>
T*	cList<T>::search(const T*e)
{
	return	offset_search_function<T,0>(e);
}

template<class T>
void	cList<T>::vacuum(int full)
{
	int	i,j;
	j=0;
	for(i=0;i<mcontent;i++)
	{
		if(content[i])
		{
			if(j)
				content[i-j]=content[i];
		}
		else
			j++;
	}
	assert(j+ncontent == mcontent);
	if(full)
	{
		if(mcontent-ncontent>LIST_EXPAND_STEP_SIZE)
		{
			mcontent=ROUNDUP(ncontent+1,LIST_EXPAND_STEP_SIZE);
			content=(T**)realloc(content,sizeof(T*)*mcontent);
			first_free=ncontent;
			j=mcontent-ncontent;
		}
	}
	memset(content+ncontent,0,j*sizeof(T*));
}


//FIXME DEPRECATE THIS
template<class T,const int offset>
inline	int	list_compare(const void*__lop,const void*__rop)
{
	const T*_lop=*((T**)__lop);
	const T*_rop=*((T**)__rop);
	if(!_lop && !_rop)	return	 0;
	if(_rop==0	)	return	-1;
	if(_lop==0)		return	 1;
	_lop=(T*)(((char*)_lop)+offset);
	_rop=(T*)(((char*)_rop)+offset);
	return	((const  T*)_lop)->cmp((const T*)_rop);
}

template<class T,const int offset>
inline	int	direct_list_compare(const void*__lop,const void*__rop)
{
	const T*_lop=((T*)__lop);
	const T*_rop=((T*)__rop);
	if(!_lop && !_rop)	return	 0;
	if(_rop==0	)	return	-1;
	if(_lop==0)		return	 1;
	_lop=(T*)(((char*)_lop)+offset);
	_rop=(T*)(((char*)_rop)+offset);
	return	((const  T*)_lop)->cmp((const T*)_rop);
}


template<class T>
void	cList<T>::sort()
{
// 	printf("offsetlist_sort");
	qsort(content,mcontent,sizeof(T*),&(list_compare<T,0>));
	flags|=list_ordered;
}

template<class T,class E,int offset>
void	cOffsetList<T,E,offset>::sort()
{
//	LOG(D_08,"offsetlist_sort");
//	printf("offsetlist_sort");
	qsort(	cList<T>::content,
			cList<T>::mcontent,
			sizeof(T*),
			&(list_compare<E,offset>));
	cList<T>::flags|=cList<T>::list_ordered;
}



// template<class T,class E,int offset>
// int	idx_offset_search_function(cList<T>*list,const E*e)
// {
// 	if(!list->ncontent)	return	0;
// 	XCHKR(	e,	0,	CRI,	"e is null");
// 	if(!(list->flags & list->list_ordered))
// 		list->sort();
// // #define	VAL	(e)
// #define	VAL		((E*)(((char*)e)-offset))
// #define	OMAP(V)	((E*)(((char*)V)+offset))
// #define	CMP(A,B)	(OMAP(A))->cmp(OMAP(B))
// #define	CI	(*(list->content+i))
// #define	CJ	(*(list->content+j))
// #define	CK	(*(list->content+k))
// 	int	i,j,k;
// 	int	r;
// 	i=0;
// 	k=list->ncontent-1;
// 	if(CMP(VAL,CI) < 0 || 0 < CMP(VAL,CK))
// 		return	-1;
// 	while(k-i>0)
// 	{
// 		j=k+i;	j>>=1;
// 		r=CMP(VAL,CJ);
// 		r=SIGN(r);
// 		switch(r)
// 		{
// 			case	-1:	k=j-1;		break;
// 			case	 0:	return		j;
// 			case	 1:	i=j+1;		break;
// 			default:
// 				LOG(CRI,"fuck");
// 		}
// 	}
// 	if(k==i && !CMP(VAL,CI))
// 				return	i;
// 	return	-1;
// #undef	VAL
// #undef	OMAP
// #undef	CMP
// #undef	CI
// #undef	CJ
// #undef	CK
// 
// }

template<class T,class E,int offset>
int	idx_offset_search_function(cList<T>*list,const E*e,int lo,int hi)
{

	if(!list->ncontent)
		return	-1;
	XCHKR(	e,	0,	CRI,	"e is null");
	if(!(list->flags & list->list_ordered))
		list->sort();
// #define	VAL	(e)
#define	VAL		((E*)(((char*)e)-offset))
#define	OMAP(V)	((E*)(((char*)V)+offset))
#define	CMP(A,B)	(OMAP(A))->cmp(OMAP(B))
#define	CI	(*(list->content+i))
#define	CJ	(*(list->content+j))
#define	CK	(*(list->content+k))
	int	i,j,k;
	int	r;
	i=lo;	//0;
	k=hi;	//list->ncontent-1;
	if(CMP(VAL,CI) < 0 || 0 < CMP(VAL,CK))
		return	-1;
	while(k-i>0)
	{
		j=k+i;	j>>=1;
		r=CMP(VAL,CJ);
		r=SIGN(r);
		switch(r)
		{
			case	-1:	k=j-1;		break;
			case	 0:	return		j;
			case	 1:	i=j+1;		break;
			default:
				LOG(CRI,"fuck");
		}
	}
	if(k==i && !CMP(VAL,CI))
				return	i;
	return	-1;
#undef	VAL
#undef	OMAP
#undef	CMP
#undef	CI
#undef	CJ
#undef	CK

}

template<class T,class E,int offset>
T*	offset_search_function(cList<T>*list,const E*e)
{
	int	idx;
	idx=idx_offset_search_function<T,E,offset>(list,e,0,list->ncontent-1);
	if(idx<0)
		return	0;
	else	return	(*(list->content+idx));
}


template<class T>
void	cList<T>::iterate(int iterator_flags,list_iterator*call)
{
	int	i;
	for(i=0;i<mcontent;i++)
		if(content[i])
			(*call)(content[i],i);
}

template<class T,class E,int offset>
T*	cOffsetList<T,E,offset>::search(const E*e)
{
	return	offset_search_function<T,E,offset>(this,e);
}

// template<class T,class E,size_t offset>
// void	interval_search(const T*base,const E*e,int&lo,int&hi,int i,int k)
enum	el2SearchMode
{
	GREATEST_SMALLER,
	SMALLEST_GREATER,
};

template<class T,class E,size_t offset,el2SearchMode SM>
int	log2search(const T*base,const E*e,int i,int k)
{
	int	j;
	int	r;
	const	E*eq=(E*)((const char*)e-offset);
	while(k-i>1)
	{
		j=i+k;	j>>=1;
		r=direct_list_compare<E,offset>(base+j,e);
		switch(SM)
		{
			case	SMALLEST_GREATER:
					if(r<0)	i=j+1;
					else		k=j;
				break;
			case	GREATEST_SMALLER:
					if(r>0)	k=j-1;
					else		i=j;
				break;
		}
	}
	switch(SM)
	{
		case	SMALLEST_GREATER:
			return	k;
		case	GREATEST_SMALLER:
			return	i;
	}

}

template<class T,class E,int offset>
int	cOffsetList<T,E,offset>::idx_search(const E*e)
{
	if(!lhash)
		return	idx_offset_search_function<T,E,offset>(this,e,0,cList<T>::ncontent-1);
	
// 	sort();
	int	lo,hi;
	lo=0;
	hi=nlhash-1;
	if(lhash[0].key_val.cmp(e) > 0 || 0 > lhash[nlhash-1].key_val.cmp(e) )
		return	-1;

// 	interval_search<T,E,0>(lhash,e,lo,hi,0,nlhash-1);
// // 	printf("enter\n");
	lo=log2search<sLogHash,E,0,GREATEST_SMALLER>(lhash,e,0,nlhash-1);
// 	printf("enter2\n");
	hi=log2search<sLogHash,E,0,SMALLEST_GREATER>(lhash,e,0,nlhash-1);
	
// 	while(lo<nlhash-1 && lhash[lo+1].key_val.cmp(e) < 0)
// 		lo++;

	while(lo<nlhash-1 && lhash[lo+1].key_val.cmp(e) < 0)
		lo++;
	while(hi>0 && lhash[hi-1].key_val.cmp(e) > 0)
		hi--;
//	for(int i=0;i<nlhash;i++)
//		printf("	%d	% 4x	\n",i,(int)lhash[i].key_val.val);
// 	printf("%d	%d\n",lo,hi);
	
	lo=lhash[lo].idx;
	hi=lhash[hi].idx;
	return	idx_offset_search_function<T,E,offset>(this,e,lo,hi);


// 	printf("	%d	% 4x	\n",i,(int)lhash[i].key_val.val);


// 	return	-1;
// 	int	lo,hi,mi;
// 	int	r;
// 	lo=0;
// 	hi=nlhash;
// 	if(lhash[0].key_val.cmp(e) < 0 || 0 < lhash[lhash-1].key_val.cmp(e) )
// 		return	-1;
// 	while(hi-lo>0)
// 	{
// 		mi=(lo+hi);	mi>>=1;
// 		r=lhash[mi].key_val.cmp(e);
// 		r=SIGN(r);
// 		switch(e)
// 		{
// 			case	-1:	hi=mi-1;	break;
// 			case	0:
// 		}
// 		
// 	}


// 	if(CMP(VAL,CI) < 0 || 0 < CMP(VAL,CK))
// 		return	-1;
// 	while(k-i>0)
// 	{
// 		j=k+i;	j>>=1;
// 		r=CMP(VAL,CJ);
// 		r=SIGN(r);
// 		switch(r)
// 		{
// 			case	-1:	k=j-1;		break;
// 			case	 0:	return		j;
// 			case	 1:	i=j+1;		break;
// 			default:
// 				LOG(CRI,"fuck");
// 		}
// 	}
// 	if(k==i && !CMP(VAL,CI))
// 				return	i;
// 	return	-1;


}


template<class T>
void	cList<T>::dump()
{
	int	i;
	for(i=0;i<mcontent;i++)
		if(content[i])
			content[i]->print();
	
}


template<class T,class E,size_t O>
class	cIndexCursor	//:	public	cListCursor<T>
{
public:
	cIndexCursor(cOffsetList<T,E,O>*_list)										{	list=_list;		}
	cIndexCursor(cOffsetList<T,E,O>*_list,E	_idx_value)	:	idx_value(_idx_value)	{	list=_list;reset();	}
	void	reset();
	void	reset(E	_idx_value)			{	idx_value=_idx_value;	reset();	}

	T*	next();

protected:
	cOffsetList<T,E,O>	*list;
	T*		curr;
	E		idx_value;
	int		curr_idx;
};

template<class T,class E,size_t O>
void	cIndexCursor<T,E,O>::reset()
{
	curr_idx=list->idx_search(&idx_value);
	if(curr_idx!=-1)
	{
		while(curr_idx>0 && list->idx_val(curr_idx-1)==idx_value)
			curr_idx--;
//		curr=list->content[
	}
	curr=0;
}

template<class T,class E,size_t O>
T*	cIndexCursor<T,E,O>::next()
{
	if(curr)
	{
// 		printf("X");
// 		list->content[curr_idx+1]->print("q");
		if(curr_idx+1<list->ncontent && list->idx_val(curr_idx+1)==idx_value)
			curr_idx++;
		else	curr_idx=-1;
	}
 	if(curr_idx==-1)
 		return	0;
	curr=list->content[curr_idx];
	return	curr;
}



#endif
