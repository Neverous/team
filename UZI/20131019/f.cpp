/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstring>

int tests,
    digits,
    maxlen;
long long int imax;
char max[32],
     pattern[32];
long long int memory[32][32];

long long int solve(void);
void align(char *a, char *b);
long long int count(int pos, int same);
long long int pow(long long int n, int a){if(!n) return 0; if(!a) return 1; return n * pow(n, a - 1);}


int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        for(int p = 0; p < 32; ++ p)
            for(int s = 0; s < 32; ++ s)
                memory[p][s] = -1;

        scanf("%lld %s %d", &imax, pattern, &digits);
        sprintf(max, "%lld", imax - 1);
        maxlen = strlen(max);
        align(max, pattern);
        printf("%lld\n", solve());
    }

    return 0;
}

long long int solve(void)
{
    long long int result = 0;
    int starting = 0;
    for(int i = 0; max[i]; ++ i)
    {
        for(int d = '0'; d < max[i]; ++ d)
            result += count(i + 1, (d == pattern[i] || starting >= digits) ? starting + 1 : 0);

        if(max[i] == pattern[i])
            ++ starting;

        else if(starting < digits)
            starting = 0;
    }

    return result + (starting >= digits);
}

void align(char *a, char *b)
{
    int alen = strlen(a),
        blen = strlen(b);

    if(alen <= blen)
        return;

    for(int p = blen; p >= 0; -- p)
        b[p + alen - blen] = b[p];

    for(int p = 0; p < alen - blen; ++ p)
        b[p] = '0';
}

long long int count(int pos, int same)
{
    if(memory[pos][same] != -1)
        return memory[pos][same];

    if(pos >= maxlen)
        return memory[pos][same] = same >= digits;

    if(same >= digits)
        return memory[pos][same] = pow(10, maxlen - pos);

    return memory[pos][same] = 9 * count(pos + 1, 0) + count(pos + 1, same + 1);
}
