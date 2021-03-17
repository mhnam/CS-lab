#include "header.h"

int main(){
	int num_input, index;

	//get a number of inputs as the first input
	scanf("%d", &num_input);

	//dynamic memory allocation for number of inputs (11 integers for each input)
	//1st column: input
	//2nd - 11th column: counts the number of occurrence of each digit (0-9)
	int* input_ary=(int*)calloc(11*num_input, sizeof(int));
  if(input_ary==NULL){
		printf("Memory allocation error\n");
		exit(-1);
  }

	//get inputs: repeat for 'num_input' times
	for(int i=0; i<num_input; i++){
		index=i*11;
		scanf("%d", &input_ary[index]);
	}

  compute_all(num_input, input_ary); /*counts the number of occurrence of each digits for all input*/
  print_out(num_input, input_ary); /*print out the results*/

  free(input_ary); /*free dynamic allocated memory*/
  return 0;
}
