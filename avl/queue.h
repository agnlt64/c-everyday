#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_INIT_CAP 256

typedef struct node_t {
    int value;
    int height;
    struct node_t* left;
    struct node_t* right;
} node_t;

typedef struct queue {
    size_t count;
    size_t capacity;
    node_t** items;
    int front;
    int rear;
} queue_t;

// Allocates a new queue
queue_t* queue_alloc();

// Adds a node to the queue
void queue_enqueue(queue_t* queue, node_t* item);

// Returns true if the queue is empty, false otherwise
bool queue_is_empty(queue_t* queue);

// Returns the last node of the queue
node_t* queue_dequeue(queue_t* queue);

// Prints the queue
void queue_print(queue_t* queue);

// Frees the memory allocated by the queue
void queue_free(queue_t* queue);

#ifdef QUEUE_IMPLEMENTATION
queue_t* queue_alloc()
{
    queue_t* queue = malloc(sizeof(queue_t));
    queue->count = 0;
    queue->capacity = QUEUE_INIT_CAP;
    queue->items = malloc(sizeof(node_t*) * queue->capacity);
    queue->front = 0;
    queue->rear = -1;
    return queue;
}

void queue_enqueue(queue_t* queue, node_t* item)
{
    if (queue->rear == queue->capacity - 1)
    {
        queue->capacity *= 2;
        queue->items = realloc(queue->items, queue->capacity * sizeof(node_t*));
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

node_t* queue_dequeue(queue_t* queue)
{
    if (queue->front == -1 || queue_is_empty(queue))
    {
        fprintf(stderr, "Can't dequeue from an empty queue!\n");
        exit(1);
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
#endif // QUEUE_IMPLEMENTATION