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

int strcmp_noob(const char* s1, const char* s2)
{
    int m = strlen_pro(s1);
    int n = strlen_pro(s2);

    if (m != n)
        return 1;

    for (size_t i = 0; i < n; i++)
    {
        if (s1[i] != s2[i])
            return 1;
    }
    return 0;
}

int strcmp_pro(const char* s1, const char* s2)
{
    const char* p1 = s1;
    const char* p2 = s2;

    while (*p1)
    {
        if (*p1++ != *p2++)
            return 1;
    }
    if (*p2 != '\0') return 1;
    return 0;
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

    printf("%d\n", strcmp_noob("windows", "linux"));
    printf("%d\n", strcmp_pro("windows", "linux"));

    printf("%d\n", strcmp_noob("hello", "hello"));
    printf("%d\n", strcmp_pro("hello", "hello"));

    return 0;
}
