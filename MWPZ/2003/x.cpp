/* 2011
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int tests,
	numbers,
	number,
	result;

int main(void)
{
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%d", &numbers);
		result = 0;
		for(int n = 0; n < numbers; ++ n)
		{
			scanf("%d", &number);
			result += number;
		}

		printf("%d\n", result);
	}

	return 0;
}

