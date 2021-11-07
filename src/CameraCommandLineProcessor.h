#ifndef CAMERA_COMMAND_LINE_PROCESSOR_H
#define CAMERA_COMMAND_LINE_PROCESSOR_H

//#include <ICommandLineProcessor.h>
#include <Arduino.h>
#include <functional>


struct ICameraCommandLineProcessor
{
    /* data */
public:

    virtual void ExecuteStartUpProcedures() = 0; //Find, init, connect, test...
    virtual bool IsConnectionAlive() = 0;
    virtual String GetHardwareDescription() = 0; //ex. UART port 5, 115200, etc...
    virtual void CommandsTest() = 0;

protected:

    virtual  void ProcessIncomingData(String esp32Data) = 0; 
};

using CameraReadyCallback = std::function<void(bool)>;
using CameraIPSetCallback = std::function<void(IPAddress)>;
using CameraNwkResultCallback = std::function<void(bool)>;


class CameraCommandLineProcessor : public ICameraCommandLineProcessor
{
private:
    
    //DO NOT USE SERIAL 1 Anywhere else in the programm!
    static HardwareSerial CameraPort;

    //UART Read Task Handle (Commands ESP32-CAM ---> ESP32-MAIN Direction)
    static TaskHandle_t CameraCommandsReaderTask;

private:

    CameraReadyCallback     _readyCallback;
    CameraIPSetCallback     _ipSetCallback;
    CameraNwkResultCallback _nwlResultCallback; //skip now, add later.

private:

//incoming

    String _ipMessagePrefix = "ip_";
    String _cameraReadyPrefix = "ready_";

//outgoing
    String _ssidMesssagePrefix = "ssid_";
    String _pswdMessagePrefix = "pswd_";

public:

    //IMPROVING: Add this functionality
    virtual bool IsConnectionAlive() override final
    {
        //Return true allways.. Hope so)
        return true;
    }

    virtual void ExecuteStartUpProcedures() override final;
    
    //TODO:: Add actual hardware description...
    virtual String GetHardwareDescription() override final
    {
        return "UART2 BaudRate 115200.";
    }

    virtual void CommandsTest() override final;

public:

    void SendSetSSIDCommand(String ssid) { CameraPort.println("ssid_" + ssid); }
    void SendSetPasswordCommand(String pswd) { CameraPort.println("pswd_" + pswd); }

private:

    //https://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
    virtual void ProcessIncomingData(String esp32Data) override final;
    static void CameraCommandsReader(void *parameter);

public:

    void SetCameraCommandLineCallbacks(CameraReadyCallback readyCallback, CameraIPSetCallback ipSetCallBack)
    {
        _readyCallback = readyCallback;
        _ipSetCallback = ipSetCallBack;
    }



public:

    CameraCommandLineProcessor();
    ~CameraCommandLineProcessor();
};



#endif