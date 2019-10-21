#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_FILE_NAME 64
#define MAX_VECTOR_DIMENTION 128

typedef struct {
	int32_t dimention;
	double components[MAX_VECTOR_DIMENTION];
} vector_t;


typedef struct {
	int32_t processes_number;
	char input_prefix[MAX_FILE_NAME];
	char output_prefix[MAX_FILE_NAME];
	int32_t vectors_number;
	int32_t vectors_dimention;
} arguments_t;


int32_t parse_args(int32_t argc, char *argv[], arguments_t *parsed_args);
void print_usage(int32_t argc, char *argv[]);

int32_t generate_vector(int32_t dimention, vector_t *vector);
int32_t generate_vectors(int32_t vectors_number, int32_t dimention, vector_t *vectors);
int32_t vectors_sum(vector_t *vectors, int32_t vectors_number, vector_t *sum);
int32_t vector_module(vector_t vector, double *module);

int32_t generate_files_names(int32_t quantity, char *prefix, char generated_files[][MAX_FILE_NAME]);
int32_t create_file_with_vectors(char file_name[MAX_FILE_NAME], vector_t *vectors, int32_t vectors_number);
int32_t generate_files_with_vectors(char *files_prefix, int32_t files_number, int32_t vectors_number, int32_t vectors_dimention, char files_names[][MAX_FILE_NAME]);
int32_t calculate_for_file(char *input_file_name, int32_t vectors_number, int32_t vectors_dimention, vector_t *sum, double *module, double *calc_duration);
int32_t generate_output_file(char *out_file_name, vector_t sum, double module, int32_t pid, double calc_duration, int32_t ppid);



int main(int32_t argc, char *argv[])
{
	int32_t ret = 0;
	arguments_t args = {0};
	ret = parse_args(argc, argv, &args);
	if (ret < 0) {
		if (ret == -1) {
			print_usage(argc, argv);
		}
		return 1;
	}

	srand(time(NULL));
	char input_files_names[args.processes_number][MAX_FILE_NAME];
	generate_files_with_vectors(args.input_prefix, args.processes_number, args.vectors_number, args.vectors_dimention, input_files_names);

	
	char output_files_names[args.processes_number][MAX_FILE_NAME];
	if ((ret = generate_files_names(args.processes_number, args.output_prefix, output_files_names)) != 0) {
		return ret;
	}
	
	for (int32_t i = 0; i < args.processes_number; i++) {
		pid_t pid = fork();
 		if (pid == -1) {
			printf("Unable to create proccess\n");
			exit(1);
		}
		else if (pid == 0) {
			double module, time;
			vector_t sum;
			calculate_for_file(input_files_names[i], args.vectors_number, args.vectors_dimention, &sum, &module, &time);
			generate_output_file(output_files_names[i], sum, module, getpid(), time, getppid());
			exit(0);
		}
	}

	for (int proc = 0; proc < args.processes_number; proc++) {
		int status;
		wait(&status);
	}
	
}


int32_t calculate_for_file(char *input_file_name, int32_t vectors_number, int32_t vectors_dimention, vector_t *vec_sum, double *vec_module,  double *calc_duration)
{
	FILE *file = fopen(input_file_name, "r");

	vector_t vectors[vectors_number];
	for (int32_t i = 0; i < vectors_number; i++) {
		vectors[i].dimention = vectors_dimention;
		for (int32_t j = 0; j < vectors_dimention; j++) {
			fscanf(file, "%lf", &vectors[i].components[j]);
		}
	}
	struct timespec start_tp;
	struct timespec end_tp;

	clock_gettime(CLOCK_REALTIME, &start_tp);
	vectors_sum(vectors, vectors_number, vec_sum);
	vector_module(vectors[0], vec_module);
	clock_gettime(CLOCK_REALTIME, &end_tp);

	*calc_duration = end_tp.tv_nsec - start_tp.tv_nsec;
	return 0;
}

int32_t generate_files_names(int32_t quantity, char *prefix, char generated_files[][MAX_FILE_NAME])
{
	char tmp_str[MAX_FILE_NAME];
	if (prefix == NULL) {
		printf("[generate_files_names] Invalid argument: prefix: NULL\n");
		return -1;
	}

	for (int32_t i = 0; i < quantity; ++i) {
		snprintf(generated_files[i], MAX_FILE_NAME, "%s%d", prefix, i);
	}
	return 0;
}

int32_t generate_vector(int32_t dimention, vector_t *vector)
{
	if (vector == NULL) {
		printf("[generate_vector] Invalid argument: NULL\n");
		return -1;				
	}

	vector->dimention = dimention;
	for (int32_t i = 0; i < dimention; i++) {
		vector->components[i] = rand() % 50;
	}

	return 0;
}


int32_t generate_vectors(int32_t vectors_number, int32_t dimention, vector_t *vectors)
{
	vector_t tmp_vec;
	if (vectors == NULL) {
		printf("[generate_vectors] Invalid argument: NULL\n");
		return -1;				
	}	
	for (int32_t i = 0; i < vectors_number; i++) {
		if (generate_vector(dimention, &tmp_vec) < 0) {
			printf("[generate_vectors] Unable to generate vectors\n");
			return -2;
		}
		vectors[i] = tmp_vec;
	}

	return 0;
}

// TODO: add checking errno
int32_t create_file_with_vectors(char file_name[MAX_FILE_NAME], vector_t *vectors, int32_t vectors_number)
{
	FILE *file = fopen(file_name, "w");
	for (int32_t k = 0; k < vectors_number; k++) {
		for (int32_t i = 0; i < vectors[k].dimention; i++) {
			fprintf(file, "%lf ", vectors[k].components[i]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
	return 0;
}


int32_t generate_files_with_vectors(char *files_prefix, int32_t files_number, int32_t vectors_number, int32_t vectors_dimention, char files_names[][MAX_FILE_NAME])
{
	int32_t ret = 0;
	if (files_prefix == NULL) {
		printf("[generate_files_with_vectors] Invalid argument: NULL\n");
		return -1;				
	}

	if ((ret = generate_files_names(files_number, files_prefix, files_names)) != 0) {
		return ret;
	}

	for (int32_t i = 0; i < files_number; i++) {
		vector_t vectors[vectors_number];
		if ((ret = generate_vectors(vectors_number, vectors_dimention, vectors)) != 0) {
			return ret;
		}

		if ((ret = create_file_with_vectors(files_names[i], vectors, vectors_number)) != 0) {
			return ret;
		}
	}

	return 0;
}


// TODO: add duration measurement
int32_t vectors_sum(vector_t *vectors, int32_t vectors_number, vector_t *sum)
{
	if (vectors == NULL || sum == NULL) {
		printf("[vectors_sum] Invalid argument: NULL\n");
		return -1;				
	}

	memset(sum->components, 0, sizeof(MAX_VECTOR_DIMENTION));
	
	sum->dimention = vectors[0].dimention;
	for (int32_t i = 0; i < vectors_number; ++i) {
		for (int32_t j = 0; j < vectors[i].dimention; j++) {
			sum->components[j] += vectors[i].components[j];
		}
	}
	return 0;
}

// TODO: add duration measurement
int32_t vector_module(vector_t vector, double *module)
{
	if (module == NULL) {
		printf("[vector_module] Invalid argument: NULL\n");
		return -1;				
	}

	double sum = 0;
	
	for(int32_t i = 0; i < vector.dimention; i++) {
		sum += pow(vector.components[i], 2);
	}
	*module = sqrt(sum);
	return 0;
}

// TODO: separate duration of calculation for sum and module
int32_t generate_output_file(char *out_file_name, vector_t sum, double module, int32_t pid, double calc_duration, int32_t ppid)
{
	if (out_file_name == NULL) {
		printf("[generate_output_file] Invalid file name: NULL\n");
		return -1;
	}
	
	FILE *file = fopen(out_file_name, "w");

	fprintf(file, "sum: ");
	for (int i = 0; i < sum.dimention; i++) { 
		fprintf(file, "%lf ", sum.components[i]);
	}
	fprintf(file, "\n");
	fprintf(file, "module: %lf\n", module);
	fprintf(file, "pid: %d\n", pid);
	fprintf(file, "duration of calculation: %lf (ns)\n", calc_duration);
	fprintf(file, "ppid: %d\n", ppid);

	fclose(file);
	return 0;
}

// TODO: re-work parser with getopt_long()
int32_t parse_args(int32_t argc, char *argv[], arguments_t *parsed_args)
{
	int32_t opt;

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

void print_usage(int32_t argc, char *argv[])
{
	if (argc != 0) {
		printf("Usage: %s -p [N] -i INPTUT_PREFIX -o OUTPUT_PREFIX -n [N] -d [N]\n", argv[0]);
	}
	printf("-p processes number\n"
	       "-i input files prefix\n"
	       "-o output files prefix\n"
	       "-n number of vectors\n"
	       "-d dimention of vectors\n");
}
