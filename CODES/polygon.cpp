/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  Sprawdź czy dany punkt należy do wielokąta wypukłego.
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int tests,
	verts;

long long int pX, pY,
	aX, aY,
	fX, fY,
	lX, lY;
bool result;

int main(void)
{
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%lld %lld", &pX, &pY);
		scanf("%d", &verts);
		result = true;
		for(int v = 0; v < verts; ++ v)
		{
			scanf("%lld %lld", &aX, &aY);
			if(!v)
			{
				fX = aX;
				fY = aY;
			}

			if(v)
				result &= ((long long int)1LL * (aX - lX) * (pY - lY) - (aY - lY) * (pX - lX)) >= 0;

			lX = aX;
			lY = aY;
		}

		result &= ((long long int)1LL * (fX - lX) * (pY - lY) - (fY - lY) * (pX - lX)) >= 0;
		puts(result?"Tak":"Nie");
	}

	return 0;
}

