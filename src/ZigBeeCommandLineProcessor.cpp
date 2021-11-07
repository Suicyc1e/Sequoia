#include <ZigBeeCommandLineProcessor.h>
#include <GlobalPresets.h>


int StringSplit(String sInput, char cDelim, String sParams[], int iMaxParams)
{
    int iParamCount = 0;
    int iPosDelim, iPosStart = 0;

    do {
        // Searching the delimiter using indexOf()
        iPosDelim = sInput.indexOf(cDelim,iPosStart);
        if (iPosDelim > (iPosStart+1)) {
            // Adding a new parameter using substring() 
            sParams[iParamCount] = sInput.substring(iPosStart,iPosDelim-1);
            iParamCount++;
            // Checking the number of parameters
            if (iParamCount >= iMaxParams) {
                return (iParamCount);
            }
            iPosStart = iPosDelim + 1;
        }
    } while (iPosDelim >= 0);
    if (iParamCount < iMaxParams) {
        // Adding the last parameter as the end of the line
        sParams[iParamCount] = sInput.substring(iPosStart);
        iParamCount++;
    }

    return (iParamCount);
}



    void ZigBeeCommandLineProcessor::ProcessIncomingData(String esp32Data)
    {
        Serial.println("ZigBee Message Data: " + esp32Data);

        if (esp32Data.startsWith(_attributeMessagePrefix))
        {
            Serial.println("Message is ATTRIBUTE_CHANGE");
            int attributeID = 0;
            int attributeValue = 0;

            String splitControl = _attributeMessagePrefix + "%d" + " " + "%d";

            int n = sscanf(esp32Data.c_str(), splitControl.c_str(), &attributeID, &attributeValue);
            
            if (n != 2)
            {
                Serial.println("WARNING! Sscanf didn't parse the message correctly!");
            }

            Serial.printf(" Attribute ID && Data: %d && %d \n", attributeID, attributeValue);
            ZigBeeMessage_AttributeChanged message = ZigBeeMessage_AttributeChanged(attributeID, attributeValue, esp32Data);
            _attributeChangedCallback(message);
        }
        else if (esp32Data.startsWith(_nwkSuccessMessagePrefix))
        {
            Serial.println("Message is NWK_SUCCESS");

            int finalState = 0;

            String splitControl = _nwkSuccessMessagePrefix + "%d";

            int n = sscanf(esp32Data.c_str(), splitControl.c_str(), &finalState);

            if (n != 1)
            {
                Serial.println("WARNING! Sscanf didn't parse the message correctly!");
            }

            Serial.printf(" Final State: %d \n", finalState);
            ZigBeeMessage_NwkSuccess message = ZigBeeMessage_NwkSuccess(finalState, esp32Data);
            _nwkStatusSuccededCallback(message);
        }
        else if (esp32Data.startsWith(_nwkFailedMessagePrefix))
        {
            Serial.println("Message is NWK_FAILED");

            int finalState = 0;

            String splitControl = _nwkFailedMessagePrefix + "%d";

            int n = sscanf(esp32Data.c_str(), splitControl.c_str(), &finalState);

            if (n != 1)
            {
                Serial.println("WARNING! Sscanf didn't parse the message correctly!");
            }

            Serial.printf(" Final State: %d \n", finalState);
            ZigBeeMessage_NwkFailed message = ZigBeeMessage_NwkFailed(finalState, esp32Data);
            _nwkStatusFailedCallback(message);
        }
        else
        {
            Serial.println("Message is UNKNOWN");
        }        
    }


    void ZigBeeCommandLineProcessor::ZigbeeCommandsReader(void *parameter)
    {        
        ZigBeeCommandLineProcessor *processor = (ZigBeeCommandLineProcessor*) parameter;

        while (1)
        {
            TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
            TIMERG0.wdt_feed=1;
            TIMERG0.wdt_wprotect=0;
            
            if (ZigBeePort.available())
            {
                String esp32Data = ZigBeePort.readStringUntil('\n');
                processor->ProcessIncomingData(esp32Data);
            }

            // unsigned long thedelay;
            // thedelay = micros() + 100;
            // while (micros() < thedelay) 
            // {

            // }

            //Too short delay for music to play.
            vTaskDelay(UART_COMMAND_LINE_TASK_DELAY_MS/portTICK_PERIOD_MS);
        }
    }

    ZigBeeCommandLineProcessor::ZigBeeCommandLineProcessor( AttributeChangedCallback attrChangedCallback, 
                                NwkStatusFailedCallback nwkFailedCallback, 
                                NwkStatusSuccededCallback nwkOkCallback)
    {
        _attributeChangedCallback = attrChangedCallback;
        _nwkStatusFailedCallback = nwkFailedCallback;
        _nwkStatusSuccededCallback = nwkOkCallback;

        //ExecuteStartUpProcedures();
    }

    void ZigBeeCommandLineProcessor::ExecuteStartUpProcedures()
    {
        //Init UART, ask ZigBee if it's ready, etc...
        //ZigBee Console
        ZigBeePort.begin(115200);
        //DBG Message. Delete later.
        ZigBeePort.println("ZigBee TEST UART Is Initialized.");

        Serial.println("Starting Thread.");

        xTaskCreate(
                ZigbeeCommandsReader,          /* Task function. */
                "zigbeeCommandsReader",        /* String with name of task. */
                10000,            /* Stack size in bytes. */
                this,             /* Parameter passed as input of the task */
                1,                /* Priority of the task. */
                &ZigBeeCommandsReaderTask);            /* Task handle. */
    }


    void ZigBeeCommandLineProcessor::CommandsTest()
    {
        ZigBeePort.println(ZigBeeCommand_TryToConnect(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SoundSaving(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetSoundDetect(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetAutoAnswer(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_PhotoSaving(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_MicrophoneTriggered(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_ButtonTriggered(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_BoxVibrationAlarmTriggered(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_AdditionalSensorMapperState(19).GetCommand());

        ZigBeePort.println(ZigBeeCommand_SetPlantCareDeviceEnable(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetPlantCareFunctionalMode(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetPlantCareWaterValvesMapper(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetPlantCareSoilHumidityMapper(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetPlantCareLowerPlantMapper(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetPlantCareMiddlePlantMapper(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetPlantCareUpperPlantMapper(1).GetCommand());
        ZigBeePort.println(ZigBeeCommand_SetPlantCareNoWaterAlarm(1).GetCommand());
    }

HardwareSerial ZigBeeCommandLineProcessor::ZigBeePort = Serial2;
TaskHandle_t ZigBeeCommandLineProcessor::ZigBeeCommandsReaderTask = NULL;