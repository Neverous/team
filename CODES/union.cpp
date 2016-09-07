/* 2009
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>

int verts,
	edges,
	questions,
	from,
	to,
	action,
	unionTab[300030];

bool unionUnion(int &start, int &end);
int unionFind(int &vert);

int main(void)
{
	for(int u = 0; u < 300030; ++ u)
		unionTab[u] = u;

	scanf("%d %d %d", &verts, &edges, &questions);
	for(int e = 0; e < edges; ++ e)
	{
		scanf("%d %d", &from, &to);
		unionUnion(from, to);
	}

	for(int q = 0; q < questions; ++ q)
	{
		scanf("%d %d %d", &action, &from, &to);
		if(action)
			unionUnion(from, to);

		else
			printf("%s\n", unionFind(from) == unionFind(to)?"TAK":"NIE");
	}

	return 0;
}

bool unionUnion(int &start, int &end)
{
	int s = unionFind(start),
		e = unionFind(end);

	if(s == e)
		return true;

	if(s < e)
		unionTab[s] = e;

	else
		unionTab[e] = s;

	return false;
}

int unionFind(int &vert)
{
	if(vert != unionTab[vert])
		unionTab[vert] = unionFind(unionTab[vert]);

	return unionTab[vert];
}
