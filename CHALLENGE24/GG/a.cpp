/* 2014
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>
#include <queue>
#include <cstring>
#include <map>

typedef std::pair<std::pair<long long int, int>, long long int> Triple;

int words,
    passwords,
    len[32];
char word[32][16];
char password[64];

long long int nextState(long long int state, int pos);
long long int getState(long long int state, int pos);
long long int solve(char *pass);

int main(void)
{
    scanf("%d", &words);
    for(int w = 0; w < words; ++ w)
    {
        scanf("%s", word[w]);
        len[w] = strlen(word[w]);
    }

    scanf("%d", &passwords);
    for(int p = 0; p < passwords; ++ p)
    {
        scanf("%s", password);
        printf("%lld\n", solve(password));
    }

    return 0;
}

inline
long long int solve(char *pass)
{
    int plen = strlen(pass);
    // steps, letter, state mask
    std::priority_queue<Triple, std::vector<Triple>, std::greater<Triple> > que;
    que.push(std::make_pair(std::make_pair(0, 0), 0));

    std::map<std::pair<long long int, int>, bool> checked;

    while(!que.empty())
    {
        auto act = que.top(); que.pop();
        if(act.first.second == -plen)
            return act.first.first;

        if(act.first.second == 0)
        {
            for(int w = 0; w < words; ++ w)
            {
                long long int state = nextState(act.second, w);
                if(checked[std::make_pair(state, 0)])
                    continue;

                checked[std::make_pair(state, 0)] = true;
                que.push(std::make_pair(std::make_pair(act.first.first + 1, 0), state));
            }
        }

        for(int w = 0; w < words; ++ w)
        {
            long long int state = nextState(act.second, w);
            if(word[w][getState(act.second, w)] != pass[-act.first.second])
                continue;

            int pos = -act.first.second + 1;
            if(checked[std::make_pair(state, pos)])
                continue;

            checked[std::make_pair(state, pos)] = true;
            que.push(std::make_pair(std::make_pair(act.first.first + 1, -pos), state));
        }
    }

    return -1;
}

inline
long long int nextState(long long int state, int pos)
{
    return (state & ~(0xFLL << (pos * 4))) | (((getState(state, pos) + 1) % len[pos]) << (pos * 4));
}

inline
long long int getState(long long int state, int pos)
{
    return (state & (0xFLL << (pos * 4))) >> (pos * 4);
}
