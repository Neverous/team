/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>

int tests;
long long int a, b;

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%lld %lld", &a, &b);
        printf("%lld\n", a + b);
    }

    return 0;
}
