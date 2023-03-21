#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item_data.h"

typedef struct list_item_s {
	struct list_item_s *next;
	struct list_item_s *prev;
	item_data_s data;
} *list_item_p;

typedef struct linked_list_s {
	list_item_p head;
	int length;
} *linked_list_p;

linked_list_p init_list(void)
{
	linked_list_p list = malloc(sizeof(*list));
	return list;
}

list_item_p find_by_position(linked_list_p list, unsigned position)
{
	list_item_p head = list->head;
	list_item_p current_item = head;

	if (!head) 
		return NULL;

	if (position == 0)
		return head->prev;

	for (unsigned i = 1; i < position; i++) {
		current_item = current_item->next;

		if (current_item == head)
			return NULL;
	}
	return current_item;
}

void insert_after(linked_list_p list, list_item_p previous, list_item_p new)
{
	new->prev = previous;
	new->next = previous->next;
	previous->next = new;
	new->next->prev = new;
	list->length++;
}

void insert_before(linked_list_p list, list_item_p subsequent, list_item_p new)
{
	new->next = subsequent;
	new->prev = subsequent->prev;
	subsequent->prev = new;
	new->prev->next = new;

	if (subsequent == list->head)
		list->head = new;
	list->length++;
}

list_item_p init_item(item_data_s *data)
{
	list_item_p item = malloc(sizeof(*item)); 	

	if (item == NULL)
		return NULL;

	item->data = *data;

	return item;
}

void create_first_item(linked_list_p list, list_item_p item)
{
	item->next = item;
	item->prev = item;
	list->head = item;
	list->length++;
}

int add_item_to_position(linked_list_p list, item_data_s *data, unsigned position)
{
	// returns position if insertion was successful, otherwise returns 0

	list_item_p item;
	list_item_p previous;
	list_item_p head;

	if (position < 1)
		return 0;

	item = init_item(data);

	if (!item)
		return 0;

	head = list->head;
	previous = find_by_position(list, position - 1);
	if (!head && position == 1) {
		create_first_item(list, item);
	} else if (position == 1) {
		insert_before(list, head, item);
	} else if (previous) {
		insert_after(list, previous, item);
	} else {
		free(item);
		return 0;
	}

	return position;
}

int add_item_to_end(linked_list_p list, item_data_s *data)
{
	// returns 0 on success 1 on failure

	list_item_p head = list->head;
	list_item_p item = init_item(data);

	if (!item)
		return 1;
	else if (!head)
		create_first_item(list, item);
	else
		insert_after(list, head->prev, item);
	return 0;
}

void unlink_item(linked_list_p list, list_item_p item)
{
	item->next->prev = item->prev;
	item->prev->next = item->next;
	list->length--;
}

int delete_item_by_position(linked_list_p list, int position)
{
	list_item_p item = find_by_position(list, position);

	if (!item)
		return 0;

	unlink_item(list, item);
	free(item);

	return position;
}

void map_list(linked_list_p list, int (*foreach_function)(void *list_item, void *args), void *foreach_args)
{
	list_item_p head = list->head;
	list_item_p current_item = head;
	list_item_p next_item = NULL;

	while (next_item != head) {
		next_item = current_item->next;
		if (foreach_function((void *) current_item, foreach_args))
			break;
		current_item = next_item;
	}
}
