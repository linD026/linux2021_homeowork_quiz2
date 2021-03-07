#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_sort.h"

#define len 10000
#define times 1000

u16 set_rand(void){
  time_t current_time;
  srandom(time(&current_time));
  return self_random(random() % 1024, random() % 1024);
}

#define time_diff(start, end) (end.tv_nsec - start.tv_nsec < 0 ? (1000000000 + end.tv_nsec - start.tv_nsec): (end.tv_nsec - start.tv_nsec) )

void analysis(void) {
    FILE *ptr = NULL;
    ptr = fopen("list_sort.txt", "w"); if(!ptr) return;
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

    struct timespec time_start;
    struct timespec time_end;
    double during;
    for (int time_i = 0;time_i < times;time_i++) {
        //printf("%d\n", time_i);
        list_for_each_entry_safe(item, is, &testlist, list) {
            item->i = set_rand();
        }
        
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        list_sort(&order, &testlist, cmpu16);
        clock_gettime(CLOCK_MONOTONIC, &time_end);  
        during = time_diff(time_start, time_end);
        if(check(&testlist, &order, cmpu16)) {
          printf("%d check\n", time_i);
          fprintf(ptr, "%d %f\n" , time_i, during);
        }
    }
    fclose(ptr);
}

int main(int argc, char **argv) {
  analysis();
}