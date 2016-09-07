/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  Policz iloczyn skalarny
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

long long int size,
			  vect[1048576],
			  temp,
			  result;

int main(void)
{
	scanf("%lld", &size);
	for(int s = 0; s < size; ++ s)
		scanf("%lld", &vect[s]);

	for(int s = 0; s < size; ++ s)
	{
		scanf("%lld", &temp);
		result += vect[s] * temp;
	}

	printf("%lld\n", result);
	return 0;
}

