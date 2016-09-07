/* 2011
 * Maciej Szeptuch
 * XIV LO Wrocław
 * ----------
 *  MAXFLOWS
 */
#include<cstdio>
#include<algorithm>
#include<vector>
#include<queue>
#include<cstring>

int verts,
	edges,
	from,
	to,
	speed;
int weight[1024][1024];
short int visited[1024];
std::vector<short int> graph[1024];

inline static int MIN(const int &A, const int &B){return A<B?A:B;}

/* Ford-Fulkerson algorithm written with DFS *//*
int flag;
inline static int fordFulkerson(const int &start, const int &end);
inline static int fordFulkersonDFS(const int &v, const int &end, int push, const int &color);*/

/* Ford-Fulkerson algorithm written with BFS *//*
std::queue<std::pair<short int, int> > que;
std::pair<short int, int> qAct;
inline static int fordFulkerson2(const int &start, const int &end);
inline static int fordFulkersonBFS(const int &v, const int &end, int push);*/

/* Dinic algorithm - I suppose */
std::queue<short int> que;
short int qAct,
		  latest[1024];
bool impossible[1024];
inline static int dinic(const int &start, const int &end);
inline static int dinicBFS(const int &start, const int &end);
inline static int dinicDFS(const int &v, const int &end, int push);

int main(void)
{
	scanf("%d %d", &verts, &edges);
	for(int v = 0; v < verts; ++ v)
	{
		for(int v2 = 0; v2 < verts; ++ v2)
			weight[v][v2] = 0;

		graph[v].clear();
		visited[v] = 0;
	}

	for(int e = 0; e < edges; ++ e)
	{
		scanf("%d %d %d", &from, &to, &speed);
		-- from;
		-- to;
		if(!weight[from][to])
		{
			graph[from].push_back(to);
			graph[to].push_back(from);
		}

		weight[from][to] += speed;
	}

	/* TO AVOID BAD CASES */
	for(int v = 0; v < verts; ++ v)
		std::random_shuffle(graph[v].begin(), graph[v].end());

	scanf("%d %d", &from, &to);
	-- from;
	-- to;

	//printf("%d\n", fordFulkerson(from, to));
	//printf("%d\n", fordFulkerson2(from, to));
	printf("%d\n", dinic(from, to));
	return 0;
}

/* Ford-Fulkerson algorithm written with DFS *//*
inline static int fordFulkerson(const int &start, const int &end)
{
	int flow = 0,
		act = 0;

	do
	{
		flow += act = fordFulkersonDFS(start, end, (1 << 30), ++ flag);
	}
	while(act);

	return flow;
}

inline static int fordFulkersonDFS(const int &v, const int &end, int push, const int &color)
{
	if(v == end)
		return push;

	if(visited[v] == color || !push)
		return 0;

	visited[v] = color;
	int flow = 0,
		act = 0;
	for(unsigned int n = 0, g = graph[v][n]; push && n < graph[v].size(); g = graph[v][++ n])
		if(visited[g] != color && weight[v][g] > 0 &&
			(act = fordFulkersonDFS(g, end, MIN(push, weight[v][g]), color)))
		{
			weight[v][g] -= act;
			weight[g][v] += act;
			flow += act;
			push -= act;
		}

	return flow;
}*/

/* Ford-Fulkerson algorithm written with BFS *//*
inline static int fordFulkerson2(const int &start, const int &end)
{
	int flow = 0,
		act = 0;

	do
	{
		flow += act = fordFulkersonBFS(start, end, (1 << 30));
	}
	while(act);

	return flow;
}

inline static int fordFulkersonBFS(const int &v, const int &end, int push)
{
	que = std::queue<std::pair<short int, int> >();
	que.push(std::make_pair(v, push));
	bzero(visited, sizeof(visited));
	visited[v] = v + 1;
	while(!que.empty())
	{
		qAct = que.front();
		que.pop();

		if(qAct.first == end)
			break;

		for(unsigned int n = 0, g = graph[qAct.first][n]; n < graph[qAct.first].size(); g = graph[qAct.first][++ n])
			if(!visited[g] && weight[qAct.first][g] > 0)
			{
				visited[g] = qAct.first + 1;
				que.push(std::make_pair(g, MIN(qAct.second, weight[qAct.first][g])));
			}
	}

	if(qAct.first != end || !qAct.second)
		return 0;

	while(qAct.first != v)
	{
		weight[visited[qAct.first] - 1][qAct.first] -= qAct.second;
		weight[qAct.first][visited[qAct.first] - 1] += qAct.second;
		qAct.first = visited[qAct.first] - 1;
	}

	return qAct.second;
}*/

/* Dinic algorithm - I suppose */
inline static int dinic(const int &start, const int &end)
{
	int flow = 0,
		act = 0;

	while(dinicBFS(start, end))
	{
		do
		{
			flow += act = dinicDFS(start, end, (1 << 30));
		}
		while(act);
	}

	return flow;
}

inline static int dinicBFS(const int &start, const int &end)
{
	que = std::queue<short int>();
	bzero(visited, sizeof(visited));
	bzero(latest, sizeof(latest));
	bzero(impossible, sizeof(impossible));
	visited[end] = 1;
	que.push(end);
	while(!que.empty() && !visited[start])
	{
		qAct = que.front();
		que.pop();
		for(unsigned int n = 0, g = graph[qAct][n]; n < graph[qAct].size(); g = graph[qAct][++ n])
			if(!visited[g] && weight[g][qAct] > 0)
			{
				visited[g] = visited[qAct] + 1;
				que.push(g);
			}
	}

	return visited[start];

}

inline static int dinicDFS(const int &v, const int &end, int push)
{
	if(v == end)
		return push;

	if(!push || impossible[v])
		return 0;

	int flow = 0,
		act = 0;
	for(unsigned int n = latest[v], g = graph[v][n]; push && n < graph[v].size(); g = graph[v][n = ++ latest[v]])
		if(visited[g] + 1 == visited[v] && weight[v][g] > 0 &&
			(act = dinicDFS(g, end, MIN(push, weight[v][g]))))
		{
			weight[v][g] -= act;
			weight[g][v] += act;
			flow += act;
			push -= act;
		}

	latest[v] -= !push;
	impossible[v] = !flow;
	return flow;
}
