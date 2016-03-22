/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <queue>

int size,
    seq[2097152];

int main(void)
{
    while(scanf("%d", &size) != - 1 && size > 0)
    {
        std::deque<int> que;
        for(int s = 0; s < size; ++ s)
        {
            scanf("%d", &seq[s]);
            if(s) seq[s] += seq[s-1];
        }

        for(int s = 0; s < size; ++ s)
            seq[size + s] = seq[s] + seq[size - 1];

        for(int s = 0; s < size; ++ s)
        {
            while(!que.empty() && seq[que.back()] >= seq[s])
                que.pop_back();

            que.push_back(s);
        }

        int count = seq[que.front()] >= 0;
        for(int s = 1; s < size; ++ s)
        {
            while(!que.empty() && que.front() < s)
                que.pop_front();

            while(!que.empty() && seq[que.back()] >= seq[size - 1 + s])
                que.pop_back();

            que.push_back(size - 1 + s);
            count += seq[que.front()] - seq[s - 1] >= 0;
        }

        printf("%d\n", count);
    }

    return 0;
}
