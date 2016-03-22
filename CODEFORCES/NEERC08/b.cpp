/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cstring>

bool visited[64][64];

void dfs(const char *msg, int h = 31, int w = 31);
bool moved(const char *msg);

int main(void)
{
    visited[31][31] = true;
    dfs("NORTH");
    puts("DONE");
    fflush(stdout);
    return 0;
}

void dfs(const char *msg, int h, int w)
{
    if(!visited[h-1][w])
    {
        visited[h-1][w] = true;
        if(moved("NORTH"))
            dfs("SOUTH", h - 1, w);
    }

    if(!visited[h+1][w])
    {
        visited[h+1][w] = true;
        if(moved("SOUTH"))
            dfs("NORTH", h + 1, w);
    }

    if(!visited[h][w-1])
    {
        visited[h][w-1] = true;
        if(moved("EAST"))
            dfs("WEST", h, w - 1);
    }

    if(!visited[h][w+1])
    {
        visited[h][w+1] = true;
        if(moved("WEST"))
            dfs("EAST", h, w + 1);
    }

    moved(msg);
    fflush(stdout);
}

bool moved(const char *msg)
{
    puts(msg);
    fflush(stdout);
    char res[128];
    scanf("%s", res);
    return strcmp("EMPTY", res) == 0;
}
