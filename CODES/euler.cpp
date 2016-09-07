/* 2011
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>
#include<vector>

int verts,
	edges,
	start, end,
	weight[1048576];
bool available[1048576];
std::vector<std::pair<int, int> > graph[1048576];
std::vector<int> cycle;

inline static void eulerPath(const int &v);

int main(void)
{
	scanf("%d %d", &verts, &edges);
	for(int e = 0; e < edges; ++ e)
	{
		scanf("%d %d", &start, &end);
		-- start;
		-- end;
		graph[start].push_back(std::make_pair(end, e));
		graph[end].push_back(std::make_pair(start, e));
		available[e] = true;
		++ weight[start];
		++ weight[end];
	}

	for(int v = 0; v < verts; ++ v)
	{
		if(weight[v] % 2)
		{
			puts("NIE");
			return 0;
		}

		weight[v] = 0;
	}

	puts("TAK");
	eulerPath(0);
	for(int c = cycle.size() - 1; c >= 0; -- c)
		printf("%d ", cycle[c]);

	puts("");
	return 0;
}

inline static void eulerPath(const int &v)
{
	for(long unsigned int n = weight[v]; n < graph[v].size(); n = ++ weight[v])
		if(available[graph[v][n].second])
		{
			available[graph[v][n].second] = false;
			eulerPath(graph[v][n].first);
		}

	cycle.push_back(v + 1);
	return;
}
