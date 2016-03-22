/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>

int cities,
    boxes,
    people[524288];

int findBest(void);
bool check(int wanted);

int main(void)
{
    while(scanf("%d %d", &cities, &boxes) != -1 && cities > 0 && boxes > 0)
    {
        for(int c = 0; c < cities; ++ c)
            scanf("%d", &people[c]);

        printf("%d\n", findBest());
    }

    return 0;
}

inline
int findBest(void)
{
    int start = 1,
        end = 5000001;

    while(start < end)
    {
        int middle = (start + end) / 2;
        if(check(middle))
            end = middle;

        else
            start = middle + 1;
    }

    return start;
}

inline
bool check(int wanted)
{
    int needed = 0;
    for(int c = 0; needed <= boxes && c < cities; ++ c)
        needed += (people[c] + wanted - 1) / wanted;

    return needed <= boxes;
}
