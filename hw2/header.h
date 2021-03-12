#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define MAXNUM 1000000000

void compute_all(int num_input, int* input_ary);
void compute(int max_page, int* addr);
void count_number(int i, int* addr);