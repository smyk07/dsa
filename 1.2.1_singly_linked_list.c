/*
 * @file: 1.2.1_singly_linked_list.c
 * @brief: Implements a singly linked list and multiple functions around it.
 * @compile: "clang -g -o 1.2.1_singly_linked_list 1.2.1_singly_linked_list.c"
 * @run: "./1.2.1_singly_linked_list"
 * @leak checks: "valgrind --leak-check=full ./1.2_linked_list"
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

typedef struct List {
  Node *head;
  unsigned int length;
} List;

/*
 * Malloc's a new node, initializes data, sets next to NULL and returns.
 */
Node *create_node(void *data) {
  Node *newnode = malloc(sizeof(Node));
  newnode->data = data;
  newnode->next = NULL;
  return newnode;
}

/*
 * Initializes the list struct.
 */
void list_init(List *list, void *data) {
  list->head = create_node(data);
  list->length = 1;
}

/*
 * Traverses the list and returns the last element, or the tail.
 */
Node *get_tail(List *list) {
  if (list->head == NULL)
    return NULL;
  Node *tail = list->head;
  while (tail->next != NULL) {
    tail = tail->next;
  }
  return tail;
}

/*
 * Traverses the list until reaching a specific index in the list, and returns
 * the element at that index.
 */
Node *get_at_index(List *list, unsigned int index) {
  if (index >= list->length) {
    fprintf(stderr, "Index out of bounds\n");
    exit(1);
  }

  Node *node = list->head;

  unsigned int pos = 0;
  while (pos < index) {
    node = node->next;
    pos++;
  }

  return node;
}

/*
 * Inserts a node at the beginning of the list.
 */
void prepend_node(List *list, void *data) {
  Node *newnode = create_node(data);
  newnode->next = list->head;

  list->head = newnode;
  list->length++;
}

/*
 * Inserts a node at the end of the list.
 */
void append_node(List *list, void *data) {
  Node *newnode = create_node(data);
  Node *tail = get_tail(list);

  tail->next = newnode;
  list->length++;
}

/*
 * Inserts a node at the given index in the list.
 */
void insert_node(List *list, unsigned int index, void *data) {
  Node *prev_node = list->head;
  unsigned int prev_node_pos = 0;

  if (index == 0) {
    prepend_node(list, data);
    return;
  } else if (index >= list->length) {
    fprintf(stderr, "Index out of bounds\n");
    exit(1);
  }

  while (prev_node_pos < index - 1) {
    prev_node = prev_node->next;
    prev_node_pos++;
  }

  Node *at_index = get_at_index(list, index);

  Node *inserted_node = create_node(data);

  prev_node->next = inserted_node;
  inserted_node->next = at_index;

  list->length++;
}

/*
 * Delets the head or the first element of the list.
 */
void delete_head(List *list) {
  if (list->length == 0)
    return;

  Node *head = list->head;

  list->head = head->next;

  free(head);
  list->length--;
}

/*
 * Deletes the tail or the last element of the list.
 */
void delete_tail(List *list) {
  if (list->length == 0)
    return;

  if (list->length == 1) {
    free(list->head);
    list->head = NULL;
  } else {
    Node *new_tail = list->head;
    while (new_tail->next->next != NULL) {
      new_tail = new_tail->next;
    }
    free(new_tail->next);
    new_tail->next = NULL;
  }

  list->length--;
}

/*
 * Deletes the node present at the given index in the list.
 */
void delete_at_index(List *list, unsigned int index) {
  Node *prev_node = list->head;
  unsigned int prev_node_pos = 0;

  if (index == 0) {
    delete_head(list);
    return;
  } else if (index == list->length - 1) {
    delete_tail(list);
    return;
  } else if (index >= list->length) {
    fprintf(stderr, "Index greater than list length\n");
    exit(1);
  }

  while (prev_node_pos < index - 1) {
    prev_node = prev_node->next;
    prev_node_pos++;
  }

  Node *at_index = get_at_index(list, index);

  Node *next_node = get_at_index(list, index + 1);

  prev_node->next = next_node;

  free(at_index);
  list->length--;
}

/*
 * Walks down a node and frees all of them.
 */
void free_chain(Node *node) {
  while (node != NULL) {
    Node *next = node->next;
    free(node);
    node = next;
  }
}

/*
 * Frees the whole list.
 */
void free_list(List *list) {
  free_chain(list->head);
  list->head = NULL;
  list->length = 0;
}

int main() {
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  int e = 5;
  int f = 6;
  int g = 7;
  int h = 8;
  int num = 16;

  List list;
  list_init(&list, &a);
  append_node(&list, &b);
  append_node(&list, &c);
  append_node(&list, &d);
  append_node(&list, &e);
  append_node(&list, &f);
  append_node(&list, &g);
  append_node(&list, &h);

  // Print head
  printf("Head: %d\n", *(int *)list.head->data);

  // Print tail
  Node *node = get_tail(&list);
  printf("Tail: %d\n", *(int *)node->data);
  printf("Length: %d\n\n", list.length);

  // Insert a node at index
  node = get_at_index(&list, 1);
  printf("List[1] (before insert): %d\n", *(int *)node->data);
  printf("Length (before insert): %d\n", list.length);
  insert_node(&list, 1, &num);
  node = get_at_index(&list, 1);
  printf("List[1] (after insert): %d\n", *(int *)node->data);
  printf("Length (after insert): %d\n\n", list.length);

  // Delete head
  printf("Head (before delete): %d\n", *(int *)list.head->data);
  printf("Length (before delete): %d\n", list.length);
  delete_head(&list);
  printf("Head (after delete): %d\n", *(int *)list.head->data);
  printf("Length (after delete): %d\n\n", list.length);

  // Delete tail
  node = get_tail(&list);
  printf("Tail (before delete): %d\n", *(int *)node->data);
  printf("Length (before delete): %d\n", list.length);
  delete_tail(&list);
  node = get_tail(&list);
  printf("Tail (after delete): %d\n", *(int *)node->data);
  printf("Length (after delete): %d\n\n", list.length);

  // Delete at index
  node = get_at_index(&list, 1);
  printf("List[1] (before delete): %d\n", *(int *)node->data);
  printf("Length (before delete): %d\n", list.length);
  delete_at_index(&list, 1);
  node = get_at_index(&list, 1);
  printf("List[1] (after delete): %d\n", *(int *)node->data);
  printf("Length (after delete): %d\n\n", list.length);

  // Free
  free_list(&list);
  printf("List freed\n");
  printf("Length: %d\n", list.length);

  return 0;
}
