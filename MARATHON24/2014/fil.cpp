/* 2014
 * Maciej Szeptuch
 * II UWr
 */

#include <cstdio>
#include <algorithm>

int tests,
    cuts,
    fullpieces,
    fullpiece[131072];

char word[131072];

int solve(int left);

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %s", &cuts, word);
        for(int f = 0; f <= fullpieces; ++ f)
            fullpiece[f] = 0;

        fullpieces = 0;
        int w = 0;
        while(word[w] == 'D')
        {
            ++ fullpiece[fullpieces];
            ++ w;
        }

        ++ fullpieces;
        while(word[w])
        {
            while(word[w] == 'S')
                ++ w;

            while(word[w] == 'D')
            {
                ++ fullpiece[fullpieces];
                ++ w;
            }

            ++ fullpieces;
        }

        /*
        printf("%d: ", cuts);
        for(int f = 0; f < fullpieces; ++ f)
            printf("%d|", fullpiece[f]);

        puts("");*/

        int result = fullpiece[0];
        if(fullpieces == 1)
        {
            printf("%d\n", result);
            continue;
        }

        for(int f = 0; f < fullpieces; ++ f)
            result = std::max(result, fullpiece[f]);

        if(cuts == 0)
        {
            printf("%d\n", result);
            continue;
        }

        if(cuts == 1)
        {
            result = std::max(result, fullpiece[0] + fullpiece[fullpieces - 1]);
            printf("%d\n", result);
            continue;
        }

        std::sort(fullpiece + 1, fullpiece + fullpieces - 1);
        printf("%d\n", std::max(
            solve((cuts + 1) / 2) + std::max(fullpiece[0], fullpiece[fullpieces - 1]),
            std::max(fullpiece[0] + fullpiece[fullpieces - 1] + solve(cuts / 2),
                solve(cuts / 2 + 1))));
    }

    return 0;
}

inline
int solve(int left)
{
    if(!left)
        return 0;

    int result = 0;
    for(int f = fullpieces - 2; f > 0 && left -- > 0; -- f)
        result += fullpiece[f];

    return result;
}
