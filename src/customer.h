//
//  customer.h
//  howTable_Base
//
//  Created by Adam Lassy on 8/7/14.
//
//

#ifndef __howTable_Base__customer__
#define __howTable_Base__customer__

#include <iostream>

#include "constants.h"
#include "ofxJSON.h"

class customer {
    
public:
    
    customer();
    customer(int _tablePosition);

    void render();
    void update();
    void renderDebugView();
    void renderDebugGlass(int _i);
    void initUser(string _rfid);

    //API functions
    void handleApiResponse(string _body);
    void apiSend(string _url, int _type);

    //Handle Serial from controller
    void handleSerialMessage(int _command, string _valString, int _valInt);
    
    //UPDATE CUSTOMER STATE
    void updateCustomerState(int _state);
    void clearStateActions();

    bool initRfid;
    int tablePosition;
    ofPoint displayOrigin;
    bool displayOrientation;
    
    bool glassOn[2];
    
    string name;
    string rfid;
    
    //Debug strings
    string lastButton;
    string apiStatus;
    
    ofTrueTypeFont defaultFont;
    ofTrueTypeFont verdana10Font;
    ofTrueTypeFont verdana12Font;

    bool debug = false;
    
    //CUSTOMER STATE VALUES
    int currentCustomerState = 0;
    int parentState = 0;

    bool stateAttributeGlass[2] = {false, false};
    bool stateAttributeButtonPress[3] = {false, false, false};
    
    string sActionString;
    
    /*
    bool stateAttributeGlass1 = false;
    bool stateAttributeGlass2 = false;
    bool stateAttributeFacebookPress = false;
    bool stateAttributeEmailPress = false;
    bool stateAttributeTwitterPress = false;
     */

    
};

#endif /* defined(__howTable_Base__customer__) */
