#include "testApp.h"

bool state = false; //on / off
int numLights = 3;

void testApp::setup(){

	lastBrightness = 0;
	ofBackground(22);
	ofSetFrameRate(60); //ghetto limit for the api call rate
	ofSetWindowTitle("ofxPhilipsHue");

	hue.setup("192.168.1.200", "2d5ff43c38ba7ecf1bc852143f75647");
}


void testApp::draw(){

	//background
	ofColor c = ofColor::white;
	c.setHsb(255 * lastHue, 255, 255 * lastBrightness);
	ofSetColor(c);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());

	//current color
	c.setHsb(
			 255 * ofMap(mouseX, 0, ofGetWidth(), 0, 1, true),
			 255,
			 255 * ofMap(mouseY, 0, ofGetHeight(), 1, 0, true)
			 );
	ofSetColor(c);
	ofCircle(mouseX, mouseY, 12);

	ofSetColor(255);
	ofDrawBitmapString("click to set new light values\nmouseX controls hue\n"
					   "mouseY controls brightness\nkeyPress toggles on/off\n\n"
					   "lastHue: " + ofToString(lastHue) + "\n"
					   "lastBrightness: " + ofToString(lastBrightness) + "\n"
					   "lastState: " + string(state ? "on" : "off"),
					   20, 20);
}


void testApp::keyPressed(int key){
	state = !state;
	for(int i = 1; i <= numLights; i++){
		hue.setLightState(i, state);
	}

}

void testApp::mousePressed( int x, int y, int button ){

	lastBrightness = ofMap(mouseY, 0, ofGetHeight(), 1, 0, true);
	lastHue = ofMap(mouseX, 0, ofGetWidth(), 0, 1, true);

	for(int i = 1; i <= numLights; i++){
		hue.setLightState(
						i,		//light bulb ID #
						true,			//on-off
						lastBrightness,	//brightness
						1.0,			//sat
						lastHue,		//hue
						300				//transition duration in ms
					  );
	}
}
