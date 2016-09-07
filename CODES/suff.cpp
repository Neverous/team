/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<algorithm>

char word[1000001];
int sortMe[1000001],
	size,
	id[21][1000001],
	logarithm[1000001],
	power[21];
std::pair<long long int, int> elements[1000001];

struct kmrCompare
{
	inline const bool operator() (const int &A, const int &B) const
	{
		int minLength = size - ((A > B)?A:B),
			log = logarithm[minLength];

		if(id[log][A] == id[log][B] && id[log][A + minLength - power[log]] == id[log][B + minLength - power[log]])
			return B < A;

		if(id[log][A] == id[log][B])
			return id[log][A + minLength - power[log]] < id[log][B + minLength - power[log]];

		return id[log][A] < id[log][B];
	}
};

void kmrPreprocess(void);

int main(void)
{
	power[0] = 1;
	for(int i = 2, n = 0; i < 1000001; ++ i)
	{
		if((i & (i - 1)) == 0)
			++ n;

		logarithm[i] = n;
	}

	for(int p = 1, i = 0; i < 21; ++ i, p *= 2)
		power[i] = p;

	scanf("%s", word);
	for(size = 0; word[size]; ++ size)
		sortMe[size] = size;

	kmrPreprocess();
	std::sort(sortMe, sortMe + size, kmrCompare());
	for(size = 0; word[size]; ++ size)
		printf("%d%c", sortMe[size], word[size + 1]?' ':'\n');

	return 0;
}

void kmrPreprocess(void)
{
	/* First level */
	for(int s = 0; s < size; ++ s)
		id[0][s] = word[s];

	/* Next levels */
	for(int l = 1; power[l] <= size; ++ l)
	{
		for(int p = 0; p < size - power[l - 1]; ++ p)
			elements[p] = std::make_pair((long long int)id[l - 1][p] * 1000000 + id[l - 1][p + power[l - 1]], p);

		std::sort(elements, elements + size - power[l - 1]);
		for(int p = 0, n = 0; p < size - power[l - 1]; ++ p)
		{
			if(p && elements[p].first != elements[p - 1].first)
				++ n;

			id[l][elements[p].second] = n;
		}
	}
}

