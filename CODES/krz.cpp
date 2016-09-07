/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  Krzyżak - AMPPZ 2011
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int points,
	d;
long long int x[131072],
			  y[131072],
			  z[131072];
bool collinear;

inline static const bool arePointsCollinear(const long long int &x1, const long long int &y1, const long long int &z1, const long long int &x2, const long long int &y2, const long long int &z2, const long long int &x3, const long long int &y3, const long long int &z3);
inline static const bool arePointsCoplanar(const long long int &xp, const long long int &yp, const long long int &zp, const long long int &x1, const long long int &y1, const long long int &z1, const long long int &x2, const long long int &y2, const long long int &z2, const long long int &x3, const long long int &y3, const long long int &z3);

int main(void)
{
	scanf("%d", &points);
	for(int p = 0; p < points; ++ p)
		scanf("%lld %lld %lld", &x[p], &y[p], &z[p]);

	if(points < 4)
	{
		puts("TAK");
		return 0;
	}

	collinear = true;
	for(d = 2; collinear && d < points; ++ d)
		collinear &= arePointsCollinear(x[d - 2], y[d - 2], z[d - 2], x[d - 1], y[d - 1], z[d - 1], x[d], y[d], z[d]);

	if(collinear)
	{
		puts("TAK");
		return 0;
	}

	for(int p = 0; p < points; ++ p)
		if(!arePointsCoplanar(x[p], y[p], z[p], x[d - 2], y[d - 2], z[d - 2], x[d - 1], y[d - 1], z[d - 1], x[d], y[d], z[d]))
		{
			puts("NIE");
			return 0;
		}

	puts("TAK");
	return 0;
}

inline static const bool arePointsCollinear(const long long int &x1, const long long int &y1, const long long int &z1, const long long int &x2, const long long int &y2, const long long int &z2, const long long int &x3, const long long int &y3, const long long int &z3)
{
	return (x3 - x1) * (y2 - y1) + (x2 - x1) * (z3 - z1) + (y3 - y1) * (z2 - z1) == (x3 - x1) * (z2 - z1) + (x2 - x1) * (y3 - y1) + (y2 - y1) * (z3 - z1);
}

inline static const bool arePointsCoplanar(const long long int &xp, const long long int &yp, const long long int &zp, const long long int &x1, const long long int &y1, const long long int &z1, const long long int &x2, const long long int &y2, const long long int &z2, const long long int &x3, const long long int &y3, const long long int &z3)
{
	return (xp - x1) * (yp - y2) * (zp - z3) + (yp - y1) * (zp - z2) * (xp - x3) + (zp - z1) * (xp - x2) * (yp - y3) == (xp - x1) * (zp - z2) * (yp - y3) + (yp - y1) * (xp - x2) * (zp - z3) + (zp - z1) * (yp - y2) * (xp - x3);
}
