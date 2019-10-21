#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define MAX_FILE_NAME 64
#define MAX_VECTOR_DIMENTION 128

typedef struct {
	int dimention;
	double components[MAX_VECTOR_DIMENTION];
} vector_t;


typedef struct {
	int processes_number;
	char input_prefix[MAX_FILE_NAME];
	char output_prefix[MAX_FILE_NAME];
	int vectors_number;
	int vectors_dimention;
} arguments_t;


int print_output_file(char *out_file_name, double sum, double module, int pid, double calc_duration, int ppid);
int parse_args(int argc, char *argv[], arguments_t *parsed_args);
void print_usage(int argc, char *argv[]);

int generate_vector(int dimention, vector_t *vector);
int generate_vectors(int vectors_number, int dimention, vector_t *vectors);
int vectors_sum(vector_t *vectors, int vectors_number, vector_t *sum);
int vector_module(vector_t vector, double *module);

int generate_files_names(int quantity, char *prefix, char generated_files[][MAX_FILE_NAME]);
int create_file_with_vector(char file_name[MAX_FILE_NAME], vector_t vector);
int generate_files_with_vectors(char *files_prefix, int files_number, int vectors_number, int vectors_dimention);


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

 	/* char input_files_names[args.processes_number][MAX_FILE_NAME]; */
	/* char output_files_names[args.processes_number][MAX_FILE_NAME]; */
	
	/* if ((ret = generate_files_names(args.processes_number, args.input_prefix, input_files_names)) != 0) { */
	/* 	return ret; */
	/* } */

	/* if ((ret = generate_files_names(args.processes_number, args.output_prefix, output_files_names)) != 0) { */
	/* 	return ret; */
	/* } */

	generate_files_with_vectors(args.input_prefix, args.processes_number, args.vectors_number, args.vectors_dimention);
	
}


int generate_files_names(int quantity, char *prefix, char generated_files[][MAX_FILE_NAME])
{
	char tmp_str[MAX_FILE_NAME];
	if (prefix == NULL) {
		printf("[generate_files_names] Invalid argument: prefix: NULL\n");
		return -1;
	}

	for (int i = 0; i < quantity; ++i) {
		snprintf(generated_files[i], MAX_FILE_NAME, "%s%d", prefix, i);
	}
	return 0;
}

int generate_vector(int dimention, vector_t *vector)
{
	if (vector == NULL) {
		printf("[generate_vector] Invalid argument: NULL\n");
		return -1;				
	}

	vector->dimention = dimention;
	for (int i = 0; i < dimention; i++) {
		vector->components[i] = rand() % 112;
	}

	return 0;
}


int generate_vectors(int vectors_number, int dimention, vector_t *vectors)
{
	vector_t tmp_vec;
	if (vectors == NULL) {
		printf("[generate_vectors] Invalid argument: NULL\n");
		return -1;				
	}

	for (int i = 0; i < vectors_number; i++) {
		if (generate_vector(dimention, &tmp_vec) < 0) {
			printf("[generate_vectors] Unable to generate vectors\n");
			return -2;
		}
		vectors[i] = tmp_vec;
	}

	return 0;
}

// TODO: add checking errno
int create_file_with_vector(char file_name[MAX_FILE_NAME], vector_t vector)
{
	FILE *file = fopen(file_name, "w");
	for (int i = 0; i < vector.dimention; i++) {
		fprintf(file, "%lf ", vector.components[i]);
	}
	fclose(file);
	return 0;
}


int generate_files_with_vectors(char *files_prefix, int files_number, int vectors_number, int vectors_dimention)
{
	int ret = 0;
	if (files_prefix == NULL) {
		printf("[generate_files_with_vectors] Invalid argument: NULL\n");
		return -1;				
	}

	char input_files_names[files_number][MAX_FILE_NAME];
	if ((ret = generate_files_names(files_number, files_prefix, input_files_names)) != 0) {
		return ret;
	}

	vector_t vectors[vectors_number];
	if ((ret = generate_vectors(vectors_number, vectors_dimention, vectors)) != 0) {
		return ret;
	}

	for (int i = 0; i < files_number; i++) {
		if ((ret = create_file_with_vector(input_files_names[i], vectors[i])) != 0) {
			return ret;
		}
	}

	return 0;
}


int vectors_sum(vector_t *vectors, int vectors_number, vector_t *sum)
{
	if (vectors == NULL || sum == NULL) {
		printf("[vectors_sum] Invalid argument: NULL\n");
		return -1;				
	}

	memset(sum->components, 0, sizeof(MAX_VECTOR_DIMENTION));
	
	sum->dimention = vectors[0].dimention;
	for (int i = 0; i < vectors_number; ++i) {
		for (int j = 0; j < vectors[i].dimention; j++) {
			sum->components[j] += vectors[i].components[j];
		}
	}
	return 0;
}


int vector_module(vector_t vector, double *module)
{
	if (module == NULL) {
		printf("[vector_module] Invalid argument: NULL\n");
		return -1;				
	}

	double sum = 0;
	
	for(int i = 0; i < vector.dimention; i++) {
		sum += pow(vector.components[i], 2);
	}
	*module = sqrt(sum);
	return 0;
}

// TODO: separate duration of calculation for sum and module
int print_output_file(char *out_file_name, double sum, double module, int pid, double calc_duration, int ppid)
{
	if (out_file_name == NULL) {
		printf("[print_output_file] Invalid file name: NULL\n");
		return -1;
	}
	FILE *file = fopen(out_file_name, "w");

	fprintf(file, "sum: %lf\n", sum);
	fprintf(file, "module: %lf\n", module);
	fprintf(file, "pid: %d\n", pid);
	fprintf(file, "duration of calculation: %lf\n", calc_duration);
	fprintf(file, "ppid: %d\n", ppid);

	fclose(file);
	return 0;
}

// TODO: re-work parser with getopt_long()
int parse_args(int argc, char *argv[], arguments_t *parsed_args)
{
	int opt;

	if (argc < 5) {
		return -1;
	}

	if (parsed_args == NULL) {
		printf("[parse_args] Invalid argument: parsed_args: NULL\n");
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
