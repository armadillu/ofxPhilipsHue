#include "testApp.h"

bool state = false; //on / off
int lightID = 3; //see hue API docs for this

void testApp::setup(){

	ofBackground(22);
	ofSetFrameRate(2); //ghetto limit for the api call rate

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

	float newBrightness = ofMap(mouseY, 0, ofGetHeight(), 1, 0, true);
	float newHue = ofMap(mouseX, 0, ofGetWidth(), 0, 1, true);

	hue.setLightState(
						lightID,		//light bulb ID #
						true,			//on-off
						newBrightness,	//brightness
						1.0,			//sat
						newHue,			//hue
						300				//transition duration in ms
					  );
}
