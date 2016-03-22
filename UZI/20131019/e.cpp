/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

int tests,
    steps;
char path[131072];
map<pair<int, int>, bool> available;

int bfs(pair<int, int> start, pair<int, int> end);
pair<int, int> operator+(const pair<int, int> &A, const pair<int, int> B)
{
    return {A.first + B.first, A.second + B.second};
}

pair<int, int> operator-(const pair<int, int> &A, const pair<int, int> B)
{
    return {A.first - B.first, A.second - B.second};
}

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %s", &steps, path);
        pair<int, int> act(0, 0);
        available.clear();
        available[act] = true;
        for(int s = 0; s < steps; ++ s)
        {
            switch(path[s])
            {
                case 'N':
                    act.second += 1;
                    break;

                case 'S':
                    act.second -= 1;
                    break;

                case 'E':
                    act.first += 1;
                    break;

                case 'W':
                    act.first -= 1;
                    break;
            }

            available[act] = true;
        }

        if(bfs({0, 0}, act) == 0)
            puts("0\nGdzie leziesz?");
    }

    return 0;
}

int bfs(pair<int, int> start, pair<int, int> end)
{
    map<pair<int, int>, int> shortest;
    map<pair<int, int>, pair<int, int> > father;
    queue<pair<int, int> > que;
    available[start] = false;
    que.push(start);

    while(!que.empty() && available[end])
    {
        pair<int, int> act = que.front(); que.pop();
        for(const auto &step: (vector<pair<int, int> >) {{0, 1}, {0, -1}, {1, 0}, {-1, 0}})
        {
            pair<int, int> next = act + step;
            if(!available[next])
                continue;

            available[next] = false;
            father[next] = act;
            shortest[next] = shortest[act] + 1;
            que.push(next);
        }
    }

    if(shortest[end] == 0)
        return 0;

    printf("%d\n", shortest[end]);
    pair<int, int> act = end;
    string _path; _path.reserve(131072);
    while(act != start)
    {
        pair<int, int> step = act - father[act];
        if(step.first == 1)
            _path += 'E';

        else if(step.first == -1)
            _path += 'W';

        else if(step.second == 1)
            _path += 'N';

        else
            _path += 'S';

        act = father[act];
    }

    reverse(_path.begin(), _path.end());
    puts(_path.c_str());

    return shortest[end];
}
