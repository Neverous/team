/* 2011
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<vector>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int size,
	best;
char map[128][128];
bool visited[128][128];

std::vector<std::pair<int, int> > res;
std::pair<int, std::vector<std::pair<int, int> > > ans;
inline const static std::pair<int, std::vector<std::pair<int, int> > > DFS(const int &h, const int &w);

int main(void)
{
	scanf("%d", &size);
	for(int s = 0; s < size; ++ s)
	{
		scanf("%s", map[s]);
		for(int w = 0; w < size; ++ w)
			visited[s][w] = map[s][w] == '#';
	}

	for(int h = 0; h < size; ++ h)
		for(int w = 0; w < size; ++ w)
			if(!visited[h][w])
			{
				ans = DFS(h, w);
				if(ans.first > best)
				{
					res = ans.second;
					best = ans.first;
				}
			}

	printf("%d\n", best);
	for(int r = 0; r < best; ++ r)
		printf("%d %d\n", res[r].first + 1, res[r].second + 1);

	return 0;
}

inline const static std::pair<int, std::vector<std::pair<int, int> > > DFS(const int &h, const int &w)
{
	visited[h][w] = true;
	std::pair<int, std::vector<std::pair<int, int> > > tmp,
													   Res;
	Res.first = 0;

	if(w - 1 > 0 && !visited[h][w - 1])
	{
		tmp = DFS(h, w - 1);
		if(tmp.first > Res.first)
		{
			Res.first = tmp.first;
			Res.second = tmp.second;
		}
	}

	if(h - 1 > 0 && !visited[h - 1][w])
	{
		tmp = DFS(h - 1, w);
		if(tmp.first > Res.first)
		{
			Res.first = tmp.first;
			Res.second = tmp.second;
		}
	}

	if(h + 1 < size && !visited[h + 1][w])
	{
		tmp = DFS(h + 1, w);
		if(tmp.first > Res.first)
		{
			Res.first = tmp.first;
			Res.second = tmp.second;
		}
	}

	if(w + 1 < size && !visited[h][w + 1])
	{
		tmp = DFS(h, w + 1);
		if(tmp.first > Res.first)
		{
			Res.first = tmp.first;
			Res.second = tmp.second;
		}
	}

	Res.first += 1;
	Res.second.push_back(std::make_pair(h, w));

	//visited[h][w] = false;
	return Res;
}
