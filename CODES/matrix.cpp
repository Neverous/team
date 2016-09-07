/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  Policz wyznacznik macierzy 3x3
 */
#include<cstdio>
#include<cmath>

long long int a[3],
			  b[3],
			  c[3];

int main(void)
{
	for(int s = 0; s < 3; ++ s)
		scanf("%lld", &a[s]);

	for(int s = 0; s < 3; ++ s)
		scanf("%lld", &b[s]);

	for(int s = 0; s < 3; ++ s)
		scanf("%lld", &c[s]);

	printf("%lld\n", a[0] * b[1] * c[2] + a[1] * c[0] * b[2] + b[0] * c[1] * a[2] - b[1] * c[0] * a[2] - a[1] * b[0] * c[2] - b[2] * a[0] * c[1]);
	return 0;
}

