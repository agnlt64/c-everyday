#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define OUT 0
#define IN 1

int count_words(const char* str)
{
    int n = strlen(str);
    if (n == 0)
        return 0;
    
    int count = 0;
    int state = OUT;

    for (size_t i = 0; i < n; i++)
    {
        if (isalnum(str[i]))
        {
            if (state == OUT)
            {
                count++;
                state = IN;
            }
        }
        else
            state = OUT;
    }
    return count;
}

int main()
{
    const char* sentence = "Hello, I'm a big fan of Linux and macOS, but I don't really like Windows";
    printf("words: %d\n", count_words(sentence));

    return 0;
}