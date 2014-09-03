//
//  constants.h
//  howTable_Base
//
//  Created by Adam Lassy on 8/7/14.
//
//

#ifndef howTable_Base_constants_h
#define howTable_Base_constants_h

#include "ofMain.h"

//STATES
const int STATE_WAIT = 0;
const int STATE_FACT_1 = 1;
const int STATE_FACT_2 = 2;
const int STATE_FACT_3 = 3;
const int STATE_FACT_4 = 4;
const int STATE_FACT_5 = 5;
const int STATE_FACT_6 = 6;
const int STATE_FACT_7 = 7;
const int STATE_FINAL = 8;

const int CUSTOMER_STATE_RFID_PROMPT = 0;
const int CUSTOMER_STATE_WELCOME_MSG = 1;
const int CUSTOMER_STATE_FACT = 2;
const int CUSTOMER_STATE_ACTION_PROMPT = 3;


//CANVAS SIZE PROPERTIES
const float SCALE = .35;
const int NUM_SCREENS = 2;
const int REAL_CANVAS_WIDTH = 1920*SCALE;
const int REAL_CANVAS_HEIGHT = 1080*NUM_SCREENS*SCALE;

const int NUM_CUSTOMERS = 4;

//MESSAGES FROM SERIAL
const int MSG_RFID = 0;
const int MSG_GLASS_OFF = 1;
const int MSG_GLASS_ON = 2;
const int MSG_BUTTON_ON = 3;

const int ACTION_TWITTER = 0;
const int ACTION_FACEBOOK = 1;
const int ACTION_EMAIL = 2;

//API ACTIONS
const int API_ACTION_RFID = 3;
const int API_ACTION_TWITTER = 0;
const int API_ACTION_FACEBOOK = 1;
const int API_ACTION_EMAIL = 2;

//API
const string API_DEST = "http://lab.madsci1.havasworldwide.com/jwtable/";
const string API_GET_RFID = "/getCustomer";
const string API_PUT_FACEBOOK = "/doFacebook";
const string API_PUT_TWITTER = "/doTwitter";
const string API_PUT_EMAIL = "/doEmail";

//Enumerate the 3 actions to simplify
const string ACTION_NAMES[3] = {"Twitter", "Facebook", "Email"};
const string API_ACTION_VALUES[3] = {API_PUT_TWITTER, API_PUT_FACEBOOK, API_PUT_EMAIL};

#endif
