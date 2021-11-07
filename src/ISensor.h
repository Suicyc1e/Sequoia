#ifndef ISENSOR_H
#define ISENSOR_H

#include <string>

enum class SensorRole 
{
    ParametersMeasurement = 0,
    Alarm = 1
};

enum class SensorState
{
    Enabled = 0,
    Disabled = 1
};

enum class SensorType
{
    AirTemperatureHumidity = 0,
    SoilHumidity = 1,
    SoundAlarm = 2,
    FireDetector = 3,
    ButtonTrigger = 4,
    FloorPressureSensor = 5,
    VoiceCommandsDetector = 6
};

template < typename T >
struct ISensor
{
/* data */
public:

    virtual std::string GetHumanReadableName(); //ex. "Temperature/Humidity Sensor"
    virtual std::string GetHumanReadableId(); //ex. SHT21 
    virtual std::string GetHumanReadableFunction(); //ex. "Air condition observation"

    virtual SensorRole GetSensorRole(); //ex. SensorRole::ParametersMeasurement
    virtual SensorState GetSensorState() ; //ex. SensorState::Enabled
    virtual SensorType GetSensorType(); //ex. SensorType::AirTemperatureHimidity

    virtual <T> GetData(); //ex. 3

};




#endif