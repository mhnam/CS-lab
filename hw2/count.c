#include "header.h"

void compute_all(int num_input, int* input_ary){
	for(int i=0; i<num_input; i++){
        int index=i*11;
		int max_page = input_ary[index];
    	int* addr = &input_ary[index];
    	compute(max_page, addr);
	}
}

void compute(int max_page, int* addr){
	for(int i=max_page; i>0; i--){
		count_number(i, addr);
	}
}

void count_number(int i, int* addr){
    int q, r;
    q = i;
	while(q>0){
		r = q%10;
        addr[r+1] = addr[r+1]+1;
		q = q/10;
	}
}