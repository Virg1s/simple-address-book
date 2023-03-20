#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct list_item_s {
        struct list_item_s *next;
        struct list_item_s *prev;
        item_data_s data;
} *list_item_p;

typedef struct linked_list_s {
        list_item_p head;
        int length;
} *linked_list_p;

linked_list_p init_list(void);
list_item_p find_by_position(linked_list_p list, unsigned position);
int add_item_to_position(linked_list_p list, item_data_s *data, unsigned position);
int add_item_to_end(linked_list_p list, item_data_s *data);
int delete_item_by_position(linked_list_p list, int position);
void map_list(linked_list_p list, int (*foreach_function)(void *list_item, void *args), void *foreach_args);

#endif
