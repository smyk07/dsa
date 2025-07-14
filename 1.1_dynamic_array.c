/*
 * @file: 1.1_dynamic_array.c
 * @brief: Implements a dynamic array data type with multiple functions around
 * it.
 * @compile: "clang -g -o 1.1_dynamic_array 1.1_dynamic_array.c"
 * @run: "./1.1_dynamic_array"
 * @leak checks: "valgrind --leak-check=full ./1.1_dynamic_array"
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DA_INITIAL_CAPACITY 4
#define DA_RESIZE_FACTOR 2

typedef struct dynamic_array {
  void *items;
  size_t item_size;
  unsigned int count;
  unsigned int capacity;
} dynamic_array;

/*
 * Initializes the dynamic_array.
 */
void da_init(dynamic_array *da, size_t size) {
  da->items = NULL;
  da->item_size = size;
  da->count = 0;
  da->capacity = 0;
}

/*
 * Gets the element at index, and 'memcpy's the element to item.
 */
int da_get_item(dynamic_array *da, unsigned int index, void *item) {
  if (!da || !item || index >= da->count || !da->items) {
    perror("Invalid Dynamic array passed to function.\n");
    return -1;
  }

  void *src = (char *)da->items + (index * da->item_size);
  memcpy(item, src, da->item_size);
  return 0;
}

/*
 * Gets the destination at index, then 'memcpy's the given item to the
 * destination.
 */
int da_set_item(dynamic_array *da, unsigned int index, void *item) {
  if (!da || !item || !da->items || da->item_size == 0 || index >= da->count ||
      da->capacity == 0) {
    perror("Invalid dynamic array passed to function.\n");
    return -1;
  }

  void *dest = (char *)da->items + (index * da->item_size);
  memcpy(dest, item, da->item_size);
  return 0;
}

/*
 * If the capacity is 0, sets the initial capacity to 4 and allocates memory.
 * If count == capacity, doubles the capacity and allocates new memory.
 * 'memcpy's the given item to the end of the current buffer and increments
 * count.
 */
int da_append(dynamic_array *da, void *item) {
  if (!da || !item || da->item_size == 0) {
    perror("Invalid dynamic array passed to function.\n");
    return -1;
  }

  if (da->capacity == 0) {
    da->capacity = DA_INITIAL_CAPACITY;
    da->items = malloc(da->item_size * da->capacity);
    if (!da->items) {
      perror("Failed to allocate dynamic array.\n");
      return -1;
    }
  }

  if (da->count == da->capacity) {
    unsigned int new_capacity = da->capacity * DA_RESIZE_FACTOR;
    void *new_items = realloc(da->items, da->item_size * new_capacity);
    if (!new_items) {
      perror("Failed to resize dynamic array.\n");
      return -1;
    }
    da->items = new_items;
    da->capacity = new_capacity;
  }

  memcpy((char *)da->items + (da->count * da->item_size), item, da->item_size);
  da->count++;
  return 0;
}

/*
 * If count == capacity, doubles the capacity and allocates new memory.
 * Moves the items from arr[index] to arr[count - 1] to arr[index + 1].
 * 'memcpy's given item to 'index'.
 */
int da_insert_item(dynamic_array *da, unsigned int index, void *item) {
  if (!da || !item || da->item_size == 0 || index > da->count) {
    perror("Invalid dynamic array passed to function.\n");
    return -1;
  }

  if (da->count == da->capacity) {
    unsigned int new_capacity = da->capacity * DA_RESIZE_FACTOR;
    void *new_items = realloc(da->items, da->item_size * new_capacity);
    if (!new_items) {
      perror("Failed to resize dynamic array.\n");
      return -1;
    }
    da->items = new_items;
    da->capacity = new_capacity;
  }

  memmove((char *)da->items + (index * da->item_size),
          (char *)da->items + (index * da->item_size) + da->item_size,
          (da->count * da->item_size) - ((da->count - index) * da->item_size));
  memcpy((char *)da->items + (index * da->item_size), item, da->item_size);
  da->count++;
  return 0;
}

/*
 * If index != count - 1, moves the elements from arr[index + 1] to arr[index]
 * and decrements count.
 * If index == count - 1 (referring to the last element),
 * only count is decremented.
 */
int da_remove_item(dynamic_array *da, unsigned int index) {
  if (!da || da->item_size == 0 || index >= da->count) {
    perror("Invalid dynamic array passed to function.\n");
    return -1;
  }

  if (index != da->count - 1) {
    memmove((char *)da->items + (index * da->item_size),
            (char *)da->items + (index * da->item_size) + da->item_size,
            (da->count - index - 1) * da->item_size);
  }

  da->count--;
  return 0;
}

/*
 * Gets the last item from the array and assigns it to given item ptr and
 * decrements count.
 */
int da_pop_item(dynamic_array *da, void *item) {
  if (!da || da->item_size == 0 || da->count == 0) {
    perror("Invalid dynamic array passed to function.\n");
    return -1;
  }

  int return_value = da_get_item(da, da->count - 1, item);
  if (return_value != 0)
    return return_value;

  da->count--;
  return 0;
}

/*
 * Frees the allocated array, sets it to null, and sets all unsigned int and
 * size_t values to 0.
 */
void da_free(dynamic_array *da) {
  if (!da)
    return;
  free(da->items);
  da->items = NULL;
  da->count = 0;
  da->capacity = 0;
  da->item_size = 0;
}

/*
 * Prints the current state of the array.
 */
void da_print_state(dynamic_array *da) {
  printf("\tItem Size: %zu\n", da->item_size);
  printf("\tCount: %u\n", da->count);
  printf("\tCapacity: %u\n", da->capacity);
}

int main() {
  // Create and initialize array
  printf("Create and initialize array:\n");
  dynamic_array array;
  da_init(&array, sizeof(int));
  da_print_state(&array);

  // Append an element
  printf("Append an element and print the value:\n");
  int a = 2;
  da_append(&array, &a);
  da_print_state(&array);
  int ptr;
  da_get_item(&array, 0, &ptr);
  printf("\tValue at arr[0] = %d\n", ptr);

  // Update value of an element
  printf("Update the value of an element:\n");
  int b = 16;
  da_set_item(&array, 0, &b);
  da_print_state(&array);
  da_get_item(&array, 0, &ptr);
  printf("\tValue at arr[0] = %d\n", ptr);

  // Append multiple elements - check if the array resizes
  printf("Append multiple elements:\n");
  da_append(&array, &a);
  da_append(&array, &a);
  da_append(&array, &a);
  da_append(&array, &a);
  da_append(&array, &a);
  da_append(&array, &a);
  da_append(&array, &a);
  da_append(&array, &a);
  da_print_state(&array);

  // Insert an item
  printf("Insert an Item:\n");
  da_get_item(&array, 3, &ptr);
  printf("\tValue at arr[3] before insert = %d\n", ptr);
  da_insert_item(&array, 3, &b);
  da_get_item(&array, 3, &ptr);
  printf("\tValue at arr[3] after insert = %d\n", ptr);
  da_print_state(&array);

  // Remove an item
  printf("Remove an Item:\n");
  da_get_item(&array, 3, &ptr);
  printf("\tValue at arr[3] before remove = %d\n", ptr);
  da_remove_item(&array, 3);
  da_get_item(&array, 3, &ptr);
  printf("\tValue at arr[3] after remove = %d\n", ptr);
  da_print_state(&array);

  // Free the dynamic array
  printf("Free the dynamic array:\n");
  da_free(&array);
  da_print_state(&array);

  return 0;
}
