/*
 * @file: 2.2_binary_search.c
 * @brief: Implements a simple binary search algorithm.
 * @compile: "clang -g -o 2.2_binary_search 2.2_binary_search.c"
 * @run: "./2.2_binary_search"
 */

#include <stdio.h>

// Recursive
int binary_search(int *arr, int low, int high, int search_key) {
  if (low > high)
    return -1;

  int mid = (low + high) / 2;

  if (arr[mid] == search_key)
    return mid;

  if (arr[mid] < search_key) {
    low = mid + 1;
  } else {
    high = mid;
  }

  return binary_search(arr, low, high, search_key);
}

/*
// while-loop
int binary_search(int *arr, int low, int high, int search_key) {
  while (low <= high) {
    int mid = (low + high) / 2;

    if (arr[mid] == search_key)
      return mid;

    if (arr[mid] < search_key)
      low = mid + 1;
    else
      high = mid - 1;
  }

  return -1;
}
*/

int main() {
  int array[] = {0,  1,  2,  3,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                 16, 17, 18, 19, 20, 21, 22, 24, 25, 27, 28, 29, 30, 31, 32,
                 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 45, 46, 47, 49, 50,
                 51, 54, 55, 58, 61, 62, 63, 64, 65, 66, 67, 70, 71, 74, 75,
                 76, 77, 79, 80, 83, 84, 87, 91, 92, 93, 94, 95, 97, 98, 99};

  int low = 0;
  int high = (sizeof(array) / sizeof(array[0])) - 1;

  int sk = 79;
  int index = binary_search(array, low, high, sk);

  if (index != -1) {
    printf("Search key %d found at index %d\n", sk, index);
    printf("Verification: arr[%d] = %d\n", index, array[index]);
  } else {
    printf("Search key %d not found in array\n", sk);
  }

  return 0;
}
