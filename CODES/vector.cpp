/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  Policz iloczyn wektorowy
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

long long int a[3],
			  b[3],
			  r[3];

int main(void)
{
	for(int s = 0; s < 3; ++ s)
		scanf("%lld", &a[s]);

	for(int s = 0; s < 3; ++ s)
		scanf("%lld", &b[s]);

	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];

	printf("%lld %lld %lld\n", r[0], r[1], r[2]);
	return 0;
}

