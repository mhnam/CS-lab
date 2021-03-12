#include "header.h"

void print_out(int num_input, int* input_ary){
    int cnt=1;
    for(int i=0; i<num_input; i++){
        for(int j=0; j<10; j++){
            printf("%d ", input_ary[cnt++]);
        }
        printf("\n");
        cnt++; /*첫번째 인덱스에 들어있는 max_page 출력을 막기 위함*/
    }
}