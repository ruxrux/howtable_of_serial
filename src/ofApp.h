#pragma once

#include "ofMain.h"
#include "customer.h"
#include "ofxSimpleSerial.h"
#include "ofxHapPlayer.h"
#include "ofxOsc.h"

#define PORT 12345

class SerialMessage
{
public:

    SerialMessage(): fade(0)
    {
    }
    
    SerialMessage(const std::string& _message,
                  const std::string& _exception,
                  int _fade):
    message(_message),
    exception(_exception),
    fade(_fade)
    {
    }
    
    std::string message;
    std::string exception;
    int fade;
};

class ofApp : public ofBaseApp{
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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void updateDebugAnimation();
        void renderDebugAnimation();
    
        void advanceState();
        void initState();
    
        void onNewMessage(string & message);
        void urlResponse(ofHttpResponse &httpResponse);
    
        void renderBackground();

        customer arrCustomers[NUM_CUSTOMERS];
    
        ofxSimpleSerial	serial;
        //string		message;
        int currentState;
    
        //Debugging
        bool debug = true;
        float rotateAngle = 0;
        ofTrueTypeFont font32;
    
    // CONSUMER FACT VIDEO TEST
    
    ofxHapPlayer cVideoTest[4];
    bool bTestPlay;
    
    // VIDEO SYNC
    
        void videoSyncUpdate();
    
        ofxHapPlayer videoPlayer;
    
        ofxOscReceiver client;
    
        int syncPOSITION;
        bool bPLAY;
    
        ofTrueTypeFont font250;
        bool bINFO;
        bool bFULLSCREEN;


};
