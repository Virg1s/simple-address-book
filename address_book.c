#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *open_file(char *filename)
{
	char filepath[150];
	char *home_path = getenv("HOME");
	char h_path_len = strlen(home_path);
	int f_name_len = strlen(filename);
	FILE *data_file;

	if (f_name_len + h_path_len + 2 > 150) {
		fprintf(stderr, "file path too long\n");
		exit(1);
	}

	strcpy(filepath, home_path);
	strcat(filepath, "/");
	strcat(filepath, filename);

	data_file = fopen(filepath, "r");

	if (data_file == NULL) {
		fprintf(stderr, "unable to open file '%s'\n", filepath);
		exit(1);
	}
	
	return data_file;
}

//void read_data(FILE *data_file, 

int main()
{

	FILE *okei = open_file("test.test");
}
