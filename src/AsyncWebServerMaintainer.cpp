#include <AsyncWebServerMaintainer.h>
#include <SmartDoorBell.h>
#include <SDCardMaintainer.h>
#include <SmartPlantCarer.h>
#include <SpeakerMaintainer.h>
#include <TesterOfTheFeatures.h>
#include <ClampMaintainer.h> 
#include <ScalesMaintainer.h>
#include <SmartPetFeeder.h>



const char* UI_CONTROL_ELEMENT_ID = "output";
const char* UI_CONTROL_ELEMNT_STATE = "state";
const char* UI_CONTROL_CONNECT_BUTTON_CONNECT = "connect";

AsyncWebServerMaintainer::AsyncWebServerMaintainer() : _asyncWebServer(AsyncWebServer(80)),
                                                       _processor([this](const String &data) -> String
                                                                  { return VariablesProcessor(data); })
{
    //InitializeMainStageWebServer();
    //InitializeInitialWebServer();
    //_asyncWebServer.begin();
    Serial.println("Starting Async WebServer.");
    InitializeInitialWebServer();
    _asyncWebServer.begin();
}


void AsyncWebServerMaintainer::StartWebServer(WebServerPhase phase)
{
    switch (phase)
    {
    case WebServerPhase::INIT_PHASE:
        {
            _asyncWebServer.reset();
            InitializeInitialWebServer();            
            //_asyncWebServer.begin();
        }
        break;
    case WebServerPhase::READY_PHASE:
        {
            _asyncWebServer.reset();
            InitializeMainStageWebServer();            
            //_asyncWebServer.begin();
        }
        break;
    }
}

AsyncWebServerMaintainer::~AsyncWebServerMaintainer()
{
}

String AsyncWebServerMaintainer::VariablesProcessor(const String &data)
{
    //TODO::Implement these variables regarding to the current state of things...
    //Callbacks...
    // date
    // ZConnect
    // ip_video_stream
    // fire_s_state
    // vibro_s_state
    // mat_s_state
    // mot_s_state
    Serial.println(data);
    if (data == "fire_s_state")
    {
        return "no alarm";
    }
    if (data == "date")
    {
        return "19/10/2021";
    }
    if (data == "ZConnect")
    {
        return "Connected!";
    }
    if (data == "ip_video_stream")
    {
        // String address = SmartDoorBell::GetInstance()->GetOwnIPAddress().toString();
        // return address;
        return "/"; //yeah, baby.
    }
    if (data == "mat_s_state")
    {
        return "no alaram";
    }
    if (data == "water_level")
    {
        return "100";
    }
    if (data == "vibro_s_state")
    {
        return "no alaram";
    }
    if (data == "mot_s_state")
    {
        return "no alaram";
    }
    if (data == "current_weight")
    {       
        return String(SmartPetFeeder::GetInstance()->GetCurrentWeight() * 1000, 2);
    }
    return String();
}

String AsyncWebServerMaintainer::ButtonsProcessor(const String &data)
{    
    Serial.println("Entering ButtonsProcessor");

    Serial.println(data);

    if (data == "ZigBeeConnect")
    {
        Serial.println("ZigBeeConnect ---> OK");
        return String();
    }
    return String();
}


void AsyncWebServerMaintainer::PlantCarerZigBeeStart(AsyncWebServerRequest *request)
{
    SmartPlantCarer::GetInstance()->StartZigBee();

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PlantCarerWaterPumpControl(AsyncWebServerRequest *request)
{
    if(!request->hasArg("value"))
    {
        request->send(404);
        return;
    }
    String var = request->arg("value");
    int val = atoi(request->arg("value").c_str());

    SmartPlantCarer::GetInstance()->EnableWaterPump(val);

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PlantCarerWaterLockerControl(AsyncWebServerRequest *request)
{
    if(!request->hasArg("value") || !request->hasArg("id"))
    {
        request->send(404);
        return;
    }

    int id = atoi(request->arg("id").c_str());
    int val = atoi(request->arg("value").c_str());

    SmartPlantCarer::GetInstance()->EnableWaterLocker(id, val);

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    
    request->send(response);
}


void AsyncWebServerMaintainer::PlantCarerLedBulbControl(AsyncWebServerRequest *request)
{
    if(!request->hasArg("value"))
    {
        request->send(404);
        return;
    }

    int val = atoi(request->arg("value").c_str());

    SmartPlantCarer::GetInstance()->EnableLedBulbRelay(val);

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PlantCarerWaterLevelCheck(AsyncWebServerRequest *request)
{
    Serial.println("DEBUG: Check WATER LEVEL!");
    
    //1 ---> HIGH
    //0 ---> LOW
    if(!request->hasArg("level"))
    {
        request->send(404);
        return;
    }

    int level = atoi(request->arg("level").c_str());

    if (level == 0)
    {
        Serial.println("Check LOW side water level: " + SmartPlantCarer::GetInstance()->GetLowWaterLevelState());
    }
    if (level == 1)
    {
        Serial.println("Check HIGH side water level: " + SmartPlantCarer::GetInstance()->GetHightWaterLevelState());
    }
    else
    {
        Serial.printf("WARNING: Unknown level parameter: %d. HIGH side water level: %d \n", level, SmartPlantCarer::GetInstance()->GetHightWaterLevelState());
    }

    int currentLevel = level ? SmartPlantCarer::GetInstance()->GetHightWaterLevelState() : SmartPlantCarer::GetInstance()->GetLowWaterLevelState();
    

    static char json_response[1024];

    char * p = json_response;
    *p++ = '{';
    p+=sprintf(p, "\"waterLevel\":%d,", currentLevel);
    *p++ = '}';
    *p++ = 0;

    AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PlantCarerSoilHumidityLevelCheck(AsyncWebServerRequest *request)
{
    if(!request->hasArg("id"))
    {
        request->send(404);
        return;
    }

    int id = atoi(request->arg("id").c_str());

    int currentLevel = SmartPlantCarer::GetInstance()->GetSoilHumididtyState(id);

    static char json_response[1024];

    char * p = json_response;
    *p++ = '{';
    p+=sprintf(p, "\"soilHumidityLevel\":%d,", currentLevel);
    *p++ = '}';
    *p++ = 0;

    AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellZigBeeStart(AsyncWebServerRequest *request)
{
    
#ifndef DEBUG_MOBILE_APPLICATION_DEVELOPMENT

    SmartDoorBell::GetInstance()->StartZigBee();

#endif

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}



void AsyncWebServerMaintainer::DoorBellSetControl(AsyncWebServerRequest *request)
{
    if(!request->hasArg("value") || !request->hasArg("controlId"))
    {
        request->send(404);
        return;
    }

    int id = atoi(request->arg("controlId").c_str());
    int val = atoi(request->arg("value").c_str());

    if (!IsValidDoorBellAttribute(id))
    {
        static char json_response[1024];

        char * p = json_response;
        *p++ = '{';
        p+=sprintf(p, "\"errorDescription\": unhandled attribute id!");
        *p++ = '}';
        *p++ = 0;

        AsyncWebServerResponse * response = request->beginResponse(404, "application/json", json_response);
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    }

#ifndef DEBUG_MOBILE_APPLICATION_DEVELOPMENT

    SmartDoorBell::GetInstance()->AttributeChangedProcessor(id, val, AttributeChangeSource::SYSTEM_INTERNAL);

#endif

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellCheckButtonAlarm(AsyncWebServerRequest *request)
{
    bool alarm = SmartDoorBell::GetInstance()->GetButtonTriggered();

    static char json_response[1024];

    char * p = json_response;
    *p++ = '{';
    p+=sprintf(p, "\"buttonTriggered\":%d,", alarm);
    *p++ = '}';
    *p++ = 0;

    AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellCheckMicrophoneAlarm(AsyncWebServerRequest *request)
{
    bool alarm = SmartDoorBell::GetInstance()->GetMicrophoneTriggered();

    static char json_response[1024];

    char * p = json_response;
    *p++ = '{';
    p+=sprintf(p, "\"microPhoneTriggered\":%d,", alarm);
    *p++ = '}';
    *p++ = 0;

    AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellCheckBoxVibrationAlarm(AsyncWebServerRequest *request)
{
    bool alarm = SmartDoorBell::GetInstance()->GetBoxVibrationTriggered();

    static char json_response[1024];

    char * p = json_response;
    *p++ = '{';
    p+=sprintf(p, "\"boxVibrationTriggered\":%d,", alarm);
    *p++ = '}';
    *p++ = 0;

    AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellCheckAdditionalSensorsMapper(AsyncWebServerRequest *request)
{
    uint32_t sensorsMapper = SmartDoorBell::GetInstance()->GetAdditionalSensorMaper();

    static char json_response[1024];

    char * p = json_response;
    *p++ = '{';
    p+=sprintf(p, "\"sensorsMapper\":%d,", sensorsMapper);
    *p++ = '}';
    *p++ = 0;

    AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellGetDeviceStatus(AsyncWebServerRequest *request)
{
    //TODO:: Implement it!
}


void AsyncWebServerMaintainer::DoorBellGetSoundTrack(AsyncWebServerRequest *request)
{
    //TODO:: Implement it!    
}


bool isValidSpeakerCommand(int commandId)
{
    return (commandId == static_cast<int>(SPEAKER_COMMANDS::STOP_RESUME) ||
            commandId == static_cast<int>(SPEAKER_COMMANDS::SET_LEVEL) ||
            commandId == static_cast<int>(SPEAKER_COMMANDS::PLAY_NEXT_SONG));
}





void AsyncSetSoundControl(void *parameter)
{
    SoundControlCommand *command = (SoundControlCommand*) parameter;

    if (command->_id == static_cast<int>(SPEAKER_COMMANDS::STOP_RESUME)) //sound control
    {
        SpeakerMaintainer::GetInstance()->MusicFlowControl(command->_val);
    }
    else if (command->_id == static_cast<int>(SPEAKER_COMMANDS::SET_LEVEL)) //sound resume 
    {
        SpeakerMaintainer::GetInstance()->SetLevel(command->_val);
    }
    else if (command->_id == static_cast<int>(SPEAKER_COMMANDS::PLAY_NEXT_SONG)) //sound resume 
    {
        //Under TEST!
        SpeakerMaintainer::GetInstance()->SetNextSong(NEXT_SONG, true);
    }

    vTaskDelete(NULL);
};


void AsyncWebServerMaintainer::DoorBellSoundControl(AsyncWebServerRequest *request)
{
    if(!request->hasArg("value") || !request->hasArg("controlId"))
    {
        request->send(404);
        return;
    }

    int id = atoi(request->arg("controlId").c_str());
    int val = atoi(request->arg("value").c_str());


    if(!isValidSpeakerCommand(id))
    {
        static char json_response[1024];

        char * p = json_response;
        *p++ = '{';
        p+=sprintf(p, "\"errorDescription\": unhandled controlId!");
        *p++ = '}';
        *p++ = 0;

        AsyncWebServerResponse * response = request->beginResponse(404, "application/json", json_response);
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    }

    SpeakerMaintainer::GetInstance()->AsyncSoundControl(SoundControlCommand{id, val});

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");    
    request->send(response);
}


void  AsyncWebServerMaintainer::NetworkSetupControl(AsyncWebServerRequest *request)
{
    Serial.println("Nothing to do");
}



void AsyncWebServerMaintainer::DoorBellLedTest(AsyncWebServerRequest *request)
{
    TesterOfTheFeatures::GetInstance()->IncrementLedTestPhase();
    
    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellScalesTare(AsyncWebServerRequest *request)
{
    ScalesMaintainer::GetInstance()->AsyncTare();
    
    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellScalesMeasure(AsyncWebServerRequest *request)
{
    ScalesMaintainer::GetInstance()->AsyncGetWeight();
    
    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellToggleClamp(AsyncWebServerRequest *request)
{
    //DEBUG!!!!    
    bool currentState = digitalRead(CLAMP_PIO); 
    Serial.printf("Current state: %d", currentState);
    digitalWrite(CLAMP_PIO, !currentState);

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::DoorBellToggleClampTimed(AsyncWebServerRequest *request)
{
    if(!request->hasArg("timeMs"))
    {
        request->send(404);
        return;
    }

    int msToBeOpened = atoi(request->arg("timeMs").c_str());

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);

    ClampMaintainer::GetInstance()->Feed(msToBeOpened);
}


void AsyncWebServerMaintainer::PetFeederScalesTare(AsyncWebServerRequest *request)
{
    ScalesMaintainer::GetInstance()->AsyncTare();
    
    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PetFeederScalesMeasure(AsyncWebServerRequest *request)
{
    ScalesMaintainer::GetInstance()->AsyncGetWeight();
    
    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PetFeederToggleClamp(AsyncWebServerRequest *request)
{
    //DEBUG!!!!    
    bool currentState = digitalRead(CLAMP_PIO); 
    Serial.printf("Current state: %d", currentState);
    digitalWrite(CLAMP_PIO, !currentState);

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PetFeederToggleClampTimed(AsyncWebServerRequest *request)
{
    if(!request->hasArg("timeMs"))
    {
        request->send(404);
        return;
    }

    int msToBeOpened = atoi(request->arg("timeMs").c_str());

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);

    ClampMaintainer::GetInstance()->Feed(msToBeOpened);
}


void AsyncWebServerMaintainer::PetFeederScalesCalibrate(AsyncWebServerRequest *request)
{
    ScalesMaintainer::GetInstance()->AsyncCalibrateMeasure();
    
    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PetFeederSetScale(AsyncWebServerRequest *request)
{
    if(!request->hasArg("scalesFactor"))
    {
        request->send(404);
        return;
    }

    int32_t scalesFactor = atoi(request->arg("scalesFactor").c_str());

    ScalesMaintainer::GetInstance()->AsyncSetScalesFactor(scalesFactor);

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PetFeederDirectMeasure(AsyncWebServerRequest *request)
{
    if(!request->hasArg("iterationsNumber"))
    {
        request->send(404);
        return;
    }

    int32_t iterationsNumber = atoi(request->arg("iterationsNumber").c_str());

    float result = ScalesMaintainer::GetInstance()->DirectGetWeight(iterationsNumber);

    SmartPetFeeder::GetInstance()->SetCurrentWeight(result);

    static char json_response[1024];

    char * p = json_response;
    *p++ = '{';
    p+=sprintf(p, "\"weight\":%f,", result);
    *p++ = '}';
    *p++ = 0;

    AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);

}


void AsyncWebServerMaintainer::PetFeederFeedThePet(AsyncWebServerRequest *request)
{
    if(!request->hasArg("foodWeightGramms"))
    {
        request->send(404);
        return;
    }

    int32_t foodWeight = atoi(request->arg("foodWeightGramms").c_str());

    SmartPetFeeder::GetInstance()->FeedThePetAsync(foodWeight);
    
    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::PetFeederSoundControl(AsyncWebServerRequest *request)
{
        if(!request->hasArg("value") || !request->hasArg("controlId"))
    {
        request->send(404);
        return;
    }

    int id = atoi(request->arg("controlId").c_str());
    int val = atoi(request->arg("value").c_str());


    if(!isValidSpeakerCommand(id))
    {
        static char json_response[1024];

        char * p = json_response;
        *p++ = '{';
        p+=sprintf(p, "\"errorDescription\": unhandled controlId!");
        *p++ = '}';
        *p++ = 0;

        AsyncWebServerResponse * response = request->beginResponse(404, "application/json", json_response);
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    }

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");

    SoundControlCommand command{id, val};

    request->send(response);

#ifndef DEBUG_MOBILE_APPLICATION_DEVELOPMENT

    if (id == static_cast<int>(SPEAKER_COMMANDS::STOP_RESUME)) //sound control
    {
        SpeakerMaintainer::GetInstance()->MusicFlowControl(val);
    }
    else if (id == static_cast<int>(SPEAKER_COMMANDS::SET_LEVEL)) //sound resume 
    {
        SpeakerMaintainer::GetInstance()->SetLevel(val);
    }
    else if (id == static_cast<int>(SPEAKER_COMMANDS::PLAY_NEXT_SONG)) //sound resume 
    {
        //Under TEST!
        SpeakerMaintainer::GetInstance()->SetNextSong(NEXT_SONG, true);
    }

#endif
}


void AsyncWebServerMaintainer::PetFeederGetSoundTrack(AsyncWebServerRequest *request)
{
    //TODO:: Implement it!   
}


void AsyncWebServerMaintainer::PetFeederLedTest(AsyncWebServerRequest *request)
{
    TesterOfTheFeatures::GetInstance()->IncrementLedTestPhase();
    
    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void AsyncWebServerMaintainer::InitializeInitialWebServer()
{
    
    //Main
    _asyncWebServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                       {
                           //request->send(SD, "/index.html", String(), false, std::bind(&AsyncWebServerMaintainer::Processor, this, std::placeholders::_1));
                           request->send(SD, "/AccessPointInterface/index.html", String(), false, VariablesProcessor);
                       });

    // Route to load style.css file
    _asyncWebServer.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                       { request->send(SD, "/AccessPointInterface/style.css", "text/css"); });


    //_asyncWebServer.on("/networkCredentials/formData", HTTP_POST, PlantCarerWaterLockerControl);

    

    _networkCredentialsHandler = 
        new AsyncCallbackJsonWebHandler("/networkCredentials/formData", [&](AsyncWebServerRequest *request, JsonVariant &json) 
        {
            StaticJsonDocument<200> data;

            Serial.println("Entering FormHandler.");

            if (json.is<JsonArray>())
            {
                data = json.as<JsonArray>();
            }
            else if (json.is<JsonObject>())
            {
                data = json.as<JsonObject>();
            }

            //parse json here
            const char *ssid = data["ssid"];
            const char *password = data["password"];

            Serial.printf("Network Credentials. SSID: %s PASSWORD: %s\n", ssid, password);

            String response;
            serializeJson(data, response);
            request->send(200, "application/json", response);
            Serial.println(response);

            //CONTINUE HERE!
            //Create ASYNC Call to the Handler ---> Since Handler is not Async And may block the server.
            _networkSelectedCallBack(ssid, password);
            
        });
    
    _asyncWebServer.addHandler(_networkCredentialsHandler);
}


void AsyncWebServerMaintainer::InitializeMainStageWebServer()
{
    //AwsTemplateProcessor pr = [this](const String& data) -> String { return Processor(data);};


    //Plant Carer
    _asyncWebServer.on("/plantCarer/waterPumpState", HTTP_POST, PlantCarerWaterPumpControl);
    _asyncWebServer.on("/plantCarer/waterLocker", HTTP_POST, PlantCarerWaterLockerControl);
    _asyncWebServer.on("/plantCarer/ledBulb", HTTP_POST, PlantCarerLedBulbControl);
    _asyncWebServer.on("/plantCarer/waterLevel", HTTP_GET, PlantCarerWaterLevelCheck);
    _asyncWebServer.on("/plantCarer/soilHumidityLevel", HTTP_GET, PlantCarerSoilHumidityLevelCheck);
    _asyncWebServer.on("/plantCarer/startZigBee", HTTP_POST, PlantCarerZigBeeStart);

    //Smart-Door-Bell
    _asyncWebServer.on("/doorBell/deviceStatus", HTTP_GET, DoorBellGetDeviceStatus);

    _asyncWebServer.on("/doorBell/startZigBee", HTTP_POST, DoorBellZigBeeStart);
    _asyncWebServer.on("/doorBell/setControl", HTTP_POST, DoorBellSetControl);
    _asyncWebServer.on("/doorBell/checkButtonAlaram", HTTP_GET, DoorBellCheckButtonAlarm);
    _asyncWebServer.on("/doorBell/checkMicroPhoneAlaram", HTTP_GET, DoorBellCheckMicrophoneAlarm);
    _asyncWebServer.on("/doorBell/checkBoxVibrationAlarm", HTTP_GET, DoorBellCheckBoxVibrationAlarm);
    _asyncWebServer.on("/doorBell/getAdditionalSensorsMapper", HTTP_GET, DoorBellCheckAdditionalSensorsMapper);

    _asyncWebServer.on("/doorBell/soundControl", HTTP_POST, DoorBellSoundControl);
    _asyncWebServer.on("/doorBell/getCurrentTrack", HTTP_GET, DoorBellGetSoundTrack);

    _asyncWebServer.on("/doorBell/ledTest", HTTP_GET, DoorBellLedTest);

#ifdef BUILD_FOR_DOOR_BELL    
    _asyncWebServer.on("/doorBell/scalesTare", HTTP_GET, DoorBellScalesTare);
    _asyncWebServer.on("/doorBell/scalesMeasure", HTTP_GET, DoorBellScalesMeasure);
#endif    
    _asyncWebServer.on("/doorBell/toggleClamp", HTTP_GET, DoorBellToggleClamp);
    _asyncWebServer.on("/doorBell/toggleClampTimed", HTTP_GET, DoorBellToggleClampTimed);

    //Pet Feeder
    _asyncWebServer.on("/petFeeder/scalesTare", HTTP_GET, PetFeederScalesTare);
    _asyncWebServer.on("/petFeeder/scalesMeasure", HTTP_GET, PetFeederScalesMeasure);   
    _asyncWebServer.on("/petFeeder/toggleClamp", HTTP_GET, PetFeederToggleClamp);
    _asyncWebServer.on("/petFeeder/toggleClampTimed", HTTP_GET, PetFeederToggleClampTimed);
    _asyncWebServer.on("/petFeeder/scalesCalibrate", HTTP_GET, PetFeederScalesCalibrate);
    _asyncWebServer.on("/petFeeder/setScale", HTTP_GET, PetFeederSetScale);
    _asyncWebServer.on("/petFeeder/scalesDirectMeasure", HTTP_GET, PetFeederDirectMeasure);
    _asyncWebServer.on("/petFeeder/feedThePet", HTTP_GET, PetFeederFeedThePet);

    _asyncWebServer.on("/petFeeder/soundControl", HTTP_POST, PetFeederSoundControl);
    _asyncWebServer.on("/petFeeder/getCurrentTrack", HTTP_GET, PetFeederGetSoundTrack);
    _asyncWebServer.on("/petFeeder/ledTest", HTTP_GET, PetFeederLedTest);


    //TODO:: Add Web GUI Enable-Disable.

#ifndef DEBUG_MOBILE_APPLICATION_DEVELOPMENT


    // _asyncWebServer.serveStatic("/", SD, "index.html").setCacheControl("max-age=600").setTemplateProcessor(VariablesProcessor);
    // _asyncWebServer.serveStatic("/style.css", SD, "style.css").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/MainLogo.png", SD, "/Assets/MainLogo.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/CornerRounder.png", SD, "/Assets/CornerRounder.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/play.png", SD, "/Assets/play.png").setCacheControl("max-age=600");

    //Main
    _asyncWebServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                       {
                           //request->send(SD, "/index.html", String(), false, std::bind(&AsyncWebServerMaintainer::Processor, this, std::placeholders::_1));
                           request->send(SD, "/index.html", String(), false, VariablesProcessor);
                       });

    // Route to load style.css file
    _asyncWebServer.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                       { request->send(SD, "/style.css", "text/css"); });

    // //Images Web (Common)
    // _asyncWebServer.on("/MainLogo.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/MainLogo.png", "image/png"); });

    // _asyncWebServer.on("/CornerRounder.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/CornerRounder.png", "image/png"); });

    // _asyncWebServer.on("/play.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/play.png", "image/png"); });

    

    //Images Web (Plant Carer)
#ifdef BUILD_FOR_PLANT_CARER


    _asyncWebServer.serveStatic("/bulbs.png", SD, "/Assets/bulbs.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/custom.png", SD, "/Assets/custom.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/ferns.png", SD, "/Assets/ferns.png").setCacheControl("max-age=600");
    
    _asyncWebServer.serveStatic("/fruit.png", SD, "/Assets/fruit.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/gorsh013.png", SD, "/Assets/gorsh013.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/modebl.png", SD, "/Assets/modebl.png").setCacheControl("max-age=600");

    _asyncWebServer.serveStatic("/model.png", SD, "/Assets/model.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/palm.png", SD, "/Assets/palm.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/succulents.png", SD, "/Assets/succulents.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/tropl.png", SD, "/Assets/tropl.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/tropbl.png", SD, "/Assets/tropbl.png").setCacheControl("max-age=600");
    _asyncWebServer.serveStatic("/100.png", SD, "/Assets/100.png").setCacheControl("max-age=600");

    // _asyncWebServer.on("/bulbs.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/bulbs.png", "image/png"); });

    // _asyncWebServer.on("/custom.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/custom.png", "image/png"); });

    // _asyncWebServer.on("/ferns.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/ferns.png", "image/png"); });

    // _asyncWebServer.on("/fruit.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/fruit.png", "image/png"); });

    // _asyncWebServer.on("/gorsh013.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/gorsh013.png", "image/png"); });

    // _asyncWebServer.on("/modebl.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/modebl.png", "image/png"); });

    // _asyncWebServer.on("/model.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/model.png", "image/png"); });

    // _asyncWebServer.on("/palm.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/palm.png", "image/png"); });

    // _asyncWebServer.on("/succulents.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/succulents.png", "image/png"); });

    // _asyncWebServer.on("/tropbl.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/tropbl.png", "image/png"); });

    // _asyncWebServer.on("/tropbl.png", HTTP_GET, [](AsyncWebServerRequest *request)
    //                    { request->send(SD, "/Assets/tropbl.png", "image/png"); });

#endif

    //Buttons (Smart Door Bell Main Control)

    _asyncWebServer.on("/buttonUpdateState", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
        String inputMessage1;
        String inputMessage2;
        // GET input1 value on <ESP_IP>/buttonUpdateState?output=<inputMessage1>&state=<inputMessage2>
        if (request->hasParam(UI_CONTROL_ELEMENT_ID) && request->hasParam(UI_CONTROL_ELEMNT_STATE)) 
        {
            inputMessage1 = request->getParam(UI_CONTROL_ELEMENT_ID)->value();
            inputMessage2 = request->getParam(UI_CONTROL_ELEMNT_STATE)->value();
        //digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
        }
        else 
        {
            inputMessage1 = "No message sent";
            inputMessage2 = "No message sent";
        }
        Serial.print("Button: ");
        Serial.print(inputMessage1);
        Serial.print(" - Set to: ");
        Serial.println(inputMessage2);
        
        if (inputMessage1.toInt() == 1)
        {
            //ScalesMaintainer::GetInstance()->AsyncCalibrateMeasure();
        }
        
        request->send(200, "text/plain", "OK");
    });



    _asyncWebServer.on("/ZigBeeConnect", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
        String inputMessage1;
        // GET input1 value on <ESP_IP>/buttonUpdateState?connect=<inputMessage1>
        if (request->hasParam(UI_CONTROL_ELEMENT_ID)) 
        {
            inputMessage1 = request->getParam(UI_CONTROL_CONNECT_BUTTON_CONNECT)->value();
        }
        else 
        {
            inputMessage1 = "No message sent";
        }
        Serial.print("Connect Set to: ");
        Serial.println(inputMessage1);
        

        //ScalesMaintainer::GetInstance()->AsyncCalibrateMeasure();
        
        
        request->send(200, "text/plain", "OK");
    });

#endif

}


AsyncWebServerMaintainer *AsyncWebServerMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new AsyncWebServerMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


AsyncWebServerMaintainer *AsyncWebServerMaintainer::pinstance_{nullptr};
SemaphoreHandle_t AsyncWebServerMaintainer::mutex_{xSemaphoreCreateMutex()};