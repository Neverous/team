/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<ctime>
#include<cstdlib>
#include<cmath>
#define TRIES 4
#define RANDOMIZE 8
#define bones(x) ((((x) < (1LLU << 32))?32:64) - __builtin_clz(x))

int tests;
long long unsigned int test;

inline long long unsigned int random(long long unsigned int start, long long unsigned int end);
inline bool isPrime(long long unsigned int &number);
inline bool isComposite(long long unsigned int base, long long unsigned int number);
long long unsigned int fastPower(long long unsigned int b, long long unsigned int p, long long unsigned int MOD);
long long unsigned int multiply(long long unsigned int a, long long unsigned int b, long long unsigned int MOD);

int main(void)
{
	srand(time(0));
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%llu", &test);
		printf("%sprime\n", isPrime(test)?"":"non-");
	}

	return 0;
}

inline bool isPrime(long long unsigned int &number)
{
	if(number < 2)
		return false;

	if(number < 4)
		return true;

	if(number % 2 == 0)
		return false;

	long long unsigned int base;
	for(int t = 0; t < TRIES; ++ t)
	{
		base = random(2, number - 2);
		if(isComposite(base, number))
			return false;
	}

	return true;
}

inline bool isComposite(long long unsigned int base, long long unsigned int number)
{
	if(number < 2)
		return false;

	if(number % 2 == 0)
		return true;

	unsigned int power = 0;
	long long unsigned int temp = number - 1,
		 act = 0;
	while(temp % 2 == 0 && ++ power)
		temp /= 2;

	act = fastPower(base, temp, number);
	if(act == 1 || act == number - 1)
		return false;

	for(unsigned int r = 1; r < power; ++ r)
	{
		act = multiply(act, act, number);
		if(act == 1)
			return true;

		if(act == number - 1)
			return false;
	}

	return true;
}

long long unsigned int fastPower(long long unsigned int b, long long unsigned int p, long long unsigned int MOD)
{
	if(p == 0 || b == 1)
		return 1;

	long long unsigned int result = fastPower(b, p / 2, MOD);
	result = multiply(result, result, MOD);

	if(p % 2)
		result = multiply(result, b, MOD);

	return result;
}

inline long long unsigned int random(long long unsigned int start, long long unsigned int end)
{
	if(start == end)
		return start;

	long long unsigned int result = 0;
	for(int r = 0; r < RANDOMIZE; ++ r)
	{
		result <<= 64 / RANDOMIZE;
		result |= random() % (1LLU << (64 / RANDOMIZE));
	}

	return start + (result % (end - start));
}

long long unsigned int multiply(long long unsigned int a, long long unsigned int b, long long unsigned int MOD)
{
	if(bones(a) + bones(b) < 64)
		return (a * b) % MOD;

	long long unsigned int result = 0;
	for(long long unsigned int p = 1; p <= b; p <<= 1)
	{
		if(b & p)
			result = (result + a) % MOD;

		a = (a <<  1) % MOD;
	}

	return result;
}
