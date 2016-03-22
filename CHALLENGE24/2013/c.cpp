/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <algorithm>
#include <unistd.h>

int x, y;
int sX = -(1 << 30), sY = - (1 << 30),
    eX = (1 << 30), eY = (1 << 30);

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

int direction = LEFT;

int main(void)
{
    scanf("%*d");
    while(scanf("%d %d", &x, &y) != -1)
    {
        sX = std::max(sX, x);
        sY = std::max(sY, y);

        eX = std::min(eX, x);
        eY = std::min(eY, y);
    }

    int px = - (1 << 30), py = - (1 << 30);
    x = sX; y = sY;
    ++ sX;
    int stopped = 0;
    do
    {
        if(px != x || py != y)
        {
            printf("%d %d\n", x, y);
            //fprintf(stderr, "dr.line((%d + 1000, %d + 1000, %d + 1000, %d + 1000), fill = (0,0,0))\n", px, py, x, y);
            stopped = 0;
        }

        else
            ++ stopped;

        px = x;
        py = y;
        //printf("X: %d %d Y: %d %d\n", eX, sX, eY, sY);
        //printf(">>%d\n", direction);
        switch(direction)
        {
            case LEFT:
                sX -= 1;
                x = eX;
                direction = DOWN;
                break;

            case DOWN:
                sY -= 1;
                y = eY;
                direction = RIGHT;
                break;

            case RIGHT:
                eX += 1;
                x = sX;
                direction = UP;
                break;

            case UP:
                eY += 1;
                y = sY;
                direction = LEFT;
                break;
        }
    }
    while(sX >= eX && sY >= eY && stopped < 5);

    return 0;
}
