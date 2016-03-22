/* 2011
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int tests,
	people,
	box,
	time,
	result;

int main(void)
{
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%d %d", &people, &box);
		result = 0;
		for(int p = 0; p < people; ++ p)
		{
			scanf("%d", &time);
			result += 24*60*60 / time;
		}

		printf("%d\n", (result + box - 1) / box);
	}

	return 0;
}

