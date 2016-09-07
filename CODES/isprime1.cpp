/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)
#define SIZE 1000000

int tests;
long long int test,
			  prime[83000],
			  primes;
bool composite[SIZE];

inline void sieve(void);
inline bool isPrime(long long int &number);

int main(void)
{
	sieve();
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%lld", &test);
		printf("%s\n", isPrime(test)?"YES":"NO");
	}

	return 0;
}

inline bool isPrime(long long int &number)
{
	if(number < SIZE)
		return !composite[number];

	for(int p = 0; p < primes && prime[p] * prime[p] <= number; ++ p)
		if(number % prime[p] == 0)
			return false;

	return true;
}

inline void sieve(void)
{
	for(long long int p = 2, pp = 0; p < SIZE; ++ p)
	{
		if(composite[p])
			continue;

		++ primes;
		prime[pp ++] = p;

		for(long long int c = p * p; c < SIZE; c += p)
			composite[c] = true;
	}
}

