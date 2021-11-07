#include <ZigBeeCommon.h>

bool IsValidDoorBellAttribute(uint8_t value)
{
    return (value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::AUTO_ANSWER) ||
            value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::SOUND_DETECT) ||
            value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::PHOTO_SAVING) ||
            value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::SOUND_SAVING) ||
            value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::BUTTON_TRIGGERED) ||
            value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::MICROPHONE_TRIGGERED) ||
            value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::BOX_VIBRATION_TRIGGERED) ||
            value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::ADDITIONAL_MAPPER_STATE) ||
            value == static_cast<uint8_t>(ZigBeeAttributeMapperDoorBell::WEB_INTERFACE));
}