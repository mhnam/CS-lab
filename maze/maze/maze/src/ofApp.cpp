/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include <ctime>
#define drawOffset 0
#define edgeLen 20
#define recSize 0.2
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = false;
	isBFS = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

	dir[0] = { -1, 0 }; /*go down*/
	dir[1] = { 0, 1 }; /*go right*/
	dir[2] = { 1, 0 }; /*go up*/
	dir[3] = { 0, -1 }; /*go left*/

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)	{
			if (!isDFS) {
				isDFS = true; isBFS = !isDFS;
				mazeInit();
				DFS();
				menu->SetPopupItem("Show BFS", false);
				menu->SetPopupItem("Show DFS", true);
			}
			else {
				mazeInit();
				menu->SetPopupItem("Show DFS", false);
				isDFS = false;
			}
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Show BFS") {
		if (isOpen) {
			if (!isBFS) {
				isBFS = true; isDFS = !isBFS;
				mazeInit();
				BFS();
				bShowInfo = bChecked;
				menu->SetPopupItem("Show DFS", false);
				menu->SetPopupItem("Show BFS", true);
			}
			else {
				mazeInit();
				menu->SetPopupItem("Show BFS", false);
				isBFS = false;
			}
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// DRAW MAZE
	if (isOpen) {
		for (i = 0; i < HEIGHT; i += 2) {
			for (j = 0; j < WIDTH; j += 2) {
				/*not last row*/
				if (i != HEIGHT - 1) {
					if (input[i][j + 1] == '-' && j != WIDTH - 1) {
						ofDrawLine((j * edgeLen), i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
					}
					if (input[i + 1][j] == '|' && i != HEIGHT - 1) {
						ofDrawLine(j * edgeLen, i * edgeLen, j * edgeLen, (i + 2) * edgeLen);
					}
				}
				/*last row*/
				else {
					if (input[i][j + 1] == '-' && j != WIDTH - 1) {
						ofDrawLine((j * edgeLen), i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
					}
				}
			}
		}
	}

	if (isDFS){
		ofSetLineWidth(5);

		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if (bShowInfo){
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}

	if (isBFS){
		ofSetLineWidth(5);

		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}
} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked cur
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed cur
		menu->SetPopupItem("Show DFS", bShowInfo);
		menu->SetPopupItem("Show BFS", !bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;

	if (input) freeMemory();
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;

			// CREATE MAZE
			int i, j;
			// STEP 1: get HEIGHT and WIDTH
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; it++) HEIGHT++;
			ofBuffer::Line it = buffer.getLines().begin();
			string line = *it;
			WIDTH = line.size();

			goalX = WIDTH - 2;
			goalY = HEIGHT - 2;

			input = (char**)malloc(sizeof(char*) * (HEIGHT)); /*saves actual maze*/
			visited = (int**)malloc(sizeof(int*) * (HEIGHT)); /*saves visited node*/
			backMaze = (point**)malloc(sizeof(point*) * (HEIGHT));
			for (i = 0; i < HEIGHT; i++) {
				input[i] = (char*)malloc(sizeof(char) * WIDTH);
				visited[i] = (int*)malloc(sizeof(int) * WIDTH);
				backMaze[i] = (point*)malloc(sizeof(point) * WIDTH);
			}

			// STEP 2: READ .MAZ FILE INTO 2D ARRAY
			i = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; it++, i++) {
				string line = *it;

				for (int j = 0; j < WIDTH; j++) {
					input[i][j] = line[j];
					if (line[j] == ' ') {
						visited[i][j] = CAN_MOVE;
						//printf(" ");
					}
					else {
						visited[i][j] = WALL;
						//printf("+");
					}
				}
				//printf("\n");
			}
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {
	for (int i = 0; i < HEIGHT; ++i) {
		free(input[i]);
		free(visited[i]);
		free(backMaze[i]);
	}
	free(input);
	free(visited);
	free(backMaze);
}

//ITERATIVE DFS WITH TARGET
bool ofApp::DFS() {
	//STEP 1: INITALISE
	point cur = { 1, 1 };
	visited[1][1] = PATH;
	stck.push(cur);

	while (!stck.empty()){
		bool fl = true;

		//STEP 2: CHECK WHETHER TARGET, IF SO TRUE
		if (cur.x == WIDTH - 2 && cur.y == HEIGHT - 2){
			cout << "DFS Mode: Found the path!" << endl;
			visited[cur.y][cur.x] = PATH;
			return true;
		}

		// STEP 3: S.TOP HAS AN UNVISITIED ADJACNET NODE
		for (int i = 0; i < 4; i++){
			if (visited[cur.y + dir[i].y][cur.x + dir[i].x] == CAN_MOVE){
				visited[cur.y + dir[i].y][cur.x + dir[i].x] = VISIT;
				visited[cur.y][cur.x] = PATH;
				stck.push(cur);

				/*update cur position*/
				if (i == 0 || i == 2) cur = { cur.y + dir[i].y * 2, cur.x + dir[i].x }; /*go up or down*/
				else if (i == 1 || i == 3) cur = { cur.y + dir[i].y, cur.x + dir[i].x * 2 }; /*go left or right*/

				//cout << "Next position to push y : " << cur.y << " x : " << cur.x << endl;
				fl = false;
				break;
			}
		}

		if (fl) {
			cur = stck.top();
			stck.pop();
			visited[cur.y][cur.x] = NO_PATH;
			//cout << "Next position to pop y : " << cur.y << " x : " << cur.x << endl;
		}
	}
	return false;
}
void ofApp::dfsdraw() {
	for (int i = 1; i < HEIGHT - 1; i += 2) {
		for (int j = 1; j < WIDTH - 1; j += 2) {
			if (visited[i][j] == PATH || visited[i][j] == NO_PATH) {

				if (i != HEIGHT - 1) {/*not last row*/
					//check horizontally
					if (visited[i][j + 1] == VISIT && j != WIDTH - 2) {
						if (visited[i][j + 2] == PATH) {
							(visited[i][j] == PATH) ? ofSetColor(0,255,0) : ofSetColor(255,0,0);
							ofDrawLine(j * edgeLen, i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
						}
						else if (visited[i][j + 2] == NO_PATH) {
							(visited[i][j] == PATH) ? ofSetColor(255,0,0) : ofSetColor(255,0,0);
							ofDrawLine(j * edgeLen, i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
						}

					}
					//check vertically
					if (visited[i + 1][j] == VISIT) {
						if (visited[i + 2][j] == PATH) {
							(visited[i][j] == PATH) ? ofSetColor(0,255,0) : ofSetColor(255,0,0);
							ofDrawLine(j * edgeLen, i * edgeLen, j * edgeLen, (i + 2) * edgeLen);
						}
						else if (visited[i + 2][j] == NO_PATH) {
							(visited[i][j] == PATH) ? ofSetColor(255,0,0) : ofSetColor(255,0,0);
							ofDrawLine(j * edgeLen, i * edgeLen, j * edgeLen, (i + 2) * edgeLen);
						}

					}
				}

				else { /*last row: only check horizontally*/
					if (visited[i][j + 1] == VISIT && j != WIDTH - 2) {
						if (visited[i][j + 2] == PATH) {
							(visited[i][j] == PATH) ? ofSetColor(0,255,0) : ofSetColor(255,0,0);
							ofDrawLine((j * edgeLen), i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
						}
						else if (visited[i][j + 2] == NO_PATH) {
							(visited[i][j] == PATH) ? ofSetColor(255,0,0) : ofSetColor(255,0,0);
							ofDrawLine((j * edgeLen), i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
						}

					}
				}
			}
		}
	}
}

bool ofApp::BFS() {
	//STEP 1: INITIALISE
	point cur = { 1, 1 };

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			backMaze[i][j] = { 0, 0 };
		}
	}
	backMaze[cur.y][cur.x] = { 0 ,0 };
	visited[cur.y][cur.x] = VISIT;
	qu.push(cur);

	while (!qu.empty()) {
		cur = qu.front();
		qu.pop();
		visited[cur.y][cur.x] = NO_PATH;

		// STEP 2: CHECK WHETHER TARGET, IF SO TRUE
		if (cur.x == WIDTH - 2 && cur.y == HEIGHT - 2) {
			cout << "BFS Mode: Found the path!" << endl;

			while (backMaze[cur.y][cur.x].x != 0 || backMaze[cur.y][cur.x].y != 0) {
				visited[cur.y][cur.x] = PATH;
				cur = { backMaze[cur.y][cur.x].y, backMaze[cur.y][cur.x].x };
			}
			visited[1][1] = PATH;

			return true;
		}

		// STEP 3: Q.TOP HAS AN UNVISITIED ADJACNET NODE
		for (int i = 0; i < 4; i++) {
			if (visited[cur.y + dir[i].y][cur.x + dir[i].x] == CAN_MOVE) {
				point next = { cur.y + dir[i].y, cur.x + dir[i].x };
				visited[next.y][next.x] = VISIT;

				/*update cur position*/
				if (i == 0 || i == 2) next = { cur.y + dir[i].y * 2, cur.x + dir[i].x };
				else if (i == 1 || i == 3) next = { cur.y + dir[i].y, cur.x + dir[i].x * 2 };

				/*set next.parent = cur*/
				backMaze[next.y][next.x].y = cur.y;
				backMaze[next.y][next.x].x = cur.x;
				qu.push(next);

				//cout << "Next Position Push y : " << next.y << " x : " << next.x << endl;
			}
		}
	}
	return false;
}

void ofApp::bfsdraw() {
	for (int i = 1; i < HEIGHT - 1; i += 2) {
		for (int j = 1; j < WIDTH - 1; j += 2) {
			if (visited[i][j] == PATH || visited[i][j] == NO_PATH) {
				if (i != HEIGHT - 1) {
					//check horizontally
					if (visited[i][j + 1] == VISIT && j != WIDTH - 2) {
						if (visited[i][j + 2] == PATH) {
							(visited[i][j] == PATH) ? ofSetColor(0,0,255) : ofSetColor(255,0,0);
							ofDrawLine((j * edgeLen), i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
						}
						else if (visited[i][j + 2] == NO_PATH) {
							(visited[i][j] == PATH) ? ofSetColor(255, 0, 0) : ofSetColor(255, 0, 0);
							ofDrawLine((j * edgeLen), i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
						}
					}
					//check vertically
					if (visited[i + 1][j] == VISIT) {
						if (visited[i + 2][j] == PATH) {
							(visited[i][j] == PATH) ? ofSetColor(0,0,255) : ofSetColor(255, 0, 0);
							ofDrawLine((j * edgeLen), i * edgeLen, j * edgeLen, (i + 2) * edgeLen);
						}
						else if (visited[i + 2][j] == NO_PATH) {
							(visited[i][j] == PATH) ? ofSetColor(255, 0, 0) : ofSetColor(255, 0, 0);
							ofDrawLine((j * edgeLen), i * edgeLen, j * edgeLen, (i + 2) * edgeLen);
						}

					}
				}

				else { /*last row: only check horizontally*/
					if (visited[i][j + 1] == VISIT && j != WIDTH - 2) {
						if (visited[i][j + 2] == PATH) {
							(visited[i][j] == PATH) ? ofSetColor(0,0,255) : ofSetColor(255, 0, 0);
							ofDrawLine((j * edgeLen), i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
						}
						else if (visited[i][j + 2] == NO_PATH) {
							(visited[i][j] == PATH) ? ofSetColor(255, 0, 0) : ofSetColor(0,0,255);
							ofDrawLine((j * edgeLen), i * edgeLen, (j + 2) * edgeLen, i * edgeLen);
						}

					}
				}
			}
		}
	}
}

void ofApp::mazeInit() {
	while (!qu.empty()) qu.pop();
	while (!stck.empty()) stck.pop();
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			if (visited[i][j] != WALL) visited[i][j] = CAN_MOVE;
			backMaze[i][j] = { 0,0 };
		}
	}
}