#include "Comparisons.h"
#include "Lines.h"


int DirectComparisonForLines(const void* a, const void* b)
{
	assert(a != NULL);
	assert(b != NULL);
	
	const Line fst  = *(const Line*)a;
	const Line snd  = *(const Line*)b;

	
	int i = 0;
	int j = 0;

	while(i < fst.length && j < snd.length)
	{
		if (isletter(fst.start[i])) ; else { i++; continue;}
		if (isletter(snd.start[j])) ; else { j++; continue;}
		
		unsigned char a = toLower(fst.start[i]);
		unsigned char b = toLower(snd.start[j]);

		if(a != b)  return  (a - b);
		i++;
		j++;
	}

	return (i - j);
}


int ReverseComparisonForLines(const void* a, const void* b)
{
	assert(a != NULL);
	assert(b != NULL);

	const Line fst  = *(const Line*)a;
	const Line snd  = *(const Line*)b;

	int i = 1;
	int j = 1;

	while(i <= fst.length && j <= snd.length)
	{
		if (isletter(fst.finish[-i])) ; else { i++; continue;}
		if (isletter(snd.finish[-j])) ; else { j++; continue;}

		if (fst.finish[-i] != snd.finish[-j]) return (fst.finish[-i] - snd.finish[-j]);
		
		i++;
		j++;
	}
	
	return (i - j);
}