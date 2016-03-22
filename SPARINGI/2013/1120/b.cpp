/* 2013
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>

int tests,
    a, b;

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %d", &a, &b);
        if(a == 1 && b == 1)
            puts("AMBIGUOUS");

        else if( (a < 0 || b < 0)
        ||  (a == 0 && b != 0)
        ||  (a == 1 || a > b))
            puts("IMPOSSIBLE");

        else if(a == b)
            printf("%d\n", a);

        else
        {
            int result[64] = {},
                r = 0,
                sum = 0;

            while(b > 0)
            {
                sum += result[r ++] = b % a;
                b /= a;
            }

            if(sum != a)
            {
                puts("IMPOSSIBLE");
                continue;
            }

            while(r > 0)
                printf("%d ", result[-- r]);

            puts("");
        }
    }

    return 0;
}
