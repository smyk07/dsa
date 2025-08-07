/*
 * @file: 2.6_quick_sort.c
 * @brief: Implements a simple quick sort algorithm.
 * @compile: "clang -g -o 2.6_quick_sort 2.6_quick_sort.c"
 * @run: "./2.6_quick_sort"
 */

#include <stdio.h>

#ifndef swap //(x, y)
#define swap(x, y)                                                             \
  {                                                                            \
    int temp = x;                                                              \
    x = y;                                                                     \
    y = temp;                                                                  \
  }
#endif /* ifndef swap(x, y) */

int partition(int *arr, unsigned int low, unsigned int high) {
  int pivot = arr[high];
  int i = low - 1;

  for (int j = low; j < high; j++) {
    if (arr[j] <= pivot) {
      i++;
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i + 1], arr[high]);
  return i + 1;
}

void quick_sort(int *arr, unsigned int low, unsigned int high) {
  if (low < high) {
    int mid = partition(arr, low, high);
    if (mid > 0)
      quick_sort(arr, low, mid - 1);
    quick_sort(arr, mid + 1, high);
  }
}

int main() {
  int arr[] = {847, 123, 589, 312, 967, 634, 191, 456, 778, 245, 629, 883, 161,
               717, 394, 538, 472, 855, 226, 981, 714, 369, 892, 437, 658, 175,
               819, 286, 541, 764, 428, 695, 152, 873, 416, 587, 744, 271, 933,
               596, 259, 822, 485, 748, 376, 631, 968, 193, 554, 777, 415, 684,
               342, 879, 136, 763, 290, 857, 524, 488, 651, 374, 127, 982, 449,
               566, 839, 297, 760, 523, 618, 385, 946, 572, 235, 789, 462, 178,
               841, 694, 353, 276, 829, 187, 464, 591, 748, 375, 932, 283, 756,
               469, 142, 896, 659, 374, 537, 188, 261, 795};

  unsigned int len = sizeof(arr) / sizeof(arr[0]);

  unsigned int low = 0;
  unsigned int high = len - 1;

  quick_sort(arr, low, high);

  printf("Sorted array:\n");
  for (int i = 0; i < len; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  return 0;
}
