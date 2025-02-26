#include <stdio.h>
#include <stdlib.h>

typedef struct list_t list_t;

struct list_t {
    size_t count;
    int value;
    list_t* next;
};

list_t* list_alloc()
{
    list_t* list = malloc(sizeof(list_t));
    list->count = 0;
    list->value = 0;
    list->next = NULL;
    return list;
}

void list_free(list_t* list)
{
    list_t* current = list;
    list_t* next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

void list_print(list_t* list)
{
    list_t* current = list->next; // skip the first dummy value
    while (current != NULL)
    {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}

void list_add(list_t* list, int value)
{
    list_t* current = list;
    while (current->next != NULL) current = current->next;

    current->next = malloc(sizeof(list_t));
    current->next->value = value;
    current->next->next = NULL;
    list->count++;
}

void list_remove_value(list_t* list, int value)
{
    list_t* current = list;
    while (current->next != NULL)
    {
        if (current->next->value == value)
        {
            list_t* temp = current->next;
            current->next = current->next->next;
            free(temp);
            list->count--;
            return;
        }
        current = current->next;
    }
}

int list_remove_last(list_t* list)
{
    int value = -1;
    if (list->next == NULL)
    {
        value = list->value;
        free(list);
    }

    list_t* current = list;
    while (current->next->next != NULL) current = current->next;
    value = current->next->value;
    free(current->next);
    current->next = NULL;
    list->count--;

    return value;
}

int list_at(list_t* list, int index)
{
    if (index < 0 || index >= list->count)
    {
        fprintf(stderr, "Index out of bounds!\n");
        return -1;
    }
    int idx = 0;
    list_t* current = list->next;
    while (idx++ != index) current = current->next;
    return current->value;
}

void list_reverse(list_t* list)
{
    if (list->next == NULL) return;

    list_t* prev = NULL, *current = list->next, *next = NULL;
    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->next = prev;
}

int list_find(list_t* list, int value)
{
    if (list->value == value) return 0;

    int idx = 0;
    list_t* current = list;
    while (current->next != NULL)
    {
        if (current->next->value == value) return idx;
        current = current->next;
        idx++;
    }

    return -1; // not found
}

int main()
{
    list_t* list = list_alloc();

    for (size_t i = 0; i < 10; i++) list_add(list, i);
    printf("Size of the list: %d\n", list->count);
    list_print(list);

    printf("Removing 1: \n");
    list_remove_value(list, 1);
    list_print(list);

    printf("First element: %d\n", list_at(list, 0));
    int last = list_remove_last(list);
    printf("Last element: %d\n", last);
    printf("%d has been removed: \n", last);
    list_print(list);

    printf("Number 3 is at index %d\n", list_find(list, 3));

    printf("Reversed list: \n");
    list_reverse(list);
    list_print(list);

    printf("Number 3 is at index (in reversed list) %d\n", list_find(list, 3));

    list_free(list);
    return 0;
}