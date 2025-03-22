#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// only lowercase letters
#define MAX_CHARS 26

bool anagram(const char* s1, const char* s2)
{
    int freq[MAX_CHARS] = {0};

    for (size_t i = 0; s1[i] != '\0'; i++)
        freq[s1[i] - 'a']++;

    for (size_t i = 0; s2[i] != '\0'; i++)
        freq[s2[i] - 'a']--;

    for (size_t i = 0; i < MAX_CHARS; i++)
    {
        if (freq[i] != 0)
            return false;
    }
    return true;
}

bool palindrome(const char* s)
{
    int n = strlen(s);
    char* buf = malloc(n + 1);

    for (size_t i = 0; i < n; i++)
        buf[i] = s[n - 1 - i];

    return !strcmp(s, buf);
}

int main()
{
    printf("Anagram tests:\n");
    printf("%d\n", anagram("linux", "xulin"));
    printf("%d\n", anagram("linux", "windows"));

    printf("Palindrome tests:\n");
    printf("%d\n", palindrome("kayak"));
    printf("%d\n", palindrome("linux"));

    return 0;
}