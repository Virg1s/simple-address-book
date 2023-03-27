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

FILE *open_file(char *filename) {
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

int parse_line(char *line_buffer, struct item_data *item) {
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

int read_data(FILE *data_file, struct linked_list *address_book) {
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

void print_list(struct linked_list *list) {
  map_list(list, foreach_print_item, NULL);
}

void delete_list(struct linked_list *list) {
  map_list(list, foreach_delete_item, NULL);
  free(list);
}

int append_contact(struct linked_list *address_book, char *name, char *surname, char *email, char *phone)
{
	struct item_data contact;
	char *fields[] = {name, surname, email, phone};

	init_contact(&contact, fields);
	return add_item_to_end(address_book, &contact);
}

int add_contact_to_position(struct linked_list *address_book, unsigned position, char *name, char *surname, char *email, char *phone)
{
	struct item_data contact;
	char *fields[] = {name, surname, email, phone};

	init_contact(&contact, fields);
	return add_item_to_position(address_book, &contact, position);
}

/*
not sure about the task saying that there must be at least 10 addresses
ant that program is supposed to continue it's work if the file does not exist
        - so which one of those two conditions overrules?
        - what should the program do if file does not exist? Be zombie process?
Sleep and check for list every 20s?
*/

int main() {
  struct linked_list *address_book = init_list();
  FILE *okei = open_file("addresses.csv");
  int number_of_contacts = read_data(okei, address_book);
  char *name, *surname, *email, *phone;

  fclose(okei);
  printf("Num of contacts read from file: %d\n", number_of_contacts);
  puts("#########################\n");
  printf("Address book contents:\n\n");
  print_list(address_book);
  puts("#########################\n");
  name = "vardenis2", surname = "", email = "", phone = "";
  printf("Finding item by fields name : '%s', surname: '%s', email: '%s', phone: '%s'\n", name, surname, email, phone);
  find_item(address_book, name, surname, email, phone);
  puts("#########################\n");
  printf("Adding item: '%s', surname: '%s', email: '%s', phone: '%s' to end\n", name, surname, email, phone);
  name = "paskutinis_kontaktas", surname = "paskutine_pavarde", email = "paskutinis_emailas", phone = "paskutinis_telefonas";
	append_contact(address_book, name, surname, email, phone);
  printf("Address book contents:\n\n");
  print_list(address_book);
  puts("#########################\n");
  delete_list(address_book);
  printf("contents after delete:\n\n");

  return 0;
}
