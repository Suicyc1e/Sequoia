#ifndef SEQUOIA_START_UP_MANAGER_H
#define SEQUOIA_START_UP_MANAGER_H

#include <ISequoiaStartUpManager.h>
#include <ISequoiaDevice.h>
#include <UserSettings.h>


//REFACTOR NOTES:: TEMPLATE IT!!!
class SequoiaWiFiStartUpManager : public virtual ISequoiaStartUpManager
{

//Singletoning)))
private:

    static SequoiaWiFiStartUpManager *pinstance_;
    static SemaphoreHandle_t mutex_;

private:

    static const String AccessPointNamePrefix;

    static String _accessPointSSID;
    static String _accesssPointPassword;
    SequoiaDeviceType _deviceType;

private:

    ISequoiaDevice *m_devicePtr = NULL;

private:

    void FormAccessPointName();
    void SetAccessPointPassword(); //may it be stronger later...

private:
    
    virtual void LoadConfiguration() override final;
    virtual void StartInitialUserInterface() override final;
    virtual void SaveUserDefinedConfiguration() override final;
    virtual bool StartUpUserDefinedInterface() override final;
    virtual bool CreateDevice() override final;

public:

    virtual void StartDevice() override final;

private:

    bool UserSettingsHaveValidNetworkConfig() { return _userSettings.m_network_ssid != "" && _userSettings.m_network_password != ""; }
    static UserSettings _userSettings;

private:

    void CreateAndStartAccessPoint();
    void CreateAndStartAsyncWebServer();

private:

    void ExecuteStartUpRoutines();

public:

    void ConnectToNetwork(const String& ssid, const String& password);

public:
    
    //I need Handler here ---> To Set Password;
    void OnNetworkSelectedHandler(const String& ssid, const String& password);

private:

    void SetNetworkSelectedCallback();

private:

    bool ConnectToNetworkSetByUser(); // This is NOT Async Call!

public:

    void ProcessUpdatedNetworkCredentials(); 

private:

    //Thanks to the https://stackoverflow.com/questions/36987788/pointer-to-function-member
    //Ali Akdurak 
    TaskHandle_t *moduleLoopTaskHandle;
    bool AsyncUpdateNetworkCredentials();
    static void AsyncProcessUpdatedNetworkCredentialsTask(void *taskStartParameters);

    String _testString = "ololo";

public:

    void Begin();


protected:

    SequoiaWiFiStartUpManager();
    ~SequoiaWiFiStartUpManager();

public:

    SequoiaWiFiStartUpManager(SequoiaWiFiStartUpManager &other) = delete;
    void operator=(const SequoiaWiFiStartUpManager &) = delete;

    static SequoiaWiFiStartUpManager *GetInstance();
};




#endif