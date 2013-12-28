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

void ofxPhilipsHue::setLightState(int lightID, bool state, float brightness, float saturation, float hue, int transitionDuration){

	if (bridge.length() == 0 || apiUser.length() == 0){
		cout << "you need to setup first!" << endl;
		return;
	}

	float timeOut = 3.0; //seconds
	Poco::URI uri = Poco::URI( "http://" + bridge + "/api/" + apiUser +"/lights/" + ofToString(lightID) + "/state" );

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


	cout << json << endl;

	try{
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";
		string host = uri.getHost();

		cout << "http://" + host << "/" << path << endl;
		cout << json << endl << endl;

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
		cout << "response : " << responseBody << endl;

	}catch(Exception& exc){
		ofLog( OF_LOG_ERROR, "HttpFormManager::executeForm(%s) >> Exception: %s\n", uri.toString().c_str(), exc.displayText().c_str() );
	}
}