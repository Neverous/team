/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<vector>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

using namespace std;

int width,
	height,
	blocks,
	count[40],
	size[40],
	rest;
bool added;
char repr[8][8];
bool table[100][100];

vector<pair<int, int> > block[40][4];

bool checkPosition(int b, int r, int H, int W);
int markPosition(int b, int r, int H, int W);

int main(void)
{
	scanf("%d %d %d", &width, &height, &blocks);
	for(int b = 0; b < blocks; ++ b)
	{
		scanf("%d", &count[b]);
		for(int r = 0; r < 5; ++ r)
			scanf("%s", repr[r]);

		for(int h = 0; h < 5; ++ h)
			for(int w = 0; w < 5; ++ w)
				if(repr[h][w] == 'x')
				{
					block[b][0].push_back(make_pair(h - 2, w - 2));
					block[b][1].push_back(make_pair(w - 2, -h + 2));
					block[b][2].push_back(make_pair(-h + 2, -w + 2));
					block[b][3].push_back(make_pair(-w + 2, h - 2));
					++ size[b];
				}

		DEBUG("\n%d: %d\n", b, size[b]);
	}

	rest = width * height;
	for(int b = 0; rest && b < blocks; ++ b)
	{
		added = true;
		for(int c = 0; rest && added && c < count[b];)
		{
			added = false;
			for(int h = 0; rest && h < height && c < count[b]; ++ h)
				for(int w = 0; rest && w < width && c < count[b]; ++ w)
					for(int r = 0; r < 4; ++ r)
					if(checkPosition(b, r, h, w))
					{
						rest -= markPosition(b,r, h, w);
						printf("%d %d %d %d\n", b + 1, r * 90, w + 1, h + 1);
						++ c;
						added = true;
					}
		}
	}

	printf("0 0 0 0\n");
	DEBUG("%d %d\n", rest, width * height);
	return 0;
}

bool checkPosition(int b, int r, int H, int W)
{
	for(unsigned int p = 0; p < block[b][r].size(); ++ p)
	{
		if(H + block[b][r][p].first < 0 ||
			W + block[b][r][p].second < 0 ||
			H + block[b][r][p].first >= height ||
			W + block[b][r][p].second >= width)
			return false;

		if(table[H + block[b][r][p].first][W + block[b][r][p].second])
			return false;
	}

	return true;
}

int markPosition(int b, int r, int H, int W)
{
	for(unsigned int p = 0; p < block[b][r].size(); ++ p)
		table[H + block[b][r][p].first][W + block[b][r][p].second] = true;

	DEBUG("%d: %d\n", b, size[b]);
	for(int h = 0; h < height; ++ h)
	{
		for(int w = 0; w < width; ++ w)
			DEBUG("%2d", table[h][w]);

		DEBUG("\n");

	}

	return size[b];
}
