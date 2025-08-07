/*
 * @file: singly_linked_list.c
 * @brief: Implements a singly linked list and multiple functions around it.
 */

#include <stdlib.h>

enum sll_errors {
  SLL_SUCCESS = 0,
  SLL_ERR_NULL,
  SLL_ERR_INDEX,
  SLL_ERR_UNINIT,
  SLL_ERR_ALLOC
};

char *sll_get_error_string(enum sll_errors error) {
  switch (error) {
  case SLL_SUCCESS:
    return "SUCCESS";
  case SLL_ERR_NULL:
    return "NULL_PARAMETER";
  case SLL_ERR_INDEX:
    return "INDEX_ERROR";
  case SLL_ERR_UNINIT:
    return "UNINITIALIZED";
  case SLL_ERR_ALLOC:
    return "ALLOCATION_ERROR";
  default:
    return "UNKNOWN_ERROR";
  }
}

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

typedef struct List {
  Node *head;
  Node *tail;
  size_t length;
} List;

Node *sll_create_node(void *data) {
  Node *newnode = malloc(sizeof(Node));
  if (newnode == NULL)
    return NULL;

  newnode->data = data;
  newnode->next = NULL;

  return newnode;
}

int sll_list_init(List *list, void *data) {
  list->head = sll_create_node(data);

  if (list->head == NULL) {
    return SLL_ERR_ALLOC;
  }

  list->tail = list->head;
  list->length = 1;

  return SLL_SUCCESS;
}

Node *sll_get_at_index(List *list, size_t index) {
  if (!list || list->head == NULL || list->length == 0 ||
      index >= list->length) {
    return NULL;
  }

  Node *node = list->head;

  size_t pos = 0;
  while (pos < index) {
    node = node->next;
    pos++;
  }

  return node;
}

int sll_prepend_node(List *list, void *data) {
  if (!data || !list)
    return SLL_ERR_NULL;

  if (!list || list->head == NULL || list->length == 0)
    return SLL_ERR_UNINIT;

  Node *newnode = sll_create_node(data);
  if (newnode == NULL)
    return SLL_ERR_ALLOC;

  newnode->next = list->head;
  list->head = newnode;
  list->length++;
  return SLL_SUCCESS;
}

int sll_append_node(List *list, void *data) {
  if (!data || !list)
    return SLL_ERR_NULL;

  if (!list || list->head == NULL || list->length == 0)
    return SLL_ERR_UNINIT;

  Node *newnode = sll_create_node(data);
  if (newnode == NULL)
    return SLL_ERR_ALLOC;

  list->tail->next = newnode;
  list->tail = newnode;
  list->length++;

  return SLL_SUCCESS;
}

int sll_insert_node(List *list, size_t index, void *data) {
  if (!data || !list)
    return SLL_ERR_NULL;

  if (!list || list->head == NULL || list->length == 0)
    return SLL_ERR_UNINIT;

  if (index == 0) {
    return sll_prepend_node(list, data);
  } else if (index > list->length) {
    return SLL_ERR_INDEX;
  }

  Node *prev_node = list->head;
  size_t prev_node_pos = 0;

  while (prev_node_pos < index - 1) {
    prev_node = prev_node->next;
    prev_node_pos++;
  }

  Node *at_index = sll_get_at_index(list, index);
  Node *inserted_node = sll_create_node(data);
  if (inserted_node == NULL)
    return SLL_ERR_ALLOC;

  prev_node->next = inserted_node;
  inserted_node->next = at_index;

  if (index == list->length) {
    list->tail = inserted_node;
  }

  list->length++;
  return SLL_SUCCESS;
}

int sll_delete_head(List *list) {
  if (!list)
    return SLL_ERR_NULL;

  if (!list || list->head == NULL || list->length == 0)
    return SLL_ERR_UNINIT;

  Node *head = list->head;
  list->head = head->next;

  if (list->length == 1) {
    list->tail = NULL;
  }

  free(head);
  list->length--;
  return SLL_SUCCESS;
}

int sll_delete_tail(List *list) {
  if (!list)
    return SLL_ERR_NULL;

  if (!list || list->head == NULL || list->length == 0)
    return SLL_ERR_UNINIT;

  if (list->length == 1) {
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
  } else {
    Node *new_tail = list->head;
    while (new_tail->next->next != NULL) {
      new_tail = new_tail->next;
    }
    free(new_tail->next);
    new_tail->next = NULL;
    list->tail = new_tail;
  }

  list->length--;
  return SLL_SUCCESS;
}

int sll_delete_at_index(List *list, size_t index) {
  if (!list)
    return SLL_ERR_NULL;

  if (!list || list->head == NULL || list->length == 0)
    return SLL_ERR_UNINIT;

  if (index >= list->length) {
    return SLL_ERR_INDEX;
  }

  if (index == 0) {
    return sll_delete_head(list);
  } else if (index == list->length - 1) {
    return sll_delete_tail(list);
  } else if (index >= list->length) {
    return SLL_ERR_INDEX;
  }

  Node *prev_node = list->head;
  size_t prev_node_pos = 0;

  while (prev_node_pos < index - 1) {
    prev_node = prev_node->next;
    prev_node_pos++;
  }

  Node *at_index = sll_get_at_index(list, index);
  Node *next_node = sll_get_at_index(list, index + 1);
  prev_node->next = next_node;

  free(at_index);
  list->length--;
  return SLL_SUCCESS;
}

int sll_free_chain(Node *node) {
  while (node != NULL) {
    Node *next = node->next;
    free(node);
    node = next;
  }
  return SLL_SUCCESS;
}

int sll_free_list(List *list) {
  if (!list)
    return SLL_ERR_NULL;

  if (list->head != NULL) {
    sll_free_chain(list->head);
  }

  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return SLL_SUCCESS;
}
