#include <cstdio>
#include <queue>

int verts,
    edges,
    paths,
    from, to, time;
char type[8];

std::vector<std::pair<std::pair<int, int>, bool> > graph[4096];
void shortestPath(int a, int b);

int main(void)
{
    scanf("%d %d %d", &verts, &edges, &paths);
    for(int e = 0; e < edges; ++ e)
    {
        scanf("%d %d %d %s", &from, &to, &time, type);
        graph[from].push_back(std::make_pair(std::make_pair(to, time), type[0] == 'I'));
        graph[to].push_back(std::make_pair(std::make_pair(from, time), type[0] == 'I'));
    }

    for(int p = 0; p < paths; ++ p)
    {
        scanf("%d %d", &from, &to);
        shortestPath(from, to);
    }

    return 0;
}

inline
void shortestPath(int a, int b)
{
    std::priority_queue<std::pair<std::pair<long long int, long long int>, int> > que;
    bool visited[4096] = {};
    std::pair<long long int, long long int> shortest[4096];
    for(int v = 0; v < verts; ++ v)
        shortest[v].first = shortest[v].second = 1048576;

    shortest[a].first = shortest[a].second = 0;
    que.push(std::make_pair(std::make_pair(0, 0), a));

    while(!que.empty())
    {
        std::pair<std::pair<long long int, long long int>, int> act = que.top();
        que.pop();

        act.first.first *= -1;
        act.first.second *= -1;

        if(visited[act.second])
            continue;

        visited[act.second] = true;
        if(act.second == b)
            break;

        for(unsigned int n = 0; n < graph[act.second].size(); ++ n)
        {
            if(visited[graph[act.second][n].first.first])
                continue;

            std::pair<long long int, long long int> temp = act.first;
            if(graph[act.second][n].second)
                temp.second += graph[act.second][n].first.second;

            else
                temp.first += graph[act.second][n].first.second;

            if(temp < shortest[graph[act.second][n].first.first])
            {
                que.push(std::make_pair(std::make_pair(-temp.first, -temp.second), graph[act.second][n].first.first));
                shortest[graph[act.second][n].first.first] = temp;
            }

        }
    }

    if(!visited[b])
        puts("IMPOSSIBLE");

    else
        printf("%lld %lld\n", shortest[b].first, shortest[b].first + shortest[b].second);

    return;
}
