#include "header.h"

/***
@purpose
This function prints out the results

@input
- int num_input: number of total input
- int* input_ary: address of array which saves results
***/

void print_out(int num_input, int* input_ary){
    int cnt=1; /*cnt manages the index of array*/
    for(int i=0; i<num_input; i++){ /*repeats for 'number of input' times*/
        for(int j=0; j<10; j++){ /*repeats for 10 times for each digit (0-9)*/
            printf("%d ", input_ary[cnt++]);
        }
        printf("\n"); /*new line for each input*/
        cnt++; /*to avoid print-out an input which is saved as the first element for each case*/
    }
}
