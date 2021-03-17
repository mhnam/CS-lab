#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

//initiate computation for every input
void compute_all(int num_input, int* input_ary);

//starts to count number of digits for given input and update in addr
void compute(int max_page, int* addr);

//counts occurrence of given number and update in addr
void count_number(int i, int* addr);

//prints out the results
void print_out(int num_input, int* input_ary);
