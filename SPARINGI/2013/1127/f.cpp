/* 2013
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>
#include <vector>
#include <set>

int passengers,
    rows, zones,
    place;
std::vector<int> people[1024];

bool possible(int max);

int main(void)
{
    scanf("%d %d %d", &passengers, &rows, &zones);
    for(int p = 0; p < passengers; ++ p)
    {
        scanf("%d", &place); -- place;
        people[place].push_back(p);
    }

    int start   = 0,
        end     = 1048576;
    while(start < end)
    {
        int mid = (start + end) / 2;
        if(possible(mid))
            end = mid;

        else
            start = mid + 1;
    }

    printf("%d\n", start);
    return 0;
}

bool possible(int max)
{
    std::set<int> act;
    int cur = 0,
        z = 0;

    printf("][ %d ][\n", max);
    for(int r = 0; r < rows && z < zones; ++ r)
    {
        printf("%d: %d? ", r + 1, cur);
        for(unsigned int p = 0; p < people[r].size(); ++ p)
        {
            cur += std::distance(act.begin(), act.lower_bound(people[r][p]));
            printf("%d, ", cur);
        }

        printf(" %d!\n", cur);
        if(cur > max)
        {
            ++ z;
            cur = 0;
            act.clear();
        }

        for(unsigned int p = 0; p < people[r].size(); ++ p)
            act.insert(people[r][p]);
    }

    puts("");
    return z < zones;
}
