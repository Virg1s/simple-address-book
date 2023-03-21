#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "item_data.h"
#include "linked_list.h"
#include <signal.h> //temp

#define FILE_PATH_MAX 150
#define CSV_LINE_COUNT_MAX 1000
#define CSV_MAX_LINE_LENGTH 300
#define CSV_ENTRY_MAX_LENGTH 100
#define CSV_NUM_ENTRIES_PER_LINE 4

FILE *open_file(char *filename)
{
	char filepath[FILE_PATH_MAX];
	char *home_path = getenv("HOME");
	FILE *data_file;
	int path_len;

	path_len = snprintf(filepath, FILE_PATH_MAX, "%s/%s", home_path, filename); 

	if (path_len == FILE_PATH_MAX)
		fprintf(stderr, "file path too long\n");

	data_file = fopen(filepath, "r");

	if (data_file == NULL) {
		fprintf(stderr, "unable to open file '%s'\n", filepath);
		exit(1);
	}
	
	return data_file;
}

item_data_s *parse_line(char *line_buffer, item_data_s *item)
{
	char *fields[CSV_NUM_ENTRIES_PER_LINE];

	fields[0] = strtok(line_buffer, ",");
	fields[1] = strtok(NULL, ",");
	fields[2] = strtok(NULL, ",");
	fields[3] = strtok(NULL, "\n");

	for (int i = 0; i < CSV_NUM_ENTRIES_PER_LINE; i++)
		if (fields[i] == NULL)
			return NULL; 

	strcpy(item->name, fields[0]);
	strcpy(item->surname, fields[1]); 
	strcpy(item->email, fields[2]);
	strcpy(item->phone, fields[3]);

	return item;
}

int read_data(FILE *data_file, linked_list_p address_book)
{
	// returns number of items added to address book
	char line_buffer[CSV_MAX_LINE_LENGTH];
	item_data_s current_item;
	int item_counter = 0;

	while (fgets(line_buffer, CSV_MAX_LINE_LENGTH, data_file)) {
		if(!parse_line(line_buffer, &current_item))
			break;

		add_item_to_end(address_book, &current_item);
		item_counter++;
	}
	return item_counter;
}

int foreach_print_item(void *void_item_pointer, void *args)
{
	(void) args; // feels like money laundering lol :D
	char *separator = "---------------------";
	char *indentation = "\t";
	list_item_p item = (list_item_p) void_item_pointer;

	printf("%sname: %s\n", indentation, item->data.name);
	printf("%ssurname: %s\n", indentation, item->data.surname);
	printf("%semail: %s\n", indentation, item->data.email);
	printf("%sphone: %s\n", indentation, item->data.phone);
	printf("%s%s\n", indentation, separator);

	return 0;
}

int foreach_delete_item(void *void_item_pointer, void *args)
{
	// wrapper is neccessary because "foreach" functions are expected to return a signal
	(void) args;
	free(void_item_pointer);
	return 0;
}

struct foreach_find_item_args {
	item_data_s *data;
	list_item_p match_address;
};

int _match_contact(char **query_strings, char **item_strings, int query_size)
{
	for (int i = 0; i < query_size; i++) {
		if (query_strings[i][0] && strcmp(query_strings[i], item_strings[i]))
			return 0;
	}
	return 1;
}

int foreach_find_item(void *void_item_pointer, void *args)
{
	struct foreach_find_item_args *shuffle = (struct foreach_find_item_args *) args;
	list_item_p item = (list_item_p) void_item_pointer;
	int match;

	char *query_strings[] = {
		shuffle->data->name,
		shuffle->data->surname,
		shuffle->data->email,
		shuffle->data->phone
	};
	char *item_strings[] = {
		item->data.name,
		item->data.surname,
		item->data.email,
		item->data.phone
	};
	
	match = _match_contact(query_strings, item_strings, sizeof(query_strings) / sizeof(query_strings[0]));

	if (match)
		shuffle->match_address = item;

	return match;
}

void print_list(linked_list_p list)
{
	map_list(list, foreach_print_item, NULL);
}

void delete_list(linked_list_p list)
{
	map_list(list, foreach_delete_item, NULL);
	free(list);
}

list_item_p match_item_by_data(linked_list_p list, item_data_s *data)
{
	list_item_p match = NULL; 
	struct foreach_find_item_args args = {data, match};

	map_list(list, foreach_find_item, &args); 

	return args.match_address;
}


/*
not sure about the task saying that there must be at least 10 addresses
ant that program is supposed to continue it's work if the file does not exist
	- so which one of those two conditions overrules?
	- what should the program do if file does not exist? Be zombie process? Sleep and check for list every 20s?
*/

int main()
{
	linked_list_p address_book = init_list();
	FILE *okei = open_file("addresses.csv");
	int number_of_contacts = read_data(okei, address_book);

	printf("Num of contacts read from file: %d\n", number_of_contacts);
	printf("Address book contents:\n\n");
	print_list(address_book);

	return 0;
}
