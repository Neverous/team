/* 2014
 * Maciej Szeptuch
 * II UWr
 */

#include <cstdio>
#include <algorithm>

int resolutionH, sizeH,
    resolutionV, sizeV,
    resH, sizH,
    resV, sizV,
    screens, price,
    result = (1 << 30);

int main(void)
{
#ifndef SELENE
    freopen("giant.in", "r", stdin);
    freopen("giant.out", "w", stdout);
#endif // SELENE

    scanf("%d %d %d %d %d", &resolutionH, &resolutionV, &sizeH, &sizeV, &screens);
    for(int s = 0; s < screens; ++ s)
    {
        scanf("%d %d %d %d %d", &resH, &resV, &sizH, &sizV, &price);
        result = std::min(result,
            std::max((resolutionH + resH - 1) / resH, (sizeH + sizH - 1) / sizH) * std::max((resolutionV + resV - 1) / resV, (sizeV + sizV - 1) / sizV) * price);

        result = std::min(result,
            std::max((resolutionH + resV - 1) / resV, (sizeH + sizV - 1) / sizV) * std::max((resolutionV + resH - 1) / resH, (sizeV + sizH - 1) / sizH) * price);
    }

    printf("%d\n", result);
    return 0;
}
