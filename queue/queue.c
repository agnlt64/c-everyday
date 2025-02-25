#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_INIT_CAP 256

typedef struct queue {
    size_t count;
    size_t capacity;
    int* items;
    int front;
    int rear;
} queue_t;

queue_t* queue_alloc()
{
    queue_t* queue = malloc(sizeof(queue_t));
    queue->count = 0;
    queue->capacity = QUEUE_INIT_CAP;
    queue->items = malloc(sizeof(int) * queue->capacity);
    queue->front = 0;
    queue->rear = -1;
    return queue;
}

void queue_enqueue(queue_t* queue, int item)
{
    if (queue->rear == queue->capacity - 1)
    {
        queue->capacity *= 2;
        queue->items = realloc(queue->items, queue->capacity * sizeof(int));
    }
    if (queue->front == -1) queue->front = 0;
    queue->rear++;
    queue->items[queue->rear] = item;
    queue->count++;
}

bool queue_is_empty(queue_t* queue)
{
    return queue->front > queue->rear;
}

int queue_dequeue(queue_t* queue)
{
    if (queue->front == -1 || queue_is_empty(queue))
    {
        fprintf(stderr, "Can't dequeue from an empty queue!\n");
        return -1;
    }
    return queue->items[queue->front++];
}

void queue_print(queue_t* queue)
{
    for (size_t i = queue->front; i <= queue->rear; i++)
    {
        printf("%d ", queue->items[i]);
    }
    printf("\n");
}

void queue_free(queue_t* queue)
{
    free(queue->items);
    free(queue);
}

int main()
{
    queue_t* queue = queue_alloc();

    for (size_t i = 1; i <= 10; i++) queue_enqueue(queue, i);
    queue_print(queue);

    int first = queue_dequeue(queue);
    printf("First item: %d\n", first);

    int last;
    while (!queue_is_empty(queue)) last = queue_dequeue(queue);
    printf("Last item: %d\n", last);

    queue_free(queue);
    return 0;
}