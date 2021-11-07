#include <SequoiaWiFiStartUpManager.h>

#include <Arduino.h>
#include <WiFi.h>
#include <GlobalPresets.h>
#include <AsyncWebServerMaintainer.h>
#include <SmartDoorBell.h>
#include <SmartPlantCarer.h>
#include <SDCardMaintainer.h>
#include <ESPmDNS.h>
#include <SmartPetFeeder.h>

const String SequoiaWiFiStartUpManager::AccessPointNamePrefix{ACCESS_POINT_PREFIX};




void SequoiaWiFiStartUpManager::SetAccessPointPassword()
{
    Serial.println("Set AP password");
    _accesssPointPassword = ACCESS_POINT_DEFAULT_PASSWORD;
}

void SequoiaWiFiStartUpManager::FormAccessPointName()
{
    Serial.println("Form AP SSID");
    _accessPointSSID = "";
    _accessPointSSID.concat(AccessPointNamePrefix);

    switch (_deviceType)
    {
    case SequoiaDeviceType::DOOR_BELL:
        _accessPointSSID.concat("DOOR_BELL_");
        break;
    case SequoiaDeviceType::PET_FEEDER:
        _accessPointSSID.concat("PET_FEEDER_");
        break;
    case SequoiaDeviceType::PLANT_CARER:
        _accessPointSSID.concat("PLANT_CARER_");
        break;
    }
    //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system.html
    _accessPointSSID.concat(WiFi.macAddress()); //This SHOULD be called prior to any MAC Chahnge functions...
}


void SequoiaWiFiStartUpManager::CreateAndStartAccessPoint()
{
    Serial.printf("Starting access point with SSID: %s and Password: %s \n", _accessPointSSID.c_str(), _accesssPointPassword.c_str());
    WiFi.softAP(_accessPointSSID.c_str(), _accesssPointPassword.c_str());
}


void SequoiaWiFiStartUpManager::LoadConfiguration()
{
    Serial.println("FAKE Load configuration");
    

#ifdef ALEX_WIFI

    String ssid = "Gaika";
    String password = "987456PSh";
#else

    String ssid = "Unknown";
    String password = "somepassword"

#endif
    
    //EMULATE LOADING!
    _userSettings = UserSettings{ssid, password, false};
}


void SequoiaWiFiStartUpManager::SetNetworkSelectedCallback()
{
    AsyncWebServerMaintainer::GetInstance()->SetNetworkSelectedCallback(
        [&](const String& ssid, const String& pswd){OnNetworkSelectedHandler(ssid, pswd);}
    );
}



//ASYNC CALL!
//REFACTORING --> MACRO SHOULD ADD VTaskDelete(NULL) to the end of every such call... 
void SequoiaWiFiStartUpManager::ProcessUpdatedNetworkCredentials()
{
    Serial.println("Starting updating Network Credentials.");
    
    //Stop WebServer
    //Serial.println("Stopping Web-Server");
    //AsyncWebServerMaintainer::GetInstance()->StopWebServer();
    Serial.println("Do not stop Web-Server");

    //Stop Access point...
    Serial.println("Stop Access point..");
    WiFi.softAPdisconnect (true);

    //Try to start another Connection.
    if (this->ConnectToNetworkSetByUser())
    {
        Serial.println("Starting Main Stage AsyncWebServer!");
        AsyncWebServerMaintainer::GetInstance()->StartWebServer(WebServerPhase::READY_PHASE);

        Serial.println("Starting DNS-SD Service");
        if(!MDNS.begin("sequoia")) 
        {
            Serial.println("Error starting mDNS");
        }

        MDNS.addService("http", "tcp", 80);
        MDNS.addServiceTxt("http", "tcp", "Sequoia", "mDNS-test!");
    }
    else
    {
        Serial.println("Restaring Acces Point!");
        this->CreateAndStartAccessPoint();
        Serial.println("Going back to Initial Stage AsyncWebServer Configuration!");
        AsyncWebServerMaintainer::GetInstance()->StartWebServer(WebServerPhase::INIT_PHASE);
    }
}


bool SequoiaWiFiStartUpManager::AsyncUpdateNetworkCredentials()
{   
    xTaskCreate(
        AsyncProcessUpdatedNetworkCredentialsTask,          /* Task function. */
        "AsyncProcessUpdatedNetworkCredentialsTask",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        this,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */


    Serial.println("Create AsyncProcessUpdatedNetwrokCredentials Task.");
    return true;
}

void SequoiaWiFiStartUpManager::AsyncProcessUpdatedNetworkCredentialsTask(void *taskStartParameters)
{
    SequoiaWiFiStartUpManager::GetInstance()->ProcessUpdatedNetworkCredentials();

    vTaskDelete(NULL);
}


void SequoiaWiFiStartUpManager::OnNetworkSelectedHandler(const String& ssid, const String& password)
{
    Serial.printf("OnNetworkSelected is Called! Values are: SSID: %s PASSWORD %s \n", ssid.c_str(), password.c_str());
    //_userSettings.m_network_ssid.clear(); = ssid;
    //_userSettings.m_network_password.clear(); = password;

    _userSettings.m_network_ssid = ssid.c_str();
    _userSettings.m_network_password = password.c_str();

    Serial.printf("Struct set values are! Values are: SSID: %s PASSWORD %s \n", _userSettings.m_network_ssid.c_str(), _userSettings.m_network_password.c_str());
    //Callback should be async!!!
    
    //FUCK THE ASYNC CALLBAK!
    AsyncUpdateNetworkCredentials();
}

void SequoiaWiFiStartUpManager::StartInitialUserInterface()
{
    
}

void SequoiaWiFiStartUpManager::SaveUserDefinedConfiguration()
{

}

bool SequoiaWiFiStartUpManager::StartUpUserDefinedInterface()
{
    return true;
}


//Create Device is also start of Device)
bool SequoiaWiFiStartUpManager::CreateDevice()
{
    switch (_deviceType)
    {
    case SequoiaDeviceType::DOOR_BELL:
        m_devicePtr = SmartDoorBell::GetInstance();
        break;
    case SequoiaDeviceType::PET_FEEDER:
        m_devicePtr = SmartPetFeeder::GetInstance();
        break;
    case SequoiaDeviceType::PLANT_CARER:
        m_devicePtr = SmartPlantCarer::GetInstance();
        break;
    }

    return true;
}

void SequoiaWiFiStartUpManager::StartDevice()
{
    switch (_deviceType)
    {
    case SequoiaDeviceType::DOOR_BELL:
        SmartDoorBell::GetInstance()->StartLoopedServices();
        break;
    case SequoiaDeviceType::PET_FEEDER:
        //_accessPointSSID.concat("PET_FEEDER_");
        SmartPetFeeder::GetInstance()->StartLoopedServices();
        break;
    case SequoiaDeviceType::PLANT_CARER:
        SmartPlantCarer::GetInstance()->StartLoopedServices();
        break;
    }
}

//Can't use local variables here....
bool SequoiaWiFiStartUpManager::ConnectToNetworkSetByUser()
{
    
    //UserSettings newVal = _userSettings;
    Serial.println("Connecting to ");
    //Serial.printf("Check line! %s", _testString.c_str());
    Serial.printf("Struct set values are! Values are: SSID: %s PASSWORD %s \n", _userSettings.m_network_ssid.c_str(), _userSettings.m_network_password.c_str());
    WiFi.begin(_userSettings.m_network_ssid.c_str(), _userSettings.m_network_password.c_str());
    
    unsigned long startTime = millis();
    uint32_t millisecondsToWait = 1000*WIFI_CONNECT_ATTEMPT_TIME_S;
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime) <= millisecondsToWait) 
    {
        delay(500);
        Serial.print(".");
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.disconnect(true);
        Serial.printf("WiFi connection failed. Tried for %d seconds. SSID: %s Password: %s \n", 
        WIFI_CONNECT_ATTEMPT_TIME_S, 
        _userSettings.m_network_ssid.c_str(),
        _userSettings.m_network_password.c_str());
        return false;
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    //Save this IP HERE!
    Serial.println(WiFi.localIP());

    if (m_devicePtr != NULL)
    {
        m_devicePtr->SetNetworkCredentials(NetworkCredentials(
            WiFi.localIP(),
            _userSettings.m_network_ssid, 
            _userSettings.m_network_password));
    }

    return true;

}

void SequoiaWiFiStartUpManager::ExecuteStartUpRoutines()
{    
    
#ifndef DEBUG_MOBILE_APPLICATION_DEVELOPMENT
    //FIRST OF ALL, START FILE SYSTEM!
    SDCardMaintainer::GetInstance(); //START FS!
    
#endif

    SetAccessPointPassword();
    FormAccessPointName();
    LoadConfiguration();
}


SequoiaWiFiStartUpManager::SequoiaWiFiStartUpManager()
{
#ifdef BUILD_FOR_DOOR_BELL
    _deviceType = SequoiaDeviceType::DOOR_BELL;
#endif
#ifdef BUILD_FOR_PLANT_CARER
    _deviceType = SequoiaDeviceType::PLANT_CARER;
#endif
#ifdef BUILD_FOR_PET_FEEDER
    _deviceType = SequoiaDeviceType::PET_FEEDER;
#endif



    ExecuteStartUpRoutines();

}

SequoiaWiFiStartUpManager::~SequoiaWiFiStartUpManager()
{
}


void SequoiaWiFiStartUpManager::Begin()
{
    Serial.println("Create device!");
    CreateDevice();
    
    Serial.println("Starting Initial Stage AsyncWebServer Configuration!");

    //AsyncWebServerMaintainer::GetInstance()->StartWebServer(WebServerPhase::INIT_PHASE);
    
#ifdef DEBUG_SKIP_WELCOME_AP

    ConnectToNetworkSetByUser();
    Serial.println("Starting Main Stage AsyncWebServer!");
    AsyncWebServerMaintainer::GetInstance()->StartWebServer(WebServerPhase::READY_PHASE);

    Serial.println("Starting DNS-SD Service");
    if(!MDNS.begin("sequoia")) 
    {
        Serial.println("Error starting mDNS");
    }

    MDNS.addService("http", "tcp", 80);
    MDNS.addServiceTxt("http", "tcp", "Sequoia", "mDNS-test!");

#else

    CreateAndStartAccessPoint();
    
    if (UserSettingsHaveValidNetworkConfig())
    {
        Serial.println("User settings have valid Netwrok Credentials. Starting with Full GUI. (FAKE for NOW)");
    }

#endif


    AsyncWebServerMaintainer::GetInstance(); ///Create it. (May no be first-call, nonetheles...)
    SetNetworkSelectedCallback();
}


UserSettings SequoiaWiFiStartUpManager::_userSettings{"", "", false};
String SequoiaWiFiStartUpManager::_accessPointSSID{""};
String SequoiaWiFiStartUpManager::_accesssPointPassword{""};

SequoiaWiFiStartUpManager *SequoiaWiFiStartUpManager::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new SequoiaWiFiStartUpManager();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}

SequoiaWiFiStartUpManager *SequoiaWiFiStartUpManager::pinstance_{nullptr};
SemaphoreHandle_t SequoiaWiFiStartUpManager::mutex_{xSemaphoreCreateMutex()};