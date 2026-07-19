#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av)
{
    if(ac != 4)
        return (1);
    int w = atoi(av[1]);
    int h = atoi(av[2]);
    int it = atoi(av[3]);
    int arr[2][w + 2][h + 2];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < h + 2; j++)
        {
            for (int k = 0; k < w + 2; k++)
                arr[i][k][j] = 0;
        }   
    }
    int x = 1, y = 1, p = 0;
    char cmd;
    while (read(0, &cmd, 1) > 0)
    {
        if (cmd == 'w' && y > 1)
            y--;
        else if (cmd == 'a' && x > 1)
            x--;
        else if (cmd == 's' && y < h)
            y++;
        else if (cmd == 'd' && x < w)
            x++;
        else if (cmd == 'x')
            p = !p;
        if (p)
            arr[0][x][y] = 1;
    }
    for (int i = 0; i < it; i++)
    {
        for (int j = 1; j < h + 1; j++)
        {
            for (int k = 1; k < w + 1; k++)
            {
                int nb = 0;
                for (int y = -1; y <= 1; y++)
                {
                    for (int x = -1; x <= 1; x++)
                    {
                       if (!(x == 0 && y == 0))
                        nb += arr[i % 2][k + x][j + y];
                    }
                }
                if (arr[i % 2][k][j] == 1)
                {
                    if (nb == 2 || nb == 3)
                        arr[(i + 1) % 2][k][j] = 1;
                    else
                        arr[(i + 1) % 2][k][j] = 0;
                }
                else
                {
                    if (nb == 3)
                        arr[(i + 1) % 2][k][j] = 1;
                    else
                        arr[(i + 1) % 2][k][j] = 0;
                }
            }
        }
    }
    for (int i = 1; i < h + 1; i++)
    {
        for (int j = 1; j < w + 1; j++)
        {
            putchar(arr[it % 2][j][i] ? '0' : ' ');
        }
        putchar('\n');
    }
    return 0;
}