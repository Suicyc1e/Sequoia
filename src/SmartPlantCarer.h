#ifndef SMART_PLANT_CARER_H
#define SMART_PLANT_CARER_H

#include <Arduino.h>
#include <ISequoiaDevice.h>
#include <ZigBeeCommandLineProcessor.h>

class SmartPlantCarer : public virtual ISequoiaDevice
{

//overriding
public:

    virtual void SetNetworkCredentials(const NetworkCredentials& credentials) override final
    {
        _networkCredentials = credentials;
    }

    virtual void StartLoopedServices() override final
    {
        //nothing to do here for now
    }

private:

    NetworkCredentials _networkCredentials;

//Singletoning)))
private:

    static SmartPlantCarer *pinstance_;
    static SemaphoreHandle_t mutex_;

private:
    /* data */

protected:

    SmartPlantCarer(/* args */);
    ~SmartPlantCarer();

private:

//Watering

    const uint8_t _waterPump_PIO;
    
    const uint8_t _waterValve_1_PIO;
    const uint8_t _waterValve_2_PIO;
    const uint8_t _waterValve_3_PIO;
    const uint8_t _waterValve_4_PIO;

//Humidity Sensor

    const uint8_t _soilHumiditySensor_1_PIO;
    
//Water Level

    const uint8_t _waterLevelLow_PIO;
    const uint8_t _waterLevelHigh_PIO;

//

    const uint8_t _ledLampRelay_PIO;


private:

    void HardwareInit();
    void InitServices();


//TODO: Refactor to be One class of "Device"!
private:

    ZigBeeCommandLineProcessor _zigBeeCommandLineProcessor;

    //Callback Handlers

    void OnAttributeChangedHandler(ZigBeeMessage_AttributeChanged msg);
    void OnNwkSuccededHandler(ZigBeeMessage_NwkSuccess msg);
    void OnNwkFailedHandler(ZigBeeMessage_NwkFailed msg);
    void OnZigBeeCommandReceivedHandler(SmartPlantCarerCommand command);

    bool _zigBeeIsConnected;

    IPAddress _ownIPAddress;
    IPAddress _cameraIPAddress;

public:

    void SetOwnIPAddress(IPAddress givenAddress) { _ownIPAddress = givenAddress; }
    IPAddress GetOwnIPAddress() const { return _ownIPAddress; }

public:

    void AttributeChangedProcessor(uint32_t attributeID, uint32_t attributeValue, AttributeChangeSource source);

public:

    void StartZigBee();


public:

    void HardwareTest();

public:

    void EnableWaterPump(bool enable);
    void EnableWaterLocker(uint8_t id, bool enable);
    bool GetHightWaterLevelState();
    bool GetLowWaterLevelState();
    void EnableLedBulbRelay(bool enabel);
    bool GetSoilHumididtyState(uint8_t id);

private:

    void SetHardwarePresets();


public:

    SmartPlantCarer(SmartPlantCarer &other) = delete;
    void operator=(const SmartPlantCarer &) = delete;

    static SmartPlantCarer *GetInstance();
};





#endif