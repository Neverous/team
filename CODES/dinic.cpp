/* 2011
 * Maciej Szeptuch
 * -*-*-*-*-*-*-*-*-
 */
#include<cstdio>
#include<vector>
#include<queue>
#include<cstring>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

const int BUFFER = 4096;
int p = BUFFER - 1;
char buffer[BUFFER];

int getInt(void)
{
	int number = 0;
	while((buffer[p] < '0' || buffer[p] > '9') && ++ p)
		if(p == BUFFER)
		{
			fread(buffer, sizeof(char), BUFFER, stdin);
			p = 0;
		}

	while(buffer[p] >= '0' && buffer[p] <= '9')
	{
		number = number * 10 + buffer[p] - '0';
		++ p;

		if(p == BUFFER)
		{
			fread(buffer, sizeof(char), BUFFER, stdin);
			p = 0;
		}
	}

	return number;
}

int tests,
	verts,
	edges,
	transferred[4096],
	least[4096],
	limit[4096],
	start, end, min, max,
	temp,
	visited[256];
bool done;
std::vector<std::pair<int, int> > graph[256];

static inline const int $(const int &a){return a >= edges?a - edges:a + edges;}
static inline const int MAX(const int &a, const int &b){return a > b?a:b;}
static inline const int MIN(const int &a, const int &b){return a < b?a:b;}
/* DINIC */
std::queue<short int> que;
short int qAct,
	  latest[256];
bool impossible[256];
inline static int dinic(const int &s, const int &e, int push);
inline static int dinicBFS(const int &s, const int &e);
inline static int dinicDFS(const int &v, const int &e, int push);

int main(void)
{
	tests = getInt();
	for(int t = 0; t < tests; ++ t)
	{
		verts = getInt(); edges = getInt();
		for(int v = 0; v < 256; ++ v)
			graph[v].clear();

		for(int e = 0; e < 2 * edges; ++ e)
			transferred[e] = least[e] = limit[e] = 0;

		for(int e = 0; e < edges; ++ e)
		{
			start = getInt() - 1; end = getInt() - 1; min = getInt(); max = getInt();
			graph[start].push_back(std::make_pair(end, e));
			graph[end].push_back(std::make_pair(start, $(e)));
			least[e] = min;
			limit[e] = max;
			least[$(e)] = 0;
			limit[$(e)] = 0;
		}

		done = true;
		do
		{
			done = false;
			for(int v = 0; v < verts; ++ v)
				for(unsigned int n = 0; n < graph[v].size(); ++ n)
					if(graph[v][n].second < edges && least[graph[v][n].second] > 0 && (temp = dinic(graph[v][n].first, v, least[graph[v][n].second])))
					{
						done = true;
						least[graph[v][n].second] -= temp;
						limit[graph[v][n].second] -= temp;
						transferred[graph[v][n].second] += temp;
					}
		}
		while(done);

		for(int e = 0; e < edges; ++ e)
			printf("%d\n", transferred[e]);
	}

	return 0;
}

/* DINIC */

inline static int dinic(const int &s, const int &e, int push)
{
	int flow = 0,
		act = 0;

	while(push && dinicBFS(s, e))
	{
		do
		{
			flow += act = dinicDFS(s, e, push);
			push -= act;
		}
		while(push && act);
	}

	return flow;
}

inline static int dinicBFS(const int &s, const int &e)
{
	que = std::queue<short int>();
	bzero(visited, sizeof(visited));
	bzero(latest, sizeof(latest));
	bzero(impossible, sizeof(impossible));
	visited[e] = 1;
	que.push(e);
	while(!que.empty() && !visited[s])
	{
		qAct = que.front();
		que.pop();
		std::pair<int, int> g = graph[qAct][0];
		for(unsigned int n = 0; n < graph[qAct].size(); g = graph[qAct][++ n])
			if(!visited[g.first] && limit[$(g.second)] > 0)
			{
				visited[g.first] = visited[qAct] + 1;
				que.push(g.first);
			}
	}

	return visited[s];

}

inline static int dinicDFS(const int &v, const int &e, int push)
{
	if(v == e)
		return push;

	if(!push || impossible[v])
		return 0;

	int flow = 0,
		act = 0;
	std::pair<int, int> g = graph[v][latest[v]];
	for(unsigned int n = latest[v]; push && n < graph[v].size(); g = graph[v][n = ++ latest[v]])
		if(visited[g.first] + 1 == visited[v] && limit[g.second] > 0 &&
			(act = dinicDFS(g.first, e, MIN(push, limit[g.second]))))
		{
			limit[g.second] -= act;
			limit[$(g.second)] += act;
			least[g.second] -= act;
			least[$(g.second)] += act;
			transferred[g.second] += act;
			transferred[$(g.second)] -= act;
			flow += act;
			push -= act;
		}

	latest[v] -= !push;
	impossible[v] = !flow;
	return flow;
}
