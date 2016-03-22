#include <cstdio>
#include <vector>
#include <queue>

typedef std::pair<short, short> Pair;

int tests,
    verts, edges,
    from, to, cost,
    prev[128][1024],
    len[128][128],
    length, start, end;

std::vector<Pair> graph[128];

bool solve(void);

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        for(int v = 0; v < 128; ++ v)
        {
            for(int l = 0; l < 1024; ++ l)
                prev[v][l] = -1;

            graph[v].clear();
        }

        scanf("%d %d", &verts, &edges);
        for(int e = 0; e < edges; ++ e)
        {
            scanf("%d %d %d", &from, &to, &cost); -- from, -- to;
            len[from][to] = len[to][from] = cost;
            graph[from].push_back(Pair(to, cost));
            graph[to].push_back(Pair(from, cost));
        }

        scanf("%d %d %d", &length, &start, &end);
        -- start; -- end;
        solve();
        if(false)
        {
            int act = end;
            std::vector<int> result;
            result.push_back(act);

            do
            {
                int lenb = length;
                length -= len[act][prev[act][length]];
                act = prev[act][lenb];
                result.push_back(act);
            }
            while(length > 0);

            printf("%u\n", result.size());
            for(int r = result.size() - 1; r >= 0; -- r)
                printf("%d ", result[r] + 1);

            puts("");
        }

        else
            puts("BRAK TRASY");
    }

    return 0;
}

bool solve(void)
{
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> > que;
    que.push(Pair(0, start));

    while(!que.empty())
    {
        Pair act = que.top(); que.pop();
        if(act.second > length)
            break;

        for(unsigned int n = 0; n < graph[act.second].size(); ++ n)
        {
            int le = act.first + graph[act.second][n].second;
            int v = graph[act.second][n].first;
            if(le > length || prev[v][le] != -1)
                continue;

            prev[v][le] = act.second;
            que.push(Pair(le, v));
        }
    }

    return prev[end][length] != -1;
}
