#include <stdio.h>
#include <string.h>

int strlen_noob(const char* s)
{
    int size = 0;
    for (size_t i = 0; s[i] != '\0'; i++)
        size++;
    return size;
}

int strlen_med(const char* s)
{
    int i = 0;
    while (s[++i] != '\0');
    return i;
}

int strlen_pro(const char* s)
{
    const char* p = s;
    while (*p) p++;
    return p - s;
}

char* reverse(char* s)
{
    int n = strlen_pro(s);
    for (size_t i = 0; i < n / 2; i++)
    {
        char temp = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = temp;
    }
    return s;
}

char* strcat_noob(char* dest, char* src)
{
    size_t n = strlen_pro(dest);

    size_t j = 0;
    for (size_t i = n; src[j] != '\0'; i++)
        dest[i] = src[j++];
    dest[n + j] = '\0';
    return dest;
}

char* strcat_pro(char* dest, char* src)
{
    char* d = dest;
    char* s = d;

    while (*s) s++;
    while (*src) *s++ = *src++;
    *s = '\0';
    return dest;
}

char* strcpy_noob(char* dest, char* src)
{
    for (size_t i = 0; dest[i] != '\0'; i++)
        dest[i] = src[i];
    return dest;
}

char* strcpy_pro(char* dest, char* src)
{
    char* s = dest;
    while ((*dest++ = *src++) != '\0');
    return s;
}

int main()
{
    char s[] = "hello world";
    printf("actual len is %d\n", strlen(s));
    printf("custom len functions: %d, %d, %d\n", strlen_noob(s), strlen_med(s), strlen_pro(s));

    printf("reversed string is: %s\n", reverse(s));

    char buf[40] = "hello";
    printf("%s\n", strcat_noob(buf, " world"));
    printf("%s\n", strcat_pro(buf, " world"));

    printf("%s\n", strcpy_noob(buf, "windows"));
    printf("%s\n", strcpy_pro(buf, "linux"));

    return 0;
}