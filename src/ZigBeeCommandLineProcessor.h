#ifndef IZIGBEE_COMMANDLINE_PROCESSOR
#define IZIGBEE_COMMANDLINE_PROCESSOR

#include <HardwareSerial.h>

#include <ICommandLineProcessor.h>
#include <ZigBeeCommon.h>
#include <ZigBeeCommands.h>
#include <ZigBeeMessages.h>
#include <functional>


//https://github.com/espressif/arduino-esp32/issues/595
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

//Some beautifullness
//https://stackoverflow.com/questions/2582161/function-pointer-as-parameter

// typedef void (SmartDoorBell::*AttributeChangedCallback)(ZigBeeMessage_AttributeChanged msg);
// typedef void (SmartDoorBell::*NwkStatusFailedCallback)(ZigBeeMessage_NwkFailed msg);
// typedef void (SmartDoorBell::*NwkStatusSuccededCallback)(ZigBeeMessage_NwkSuccess msg);


using AttributeChangedCallback = std::function<void(ZigBeeMessage_AttributeChanged)>;
using NwkStatusFailedCallback = std::function<void(ZigBeeMessage_NwkFailed)>;
using NwkStatusSuccededCallback = std::function<void(ZigBeeMessage_NwkSuccess)>;
using SmartPlantCarerCommandsCallback = std::function<void(SmartPlantCarerCommand)>;




class ZigBeeCommandLineProcessor : public ICommandLineProcessor
{

private:
    
    //DO NOT USE SERIAL 2 Anywhere else in the programm!
    static HardwareSerial ZigBeePort;

    //UART Read Task Handle (Commands EFR32 ---> ESP32 Direction)
    static TaskHandle_t ZigBeeCommandsReaderTask;

private:

    String _attributeMessagePrefix = "change_";
    String _nwkSuccessMessagePrefix = "nwksuccess_";
    String _nwkFailedMessagePrefix = "nwkfailed_";

private:

    // void (*_attributeChangedCallback)(ZigBeeMessage_AttributeChanged msg);
    // void (*_nwkStatusFailedCallback)(ZigBeeMessage_NwkFailed msg);
    // void (*_nwkStatusSuccededCallback)(ZigBeeMessage_NwkSuccess msg);

    AttributeChangedCallback _attributeChangedCallback;
    NwkStatusFailedCallback _nwkStatusFailedCallback;
    NwkStatusSuccededCallback _nwkStatusSuccededCallback;
    SmartPlantCarerCommandsCallback _smartPlantCarerCommandsCallback;

private:

    //https://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
    virtual void ProcessIncomingData(String esp32Data) override final;

private:

    static void ZigbeeCommandsReader(void *parameter);
   

public:

    ZigBeeCommandLineProcessor(AttributeChangedCallback attrChangedCallback, 
                                NwkStatusFailedCallback nwkFailedCallback, 
                                NwkStatusSuccededCallback nwkOkCallback);
    

    virtual void ExecuteStartUpProcedures() override final;


    //IMPROVING: Add this functionality
    virtual bool IsConnectionAlive() override final
    {
        //Return true allways.. Hope so)
        return true;
    }

    //TODO:: Return here something interesting - actual IC name.
    String GetZigBeeICName()
    {
        return "ESP32...";
    }

    //TODO:: Add actual hardware description...
    virtual String GetHardwareDescription() override final
    {
        return "UART2 BaudRate 115200.";
    }

    virtual void CommandsTest() override final;

//Commands Senders
//IMPROVING: Yeah, TEMPLATE it, baby!
public:

    void SendZigBeeStartUp()
    {
        Serial.println("Sending ZigBee startup command! " + ZigBeeCommand_TryToConnect(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_TryToConnect(1).GetCommand());
    }

    void SendAutoAnswerCommand(bool value)
    {
        ZigBeePort.println(ZigBeeCommand_SetAutoAnswer(value).GetCommand());
    }
    
    void SendSoundDetectionCommand(bool value)
    {
        ZigBeePort.println(ZigBeeCommand_SetSoundDetect(value).GetCommand());
    }

    void SendPhotoSavingCommand(bool value)
    {
        ZigBeePort.println(ZigBeeCommand_PhotoSaving(value).GetCommand());
    }

    void SendSoundSavingCommand(bool value)
    {
        ZigBeePort.println(ZigBeeCommand_SoundSaving(value).GetCommand());
    }

    void SendButtonTriggeredCommand(bool value)
    {
        ZigBeePort.println(ZigBeeCommand_ButtonTriggered(value).GetCommand());
    }

    void SendMicroPhoneTriggeredCommand(bool value)
    {
        ZigBeePort.println(ZigBeeCommand_MicrophoneTriggered(value).GetCommand());
    }

    void SendBoxVibrationTriggeredCommand(bool value)
    {
        ZigBeePort.println(ZigBeeCommand_BoxVibrationAlarmTriggered(value).GetCommand());
    }

    void SendAdditionalMapperCommand(uint32_t value)
    {
        ZigBeePort.println(ZigBeeCommand_AdditionalSensorMapperState(value).GetCommand());
    }


public:

    void SetPlantCarerCommandsCallback(SmartPlantCarerCommandsCallback callback) { _smartPlantCarerCommandsCallback = callback; }

};

#endif