#include <cstdio>

int tests,
    people,
    res;

char treats[131072];

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %s", &people, treats);
        res = 0;
        for(int tr = 0; treats[tr]; ++ tr)
        {
            res = res * 10 + treats[tr] - '0';
            res %= people;
        }

        puts(res ? "NIE" : "TAK");
    }

    return 0;
}
