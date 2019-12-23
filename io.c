#include "rainbow.h"
#include "table.h"

/*
 * This function saves the table to a file given the corresponding starting/end points,
 * the size of the table and the file name.
 * For the moment, as during the precomputation itself, the storing is not optimized whatsoever.
 * For example, you could reduce the size of the tables by taking advantage of the format of the starting points to store less
 * information.
 *
 * @param start_pts	a pointer to the starting points array
 * @param end_pts	a pointer to the end points array
 * @param m		the size of the start/end points array (i.e the number of lines in the table)
 * @param fname		a string containing the path to the file where we will save the table
 */
void save_table_to_file(char *start_pts, char *end_pts, int m, char *fname) {
	int i;
	FILE *f;

	if ( (f = fopen(fname, "w")) != NULL) {
		for (i = 0; i < m; i++) {
			fprintf(f, "%s,%s\n", start_pts+i*plaintext_size, end_pts+i*plaintext_size);
		}

		fclose(f);
	}

	else {
		fprintf(stderr, "Error while opening file %s\n", fname);
		exit(EXIT_FAILURE);
	}

}

