#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "item_data.h"

struct list_item {
  struct list_item *next;
  struct list_item *prev;
  struct item_data data;
};

struct linked_list {
  struct list_item *head;
  int length;
};

struct linked_list *init_list(void);
struct list_item *find_by_position(struct linked_list *list, unsigned position);
int add_item_to_position(struct linked_list *list, struct item_data *data,
                         unsigned position);
int add_item_to_end(struct linked_list *list, struct item_data *data);
int delete_item_by_position(struct linked_list *list, int position);
void map_list(struct linked_list *list,
              int (*foreach_function)(void *list_item, void *args),
              void *foreach_args);

#endif
