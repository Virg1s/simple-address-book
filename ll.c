#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define NAME_MAX 50
#define SURNAME_MAX 50
#define EMAIL_MAX 100
#define PHONE_MAX 30

typedef struct list_item_s {
	struct list_item_s *next;
	struct list_item_s *prev;
	char name[NAME_MAX];
	char surname[SURNAME_MAX];
	char email[EMAIL_MAX];
	char phone[PHONE_MAX];
} *list_item_p;

typedef struct linked_list_s {
	list_item_p head;
	int length;
} *linked_list_p;

struct contact_data {
	char *name;
	char *surname;
	char *email;
	char *phone;
};

linked_list_p init_list(void)
{
	linked_list_p list = malloc(sizeof(struct linked_list_s));
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

list_item_p init_item(struct contact_data *data)
{
	list_item_p item = malloc(sizeof(struct list_item_s)); 	

	strcpy(item->name, data->name);
	strcpy(item->surname, data->surname);
	strcpy(item->email, data->email);
	strcpy(item->phone, data->phone);

	return item;
}

void create_first_item(linked_list_p list, list_item_p item)
{
	item->next = item;
	item->prev = item;
	list->head = item;
	list->length++;
}

int add_item_to_position(linked_list_p list, struct contact_data *data, unsigned position)
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

int add_item_to_end(linked_list_p list, struct contact_data *data)
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

int _foreach_print_item(void *void_item_pointer, void *args)
{
	(void) args; // feels like money laundering lol :D
	char *separator = "---------------------";
	list_item_p item = (list_item_p) void_item_pointer;

	printf("name: %s\n", item->name);
	printf("surname: %s\n", item->surname);
	printf("email: %s\n", item->email);
	printf("phone: %s\n", item->phone);
	printf("%s\n", separator);

	return 0;
}

int _foreach_delete_item(void *void_item_pointer, void *args)
{
	// wrapper is neccessary because "foreach" functions are expected to return a signal
	(void) args;
	free(void_item_pointer);
	return 0;
}

struct _foreach_find_item_args {
	struct contact_data *data;
	list_item_p match_address;
};

int _match_contact(char **query_data, char **item_data, int data_size)
{
	for (int i = 0; i < data_size; i++) {
		if (query_data[i] && strcmp(query_data[i], item_data[i]))
			return 0;
	}
	return 1;
}

int _foreach_find_item(void *void_item_pointer, void *args)
{
	struct _foreach_find_item_args *shuffle = (struct _foreach_find_item_args *) args;
	list_item_p item = (list_item_p) void_item_pointer;
	int match;

	char *query_data[] = {
		shuffle->data->name,
		shuffle->data->surname,
		shuffle->data->email,
		shuffle->data->phone
	};
	char *item_data[] = {
		item->name,
		item->surname,
		item->email,
		item->phone
	};
	
	match = _match_contact(query_data, item_data, sizeof(query_data) / sizeof(query_data[0]));

	if (match)
		shuffle->match_address = item;

	return match;
}

void print_list(linked_list_p list)
{
	map_list(list, _foreach_print_item, NULL);
}

void delete_list(linked_list_p list)
{
	map_list(list, _foreach_delete_item, NULL);
	free(list);
}

list_item_p match_item_by_data(linked_list_p list, char *name, char *surname, char *email, char *phone)
{
	struct contact_data data = {name, surname, email, phone};
	list_item_p match = NULL; 
	struct _foreach_find_item_args args = {&data, match};

	map_list(list, _foreach_find_item, &args); 

	return args.match_address;
}

int main(void)
{
	linked_list_p address_book = init_list();
	struct contact_data data09 = {"0.9Virginijus", "0.9Ruibys", "labas@labas.com", "+0.90.90.90.90.90.90.9"};
	struct contact_data data1 = {"1Virginijus", "1Ruibys", "labas@labas.com", "+1111111"};
	struct contact_data data2 = {"2Virginijus", "2Ruibys", "labas@labas.com", "+2222222"};
	struct contact_data data3 = {"3Virginijus", "3Ruibys", "labas@labas.com", "+3333333"};
	struct contact_data data4 = {"4Virginijus", "4Ruibys", "labas@labas.com", "+4444444"};
	struct contact_data data5 = {"5Virginijus", "5Ruibys", "labas@labas.com", "+5555555"};
	struct contact_data data6 = {"6Virginijus", "6Ruibys", "labas@labas.com", "+6666666"};
	int append_status, insert_position, delete_position;

	//////////////////// APPENDS ///////////////
	append_status = add_item_to_end(address_book, &data09);
	insert_position = add_item_to_position(address_book, &data2, 2);
	insert_position = add_item_to_position(address_book, &data1, 2);
	insert_position = add_item_to_position(address_book, &data3, 4);
	insert_position = add_item_to_position(address_book, &data6, 10); // supposd to fail because out of bounds
	append_status = add_item_to_end(address_book, &data4);

	/////////////////// DELETES ////////////////
	delete_position = delete_item_by_position(address_book, 4);
	delete_position = delete_item_by_position(address_book, 10); // supposed to fail because out of bounds

	////////////////// MAPS ///////////////////
	print_list(address_book);
	//raise(SIGTRAP);
	list_item_p match = match_item_by_data(address_book, "1Virginijus", NULL, NULL, NULL);
	printf("match: %p, name: %s\n", match, match->name);
	delete_list(address_book);

}


/*
Address book should save this information:

    Name
    Surname
    Email
    Phone number

Your program should have this functionality:

    -Display all records in the address book
    -Add new address to the book (to the end)
    -Add new address indicating the position where to put it
    -Delete address indicating its position in the book
    -Delete whole address book
    -Find address by position

    // could use some additional testing
    -Find address by name, surname, email or phone number

All the addresses must be entered by the user.
*/