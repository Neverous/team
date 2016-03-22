/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <algorithm>

const double LENGTH = 10000.0;

int old, add;
double res[2];

double dist_add(int idz);
double dist_old(int idz);

int main(void)
{
#ifndef SELENE
    freopen("graveyard.in", "r", stdin);
    freopen("graveyard.out", "w", stdout);
#endif // SELENE

    scanf("%d %d", &old, &add);
    for(int o = 0, n = 1, c = 1, p = 1; o < old && n < add + old;)
    {
        if(c == 1 && p == 1)
            res[0] = res[1] = std::min(res[0], res[1]);

        else if(c == 0)
        {
            res[1] = std::min(res[0], res[1]) + dist_add(n + 1) - dist_old(o);
            res[0] += dist_old(o) - dist_add(n);
        }

        if(dist_add(n + 1) < dist_old(o + 1))
        {
            ++ n;
            p = c;
            c = 1;
        }

        else
        {
            ++ o;
            p = c;
            c = 0;
        }
    }

    printf("%.6lf\n", std::min(res[0], res[1]));
    return 0;
}

inline
double dist_add(int idz)
{
    return LENGTH * idz / (add + old);
}

inline
double dist_old(int idz)
{
    return LENGTH * idz / old;
}
