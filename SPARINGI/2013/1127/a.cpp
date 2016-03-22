/* 2013
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>

int names;
char name[32];
bool have[32];

int main(void)
{
#ifdef ONLINE_JUDGE
    freopen("arrange.in", "r", stdin);
    freopen("arrange.out", "w", stdout);
#endif // ONLINE_JUDGE

    scanf("%d", &names);
    for(int n = 0; n < names; ++ n)
    {
        scanf("%s", name);
        have[name[0] - 'A'] = true;
    }

    int r = 0;
    while(have[r])
        ++ r;

    printf("%d\n", r);
    return 0;
}
