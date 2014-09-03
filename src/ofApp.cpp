#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofSetWindowPosition(0, 0);
    ofSetFrameRate(30);

    ofEnableAlphaBlending();
    
    //Initialize place settings
    for (int i=0; i<NUM_CUSTOMERS; i++) {
        arrCustomers[i] = customer(i);
    }
    
    //Set up serial
    serial.setup(0, 9600);
    
	serial.startContinuousRead(false);
	ofAddListener(serial.NEW_MESSAGE,this,&ofApp::onNewMessage);
	
    //Register the curl async response
    ofRegisterURLNotification(this);
    
    //Init the state
    initState();
    
    font32.loadFont("Lato-Light.ttf", 32);
    
    // - - - - - - - - - - - - - - - - VIDEO SYNC! - - - [ SETUP ]- - - - - - - -
   
    
    client.setup(PORT);
    
    videoPlayer.loadMovie("testHAP.mov");
    font250.loadFont("Lato-Light.ttf", 250);
    
    bFULLSCREEN = false;
    bINFO = true;
    bPLAY = false;

    // - - - - - - - - - - - - - - - - -  FACT VIDEO TEST  - - - - - - - - - - - -
    for (int i = 0; i <4; i++)     cVideoTest[i].loadMovie("txtTrayAlpha.mov");
    bTestPlay = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){

    updateDebugAnimation();
    
    videoSyncUpdate();
    
    // - - - - - - - - - - - - - - - - -  FACT VIDEO TEST  - - - - - - - - - - - -
    for (int i = 0; i <4; i++)     cVideoTest[i].update();
    if (cVideoTest[0].isPlaying() == false && bTestPlay){
        
        for (int i = 0; i <4; i++)     cVideoTest[i].play();
        
    } else if (cVideoTest[0].isPlaying() == true && bTestPlay) {
        for (int i = 0; i <4; i++){
            cVideoTest[i].stop();
            cVideoTest[i].setFrame(0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // - - - - - - - - - - - - - - - - VIDEO SYNC! - - - [ DRAW ]- - - - - - - -

    ofSetColor(255);
    videoPlayer.draw(0,0, 1920, 2160);
    
    if (bINFO) {
        string num = ofToString(syncPOSITION) + "\n" + ofToString(ofGetFrameRate(),1);
        font250.drawString(num, (ofGetWidth()-font250.stringWidth(num))*.5, (ofGetHeight()-font250.stringHeight(num))*.5);
        
        ofDrawBitmapString(ofToString(syncPOSITION, 5), 10,30);
        ofDrawBitmapString(ofToString(bPLAY), 10,10);
 

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    //Render background
    renderBackground();
    
    //Set line and color stuff
    ofNoFill();
    ofSetColor( 255, 255, 0 );

    //for each customer render their display space. For now just their number.
    for (int i=0; i<NUM_CUSTOMERS; i++) {

        arrCustomers[i].render();
    }

    //Render debug animation
    renderDebugAnimation();
   }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    ofSetColor(255,255,255);
    
    if (bTestPlay) {
        
        cVideoTest[0].draw(0, 0);
        cVideoTest[1].draw(ofGetWidth() - cVideoTest[0].getWidth(),0);
        cVideoTest[2].draw(0, 1080);
        cVideoTest[3].draw(ofGetWidth() - cVideoTest[0].getWidth(), 1080);
        
    }
    // - - - - - - - - - - - - - - - - -  FACT VIDEO TEST  - - - - - - - - - - - -
    
}

void ofApp::updateDebugAnimation()
{
    rotateAngle += 1;
    if (rotateAngle >= 360) {
        rotateAngle -= 360;
    }
}

void ofApp::renderBackground()
{
    font32.drawString(ofToString(currentState), REAL_CANVAS_WIDTH*.5, REAL_CANVAS_HEIGHT*.5);
}

void ofApp::renderDebugAnimation()
{
    ofNoFill();
    ofPushMatrix();
    ofTranslate(REAL_CANVAS_WIDTH/2, REAL_CANVAS_HEIGHT/2,2);
    ofRotate(rotateAngle,0.0,0.0,1.0);
    ofRect(-50,-50,100,100);
    ofPopMatrix();
}

//----------------------------------------------
// STATE LOGIC
//
//---------------------------------------------
void ofApp::initState()
{
    currentState = STATE_WAIT;
}

void ofApp::advanceState()
{
    currentState++;
    if (currentState > STATE_FINAL) {
        currentState = STATE_FINAL;
    }
    
    //UPDATE STATE OF EACH CUSTOMER
    //for each customer render their display space. For now just their number.
    for (int i=0; i<NUM_CUSTOMERS; i++) {
        
        arrCustomers[i].updateCustomerState(currentState);
    }
    
}

//---------------------------------------------------
// On Serial Message
//
//   Get incomming message and send to correct Customer
//
void ofApp::onNewMessage(string & message)
{
	cout << "onNewMessage, message: " << message << "\n";
	
    int _customerIndex;
    int _command;
    int _valInt;
    string _valString;

	vector<string> input = ofSplitString(message, ":");
	if(input.size() == 3)
	{
        _customerIndex = ofToInt(input.at(0));
        _command = ofToInt(input.at(1));
        _valString = ofToString(input.at(2));
        _valInt = ofToInt(input.at(2));

        arrCustomers[_customerIndex].handleSerialMessage(_command, _valString, _valInt);

	}
}

//----------------------------------------------------
// ON CURL response to JSON API
//
//   Get curl response and send to correct Customer
//
void ofApp::urlResponse(ofHttpResponse &response){

    int _customerIndex;
    
    
    ofLog() << "URL RESPONSE Name: " << response.request.name << " ID : " << ofToString(response.request.getID());

    if(response.status==200)
    {
    
        _customerIndex = ofToInt(response.request.name);
        
        arrCustomers[_customerIndex].handleApiResponse(response.data.getText());
    }
}

void ofApp::videoSyncUpdate(){
    videoPlayer.update();
    
    
    if  (!videoPlayer.isPlaying() && bPLAY) videoPlayer.play();
    if(bPLAY == 0) videoPlayer.stop();
    
    
    
    
    // check for waiting messages
	while(client.hasWaitingMessages()){
        
		ofxOscMessage m;
		client.getNextMessage(&m);
        
		if(m.getAddress() == "/movie/frame"){
            
            syncPOSITION = m.getArgAsInt32(0);
            
        } else if (m.getAddress() == "/movie/state"){
            
			bPLAY = m.getArgAsInt32(0);
            
        } else if (m.getAddress() == "/ctrl/fs"){
            
            ofSetFullscreen(m.getArgAsInt32(0));
            
        } else if (m.getAddress() == "/ctrl/info"){
            
            bINFO = m.getArgAsInt32(0);
            
        } else if (m.getAddress() == "/movie/testConsumerPlay"){
            
            bTestPlay = m.getArgAsInt32(0);
            
        } else {
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
            
		}
        
	}
    
    
    int diff = abs(videoPlayer.getCurrentFrame() - syncPOSITION);
    if (diff > 10) {
        videoPlayer.setFrame(syncPOSITION);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    switch(key) {
        case 'p':
            if (!videoPlayer.isPlaying()) videoPlayer.play();
            else videoPlayer.stop();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'i':
            bINFO =! bINFO;
            break;

        //Commands to control state
        case 's':
            advanceState();
            break;

        case 'o':
            initState();
            break;
        
        case 'd':
            if (debug) { debug = false;} else {debug = true;}
            break;
            
        //Other
        case 't':
            //List Serial Devices
            serial.listDevices();
            //vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
            break;
    }
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
