/*
 * @file: 2.1_linear_search.c
 * @brief: Implements a simple linear search algorithm.
 * @compile: "clang -g -o 2.1_linear_search 2.1_linear_search.c"
 * @run: "./2.1_linear_search"
 */

#include <stdio.h>

int linear_search(int *arr, int len, int search_key) {
  for (int i = 0; i < len; i++) {
    if (arr[i] == search_key)
      return i;
  }
  return -1;
}

int main() {
  int array[] = {0,  1,  2,  3,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                 16, 17, 18, 19, 20, 21, 22, 24, 25, 27, 28, 29, 30, 31, 32,
                 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 45, 46, 47, 49, 50,
                 51, 54, 55, 58, 61, 62, 63, 64, 65, 66, 67, 70, 71, 74, 75,
                 76, 77, 79, 80, 83, 84, 87, 91, 92, 93, 94, 95, 97, 98, 99};

  int len = sizeof(array) / sizeof(array[0]);

  int sk = 79;
  int index = linear_search(array, len, sk);

  if (index != -1) {
    printf("Search key %d found at index %d\n", sk, index);
    printf("Verification: arr[%d] = %d\n", index, array[index]);
  } else {
    printf("Search key %d not found in array\n", sk);
  }

  return 0;
}
