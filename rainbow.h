#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <omp.h>

#ifndef RAINBOW_H
#define RAINBOW_H

extern int plaintext_size;

#define PERF_LOG(file, f_, ...) if (file != NULL) { fprintf(file, (f_), ##__VA_ARGS__); }

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#endif
