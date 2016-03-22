#include <cstdio>

int pass,
    left,
    right,
    leftPassed;

void init(void);
void loop(void);

int main(void)
{
    scanf("%d %d %d", &pass, &left, &right);
    init();
    while(leftPassed < left)
        loop();

    puts("0");
    return 0;
}

inline
void init(void)
{
    // MOVE LEFT
    for(int p = 0; p < pass + 1; ++ p)
        printf("1 1 1 %d 0 0\n", pass - 1);

    // PASS RIGHT
    for(int p = 0; p < pass + 3 + right; ++ p)
        printf("2 2 0 0 -1 %d\n", right);

    // MOVE LEFT
    for(int p = 0; p < pass + 1; ++ p)
        printf("1 1 1 %d 0 0\n", pass - 1);

    leftPassed = pass - 1;
}

inline
void loop(void)
{
    // MOVE RIGHT
    for(int p = 0; p < pass + 2 + right; ++ p)
        printf("1 1 0 0 1 %d\n", pass - 1);

    printf("1 1 0 0 1 0\n");

    // MOVE RIGHT
    for(int p = 0; p < pass + 3 + right + pass - 1; ++ p)
        printf("0 0 0 0 -1 %d\n", right);

    for(int p = 0; p < pass - 1; ++ p)
        printf("0 0 0 0 1 %d\n", left - leftPassed);

    // MOVE LEFT
    for(int p = 0; p < right + 1 + 1; ++ p)
        printf("1 1 -1 0 0 0\n");

    for(int p = 0; p < pass + 1; ++ p)
        printf("1 1 1 %d 0 0\n", leftPassed + pass - 1);

    leftPassed += pass - 1;
    printf(">>%d\n", leftPassed - left);
}
