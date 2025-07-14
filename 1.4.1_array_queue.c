/*
 * @file: 1.4.1_array_queue.c
 * @brief: Implements a queue data type, built on top of a dynamic array.
 * @compile: "clang -g -o 1.4.1_array_queue 1.4.1_array_queue.c"
 * @run: "./1.4.1_array_queue"
 * @leak checks: "valgrind --leak-check=full --show-leak-kinds=all
 *                ./1.4.1_array_queue"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_INITIAL_CAPACITY 4
#define QUEUE_RESIZE_FACTOR 2

typedef struct queue {
  void *items;
  size_t item_size;
  unsigned int count;
  unsigned int capacity;
} queue;

/*
 * Initialize queue
 */
void queue_init(queue *q, size_t item_size) {
  q->items = NULL;
  q->item_size = item_size;
  q->count = 0;
  q->capacity = 0;
}

/*
 * Enqueue:
 * Error out if an invalid item or queue is passed.
 * If the capacity is zero, set to QUEUE_INITIAL_CAPACITY.
 * If count is equal to capacity, set a new capacity which is old capacity times
 * QUEUE_RESIZE_FACTOR, then re-allocate the existing items, and update
 * capacity. 'memcpy' the given item to the end of the queue.
 */
void queue_enqueue(queue *q, void *item) {
  if (!item || !q || q->item_size == 0) {
    fprintf(stderr, "Invalid  queue or item passed to function\n");
    exit(1);
  }

  if (q->capacity == 0) {
    q->capacity = QUEUE_INITIAL_CAPACITY;
    q->items = malloc(q->item_size * q->capacity);
    if (q->items == NULL) {
      fprintf(stderr, "Cannot allocate memory for queue");
      exit(-1);
    }
  }

  if (q->count == q->capacity) {
    unsigned int new_capacity = q->capacity * QUEUE_RESIZE_FACTOR;
    void *new_items = realloc(q->items, q->item_size * new_capacity);
    if (new_items == NULL) {
      fprintf(stderr, "Cannot re-allocate memory for queue");
      exit(-1);
    }
    q->items = new_items;
    q->capacity = new_capacity;
  }

  memcpy((char *)q->items + (q->count * q->item_size), item, q->item_size);
  q->count++;
}

/*
 * Dequeue:
 * Error out if an invalid queue, or an invalid pointer to an item is passed, or
 * item_size is zero, or item_count is zero. 'memcpy' the front-most item in the
 * queue to the given item pointer.
 * Move everything from items+item_size to items.
 * If count falls below capacity /
 * QUEUE_RESIZE_FACTOR, set a new capacity and re-allocate the current items.
 */
void queue_dequeue(queue *q, void *item) {
  if (!item || !q || q->item_size == 0 || q->count == 0) {
    fprintf(stderr, "Invalid  queue or item passed to function\n");
    exit(1);
  }

  memcpy(item, q->items, q->item_size);
  q->count--;

  memmove(q->items, q->items + q->item_size, q->item_size * q->count);

  if (q->count < (q->capacity / QUEUE_RESIZE_FACTOR)) {
    unsigned int new_capacity = q->capacity / QUEUE_RESIZE_FACTOR;

    if (new_capacity < QUEUE_INITIAL_CAPACITY) {
      new_capacity = QUEUE_INITIAL_CAPACITY;
      return;
    }

    void *new_items = realloc(q->items, new_capacity * q->item_size);
    if (new_items == NULL) {
      fprintf(stderr, "Cannot re-allocate memory for queue");
      exit(-1);
    }
    q->items = new_items;
    q->capacity = new_capacity;
  }
}

/*
 * Free queue
 */
void queue_free(queue *q) {
  if (!q)
    return;
  free(q->items);
  q->items = NULL;
  q->item_size = 0;
  q->count = 0;
  q->capacity = 0;
}

int main() {
  queue my_queue = {0};
  queue_init(&my_queue, sizeof(int));

  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (int i = 0; i < 10; i++)
    queue_enqueue(&my_queue, arr + i);

  int *item = malloc(sizeof(int));
  for (int i = 0; i < 10; i++) {
    queue_dequeue(&my_queue, item);
    printf("%d\n", *item);
  }
  free(item);

  queue_free(&my_queue);

  return 0;
}
