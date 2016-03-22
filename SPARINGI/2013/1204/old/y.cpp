#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <utility>

using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;

void print(ll n, ll k);

int main() {
    int t;
    scanf("%d", &t);
    while(t --> 0) {
        ll n, k;
        scanf("%lld %lld", &n, &k);
        print(n, k-1);
        puts("");
    }

    return 0;
}

pl megafib(ll k) {
    ll block = 1;
    ll fib = 1;
    ll prevfib = 0;
    ll sum = 1;

    while(sum < k) {
        block++;
        ll c = fib;
        fib += prevfib;
        prevfib = c;
        sum += fib;
    }

    //printf("For k=%lld got block=%lld, fib=%lld, sum=%lld\n", k, block, fib, sum - fib);

    return make_pair(block, sum - fib);
}

bool has_zero_in_front(ll n, ll k) {
    if(n == 1)
        return k == 0;
    if(n == 2)
        return k < 2;
    if(n == 3)
        return k < 3;

    pl res = megafib(k);
    return res.first < n;
}

void print(ll n, ll k) {
    if(n <= 0)
        return;

    pl res = megafib(k);
    if(res.first > n) {
        printf("-1");
        return;
    }

    if(has_zero_in_front(n, k)) {
        printf("0");
        print(n-1, k);
    } else if (n == 1) {
        printf("1");
    } else {
        printf("10");
        print(n-2, k - res.second - 1);
    }
}

