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



vector<ofxPhilipsHueLight> ofxPhilipsHue::getLights(){
	string responce = sendGetCommand("/lights");
	vector<ofxPhilipsHueLight> result;
	if(responce == "error"){
		
	}else{
		ofxJsonxx::Object jsonObj;
		ofxJsonxx::loadFromString(jsonObj, responce);
		
		for(auto & kv : jsonObj.kv_map()){
			ofxJsonxx::Object light = jsonObj.get<ofxJsonxx::Object>(kv.first);
			ofxJsonxx::Object state = light.get<ofxJsonxx::Object>("state");
			
			result.push_back((ofxPhilipsHueLight){
				ofToInt(kv.first),
				light.get<string>("name"),
				state.get<bool>("on"),
				ofMap(state.get<ofxJsonxx::Number>("bri"), 0, 255,   0, 1, true),
				ofMap(state.get<ofxJsonxx::Number>("hue"), 0, 65535, 0, 1, true),
				ofMap(state.get<ofxJsonxx::Number>("sat"), 0, 255,   0, 1, true)
			});

//			ofxPhilipsHueLight & l = result.back();
//
//			cout << l.id << ", " << l.name << ", " << l.on << ", " << l.brightness << ", " << l.hue << ", " << l.saturation << endl;
		}
	}
	
	return result;
	
}

vector<ofxPhilipsHueGroup> ofxPhilipsHue::getGroups(){
	string responce = sendGetCommand("/groups");
	vector<ofxPhilipsHueGroup> result;
	if(responce == "error"){
		
	}else{
		ofxJsonxx::Object jsonObj;
		ofxJsonxx::loadFromString(jsonObj, responce);
		
		for(auto & kv : jsonObj.kv_map()){
			ofxJsonxx::Object group  = jsonObj.get<ofxJsonxx::Object>(kv.first);
			ofxJsonxx::Object action = group.get<ofxJsonxx::Object>("action");
			ofxJsonxx::Array lights  = group.get<ofxJsonxx::Array>("lights");
			
			vector<int> lightsId;
			for (auto & l : lights.values()) {
				lightsId.push_back(ofToInt(l->get<ofxJsonxx::String>()));
			}
			
			result.push_back((ofxPhilipsHueGroup){
				ofToInt(kv.first),
				group.get<string>("name"),
				action.get<bool>("on"),
				ofMap(action.get<ofxJsonxx::Number>("bri"), 0, 255,   0, 1, true),
				ofMap(action.get<ofxJsonxx::Number>("hue"), 0, 65535, 0, 1, true),
				ofMap(action.get<ofxJsonxx::Number>("sat"), 0, 255,   0, 1, true),
				lightsId
			});
			
//			ofxPhilipsHueGroup & g = result.back();
//			
//			cout << g.id << ", " << g.name << ", " << g.on << ", " << g.brightness << ", " << g.hue << ", " << g.saturation << endl;
//			for (auto & i : g.lightsId) {
//				cout << i << ",";
//			}
//			cout << endl;
		}
	}
	
	return result;

}

void ofxPhilipsHue::setGroupState(int groupID, bool state, float brightness, float saturation, float hue, int transitionDuration){
	
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
	
	sendPutCommand("/groups/"+ofToString(groupID)+"/action", json);
	
}




bool ofxPhilipsHue::sendCommand(int lightID, string json){
	string responce = sendPutCommand("/lights/" + ofToString(lightID) + "/state", json);
	if(responce == "error"){
		return false;
	}else{
		return true;
	}
}


string ofxPhilipsHue::sendGetCommand(string uri, string json){
	return sendRequest(HTTPRequest::HTTP_GET, uri, json);
}
string ofxPhilipsHue::sendPutCommand(string uri, string json){
	return sendRequest(HTTPRequest::HTTP_PUT, uri, json);
}
string ofxPhilipsHue::sendPostCommand(string uri, string json){
	return sendRequest(HTTPRequest::HTTP_POST, uri, json);
}
string ofxPhilipsHue::sendDeleteCommand(string uri, string json){
	return sendRequest(HTTPRequest::HTTP_DELETE, uri, json);
}

string ofxPhilipsHue::sendRequest(string requestType, string uri, string json){
	if (bridge.length() == 0 || apiUser.length() == 0){
		ofLogError("ofxPhilipsHue") << "setLightState(); Can't set Light State! You need to setup first!";
		return;
	}
	float timeOut = 1.0; //seconds
	Poco::URI pocoUri = Poco::URI( "http://" + bridge + "/api/" + apiUser +uri );
	std::string path(pocoUri.getPathAndQuery());
	if (path.empty()) path = "/";
	
	
	try{
		
		HTTPClientSession session( pocoUri.getHost(), pocoUri.getPort() );
		HTTPRequest req(requestType, path, HTTPMessage::HTTP_1_1);
		
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
		return responseBody;
//		ofLogVerbose("ofxPhilipsHue") << "Response : " << responseBody;
//		return true;
	}catch(Exception& exc){
		ofLog( OF_LOG_ERROR, "ofxPhilipsHue::sendCommand(%s) to %s >> Exception: %s\n", json.c_str(), pocoUri.toString().c_str(), exc.displayText().c_str() );
		return "error";
	}
}