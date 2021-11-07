#ifndef ZIGBEE_COMMANDS_H
#define ZIGBEE_COMMANDS_H

#include "WString.h"

//Can't using std::vector, since we are oblidged to use ArduinoFramework...
//Not Orange PI I can say)
//#include <vector>

// enum class ZigBeeCommandType
// {
//     Request = 0,
//     Response = 1
// };


//ZigBee Commands are issued for all the devices. Not only the specific ones.
//Later refcator it by registering command-handlers for specific devices.


enum class ZigBeeCommandFrameWork
{
    EmberZNET,
    NotDefined
};


enum class ZigBeeCommandType
{
    CONNECT_TO_NETWORK,
    SET_AUTO_ANSWER,
    SET_SOUND_DETECT,
    SET_PHOTO_SAVING,
    SET_SOUND_SAVING,
    SET_BUTTON_TRIGGERED,
    SET_MICROPHONE_TRIGGERED,
    SET_BOX_VIBRATION_TRIGGERED,
    SET_ADDITIONAL_MAPPER_STATE,

    SET_PLANT_CARE_DEVICE_ENABLE,
    SET_FUNCTIONAL_MODE,
    SET_WATER_VALVES_MAPPER,
    SET_SOIL_HUMIDITY_MAPPER,
    SET_LOWER_PLANT_MAPPER,
    SET_MIDDLE_PLANT_MAPPER,
    SET_UPPER_PLANT_MAPPER,
    SET_NO_WATER_ALARM

};

//To simplify - all commands within all Sequoia's Projects are custom commands. 
struct IZigBeeCommand
{
    //This prefix is defined by Ember ZNet CLI Syntax and should be allways used with custom commands
    virtual String GetHumanReadableCommand() const = 0;
    virtual String GetCommand() const = 0;

    // protected:
    //     ZigBeeCommandDefinition definition;
    //     String prefix;

};


class ZigBeeCommandBase : public virtual IZigBeeCommand
{   
protected:
    
    ZigBeeCommandType _commandType;
    const String _commandDescription;
    const String _commandSyntax;
    const String _commandPrefix;
    const String _commandBody;
    String _resultCommand;

public:

    ZigBeeCommandBase(ZigBeeCommandType type, String description, String syntax, String prefix, String body) :
        _commandType(type),
        _commandDescription(description),
        _commandSyntax(syntax), 
        _commandPrefix(prefix),
        _commandBody(body)
    { };
  
    virtual String GetHumanReadableCommand() const override final
    {
        return "Description: " + _commandDescription + " Syntax: " + _commandSyntax;
    }

    virtual String GetCommand() const = 0;
};

// Actually, all these parameters might be used while sending 'serious commands'. Nonetheless, in the demo-case it is not necessary.
// Simplify everything.
    // const std::string _commandPrefix;
    // const ZigBeeCommandFrameWork _framework;
    // const ZigBeeCommandDefinition _definition;
    // const bool _isCustomCommand; 
    // const std::vector<uint32_t> _commandData;


class ZigBeeCommand_TryToConnect : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_TryToConnect(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::CONNECT_TO_NETWORK, 
        "Try to connect to ZigBee Network.", 
        "custom connect-to-network {1}", 
        "custom", 
        "connect-to-network")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetAutoAnswer : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetAutoAnswer(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_AUTO_ANSWER, 
        "Set AutoAnswer Control to the given state.", 
        "custom set-auto-answer {1}", 
        "custom", 
        "set-auto-answer")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetSoundDetect : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetSoundDetect(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_SOUND_DETECT, 
        "Set Sound Detect control to the given state.", 
        "custom set-sound-detect {1}", 
        "custom", 
        "set-sound-detect")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};



class ZigBeeCommand_PhotoSaving : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_PhotoSaving(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_PHOTO_SAVING, 
        "Set Photo Saving control to the given state.", 
        "set-photo-saving {1}", 
        "custom", 
        "set-photo-saving")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};



class ZigBeeCommand_SoundSaving : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SoundSaving(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_SOUND_SAVING, 
        "Set Sound Saving control to the given state.", 
        "set-sound-saving {1}", 
        "custom", 
        "set-sound-saving")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_ButtonTriggered : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_ButtonTriggered(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_BUTTON_TRIGGERED, 
        "Set Button Triggered Alarm to the given state.", 
        "set-button-triggered {1}", 
        "custom", 
        "set-button-triggered")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_MicrophoneTriggered : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_MicrophoneTriggered(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_MICROPHONE_TRIGGERED, 
        "Set Microphone Triggered Alarm to the given state.", 
        "set-microphone-triggered {1}", 
        "custom", 
        "set-microphone-triggered")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_BoxVibrationAlarmTriggered : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_BoxVibrationAlarmTriggered(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_BOX_VIBRATION_TRIGGERED, 
        "Set Button Triggered Alarm to the given state.", 
        "set-box-vibration-triggered {1}", 
        "custom", 
        "set-box-vibration-triggered")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_AdditionalSensorMapperState : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_AdditionalSensorMapperState(uint32_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_ADDITIONAL_MAPPER_STATE, 
        "Set Additional Sensors Mapper to the given state.", 
        "set-additionalSensorMapper-state {1}", 
        "custom", 
        "set-additionalSensorMapper-state")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetPlantCareDeviceEnable : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetPlantCareDeviceEnable(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_PLANT_CARE_DEVICE_ENABLE, 
        "Set Device Enable Control to the given state.", 
        "set-device-enable {1}", 
        "custom", 
        "set-device-enable")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetPlantCareFunctionalMode : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetPlantCareFunctionalMode(uint8_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_FUNCTIONAL_MODE, 
        "Set Functional Mode control to the given state.", 
        "set-functional-mode {1}", 
        "custom", 
        "set-functional-mode")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetPlantCareWaterValvesMapper : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetPlantCareWaterValvesMapper(uint32_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_WATER_VALVES_MAPPER, 
        "Set Water Valves Mapper control to the given state.", 
        "set-water-valves-mapper {1}", 
        "custom", 
        "set-water-valves-mapper")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetPlantCareSoilHumidityMapper : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetPlantCareSoilHumidityMapper(uint32_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_SOIL_HUMIDITY_MAPPER, 
        "Set Soil Humidity Mapper control to the given state.", 
        "set-soil-humidity-mapper {1}", 
        "custom", 
        "set-soil-humidity-mapper")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetPlantCareLowerPlantMapper : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetPlantCareLowerPlantMapper(uint32_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_LOWER_PLANT_MAPPER, 
        "Set Soil Humidity Mapper control to the given state.", 
        "set-lower-plant-mapper {1}", 
        "custom", 
        "set-lower-plant-mapper")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetPlantCareMiddlePlantMapper : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetPlantCareMiddlePlantMapper(uint32_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_MIDDLE_PLANT_MAPPER, 
        "Set Middle Plant Mapper to the given state.", 
        "set-middle-plant-mapper {1}", 
        "custom", 
        "set-middle-plant-mapper")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


class ZigBeeCommand_SetPlantCareUpperPlantMapper : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetPlantCareUpperPlantMapper(uint32_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_UPPER_PLANT_MAPPER, 
        "Set Upper Plant Mapper to the given state.", 
        "set-upper-plant-mapper {1}", 
        "custom", 
        "set-upper-plant-mapper")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};



class ZigBeeCommand_SetPlantCareNoWaterAlarm : public virtual ZigBeeCommandBase
{    
    public:

        ZigBeeCommand_SetPlantCareNoWaterAlarm(uint32_t value) : 
        ZigBeeCommandBase(ZigBeeCommandType::SET_NO_WATER_ALARM, 
        "Set No Water Alarm to the given state.", 
        "set-no-water-alarm {1}", 
        "custom", 
        "set-no-water-alarm")
        {
            _resultCommand = _commandPrefix + " " + _commandBody + " " + value;
        }

        virtual String GetCommand() const override final { return _resultCommand; }
};


#endif