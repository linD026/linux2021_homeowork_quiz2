#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_sort.h"

#define len 50
u16 values[len];
int main(int argc, char **argv) {
  // create
  struct list_head testlist;
  arrangement order = ascend;
  struct listitem *item, *is = NULL;
  size_t i;

  u16 seed = (uintptr_t)*argv;
  random_array(values, (u16)ARRAY_SIZE(values), seed);

  INIT_LIST_HEAD(&testlist);

  for (i = 0;i < ARRAY_SIZE(values);i++) {
    item = (struct listitem*)malloc(sizeof(struct listitem));
    item->i = values[i];
    list_add(&item->list, &testlist);
  }
  // sort
  list_sort(&order, &testlist, cmpu16);

  if(check(&testlist, &order, cmpu16))
    printf("in order\n");
  else 
    printf("not in order\n");

  print_check(&testlist);

  // free
  return 0;
}
