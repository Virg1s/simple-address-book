#ifndef CONTACT_H_
#define CONTACT_H_

#include "item_data.h"
#include "linked_list.h"

struct foreach_find_item_args {
  struct item_data *data;
  struct list_item *match_address;
};

void init_contact(struct item_data *item, char *fields[static 4]);
struct list_item *match_item_by_data(struct linked_list *list,
                                     struct item_data *data);
int foreach_print_item(void *void_item_pointer, void *args);
int foreach_delete_item(void *void_item_pointer, void *args);
int match_contact(char **query_strings, char **item_strings, int query_size);
int foreach_find_item(void *void_item_pointer, void *args);
struct list_item *find_item(struct linked_list *list, char *name, char *surname, char *email, char *phone);

#endif
