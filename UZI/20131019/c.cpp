/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstring>

int tests,
    planets;
long long int x, y, z, r;
char name[128];

struct Planet
{
    char name[128];
    long long int dist;
    long long int r;
} best;

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d", &planets);
        for(int p = 0; p < planets; ++ p)
        {
            scanf("%s %lld %lld %lld %lld", name, &x, &y, &z, &r);
            if(!best.r || best.dist * r * r > (x*x + y*y + z*z) * best.r)
            {
                strcpy(best.name, name);
                best.dist = x*x + y*y + z*z;
                best.r = r*r;
            }
        }

        puts(best.name);
        memset(&best, 0, sizeof(Planet));
    }

    return 0;
}
