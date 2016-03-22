/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>

typedef std::pair<int, std::pair<int, int> > Point;

int points,
    max,
    tests,
    left,
    leftpoints,
    leftpoint[1024];
Point point[1024];
Point center;

bool less(const Point &a, const Point &b);
long long int eval_max();
long long int eval_min();

int main(void)
{
    srand(time(NULL));
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d %d", &points, &left);
        leftpoints = points;
        for(int p = 0; p < points; ++ p)
        {
            scanf("%d %d %d", &point[p].second.second, &point[p].first, &point[p].second.first);
            center.first += point[p].first;
            center.second.first += point[p].second.first;
            leftpoint[p] = p;
        }

        center.first /= points;
        center.second.first /= points;

        std::sort(point, point + points, less);

        leftpoints = 0;
        for(int p = 0, out = 0; p < points; ++ p)
        {
            if(out < left && rand() % 3 == 2)
            {
                ++ out;
                continue;
            }

            leftpoint[leftpoints ++] = leftpoint[p];
        }

        printf("%d ", points);
        for(int p = 0; p < points; ++ p)
            printf("%d ", point[p].second.second);

        puts("");
        long long int max_area = eval_max();
        long long int min_area = eval_min();
        if(min_area < max_area)
        {
            printf("%d ", leftpoints);
            for(int l = 0; l < leftpoints; ++ l)
                printf("%d ", point[leftpoint[l]].second.second);

            puts("");
            printf("%lld\n", max_area - min_area);
        }

        else
        {
            printf("%d ", points);
            for(int p = 0; p < points; ++ p)
                printf("%d ", point[p].second.second);

            puts("");
            puts("0");
        }
    }

    return 0;
}

bool less(const Point &a, const Point &b)
{
    if (a.first - center.first >= 0 && b.first - center.first < 0)
        return true;
    if (a.first - center.first < 0 && b.first - center.first >= 0)
        return false;
    if (a.first - center.first == 0 && b.first - center.first == 0) {
        if (a.second.first - center.second.first >= 0 || b.second.first - center.second.first >= 0)
            return a.second.first > b.second.first;
        return b.second.first > a.second.first;
    }

    // compute the cross product of vectors (center -> a) x (center -> b)
    int det = (a.first - center.first) * (b.second.first - center.second.first) - (b.first - center.first) * (a.second.first - center.second.first);
    if (det < 0)
        return true;
    if (det > 0)
        return false;

    // points a and b are on the same line from the center
    // check which point is closer to the center
    int d1 = (a.first - center.first) * (a.first - center.first) + (a.second.first - center.second.first) * (a.second.first - center.second.first);
    int d2 = (b.first - center.first) * (b.first - center.first) + (b.second.first - center.second.first) * (b.second.first - center.second.first);
    return d1 < d2;
}

long long int eval_max()
{
    long long int suma = 0;
    for(int p = 0; p < points; ++ p)
    {
        int a = p;
        int b = (p + 1) % points;
        int c = (points + p - 1) % points;

        suma += 10 * (long long int) point[a].first * (point[b].second.first - point[c].second.first);
    }

    suma /= 2;
    suma = std::abs(suma);
    return suma;
}

long long int eval_min()
{
    long long int suma = 0;
    for(int p = 0; p < leftpoints; ++ p)
    {
        int a = p;
        int b = (p + 1) % leftpoints;
        int c = (leftpoints + p - 1) % leftpoints;

        suma += 10 * (long long int) point[leftpoint[a]].first * (point[leftpoint[b]].second.first - point[leftpoint[c]].second.first);
    }

    suma /= 2;
    suma = std::abs(suma);
    return suma;
}
