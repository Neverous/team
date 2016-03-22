/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>

char identifier[512],
     output[512];

bool error,
     upper,
     is_java,
     is_cpp;

int main(void)
{
#ifndef SELENE
    freopen("java_c.in", "r", stdin);
    freopen("java_c.out", "w", stdout);
#endif // SELENE

    scanf("%s", identifier);
    error = identifier[0] < 'a' || identifier[0] > 'z';
    for(int i = 0, o = 0; !error && identifier[i]; ++ i)
    {
        if('A' <= identifier[i] && identifier[i] <= 'Z')
        {
            error |= is_cpp;
            is_java = true;

            output[o ++] = '_';
            output[o ++] = identifier[i] - 'A' + 'a';
        }

        else if(identifier[i] == '_')
        {
            error |= is_java || upper;
            is_cpp = true;
            upper = true;
        }

        else
        {
            output[o ++] = identifier[i] - (upper ? 'a' - 'A' : 0);
            upper = false;
        }

        output[o] = 0;
    }

    if(error || upper)
        puts("Error!");

    else
        puts(output);

    return 0;
}
