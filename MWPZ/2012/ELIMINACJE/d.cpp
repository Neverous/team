/* 2012
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <algorithm>

using namespace std;

long long int tests,
    size,
    seq[10001],
    res,
    seqSum,
    numSum;

int main(void)
{
    scanf("%lld", &tests);
    for(long long int t = 0; t < tests; ++ t)
    {
        seqSum = 0;
        numSum = 0;
        res = 0;
        scanf("%lld", &size);
        for(long long int s = 0; s < size; ++ s)
            scanf("%lld", &seq[s]);

        sort(seq, seq + size);
        reverse(seq, seq + size);
        for(long long int s = 0; s < size; ++ s)
        {
            seqSum += seq[s];
            numSum += s;
            res = max(res, seqSum - numSum);
        }

        printf("%lld\n", res);
    }

    return 0;
}
