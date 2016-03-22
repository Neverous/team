/* 2011
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<vector>
#include<queue>
#include<cstring>
#include<algorithm>
#define source (currencies * 2)
#define sink (currencies * 2 + 1)

using namespace std;

int currencies,
	exchanges,
	from,
	to;
long double starting,
	 		rate,
			maks,
			weight[2048][256][256],
			exchange[2048];

int visited[256];
std::vector<pair<int, int> > graph[256];

inline static long double MIN(const long double &A, const long double &B){return A<B?A:B;}

/* Dinic algorithm - I suppose */
std::queue<int> que;
int qAct,
	latest[256];
bool impossible[256];
inline static long double dinic(const int &start, const int &end);
inline static int dinicBFS(const int &start, const int &end);
inline static long double dinicDFS(const int &v, const int &end, long double push);

int main(void)
{
	scanf("%d %d %Lf", &currencies, &exchanges, &starting);
	for(int e = 0; e < exchanges; ++ e)
	{
		scanf("%d %d %Lf %Lf", &from, &to, &rate, &maks);
		graph[from].push_back(make_pair(to + currencies + 1, e));
		graph[to + currencies + 1].push_back(make_pair(from, e));
		weight[e][from][to + currencies + 1] = maks;
		weight[e][to + currencies + 1][from] = 0;
		exchange[e] = rate;
	}

	graph[source].push_back(make_pair(0, exchanges));
	graph[0].push_back(make_pair(source, exchanges));
	weight[exchanges][source][0] = starting;
	weight[exchanges][0][source] = 0;
	exchange[exchanges] = 1;

	graph[sink].push_back(make_pair(currencies + 1, exchanges + 1));
	graph[currencies + 1].push_back(make_pair(sink, exchanges + 1));
	weight[exchanges][sink][currencies + 1] = (1 << 31);
	weight[exchanges][currencies + 1][sink] = 0;
	exchange[exchanges + 1] = 1;

	printf("%Lf\n", dinic(source, sink) - starting);
	return 0;
}

/* Dinic algorithm - I suppose */
inline static long double dinic(const int &start, const int &end)
{
	long double flow = 0,
		 		act = 0;

	while(dinicBFS(start, end))
	{
		do
		{
			flow += act = dinicDFS(start, end, (1 << 31));
		}
		while(act > 0.00001);
	}

	return flow;
}

inline static int dinicBFS(const int &start, const int &end)
{
	que = std::queue<int>();
	bzero(visited, sizeof(visited));
	bzero(latest, sizeof(latest));
	bzero(impossible, sizeof(impossible));
	visited[end] = 1;
	que.push(end);
	while(!que.empty() && !visited[start])
	{
		qAct = que.front();
		que.pop();
		for(unsigned int n = 0; n < graph[qAct].size(); ++ n)
		{
			pair<int, int> g = graph[qAct][n];
			if(!visited[g.first] && weight[g.second][g.first][qAct] > 0)
			{
				visited[g.first] = visited[qAct] + 1;
				que.push(g.first);
			}
		}
	}

	return visited[start];

}

inline static long double dinicDFS(const int &v, const int &end, long double push)
{
	if(v == end)
		return push;

	if(!push || impossible[v])
		return 0;

	long double flow = 0,
		act = 0;
	for(unsigned int n = latest[v]; push > 0.00001 && n < graph[v].size(); n = ++ latest[v])
	{
		pair<int, int> g = graph[v][n];
		if(visited[g.first] + 1 == visited[v] && weight[g.second][v][g.first] > 0 &&
			(act = dinicDFS(g.first, end, MIN(push, weight[g.second][v][g.first]))))
		{
			weight[g.second][v][g.first] -= act;
			weight[g.second][g.first][v] += act;
			flow += act;
			push -= act;
		}
	}

	latest[v] -= !push;
	impossible[v] = !flow;
	return flow;
}
