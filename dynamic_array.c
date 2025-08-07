/*
 * @file: dynamic_array.c
 * @brief: Implements a dynamic array data type with multiple functions around
 * it.
 */

#include <stdlib.h>
#include <string.h>

enum da_errors {
  DA_SUCCESS = 0,
  DA_ERR_NULL,
  DA_ERR_INDEX,
  DA_ERR_UNINIT,
  DA_ERR_ALLOC,
  DA_ERR_RESIZE,
  DA_ERR_EMPTY
};

char *sll_get_error_string(enum da_errors error) {
  switch (error) {
  case DA_SUCCESS:
    return "SUCCESS";
  case DA_ERR_NULL:
    return "NULL_PARAMETER";
  case DA_ERR_INDEX:
    return "INDEX_ERROR";
  case DA_ERR_UNINIT:
    return "UNINITIALIZED";
  case DA_ERR_ALLOC:
    return "ALLOCATION_ERROR";
  case DA_ERR_RESIZE:
    return "RESIZE_ERROR";
  case DA_ERR_EMPTY:
    return "STACK_EMPTY";
  default:
    return "UNKNOWN_ERROR";
  }
}

typedef struct dynamic_array {
  void *items;
  size_t item_size;
  size_t count;
  size_t capacity;
} dynamic_array;

#define DA_INITIAL_CAPACITY 4
#define DA_RESIZE_FACTOR 2

int da_init(dynamic_array *da, size_t size) {
  if (!da)
    return DA_ERR_NULL;
  da->item_size = size;
  da->capacity = DA_INITIAL_CAPACITY;
  da->items = malloc(da->item_size * da->capacity);
  if (!da->items) {
    da->capacity = 0;
    return DA_ERR_ALLOC;
  }
  da->count = 0;
  return DA_SUCCESS;
}

int da_expand(dynamic_array *da) {
  if (!da)
    return DA_ERR_NULL;
  if (!da->items)
    return DA_ERR_UNINIT;
  size_t new_capacity = da->capacity * DA_RESIZE_FACTOR;
  void *new_items = realloc(da->items, da->item_size * new_capacity);
  if (!new_items)
    return DA_ERR_ALLOC;
  da->items = new_items;
  da->capacity = new_capacity;
  return DA_SUCCESS;
}

int da_shrink(dynamic_array *da) {
  if (!da)
    return DA_ERR_NULL;
  if (!da->items)
    return DA_ERR_UNINIT;
  size_t new_capacity = da->capacity / DA_RESIZE_FACTOR;
  if (new_capacity < DA_INITIAL_CAPACITY) {
    new_capacity = DA_INITIAL_CAPACITY;
  }
  void *new_items = realloc(da->items, new_capacity * da->item_size);
  if (new_items == NULL)
    return DA_ERR_ALLOC;
  da->items = new_items;
  da->capacity = new_capacity;
  return DA_SUCCESS;
}

int da_get_item(dynamic_array *da, size_t index, void *item) {
  if (!da || !item)
    return DA_ERR_NULL;
  if (index >= da->count)
    return DA_ERR_INDEX;
  if (!da->items)
    return DA_ERR_UNINIT;

  void *src = (char *)da->items + (index * da->item_size);
  memcpy(item, src, da->item_size);
  return DA_SUCCESS;
}

int da_set_item(dynamic_array *da, size_t index, void *item) {
  if (!da || !item)
    return DA_ERR_NULL;
  if (index >= da->count)
    return DA_ERR_INDEX;
  if (!da->items)
    return DA_ERR_UNINIT;

  void *dest = (char *)da->items + (index * da->item_size);
  memcpy(dest, item, da->item_size);
  return DA_SUCCESS;
}

int da_push(dynamic_array *da, void *item) {
  if (!da || !item)
    return DA_ERR_NULL;
  if (!da->items || da->item_size == 0)
    return DA_ERR_UNINIT;

  if (da->count == da->capacity)
    if (da_expand(da) != 0)
      return DA_ERR_RESIZE;

  memcpy((char *)da->items + (da->count * da->item_size), item, da->item_size);
  da->count++;
  return DA_SUCCESS;
}

int da_insert_item(dynamic_array *da, size_t index, void *item) {
  if (!da || !item)
    return DA_ERR_NULL;
  if (index >= da->count)
    return DA_ERR_INDEX;
  if (!da->items || da->item_size == 0)
    return DA_ERR_UNINIT;

  if (da->count == da->capacity) {
    if (da_expand(da) != DA_SUCCESS)
      return DA_ERR_RESIZE;
  }

  memmove((char *)da->items + ((index + 1) * da->item_size),
          (char *)da->items + (index * da->item_size),
          (da->count - index) * da->item_size);
  memcpy((char *)da->items + (index * da->item_size), item, da->item_size);
  da->count++;
  return DA_SUCCESS;
}

int da_remove_item(dynamic_array *da, size_t index) {
  if (!da)
    return DA_ERR_NULL;
  if (index >= da->count)
    return DA_ERR_INDEX;
  if (!da->items || da->item_size == 0)
    return DA_ERR_UNINIT;

  if (index != da->count - 1) {
    memmove((char *)da->items + (index * da->item_size),
            (char *)da->items + (index * da->item_size) + da->item_size,
            (da->count - index - 1) * da->item_size);
  }

  da->count--;

  if (da->count < (da->capacity / DA_RESIZE_FACTOR))
    if (da_shrink(da) != 0)
      return DA_ERR_RESIZE;

  return DA_SUCCESS;
}

int da_pop_item(dynamic_array *da, void *item) {
  if (!da)
    return DA_ERR_NULL;
  if (!da->items || da->item_size == 0)
    return DA_ERR_UNINIT;
  if (da->count == 0)
    return DA_ERR_EMPTY;

  int return_value = da_get_item(da, da->count - 1, item);
  if (return_value != 0)
    return return_value;

  da->count--;

  if (da->count < (da->capacity / DA_RESIZE_FACTOR))
    if (da_shrink(da) != 0)
      return DA_ERR_RESIZE;

  return DA_SUCCESS;
}

void da_free(dynamic_array *da) {
  if (!da)
    return;
  free(da->items);
  da->items = NULL;
  da->count = 0;
  da->capacity = 0;
  da->item_size = 0;
}

#undef DA_INITIAL_CAPACITY
#undef DA_RESIZE_FACTOR
