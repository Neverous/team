/* 2012
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>

int rows,
	cols,
	map[1024][1024],
	result;

inline static void bomb(const int &X, const int &Y, const int value)
{
	for(int x = X - 1; x <= X + 1; ++ x)
		for(int y = Y - 1; y <= Y + 1; ++ y)
			map[x][y] -= value;
}

int main(void)
{
	scanf("%d %d", &rows, &cols);
	for(int r = 1; r <= rows; ++ r)
		for(int c = 1; c <= cols; ++ c)
			scanf("%d", &map[r][c]);

	for(int r = 1; r <= rows; ++ r)
		for(int c = 1; c <= cols; ++ c)
			if(map[r][c] > 0)
			{
				result += map[r][c];
				bomb(r + 1, c + 1, map[r][c]);
			}

	printf("%d\n", result);
}
