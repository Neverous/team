/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<cassert>

int length,
	radius[500050];
long long unsigned int result;
char word[500050];

int main(void)
{
	scanf("%d", &length);
	scanf("%s", word);

	for(int l = 1, p = 0; l < length; ++ l)
	{
		if(p + radius[p] - 1 >= l)
		{
			if(l + radius[2 * p - l] <= p + radius[p])
				radius[l] = radius[2 * p - l];

			else
				radius[l] = p + radius[p] - l;
		}

		while(l - radius[l] - 1 >= 0 && l + radius[l] < length && word[l - radius[l] - 1] != word[l + radius[l]])
			++ radius[l];

		result += radius[l];
		if(radius[l] + l > radius[p] + p)
			p = l;
	}

	printf("%llu\n", result);
	return 0;
}

