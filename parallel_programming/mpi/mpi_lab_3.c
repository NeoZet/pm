#include <getopt.h>
#include <inttypes.h>
#include <mpi/mpi.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_MSG_TAG 42    // Tag for sent from root process
#define RESULT_MSG_TAG 24  // Tag for sent to root process

#define RETURN_IF_ERROR(ret_code) \
    do                            \
    {                             \
        int rt = (ret_code);      \
        if (rt != 0)              \
            return rt;            \
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
static int transpose_matrix(matrix_t matrix, matrix_t *transposed_matrix);
static int matrix_multiplication(matrix_t first, matrix_t second, matrix_t *result);
static int matrix_multiplication_MPI(matrix_t first, matrix_t second, matrix_t *result);
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

    matrix_t matrix_first = {
        .values = NULL,
        .num_of_row = args.num_of_row_first,
        .num_of_col = args.num_of_col_first};

    matrix_t matrix_second = {
        .values = NULL,
        .num_of_row = args.num_of_row_second,
        .num_of_col = args.num_of_col_second};

    matrix_t matrix_result = {
        .values = NULL,
        .num_of_row = args.num_of_row_first,
        .num_of_col = args.num_of_col_second};

    RETURN_IF_ERROR(init_matrix(&matrix_first));
    RETURN_IF_ERROR(init_matrix(&matrix_second));
    RETURN_IF_ERROR(fill_matrix_with_random(&matrix_first));
    RETURN_IF_ERROR(fill_matrix_with_random(&matrix_second));

    RETURN_IF_ERROR(init_matrix(&matrix_result));

    struct timespec start_tp;
    struct timespec end_tp;
    int64_t matrix_multiplication_duration = 0;

    double start = 0;
    double end = 0;

    MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    start = MPI_Wtime();
    matrix_multiplication(matrix_first, matrix_second, &matrix_result);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\nStandart\n\n");
        printf("Matrix [1]:\n");
        print_matrix(matrix_first);
        printf("\n");
        printf("Matrix [2]:\n");
        print_matrix(matrix_second);
        printf("\n");
        printf("Result:\n");
        print_matrix(matrix_result);
        printf("----------\n");
        printf("Duration: %lf\n", end - start);
    }

    matrix_t matrix_result_p = {
        .values = NULL,
        .num_of_row = args.num_of_row_first,
        .num_of_col = args.num_of_col_second};
    RETURN_IF_ERROR(init_matrix(&matrix_result_p));

    start = MPI_Wtime();
    matrix_multiplication_MPI(matrix_first, matrix_second, &matrix_result_p);
    end = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    if (rank == 0)
    {
        printf("\n==================\n\n");
        printf("\nMPI parallel\n\n");
        printf("Matrix [1]:\n");
        print_matrix(matrix_first);
        printf("\n");
        printf("Matrix [2]:\n");
        print_matrix(matrix_second);
        printf("\n");
        printf("Result:\n");
        print_matrix(matrix_result);
        printf("----------\n");
        printf("Duration: %lf\n", end - start);
    }
    deinit_matrix(&matrix_first);
    deinit_matrix(&matrix_second);
    deinit_matrix(&matrix_result);
    deinit_matrix(&matrix_result_p);
    return 0;
}

static int init_matrix(matrix_t *matrix)
{
    matrix->values = (int64_t **)malloc(sizeof(int64_t *) * matrix->num_of_row);
    if (matrix->values == NULL)
    {
        fprintf(stderr, "[%s]: unable to allocate memory for row\n", __func__);
        return -1;
    }

    for (size_t i = 0; i < matrix->num_of_row; ++i)
    {
        matrix->values[i] = (int64_t *)malloc(sizeof(int64_t) * matrix->num_of_col);
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

static int matrix_multiplication(matrix_t first, matrix_t second, matrix_t *result)
{
    if (result == NULL)
    {
        fprintf(stderr, "[%s]: parameter is NULL: result==%p\n", __func__, result);
        return -1;
    }

    if (first.num_of_col != second.num_of_row)
    {
        fprintf(stderr, "[%s]: unable to multiply matrix: first matrix columns %lu != second matrix rows %lu\n",
                __func__, first.num_of_col, second.num_of_row);
        return -2;
    }

    for (size_t i = 0; i < first.num_of_row; ++i)
    {
        for (size_t j = 0; j < second.num_of_col; ++j)
        {
            result->values[i][j] = 0;
            for (size_t k = 0; k < second.num_of_row; ++k)
            {
                result->values[i][j] += first.values[i][k] * second.values[k][j];
            }
        }
    }

    return 0;
}

static int matrix_multiplication_MPI(matrix_t first, matrix_t second, matrix_t *result)
{
    if (result == NULL)
    {
        fprintf(stderr, "[%s]: parameter is NULL: result==%p\n", __func__, result);
        return -1;
    }
    if (first.num_of_col != second.num_of_row)
    {
        fprintf(stderr, "[%s]: unable to multiply matrix: first matrix columns %lu != second matrix rows %lu\n",
                __func__, first.num_of_col, second.num_of_row);
        return -1;
    }

    int processes_number = 0;
    int process_rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &processes_number);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    MPI_Request request;
    MPI_Status status;
    if (processes_number > first.num_of_row)
    {
        processes_number = first.num_of_row + 1;
    }

    int piece_multiplier = first.num_of_row / (processes_number - 1);
    int piece_size = 0;
    int offset = 0;
    if (process_rank == 0)
    {
        int low_bord;
        int upper_bord;
        for (int proc_count = 1; proc_count < processes_number; ++proc_count)
        {
            low_bord = (proc_count - 1) * piece_multiplier;
            if (((proc_count + 1 == processes_number) && (first.num_of_row % (processes_number - 1))) != 0)
            {
                // Remaining unsent portion
                piece_size = first.num_of_row * first.num_of_col - piece_multiplier * first.num_of_col * (proc_count - 1);
                upper_bord = first.num_of_row;
            }
            else
            {
                piece_size = piece_multiplier * first.num_of_col;
                upper_bord = low_bord + piece_multiplier;
            }
            MPI_Isend(first.values[proc_count - 1], piece_size, MPI_LONG, proc_count, DATA_MSG_TAG, MPI_COMM_WORLD, &request);
            MPI_Isend(&low_bord, 1, MPI_INT, proc_count, DATA_MSG_TAG + 1, MPI_COMM_WORLD, &request);
            MPI_Isend(&upper_bord, 1, MPI_INT, proc_count, DATA_MSG_TAG + 2, MPI_COMM_WORLD, &request);
        }
    }

    MPI_Bcast(&(second.values[0][0]), second.num_of_row * second.num_of_col, MPI_LONG, 0, MPI_COMM_WORLD);

    if (process_rank > 0 && process_rank < processes_number)
    {
        int elem_count;
        int low_bord;
        int upper_bord;
        int64_t row_len_first;
        MPI_Probe(0, DATA_MSG_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_LONG, &elem_count);
        int64_t first_matix_buf[elem_count];

        MPI_Recv(first_matix_buf, elem_count, MPI_LONG, 0, DATA_MSG_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&low_bord, 1, MPI_INT, 0, DATA_MSG_TAG + 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&upper_bord, 1, MPI_INT, 0, DATA_MSG_TAG + 2, MPI_COMM_WORLD, &status);
        int64_t res[row_len_first][second.num_of_col];
        for (size_t i = low_bord; i < upper_bord; ++i)
        {
            for (size_t j = 0; j < second.num_of_col; ++j)
            {
                res[i][j] = 0;
                for (size_t k = 0; k < second.num_of_row; ++k)
                {
                    res[i][j] += first_matix_buf[i * second.num_of_row + k] * second.values[k][j];
                }
            }
        }
        MPI_Isend(&res[low_bord][0], (upper_bord - low_bord) * second.num_of_col, MPI_LONG, 0, RESULT_MSG_TAG, MPI_COMM_WORLD, &request);
        MPI_Isend(&low_bord, 1, MPI_INT, 0, DATA_MSG_TAG + 1, MPI_COMM_WORLD, &request);
        MPI_Isend(&upper_bord, 1, MPI_INT, 0, DATA_MSG_TAG + 2, MPI_COMM_WORLD, &request);
    }

    if (process_rank == 0)
    {
        int low_bord;
        int upper_bord;
        int64_t res[low_bord][second.num_of_col];
        for (int i = 1; i < processes_number; i++)
        {              
            MPI_Recv(&low_bord, 1, MPI_INT, i, DATA_MSG_TAG + 1, MPI_COMM_WORLD, &status);           
            MPI_Recv(&upper_bord, 1, MPI_INT, i, DATA_MSG_TAG + 2, MPI_COMM_WORLD, &status);            
            MPI_Recv(&result->values[low_bord][0], (upper_bord - low_bord) * second.num_of_col, MPI_DOUBLE, i, DATA_MSG_TAG, MPI_COMM_WORLD, &status);
        }
    }
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

    parsed_args->num_of_row_first = -1;
    parsed_args->num_of_col_first = -1;
    parsed_args->num_of_row_second = -1;
    parsed_args->num_of_col_second = -1;

    static struct option long_option[] = {
        {"rows-number-first", required_argument, NULL, 0},
        {"cols-number-first", required_argument, NULL, 1},
        {"rows-number-second", required_argument, NULL, 2},
        {"cols-number-second", required_argument, NULL, 3},
        {"help", no_argument, NULL, 'h'},
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

    if (parsed_args->num_of_row_first == -1 ||
        parsed_args->num_of_col_first == -1 ||
        parsed_args->num_of_row_second == -1 ||
        parsed_args->num_of_col_second == -1) return -1;

    return 0;
}

void print_usage(int32_t argc, char *argv[])
{
    printf("Usage: %s --rows-number-first [N] --cols-number-first [N] --rows-number-second [N] --cols-number-second [N]\n", argv[0]);
    printf("-h, --help print this help message\n");
}
