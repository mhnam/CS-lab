#include "header.h"

int main(){
	int num_input;
	scanf("%d", num_input);

	int input_ary[num_input-1][11] = {0};
	for(int i=0; i<num_input; i++){
		scanf("%d", input_ary[i][0]);
	}
	compute_all(num_input, &input_ary);
}

void compute_all(int num_input, int* input_ary){
	for(int i=0; i<num_input; i++){
		int max_page = input_ary[i][0];
		int* addr = &input_ary[i];
		compute(max_page, addr);
	}
}

void compute(int max_page, int* addr){
	for(int i=max_page; i>0; i--){
		count_number(i, addr);
	}
}

void count_number(int i, int* addr){
	while(i>0){
		int q, r;
		q = int(i/10);
		addr+sizeof(int*)*(r+1) = i%10;
	}
}