/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <vector>

using namespace std;

int llamas,
    actions,
    a, b, weight;
char action[2];
pair<int, int> repr[131072],
               A, B;

void unionUnion(int a, int b, int weight);
pair<int, int> unionFind(int a);

int main(void)
{
    while(scanf("%d %d", &llamas, &actions) != -1 && actions && llamas)
    {
        for(int l = 0; l < llamas; ++ l)
            repr[l] = make_pair(l, 0);

        for(int _a = 0; _a < actions; ++ _a)
        {
            scanf("%s %d %d", action, &a, &b); -- a, --b;
            switch(action[0])
            {
                case '!':
                    scanf("%d", &weight);
                    unionUnion(a, b, weight);
                    break;

                case '?':
                    A = unionFind(a);
                    B = unionFind(b);

                    if(A.first != B.first)
                        puts("UNKNOWN");

                    else
                        printf("%d\n", A.second - B.second);

                    break;
            }
        }
    }

    return 0;
}

void unionUnion(int _a, int _b, int _weight)
{
    pair<int, int> fa = unionFind(_a),
                   fb = unionFind(_b);

    if(fa.first == fb.first)
        return;

    if(fa.first > fb.first)
    {
        repr[fa.first].first = fb.first;
        repr[fa.first].second += _weight - (fa.second - fb.second);
        return;
    }

    repr[fb.first].first = fa.first;
    repr[fb.first].second += (fa.second - fb.second) - _weight;
    return;
}

pair<int, int> unionFind(int _a)
{
    if(repr[_a].first == _a)
        return repr[_a];

    pair<int, int> step = unionFind(repr[_a].first);
    repr[_a].first = step.first;
    repr[_a].second += step.second;
    return repr[_a];
}
