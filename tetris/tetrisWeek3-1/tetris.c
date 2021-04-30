/*
TODO
* space 누르면 제일 밑으로 내려가도록 구현
* rank 기능 2가지 추가구현
*/

#include "tetris.h"

static struct sigaction act, oact;
int B,count,score_number;
RankPointer ranklist = NULL;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);
    createRankList();

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
    case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j,k;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;
	gameOver=0;
	timed_out=0;

    k = recommend(field, 2);
	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
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

	move(9,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(10,WIDTH+10,4,8);

    /* score를 보여주는 공간의 태두리를 그린다.*/
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);
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
    case ' ':
        while(drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX))
            blockY++;
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
	move(18,WIDTH+11); /*todo: check whether the position is okay*/
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
	for( i = 0; i < 4; i++ ){
		move(11+i,WIDTH+13); /*todo: check whether the position is okay*/
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
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
                        newRank(score);
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
    DrawBlockWithFeatures(blockY,blockX,currentBlock,blockRotate);
}

void BlockDown(int sig){
    int recScore = 0;
    /*check whether can go down further*/
    if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)){
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    }
    else{ /*block cannot go further*/
        //current block is at -1 (initial): GAMEOVER!!
        if(blockY == -1) gameOver = TRUE;

        //there is a block below. stop and fix to field
        score = score + AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
        score = DeleteLine(field);
        PrintScore(score);

        //update block
        nextBlock[0] = nextBlock[1];
        nextBlock[1] = nextBlock[2];
        nextBlock[2] = rand()%7;
        DrawNextBlock(nextBlock);
        printw("hi0", recScore); /*delete*/
        recScore = recommend(field, 2); /*todo: current block만 고려*/

        //initialise location of currentBlock
        blockRotate=0;
        blockY=-1;/*top*/
        blockX=WIDTH/2-2; /*middle of the row*/
    }
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	//Block이 추가된 영역의 필드값을 바꾼다.
    int i, j;
    int touched = 0;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(block[currentBlock][blockRotate][i][j]){
                if (f[blockY+i+1][blockX+j] == 1 || blockY+i+1 == HEIGHT)
                    touched++;
            }
        }
    }
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(block[currentBlock][blockRotate][i][j]){
                f[blockY+i][blockX+j] = 1;
            }
        }
    }
    return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
    //1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
    int i, j, k, l;
    int fl = 1;
    int deletedline = 0;
    for(i=HEIGHT-1; i>=0; i--){
        for(j=0; j<WIDTH; j++){
            if(f[i][j] == 0){
                fl = 0;
            }
            if((j == WIDTH - 1) && (fl == 1)){ /*if the row is filled till the end*/
                deletedline ++;
                /*then delete whole line*/
                for(k=i-1; k>=0; k--){
                    for(l=0; l<WIDTH; l++){
                        f[k+1][l] = f[k][l]; /*update whole field from previous row*/
                        if(k == 0){
                            f[k][l] = 0; /*initialise as 0 for the first row*/
                        }
                    }
                }
            }
        }
        fl = 1;
    }
    DrawField();
    score = score + (deletedline << 2)*100;
    return score;
}

int RecAddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	//Block이 추가된 영역의 필드값을 바꾼다.
    int i, j;
    int touched = 0;
    int buttomTouched = 0;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(block[currentBlock][blockRotate][i][j]){
                if (f[blockY+i+1][blockX+j] == 1)
                    touched++;
                else if (blockY+i+1 == HEIGHT)
                    buttomTouched++;
            }
        }
    }
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(block[currentBlock][blockRotate][i][j]){
                f[blockY+i][blockX+j] = 1;
            }
        }
    }
    return touched * 10 + buttomTouched * 50 + blockY * 10;
}

int RecDeleteLine(char f[HEIGHT][WIDTH], int childScore){
    //1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
    int i, j, k, l;
    int fl = 1;
    int deletedline = 0;
    for(i=HEIGHT-1; i>=0; i--){
        for(j=0; j<WIDTH; j++){
            if(f[i][j] == 0){
                fl = 0;
            }
            if((j == WIDTH - 1) && (fl == 1)){ /*if the row is filled till the end*/
                deletedline ++;
                /*then delete whole line*/
                for(k=i-1; k>=0; k--){
                    for(l=0; l<WIDTH; l++){
                        f[k+1][l] = f[k][l]; /*update whole field from previous row*/
                        if(k == 0){
                            f[k][l] = 0; /*initialise as 0 for the first row*/
                        }
                    }
                }
            }
        }
        fl = 1;
    }
    childScore = childScore + (deletedline << 2)*100;
    return childScore;
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
    int i=y;
    for(i=y; i<HEIGHT-1; i++){
        if (!CheckToMove(field, blockID, blockRotate, i, x))
            break;
    }
	DrawBlock(--i, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
    DrawField();
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
    DrawRecommend(recommendY, recommendX, recommendID, recommendR);
}

////////////////////////////////////////////////////////////////////////////////////

void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *fp;
	int i, j, len;
  int cnt = EOF;
  int score;
  char name[NAMELEN];

	//1. 파일 열기
	fp = fopen("rank.txt", "r");

	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
	if (!feof(fp)) {
        fscanf(fp, "%d", &score_number);
        for(i=0; i<score_number; i++){
            fscanf(fp, "%s %d", name, &score);
            insert(&ranklist, score, name);
            cnt++;
        }
	}

	// 4. 파일닫기
	fclose(fp);
}

void rank(){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X=1, Y=score_number, ch, i, j;
  int cnt = 0;
  RankPointer cur = ranklist;
	clear();

	//2. printw()로 3개의 메뉴출력
	printw("1. List ranks from X to Y\n");
	printw("2. List ranks by a specific name\n");
	printw("3. Delete a specific rank X\n");

	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
  ch = wgetch(stdscr);

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {
    printw("X: ");
    echo();
    scanw("%d", &X);
    printw("Y: ");
    echo();
    scanw("%d", &Y);
    if(X>Y)
        printw("search failure: no rank in the list\n");
    else{
      printw("\n    name    |    score    ");
      printw("\n===========================");
      if(!(IS_EMPTY(ranklist))){
        for(; cur; cur = cur -> next){
            cnt++;
            if(cnt>=X && cnt <= Y){
              printw("\n%-20s%-10d", cur->name, cur->score);
            }
            if(cnt>Y)
              break;
        }
      }
    }
		noecho();
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;
        bool fl = TRUE;
        printw("Input the name: ");
        echo();
        scanw("%s", &str);
        noecho();
        printw("\n    name    |    score    ");
        printw("\n===========================");
        if(!(IS_EMPTY(ranklist))){
            for(; cur; cur = cur -> next){
                if(strcmp(cur->name, str) == 0){
                    printw("\n%-20s%-10d", cur->name, cur->score);
                    fl = FALSE;
                }
            }
        }
        if(fl) printw("\nsearch failure: no name on the list");
    }

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;
        RankPointer tmpptr;
        printw("Input the rank: ");
        echo();
        scanw("%d", &num);
        noecho();
        if(num < 0 | num > score_number | IS_EMPTY(ranklist))
            printw("\nSearch failure: the rank not in the list");
        else if (num == 1){
            if(!(IS_EMPTY(ranklist))){
                ranklist = cur->next;
                score_number--;
                writeRankFile();
            }
        }
        else{
            if(!(IS_EMPTY(ranklist))){
                for(; cur; cur = cur -> next){
                    cnt++;
                    if(cnt == num-1){
                        tmpptr = cur->next->next;
                        cur->next->next = NULL;
                        cur->next = tmpptr;
                        break;
                    }
                }
            score_number--;
            writeRankFile();
            }
        }
	}

	getch();
}

void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	int sn, i;
    int cnt = 0;
    RankPointer cur = ranklist;

	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "w");

    if(ranklist){
        //2. 랭킹 정보들의 수를 "rank.txt"에 기록
        fprintf(fp, "%d", score_number);
        fprintf(fp, "\n");
        //3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
        for(; cur; cur = cur->next){
            fwrite(cur->name, sizeof(cur->name), 1, fp);
            fprintf(fp, " ");
            fprintf(fp, "%d", cur->score);
            fprintf(fp, "\n");
        }
    }

    fclose(fp);
}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	clear();

	//1. 사용자 이름을 입력받음
  echo();
	printw("Enter your name: ");
  scanw("%s", str);
  noecho();

	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
	if(!str)
		strcpy(str, "N/A");
  insert(&ranklist, score, str);
  score_number++;
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(char fieldOri[HEIGHT][WIDTH], int lv){
    int i, j;
    int childCnt = 0;
    int travCnt = 0;
    int maxScore = 0;
    int fl = 0;
    RecNode* maxNode = NULL;

    //tree 만들기
    // RecNode* root = (RecNode*)malloc(sizeof(RecNode));
    RecNode* root = (RecNode*)calloc(1, sizeof(RecNode));
    root->lv = 0; root->score = score; memcpy(root->recField, fieldOri, sizeof(char)*HEIGHT*WIDTH);
    for(i = 0; i <= 3; i++){ /*all possible rotation*/
        for(j = 0; j <= 9; j++){ /*all possible location*/
            if(CheckToMove(root->recField, nextBlock[root->lv], i, -1, j))
                root->children[childCnt++] = createRecNode(root, lv, j, i);
        }
    }
    
    //max score 찾기
    maxNode = root;
    maxScore = root->score;
    travTree(root, lv, &maxNode, &maxScore, &fl);
    
    //draw recommend block on the field
    while(maxNode->lv > 1)
        maxNode = maxNode->parent;
    recommendID = maxNode -> curBlockID;
    recommendR = maxNode -> recBlockRotate;
    recommendY = maxNode -> recBlockY;
    recommendX = maxNode -> recBlockX;

    //free memory
    freeTree(root, lv);
    
    return maxScore;
}

RecNode* createRecNode(RecNode* parent, int maxlv, int recBlockX, int recRotate){
    if (parent->lv == maxlv-1){
        // RecNode* child = (RecNode*)malloc(sizeof(RecNode));
        RecNode* child = (RecNode*)calloc(1, sizeof(RecNode));
        child->lv = (parent->lv)+1; /*should be maxlv*/
        child->score = parent->score;
        child->parent = parent;
        *(child->children) = NULL;
        memcpy(child->recField, parent->recField, sizeof(char)*HEIGHT*WIDTH);
        child->curBlockID = nextBlock[parent->lv];
        child->recBlockX = recBlockX;
        child->recBlockY = -1;
        child->recBlockRotate = recRotate;
        while(CheckToMove(child->recField, child->curBlockID, child->recBlockRotate, child->recBlockY+1, child->recBlockX)) (child->recBlockY)++;
        child->score = (child->score) + RecAddBlockToField(child->recField, child->curBlockID, child->recBlockRotate, child->recBlockY, child->recBlockX);
        child->score = RecDeleteLine(child->recField, child->score);
        return child;
    }
    else{
        int i, j;
        int childCnt = 0;
        // RecNode* child = (RecNode*)malloc(sizeof(RecNode));
        RecNode* child = (RecNode*)calloc(1, sizeof(RecNode));
        child->lv = (parent->lv)+1; /*should be maxlv*/
        child->score = parent->score;
        child->parent = parent;
        memcpy(child->recField, parent->recField, sizeof(char)*HEIGHT*WIDTH);
        child->curBlockID = nextBlock[parent->lv];
        child->recBlockX = recBlockX;
        child->recBlockY = -1;
        child->recBlockRotate = recRotate;
        while(CheckToMove(child->recField, child->curBlockID, child->recBlockRotate, child->recBlockY+1, child->recBlockX)) (child->recBlockY)++;
        child->score = (child->score) + RecAddBlockToField(child->recField, child->curBlockID, child->recBlockRotate, child->recBlockY, child->recBlockX);
        child->score = RecDeleteLine(child->recField, child->score);
        for(i = 0; i <= 3; i++){ /*all possible rotation*/
            for(j = 0; j <= 9; j++){ /*all possible location*/
                if(CheckToMove(child->recField, nextBlock[child->lv], i, -1, j))
                        child->children[childCnt++] = createRecNode(child, maxlv, j, i);
            }
        }
        return child;
    }
}

void travTree(RecNode* Node, int lv, RecNode** maxNode, int* maxScore, int* fl){
    int i = 0;
    RecNode* travNode = NULL;
    if(Node->lv == lv){
        if(fl == 0){
            *maxNode = Node; *maxScore = Node->score; *fl = 1;
        }
      if(Node->score > *maxScore){
        *maxNode = Node; *maxScore = Node->score;
      }
    }
    else{
        travNode = Node->children[i];
        for(; travNode; travNode = Node->children[i++])
                travTree(travNode, lv, maxNode, maxScore, fl);
    }
}

void freeTree(RecNode* Node, int lv){
    int i = 0;
    RecNode* travNode = NULL;
    if(Node->lv == lv)
      free(Node);
    else{
        for(; travNode; travNode = travNode->children[i++])
            freeTree(travNode, lv);
    }
}

void recommendedPlay(){
	// user code
}
