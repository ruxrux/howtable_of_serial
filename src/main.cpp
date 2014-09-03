//#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"


//========================================================================
int main( ){

    ofAppGLFWWindow win;
    
	//win.setNumSamples(8);
	win.setMultiDisplayFullscreen(true); //this makes the fullscreen window span across all your monitors
    //ofAppBaseWindow window;
	ofSetupOpenGL(&win, 1920,2160, OF_WINDOW);
	//ofSetupOpenGL(1920,2160, OF_WINDOW);			// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
