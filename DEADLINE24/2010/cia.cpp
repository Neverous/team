/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

long long int first,
	second,
	need,
	mod,
	tests;

long long int fib(long long int st, long long int nd, long long int nth, long long int MOD);

int main(void)
{
	scanf("%lld %lld", &mod, &tests);
	for(long long int t = 0; t < tests; ++ t)
	{
		scanf("%lld %lld %lld", &first, &second, &need);
		printf("%lld\n", fib(first, second, need, mod));
	}

	return 0;
}

long long int fib(long long int st, long long int nd, long long int nth, long long int MOD)
{
	long long int tmp = 0;
	if(nth == 0)
		return st;

	if(nth == 1)
		return nd;

	while(nth)
	{
		tmp = nd;
		nd = (2 * st + nd) % MOD;
		st = tmp;
		-- nth;
	}

	return st;
	//return (2 * fib(st, nd, nth - 2, MOD) + fib(st, nd, nth - 1, MOD)) % MOD;
}
