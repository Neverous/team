#include <cstdio>
#include <algorithm>

int sizes,
    colors,
    color,
    size[131072],
    result;

int main(void)
{
    scanf("%d %d", &sizes, &colors);
    for(int s = 0; s < sizes; ++ s)
        scanf("%d", &size[s]);

    std::sort(size, size + sizes);
    for(int c = 0; c < colors; ++ c)
    {
        scanf("%d", &color);
        result += *std::lower_bound(size, size + sizes, color) - color;
    }

    printf("%d\n", result);
    return 0;
}
