#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void create_maze(int width, int height);

int main(){
    int i, j;
    int width;
    int height;

    scanf("%d", &width);
    scanf("%d", &height);

    create_maze(width, height);

    return 0;
}

void create_maze(int width, int height){
    int i, j, k, l, fl, tmp;
    int cnt=5;
    int** maze = (int**)malloc(sizeof(int*) * (2*height+1));
    maze[0] = (int*)malloc(sizeof(int) * (2*height+1) * (2*width+1));
    for(i=1; i<2*height+1; i++)
        maze[i] = maze[i-1] + (2*width+1);
    
    FILE *fp = fopen("maze0.maz", "w");
    srand(time(NULL));

    //init values in maze
    for(i=0; i<2*height+1; i++){
        for(j=0; j<2*width+1; j++){
            if(i%2){ /*actual data*/
                if(j%2) maze[i][j] = 0; /*0*/
                else maze[i][j] = 1; /*|*/
            }
            else{ /*frame*/
                if(j%2) maze[i][j] = 2; /*-*/
                else maze[i][j] = 3; /*+*/
            }
        }
    }

    //generate in the middle of rows
    for(i=1; i<2*height+1; i=i+2){
        //step 1: init the first row
        for(j=1; j<2*width; j=j+2){
            if(maze[i][j] == 0){
                maze[i][j] = cnt++;
            }
        }
        
        //step 2: randomly connect the segment within a row
        for(j=1; j<2*(width-1); j=j+2){
            if(rand()%2 && maze[i][j] != maze[i][j+2]){
                maze[i][j+1] = 4; tmp = maze[i][j+2];
                for(k=1; k<2*height+1; k=k+2){
                    for(l=1; l<2*width+1; l=l+2){
                        if(maze[k][l] == tmp) maze[k][l] = maze[i][j];
                    }
                }
            }
        }

        if(i>(height-1)*2) break;

        //step 3: randomly connect with the next row
        tmp = maze[i][0]; fl = 1;
        for(j=1; j<2*width; j=j+2){
           if(tmp != maze[i][j]){
               tmp = maze[i][j]; fl = 1;
           }
           
           if(fl && tmp != maze[i][j+2]){ /*not have been connected with prev row and this is the last elem*/
                maze[i+2][j] = tmp;
                maze[i+1][j] = 4;
           }
            else{
                if(rand()%2){
                    maze[i+2][j] = tmp; fl = 0;
                    maze[i+1][j] = 4;
                }
            }
        }
    }

    //finalise maze construction
    for(i=1; i<2*width-1; i=i+2)
        if(maze[2*height-1][i] != maze[2*height-1][i+2])
            maze[2*height-1][i+1] = 4;

    //print maze
    for(i=0; i<2*height+1; i++){
        for(j=0; j<2*width+1; j++){
            switch (maze[i][j]){
            case 0: printf("0"); break;
            case 1: printf("|"); break;
            case 2: printf("-"); break;
            case 3: printf("+"); break;
            case 4: printf(" "); break;
            default:
                printf(" "); break;
            }
        }
        printf("\n");
    }

    for(i=0; i<2*height+1; i++){
        for(j=0; j<2*width+1; j++){
            switch (maze[i][j]){
            case 0: fprintf(fp, "0"); break;
            case 1: fprintf(fp, "|"); break;
            case 2: fprintf(fp, "-"); break;
            case 3: fprintf(fp, "+"); break;
            case 4: fprintf(fp, " "); break;
            default:
                fprintf(fp, " "); break;
            }
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    free(maze[0]);
    free(maze);
}
