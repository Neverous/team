/* 2009
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<vector>
#include<algorithm>
using namespace std;

int numbers,
	number[200002],
	result,
	best,
	sortSize;
vector<int> hash[20],
			rev[20],
			temp2,
			resultTab;
vector<pair<pair<int, int>, int> > temp;

inline void preprocess(void);
inline pair<int, int> getHash(int position, int count);
inline void solve(void);
inline bool specialSort(const int &A, const int &B);

int main(void)
{
	scanf("%d", &numbers);
	for(int n = 0; n < numbers; ++ n)
		scanf("%d", &number[n]);

	preprocess();
	solve();
	return 0;
}

inline void preprocess(void)
{
	hash[0].resize(numbers);
	rev[0].resize(numbers);
	for(int h = 0; h < numbers; ++ h)
	{
		hash[0][h] = number[h]; // wszystkie podsłowa jednoliterowe
		rev[0][h] = number[h];
	}

	for(int h = 1; (1 << h) < numbers; ++ h) // podsłowa o rozmiarze 2**h, h > 0
	{
		hash[h].resize(numbers - (1 << (h - 1)));
		rev[h].resize(numbers - (1 << (h - 1)));
		temp.resize(numbers - (1 << (h - 1)));
		for(int p = 0; p < numbers - (1 << (h - 1)); ++ p)
			temp[p] = make_pair(make_pair(hash[h - 1][p], hash[h - 1][p + (1 << (h - 1))]), p);

		sort(temp.begin(), temp.end());
		for(int p = 0, k = 0; p < numbers - (1 << (h - 1)); ++ p)
		{
			if(p && temp[p].first != temp[p - 1].first)
				++ k;

			hash[h][temp[p].second] = k;
		}

		for(int p = 0; p < numbers - (1 << (h - 1)); ++ p)
			temp[p] = make_pair(make_pair(rev[h - 1][p + (1 << (h - 1))], rev[h - 1][p]), p);

		sort(temp.begin(), temp.end());
		for(int p = 0, k = 0; p < numbers - (1 << (h - 1)); ++ p)
		{
			if(p && temp[p].first != temp[p - 1].first)
				++ k;

			rev[h][temp[p].second] = k;
		}

		//for(int p = 0; p < numbers - (1 << (h - 1)); ++ p)
		//	printf("%d:%d ", hash[h][p], rev[h][p]);

		//puts("");
	}
}

inline pair<int, int> getHash(int position, int count)
{
	int log;
	pair<int, int> res;
	for(log = 0; (1 << log) <= count / 2; ++ log);

	res = make_pair(hash[log][position], hash[log][position + count - (1 << log)]);
	if(rev[log][position + count - (1 << log)] < hash[log][position] || (rev[log][position + count - (1 << log)] == hash[log][position] && rev[log][position] < hash[log][position + count - (1 << log)]))
		res = make_pair(rev[log][position + count - (1 << log)], rev[log][position]);

	return res;
}

inline void solve(void)
{
	for(int l = 1; l <= numbers; ++ l)
	{
		result = 0;
		temp2.resize(numbers / l);
		sortSize = l;
		for(int n = 0, p = 0; n < numbers; n += l, ++ p)
			temp2[p] = n;

		sort(temp2.begin(), temp2.end(), specialSort);
		//printf("]]%d\n", l);
		for(int t = 0; t < numbers / l; ++ t)
		{
			//printf("H: %d %d\n", getHash(temp2[t], l).first, getHash(temp2[t], l).second);
			//for(int n = 0; n < l; ++ n)
			//	printf("%d ", number[temp2[t] + n]);

			//puts("");

			if(t && getHash(temp2[t], l) == getHash(temp2[t - 1], l))
				continue;

			++ result;
		}

		if(result > best)
		{
			resultTab.clear();
			best = result;
		}

		if(result == best)
			resultTab.push_back(l);
	}

	printf("%d %d\n", best, (int)resultTab.size());
	for(unsigned int r = 0; r < resultTab.size(); ++ r)
		printf("%d ", resultTab[r]);

	puts("");
}

inline bool specialSort(const int &A, const int &B)
{
	return getHash(A, sortSize) < getHash(B, sortSize);
}
