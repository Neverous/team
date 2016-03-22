/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

char first[128],
     second[128];
int firstlen,
    secondlen;

int check(char *prefix, int prefixlen, char *suffix, int suffixlen);

int main(void)
{
#ifndef SELENE
    freopen("kickdown.in", "r", stdin);
    freopen("kickdown.out", "w", stdout);
#endif // SELENE

    scanf("%s %s", first, second);
    firstlen    = strlen(first);
    secondlen   = strlen(second);
    printf("%d\n", firstlen + secondlen - std::max(check(first, firstlen, second, secondlen), check(second, secondlen, first, firstlen)));
    return 0;
}

inline
int check(char *prefix, int prefixlen, char *suffix, int suffixlen)
{
    int result = 0;
    for(int p = prefixlen; p >= 0; -- p)
    {
        int s = 0;
        while(s < suffixlen && p + s < prefixlen && (prefix[p + s] != '2' || suffix[s] != '2'))
            ++ s;

        if(s == suffixlen || p + s == prefixlen)
            result = std::max(result, s);
    }

    return result;
}
