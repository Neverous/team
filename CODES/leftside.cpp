/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  Sprawdź czy punkt leży na lewo od odcinka(2d)
 */
#include<cstdio>
#include<cmath>

long long int a[2],
			  b[2],
			  c[2];

int main(void)
{
	for(int s = 0; s < 2; ++ s)
		scanf("%lld", &a[s]);

	for(int s = 0; s < 2; ++ s)
		scanf("%lld", &b[s]);

	for(int s = 0; s < 2; ++ s)
		scanf("%lld", &c[s]);

	puts((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]) > 0?"TAK":"NIE");
	return 0;
}

