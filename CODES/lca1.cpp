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
const int MAX_QUESTIONS = 100000;

int verts,
	trip,
	first, second,
	act, next,
	depth[MAX_VERTS + 1],
	repr[MAX_VERTS + 1];
bool answered[MAX_QUESTIONS + 1];
long long int result;

std::vector<int> graph[MAX_VERTS + 1],
				 another[MAX_VERTS + 1],
				 question[MAX_VERTS + 1];

inline static void dfs(const int &v, const int &f = -1, const int &d = 1);
inline static void unionUnion(const int &a, const int &b);
inline static const int unionFind(const int &a);

int main(void)
{
	scanf("%d", &verts);
	for(int v = 0; v < verts; ++ v)
		repr[v] = v;

	for(int e = 1; e < verts; ++ e)
	{
		scanf("%d %d", &first, &second);
		-- second;
		-- first;
		graph[first].push_back(second);
		graph[second].push_back(first);
	}

	scanf("%d", &trip);
	for(int t = 0; t < trip; ++ t)
	{
		scanf("%d", &next);
		-- next;
		another[act].push_back(next);
		question[act].push_back(t);
		another[next].push_back(act);
		question[next].push_back(t);
		act = next;
	}

	dfs(0);
	printf("%lld\n", result);
	return 0;
}

inline static void dfs(const int &v, const int &f, const int &d)
{
	depth[v] = d;
	for(unsigned int n = 0; n < graph[v].size(); ++ n)
		if(graph[v][n] != f)
		{
			dfs(graph[v][n], v, d + 1);
			unionUnion(graph[v][n], v);
		}

	for(unsigned int q = 0; q < question[v].size(); ++ q)
		if(!answered[question[v][q]] && depth[another[v][q]])
		{
			answered[question[v][q]] = true;
			result += depth[v] + depth[another[v][q]] - 2 * depth[unionFind(another[v][q])];
		}
}

inline static void unionUnion(const int &a, const int &b)
{
	const int fA = unionFind(a),
			  fB = unionFind(b);

	if(depth[fA] < depth[fB])
		repr[fB] = fA;

	else
		repr[fA] = fB;

	return;
}

inline static const int unionFind(const int &a)
{
	if(repr[a] != a)
		repr[a] = unionFind(repr[a]);

	return repr[a];
}
