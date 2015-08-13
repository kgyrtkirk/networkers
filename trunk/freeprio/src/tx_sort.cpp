#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>
#include	<time.h>

int	tx_sort(int*s,int n)
{
	int	i;
	int	j;
	for(i=0;i<n;i++)
	for(j=i+1;j<n;j++)
		if(s[i]>s[j])
		{
			s[i]^=s[j];
			s[j]^=s[i];
			s[i]^=s[j];
		}
}


int	tx_test()
{
#define	C	32
	int	x[C];
	int	i;
	srand(time(0));
	for(i=0;i<C;i++)
	{
		x[i]=rand()%100;
		printf("%d ",x[i]);
	}
	printf("\n");
	tx_sort(x,C);

	for(i=0;i<C;i++)
	{
		assert(i>=C-1 || x[i] <= x[i+1]);
		printf("%d ",x[i]);
	}
	printf("\n");

}
