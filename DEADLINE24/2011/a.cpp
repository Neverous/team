/* 2011
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<string>
#include<algorithm>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

const int INFTY = 1024;
int words,
	cost[512][512];
std::string word[512];
char buffer[512];
int visited[512],
	perm[512];
int best,
	bestLen = 512 * 512,
	len;

inline const int MIN(const int &a, const int &b){return a<b?a:b;}
inline bool check(const std::string &A, const std::string &B, const int &length);
inline int DFS(const int &vert, const int &flag, const int &last,const bool &print = false);

int main(void)
{
	scanf("%d", &words);
	for(int w = 0; w < words; ++ w)
	{
		scanf("%s", buffer);
		word[w] = buffer;
		perm[w] = w;
	}

	for(int k = 0; k < 200; ++ k)
	for(int w = 0; w < words; ++ w)
		for(int b = 0; b < words; ++ b)
			if(b != w && word[w].find(word[b]) != std::string::npos)
				std::swap(word[b], word[-- words]);

	for(int s = 0; s < words; ++ s)
		for(int e = 0; e < words; ++ e)
		{
			cost[s][e] = word[e].length();
			for(int l = MIN(word[s].length(), word[e].length()); l > 0; -- l)
				if(check(word[s], word[e], l))
				{
					cost[s][e] = word[e].length() - l;
					break;
				}
		}

	srand(time(0));
	std::random_shuffle(perm, perm + words);
	for(int b = 0; b < words; ++ b)
	{
		len = word[b].length() + DFS(b, b + 1, b);
		if(len < bestLen)
		{
			bestLen = len;
			best = b;
		}
	}

	printf("%d\n%s", bestLen, word[best].c_str());
	DFS(best, 1024, best, true);
	return 0;
}

inline bool check(const std::string &A, const std::string &B, const int &length)
{
	for(int f = A.length() - length, fs = A.length(), s = 0; f < fs; ++ f, ++ s)
		if(A[f] != B[s])
			return false;

	return true;
}

inline int DFS(const int &vert, const int &flag, const int &last,const bool &print)
{
	visited[vert] = flag;
	if(print)
		for(int p = word[vert].length() - cost[last][vert], k = word[vert].length(); p < k; ++ p)
			putchar(word[vert][p]);

	int bestAdd = 1024,
		bbest = -1;
	for(int n = perm[0], w = 0; w < words; n = perm[++ w])
		if(visited[n] != flag && bestAdd >= cost[vert][n])
		{
			bbest = n;
			bestAdd = cost[vert][n];
		}

	if(bbest == -1)
		return 0;

	return bestAdd + DFS(bbest, flag, vert, print);
}
