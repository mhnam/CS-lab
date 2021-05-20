#pragma once

#include "ofMain.h"
constexpr auto EPSILON = 0.5;;

class waterdrop {
public:

	//constructor
	waterdrop();

	//variables
	int x;
	int y;
	int fl;
};

class lineseg {
public:

	//constructor
	lineseg();

	//variables
	int x1;
	int x2;
	int y1;
	int y2;
};

class dot {
public:

	//constructor
	dot();

	//variables
	int x;
	int y;
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	/* WaterFall-related member variables Regions */

	// flag variables
	int draw_flag;
	int load_flag;
	int water_flag;

	// Line segment and dot related variables
	int num_of_line, num_of_dot, num_of_drop;
	float dot_diameter;

	/* WaterFall-related member functions */
	int red_dot_index;
	lineseg* linearray;
	dot* dotarray;
	waterdrop* droparray;

	void processOpenFileSelection(ofFileDialogResult openFileResult);
	void initializeWaterLines(); // 2nd week portion.

};