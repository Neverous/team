/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>

char text[131072],
     *word,
     mapping[32];
bool paired[32][32],
     used[32];

bool check(int a, int e, int i, int o, int u);
void printmap(void);

int main(void)
{
#ifndef SELENE
    freopen("javanese.in", "r", stdin);
    freopen("javanese.out", "w", stdout);
#endif // SELENE

    word = text;
    while(scanf("%s", word) != -1)
    {
        while(*word)
            ++ word;

        *word ++ = ' ';
        *word = 0;
    }

    for(int t = 1; text[t]; ++ t)
        if(text[t-1] != ' ' && text[t] != ' ')
            paired[text[t-1]-'A'][text[t]-'A'] = paired[text[t]-'A'][text[t-1]-'A'] = true;

    used['A' - 'A'] = used['E' - 'A'] = used['I' - 'A'] = used['O' - 'A'] = used['U' - 'A'] = true;
    for(int a = 0; a < 26; ++ a)
        for(int e = a + 1; e < 26; ++ e)
            for(int i = e + 1; i < 26; ++ i)
                for(int o = i + 1; o < 26; ++ o)
                    for(int u = o + 1; u < 26; ++ u)
                        if(check(a, e, i, o, u))
                        {
                            printmap();
                            return 0;
                        }

    puts("impossible");
    return 0;
}

inline
bool check(int a, int e, int i, int o, int u)
{
    mapping[a] = 'A';
    mapping[e] = 'E';
    mapping[i] = 'I';
    mapping[o] = 'O';
    mapping[u] = 'U';

    for(int f = 0; f < 26; ++ f)
        for(int s = 0; s < 26; ++ s)
            if(!mapping[f] == !mapping[s] && paired[f][s])
            {
                mapping[a] = mapping[e] = mapping[i] = mapping[o] = mapping[u] = 0;
                return false;
            }

    return true;
}

inline
void printmap(void)
{
    int usecount = 0;
    for(int t = 0; text[t]; ++ t)
    {
        if(text[t] == ' ')
        {
            putchar(' ');
            continue;
        }

        if(!mapping[text[t] - 'A'])
        {
            while(used[usecount])
                ++ usecount;

            mapping[text[t] - 'A'] = usecount + 'A';
            used[usecount] = true;
        }

        putchar(mapping[text[t] - 'A']);
    }
}
