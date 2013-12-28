#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxPhilipsHue.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update(){};
		void draw();

		void keyPressed(int key);
		void mousePressed( int x, int y, int button );

		ofxPhilipsHue hue;
		float lastHue, lastBrightness;
};

#endif
