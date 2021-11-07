#ifndef SENSORS_MAINTAINER_H
#define SENSORS_MAINTAINER_H

#include <Arduino.h>
#include <Sensor.h>

class SensorsMaintainer
{
//Singletoning)))
private:

    static SensorsMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;

protected:

    SensorsMaintainer(/* args */);
    ~SensorsMaintainer();

private:

    Sensor _fireSensor;
    Sensor _vibrationSensor;
    Sensor _movementSensor;

private:

    TaskHandle_t _fireSensorHandler;

private:

    static void FireSensorPollerTask(void *taskStartParameters);

private:

    void InitializeInterrupts();
    void InitializeAnalogSensors();

    static void IRAM_ATTR FireSensorReaderISR();
    static void IRAM_ATTR VibrationSensorReaderISR();
    static void IRAM_ATTR MovementSensorReaderISR();
    static uint32_t TimesFireSensorTriggered;
    static uint32_t TimesVibrationSensorTriggered;
    static uint32_t TimesMovementSensorTriggered;


public:

    void FireSensorInterruptHandler();
    void VibrationSensorInterruptHandler();
    void MovementSensorInterruptHandler();

private:

    bool CreateRTOSFireSensorInterruptCallbackTask();
    static void FireSensorInterruptCallbackTaskStart(void *taskStartParameters);

    bool CreateRTOSVibrationSensorInterruptCallbackTask();
    static void VibrationSensorInterruptCallbackTaskStart(void *taskStartParameters);

    bool CreateRTOSMovementSensorInterruptCallbackTask();
    static void MovementSensorInterruptCallbackTaskStart(void *taskStartParameters);

public:

    void FireSensorCallBackTaskHandler();
    void VibrationSensorCallBackTaskHandler();
    void MovementSensorCallBackTaskHandler();

public:

    SensorsMaintainer(SensorsMaintainer &other) = delete;
    void operator=(const SensorsMaintainer &) = delete;

    static SensorsMaintainer *GetInstance();
};





#endif