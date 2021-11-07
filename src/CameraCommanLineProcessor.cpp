#include <CameraCommandLineProcessor.h>
#include <GlobalPresets.h>
#include <CameraCommands.h>
#include <CameraMessages.h>
#include <PIOMapper.h>

//https://github.com/espressif/arduino-esp32/issues/595
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"


void CameraCommandLineProcessor::ProcessIncomingData(String esp32CamData)
{
    Serial.println("Data to process: " + esp32CamData);
    if (esp32CamData.startsWith(_cameraReadyPrefix))
    {
        Serial.println("Message is CAMERA_READY");
        
        bool cameraIsReady = false;

        String splitControl = _cameraReadyPrefix + "%d";

        int n = sscanf(esp32CamData.c_str(), splitControl.c_str(), &cameraIsReady);
        
        if (n != 1)
        {
            Serial.println("WARNING! Sscanf didn't parse the message correctly!");
        }

        Serial.printf("Camera ready status: %d\n", cameraIsReady);
        CameraMessage_Ready message = CameraMessage_Ready(cameraIsReady, esp32CamData);
        _readyCallback(cameraIsReady);
    }
    else if (esp32CamData.startsWith(_ipMessagePrefix))
    {
        Serial.println("Message is IP_SET");

        char *ipAddrString[100];

        String splitControl = _ipMessagePrefix + "%s";

        //https://stackoverflow.com/questions/2854488/reading-a-string-with-spaces-with-sscanf
        int n = sscanf(esp32CamData.c_str(), splitControl.c_str(), ipAddrString);

        if (n != 1)
        {
            Serial.println("WARNING! Sscanf didn't parse the message correctly!");
        }

        Serial.printf("IP Address: %s \n", ipAddrString);

        IPAddress address;
        String ipAddrTextString((char *)ipAddrString);

        address.fromString(ipAddrTextString.c_str());

        Serial.printf("IP Address: %d \n", address);

        CameraMessage_IPSet message = CameraMessage_IPSet(address, esp32CamData);
        _ipSetCallback(address);
    }
    else
    {
        Serial.println("Message is UNKNOWN");
    }        
}


void CameraCommandLineProcessor::ExecuteStartUpProcedures()
{
    //Init UART, ask ZigBee if it's ready, etc...
    //ZigBee Console
    CameraPort.begin(115200, SERIAL_8N1, USER_CAMERA_UART_RX, USER_CAMERA_UART_TX);
    //DBG Message. Delete later.
    CameraPort.println("Camera TEST UART Is Initialized.");

    Serial.println("Starting Thread.");

    xTaskCreate(
            CameraCommandsReader,          /* Task function. */
            "zigbeeCommandsReader",        /* String with name of task. */
            10000,                         /* Stack size in bytes. */
            this,                          /* Parameter passed as input of the task */
            1,                             /* Priority of the task. */
            &CameraCommandsReaderTask);    /* Task handle. */
}


void CameraCommandLineProcessor::CameraCommandsReader(void *parameter)
{
    CameraCommandLineProcessor *processor = (CameraCommandLineProcessor*) parameter;

    while (1)
    {
        TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
        TIMERG0.wdt_feed=1;
        TIMERG0.wdt_wprotect=0;
        
        if (CameraPort.available())
        {
            String esp32Data = CameraPort.readStringUntil('\n');
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


void CameraCommandLineProcessor::CommandsTest()
{
    const char* ssid = "GS-Labs";
    const char* password = "NetWork55!";
    SendSetSSIDCommand(ssid);
    SendSetPasswordCommand(password);
}

CameraCommandLineProcessor::CameraCommandLineProcessor(/* args */)
{
}

CameraCommandLineProcessor::~CameraCommandLineProcessor()
{
}

HardwareSerial CameraCommandLineProcessor::CameraPort = Serial1;
TaskHandle_t CameraCommandLineProcessor::CameraCommandsReaderTask = NULL;