#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define UPPER 100
#define MAX_TRIES 10

int get_random_int()
{
    srand(time(NULL));
    return 1 + rand() % (UPPER - 1);
}

void guess(int n, int max_tries)
{
    int input;
    int tries = 0;
    bool won = false;

    while (tries < max_tries && !won)
    {
        printf("Your guess (%d tries remaining):\n> ", max_tries - tries - 1);
        scanf("%d", &input);
        tries++;

        if (input < n)
            printf("more\n");
        else if (input > n)
            printf("less\n");
        else
            won = true;
    }
    if (won)
        printf("You won in %d tries!\n", tries);
    else
        printf("You lost! Number was %d\n", n);
}

int main()
{
    int n = get_random_int();

    guess(n, MAX_TRIES);

    return 0;
}