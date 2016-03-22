/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <vector>
#include <algorithm>

const long long int MOD = 1000000007;
int questions,
    max,
    result[1048576],
    prime[16777216],
    count[16777216];
long long int cur = 1;
std::pair<int, int> question[1048576];

void preprocess(int limit);

int main(void)
{
    for(questions = 0; scanf("%d", &question[questions].first) != -1 && question[questions].first > 0; ++ questions)
    {
        question[questions].second = questions;
        max = std::max(max, question[questions].first);
    }

    preprocess(max);
    std::sort(question, question + questions);
    for(int n = 1, q = 0; n <= max; ++ n)
    {
        for(int act = n; act > 1; act /= prime[act])
        {
            if(++ count[prime[act]] > 1)
            {
                count[prime[act]] -= 2;
                cur *= prime[act];
                if(cur > MOD)
                    cur %= MOD;

                cur *= prime[act];
                if(cur > MOD)
                    cur %= MOD;
            }
        }

        if(question[q].first == n)
        {
            result[question[q].second] = cur;
            ++ q;
        }
    }

    for(int q = 0; q < questions; ++ q)
        printf("%d\n", result[q]);

    return 0;
}

inline
void preprocess(int limit)
{
    for(int n = 0; n <= limit; ++ n)
        prime[n] = n;

    for(int n = 2; (long long int) n * n <= limit; ++ n)
        if(prime[n] == n) for(long long int m = (long long int) n * n; m <= limit; m += n)
            prime[m] = n;
}
