#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_INIT_CAP 256

typedef struct stack {
    size_t count;
    size_t capacity;
    int* items;
} stack_t;

stack_t* stack_alloc()
{
    stack_t* stack = malloc(sizeof(stack_t));
    stack->count = 0;
    stack->capacity = STACK_INIT_CAP;
    stack->items = malloc(sizeof(int) * stack->capacity);
    return stack;
}

void stack_free(stack_t* stack)
{
    free(stack->items);
    free(stack);
}

void stack_print(stack_t* stack)
{
    for (size_t i = 0; i < stack->count; i++)
    {
        printf("%d ", stack->items[i]);
    }
    printf("\n");
}

void stack_push(stack_t* stack, int item)
{
    if (stack->count == stack->capacity)
    {
        stack->capacity *= 2;
        stack->items = realloc(stack->items, stack->capacity * sizeof(int));
    }
    stack->items[stack->count++] = item;
}

bool stack_is_empty(stack_t* stack)
{
    return stack->count == 0;
}

int stack_pop(stack_t* stack)
{
    if (stack_is_empty(stack))
    {
        fprintf(stderr, "Can't pop from an empty stack!\n");
        return -1;
    }
    return stack->items[--stack->count];
}

int main()
{
    stack_t* stack = stack_alloc();

    for (size_t i = 0; i < 10; i++) stack_push(stack, i);
    stack_print(stack);

    int last = stack_pop(stack);
    printf("Last item: %d\n", last);
    stack_print(stack);

    int first;
    while (!stack_is_empty(stack)) first = stack_pop(stack);
    printf("First item: %d\n", first);
    
    stack_free(stack);
    return 0;
}