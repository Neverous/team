#include <cstdio>
#include <algorithm>
#include <cmath>

int tests,
    l[8];

bool checkTriangles(void);
bool checkTriangle(int a, int b, int c);
bool checkVolume(void);

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %d %d %d %d %d", &l[0], &l[1], &l[2], &l[3], &l[4], &l[5]);
        std::sort(l, l + 6);

        bool possible = true;
        do
        {
            possible = checkTriangles() && checkVolume();
        }
        while(!possible && std::next_permutation(l, l + 6));

        puts(possible ? "YES" : "NO");
    }

    return 0;
}

inline
bool checkTriangles(void)
{
    return  checkTriangle(0, 1, 2)
        &&  checkTriangle(2, 3, 4)
        &&  checkTriangle(1, 3, 5)
        &&  checkTriangle(0, 4, 5);
}

inline
bool checkTriangle(int a, int b, int c)
{
    return  l[a] + l[b] > l[c]
        &&  l[a] + l[c] > l[b]
        &&  l[b] + l[c] > l[a];
}
/*
inline
bool checkVolume(void)
{
    // Heron: http://en.wikipedia.org/wiki/Tetrahedron#Heron-type_formula_for_the_volume_of_a_tetrahedron
    double U = l[0],
           V = l[1],
           W = l[2],
           u = l[3],
           v = l[4],
           w = l[5];

    double z = (W - u + v) * (u - v + W),
           Z = (v - W + u) * (W + u + v),
           y = (V - w + u) * (w - u + V),
           Y = (u - V + w) * (V + w + u),
           x = (U - v + w) * (v - w + U),
           X = (w - U + v) * (U + v + w);

    double a = sqrt(x * Y * Z),
           b = sqrt(y * Z * X),
           c = sqrt(z * X * Y),
           d = sqrt(x * y * z);

    return (-a + b + c + d) * (a - b + c + d) * (a + b - c + d) * (a + b + c - d) > 0;
}
*/
inline
bool checkVolume(void)
{
    // Simplex via ŁD: http://www.mathpages.com/home/kmath664/kmath664.htm
    //
    // -------------
    // | 0 1 1 1 1 |
    // | 1 0 a b c |
    // | 1 a 0 d e |
    // | 1 b d 0 f |
    // | 1 c e f 0 |
    // -------------
    //
    // Determinant with help from Wolfram cause i can't count...
    double a = (long long int) l[0] * l[0],
           b = (long long int) l[2] * l[2],
           c = (long long int) l[4] * l[4],
           d = (long long int) l[1] * l[1],
           e = (long long int) l[5] * l[5],
           f = (long long int) l[3] * l[3];

    return ( - a * a * f
             - a * b * d
             + a * b * e
             + a * b * f
             + a * c * d
             - a * c * e
             + a * c * f
             + a * d * f
             + a * e * f
             - a * f * f
             - b * b * e
             + b * c * d
             + b * c * e
             - b * c * f
             + b * d * e
             - b * e * e
             + b * e * f
             - c * c * d
             - c * d * d
             + c * d * e
             + c * d * f
             - d * e * f
            ) > 0;
}
