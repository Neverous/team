#include <cstdio>
#include <cstring>

int tests,
    people,
    count[1024],
    score;

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        memset(count, 0, 1024 * sizeof(int));
        scanf("%d", &people);
        for(int p = 0; p < people; ++ p)
        {
            scanf("%d", &score);
            ++ count[score];
        }

        int p = 1000;
        while(!count[p]) -- p;

        while(count[p] -- > 0)
            printf("%d ", p);

        for(p = 0; p < 1000; ++ p)
            while(count[p] -- > 0)
                printf("%d ", p);

        puts("");
    }

    return 0;
}
