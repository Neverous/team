/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstring>

int tests;
char first[131072],
     second[131072];

char repr[256];

int main(void)
{
    repr[(int) 'A'] = repr[(int) 'B'] = repr[(int) 'C'] = 'A';
    repr[(int) 'D'] = repr[(int) 'E'] = repr[(int) 'F'] = 'D';
    repr[(int) 'G'] = repr[(int) 'H'] = repr[(int) 'I'] = 'G';
    repr[(int) 'J'] = repr[(int) 'K'] = repr[(int) 'L'] = 'J';
    repr[(int) 'M'] = repr[(int) 'N'] = repr[(int) 'O'] = 'M';
    repr[(int) 'P'] = repr[(int) 'Q'] = repr[(int) 'R'] = repr[(int) 'S'] = 'P';
    repr[(int) 'T'] = repr[(int) 'U'] = repr[(int) 'V'] = 'T';
    repr[(int) 'W'] = repr[(int) 'X'] = repr[(int) 'Y'] = repr[(int) 'Z'] = 'W';
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%s %s", first, second);
        for(int f = 0; first[f]; ++ f)
            first[f] = repr[(int) first[f]];

        for(int s = 0; second[s]; ++ s)
            second[s] = repr[(int) second[s]];

        puts(strcmp(first, second) ? "NIE" : "TAK");
    }
}
