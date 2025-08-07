/*
 * @file: stacks.c
 * @brief: Implements a stack data type, built on top of a dynamic array.
 */

#include <stdlib.h>
#include <string.h>

enum stack_errors {
  STACK_SUCCESS = 0,
  STACK_ERR_NULL,
  STACK_ERR_INDEX,
  STACK_ERR_UNINIT,
  STACK_ERR_ALLOC,
  STACK_ERR_RESIZE,
  STACK_ERR_EMPTY
};

char *stack_get_error_string(enum stack_errors error) {
  switch (error) {
  case STACK_SUCCESS:
    return "SUCCESS";
  case STACK_ERR_NULL:
    return "NULL_PARAMETER";
  case STACK_ERR_INDEX:
    return "INDEX_ERROR";
  case STACK_ERR_UNINIT:
    return "UNINITIALIZED";
  case STACK_ERR_ALLOC:
    return "ALLOCATION_ERROR";
  case STACK_ERR_RESIZE:
    return "RESIZE_ERROR";
  case STACK_ERR_EMPTY:
    return "STACK_EMPTY";
  default:
    return "UNKNOWN_ERROR";
  }
}

typedef struct stack {
  void *items;
  size_t item_size;
  size_t count;
  size_t capacity;
} stack;

#define STACK_INITIAL_CAPACITY 4
#define STACK_RESIZE_FACTOR 2

int stack_init(stack *s, size_t item_size) {
  if (!s)
    return STACK_ERR_NULL;
  s->item_size = item_size;
  s->capacity = STACK_INITIAL_CAPACITY;
  s->items = malloc(s->item_size * s->capacity);
  if (!s->items) {
    s->capacity = 0;
    return STACK_ERR_ALLOC;
  }
  s->count = 0;
  return STACK_SUCCESS;
}

int stack_expand(stack *s) {
  if (!s)
    return STACK_ERR_NULL;
  if (!s->items)
    return STACK_ERR_UNINIT;
  size_t new_capacity = s->capacity * STACK_RESIZE_FACTOR;
  void *new_items = realloc(s->items, s->item_size * new_capacity);
  if (!new_items)
    return STACK_ERR_ALLOC;
  s->items = new_items;
  s->capacity = new_capacity;
  return STACK_SUCCESS;
}

int stack_shrink(stack *s) {
  if (!s)
    return STACK_ERR_NULL;
  if (!s->items)
    return STACK_ERR_UNINIT;
  size_t new_capacity = s->capacity / STACK_RESIZE_FACTOR;
  if (new_capacity < STACK_INITIAL_CAPACITY) {
    new_capacity = STACK_INITIAL_CAPACITY;
  }
  void *new_items = realloc(s->items, new_capacity * s->item_size);
  if (new_items == NULL)
    return STACK_ERR_ALLOC;
  s->items = new_items;
  s->capacity = new_capacity;
  return STACK_SUCCESS;
}

int stack_push(stack *s, void *item) {
  if (!s || !item)
    return STACK_ERR_NULL;

  if (!s->items || s->item_size == 0)
    return STACK_ERR_UNINIT;

  if (s->count == s->capacity)
    if (stack_expand(s) != 0)
      return STACK_ERR_RESIZE;

  memcpy((char *)s->items + (s->count * s->item_size), item, s->item_size);
  s->count++;
  return STACK_SUCCESS;
}

int stack_pop(stack *s, void *item) {
  if (!s || !item)
    return STACK_ERR_NULL;

  if (!s->items || s->item_size == 0)
    return STACK_ERR_UNINIT;

  if (s->count == 0)
    return STACK_ERR_EMPTY;

  memcpy(item, (char *)s->items + ((s->count - 1) * s->item_size),
         s->item_size);
  s->count--;

  if (s->count < (s->capacity / STACK_RESIZE_FACTOR))
    if (stack_shrink(s) != 0)
      return STACK_ERR_RESIZE;

  return STACK_SUCCESS;
}

void stack_free(stack *s) {
  if (!s)
    return;
  free(s->items);
  s->items = NULL;
  s->item_size = 0;
  s->count = 0;
  s->capacity = 0;
}

#undef STACK_INITIAL_CAPACITY
#undef STACK_RESIZE_FACTOR
