#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HASHMAP_INIT_CAP 256

typedef struct node_t node_t;

struct node_t {
    int value;
    const char* key;
    node_t* next;
};

typedef struct hashmap {
    size_t count;
    size_t capacity;
    node_t** items;
} hashmap_t;

// djb2 hash function
uint64_t hash(hashmap_t* map, const char* str)
{
    uint64_t hash = 5381;
    int c;
    while ((c = *str++)) hash = 33*hash + c;
    return hash % map->capacity;
}

node_t* node_alloc(const char* key, int value)
{
    node_t* node = malloc(sizeof(node_t));
    node->key = strdup(key);
    node->value = value;
    return node;
}

void node_print(node_t* node)
{
    printf("%s = %d\n", node->key, node->value);
}

hashmap_t* hashmap_alloc()
{
    hashmap_t* map = malloc(sizeof(hashmap_t));
    map->capacity = HASHMAP_INIT_CAP;
    map->count = 0;
    map->items = calloc(map->capacity, sizeof(node_t*));
    return map;
}

void hashmap_free(hashmap_t* map)
{
    for (size_t i = 0; i < map->capacity; i++)
    {
        node_t* current = map->items[i];
        while (current != NULL)
        {
            node_t* next = current->next;
            free((void*)current->key);
            free(current);
            current = next;
        }
    }
    free(map->items);
    free(map);
}

void hashmap_print(hashmap_t* map)
{
    for (size_t i = 0; i < map->capacity; i++)
    {
        node_t* current = map->items[i];
        while (current != NULL)
        {
            node_print(current);
            current = current->next;
        }
    }
    printf("\n");
}

void hashmap_insert(hashmap_t* map, const char* key, int value)
{
    if (map->count == map->capacity)
    {
        size_t old_cap = map->capacity;
        map->capacity *= 2;
        map->items = realloc(map->items, map->capacity * sizeof(node_t*));
        memset(map->items + old_cap, 0, (map->capacity - old_cap) * sizeof(node_t*));
    }

    uint64_t idx = hash(map, key);
    node_t* node = node_alloc(key, value);
    node->next = map->items[idx];
    map->items[idx] = node;
    map->count++;
}

node_t* hashmap_get(hashmap_t* map, const char* key)
{
    uint64_t idx = hash(map, key);
    node_t* current = map->items[idx];

    while (current != NULL)
    {
        if (strcmp(key, current->key) == 0) return current;
        current = current->next;
    }

    return NULL; // not found
}

void hashmap_delete(hashmap_t* map, const char* key)
{
    uint64_t idx = hash(map, key);
    node_t* current = map->items[idx];
    node_t* prev = NULL;

    while (current != NULL)
    {
        if (strcmp(key, current->key) == 0)
        {
            if (prev == NULL) map->items[idx] = current->next;
            else prev->next = current->next;
            free((void*)current->key);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    map->count--;
}

int main()
{
    hashmap_t* map = hashmap_alloc();

    const char* arr[5] = {
        "hello", "world", "programming", "linux", "windows"
    };

    for (size_t i = 0; i < 5; i++) hashmap_insert(map, arr[i], i);
    hashmap_print(map);

    printf("Infos about 'linux':\n");
    node_print(hashmap_get(map, "linux"));

    printf("Deleting 'linux':\n");
    hashmap_delete(map, "linux");
    hashmap_print(map);

    hashmap_free(map);
    return 0;
}