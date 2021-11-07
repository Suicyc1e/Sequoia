#ifndef SMART_PET_FEEDER_H
#define SMART_PET_FEEDER_H

#include <Arduino.h>
#include <ISequoiaDevice.h>
#include <ZigBeeCommandLineProcessor.h>

class SmartPetFeeder : public virtual ISequoiaDevice
{
//Singletoning)))
private:

    static SmartPetFeeder *pinstance_;
    static SemaphoreHandle_t mutex_;

//overriding
public:

    virtual void SetNetworkCredentials(const NetworkCredentials& credentials) override final
    {
        _networkCredentials = credentials;
    }

    virtual void StartLoopedServices() override final;


private:

    NetworkCredentials _networkCredentials;

private:

    void StartSpeakerLoop();

protected:

    SmartPetFeeder(/* args */);
    ~SmartPetFeeder();

private:

    float _currentWeight;

public:

    void SetCurrentWeight(float weight) { _currentWeight = _currentWeight; };
    float GetCurrentWeight() { return _currentWeight; }

private:

    void HardwareInit();
    void InitServices();


//TODO: Refactor to be One class of "Device"!
private:

    ZigBeeCommandLineProcessor _zigBeeCommandLineProcessor;
    //CameraCommandLineProcessor _cameraCommandLineProcessor;

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

private:

    bool _lockControl = false;

public:


    void FeedThePetAsync(uint32_t weightInGramms);

private:

    void FeedThePet(uint32_t weightInGramms);
    uint32_t _currentFeedWeightInGramms;
    static void AsyncFeedThePetTask(void *parameters);

public:

    void StartZigBee();

public:

    SmartPetFeeder(SmartPetFeeder &other) = delete;
    void operator=(const SmartPetFeeder &) = delete;

    static SmartPetFeeder *GetInstance();
};




#endif