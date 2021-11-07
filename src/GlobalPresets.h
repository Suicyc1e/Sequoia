#ifndef GLOBAL_PRESETS_H
#define GLOBAL_PRESETS_H


#define UART_COMMAND_LINE_TASK_DELAY_MS 500
#define PUSH_BUTTON_READER_TASK_DELAY_MS 500

#define ACCESS_POINT_PREFIX "SEQUOIA_AP_"
#define ACCESS_POINT_DEFAULT_PASSWORD "sequoia123"

#define WIFI_CONNECT_ATTEMPT_TIME_S 15

//Comment it to setup coorect Network Configuring procedure (with access-point definition).
//#define DEBUG_SKIP_WELCOME_AP

//This option disables Device's Startup and any Hardware config but WiFi to test and debug REST communication
//WEB-INTERFACE is DISABLED this way!
//#define DEBUG_MOBILE_APPLICATION_DEVELOPMENT



#define BUILD_FOR_DOOR_BELL
//#define BUILD_FOR_PLANT_CARER
//#define BUILD_FOR_PET_FEEDER


//CHOOSE ONLY ONE!
#define ALEX_WIFI


#define LED_TOGGLE_INTERVAL_MS 250 

#define BUTTON_DEBOUNCE_INTERVAL_MS 350

#define LED_BLINKING_TIME_DURING_TEST_S 5

#define FEED_PROCESS_CLAMP_MS 100
#define FEED_PROCESS_DELAY_MS 500

#define FEED_TOLERANCE_GRAMMS 1

#define FEED_PROCESS_MAXIMUM_NUMBER_OF_ITERATIONS 50

#define FIRE_POLL_RATE_MS 3000

#define FIRE_DETECTION_THRESHOLD 100 //LESS-> FIRE IS ON!!!!

#endif