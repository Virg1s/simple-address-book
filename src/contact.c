#include "contact.h"
#include "item_data.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_contact(struct item_data *item, char *fields[static 4])
{
	snprintf(item->name, sizeof(item->name), "%s", fields[0]);
	snprintf(item->surname, sizeof(item->surname), "%s", fields[1]);
	snprintf(item->email, sizeof(item->email), "%s", fields[2]);
	snprintf(item->phone, sizeof(item->phone), "%s", fields[3]);
}

struct foreach_find_item_args {
	char **search_data;
	struct list_item *match_address;
};

int foreach_print_item(void *void_item_pointer, void *args)
{
	(void)args;
	char *separator = "---------------------";
	char *indentation = "\t";
	struct list_item *item = (struct list_item *)void_item_pointer;

	printf("%sname: %s\n", indentation, item->data.name);
	printf("%ssurname: %s\n", indentation, item->data.surname);
	printf("%semail: %s\n", indentation, item->data.email);
	printf("%sphone: %s\n", indentation, item->data.phone);
	printf("%s%s\n", indentation, separator);

	return 0;
}

int foreach_delete_item(void *void_item_pointer, void *args)
{
	// wrapper is neccessary because function is expected to return a status
	(void)args;
	free(void_item_pointer);
	return 0;
}

int match_contact(char **query_strings, char **item_strings, int query_size)
{
	for (int i = 0; i < query_size; i++) {
		if (query_strings[i][0] &&
		    strcmp(query_strings[i], item_strings[i]))
			return 0;
	}
	return 1;
}

int foreach_find_item(void *void_item_pointer, void *args)
{
	struct foreach_find_item_args *shuffle =
		(struct foreach_find_item_args *)args;
	struct list_item *item = (struct list_item *)void_item_pointer;
	int match;

	char *query_strings[] = { shuffle->search_data[0],
				  shuffle->search_data[1],
				  shuffle->search_data[2],
				  shuffle->search_data[3] };
	char *item_strings[] = { item->data.name, item->data.surname,
				 item->data.email, item->data.phone };

	match = match_contact(query_strings, item_strings,
			      sizeof(query_strings) / sizeof(query_strings[0]));

	if (match)
		shuffle->match_address = item;

	return match;
}

struct list_item *find_item(struct linked_list *list, char *name, char *surname,
			    char *email, char *phone)
{
	char *search_data[4] = { name, surname, email, phone };
	struct foreach_find_item_args args = { search_data, NULL };

	map_list(list, foreach_find_item, &args);

	if (args.match_address)
		foreach_print_item(args.match_address, NULL);
	else
		puts("\nno match found\n");

	return args.match_address;
}
