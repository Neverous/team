/* 2011
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int tests,
	measurements,
	measure;
bool res[2][10001];

inline static const int ABS(const int &a){return a<0?-a:a;}

int main(void)
{
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%d", &measurements);
		for(int s = 0; s <= 10000; ++ s)
			res[0][s] = false;

		res[0][0] = true;
		for(int m = 0; m < measurements; ++ m)
		{
			scanf("%d", &measure);
			for(int s = 0; s <= 10000; ++ s)
				res[(m + 1) & 1][s] = false;

			for(int s = 0; s <= 10000; ++ s)
				if(res[m & 1][s])
					res[(m + 1) & 1][ABS(s - measure)] |= res[(m + 1) & 1][s + measure] |= res[m & 1][s];
		}

		for(int s = 0; s <= 10000; ++ s)
			if(res[measurements & 1][s])
			{
				printf("%d ", s);
				break;
			}

		for(int s = 10000; s >= 0; -- s)
			if(res[measurements & 1][s])
			{
				printf("%d\n", s);
				break;
			}
		
	}

	return 0;
}

