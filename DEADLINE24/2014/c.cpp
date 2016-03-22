/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>
#include <algorithm>

#define DEBUG printf
//#define DEBUG(...) {}

using namespace std;

struct Characteristic
{
    int cnt[26];

    Characteristic(void);
    Characteristic(const char* in);

    int &operator[](int i);
    const int &operator[](int i) const;

    Characteristic operator+(const Characteristic &src) const;
    Characteristic operator-(const Characteristic &src) const;

    Characteristic &operator+=(const Characteristic &src);
    Characteristic &operator-=(const Characteristic &src);

    bool operator==(const Characteristic &src) const;
    bool operator<=(const Characteristic &src) const;

    int sum(void);
}; // struct Characteristic

Characteristic checksum;
int sentences,
    S[1024], M[1024],
    cnt;
char sentence[131072];
char output[1024];
char input[1024];
vector<Characteristic>      chars(1);
vector<int>                 word[1024];
unordered_map<string, int>  hashed;
vector<string>              rev(1);
vector<int>                 sortme;

int maxPoints(int w);
bool maxPointsSort(int w1, int w2);

long int calculateResult(vector<int> &take);

int main(int, char **argv)
{
    srand(time(nullptr));
    sprintf(output, "inter/inter%02d.ans", atoi(argv[1]));
    sprintf(input, "sets/inter%02d.in", atoi(argv[1]));
    printf("INPUT: %s\n", input);
    FILE *in = fopen(input, "r");
    for(int a = 0; a < 26; ++ a)
        fscanf(in, "%d", &checksum[a]);

    fscanf(in, "%d", &sentences);
    for(int s = 0; s < sentences; ++ s)
    {
        fscanf(in, "%d %d %[^\n]", &S[s], &M[s], sentence);
        char *temp = strtok(sentence, " ");
        while(temp)
        {
            string str = temp;
            if(hashed[str] == 0)
            {
                hashed[str] = ++ cnt;
                rev.push_back(str);
                sortme.push_back(cnt);
                chars.push_back(Characteristic(temp));
            }

            word[s].push_back(hashed[str]);
            temp = strtok(nullptr, " ");
        }

        sort(begin(word[s]), end(word[s]));
    }

    fclose(in);

    DEBUG("==========\n");
    DEBUG("%d sentences\n", sentences);
    DEBUG("%d different words\n", cnt);
    DEBUG("==========\n");

    long int best = 0;
    int e = 128,
        r = 16;
    sort(begin(sortme), end(sortme), maxPointsSort);
    for(int __t = 0; true; ++ __t)
    {
        if((__t % 128) == 0) DEBUG("Next iteration!\n");
        Characteristic current;
        vector<int> take;
        for(int t = 0; t < e && current <= checksum; ++ t)
            for(int w = 0; w < cnt && current <= checksum; ++ w)
            {
                int ww = rand() % cnt;
                for(int q = 0; q < r && current + chars[sortme[ww]] <= checksum; ++ q)
                {
                    take.push_back(sortme[ww]);
                    current += chars[sortme[ww]];
                }
            }

        long int result = calculateResult(take);
        if(result < best)
            continue;

        DEBUG("Better result: %ld vs %ld\n", result, best);
        FILE *out = fopen(output, "w");
        fprintf(out, "%ld\n%lu\n", result, take.size());
        for(unsigned int t = 0; t < take.size(); ++ t)
            fprintf(out, "%s\n", rev[take[t]].c_str());

        fclose(out);
        best = result;
        e = 1 + rand() % 128;
        r = 1 + rand() % 16;
    }

    return 0;
}

inline
Characteristic::Characteristic(void)
:cnt()
{
}

inline
Characteristic::Characteristic(const char* in)
:cnt()
{
    for(int i = 0; in[i]; ++ i)
        ++ cnt[in[i] - 'a'];
}

inline
int &Characteristic::operator[](int i)
{
    return cnt[i];
}

inline
const int &Characteristic::operator[](int i) const
{
    return cnt[i];
}

inline
Characteristic Characteristic::operator+(const Characteristic &src) const
{
    Characteristic result;
    for(int i = 0; i < 26; ++ i)
        result[i] = cnt[i] + src[i];

    return result;
}

inline
Characteristic Characteristic::operator-(const Characteristic &src) const
{
    Characteristic result;
    for(int i = 0; i < 26; ++ i)
    {
        assert(cnt[i] - src[i] >= 0);
        result[i] = cnt[i] - src[i];
    }

    return result;
}

inline
Characteristic &Characteristic::operator+=(const Characteristic &src)
{
    for(int i = 0; i < 26; ++ i)
        cnt[i] += src[i];

    return *this;
}

inline
Characteristic &Characteristic::operator-=(const Characteristic &src)
{
    for(int i = 0; i < 26; ++ i)
    {
        assert(cnt[i] - src[i] >= 0);
        cnt[i] -= src[i];
    }

    return *this;
}

inline
bool Characteristic::operator==(const Characteristic &src) const
{
    for(int i = 0; i < 26; ++ i)
        if(cnt[i] != src[i])
            return false;

    return true;
}

inline
bool Characteristic::operator<=(const Characteristic &src) const
{
    for(int i = 0; i < 26; ++ i)
        if(cnt[i] > src[i])
            return false;

    return true;
}

inline
int Characteristic::sum(void)
{
    int res = 0;
    for(int i = 0; i < 26; ++ i)
        res += cnt[i];

    return res;
}

inline
bool maxPointsSort(int w1, int w2)
{
    return maxPoints(w1) > maxPoints(w2);
}

inline
int maxPoints(int w)
{
    int res = -1000000000;
    for(int s = 0; s < sentences; ++ s)
        if(binary_search(begin(word[s]), end(word[s]), w))
            res = max(res, word[s].size() > 1 ? S[s] : M[s]);

    return res;
}

inline
long int calculateResult(vector<int> &take)
{
    long int result = 0;
    sort(begin(take), end(take));

    vector<bool> used[1024];
    for(int s = 0; s < sentences; ++ s)
    {
        used[s].resize(word[s].size(), false);
        for(unsigned int t = 0, w = 0; t < take.size() && w < word[s].size(); ++ t)
        {
            while(w < word[s].size() && (take[t] > word[s][w] || used[s][w]))
                ++ w;

            if(word[s][w] == take[t] && !used[s][w])
                used[s][w] = true;
        }

        unsigned int got = 0;
        for(unsigned int w = 0; w < word[s].size(); ++ w)
            got += used[s][w];

        result += S[s] * got;
        if(got == word[s].size())
            result += M[s];
    }

    return result;
}
