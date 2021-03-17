#include "header.h"

/***
@purpose
This function initiate computation for every input

@input
- int num_input: number of total input
- int* input_ary: address of array which saves results
***/
void compute_all(int num_input, int* input_ary){
	for(int i=0; i<num_input; i++){ /*repeats for 'number of input' times*/
		//prepare to call compute() function
		int index=i*11;
		int max_page = input_ary[index];
    int* addr = &input_ary[index];
		//call compute() function for given input
    compute(max_page, addr);
	}
}

/***
@purpose
This function starts to count number of digits for given input and update in addr

@input
- int max_page: max number
- int* addr: address of array which saves results
***/
void compute(int max_page, int* addr){
	for(int i=max_page; i>0; i--){ /*repeates from the max_page (input) till 1*/
		count_number(i, addr); /*for a given page (number) count the number of occurrence of digits by calling count_number()*/
	}
}

/***
@purpose
This function counts occurrence of given number and update in addr

@input
- int i: number to counts
- int* addr: address of array which saves results
***/
void count_number(int i, int* addr){
	int q, r;
  q = i;
	while(q>0){ /*process repeats till the given input ranges between 1 to 9*/
		r = q%10; /*extract the right most digit from the given input*/
    addr[r+1] = addr[r+1]+1; /*update counts in addr array*/
		q = (int)(q/10); /*update given input removing the last digit by dividing 10*/
	}
}
