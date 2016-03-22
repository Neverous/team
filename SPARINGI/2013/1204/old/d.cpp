#include <cstdio>

int tests,
    people,
    candy;

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %d", &people, &candy);
        puts(people > 1 && candy % (people - 1) == 0 ? "NIE" : "TAK");
    }

    return 0;
}
