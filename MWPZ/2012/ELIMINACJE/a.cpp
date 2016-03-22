/* 2012
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

int tests,
    birds,
    trees,
    connected[20002],
    dist[20002],
    visited[20002],
    first, second;
vector<int> graph[20002];

int hopcroftKarp(void);
void hopcroftKarpBFS(void);
bool hopcroftKarpDFS(int where, int flag);;

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        for(int v = 0; v < 20002; ++ v)
        {
            graph[v].clear();
            connected[v] = -1;
        }

        scanf("%d %d", &trees, &birds);
        for(int b = 0; b < birds; ++ b)
        {
            scanf("%d %d", &first, &second);
            first += birds;
            second += birds;
            graph[b].push_back(first);
            graph[first].push_back(b);
            graph[b].push_back(second);
            graph[second].push_back(b);
        }

        if(hopcroftKarp() != birds)
        {
            puts("0");
            continue;
        }

        printf("1 ");
        for(int b = 0; b < birds; ++ b)
            printf("%d ", connected[b] - birds);

        puts("");
    }

    return 0;
}

int hopcroftKarp(void)
{
    int flag = 0,
        counter = 0;

    bool tmp;

    do
    {
        tmp = false;
        hopcroftKarpBFS();

        flag ++;
        for(int b = 0; b < birds; ++ b)
            if(connected[b] == -1)
                if(hopcroftKarpDFS(b, flag))
                {
                    ++ counter;
                    tmp = true;
                }

    }
    while(tmp);

    return counter;
}

void hopcroftKarpBFS(void)
{
    int act;
    queue<int> que;

    for(int b = 0; b < birds; ++ b)
    {
        dist[b] = -1;
        if(connected[b] == -1)
        {
            que.push(b);
            dist[b] = 0;
        }
    }

    while(!que.empty())
    {
        act = que.front();
        que.pop();
        for(unsigned int v = 0; v < graph[act]. size(); ++ v)
        {
            if(connected[graph[act][v]] == -1)
                continue;

            if(dist[connected[graph[act][v]]] == -1)
            {
                dist[connected[graph[act][v]]] = dist[act] + 1;
                que.push(connected[graph[act][v]]);
            }
        }
    }
    return;
}

bool hopcroftKarpDFS(int where, int flag)
{
    visited[where] = flag;
    for(unsigned int v = 0; v < graph[where]. size(); ++ v)
    {
        if(connected[graph[where][v]] == -1 ||
                (visited[connected[graph[where][v]]]!= flag &&
                 dist[connected[graph[where][v]]]- 1 == dist[where]&&
                 hopcroftKarpDFS(connected[graph[where][v]], flag))
          )
        {
            connected[graph[where][v]] = where;
            connected[where]= graph[where][v];
            return true;
        }
    }
    return false;
}
