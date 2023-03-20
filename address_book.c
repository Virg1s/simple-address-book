#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "item_data.h"

#include <signal.h>

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

item_data_s *parse_line(char *line_buffer)
{
	item_data_s *item;
	char *fields[CSV_NUM_ENTRIES_PER_LINE];

	//raise(SIGTRAP);

	fields[0] = strtok(line_buffer, ",");
	fields[1] = strtok(NULL, ",");
	fields[2] = strtok(NULL, ",");
	fields[3] = strtok(NULL, "\n");

	for (int i = 0; i < CSV_NUM_ENTRIES_PER_LINE; i++)
		if (fields[i] == NULL)
			return NULL; 

	item = malloc(sizeof(*item));
	strcpy(item->name, fields[0]);
	strcpy(item->surname, fields[1]); 
	strcpy(item->email, fields[2]);
	strcpy(item->phone, fields[3]);

	return item;
}

item_data_s **read_data(FILE *data_file)
{
	char line_buffer[CSV_MAX_LINE_LENGTH];
	// should replace the item_buffer with linked list initialisation
	item_data_s **item_buffer = malloc(sizeof(*item_buffer) * CSV_LINE_COUNT_MAX);
	item_data_s *current_item;
	int item_counter = 0;

	while (fgets(line_buffer, CSV_MAX_LINE_LENGTH, data_file)) {
		current_item = parse_line(line_buffer);

		if (current_item == NULL)
			break;

		item_buffer[item_counter++] = current_item;
	}
	
	if (item_counter == 0) {
		free(item_buffer);
		item_buffer = NULL;
	}

	return item_buffer;
}

/*
not sure about the task saying that there must be at least 10 addresses
ant that program is supposed to continue it's work if the file does not exist
	- so which one of those two conditions overrules?
	- what should the program do if file does not exist? Be zombie process? Sleep and check for list every 20s?
*/

int main()
{
	item_data_s **item_buffer;	

	//FILE *okei = open_file("failure.example");
	FILE *okei = open_file("addresses.csv");

	item_buffer = read_data(okei);
	return 0;
}
