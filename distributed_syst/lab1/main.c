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


int generate_vector(int dimention, vector_t *vector);
int generate_vectors(int vectors_number, int dimention, vector_t *vectors);
int vectors_sum(vector_t *vectors, int vectors_number, vector_t *sum);
int vector_module(vector_t vector, double *module);

int generate_files_names(int quantity, char *prefix, char generated_files[][MAX_FILE_NAME]);
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

	//	printf("%d\n%s\n%s\n%d\n%d\n", args.processes_number, args.input_prefix, args.output_prefix, args.vectors_number, args.vectors_dimention);

 	char input_files_names[args.processes_number][MAX_FILE_NAME];
	char output_files_names[args.processes_number][MAX_FILE_NAME];
	
	if ((ret = generate_files_names(args.processes_number, args.input_prefix, input_files_names)) != 0) {
		return ret;
	}

	if ((ret = generate_files_names(args.processes_number, args.output_prefix, output_files_names)) != 0) {
		return ret;
	}
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
		vector->components[i] = rand() % 23;
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
