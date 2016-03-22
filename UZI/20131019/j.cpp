/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>

int tests,
    films, maxWeight, actions,
    filmWeight[1024], filmProfit[1024],
    prefix[1024][4096],
    suffix[1024][4096],
    without, weight;

inline int max(int a, int b){return a>b?a:b;}
void preprocess(void);
int bestProfit(int _without, int _weight);

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %d %d", &films, &maxWeight, &actions);
        for(int f = 0; f < films; ++ f)
            scanf("%d %d", &filmWeight[f], &filmProfit[f]);

        preprocess();
        for(int a = 0; a < actions; ++ a)
        {
            scanf("%d %d", &weight, &without);
            printf("%d\n", bestProfit(without, weight));
        }

        for(int f = 0; f <= films + 8; ++ f)
            for(int m = 0; m <= maxWeight + 8; ++ m)
                prefix[f][m] = suffix[f][m] = 0;
    }

    return 0;
}

void preprocess(void)
{
    {
        int act[4096] = {};
        /* PREFIX */
        for(int f = 0; f < films; ++ f)
        {
            for(int m = maxWeight; m - filmWeight[f] >= 0; -- m)
                act[m] = max(act[m], act[m - filmWeight[f]] + filmProfit[f]);

            for(int m = 0; m <= maxWeight; ++ m)
                prefix[f + 1][m] = act[m];
        }
    }


    {
        int act[4096] = {};
        /* SUFFIX */
        for(int f = films - 1; f >= 0; -- f)
        {
            for(int m = maxWeight; m - filmWeight[f] >= 0; -- m)
                suffix[f + 1][m] = act[m] = max(act[m], act[m - filmWeight[f]] + filmProfit[f]);

            for(int m = 0; m <= maxWeight; ++ m)
                suffix[f + 1][m] = act[m];
        }
    }
}

int bestProfit(int _without, int _weight)
{
    int profit = 0;
    for(int i = 0; i <= _weight; ++ i)
        if(profit < prefix[_without - 1][i] + suffix[_without + 1][_weight - i])
            profit = prefix[_without - 1][i] + suffix[_without + 1][_weight - i];

    return profit;
}
