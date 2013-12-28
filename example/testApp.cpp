#include "testApp.h"

bool state = false; //on / off
int lightID = 3; //see hue API docs for this

void testApp::setup(){

	ofBackground(22);
	ofSetVerticalSync(true);
	ofSetFrameRate(10); //ghetto limit for the api call fps

	hue.setup("192.168.1.200", "2d5ff43c38ba7ecf1bc852143f75647");
}


void testApp::draw(){
	ofDrawBitmapString("mouseX controls hue\nmouseY controls brightness\nkeyPress toggles on/off", 20, 20);
}


void testApp::keyPressed(int key){
	state = !state;
	hue.setLightState(lightID, state);
}

void testApp::mouseMoved( int x, int y ){
	hue.setLightState(	lightID,
						true,											//on-off
						ofMap(mouseY, 0, ofGetHeight(), 1, 0, true),	//brightness
						1.0,											//sat
						ofMap(mouseX, 0, ofGetWidth(), 0, 1, true),		//hue
						0												//transition duration
					  );
}
