/* 2013
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>
#include <vector>
#include <queue>

int board,
    last,
    points,
    ones, twos;
long long int result;
char sizes[131072];

int main(void)
{
#ifdef ONLINE_JUDGE
    freopen("energy.in", "r", stdin);
    freopen("energy.out", "w", stdout);
#endif // ONLINE_JUDGE

    scanf("%d %s", &board, sizes);
    for(int s = 0; sizes[s]; ++ s)
    {
        switch(sizes[s])
        {
            case '1':
                if(ones)
                {
                    -- ones;
                    ++ points;
                }

                else if(last < board)
                {
                    ++ last;
                    ++ points;
                }

                else if(twos)
                {
                    ++ ones;
                    -- twos;
                }


                break;

            case '2':
                if(last + 1 < board)
                {
                    ++ points;
                    ++ twos;
                    ++ last;
                    ++ last;
                }

                break;
        }

        result += points;
    }

    printf("%lld\n", result);
    return 0;
}
