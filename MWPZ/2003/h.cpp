/* 2011
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>
#include<queue>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int tests,
	killstreak,
	residents,
	height,
	age[131072],
	growth[131072],
	passed[131072];

std::priority_queue<std::pair<int, int> > que;
std::pair<int, int> act;

inline static const int MAX(const int &a, const int &b){return a>b?a:b;}
inline static const int MIN(const int &a, const int &b){return a<b?a:b;}

int main(void)
{
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%d %d", &killstreak, &residents);
		que = std::priority_queue<std::pair<int, int> >();
		for(int r = 0; r < residents; ++ r)
		{
			scanf("%d %d %d", &height, &age[r], &growth[r]);
			passed[r] = 0;
			que.push(std::make_pair(-height, r));
		}

		for(int pass = 0; pass < 21; ++ pass)
		{
			while(passed[(act = que.top()).second] < pass)
			{
				que.pop();
				act.first -= growth[act.second] * MIN(pass - passed[act.second], MAX(20 - age[act.second] - passed[act.second], 0));
				passed[act.second] = pass;
				que.push(act);
			}

			printf("%d: %d\n", pass, -que.top().first + killstreak * killstreak * 5);
		}
	}
	return 0;
}

