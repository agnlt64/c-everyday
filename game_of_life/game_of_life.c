#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

typedef struct board {
    int width;
    int height;
    char* world;
    char* temp;
} board_t;

board_t* board_alloc()
{
    board_t* b = malloc(sizeof(board_t));
    b->width = 50;
    b->height = 50;
    b->world = malloc(b->width * b->height);
    b->temp = malloc(b->width * b->height);
    
    return b;
}

void board_print(board_t* b)
{
    printf("\e[H");
    char* current = b->world;

    for (size_t y = 0; y < b->height; y++)
    {
        for (size_t x = 0; x < b->width; x++)
        {
            printf(*current ? "\e[07m  \e[m" : "  ");
            current++;
        }
        printf("\e[E");
    }
    fflush(stdout);
}

void board_evolve(board_t* b)
{
    for (size_t y = 0; y < b->height; y++)
    {
        for (size_t x = 0; x < b->width; x++)
        {
            int n = 0;
            for (size_t y1 = y - 1; y1 <= y + 1; y1++)
            {
                for (size_t x1 = x - 1; x1 <= x + 1; x1++)
                {
                    int actual_x = (x1 + b->width) % b->width;
                    int actual_y = (y1 + b->height) % b->height;

                    if (b->world[actual_x + actual_y * b->width])
                        n++;
                }
            }
            if (b->world[x + y * b->width])
                n--;

            b->temp[x + y * b->width] = (char)(n == 3 || (n == 2 && b->world[x + y * b->width]));
        }
    }

    memcpy(b->world, b->temp, b->width * b->height);
}

void board_free(board_t* b)
{
    free(b->world);
    free(b->temp);
    free(b);
}

int main()
{
    board_t* b = board_alloc();

    srand(time(NULL));
    for (int i = b->height * b->width - 1; i >= 0; i--)
        b->world[i] = rand() % 10 == 0 ? 1 : 0;

    for (size_t i = 0; i < 1000; i++)
    {
        board_print(b);
        board_evolve(b);
        #ifdef _WIN32
        Sleep(500);
        #else
        usleep(500000);
        #endif
    }

    board_free(b);
    return 0;
}