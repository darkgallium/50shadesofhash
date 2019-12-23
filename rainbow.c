#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "rainbow.h"
#include "table.h"
#include "crypto.h"
#include "io.h"

/**
 * rainbow, a distributed program calculating Rainbow Tables
 */

int plaintext_size = 7;

int m0 = 30678350, t0 = 7000;

FILE *perf_log_f = NULL;
char *output_table_fname = "table.out";
char *perf_log_fname = NULL;

// This charset is used by the reduction function
// TODO: make this more easily changeable
const char charset[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' 
};

/*
 * This function return a UNIX timestamp in milliseconds as an unsigned long long int.
 */
unsigned long long get_timestamp() {
        struct timeval tv;
        gettimeofday(&tv, NULL);

        return (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
}

/**
 * This function initializes the starting points at the begining of the program
 * It can be changed to generate any evenly distributed set of points in the plaintext domain
 * For sake of simplicity it is here a simple hex counter.
 */
void choose_start_pts(char *start_pts, int m0) {
	int i;
	
	for (i = 0; i < m0; i++) {
		sprintf((start_pts+i*plaintext_size), "%.*x", plaintext_size-1, i);
	}
}

/**
 * This is the reduction function, used to get an element from the plaintext domain from an element from the hash domain
 * Our reduction function here corresponds to a modulo on each character of the hash. 
 * The result is then reduced to the number of character maximum of a password.
 * Note that in RT, the reduction function has to be different for each column, here we do it by summing to each character of the hash, the column number.
 *
 * @param n 	column number
 * @param pass	the hash that will be transformed (in place) to a password
 */
void reduction(int n, char *pass) {
	int i;

	for (i = 0; i < plaintext_size-1; i++) {
		pass[i] = charset[ (((int) pass[i]) + n) % sizeof(charset)];
	}

	pass[plaintext_size-1] = '\0';
}


/**
 * This function removes the duplicate endpoints from an *already sorted* endpoint list.
 * It needs also the start points because we also need to clean the ones corresponding to duplicate endpoints.
 *
 * @param start_pts	the list of starting points
 * @param end_pts	the sorted list of endpoints
 * @param m		a pointer to the current number of lines, that will be changed according to the removed duplicates
 */
void remove_duplicates(char *start_pts, char *end_pts, int *m) {

	int i, r = 0;
	for (i = 1; i < *m; i++) {
		if (strcmp(end_pts+r*plaintext_size, end_pts+i*plaintext_size) != 0) {
			r++;
			memcpy(end_pts+r*plaintext_size, end_pts+i*plaintext_size, plaintext_size);
			memcpy(start_pts+r*plaintext_size, start_pts+i*plaintext_size, plaintext_size);
		}
	}
	*m = r + 1;
}

/**
 * This function computes a chain starting and ending from given columns.
 * By computing a chain, we mean from a given starting point, calulating its hash and then applying the reduction function,
 * then calculating the hash of the result and applying the reduction function, and so on, until reaching the specified end column.
 *
 * @param start_elt	  the element (from the plaintext domain) from which we start
 * @param endpoint	  the result of the computation of the chain
 * @param start_col	  the column number from which we start (aka the column number of the start_elt)
 * @param end_col	  the column number at which we stop the computation of the chain
 */
void compute_chain_slice(char *start_elt, int start_col, int end_col, char *endpoint) {
	int i;
	char *a = malloc(HASH_LENGTH);
	char *b = malloc(HASH_LENGTH);

	memcpy(b, start_elt, plaintext_size);

	for (i = start_col; i < end_col; i++) {
		hash_from_string(b, a);
		reduction(i, a);
		memcpy(b,a, plaintext_size);
	}	
	
	memcpy(endpoint, a, plaintext_size);
	
	free(a);
        free(b);
}

void print_usage(char *argv[]) {
	printf("usage: %s [-m <val>] [-t <val>] [-p <path>] [-o path>] [-c <val>] [-N <val>] [-h]\n", argv[0]);
	exit(EXIT_FAILURE);
}


/*
 * This function return the plaintext size given the exponent of N.
 * Theory says that the reduction function shall generate <exponent of N> bits given the hash.
 * Moreover, it says that len(SP) = log2(m0), len(EP) = log2(N).
 * First, we will consider that plaintext size = len(SP) = len(EP) = log2(N) > log2(m0).
 * So we will need to store (N_exponent / 8) bytes of infomation.
 * The 2-factor is due to the data structure we use for storage which is composed of chars.
 * -> it would be good to get rid of this 2-factor but this would mean changing the data structure
 */
int get_plaintext_size(int N_exponent) {
	return 2*(N_exponent/8);
}

int main(int argc, char *argv[]) {
	char opt;	

	omp_set_num_threads(4);

	while ((opt = getopt(argc, argv, "p:N:m:t:o:h")) != -1) {
		switch (opt) {
			case 'h':
				print_usage(argv);
				break;

			case 'p':
				if ((perf_log_f = fopen(optarg, "a")) == NULL) {
					fprintf(stderr, "An error occured while attempting to open %s\nPerhaps a permission problem.\n", optarg);
					exit(EXIT_FAILURE);
				}

				break;

			case 'N':
				// The +1 is needed to store the null terminator (\0)
				plaintext_size = get_plaintext_size(atoi(optarg))+1;
				break;
		
			case 'o':
				output_table_fname = optarg;
				break;

			case 'm':
				m0 = atoi(optarg);
				break;

			case 't':
				t0 = atoi(optarg);
				break;

			case '?':
				print_usage(argv);
		}
	}
	
	// Bunch of sanity checks

	assert(plaintext_size > 1);
	assert(m0 > 0);
	assert(t0 > 0);
	assert(m0 < pow(16, plaintext_size-1));
	
	int i = 0;
	double start_time, end_time;
                                                                                                          
        char *start_pts = malloc(m0*plaintext_size);
        char *end_pts = malloc(m0*plaintext_size);
        
        start_time = omp_get_wtime();
                                                                                                          
        choose_start_pts(start_pts, m0);
        memcpy(end_pts, start_pts, plaintext_size*m0);
                 
	#pragma omp parallel for	
        for (i = 0; i < m0; i++) {
        	compute_chain_slice(end_pts+i*plaintext_size, 0, t0, end_pts+i*plaintext_size);
	//	printf("sp=%s, ep=%s\n", start_pts+i*plaintext_size, end_pts+i*plaintext_size);
        }
                                                                                                          
        quicksort(start_pts, end_pts, 0, m0-1);
        remove_duplicates(start_pts, end_pts, &m0);
                                                                                                          
        printf("saving results to file, m_fin=%d\n",m0);
        save_table_to_file(start_pts, end_pts, m0, output_table_fname);
        
        end_time = omp_get_wtime();
        
        printf("total duration: %f\n", (end_time-start_time));

	return 0;
}
