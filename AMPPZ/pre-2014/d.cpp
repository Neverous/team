/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <queue>

int cities,
    boxes,
    people[524288],
    count[524288];

int main(void)
{
    while(scanf("%d %d", &cities, &boxes) != -1 && cities != -1 && boxes != -1)
    {
        std::priority_queue<std::pair<int, int> > que;
        boxes -= cities;
        for(int c = 0; c < cities; ++ c)
        {
            scanf("%d", &people[c]);
            count[c] = 1;
            que.push(std::make_pair(people[c], c));
        }

        while(boxes -- > 0)
        {
            std::pair<int, int> act = que.top();
            que.pop();
            count[act.second] ++;
            act.first = (people[act.second] + count[act.second] - 1) / count[act.second];
            que.push(act);
        }

        printf("%d\n", que.top().first);
    }

    return 0;
}
