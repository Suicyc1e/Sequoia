#ifndef ASYNC_WEB_SERVER_H
#define ASYNC_WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>

//TODO::
//IFDEF IT!!!!

using NetworkSelectedByUserCallback = std::function<void(const String&, const String&)>;

enum class WebServerPhase
{
    INIT_PHASE,
    READY_PHASE
};


class AsyncWebServerMaintainer
{

private:

    static AsyncWebServerMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;

private:
    
    AsyncWebServer _asyncWebServer;

private:

    NetworkSelectedByUserCallback _networkSelectedCallBack;

public:

    //BAD ARCHITECTURE! This callback SHOULD be INSTALLED BEFORE ANY(!!!) USE of...
    void SetNetworkSelectedCallback(NetworkSelectedByUserCallback callback) { _networkSelectedCallBack = callback; }

private:

    void InitializeMainStageWebServer();
    void InitializeInitialWebServer();

private:

    AwsTemplateProcessor _processor;
    
    static String VariablesProcessor(const String &data);
    static String ButtonsProcessor(const String &data); 

private:

    static void NetworkSetupControl(AsyncWebServerRequest *request);
    AsyncCallbackJsonWebHandler *_networkCredentialsHandler;


private:

    static void PlantCarerZigBeeStart(AsyncWebServerRequest *request);
    static void PlantCarerWaterPumpControl(AsyncWebServerRequest *request);
    static void PlantCarerWaterLockerControl(AsyncWebServerRequest *request);
    static void PlantCarerLedBulbControl(AsyncWebServerRequest *request);
    static void PlantCarerWaterLevelCheck(AsyncWebServerRequest *request);
    static void PlantCarerSoilHumidityLevelCheck(AsyncWebServerRequest *request);


private:

    //REST-API Handlers
    static void DoorBellZigBeeStart(AsyncWebServerRequest *request);
    static void DoorBellSetControl(AsyncWebServerRequest *request);
    static void DoorBellCheckButtonAlarm(AsyncWebServerRequest *request);
    static void DoorBellCheckMicrophoneAlarm(AsyncWebServerRequest *request);
    static void DoorBellCheckBoxVibrationAlarm(AsyncWebServerRequest *request);
    static void DoorBellCheckAdditionalSensorsMapper(AsyncWebServerRequest *request);

    static void DoorBellGetDeviceStatus(AsyncWebServerRequest *request);

    static void DoorBellSoundControl(AsyncWebServerRequest *request);
    static void DoorBellGetSoundTrack(AsyncWebServerRequest *request);

    static void DoorBellLedTest(AsyncWebServerRequest *request);
    static void DoorBellToggleClamp(AsyncWebServerRequest *request);
    static void DoorBellToggleClampTimed(AsyncWebServerRequest *request);

    static void DoorBellScalesTare(AsyncWebServerRequest *request);
    static void DoorBellScalesMeasure(AsyncWebServerRequest *request);

private:

    static void PetFeederToggleClamp(AsyncWebServerRequest *request);
    static void PetFeederToggleClampTimed(AsyncWebServerRequest *request);
    static void PetFeederScalesTare(AsyncWebServerRequest *request);
    static void PetFeederScalesMeasure(AsyncWebServerRequest *request);
    static void PetFeederScalesCalibrate(AsyncWebServerRequest *request);
    static void PetFeederSetScale(AsyncWebServerRequest *request);
    static void PetFeederDirectMeasure(AsyncWebServerRequest *request);
    static void PetFeederFeedThePet(AsyncWebServerRequest *request);
    static void PetFeederSoundControl(AsyncWebServerRequest *request);
    static void PetFeederGetSoundTrack(AsyncWebServerRequest *request);
    static void PetFeederLedTest(AsyncWebServerRequest *request);

public:

    AsyncWebServerMaintainer();
    ~AsyncWebServerMaintainer();

public:

    AsyncWebServerMaintainer(AsyncWebServerMaintainer &other) = delete;
    void operator=(const AsyncWebServerMaintainer &) = delete;

    static AsyncWebServerMaintainer *GetInstance();

private:
    
    void StartWebServer() { _asyncWebServer.begin(); }

public:

    void StopWebServer() { _asyncWebServer.end(); }
    void StartWebServer(WebServerPhase phase);

};


#endif