/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>

char days[131072];
int count,
    result;

int main(void)
{
    scanf("%*d %s", days);
    for(int d = 0; days[d]; ++ d)
    {
        if(days[d] == 'S')
            ++ count;

        else
            count = 0;

        if(result < count)
            result = count;
    }

    printf("%d\n", result);
    return 0;
}
