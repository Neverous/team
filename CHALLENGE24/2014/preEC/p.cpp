/* 2014
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>
#include <cmath>
#include <vector>

using namespace std;

int points;
pair<double, double> point[1049576];
double result;

double distance(pair<double, double> &from, pair<double, double> &to);

int main(void)
{
    scanf("%d", &points);
    for(int p = 0; p < points; ++ p)
        scanf("%lf %lf", &point[p].first, &point[p].second);

    for(int p = 0; p < points; ++ p)
        result += distance(point[p], point[(p + 1) % points]);

    printf("%.10lf\n", result);
    return 0;
}

inline
double distance(pair<double, double> &from, pair<double, double> &to)
{
    double latDiff = (to.first - from.first) * M_PI / 180.0;
    double lonDiff = (to.second - from.second) * M_PI / 180.0;
    double lat1 = from.first * M_PI / 180.0;
    double lat2 = to.first * M_PI / 180.0;

    double a = sin(latDiff / 2.0) * sin(latDiff / 2.0) + sin(lonDiff / 2.0) * sin(lonDiff / 2.0) * cos(lat1) * cos(lat2);
    return 6371.0 * 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
}
