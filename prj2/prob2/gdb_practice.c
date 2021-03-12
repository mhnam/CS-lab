#include <stdio.h>

void main(void)
{
    int i;
    double num;
    
    for (i=0; i<5; i++){
        // num=i/2 + i; 
        /*not sure what is the aim of this program, but this line makes the calculation inaccurate*/
        num=i/2.0 + i;
        printf("num in %f \n", num);
    }
}