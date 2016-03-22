/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>

int reviews,
    papers,
    paper;
char name[16];
std::string institution[1024];
std::vector<int> reviewers[1024];


int main(void)
{
    while(scanf("%d %d", &reviews, &papers) != -1 && reviews > 0 && papers > 0)
    {
        for(int n = 0; n < 1024; ++ n)
            reviewers[n].clear();

        for(int p = 0; p < papers; ++ p)
        {
            scanf("%s", name);
            institution[p] = name;
            for(int r = 0; r < reviews; ++ r)
            {
                scanf("%d", &paper);
                reviewers[paper - 1].push_back(p);
            }
        }

        int count = 0;
        for(int p = 0; p < papers; ++ p)
        {
            std::sort(reviewers[p].begin(), reviewers[p].end());
            bool problem = reviewers[p].size() != (unsigned int) reviews;
            for(unsigned int r = 0; !problem && r < reviewers[p].size(); ++ r)
                problem = institution[p] == institution[reviewers[p][r]];

            for(unsigned int r = 1; !problem && r < reviewers[p].size(); ++ r)
                problem = reviewers[p][r] == reviewers[p][r-1];

            count += problem;
        }

        if(count == 0)
            puts("NO PROBLEMS FOUND");

        else if(count == 1)
            puts("1 PROBLEM FOUND");

        else
            printf("%d PROBLEMS FOUND\n", count);
    }

    return 0;
}
