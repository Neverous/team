#include <cstdio>
#include <iostream>
#include <vector>
#include <utility>

typedef long long int ll;
typedef long long unsigned int llu;

using namespace std;

int main() {
    int n;
    cin >> n;

    for(int i = 0; i < n; i++) {
        ll a, b;
        cin >> a >> b;

        if(a  < 0 || b < 0) {
            printf("IMPOSSIBLE\n");
        } else if(a == 0 && b != 0) {
            printf("IMPOSSIBLE\n");
        } else if(a == 1 && b == a) {
            printf("AMBIGUOUS\n");
        } else if((a == 1 && b != 1) || a > b) {
            printf("IMPOSSIBLE\n");
        } else if(a == b) {
            printf("%lld\n", a);
        } else {
            llu au = a;
            llu bu = b;
            llu current_power = 1;
            llu current_exp = 0;
            while(current_power <= bu) {
                current_power *= au;
                current_exp++;
            }

            current_power /= au;
            current_exp--;

            llu coins = 0;
            vector<llu> powers;
            while(bu > 0) {
                ll c = bu / current_power;
                coins += c;
                bu %= current_power;

                powers.push_back(c);
                current_power /= au;
                current_exp--;
            }

            if(coins != au) {
                printf("IMPOSSIBLE\n");
            } else {
                for(int j = 0; j < powers.size(); j++) {
                    printf("%llu ", powers[j]);
                }
                printf("\n");
            }
        }
    }

    return 0;
}

