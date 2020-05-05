#include <getopt.h>
#include <inttypes.h>
#include <omp.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RETURN_IF_ERROR(ret_code) \
    do                            \
    {                             \
        int rt = (ret_code);      \
        if (rt != 0)              \
            return rt;            \
    } while (0)

typedef enum
{
    STANDART,
    OMP,
    PTHREAD
} method_t;

typedef struct
{
    int64_t **values;
    size_t num_of_row;
    size_t num_of_col;
} matrix_t;

typedef struct
{
    int dimention;
    int processes_number;
    int method;
} arguments_t;

typedef struct
{
    int low_bord;
    int upper_bord;
    matrix_t matrix_first;
    matrix_t matrix_second;
    matrix_t *matrix_result;
} pthread_func_args_t;

int matrix_sum(method_t method, int processes_number, matrix_t matrix_first, matrix_t matrix_second, matrix_t *matrix_result);

static int parse_args(int32_t argc, char *argv[], arguments_t *parsed_args);
static void print_usage(int32_t argc, char *argv[]);
static int init_matrix(size_t row, size_t col, matrix_t *matrix);
static int fill_matrix_with_random(matrix_t *matrix);
static void *pthread_sum_matrix(void *vargs);
static int omp_sum_matrix(matrix_t matrix_first, matrix_t matrix_second, matrix_t *matrix_result);
static int standart_sum_matrix(matrix_t matrix_first, matrix_t matrix_second, matrix_t *matrix_result);
static void deinit_matrix(matrix_t *matrix);
static void print_matrix(matrix_t matrix);

int main(int argc, char *argv[])
{
    int ret = 0;
    arguments_t args = {0};
    ret = parse_args(argc, argv, &args);
    if (ret < 0)
    {
        if (ret == -1) print_usage(argc, argv);
        return 1;
    }

    srand(time(NULL));

    matrix_t matrix_first;
    matrix_t matrix_second;

    RETURN_IF_ERROR(init_matrix(args.dimention, args.dimention, &matrix_first));
    RETURN_IF_ERROR(init_matrix(args.dimention, args.dimention, &matrix_second));
    RETURN_IF_ERROR(fill_matrix_with_random(&matrix_first));
    RETURN_IF_ERROR(fill_matrix_with_random(&matrix_second));

    matrix_t matrix_result;
    RETURN_IF_ERROR(init_matrix(args.dimention, args.dimention, &matrix_result));

    matrix_sum(args.method, args.processes_number, matrix_first, matrix_second, &matrix_result);
}

int matrix_sum(method_t method, int processes_number, matrix_t matrix_first, matrix_t matrix_second, matrix_t *matrix_result)
{
    if (matrix_result == NULL)
    {
        fprintf(stderr, "[%s]: parameter is NULL: matrix_result==%p\n", __func__, matrix_result);
        return -1;
    }
    if (matrix_first.num_of_row != matrix_second.num_of_row && matrix_first.num_of_row != matrix_result->num_of_row)
    {
        fprintf(stderr, "[%s]: matrix dimentions are different\n", __func__);
        return -2;
    }

    double duration = 0;

    if (method == PTHREAD)
    {
        printf("\nPthread\n\n");
        if (processes_number > matrix_first.num_of_row)
        {
            processes_number = matrix_first.num_of_row;
        }
        pthread_t threads[processes_number + 1];

        pthread_func_args_t func_args[processes_number + 1];
        int low_bords[processes_number + 1];
        int upper_bords[processes_number + 1];
        for (int i = 0; i < processes_number; i++)
        {
            int portion = matrix_first.num_of_row / processes_number;
            low_bords[i] = i * portion;
            if ((i + 1 == processes_number) && (matrix_first.num_of_row % processes_number != 0))
            {
                upper_bords[i] = matrix_first.num_of_row;
            }
            else
            {
                upper_bords[i] = low_bords[i] + portion;
            }
            func_args[i].low_bord = low_bords[i];
            func_args[i].upper_bord = upper_bords[i];
            func_args[i].matrix_first = matrix_first;
            func_args[i].matrix_second = matrix_second;
            func_args[i].matrix_result = matrix_result;
        }

        double start, end;
        start = omp_get_wtime();
        for (int i = 0; i < processes_number; i++)
        {
            pthread_create(&threads[i], NULL, pthread_sum_matrix, (void *)(&func_args[i]));
        }

        for (int i = 0; i < processes_number; i++)
        {
            pthread_join(threads[i], NULL);
        }
        end = omp_get_wtime();
        duration = end - start;
    }
    else if (method == OMP)
    {
        printf("\nOpenMP\n\n");
        omp_set_dynamic(0);
        omp_set_num_threads(processes_number);
        double start, end;
        start = omp_get_wtime();
        omp_sum_matrix(matrix_first, matrix_second, matrix_result);
        end = omp_get_wtime();
        duration = end - start;
    }
    else
    {
        printf("\nStandart\n\n");
        struct timespec ts1, ts2;
        double start, end;
        start = omp_get_wtime();
        standart_sum_matrix(matrix_first, matrix_second, matrix_result);
        end = omp_get_wtime();
        duration = end - start;
    }

    printf("Matrix [1]:\n");
    print_matrix(matrix_first);
    printf("\n");
    printf("Matrix [2]:\n");
    print_matrix(matrix_second);
    printf("\n");
    printf("Result:\n");
    print_matrix(*matrix_result);
    printf("----------\n");
    printf("Duration: %lf (s)\n", duration);
}

static int init_matrix(size_t row, size_t col, matrix_t *matrix)
{
    matrix->num_of_row = row;
    matrix->num_of_col = col;
    matrix->values = (int64_t **)malloc(sizeof(int64_t *) * row);
    if (matrix->values == NULL)
    {
        fprintf(stderr, "[%s]: unable to allocate memory for row\n", __func__);
        return -1;
    }

    for (size_t i = 0; i < row; ++i)
    {
        matrix->values[i] = (int64_t *)malloc(sizeof(int64_t) * col);
        if (matrix->values == NULL)
        {
            fprintf(stderr, "[%s]: unable to allocate memory for col\n", __func__);
            return -1;
        }
    }

    return 0;
}

static int fill_matrix_with_random(matrix_t *matrix)
{
    if (matrix == NULL)
    {
        fprintf(stderr, "[%s]: parameter is NULL: matrix==%p\n", __func__, matrix);
        return -1;
    }

    for (size_t i = 0; i < matrix->num_of_row; ++i)
    {
        for (size_t j = 0; j < matrix->num_of_col; ++j)
        {
            matrix->values[i][j] = rand() % 100;
        }
    }

    return 0;
}

static int standart_sum_matrix(matrix_t matrix_first, matrix_t matrix_second, matrix_t *matrix_result)
{
    for (int i = 0; i < matrix_first.num_of_row; ++i)
    {
        for (int j = 0; j < matrix_first.num_of_col; ++j)
        {
            matrix_result->values[i][j] = matrix_first.values[i][j] + matrix_second.values[i][j];
        }
    }
}

static void *pthread_sum_matrix(void *vargs)
{
    pthread_func_args_t args = *((pthread_func_args_t *)vargs);
    for (int i = args.low_bord; i < args.upper_bord; ++i)
    {
        for (int j = 0; j < args.matrix_first.num_of_col; ++j)
        {
            args.matrix_result->values[i][j] = args.matrix_first.values[i][j] + args.matrix_second.values[i][j];
        }
    }
}

static int omp_sum_matrix(matrix_t matrix_first, matrix_t matrix_second, matrix_t *matrix_result)
{    
#pragma omp parallel for shared(matrix_first, matrix_second, *matrix_result) private(i, j)
    {        
        size_t i = 0;
        for (i = 0; i < matrix_first.num_of_row; ++i)
        {
            size_t j = 0;
            for (j = 0; j < matrix_first.num_of_col; ++j)
            {
                matrix_result->values[i][j] = matrix_first.values[i][j] + matrix_second.values[i][j];
            }
        }
    }
#pragma omp barrier
}

static void deinit_matrix(matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->num_of_row; i++)
    {
        free(matrix->values[i]);
    }
    free(matrix->values);
}

static void print_matrix(matrix_t matrix)
{
    for (size_t i = 0; i < matrix.num_of_row; ++i)
    {
        for (size_t j = 0; j < matrix.num_of_col; ++j)
        {
            printf("%" PRId64 " ", matrix.values[i][j]);
        }
        printf("\n");
    }
}

int parse_args(int32_t argc, char *argv[], arguments_t *parsed_args)
{
    if (parsed_args == NULL)
    {
        printf("[%s] Invalid argument: parsed_args: NULL\n", __func__);
        return -2;
    }

    parsed_args->dimention = -1;
    parsed_args->processes_number = -1;
    parsed_args->method = -1;

    static struct option long_option[] = {
        {"dimention", required_argument, NULL, 'd'},
        {"processes-number", required_argument, NULL, 'p'},
        {"method", required_argument, NULL, 'm'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};

    int opt = 0;
    int option_index;
    while ((opt = getopt_long(argc, argv, ":d:p:m:h", long_option, &option_index)) != -1)
    {
        switch (opt)
        {
            case 'd':
                parsed_args->dimention = atoi(optarg);
                break;
            case 'p':
                parsed_args->processes_number = atoi(optarg);
                break;
            case 'm':
                if (strcmp(optarg, "omp") == 0)
                {
                    parsed_args->method = OMP;
                }
                else if (strcmp(optarg, "pthread") == 0)
                {
                    parsed_args->method = PTHREAD;
                }
                else
                {
                    parsed_args->method = STANDART;
                }
                break;
            case 'h':
                print_usage(argc, argv);
        }
    }

    if (parsed_args->dimention == -1 ||
        parsed_args->processes_number == -1 ||
        parsed_args->method == -1) return -1;

    return 0;
}

void print_usage(int32_t argc, char *argv[])
{
    printf("Usage: %s -d,--dimention [N] -p,--processes-number [N] -m,--method [omp/pthread/standart] \n", argv[0]);
    printf("-h, --help print this help message\n");
}
