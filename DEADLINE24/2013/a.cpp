/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <queue>
#include <algorithm>

int tests,
    people,
    _person[2048];

int strategyA(const int person[2048]);
int strategyB(const int person[2048]);

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        scanf("%d", &people);
        for(int p = 0; p < people; ++ p)
            scanf("%d", &_person[p]);

        std::sort(_person, _person + people);
        if(people < 4)
            printf("%d\n", _person[people - 1]);

        else
            printf("%d\n", std::min(strategyA(_person), strategyB(_person)));
    }
    return 0;
}

int strategyA(const int person[2048])
{
    fprintf(stderr, "---------- Startegia A ----------\n");
    int act = 0,
        result = 0,
        max = 0;
    std::priority_queue<int> base;
    std::queue<int> asteroid;
    for(int p = 0; p < people; ++ p)
        asteroid.push(person[p]);

    while(!asteroid.empty())
    {
        max = 0;
        for(int c = 0; c < 4 && !asteroid.empty(); ++ c)
        {
            act = asteroid.front();
            asteroid.pop();
            base.push(-act);
            max = std::max(max, act);
            fprintf(stderr, "Leci: %d\n", act);
        }

        fprintf(stderr, "Polecieli za %d\n", max);
        result += max;
        if(!asteroid.empty())
        {
            act = -base.top();
            asteroid.push(act);
            base.pop();
            result += act;
            fprintf(stderr, "Wraca %d\n", act);
        }
    }

    fprintf(stderr, "StrategiaA: %d\n", result);
    return result;
}

int strategyB(const int person[2048])
{
    fprintf(stderr, "---------- Startegia B ----------\n");
    int result = 0;
    for(int p = people - 1; p > 0; p -= 3)
    {
        fprintf(stderr, "Leci: %d\n", person[0]);
        for(int c = 0; c < 3 && p + c >= 0; ++ c)
            fprintf(stderr, "Leci: %d\n", person[p + c]);

        result += person[p];
        fprintf(stderr, "Polecieli za %d\n", person[p]);
        if(p > 3)
        {
            result += person[0];
            fprintf(stderr, "Wraca %d\n", person[0]);
        }
    }

    fprintf(stderr, "StrategiaB: %d\n", result);
    return result;
}
