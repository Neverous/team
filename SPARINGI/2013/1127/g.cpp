/* 2013
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>
#include <cmath>

int W, H,
    w, h;

int main(void)
{
#ifdef ONLINE_JUDGE
    freopen("garage.in", "r", stdin);
    freopen("garage.out", "w", stdout);
#endif // ONLINE_JUDGE

    scanf("%d %d %d %d", &W, &H, &w, &h);
    printf("%d\n", (int) (round(W / 2.0 / w) * round(H / 2.0 / h)));
    return 0;
}
