/* 2013
 * Maciej Szeptuch
 * IIUWr
 */
#include <cstdio>
#include <algorithm>

int candidates,
    ballots,
    votes[16],
    invalid,
    result[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
char name[16][128],
     ballot[16];

bool valid(const char* ball, int &vote);
bool compareScores(int a, int b);

int main(void)
{
#ifdef ONLINE_JUDGE
    freopen("bad.in", "r", stdin);
    freopen("bad.out", "w", stdout);
#endif // ONLINE_JUDGE

    scanf("%d %d", &candidates, &ballots);
    for(int c = 0; c < candidates; ++ c)
        scanf("%s", name[c]);

    for(int b = 0; b < ballots; ++ b)
    {
        scanf("%s", ballot);
        int vote = -1;
        if(valid(ballot, vote))
            ++ votes[vote];

        else
            ++ invalid;
    }

    std::sort(result, result + candidates, compareScores);
    for(int c = 0; c < candidates; ++ c)
        printf("%s %.2f%%\n", name[result[c]], 100.0 * votes[result[c]] / ballots);

    printf("Invalid %.2f%%\n", 100.0 * invalid / ballots);
    return 0;
}

inline
bool valid(const char* ball, int &vote)
{
    vote = -1;
    for(int b = 0; ball[b]; ++ b)
        if(ball[b] == 'X')
        {
            if(vote != -1) return false;
            vote = b;
        }

    return vote != -1;
}

inline
bool compareScores(int a, int b)
{
    if(votes[a] > votes[b])
        return true;

    if(votes[a] < votes[b])
        return false;

    return a < b;
}
