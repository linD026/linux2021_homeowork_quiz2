#include <stdio.h>
#include <stdlib.h>

//#include "dtusage.h"
#include "list.h"
#include "list_sort.h"

#define len 1000000
#define times 1

u16 set_rand(void){
  time_t current_time;
  srandom(time(&current_time));
  return self_random(random() % 1024, random() % 1024);
}

#define time_diff(start, end) (end.tv_nsec - start.tv_nsec < 0 ? (1000000000 + end.tv_nsec - start.tv_nsec): (end.tv_nsec - start.tv_nsec) )

int main(void) {
    printf("len:%d  time:%d\n", len, times);

    arrangement order = ascend;
    struct list_head testlist;
    INIT_LIST_HEAD(&testlist);
    struct listitem *item, *is = NULL;
    size_t i;
    for (i = 0;i < len;i++) {
        item = (struct listitem*)malloc(sizeof(struct listitem));
        list_add(&item->list, &testlist);
    }

    for (int time_i = 0;time_i < times;time_i++) {
        //printf("%d\n", time_i);
        list_for_each_entry_safe(item, is, &testlist, list) {
            item->i = set_rand();
        }

        list_sort(&order, &testlist, cmpu16);
    }
}

