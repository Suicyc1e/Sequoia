#ifndef ZIGBEE_COMMON_H
#define ZIGBEE_COMMON_H

#include <Arduino.h>

enum class ZigBeeAttributeMapperDoorBell : uint8_t
{
    AUTO_ANSWER = 0,
    SOUND_DETECT = 1,
    PHOTO_SAVING = 2,
    SOUND_SAVING = 3,
    BUTTON_TRIGGERED = 4,
    MICROPHONE_TRIGGERED = 5,
    BOX_VIBRATION_TRIGGERED = 6,
    ADDITIONAL_MAPPER_STATE = 7,
    WEB_INTERFACE = 8, // "Virtual Attribute, non-existing on ZigBee"
};

bool IsValidDoorBellAttribute(uint8_t value);

enum class AttributeChangeSource
{
    SYSTEM_INTERNAL, //Web-GUI, Sensors, Buttons... 
    ZIGBEE_SIDE, //Reported from ZigBee-Side
};


#endif