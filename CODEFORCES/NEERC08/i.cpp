/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>

char commonType[256],
     variableName[256],
     specificType[256];

int main(void)
{
#ifndef SELENE
    freopen("isharp.in", "r", stdin);
    freopen("isharp.out", "w", stdout);
#endif // SELENE

    scanf("%s ", commonType);
    while(scanf("%[a-zA-Z]%s ", variableName, specificType) != -1)
    {
        printf("%s", commonType);
        int t = 0; while(specificType[1 + t ++]);
        -- t;
        while(-- t >= 0)
        {
            if(specificType[t] == ']')
                putchar('[');

            else if(specificType[t] == '[')
                putchar(']');

            else
                putchar(specificType[t]);
        }

        specificType[0] = 0;
        printf(" %s;\n", variableName);
    }

    return 0;
}
