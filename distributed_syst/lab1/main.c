#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_FILE_NAME 64


typedef struct {
	int dimentions;
	double *vector;
} vector_t;


typedef struct {
	int processes_number;
	char input_prefix[MAX_FILE_NAME];
	char output_prefix[MAX_FILE_NAME];
	int vectors_number;
	int vectors_dimention;
} arguments_t;


int vectors_sum(vector_t first_vector, vector_t second_vector, vector_t *sum);
int vector_module(vector_t vector);
int generate_vector(vector_t *vector);

int generate_files(int quantity, char *prefix, char generated_files_names[][MAX_FILE_NAME]);
int parse_args(int argc, char *argv[], arguments_t *parsed_args);
void print_usage(int argc, char *argv[]);




int main(int argc, char *argv[])
{
	int ret = 0;
	
	arguments_t args = {0};
	ret = parse_args(argc, argv, &args);
	if (ret < 0) {
		if (ret == -1) {
			print_usage(argc, argv);
		}
		return 1;
	}

	printf("%d\n%s\n%s\n%d\n%d\n", args.processes_number, args.input_prefix, args.output_prefix, args.vectors_number, args.vectors_dimention);

	char input_files_names[args.processes_number][MAX_FILE_NAME];
	
	if ((ret = generate_files(args.processes_number, args.input_prefix, input_files_names)) != 0) {
		return ret;
	}
}


int vectors_sum(vector_t first_vector, vector_t second_vector, vector_t *sum)
{

}


int vector_module(vector_t vector)
{

}


int generate_vector(vector_t *vector)
{

}


int generate_files(int quantity, char *prefix, char generated_files_names[][MAX_FILE_NAME])
{

}


// TODO: re-work parser with getopt_long()
int parse_args(int argc, char *argv[], arguments_t *parsed_args)
{
	int opt;

	if (argc < 5) {
		return -1;
	}

	if (parsed_args == NULL) {
		printf("[parse_args] Invalid argument: parsed_args: NULL");
		return -2;
	}
	
	while((opt = getopt(argc, argv, ":p:i:o:n:d:h")) != -1) {
		switch (opt) {
		case 'p':
			parsed_args->processes_number = atoi(optarg);
			break;
		case 'i':
			strncpy(parsed_args->input_prefix, optarg, MAX_FILE_NAME);
			break;
		case 'o':
			strncpy(parsed_args->output_prefix, optarg, MAX_FILE_NAME);
			break;
		case 'n':
			parsed_args->vectors_number = atoi(optarg);
			break;
		case 'd':
			parsed_args->vectors_dimention = atoi(optarg);
			break;     
		case 'h':
			print_usage(argc, argv);
		}
	}
	return 0;
}

void print_usage(int argc, char *argv[])
{
	if (argc != 0) {
		printf("Usage: %s -p [N] -i INPTUT_PREFIX -o OUTPUT_PREFIX -n [N] -d [N]\n", argv[0]);
	}		
}
