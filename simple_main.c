#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_sort.h"

#define len 1000000
u16 values[len];
int main(int argc, char **argv) {
  // create
  struct list_head testlist;
  arrangement order = ascend;
  struct listitem *item, *is = NULL;
  size_t i;

  u16 seed = (uintptr_t)*argv;
  INIT_LIST_HEAD(&testlist);
  random_u16(&testlist, len, seed);
  // sort
  list_sort(&order, &testlist, cmpu16);

  if(check(&testlist, &order, cmpu16))
    printf("in order\n");
  else 
    printf("not in order\n");

  // free
  return 0;
}
