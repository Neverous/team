/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  Sprawdź czy dany punkt należy do wielokąta.
 */
#include<cstdio>
#include<cstdlib>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

const int rands = 4;

int tests,
	verts;

long long int pX, pY,
	aX, aY,
	fX, fY,
	lX, lY,
	rX[rands], rY[rands],
	res[rands];
bool result;

inline static const int sgn(const long long int &a){return a==0?0:a<0?-1:1;}
inline static const long long int cross(const long long int &x1, const long long int &y1, const long long int &x2, const long long int &y2, const long long int &x, const long long int &y);
inline static const bool segmentIntersect(const long long int &x1, const long long int &y1, const long long int &x2, const long long int &y2,
										  const long long int &x3, const long long int &y3, const long long int &x4, const long long int &y4);

int main(void)
{
	srand(1337 * 42);
	for(int r = 0; r < rands/2; ++ r)
	{
		rX[r] = 1000000;
		rY[r] = rand() % 1000000;
	}

	for(int r = rands/2; r < rands; ++ r)
	{
		rX[r] = 1000000;
		rY[r] = -rand() % 1000000;
	}

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

			if(v) for(int r = 0; r < rands; ++ r)
				res[r] += segmentIntersect(lX, lY, aX, aY, pX, pY, rX[r], rY[r]);

			lX = aX;
			lY = aY;
		}
		
		for(int r = 0; r < rands; ++ r)
			res[r] += segmentIntersect(lX, lY, fX, fY, pX, pY, rX[r], rY[r]);

		for(int r = 0; r < rands; ++ r)
		{
			result &= res[r] % 2;
			res[r] = 0;
		}
		
		puts(result?"Tak":"Nie");
	}

	return 0;
}

inline static const bool segmentIntersect(const long long int &x1, const long long int &y1, const long long int &x2, const long long int &y2,
										  const long long int &x3, const long long int &y3, const long long int &x4, const long long int &y4)
{
	return sgn(cross(x1, y1, x2, y2, x3, y3)) * sgn(cross(x1, y1, x2, y2, x4, y4)) <= 0 &&
		   sgn(cross(x3, y3, x4, y4, x1, y1)) * sgn(cross(x3, y3, x4, y4, x2, y2)) <= 0;
}

inline static const long long int cross(const long long int &x1, const long long int &y1, const long long int &x2, const long long int &y2, const long long int &x, const long long int &y)
{
	return (long long int)1LL * (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
}
