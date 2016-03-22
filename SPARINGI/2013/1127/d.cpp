/* 2013
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>
#include <vector>
#include <queue>

typedef std::pair<int, int> Pair;
int items, crafting,
    price[16384],
    item[131072], first[131072], second[131072];
std::vector<int> create[16384];
std::priority_queue<Pair> que;

int main(void)
{
#ifdef ONLINE_JUDGE
    freopen("dwarf.in", "r", stdin);
    freopen("dwarf.out", "w", stdout);
#endif // ONLINE_JUDGE

    scanf("%d %d", &items, &crafting);
    for(int i = 0; i < items; ++ i)
    {
        scanf("%d", &price[i]);
        que.push(Pair(-price[i], i));
    }

    for(int c = 1; c <= crafting; ++ c)
    {
        scanf("%d %d %d", &item[c], &first[c], &second[c]);
        -- item[c];
        create[first[c] - 1].push_back(c);
        create[second[c] - 1].push_back(-c);
    }

    while(!que.empty())
    {
        Pair act = que.top(); que.pop();
        act.first *= -1;
        if(act.second == 0)
            break;

        if(act.first > price[act.second])
            continue;

        for(unsigned int n = 0; n < create[act.second].size(); ++ n)
        {
            int id = create[act.second][n];
            if(id > 0)
                first[id] = -act.first;

            else
                second[id *= -1] = -act.first;

            if(first[id] <= 0 && second[id] <= 0)
            {
                int cost = -first[id] - second[id];
                if(cost < price[item[id]])
                {
                    price[item[id]] = cost;
                    que.push(Pair(-cost, item[id]));
                }
            }
        }
    }

    printf("%d\n", price[0]);
    return 0;
}
