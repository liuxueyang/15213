/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * TODO: fill in your name and Andrew ID
 * @author XXX <XXX@andrew.cmu.edu>
 */

#include "queue.h"
#include "harness.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
  queue_t *q = malloc(sizeof(queue_t));

  /* What if malloc returned NULL? */
  if (!q) {
    printf("malloc failed for queue_t in queue_new\n");
    return q;
  }

  q->head = NULL;
  q->tail = NULL;
  q->size = 0;

  return q;
}

/**
 * @brief Frees a list element and the value field
 *
 * @return next list element
 */
list_ele_t* list_ele_free(list_ele_t *item) {
  if (!item) return item;

  char *value = item->value;
  if (value) free(value);
  list_ele_t *next = item->next;
  return next;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
void queue_free(queue_t *q) {
  if (!q) return;

  list_ele_t *head = q->head;

  if (!head) {
    free(q);
    return;
  }

  /* How about freeing the list elements and the strings? */

  list_ele_t *cur = head;
  while (cur) {
    cur = list_ele_free(cur);
    q->size--;
  }

  /* Free queue structure */
  free(q);
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_head(queue_t *q, const char *s) {
  /* What should you do if the q is NULL? */
  if (!q) return false;

  list_ele_t *newh;
  newh = malloc(sizeof(list_ele_t));
  if (!newh) {
    printf("malloc for list_ele_t in queue_insert_head failed\n");
    return false;
  }

  /* Don't forget to allocate space for the string and copy it */
  /* What if either call to malloc returns NULL? */
  size_t len = strlen(s);
  if (len + 1 < len) {
    printf("length is too large and malloc size is overflow\n");
    free(newh);

    return false;
  }

  newh->value = malloc(len + 1);
  if (!newh->value) {
    printf("malloc for string in queue_insert_head failed\n");
    free(newh);

    return false;
  }

  strcpy(newh->value, s);

  newh->next = q->head;
  q->head = newh;

  if (!q->tail) {
    /* head and tail are the same element */
    q->tail = q->head;
  }

  if (q->size + 1 < q->size) {
    printf("Too many elements, queue_t.size overflow\n");
  } else {
    q->size++;
  }

  return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
  /* You need to write the complete code for this function */
  /* Remember: It should operate in O(1) time */
  if (!q) return false;

  list_ele_t *newh;
  newh = malloc(sizeof(list_ele_t));
  if (!newh) {
    printf("malloc for list_ele_t in queue_insert_tail failed\n");
    return false;
  }

  size_t len = strlen(s);
  if (len + 1 < len) {
    printf("length is too large and malloc size is overflow\n");
    free(newh);

    return false;
  }

  newh->value = malloc(len + 1);
  if (!newh->value) {
    printf("malloc for string in queue_insert_tail failed\n");
    free(newh);

    return false;
  }

  strcpy(newh->value, s);

  newh->next = NULL;

  list_ele_t *tail = q->tail;
  if (tail) {
    tail->next = newh;
  } else {
    /* head must be empty, so set it to point to tail */
    q->head = newh;
  }

  q->tail = newh;

  if (q->size + 1 < q->size) {
    printf("Too many elements, queue_t.size overflow\n");
  } else {
    q->size++;
  }

  return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
  if (!q) return false;

  list_ele_t *head = q->head;
  if (!head) return false;

  /* param checking */
  if (buf && !bufsize) {
    printf("bufsize should be nonzero\n");

    return false;
  }

  list_ele_t *next = head->next;

  /* There is only one element */
  if (q->head == q->tail) q->tail = next;

  q->head = next;

  if (buf) {
    strncpy(buf, head->value, bufsize - 1);
    buf[bufsize - 1] = '\0';
  }

  list_ele_free(head);

  q->size--;

  return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
  if (!q) return 0;
  if (!q->head) return 0;

  /* You need to write the code for this function */
  /* Remember: It should operate in O(1) time */
  return q->size;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
  if (!q) return;
  if (!q->head) return;

  list_ele_t *p1 = q->head, *p2 = p1->next, *p3;

  p1->next = NULL;

  while (p2) {
    p3 = p2->next;
    p2->next = p1;
    p1 = p2;
    p2 = p3;
  }

  q->tail = q->head;
  q->head = p1;
}
