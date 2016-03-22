/* 2012
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>
#include<vector>

int verts,
	father,
	with[1048576],
	without[1048576];
std::vector<int> graph[1048576];

inline static const int MIN(const int &a, const int &b){return a<b?a:b;}

inline static const void dfs(const int &v)
{
	for(int s = 0; s < graph[v].size(); ++ s)
		dfs(graph[v][s]);

	with[v] = 1;
	for(int s = 0; s < graph[v].size(); ++ s)
	{
		without[v] += with[graph[v][s]];
		with[v] += MIN(with[graph[v][s]], without[graph[v][s]]);
	}
}

int main(void)
{
	scanf("%d", &verts);
	for(int v = 1; v < verts; ++ v)
	{
		scanf("%d\n", &father);
		-- father;
		graph[father].push_back(v);
	}

	dfs(0);
	printf("%d\n", MIN(with[0], without[0]));
}
