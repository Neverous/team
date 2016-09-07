/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  LCA z Union Findem.
 */
#include<cstdio>
#include<vector>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

const int MAX_VERTS = 600000;

int verts,
	trip,
	first, second,
	act, next, lca,
	depth[MAX_VERTS + 1],
	ord, start[MAX_VERTS + 1], end[MAX_VERTS + 1],
	father[MAX_VERTS + 1][20];
long long int result;

std::vector<int> graph[MAX_VERTS + 1];

inline static void dfs(const int &v, const int &f = -1, const int &d = 1);
inline static const int findLCA(const int &a, int b);
inline static const int inInterval(const int &v, const int &v2);

int main(void)
{
	scanf("%d", &verts);
	for(int e = 1; e < verts; ++ e)
	{
		scanf("%d %d", &first, &second);
		-- second;
		-- first;
		graph[first].push_back(second);
		graph[second].push_back(first);
	}

	dfs(0);
	scanf("%d", &trip);
	for(int t = 0; t < trip; ++ t)
	{
		scanf("%d", &next);
		-- next;
		lca = findLCA(act, next);
		result += depth[act] + depth[next] - 2 * depth[lca];
		act = next;
	}

	printf("%lld\n", result);
	return 0;
}

inline static void dfs(const int &v, const int &f, const int &d)
{
	depth[v] = d;
	if(f != -1)
	{
		father[v][0] = f;
		for(int n = 1; n < 20; ++ n)
			father[v][n] = father[father[v][n - 1]][n - 1];
	}

	start[v] = ord ++;
	for(unsigned int n = 0; n < graph[v].size(); ++ n)
		if(graph[v][n] != f)
			dfs(graph[v][n], v, d + 1);

	end[v] = ord ++;
}

inline static const int findLCA(const int &a, int b)
{
	if(inInterval(a, b))
		return b;

	if(inInterval(b, a))
		return a;

	int f = 0;
	while(!inInterval(a, b))
	{
		f = 0;
		while(!inInterval(a, father[b][++ f]));
		b = father[b][f - 1];
	}

	return b;
}

inline static const int inInterval(const int &v, const int &v2)
{
	return start[v2] <= start[v] && start[v] <= end[v2];
}
