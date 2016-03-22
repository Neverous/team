/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>

int tests,
    numbers,
    number,
    biggest;

long long int result;
const int MOD = 1000000007;

class iTree
{
    static const int SIZE = 1048576;
    long long int mul[SIZE * 2];

    public:
        iTree(void);
        void clear(void);

        void add(int position);
        long long int getMul(int start, int end, int iStart = 0, int iEnd = SIZE - 1, int pos = 1);
} tree;

int main(void)
{
    scanf("%d", &tests);
    for(int t = 0; t < tests; ++ t)
    {
        biggest = 0;
        tree.clear();
        result = 0;
        scanf("%d", &numbers);
        for(int n = 0; n < numbers; ++ n)
        {
            scanf("%d", &number);
            if(biggest > number)
                result = (result + tree.getMul(0, number - 1)) % MOD;

            else
                biggest = number;

            tree.add(number);
        }

        printf("%lld\n", result);
    }

    return 0;
}

iTree::iTree(void)
:mul()
{
    clear();
}

void iTree::clear(void)
{
    for(int s = 0; s < 2 * SIZE; ++ s)
        mul[s] = 1;
}

void iTree::add(int position)
{
    position += SIZE;
    ++ mul[position];
    position /= 2;
    while(position > 0)
    {
        mul[position] = (mul[position * 2] * mul[position * 2 + 1]) % MOD;
        position /= 2;
    }
}

long long int iTree::getMul(int start, int end, int iStart/* = 0*/, int iEnd/* = SIZE - 1*/, int pos/* = 1*/)
{
    if(start == iStart && end == iEnd)
        return mul[pos];

    int iMiddle = (iStart + iEnd) / 2;
    if(end <= iMiddle)
        return getMul(start, end, iStart, iMiddle, pos * 2);

    if(start > iMiddle)
        return getMul(start, end, iMiddle + 1, iEnd, pos * 2 + 1);

    return (getMul(start, iMiddle, iStart, iMiddle, pos * 2) * getMul(iMiddle + 1, end, iMiddle + 1, iEnd, pos * 2 + 1)) % MOD;
}
