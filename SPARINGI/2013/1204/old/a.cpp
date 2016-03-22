#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;
int t;
int x, y, z;

int main() {
    scanf("%d", &t);
    while(t--) {
        scanf("%d %d %d", &x, &y, &z);
        double c = ((double)x + (double)y*(1.0 - (double)z)) / ((double)z - 1.0);
        c *= -12.0;
        printf("%d\n", (int)round(c));
    }
    return 0;
}
