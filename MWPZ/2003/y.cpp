/* 2011
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>
#include<algorithm>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int tests,
	people,
	min[131027],
	result;

int main(void)
{
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%d", &people);
		for(int p = 0; p < people; ++ p)
			scanf("%d", &min[p]);

		std::sort(min, min + people);
		result = 0;
		while(result < people && result >= min[result])
			++ result;

		printf("%d\n", result);
	}

	return 0;
}

