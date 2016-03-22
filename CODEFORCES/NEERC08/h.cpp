/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <vector>
#include <algorithm>

int size,
    sum,
    diff,
    res[131072];
std::pair<int, int> seq[131072];

int main(void)
{
#ifndef SELENE
    freopen("hell.in", "r", stdin);
    freopen("hell.out", "w", stdout);
#endif // SELENE

    scanf("%d", &size);
    for(int s = 0; s < size; ++ s)
    {
        scanf("%d", &seq[s].first);
        seq[s].second = s;
        sum += seq[s].first;
    }

    if(sum % 2)
    {
        puts("No");
        return 0;
    }

    std::sort(seq, seq + size);
    for(int s = size - 1; s >= 0; -- s)
    {
        if(diff < 0)
        {
            diff += seq[s].first;
            seq[s].first = 1;
        }

        else
        {
            diff -= seq[s].first;
            seq[s].first = -1;
        }
    }

    for(int s = 0; s < size; ++ s)
        res[seq[s].second] = seq[s].first;

    puts("Yes");
    for(int s = 0; s < size; ++ s)
        printf("%d ", res[s]);

    puts("");

    return 0;
}
