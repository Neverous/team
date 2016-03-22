/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstring>

int size,
    distance,
    steps,
    mod,
    matrix[512],
    vector[512];

void multiply(int dst[512], int src[512], const int SIZE, const int MOD);
void fastPower(int base[512], const int SIZE, int power, const int MOD);

int main(void)
{
#ifndef SELENE
    freopen("cell.in", "r", stdin);
    freopen("cell.out", "w", stdout);
#endif // SELENE

    scanf("%d %d %d %d", &size, &mod, &distance, &steps);
    matrix[0] = 1;
    for(int d = 0; d < distance; ++ d)
        matrix[size - 1 - d] = matrix[d + 1] = 1;

    for(int s = 0; s < size; ++ s)
        scanf("%d", &vector[s]);

    fastPower(matrix, size, steps, mod);
    multiply(vector, matrix, size, mod);
    for(int s = 0; s < size; ++ s)
        printf("%d ", vector[s]);

    puts("");
    return 0;
}

inline
void multiply(int dst[512], int src[512], const int SIZE, const int MOD)
{
    int result[512] = {};
    for(int j = 0; j < SIZE; ++ j)
        for(int k = 0; k < SIZE; ++ k)
            result[k] = ((long long int) result[k] + (long long int) dst[j] * src[(SIZE + j - k) % SIZE]) % MOD;

    memcpy(dst, result, SIZE * sizeof(int));
}

inline
void fastPower(int base[512], const int SIZE, int power, const int MOD)
{
    int result[512] = {};
    result[0] = 1;
    while(power > 0)
    {
        if(power % 2)
            multiply(result, base, SIZE, MOD);

        multiply(base, base, SIZE, MOD);
        power /= 2;
    }

    memcpy(base, result, SIZE * sizeof(int));
}
