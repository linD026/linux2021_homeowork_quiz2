#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#define u8 uint8_t
#define u16 uint16_t
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member)                                        \
  ({                                                                           \
    const typeof(((type *)0)->member) *__mptr = (ptr);                         \
    (type *)((char *)__mptr - offsetof(type, member));                         \
  })

struct list_head {
  struct list_head *next, *prev;
};

struct listitem {
  u16 i;
  struct list_head list;
};

static inline void INIT_LIST_HEAD(struct list_head *list) {
  list->next = list;
  list->prev = list;
}

static inline void list_add(struct list_head *new, struct list_head *head) {
  new->prev = head;
  new->next = head->next;
  head->next->prev = new;
  head->next = new;
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member)                                    \
  list_entry((ptr)->next, type, member)

/**
 * list_next_entry - get the next element in list
 * @pos:	the type * to cursor
 * @member:	the name of the list_head within the struct.
 */
#define list_next_entry(pos, member)                                           \
  list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_entry_is_head - test if the entry points to the head of the list
 * @pos:	the type * to cursor
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry_is_head(pos, head, member) (&pos->member == (head))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against
 * removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)                         \
  for (pos = list_first_entry(head, typeof(*pos), member),                     \
      n = list_next_entry(pos, member);                                        \
       !list_entry_is_head(pos, head, member);                                 \
       pos = n, n = list_next_entry(n, member))

/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop cursor.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define list_for_each_safe(pos, n, head)                                       \
  for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)

typedef enum { ascend, descend } arrangement;

static inline int cmpu16(void *arrange, const struct list_head *a, const struct list_head *b) {
  const u16 ai = (const u16)container_of(a, struct listitem, list)->i;
  const u16 bi = (const u16)container_of(b, struct listitem, list)->i;
  const arrangement *arr_temp = (arrangement *)arrange;

  if (*arr_temp == descend)
    return bi - ai;
  else
    return ai - bi;
}

// Lagged Fibonacci generators
// Sn ≡ Sn−j ⋆ Sn−k (modm), 0 < j < k
static inline int self_random(int seed_f, int seed_s) {
  int sn_1 = 0;
  int sn_2 = 1;
  int sn = random() % 1024;
  int i = seed_f;
  while (i > 0) {
    sn = (sn_2 & sn_1) % (seed_s + 1);
    sn_1 = sn_1 + 3;
    sn_2 = sn_2 + 7;
    i--;
  }
  return sn;
}

static inline void random_array(u16 values[], u16 size, u16 seed) {
  time_t current_time;
  srandom(seed & time(&current_time));
  for (u16 i = 0; i < size; i++) {
    values[i] = self_random(random() % 1024, random() % 1024);
  }
}

/*
 * Check
 */
static inline int check(struct list_head *head, void *piv,
          int (*cmp_func)(void *, const struct list_head *,
                          const struct list_head *)) {
  struct list_head *item, *is = NULL;
  u16 det = true;
  const struct list_head *value = NULL;
  list_for_each_safe(item, is, head) {
    if (det) {
      value = item;
      det = false;
    } else {
      if (cmp_func(piv, value, item) > 0 && *(arrangement *)piv == ascend)
        return false;
      value = item;
    }
  }
  return true;
}

#define print_check(head)                                                      \
  do {                                                                         \
    struct listitem *item, *is = NULL;                                         \
    list_for_each_entry_safe(item, is, head, list) { printf("%d ", item->i); } \
    printf("\n");                                                              \
  } while (0)

#endif