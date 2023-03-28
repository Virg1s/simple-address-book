#include "contact.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_PATH_MAX 150
#define CSV_LINE_COUNT_MAX 1000
#define CSV_MAX_LINE_LENGTH 300
#define CSV_ENTRY_MAX_LENGTH 100
#define CSV_NUM_ENTRIES_PER_LINE 4
#define MENU_USER_INPUT_MAX_LENGTH 350



FILE *open_file(char *filename)
{
	char filepath[FILE_PATH_MAX];
	char *home_path = getenv("HOME");
	FILE *data_file;
	int path_len;

	path_len =
		snprintf(filepath, FILE_PATH_MAX, "%s/%s", home_path, filename);

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

int parse_line(char *line_buffer, struct item_data *item)
{
	char *fields[CSV_NUM_ENTRIES_PER_LINE];

	fields[0] = strtok(line_buffer, ",");
	fields[1] = strtok(NULL, ",");
	fields[2] = strtok(NULL, ",");
	fields[3] = strtok(NULL, "\n");

	for (int i = 0; i < CSV_NUM_ENTRIES_PER_LINE; i++)
		if (fields[i] == NULL)
			return 1;

	init_contact(item, fields);
	return 0;
}

int read_data(FILE *data_file, struct linked_list *address_book)
{
	// returns number of items added to address book
	char line_buffer[CSV_MAX_LINE_LENGTH];
	struct item_data current_item;
	int item_counter = 0;

	while (fgets(line_buffer, CSV_MAX_LINE_LENGTH, data_file)) {
		if (parse_line(line_buffer, &current_item))
			break;

		add_item_to_end(address_book, &current_item);
		item_counter++;
	}
	return item_counter;
}

void print_list(struct linked_list *list)
{
	if (!list) {
		fputs("list was deleted\n", stderr);
		return;
	}

	map_list(list, foreach_print_item, NULL);
}

void delete_list(struct linked_list *list)
{
	map_list(list, foreach_delete_item, NULL);
	free(list);
}

int append_contact(struct linked_list *address_book, char *name, char *surname,
		   char *email, char *phone)
{
	struct item_data contact;
	char *fields[] = { name, surname, email, phone };

	init_contact(&contact, fields);
	return add_item_to_end(address_book, &contact);
}

int add_contact_to_position(struct linked_list *address_book, unsigned position,
			    char *name, char *surname, char *email, char *phone)
{
	struct item_data contact;
	char *fields[] = { name, surname, email, phone };

	init_contact(&contact, fields);
	return add_item_to_position(address_book, &contact, position);
}

int get_user_input(char **actions, char user_input[MENU_USER_INPUT_MAX_LENGTH])
{
	int input_length = 0;

	while (1) {
		puts("Select an action:");

		for (int i = 0; actions[i]; i++)
			printf("  -%s\n", actions[i]);
		puts("");
		
		fgets(user_input, MENU_USER_INPUT_MAX_LENGTH, stdin);
		input_length = strlen(user_input);
		user_input[input_length - 1] = '\0';

		if (input_length > 1)
			for (int i = 0; actions[i]; i++)
				if (strstr(actions[i], user_input))
					return i;
		puts(" no such menu item\n");
	}
}

int get_args(char **arg_names, char **arg_buffers, int *arg_lengths)
{
	char *newline_pos;

	for (int i = 0; arg_names[i]; i++) {
		printf("enter %s: ", arg_names[i]);
		fgets(arg_buffers[i], arg_lengths[i], stdin);
		newline_pos = strchr(arg_buffers[i], '\n');
		if (newline_pos) {
			*newline_pos = '\0';
		} else {
			puts("invalid arguments");
			return 1;
		}
	}
	return 0;
}

void execute_request(struct linked_list **address_book_pointer, int action_index)
{
	int position = -1;
	char name[NAME_MAX], surname[SURNAME_MAX], email[EMAIL_MAX], phone[PHONE_MAX];
	char *arg_names[] = {"name", "surname", "email", "phone", NULL};
	char *arg_buffers[] = {name, surname, email, phone};
	int arg_lengths[] = {NAME_MAX, SURNAME_MAX, EMAIL_MAX, PHONE_MAX};
	struct linked_list *address_book = *address_book_pointer;

	switch (action_index) {
	case 0:
		print_list(address_book);
		break;
	case 1:
		if (!get_args(arg_names, arg_buffers, arg_lengths))
			append_contact(address_book, name, surname, email, phone);
		print_list(address_book);
		break;
	case 2:
		if (!get_args(arg_names, arg_buffers, arg_lengths)) {
			puts("enter position: ");
			scanf("%d%*c", &position);
			fflush(stdin);
			add_contact_to_position(address_book, position, name, surname, email, phone);
			print_list(address_book);
		}
		break;
	case 3:
		puts("enter position: ");
		scanf("%d%*c", &position);
		fflush(stdin);
		if (position >= 1)
			delete_item_by_position(address_book, position);
		print_list(address_book);
		break;
	case 4:
		delete_list(address_book);
		*address_book_pointer = NULL;
		printf("Address book deleted, goodbye!\n");
		exit(0);
	}
}

void prompt(struct linked_list **address_book)
{
	char *actions[] = {
		"print contacts",
		"append contact",
		"add to",
		"delete in",
		"delete all",
		NULL
	};
	char user_input[MENU_USER_INPUT_MAX_LENGTH];
	int menu_item;

	menu_item = get_user_input(actions, user_input);

	execute_request(address_book, menu_item);
}

int main(void)
{
	struct linked_list *address_book = init_list();
	FILE *okei = open_file("addresses.csv");
	int number_of_contacts = read_data(okei, address_book);

	fclose(okei);
	printf("Num of contacts read from file: %d\n", number_of_contacts);
	puts("#########################\n");

	while(1)
		prompt(&address_book);

/*
	print_list(address_book);
	find_item(address_book, name, surname, email, phone);
	append_contact(address_book, name, surname, email, phone);
	add_contact_to_position(address_book, position, name, surname, email, phone);
	delete_item_by_position(address_book, position);
	delete_list(address_book);
*/

	return 0;
}
