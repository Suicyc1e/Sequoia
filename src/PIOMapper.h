#ifndef PIO_MAPPER_H
#define PIO_MAPPER_H

#include <GlobalPresets.h>

///Corresponds to the PIO IDs which are mapped to the respective devices.
//Those commented ARE IN USE but defined by the FrameWork - like the PINS of UART2.
//All teh others are defined here and will be passed to the respective constructors.

//==========================================
//ZIGBEE
//Serial-2
#define UART_ZIGBEE_RX 17 //SERAIL2 TX
#define UART ZIGBEE_TX 16 //SERIAL2 RX
//Power ---> 3.3V
//GND
//==========================================


//==========================================
//SD-CARD ---> NEVER Forget about resistors!!!! (And test simplifying)
//
#define USER_SD_CARD_CHIP_SELECT 5 
#define USER_SD_CARD_MISO 19
#define USER_SD_CARD_MOSI 23
#define USER_SD_CARD_SCK 18
//Power ---> 5.0V
//GND
//==========================================


//==========================================
//SOUND
//
#define USER_I2S_DOUT 25
#define USER_I2S_BCLK 27
#define USER_I2S_LRC 26
//Power ---> 5.0V
//GND


//==========================================
//CAMERA_UART
//
#define USER_CAMERA_UART_RX 13
#define USER_CAMERA_UART_TX 22

//==========================================
//LED_PIO
//

//15 2 0 4 35 34

//Since  __analogRead(): GPIO14: ESP_ERR_TIMEOUT: ADC2 is in use by Wi-Fi.
//14--->34
//Sensors
#define USER_FIRE_SENSOR 34
#define USER_MOVEMENT_SENSOR 4
#define USER_VIBRATION_SENSOR 14

//!!!ADC_CHANNEL_7 ---> GPIO35! (Microphone) ADC_CHANNEL_6 ---> GPIO34???


//Scales and LEDs

#ifdef BUILD_FOR_DOOR_BELL
    #define USER_GREEN_LED_PIO 33
    #define USER_RED_LED_PIO 21
#endif

//DEBUG ONLY!!! (TILL PetFeeder is not Done!)
#ifdef BUILD_FOR_DOOR_BELL
    #define USER_SCALES_I2C_SDA 33
    #define USER_SCALES_I2C_SCL 32
#endif

#ifdef BUILD_FOR_PLANT_CARER
    #define USER_SCALES_I2C_SDA 2
    #define USER_SCALES_I2C_SCL 4
#endif


//END DEBUG

#ifdef BUILD_FOR_PET_FEEDER
    #define USER_GREEN_LED_PIO 21//21
    #define USER_RED_LED_PIO 4
    #define USER_SCALES_I2C_SDA 33
    #define USER_SCALES_I2C_SCL 32
#endif

#ifdef BUILD_FOR_PLANT_CARER
    #define USER_GREEN_LED_PIO 15
    #define USER_RED_LED_PIO 4
#endif

//==========================================
//BUTTON_PIO
//
#define USER_BUTTON_PIO 32

#define CLAMP_PIO 14//15



#ifdef BUILD_FOR_PLANT_CARER



    #define WATER_PUMP_PIO 13

    #define WATER_VALVE_1_PIO 14
    #define WATER_VALVE_2_PIO 27
    #define WATER_VALVE_3_PIO 26
    #define WATER_VALVE_4_PIO 25

    #define WATER_HUMIDITY_LEVEL_PIO 21

    #define WATER_LEVEL_LOW_PIO 32
    #define WATER_LEVEL_HIGH_PIO 33

    #define LED_LAMP_RELAY_PIO 22

#endif



#endif
