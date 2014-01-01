//
//  ofxPhilipsHue.cpp
//  ofxHttpForm example
//
//  Created by Oriol Ferrer Mesià on 28/12/13.
//
//

#include "ofxPhilipsHue.h"

ofxPhilipsHue::ofxPhilipsHue(){
	bridge = "";
	apiUser = "";
}

void ofxPhilipsHue::setup(string bridgeIP, string userName){

	bridge = bridgeIP;
	apiUser = userName;
}

void ofxPhilipsHue::setLightState(int lightID, bool state, ofColor c, int transitionDuration){

	float h = c.getHue() / 255.;
	float s = c.getSaturation() / 255.;
	float b = c.getBrightness() / 255.;

	setLightState( lightID, state, b, s, h, transitionDuration );
}


void ofxPhilipsHue::setLightState(int lightID, bool state, float brightness, float saturation, float hue, int transitionDuration){

	//build json data
	string json = "{ \"on\":" + string( state ? "true" : "false");

	if (brightness >= 0.0 && brightness <= 1.0){
		json += string(" , ") + "\"bri\":" + ofToString((int)ofMap(brightness, 0, 1, 0, 255, true));
	}

	if (saturation >= 0.0 && saturation <= 1.0){
		json += string(" , ") + "\"sat\":" + ofToString((int)ofMap(saturation, 0, 1, 0, 255, true));
	}

	if (hue >= 0.0 && hue <= 1.0){
		json += string(" , ") + "\"hue\":" + ofToString((int)ofMap(hue, 0, 1, 0, 65535, true));
	}

	json += string(" , ") + "\"transitiontime\":" + ofToString( int(transitionDuration / 100) );

	json += " }";

	//cout << json << endl;

	sendCommand(lightID, json);

}

void ofxPhilipsHue::blinkLightOnce(int lightID){

	//build json data
	string json = "{ \"on\":true, \"alert\":\"select\" }";
	//cout << json << endl;

	sendCommand(lightID, json);
}

bool ofxPhilipsHue::sendCommand(int lightID, string json){

	if (bridge.length() == 0 || apiUser.length() == 0){
		cout << "ofxPhilipsHue::setLightState(); Can't set Light State! You need to setup first!" << endl;
		return;
	}

	float timeOut = 1.0; //seconds
	Poco::URI uri = Poco::URI( "http://" + bridge + "/api/" + apiUser +"/lights/" + ofToString(lightID) + "/state" );
	std::string path(uri.getPathAndQuery());
	if (path.empty()) path = "/";
	string host = uri.getHost();


	try{

		HTTPClientSession session( host, uri.getPort() );
		HTTPRequest req(HTTPRequest::HTTP_PUT, path, HTTPMessage::HTTP_1_1);

		session.setTimeout( Poco::Timespan(timeOut,0) );
		req.set( "User-Agent", "ofxPhilipsHue");
		req.setContentLength( json.length() );

		std::ostream& os = session.sendRequest(req);
		std::istringstream is( json );
		Poco::StreamCopier::copyStream(is, os);
		req.setContentLength( json.length() );

		Poco::Net::HTTPResponse res;
		istream& rs = session.receiveResponse(res);
		string responseBody = "";
		StreamCopier::copyToString(rs, responseBody);	//copy the data...
		cout << "ofxPhilipsHue >> Response : " << responseBody << endl << endl;
		return true;
	}catch(Exception& exc){
		ofLog( OF_LOG_ERROR, "ofxPhilipsHue::sendCommand(%s) to %s >> Exception: %s\n", json.c_str(), uri.toString().c_str(), exc.displayText().c_str() );
		return false;
	}

}