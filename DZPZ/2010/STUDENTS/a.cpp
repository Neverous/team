/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int eggs,
	size,
	tests,
	speedup[10010][16];

int solve(int size, int eggs);
int MAX(int a, int b);
int MIN(int a, int b);

int main(void)
{
	for(int s = 0; s < 10010; ++ s)
		for(int e = 0; e < 16; ++ e)
			speedup[s][e] = 1000000;

	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%d %d", &eggs, &size);
		printf("%d\n", solve(size, eggs));
	}

	return 0;
}

int solve(int size, int eggs)
{
	if(size == 1)
		return 1;

	if(!size || !eggs)
		return 0;

	if(eggs == 1)
		return speedup[size][eggs] = size;

	if(speedup[size][eggs] != 1000000)
		return speedup[size][eggs];

	for(int s = 1; s < size / 2 + 5 && s < size; ++ s)
		speedup[size][eggs] = MIN(speedup[size][eggs], MAX(solve(s - 1, eggs - 1), solve(size - s, eggs)) + 1);

	return speedup[size][eggs];
}

int MAX(int a, int b)
{
	return a>b?a:b;
}

int MIN(int a, int b)
{
	return a<b?a:b;
}
