/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <algorithm>

int variables,
    variable[8],
    assign[8],
    wanted,
    e,
    next,
    mapping[32];
char expression[1024];

int evaluateExpression(void);

int main(void)
{
    while(scanf("%d", &variables) != -1 && variables > 0)
    {
        for(int v = 0; v < variables; ++ v)
        {
            scanf("%d", &variable[v]);
            assign[v] = v;
        }

        scanf("%d", &wanted);
        scanf("%s", expression);
        bool done = false;
        next = 0;
        for(int m = 0; m < 32; ++ m)
            mapping[m] = -1;

        do
        {
            e = 0;
            done = evaluateExpression() == wanted;
        }
        while(!done && std::next_permutation(assign, assign + variables));

        puts(done ? "YES" : "NO");
    }

    return 0;
}

inline
int evaluateExpression(void)
{
    //printf("eval '%s'\n", expression + e);
    if(expression[e] == '(')
    {
        //printf("bracket\n");
        ++ e;
        int l = evaluateExpression();
        char action = expression[e ++];
        int r = evaluateExpression();
        ++ e;

        //printf("return %d%c%d\n", l, action, r);
        switch(action)
        {
            case '+':
                return l + r;

            case '-':
                return l - r;

            case '*':
                return l * r;
        }
    }

    if(mapping[expression[e] - 'a'] < 0)
        mapping[expression[e] - 'a'] = next ++;

    //printf("return %c = %d\n", expression[e], variable[assign[mapping[expression[e] - 'a']]]);
    return variable[assign[mapping[expression[e ++] - 'a']]];
}
