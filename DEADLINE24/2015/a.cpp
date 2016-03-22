/* 2015
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstring>

long long unsigned int C;
long long unsigned int M;
long long unsigned int x;
int tests;

void matrix3Multiply(__uint128_t dest[3][3], __uint128_t A[3][3], __uint128_t B[3][3], __uint128_t MOD)
{
    __uint128_t result[3][3] = {};
    for(int h = 0; h < 3; ++ h)
        for(int w = 0; w < 3; ++ w)
            for(int k = 0; k < 3; ++ k)
                result[h][w] = (result[h][w] + (A[h][k] * B[k][w]) % MOD) % MOD;

    memcpy(dest, result, sizeof(__uint128_t) * 3 * 3);
}

void matrix3FastPower(__uint128_t base[3][3], __uint128_t power, __uint128_t MOD)
{
    if(power == 0)
    {
        memset(base, 0, sizeof(__uint128_t) * 3 * 3);
        base[0][0] = base[1][1] = base[2][2] = 1;
        return;
    }

    if(power == 1)
        return;

    if(power % 2)
    {
        __uint128_t matrix[3][3];
        memcpy(matrix, base, sizeof(__uint128_t) * 3 * 3);
        matrix3FastPower(matrix, power - 1, MOD);
        matrix3Multiply(base, matrix, base, MOD);
        return;
    }

    matrix3FastPower(base, power / 2, MOD);
    matrix3Multiply(base, base, base, MOD);
}

__uint128_t solve(__uint128_t n)
{
    if(n == 1)
        return 1;

    if(n == 2)
        return 2 % M;

    __uint128_t matrix[3][3] = {
        {0, 1, 0},
        {1, C, 0},
        {1, C, 1},
    };

    matrix3FastPower(matrix, n - 2, M);
    return ((matrix[2][0] + matrix[2][1]) % M + (matrix[2][2] * 2) % M) % M;
}

int main(void)
{
    scanf("%llu %llu\n", &C, &M);
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%llu", &x);
        printf("%llu\n", (long long unsigned int) (solve(x) % M));
    }

    return 0;
}
