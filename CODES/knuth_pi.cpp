/* 2009
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<cstdlib>
#include<cstring>

char word[1000001];
int tests,
	pi[1000001];

void kmpPrefix(char *word, int *pi)
{
	pi[0] = pi[1] = 0;
	for(int p = 1, l = 0; word[p]; ++ p)
	{
		while(l && word[p] != word[l])
			l = pi[l-1];

		if(word[p] == word[l])
			++ l;

		pi[p] = l;
	}
	return;
} //KNUTH PI

int main(void)
{
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%s", word);
		kmpPrefix(word, pi);
		for(int p = 0; word[p]; ++ p)
			printf("%d ", pi[p]);

		puts("");
	}
	return 0;
}
