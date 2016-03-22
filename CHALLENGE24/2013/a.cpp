/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <vector>
#include <set>

int managers,
    desire,
    zombies[2097152],
    minions,
    minion,
    father[2097152],
    result;

std::vector<int> tree[2097152],
                 level[2097152];
std::set<std::pair<int, int> > sorted;

inline int levelOrder(const int &v, const int &l = 0);

int main(void)
{
    scanf("%d %d", &managers, &desire);
    for(int m = 0; m < managers; ++ m)
    {
        scanf("%d %d", &zombies[m], &minions);
        zombies[m] += minions;
        for(int i = 0; i < minions; ++ i)
        {
            scanf("%d", &minion);
            tree[m].push_back(minion);
            father[minion] = m;
        }
    }

    for(int m = levelOrder(0); m >= 0; -- m)
    {
        sorted.clear();
        for(const int &v: level[m])
            sorted.insert(std::make_pair(zombies[v], v));

        for(const std::pair<int, int> &rep: sorted)
        {
            if(zombies[father[rep.second]] + rep.first - 1 <= desire)
            {
                zombies[father[rep.second]] += rep.first - 1;
                //printf("TITS OR GTFO: %d\n", rep.second);
                ++ result;
            }
        }
    }

    printf("%d\n", result);
    return 0;
}

inline int levelOrder(const int &v, const int &l)
{
    int max = l;
    level[l].push_back(v);
    for(const int &s: tree[v])
        if(father[v] != s)
            max = std::max(max, levelOrder(s, l + 1));

    return max;
}
