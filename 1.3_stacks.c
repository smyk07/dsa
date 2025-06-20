/*
 * @file: 1.3_stacks.c
 * @brief: Implements a stack data type, built on top of a dynamic array.
 * @compile: "clang -g -o 1.3_stacks 1.3_stacks.c"
 * @run: "./1.3_stacks"
 * @leak checks: "valgrind --leak-check=full --show-leak-kinds=all ./1.3_stacks"
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4
#define RESIZE_FACTOR 2

typedef struct stack {
  void *items;
  size_t item_size;
  unsigned int count;
  unsigned int capacity;
} stack;

/*
 * Initializes the stack.
 */
void stack_init(stack *s, size_t item_size) {
  s->items = NULL;
  s->item_size = item_size;
  s->count = 0;
  s->capacity = 0;
}

/*
 * Push items to stack.
 * Error out if an invalid item or stack is passed.
 * If the capacity is zero, set to INITIAL_CAPACITY.
 * If count is equal to capacity, set a new capacity which is old capacity times
 * RESIZE_FACTOR, then re-allocate the existing items, and update capacity.
 * 'memcpy' the given item to the end of the stack.
 */
void stack_push(stack *s, void *item) {
  if (!s || !item || s->item_size == 0) {
    fprintf(stderr, "Invalid stack or item passed to function\n");
    exit(-1);
  }

  if (s->capacity == 0) {
    s->capacity = INITIAL_CAPACITY;
    s->items = malloc(s->item_size * s->capacity);
    if (s->items == NULL) {
      fprintf(stderr, "Cannot allocate memory for stack");
      exit(-1);
    }
  }

  if (s->count == s->capacity) {
    unsigned int new_capacity = s->capacity * RESIZE_FACTOR;
    void *new_items = realloc(s->items, s->item_size * new_capacity);
    if (new_items == NULL) {
      fprintf(stderr, "Cannot re-allocate memory for stack");
      exit(-1);
    }
    s->items = new_items;
    s->capacity = new_capacity;
  }

  memcpy((char *)s->items + (s->count * s->item_size), item, s->item_size);
  s->count++;
}

/*
 * Pop items from the stack.
 * Error out if an invalid stack, or an invalid pointer to an item is passed, or
 * item_size is zero, or item_count is zero. 'memcpy' the top-most item in the
 * stack to the given item pointer. If count falls below capacity /
 * RESIZE_FACTOR, set a new capacity and re-allocate the current items.
 */
void stack_pop(stack *s, void *item) {
  if (!s || !item || s->item_size == 0 || s->count == 0) {
    fprintf(stderr, "Invalid stack or item passed to function\n");
    exit(-1);
  }

  memcpy(item, (char *)s->items + ((s->count - 1) * s->item_size),
         s->item_size);
  s->count--;

  if (s->count < (s->capacity / RESIZE_FACTOR)) {
    unsigned int new_capacity = s->capacity / RESIZE_FACTOR;

    if (new_capacity < INITIAL_CAPACITY) {
      new_capacity = INITIAL_CAPACITY;
      return;
    }

    void *new_items = realloc(s->items, new_capacity * s->item_size);
    if (new_items == NULL) {
      fprintf(stderr, "Cannot re-allocate memory for stack");
      exit(-1);
    }
    s->items = new_items;
    s->capacity = new_capacity;
  }
}

/*
 * Free the stack.
 */
void stack_free(stack *s) {
  if (!s)
    return;
  free(s->items);
  s->items = NULL;
  s->item_size = 0;
  s->count = 0;
  s->capacity = 0;
}

/*
 * Print stack state.
 */
void print_stack_state(stack *s) {
  printf("\tItem size: %zu\n", s->item_size);
  printf("\tItem Count: %d\n", s->count);
  printf("\tItem capacity: %d\n", s->capacity);
}

int main() {
  stack *my_stack = malloc(sizeof(stack));
  stack_init(my_stack, sizeof(int));
  printf("State after init:\n");
  print_stack_state(my_stack);

  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (int i = 0; i <= 9; i++) {
    stack_push(my_stack, arr + i);
  }

  printf("State after multiple push operations:\n");
  print_stack_state(my_stack);

  int *item = malloc(sizeof(int));
  for (int i = 0; i <= 9; i++) {
    stack_pop(my_stack, item);
    printf("Popped item: %d\n", *item);
  }
  free(item);

  printf("State after popping all items:\n");
  print_stack_state(my_stack);

  stack_free(my_stack);
  free(my_stack);

  return 0;
}
