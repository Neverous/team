#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

int tests,
    points;
long double x[1024],
            y[1024];
const long double EPS = 0.0000000001;

bool point_equal(long double x_a, long double y_a, long double x_b, long double y_b);
bool find_possible_x(long double y_, long double center_x, long double center_y, long double radius, long double &x_1, long double &x_2);
bool circle(long double x_a, long double y_a, long double x_b, long double y_b, long double x_c, long double y_c, long double &center_x, long double &center_y, long double &radius_2);
bool test(int x_a, int y_a, int x_b, int y_b, int x_c, int y_c);
bool solve(void);

int main(void)
{
    srand(time(NULL));
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d", &points);
        for(int p = 0; p < points; ++ p)
            scanf("%Lf", &x[p]);

        for(int p = 0; p < points; ++ p)
            scanf("%Lf", &y[p]);

        puts(solve() ? "YES" : "NO");
    }

    return 0;
}

inline
bool solve(void)
{
    std::sort(x, x + points);
    for(int T = 0; T < 1; ++ T)
    {
    int x_a = 0,
        x_b = (points - 1) / 2,
        x_c = points - 1;

    for(int y_a = 0; y_a < points; ++ y_a)
        for(int y_b = 0; y_b < points; ++ y_b)
            if(y_b != y_a) for(int y_c = 0; y_c < points; ++ y_c)
                if(y_c != y_a && y_c != y_b && test(x_a, y_a, x_b, y_b, x_c, y_c))
                    return true;
    }

    return false;
}

inline
bool test(int x_a, int y_a, int x_b, int y_b, int x_c, int y_c)
{
    bool x_used[1024] = {};
    long double center_x = 0.0L,
                center_y = 0.0L,
                radius_2 = 0.0L,
                x_1 = 0.0L, x_2 = 0.0L;

    x_used[x_a] = x_used[x_b] = x_used[x_c] = true;
    if(!circle(x[x_a], y[y_a], x[x_b], y[y_b], x[x_c], y[y_c], center_x, center_y, radius_2))
        return false;

    fprintf(stderr, "Circle (%.2Lf, %.2Lf) r=%.2Lf\n", center_x, center_y, sqrtl(radius_2));
    for(int p = 0; p < points; ++ p)
    {
        if(p == y_a || p == y_b || p == y_c)
            continue;

        if(!find_possible_x(y[p], center_x, center_y, radius_2, x_1, x_2))
        {
            fprintf(stderr, "INVALID\n");
            return false;
        }

        int idx = std::lower_bound(x, x + points, x_1 - EPS) - x;
        fprintf(stderr, "LB(%.2Lf): %d %.2Lf\n", x_1, idx, x[idx]);
        while(idx < points && x_used[idx] && std::abs(x_1 - x[idx + 1]) < EPS)
            ++ idx;

        while(idx > 0 && x_used[idx] && std::abs(x_1 - x[idx - 1]) < EPS)
            -- idx;

        fprintf(stderr, "END(%.2Lf): %d %.2Lf\n", x_1, idx, x[idx]);
        if(!x_used[idx] && std::abs(x_1 - x[idx]) < EPS)
        {
            x_used[idx] = true;
            continue;
        }

        idx = std::lower_bound(x, x + points, x_2 - EPS) - x;
        fprintf(stderr, "LB(%.2Lf): %d %.2Lf\n", x_2, idx, x[idx]);
        while(idx < points && x_used[idx] && std::abs(x_2 - x[idx + 1]) < EPS)
            ++ idx;

        while(idx > 0 && x_used[idx] && std::abs(x_2 - x[idx - 1]) < EPS)
            -- idx;

        fprintf(stderr, "END(%.2Lf): %d %.2Lf\n", x_2, idx, x[idx]);
        if(!x_used[idx] && std::abs(x_2 - x[idx]) < EPS)
        {
            x_used[idx] = true;
            continue;
        }

        fprintf(stderr, "ZUPA\n");
        return false;
    }

    fprintf(stderr, "BULLSEYE\n");
    return true;
}

inline
bool find_possible_x(long double y_, long double center_x, long double center_y, long double radius_2, long double &x_1, long double &x_2)
{
    long double a = 1.0L;
    long double b = -2.0L * center_x;
    long double c = center_x * center_x + (y_ - center_y) * (y_ - center_y) - radius_2;
    long double delta = b * b - 4.0L * a * c;
    if(delta < 0)
        return false;

    x_1 = (-b - sqrtl(delta)) / (2.0L * a);
    x_2 = (-b + sqrtl(delta)) / (2.0L * a);
    return true;
}

inline
bool circle(long double x_a, long double y_a, long double x_b, long double y_b, long double x_c, long double y_c, long double &center_x, long double &center_y, long double &radius_2)
{
    if(point_equal(x_a, y_a, x_b, y_b) || point_equal(x_a, y_a, x_c, y_c) || point_equal(x_b, y_b, x_c, y_c))
        return false;

    long double m_a = (y_b - y_a) / (x_b - x_a);
    long double m_b = (y_c - y_b) / (x_c - x_b);
    if(std::abs(m_b - m_a) < EPS)
        return false;

    center_x = m_a * m_b * (y_a - y_c) + m_b * (x_a + x_b) - m_a * (x_b + x_c);
    center_x /= 2.0L * (m_b - m_a);
    center_y = -1.0L / m_b * (center_x - (x_b + x_c) / 2.0L) + (y_b + y_c) / 2.0L;

    radius_2 = (center_x - x_a) * (center_x - x_a) + (center_y - y_a) * (center_y - y_a);
    return true;
}

inline
bool point_equal(long double x_a, long double y_a, long double x_b, long double y_b)
{
    return std::abs(x_a - x_b) < EPS && std::abs(y_a - y_b) < EPS;
}
