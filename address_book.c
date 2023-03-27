#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "contact.h"

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

	if (path_len >= FILE_PATH_MAX) {
		fprintf(stderr, "file path too long\n");
		exit(1);
	}

	data_file = fopen(filepath, "r");

	if (data_file == NULL) {
		fprintf(stderr, "unable to open file '%s'\n", filepath);
		exit(1);
	}
	
	return data_file;
}

struct item_data *parse_line(char *line_buffer, struct item_data *item)
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

int read_data(FILE *data_file, struct linked_list *address_book)
{
	// returns number of items added to address book
	char line_buffer[CSV_MAX_LINE_LENGTH];
	struct item_data current_item;
	int item_counter = 0;

	while (fgets(line_buffer, CSV_MAX_LINE_LENGTH, data_file)) {
		if(!parse_line(line_buffer, &current_item))
			break;

		add_item_to_end(address_book, &current_item);
		item_counter++;
	}
	return item_counter;
}

void print_list(struct linked_list *list)
{
	map_list(list, foreach_print_item, NULL);
}

void delete_list(struct linked_list *list)
{
	map_list(list, foreach_delete_item, NULL);
	free(list);
}

/*
not sure about the task saying that there must be at least 10 addresses
ant that program is supposed to continue it's work if the file does not exist
	- so which one of those two conditions overrules?
	- what should the program do if file does not exist? Be zombie process? Sleep and check for list every 20s?
*/

int main()
{
	struct linked_list *address_book = init_list();
	FILE *okei = open_file("addresses.csv");
	int number_of_contacts = read_data(okei, address_book);

	fclose(okei);
	printf("Num of contacts read from file: %d\n", number_of_contacts);
	printf("Address book contents:\n\n");
	print_list(address_book);
	delete_list(address_book);

	return 0;
}
