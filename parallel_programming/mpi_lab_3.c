#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#include <mpi/mpi.h>

#define RETURN_IF_ERROR(ret_code)       \
do {                                    \
    int rt = (ret_code);                \
    if (rt != 0) return rt;             \
} while (0)

typedef struct
{
    int64_t **values;
    size_t num_of_row;
    size_t num_of_col;
} matrix_t;

typedef struct
{
    size_t num_of_row_first;
    size_t num_of_col_first;
    size_t num_of_row_second;
    size_t num_of_col_second;    
} arguments_t;

static int parse_args(int32_t argc, char *argv[], arguments_t *parsed_args);
static void print_usage(int32_t argc, char *argv[]);

static int init_matrix(matrix_t *matrix);
static int fill_matrix_with_random(matrix_t *matrix);
static int matrix_multiplication(matrix_t first, matrix_t second, matrix_t *result);
static void deinit_matrix(matrix_t matrix);
static void print_matrix(matrix_t matrix);

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

    srand(time(NULL));

    matrix_t matrix_first = {
        .values = NULL,
        .num_of_row = args.num_of_row_first,
        .num_of_col = args.num_of_col_first
    };

    matrix_t matrix_second = {
        .values = NULL,
        .num_of_row = args.num_of_row_second,
        .num_of_col = args.num_of_col_second
    };

    matrix_t matrix_result = {
        .values = NULL,
        .num_of_row = args.num_of_row_first,
        .num_of_col = args.num_of_col_second
    };


    RETURN_IF_ERROR(init_matrix(&matrix_first));
    RETURN_IF_ERROR(init_matrix(&matrix_second));
    RETURN_IF_ERROR(fill_matrix_with_random(&matrix_first));
    RETURN_IF_ERROR(fill_matrix_with_random(&matrix_second));
    print_matrix(matrix_first);
    printf("\n");
    print_matrix(matrix_second);

    RETURN_IF_ERROR(init_matrix(&matrix_result));
    RETURN_IF_ERROR(matrix_multiplication(matrix_first, matrix_second, &matrix_result));
    printf("----------\n");
    print_matrix(matrix_result);

    deinit_matrix(matrix_first);
    deinit_matrix(matrix_second);
    deinit_matrix(matrix_result);

    return 0;

    int errCode;

    if ((errCode = MPI_Init(&argc, &argv)) != 0)
    {
        return errCode;
    }

    int myRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0)
    {
        printf("It works!\n");
    }

    MPI_Finalize();
    return 0;
}

static int init_matrix(matrix_t *matrix)
{
    matrix->values = (int64_t **)malloc(sizeof(int64_t *) * matrix->num_of_row);
    if (matrix->values == NULL) {
        fprintf(stderr, "[%s]: unable to allocate memory for row\n", __func__);
        return -1;
    }

    for (size_t i = 0; i < matrix->num_of_row; ++i)
    {
        matrix->values[i] = (int64_t *)malloc(sizeof(int64_t) * matrix->num_of_col);
        if (matrix->values == NULL) {
            fprintf(stderr, "[%s]: unable to allocate memory for col\n", __func__);
            return -1;
        }
    }

    return 0;
}


static int fill_matrix_with_random(matrix_t *matrix)
{
    if (matrix == NULL) {
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


static int matrix_multiplication(matrix_t first, matrix_t second, matrix_t *result)
{
    if (first.num_of_col != second.num_of_row)
    {
        fprintf(stderr, "[%s]: unable to multiply matrix: first matrix columns %lu != second matrix rows %lu\n",
                __func__, first.num_of_col, second.num_of_row);
        return -1;
    }

    for (size_t i = 0; i < first.num_of_row; ++i)
    {
        for (size_t j = 0; j < second.num_of_col; ++j)
        {
            result->values[i][j] = 0;
            for (size_t k = 0; k < second.num_of_col; k++) {
                result->values[i][j] += first.values[i][k] * second.values[k][j];
            }
        }
    }

    return 0;
}

static void deinit_matrix(matrix_t matrix)
{
    for (size_t i = 0; i < matrix.num_of_row; i++)
    {
        free(matrix.values[i]);
    }
    free(matrix.values);
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
    if (argc < 3)
    {
        return -1;
    }

    if (parsed_args == NULL)
    {
        printf("[%s] Invalid argument: parsed_args: NULL\n", __func__);
        return -2;
    }

    static struct option long_option[] = {
        {"rows-number-first", required_argument, NULL, 0},
        {"cols-number-first", required_argument, NULL, 1},
        {"rows-number-second", required_argument, NULL, 2},
        {"cols-number-second", required_argument, NULL, 3},        
        {"help", required_argument, NULL, 'h'},
        {0, 0, 0, 0}};

    int opt = 0;
    int option_index;
    while ((opt = getopt_long(argc, argv, ":h", long_option, &option_index)) != -1)
    {
        switch (opt)
        {
        case 0:
            parsed_args->num_of_row_first = atoi(optarg);
            break;
        case 1:
            parsed_args->num_of_col_first = atoi(optarg);
            break;
        case 2:
            parsed_args->num_of_row_second = atoi(optarg);
            break;
        case 3:
            parsed_args->num_of_col_second = atoi(optarg);
            break;
        case 'h':
            print_usage(argc, argv);
        }
    }

    return 0;
}

void print_usage(int32_t argc, char *argv[])
{
    if (argc != 0)
    {
        printf("Usage: %s --rows-number-first [N] --cols-number-first [N] --rows-number-second [N] --cols-number-second [N]\n", argv[0]);
    }
    printf("--rows-number-first\n"
           "--cols-number-first\n"
           "--rows-number-second\n"
           "--cols-number-second\n"           
           "-h, --help print this help message\n");
}
