#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#define EPSILON 1e-6

int validate_p(int, int);
static void program_abort(char *, char *);
static void print_usage(char *);
void print_array(long double*, char, int, int, int, int);
void matrix_multiply_add(long double *, long double *, long double *, int);
void check_comm_info(int, int, MPI_Comm, MPI_Comm, int);

int main(int argc, char * argv[]){//beginning of main=====================================================================
    int N, n;
    int num_procs, dim_proc;

    MPI_Comm comm_row, comm_col;
    int rank, rank_row, rank_col;

    long double checksum_individual = 0;
    long double checksum_total = 0;
    MPI_Init(&argc, &argv);

    //Check for the input
    if (argc != 2) {
        program_abort(argv[0], "Invalid arguments\n");
    } else {
        N = strtol(argv[1], NULL, 10);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    //validate the inputs and set n
    n = N / validate_p(num_procs, N); // n = N / sqrt(num_procs): the dimension of block
    dim_proc = N / n;//the dimension of processes

    //Alocate the block and buffer
    long double *A = (long double *)malloc(n * n * sizeof(long double));
    long double *B = (long double *)malloc(n * n * sizeof(long double));
    long double *C = (long double *)calloc(n * n,  sizeof(long double));
    long double *bufferA = (long double *)malloc(n * n * sizeof(long double));
    long double *bufferB = (long double *)malloc(n * n * sizeof(long double));
    
    //Determine the coordinates of the process
    int p_row = rank / dim_proc;
    int p_col = rank % dim_proc;

    //initialize the arrays A and B
    //A[i,j] = i, B[i,j] = [i + j].
    int global_i = p_row * n;
    int global_j = p_col * n;
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j ++) {
            A[i * n + j] = global_i;
            B[i * n + j] = global_i + global_j;
            global_j ++;
        }
        global_j -= n;
        global_i ++;
    }

    //split MPI_COMM_WORLD row-wise and get the rank in it
    MPI_Comm_split(MPI_COMM_WORLD, p_row, rank, &comm_row);
    MPI_Comm_rank(comm_row, &rank_row);
    
    //split MPI_COMM_WORLD column-wise and get the rank in it
    MPI_Comm_split(MPI_COMM_WORLD, p_col, rank, &comm_col);
    MPI_Comm_rank(comm_col, &rank_col);

    double start_time;
    double clock;
    double comm_time = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {  
        start_time = MPI_Wtime();
        clock = MPI_Wtime();
    }
    
    //===============================================================
    //Preskewing of A
    int dest, from;
    MPI_Status status;
    if (p_row != 0) {
        dest = (rank_row - p_row) % dim_proc;
        if (dest < 0) dest += dim_proc;
        from = (rank_row + p_row) % dim_proc;
        MPI_Sendrecv_replace(A, n * n, MPI_LONG_DOUBLE, dest, 0, from, 0, comm_row, &status);
    }

    //Preskewing of B    
    if (p_col != 0) {
        dest = (rank_col - p_col) % dim_proc;
        if (dest < 0) dest += dim_proc;
        from = (rank_col + p_col) % dim_proc;
        MPI_Sendrecv_replace(B, n * n, MPI_LONG_DOUBLE, dest, 0, from, 0, comm_col, &status);
    }
    if(rank == 0) {
        comm_time += MPI_Wtime() - clock;
    }
    
    // core computation
    for (int k = 0; k < dim_proc; k ++) {
        matrix_multiply_add(C, A, B, n);

        if(rank == 0) {
            clock = MPI_Wtime();
        }

        //horizontal shift
        dest = (rank_row - 1) % dim_proc;
        if(dest < 0) dest += dim_proc;
        from = (rank_row + 1) % dim_proc;
        MPI_Sendrecv_replace(A, n * n, MPI_LONG_DOUBLE, dest, 0, from, 0, comm_row, &status);

        //vertical shift
        dest = (rank_col - 1) % dim_proc;
        if(dest < 0) dest += dim_proc;
        from = (rank_col + 1) % dim_proc;
        MPI_Sendrecv_replace(B, n * n, MPI_LONG_DOUBLE, dest, 0, from, 0, comm_col, &status);
        
        if(rank == 0) {
            comm_time += MPI_Wtime() - clock;
        }
    }

    if(rank == 0) {
        clock = MPI_Wtime();
    }

    //Postskewing of A
    if(p_row != 0) {
        dest = (rank_row + p_row) % dim_proc;
        from = (rank_row - p_row) % dim_proc;
        if(from < 0) from += dim_proc;
        MPI_Sendrecv_replace(A, n * n, MPI_LONG_DOUBLE, dest, 0, from, 0, comm_row, &status);
    }

    //Postskewing of B
    if(p_col != 0) {
        dest = (rank_col + p_col) % dim_proc;
        from = (rank_col - p_col) % dim_proc;
        if(from < 0) from += dim_proc;
        MPI_Sendrecv_replace(B, n * n, MPI_LONG_DOUBLE, dest, 0, from, 0, comm_col, &status);
    }
    if(rank == 0) {
        comm_time += MPI_Wtime() - clock;
    }


    /*sanity check: 
    * Print out the communicator information for debigging.
    * check_comm_info(p_row, p_col, comm_row, comm_col, rank);
    */

    //check the sum and send to the root. report to user.
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j ++) {
            checksum_individual += C[i * n + j];
        }
    }
    MPI_Reduce(&checksum_individual, &checksum_total, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        long double t_sum = pow(N, 3) * pow(N - 1, 2) / 2.0;
        printf("Computed Sum: %.0Lf\n", checksum_total);
        printf("Theoretical Sum: %.0Lf\n", t_sum);
        if (fabs(t_sum - checksum_total) < EPSILON) {
            printf("CONGRATS: the sum is correct!\n");
        } else {
            printf("ERROR: the sum is NOT correct!\n");
        }
        printf("Elapsed time: %f\n", MPI_Wtime() - start_time);
        printf("Comm time: %f\n", comm_time);
    }

    //clean up
    MPI_Comm_free(&comm_row);
    MPI_Comm_free(&comm_col);
    MPI_Finalize();
    return 0;

}//end of main============================================================================================================

void check_comm_info(int p_row, int p_col, MPI_Comm comm_row, MPI_Comm comm_col, int rank) {
    /*
    * Helper function for debugging. Give name to each process within comm_row and comm_col 
    * and prints out information of each processor.
    */
    char name_buffer[32];
    //set the name of communicator
    snprintf(name_buffer, sizeof(name_buffer), "COMM_ROW_%i", p_row);
    MPI_Comm_set_name(comm_row, name_buffer);
    snprintf(name_buffer, sizeof(name_buffer), "COMM_COL_%i", p_col);
    MPI_Comm_set_name(comm_col, name_buffer);

    //sanity check
    char comm_row_name[MPI_MAX_OBJECT_NAME];
    char comm_col_name[MPI_MAX_OBJECT_NAME];
    int name_length;
    MPI_Comm_get_name(comm_row, comm_row_name, &name_length);
    MPI_Comm_get_name(comm_col, comm_col_name, &name_length);
    printf("I am process %i=(%i, %i), in communicator %s and %s.\n", rank, p_row, p_col, comm_row_name, comm_col_name);
}


void matrix_multiply_add(long double *C, long double *A, long double *B, int n) {
    for (int i = 0; i < n; i ++ ) {
        for (int j = 0; j < n; j ++) {
            //i, j traverses the C matrix
            for (int iter = 0; iter < n; iter ++) {
                C[i * n + j] += A[i * n + iter] * B[iter * n + j];
            }
        }
    }
}

void print_array(long double* M, char name, int rank, int p_row, int p_col, int n) {
    printf("Process rank %i (coordinates: %i, %i), block of %c:\n", rank, p_row, p_col, name);
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j ++) {
            printf("%3.0Lf ", M[i * n + j]);
        }
        printf("\n");
    }
    return;
}

int validate_p(int p, int N){
    int iroot;
    long double froot;

    froot = sqrt((long double)p);
    iroot = froot;

    if (iroot != froot) {
        program_abort(NULL, "Invalid argument: num_procs has to be a perfect square.\n");
    } else if (N % iroot != 0) {
        program_abort(NULL, "Invalid argument: sqrt(num_procs) has to evenly divide N\n");
    }
    return iroot;
}

static void program_abort(char *exec_name, char *message) {
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (my_rank == 0) {
        fprintf(stderr, "%s", message);
    }

    if (exec_name) {
        print_usage(exec_name);
    }
    //MPI_Comm_free(&MPI_COMM_WORLD);
    MPI_Finalize();
    exit(1);
}

static void print_usage(char *exec_name) {
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if(my_rank == 0){
        fprintf(stderr, "smpirun -np <num processes>\n");
        fprintf(stderr, "       -platform <XML plarform file> -hostfile <host file>\n");
        fprintf(stderr, "       %s <dimension>\n", exec_name);
        fprintf(stderr, "MPIRUN arguments:\n");
        fprintf(stderr, "\t<num processes>: number of MPI processes. Has to be a perfect square\n");
        fprintf(stderr, "\t<XML platform file>: a Simgrid plarfoem description file\n");
        fprintf(stderr, "\t<host file>: MPI host file with host names from the plarform file\n");
        fprintf(stderr, "PROGRAM arguments:\n");
        fprintf(stderr, "\t<dimension>: the dimension of square matrix. Has to be >0 and divisible by sqrt(<num processes>)");
        fprintf(stderr, "\n");
    }
    return;
}