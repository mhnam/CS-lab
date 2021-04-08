#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(9,WIDTH+10);
	printw("SCORE");
	DrawBox(10,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP: /*rotate*/
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN: /*goes down*/
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT: /*goes right*/
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT: /*goes left*/
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(11,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}
    }
    move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=0;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    // this function checks whether it can move to given position return 1 if possible 0 otherwise
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(block[currentBlock][blockRotate][i][j]){ /*if there is an element in the block*/
                if(f[blockY+i][blockX+j]){
                    /*if there is already an element in field*/
                    return 0;
                }
                if(blockY+i >= HEIGHT || blockX+j < 0 || blockX + j >= WIDTH){
                    /*if the block wants to go out of field*/
                    return 0;
                }
            }
        }
    }
    return 1; /*can proceed!*/
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
    int tmpBlockY = blockY;
    int tmpBlockX = blockX;
    int tmpblockRotate = blockRotate;
	int i,j;

    switch(command){
	case KEY_UP:
        tmpblockRotate = ((tmpblockRotate-1 >= 0) ? (tmpblockRotate-1)%4 : 3);
		break;
	case KEY_DOWN:
        tmpBlockY--;
		break;
	case KEY_RIGHT:
        tmpBlockX--;
		break;
	case KEY_LEFT:
        tmpBlockX++;
		break;
	default:
		break;
	}
    
    //2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][tmpblockRotate][i][j]==1 && i+tmpBlockY>=0){
				move(i+tmpBlockY+1,j+tmpBlockX+1);
				printw(".");
			}
		}
    }
    
    //3. 새로운 블록 정보를 그린다.
    DrawBlock(blockY,blockX,currentBlock,blockRotate,' ');
}

void BlockDown(int sig){
    int score;
    /*check whether can go down further*/
    if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)){
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    } 
    else{ /*block cannot go further*/
        //current block is at -1 (initial): GAMEOVER!!
        if(blockY == -1) gameOver = TRUE;
        
        //there is a block below. stop and fix to field
        AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX); 
        score = DeleteLine(field);
        PrintScore(score);
        
        //update block
        nextBlock[0] = nextBlock[1];
        nextBlock[1] = rand()%7;
        DrawNextBlock(nextBlock);
        
        //initialise location of currentBlock
        blockRotate=0;
        blockY=-1;/*top*/
        blockX=WIDTH/2-2; /*middle of the row*/
    }
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	//Block이 추가된 영역의 필드값을 바꾼다.
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(block[currentBlock][blockRotate][i][j])
                f[blockY+i][blockX+j] = 1;
        }
    }
}

int DeleteLine(char f[HEIGHT][WIDTH]){
    //1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
    int i, j, k, l;
    int score = 0;
    int fl = 1;
    for(i=HEIGHT-1; i>=0; i--){
        for(j=0; j<WIDTH; j++){
            if(f[i][j] == 0){
                fl = 0;
            }
            if((j == WIDTH - 1) && (fl == 1)){
                score++;
                if(j == WIDTH-1){ /*loop went over till the end of the row*/
                    for(k=i-1; k>=0; k--){
                        for(l=0; l<WIDTH; l++){
                            f[k+1][l] = f[k][l]; /*update whole field above the row*/
                            if(k == 0){
                                f[k][l] = 0; /*initialise as 0 for the first row*/
                            }
                        }
                    }
                }
            }
        }
        fl = 1;
    }
    DrawField();
    return score;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
