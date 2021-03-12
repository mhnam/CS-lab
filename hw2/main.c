#include "header.h"

int main(){
	int num_input, index;
	scanf("%d", &num_input);
    int* input_ary=(int*)calloc(11*num_input, sizeof(int));
    if(input_ary==NULL){
		printf("Memory allocation error\n");
		exit(-1);
    }

	for(int i=0; i<num_input; i++){
        index=i*11;
		scanf("%d", &input_ary[index]);
	}
    compute_all(num_input, input_ary);
    print_out(num_input, input_ary);
    free(input_ary);
    return 0;
}