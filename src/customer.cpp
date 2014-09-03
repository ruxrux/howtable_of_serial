//
//  customer.cpp
//  howTable_Base
//
//  Created by Adam Lassy on 8/7/14.
//
//

#include "customer.h"

// --------------------------------------
customer::customer()
{
    customer(0);
}

customer::customer(int _position)
{
    // in setup:
    defaultFont.loadFont("verdana.ttf", 32);
    verdana10Font.loadFont("verdana.ttf", 10);
    verdana12Font.loadFont("verdana.ttf", 10);

    tablePosition = _position;
    
    //Initialize origin point of each customer. For now done quickly and programatically but prob eventually done more explititly
    int columns = NUM_CUSTOMERS/2;
    if (_position < NUM_CUSTOMERS/2) {
        displayOrigin.y = 25;
        displayOrigin.x = _position * REAL_CANVAS_WIDTH/columns + 50;
    } else {
        displayOrigin.y = REAL_CANVAS_HEIGHT - 125;
        displayOrigin.x = (_position - columns) * REAL_CANVAS_WIDTH/columns + 50;
    }
    
    //INIT CUSTOMER STATE
    currentCustomerState = CUSTOMER_STATE_RFID_PROMPT;
}

void customer::update(){
    
    
}


// --------------------------------------
void customer::render(){
    
    
    ofPushMatrix();
    ofTranslate(displayOrigin);
    
    if (debug) {

        renderDebugView();
    } else {

        switch (currentCustomerState) {
            
            case CUSTOMER_STATE_RFID_PROMPT:
                verdana12Font.drawString("HELLO. SET YOUR CARD DOWN",0,0);
                break;
            case CUSTOMER_STATE_WELCOME_MSG:
                verdana12Font.drawString("WELCOME " + name + ". ENJOY YOUR BOOZE.",0,0);
                break;
            case CUSTOMER_STATE_FACT:
                verdana12Font.drawString("This is the relevant fact for state " + ofToString(parentState),0,0);
                break;
            case CUSTOMER_STATE_ACTION_PROMPT:
                verdana12Font.drawString("This is an action prompt for state " + ofToString(parentState),0,0);
                break;
        }
    }
    ofPopMatrix();
}

/*********************
 
 WHEN TABLE STATE ADVANCES, WE UPDATE THE CUSTOMER STATE AND CLEAR APPROPRATE VALUES
 
 ********************/
void customer::updateCustomerState(int _state)
{
    parentState = _state;
    
    //If we have Initialized the customer
    if (initRfid)
    {
        //When we begin each state, we clear the state actions
        clearStateActions();
        
        switch (parentState) {
            
            case STATE_FACT_1:
            case STATE_FACT_2:
                
                currentCustomerState = CUSTOMER_STATE_FACT;
                break;
            
            case STATE_FACT_3:
            case STATE_FACT_4:
            case STATE_FACT_5:
            case STATE_FACT_6:
            case STATE_FACT_7:

                currentCustomerState = CUSTOMER_STATE_ACTION_PROMPT;
                break;
        }
    }
}

void customer::clearStateActions()
{
    stateAttributeGlass[0] = false;
    stateAttributeGlass[1] = false;
    
    stateAttributeButtonPress[ACTION_TWITTER] = false;
    stateAttributeButtonPress[ACTION_FACEBOOK] = false;
    stateAttributeButtonPress[ACTION_EMAIL] = false;
    
    sActionString = "";

}

/*******************
 
 SERIAL MESSAGES FROM HARDWARE
 
 *************************/
void customer::handleSerialMessage(int _command, string _valString, int _valInt)
{
    
    switch (_command) {
        
    case MSG_RFID:
        
        rfid = _valString;
        apiSend(API_DEST + rfid + API_GET_RFID, API_ACTION_RFID);
        break;
        
    case MSG_GLASS_OFF:
        
        glassOn[_valInt] = false;
        if (currentCustomerState == CUSTOMER_STATE_ACTION_PROMPT)
        {
            stateAttributeGlass[_valInt] = true;
            
            switch (parentState) {
                
                case STATE_FACT_3:
                case STATE_FACT_4:
                case STATE_FACT_5:
                    
                    //If they picked up correct glass. For water and Ice states we will probably just have
                    // a timer here instead because they have already picked up the glass
                    if (stateAttributeGlass[0] == true) {
                        
                        //Advance customer state to the Fact prompt
                        currentCustomerState = CUSTOMER_STATE_FACT;
                        sActionString = "Picked up Glass 1";
                    }
                    break;
                    
                case STATE_FACT_6:
                    
                    //Picking up glass 2
                    if (stateAttributeGlass[1] == true) {
                        
                        currentCustomerState = CUSTOMER_STATE_FACT;
                        sActionString = "Picked up Glass 2";
                    }
                    break;

                case STATE_FACT_7:
                    
                    //Picking up both glasses
                    if (stateAttributeGlass[0] == true && stateAttributeGlass[1] == true) {
                        
                        currentCustomerState = CUSTOMER_STATE_FACT;
                        sActionString = "Picked up Glass 1 & Glass 2";
                    }
                    break;
            }
        }
        break;
        
    case MSG_GLASS_ON:
        
        //For now we ignor glass on actions
        glassOn[_valInt] = true;
        break;
        
    case MSG_BUTTON_ON:
        
        //If the customer state is a State Fact we can do some social shit
        if (currentCustomerState == CUSTOMER_STATE_FACT && !stateAttributeButtonPress[_valInt])
        {
            
            //************
            // Swith for button type
            //************
            apiSend(API_DEST + rfid + API_ACTION_VALUES[_valInt], _valInt);
            lastButton = ACTION_NAMES[ACTION_TWITTER];

            /*
            switch (_valInt) {
                
                case ACTION_TWITTER:
                
                    apiSend(API_DEST + rfid + API_PUT_TWITTER, API_ACTION_TWITTER);
                    lastButton = ACTION_NAMES[ACTION_TWITTER];
                    break;
                
                case ACTION_FACEBOOK:
                
                    apiSend(API_DEST + rfid + API_PUT_FACEBOOK, API_ACTION_FACEBOOK);
                    lastButton = "Facebook";
                    break;
                    
                case ACTION_EMAIL:
                
                    apiSend(API_DEST + rfid + API_PUT_EMAIL, API_ACTION_EMAIL);
                    lastButton = "Email";

                    break;
            }
            */
            
            //Set the button press to true for this button
            stateAttributeButtonPress[_valInt] = true;
            sActionString = "Press " + ACTION_NAMES[ACTION_TWITTER] + " Button";
            
        }
        break;

    }
}


/*************************
 
 API FUNCTIONS TO CUSTOMER DB
 
 **************************/
void customer::apiSend(string _url, int _type)
{
    ofLog() << _url;
    int _id;
    
    //Debug values
    switch (_type) {
        case API_ACTION_RFID:
            apiStatus = "RFID send ..."; break;
        case API_ACTION_TWITTER:
            apiStatus = "Twitter send ..."; break;
        case API_ACTION_FACEBOOK:
            apiStatus = "Facebook send ..."; break;
        case API_ACTION_EMAIL:
            apiStatus = "Email send ..."; break;
    }
    
    _id = ofLoadURLAsync(_url, ofToString(tablePosition));
}

void customer::handleApiResponse(string _body)
{
    ofxJSONElement jsonResult;

    jsonResult.parse(_body);
    
    //Debug values
    ofLog() << "TYPE " << jsonResult[0]["type"];
    
    switch (jsonResult[0]["type"].asInt()) {
            
        case API_ACTION_RFID:
            
            //Initialize user
            initRfid = true;
            apiStatus = "RFID done";
            name = jsonResult[0]["name"].asString();
            currentCustomerState = CUSTOMER_STATE_WELCOME_MSG;
            break;
        case API_ACTION_TWITTER:
            apiStatus = "Twitter done";
            break;
        case API_ACTION_FACEBOOK:
            apiStatus = "Facebook done";
            break;
        case API_ACTION_EMAIL:
            apiStatus = "Email done";
            break;
            
    }
    ofLog() << jsonResult[0]["status"].asString();
}


/*************************
 
 RENDERING FUNCTIONS
 
 **************************/
 
//----------------------------------------
// Render the debugging views
//
void customer::renderDebugGlass(int index)
{
    if (glassOn[index]) {
        ofFill();
    } else {
        ofNoFill();
    }

    ofRect(125,index*30,25,25);
    verdana12Font.drawString("Glass " + ofToString(index),155,index*30+15);
}

void customer::renderDebugView()
{
    ofNoFill();
    ofRect(0,0,100,100);
    
    for (int i=0; i<2; i++) {
        renderDebugGlass(i);
    }
    
    // in draw:
    defaultFont.drawString(ofToString(tablePosition),40,60);
    verdana12Font.drawString("Last button pressed: " + lastButton,155,75);
    verdana12Font.drawString("API Status: " + apiStatus,155,90);
    verdana12Font.drawString("Name: " + name,155,105);
}
