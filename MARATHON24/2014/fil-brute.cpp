/* 2014
 * Maciej Szeptuch
 * II UWr
 */

#include <cstdio>

int tests,
    cuts;
char word[131072];

int solve(int left, char *film);

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %s", &cuts, word);
        printf("%d => %d\n", cuts, solve(cuts, word));
    }

    return 0;
}

inline
int solve(int left, char *film)
{
    int count = 0;
    for(int f = 0; film[f]; ++ f)
        if(film[f] == 'D' && film[f + 1] == 'S')
            ++ count;

        else if(film[f] == 'S' && film[f + 1] == 'D')
            ++ count;

    return count;
}
