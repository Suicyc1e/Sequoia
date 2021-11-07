#include <SmartPlantCarer.h>
#include <GlobalPresets.h>
#include <SDCardMaintainer.h>
#include <LEDMaintainer.h>
#include <TesterOfTheFeatures.h>

#define WATER_PUMP_PIO 13

#define WATER_VALVE_1_PIO 14
#define WATER_VALVE_2_PIO 27
#define WATER_VALVE_3_PIO 26
#define WATER_VALVE_4_PIO 25

#define WATER_HUMIDITY_LEVEL_PIO 21

#define WATER_LEVEL_LOW_PIO 32
#define WATER_LEVEL_HIGH_PIO 33

#define LED_LAMP_RELAY_PIO 22




SmartPlantCarer::SmartPlantCarer() :
    _waterPump_PIO(WATER_PUMP_PIO),
    _waterValve_1_PIO(WATER_VALVE_1_PIO),
    _waterValve_2_PIO(WATER_VALVE_2_PIO),
    _waterValve_3_PIO(WATER_VALVE_3_PIO),
    _waterValve_4_PIO(WATER_VALVE_4_PIO),
    _soilHumiditySensor_1_PIO(WATER_HUMIDITY_LEVEL_PIO),
    _waterLevelLow_PIO(WATER_LEVEL_LOW_PIO),
    _waterLevelHigh_PIO(WATER_LEVEL_HIGH_PIO),
    _ledLampRelay_PIO(LED_LAMP_RELAY_PIO),
    _zigBeeCommandLineProcessor([&](ZigBeeMessage_AttributeChanged msg){OnAttributeChangedHandler(msg);} ,
    [&](ZigBeeMessage_NwkFailed msg){OnNwkFailedHandler(msg);},
    [&](ZigBeeMessage_NwkSuccess msg){OnNwkSuccededHandler(msg);})
{
    _zigBeeCommandLineProcessor.SetPlantCarerCommandsCallback([&](SmartPlantCarerCommand command){OnZigBeeCommandReceivedHandler(command);});
    
#ifndef DEBUG_MOBILE_APPLICATION_DEVELOPMENT

    HardwareInit();
    InitServices();

#endif

}


void SmartPlantCarer::InitServices()
{
    _zigBeeCommandLineProcessor.ExecuteStartUpProcedures();
    SDCardMaintainer::GetInstance();
    LEDMaintainer::GetInstance();
    TesterOfTheFeatures::GetInstance();    
}

void SmartPlantCarer::AttributeChangedProcessor(uint32_t attributeID, uint32_t attributeValue, AttributeChangeSource source)
{
    Serial.println("Entering AttributeChangedProcessor");
    Serial.printf("Attribte id: %d, Attribute value: %d \n", attributeID, attributeValue);

}


void SmartPlantCarer::OnZigBeeCommandReceivedHandler(SmartPlantCarerCommand command)
{
    Serial.println("Entering OnZigBeeCommandReceivedHandler");
    Serial.printf("ZigBee command received: %s\n", command.GetRawCommand().c_str());
}


void SmartPlantCarer::OnAttributeChangedHandler(ZigBeeMessage_AttributeChanged msg)
{
    AttributeChangedProcessor(msg.GetAttributeId(), msg.GetNewAttributeValue(), AttributeChangeSource::ZIGBEE_SIDE);
}


void SmartPlantCarer::OnNwkSuccededHandler(ZigBeeMessage_NwkSuccess msg)
{
    Serial.printf("Connected to ZigBee Network! Status: %d", msg.GetStatus());
    _zigBeeIsConnected = true;

    //TODO:: Stop LED Blinking HERE!
}


void SmartPlantCarer::OnNwkFailedHandler(ZigBeeMessage_NwkFailed msg)
{
    Serial.printf("DISCONNECTED from ZigBee Network! Status: %d", msg.GetStatus());
    _zigBeeIsConnected = false;
}


void SmartPlantCarer::EnableWaterPump(bool enable)
{
    digitalWrite(_waterPump_PIO, enable);

}

void SmartPlantCarer::EnableWaterLocker(uint8_t id, bool enable)
{
    //DEBUG! Remove it after use!
    Serial.println("TESTING WATER LOCKERS!");
    Serial.printf("Water Locker ID: %d water locker enable state: %d\n", id, enable);
    
    switch (id)
    {
    case 1: 
        digitalWrite(_waterValve_1_PIO, enable);
        break;

    case 2:
        digitalWrite(_waterValve_2_PIO, enable);
        break;

    case 3:
        digitalWrite(_waterValve_3_PIO, enable);

    case 4:
        digitalWrite(_waterValve_4_PIO, enable);

    default:
        Serial.println("WARING! Unknown PIO ID given!" + id);
        break;
    }
}

bool SmartPlantCarer::GetHightWaterLevelState()
{
    return digitalRead(_waterLevelHigh_PIO);
}

bool SmartPlantCarer::GetLowWaterLevelState()
{
    return digitalRead(_waterLevelLow_PIO);
}


bool SmartPlantCarer::GetSoilHumididtyState(uint8_t id)
{
    return digitalRead(_soilHumiditySensor_1_PIO);
}


void SmartPlantCarer::EnableLedBulbRelay(bool enable)
{
    Serial.println("TESTING LED BULB!");
    Serial.printf("LED Bulb enable state: %d", enable);
    digitalWrite(_ledLampRelay_PIO, enable);
}


void SmartPlantCarer::StartZigBee()
{
    _zigBeeCommandLineProcessor.SendZigBeeStartUp();
}


SmartPlantCarer::~SmartPlantCarer()
{


}

void SmartPlantCarer::SetHardwarePresets()
{
    //Invert all the PIO State since for the Operational Amplifiers inverse logic has been implemented: "HIGH" means "OFF", "LOW" means "ON";
    digitalWrite(_waterPump_PIO, 1);


    digitalWrite(_waterValve_1_PIO, 1);
    digitalWrite(_waterValve_2_PIO, 1);
    digitalWrite(_waterValve_3_PIO, 1);
    digitalWrite(_waterValve_4_PIO, 1);

    digitalWrite(_ledLampRelay_PIO, 1);
}


void SmartPlantCarer::HardwareInit()
{
    pinMode(_waterPump_PIO, GPIO_MODE_OUTPUT);

    pinMode(_waterValve_1_PIO, GPIO_MODE_OUTPUT);
    pinMode(_waterValve_2_PIO, GPIO_MODE_OUTPUT);
    pinMode(_waterValve_3_PIO, GPIO_MODE_OUTPUT);
    pinMode(_waterValve_4_PIO, GPIO_MODE_OUTPUT);

    pinMode(_soilHumiditySensor_1_PIO, INPUT_PULLUP);

    pinMode(_waterLevelLow_PIO, INPUT_PULLUP);
    pinMode(_waterLevelHigh_PIO, INPUT_PULLUP);

    pinMode(_ledLampRelay_PIO, GPIO_MODE_OUTPUT);

    SetHardwarePresets(); 
}


SmartPlantCarer *SmartPlantCarer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new SmartPlantCarer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


SmartPlantCarer *SmartPlantCarer::pinstance_{nullptr};
SemaphoreHandle_t SmartPlantCarer::mutex_{xSemaphoreCreateMutex()};
