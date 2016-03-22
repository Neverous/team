#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <cstdlib>

using namespace std;

typedef long long int ll;

struct Interval {
    Interval(long double a=0, long double b=0, bool left_closed=false, bool right_closed=false) {
        x = a;
        y = b;
        leftc = left_closed;
        rightc = right_closed;
    }

    bool contains(long double a) {
        return (a > x || (a == x && leftc)) && (a < y || (a == y && rightc));
    }

    bool isZero() const {
        return x == y && !rightc && !leftc;
    }

    Interval intersect(Interval& i) {
        if(isZero() || i.isZero())
            return Interval();

        if(y < i.x || (y == i.x && (!rightc || !i.leftc))) {
            return Interval(0, 0, false, false);
        }

        if(i.y < x || (i.y == x && (!leftc || !i.rightc))) {
            return Interval(0, 0, false, false);
        }

        Interval res(max(x, i.x), min(y, i.y), false, false);
        if(x > i.x) {
            res.leftc = leftc;
        } else {
            res.leftc = i.leftc;
        }

        if(y < i.y) {
            res.rightc = rightc;
        } else {
            res.rightc = i.rightc;
        }

        if(res.x > res.y) {
            swap(res.x, res.y);
            swap(res.leftc, res.rightc);
        }

        if(res.x == res.y && (res.leftc || res.rightc)) {
            res.leftc = true;
            res.rightc = false;
        }

        return res;
    }

    bool operator<(const Interval& a) const {
        if(x == a.x)
            return y < a.y;
        return x < a.x;
    }

    void print() {
        printf("%s %Lf, %Lf %s\n", leftc ? "[" : "(", x, y, rightc ? "]" : ")");
    }

    long double x, y;
    bool leftc, rightc;
};

long double read_number() {
    char buf[256];
    scanf("%s", buf);

    int len = strlen(buf);
    int p = 0;
    for(int i = 0; i < len; i++) {
        if(buf[i] == ',') {
            p = i;
            break;
        }
    }

    ll a = atoll(buf);
    ll b = atoll(buf + p + 1);

    int digits = len - p - 2;
    return a + (pow(10, -(digits + 1))) * b;
}

Interval read_interval() {
    char buf[256];
    scanf("%s", buf); // nawias
    bool leftc = buf[0] == '[';

    long double x = read_number();
    scanf("%s", buf); // srednik
    long double y = read_number();
    scanf("%s", buf);

    bool rightc = buf[0] == ']';
    return Interval(x, y, leftc, rightc);
}

ll f(const Interval& in, double prev, double next) {
    if(in.isZero()) {
        return 0;
    }

    ll low = floor(in.x);
    ll high = ceil(in.y);

    ll diff = -1;
    if(fabs(in.y - high) < 1e-9 && in.rightc)
        diff++;
    if(fabs(in.x - low) < 1e-9 && in.leftc)
        diff++;

    //printf("LOW: %lld HIGH: %lld DIFF: %lld\n", low, high, diff);
    return high - low + diff;
}

const long double CUTOFF = 2 * 1e4;
const int DUZO = 100010;

Interval ints[DUZO];

int main() {
    int t;
    scanf("%d", &t);
    while(t --> 0) {
        int n;
        scanf("%d", &n);

        for(int i = 0; i < n; i++) {
            Interval in = read_interval();
            ints[i] = in;
        }

        sort(ints, ints + n);

        ll sumres = 0;
        ll intres = 0;

        Interval in = ints[0];
        for(int i = 1; i < n; i++) {
            //in.print();
            in = in.intersect(ints[i]);
        }

        intres = f(in);
        //in.print();

        in = Interval(-CUTOFF, CUTOFF, true, true);
        for(int i = 0; i < n; i++) {
            Interval k = in.intersect(ints[i]);
            double prev = k.x - 1;
            double next = k.y + 1;

            if(i > 0 && ints[i-1].rightc) {
                prev = ints[i-1].y;
            }

            if(i < n - 1 && ints[i+1].leftc) {
                next = ints[i+1].x;
            }

            //k.print();
            //printf("F: %lld\n", f(k));
            sumres += f(k, prev, next);

            if(k.y > in.x || (k.y == in.x && !k.rightc)) {
                in.x = k.y;
                in.rightc = !k.rightc;
            }
        }

        printf("%lld\n", sumres);
        printf("%lld\n", intres);
    }

    return 0;
}

