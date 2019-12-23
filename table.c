#include "rainbow.h"
#include "table.h"

/*
 * Helper function that shows starting and ending points given the 
 * corresponding arrays and upper/lower bounds.
 */
void show_table(char *start_pts, char *end_pts, int first, int last) {
	int i;

	for (i = first; i < last; i++) {
		printf("%s | %s\n", start_pts+i*plaintext_size, end_pts+i*plaintext_size);
	}
}

/*
 * Quicksort function adapted from https://beginnersbook.com/2015/02/quicksort-program-in-c/
 * It sorts only the endpoints but moves the starting points to the 
 * same index as their corresponding end points.
 *
 * @param start_pts	array containing the starting points
 * @param end_pts	array containing the end points
 * @param first		starting index for the sort
 * @param last		ending index for the sort
 */
void quicksort(char *start_pts, char *end_pts, int first, int last) {
	int i, j, pivot;

	if(first < last){
	
		char *s = malloc(plaintext_size);
                char *e = malloc(plaintext_size);
		
		//printf("first=%d, last=%d\n", first, last);
		
		pivot=first;
		i=first;
		j=last;

		while(i<j){
			while(strcmp(end_pts+pivot*plaintext_size, end_pts+i*plaintext_size) >= 0 && i<last)
				i++;
			while(strcmp(end_pts+j*plaintext_size, end_pts+pivot*plaintext_size) > 0)
				j--;
			
			if(i<j){
				memcpy(s, start_pts+i*plaintext_size, plaintext_size);
				memcpy(e, end_pts+i*plaintext_size, plaintext_size);
				memcpy(start_pts+i*plaintext_size, start_pts+j*plaintext_size, plaintext_size);
				memcpy(end_pts+i*plaintext_size, end_pts+j*plaintext_size, plaintext_size);
				memcpy(start_pts+j*plaintext_size, s, plaintext_size);
				memcpy(end_pts+j*plaintext_size, e, plaintext_size);
			}
		}

		memcpy(s, start_pts+pivot*plaintext_size, plaintext_size);
                memcpy(e, end_pts+pivot*plaintext_size, plaintext_size);
                memcpy(start_pts+pivot*plaintext_size, start_pts+j*plaintext_size, plaintext_size);
                memcpy(end_pts+pivot*plaintext_size, end_pts+j*plaintext_size, plaintext_size);
                memcpy(start_pts+j*plaintext_size, s, plaintext_size);
                memcpy(end_pts+j*plaintext_size, e, plaintext_size);

		free(e);
		free(s);

		quicksort(start_pts, end_pts, first, j-1);
		quicksort(start_pts, end_pts, j+1, last);

	}
}

// Adapted from https://www.geeksforgeeks.org/merge-sort/
void merge(char *start_pts, char *end_pts, int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 

	/* create temp arrays */
	char *L_s = malloc(n1*plaintext_size);
	char *R_s = malloc(n2*plaintext_size);
	char *L_e = malloc(n1*plaintext_size);
	char *R_e = malloc(n2*plaintext_size);

	/* Copy data to temp arrays L[] and R[] */
	memcpy(L_s, start_pts+l*plaintext_size, n1*plaintext_size);
	memcpy(R_s, start_pts+(m+1)*plaintext_size, n2*plaintext_size);
	memcpy(L_e, end_pts+l*plaintext_size, n1*plaintext_size);	
	memcpy(R_e, end_pts+(m+1)*plaintext_size, n2*plaintext_size);

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	
	while (i < n1 && j < n2) 
    	{ 
        	if (strcmp(L_e+i*plaintext_size, R_e+j*plaintext_size) < 0) { 
			memcpy(start_pts+k*plaintext_size, L_s+i*plaintext_size, plaintext_size); 
			memcpy(end_pts+k*plaintext_size, L_e+i*plaintext_size, plaintext_size);
			i++; 
		} 
		else { 
			memcpy(start_pts+k*plaintext_size, R_s+j*plaintext_size, plaintext_size); 
	                memcpy(end_pts+k*plaintext_size, R_e+j*plaintext_size, plaintext_size); 
			j++; 
		} 
		k++; 
	} 
  
	/* Copy the remaining elements of L[], if there 
	are any */
	while (i < n1) 
	{ 
		memcpy(start_pts+k*plaintext_size, L_s+i*plaintext_size, plaintext_size); 
                memcpy(end_pts+k*plaintext_size, L_e+i*plaintext_size, plaintext_size);
		i++; 
		k++; 
	} 

	/* Copy the remaining elements of R[], if there 
	are any */
	while (j < n2) 
	{ 
		memcpy(start_pts+k*plaintext_size, R_s+j*plaintext_size, plaintext_size); 
                memcpy(end_pts+k*plaintext_size, R_e+j*plaintext_size, plaintext_size); 
		j++; 
		k++; 
	}
	
	free(L_s);
	free(L_e);
	free(R_s);
	free(R_e); 
} 

/*
 * MergeSort function adapted from https://www.geeksforgeeks.org/merge-sort/
 * It sorts only the endpoints but moves the starting points to the 
 * same index as their corresponding end points.
 *
 * @param start_pts	array containing the starting points
 * @param end_pts	array containing the end points
 * @param l		starting index for the sort
 * @param r		ending index for the sort
 */
void mergeSort(char *start_pts, char *end_pts, int l, int r) 
{ 
	if (l < r) 
	{ 
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		int m = l+(r-l)/2; 

		// Sort first and second halves 
		mergeSort(start_pts, end_pts, l, m); 
		mergeSort(start_pts, end_pts, m+1, r); 

		merge(start_pts, end_pts, l, m, r); 
	} 
} 

