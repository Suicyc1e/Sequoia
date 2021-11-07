#include <SensorsMaintainer.h>
#include <PIOMapper.h>
#include <TesterOfTheFeatures.h>
#include <GlobalPresets.h>
#include <SmartDoorBell.h>

//TODO--> True--> to "Analog" type of sensor...
SensorsMaintainer::SensorsMaintainer():
    _fireSensor(USER_FIRE_SENSOR, "FireSensor", true),
    _vibrationSensor(USER_VIBRATION_SENSOR, "VibrationSensor"),
    _movementSensor(USER_MOVEMENT_SENSOR, "MovementSensor")
{
    //Set Interrupt Handler HERE!
    InitializeInterrupts();
    InitializeAnalogSensors();
}

void SensorsMaintainer::InitializeInterrupts()
{
    //attachInterrupt(_fireSensor.GetGPIONumber(), FireSensorReaderISR, RISING);
    attachInterrupt(_vibrationSensor.GetGPIONumber(), VibrationSensorReaderISR, RISING);
    attachInterrupt(_movementSensor.GetGPIONumber(), MovementSensorReaderISR, RISING);
}


void SensorsMaintainer::FireSensorPollerTask(void *taskStartParameters)
{
    uint16_t threshold = FIRE_DETECTION_THRESHOLD;
    while (1)
    {
        uint16_t value =  analogRead(USER_FIRE_SENSOR);
        Serial.printf("Fire Sensor Value: %d\n", value);
        if (value < threshold)
        {
            //Async Call Here!
            SensorsMaintainer::GetInstance()->FireSensorInterruptHandler();
        }
        vTaskDelay(pdMS_TO_TICKS(FIRE_POLL_RATE_MS));
    }

    vTaskDelete(NULL); //How can we get here? =)))
}


void SensorsMaintainer::InitializeAnalogSensors()
{
    Serial.println("Starting Fie Sensor Poller Task");
    xTaskCreate(FireSensorPollerTask, "FireSensorPollerTask", 5000, NULL, tskNO_AFFINITY, NULL);
}

void SensorsMaintainer::FireSensorReaderISR()
{   
    //debounce
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > BUTTON_DEBOUNCE_INTERVAL_MS)
    {
        ++TimesFireSensorTriggered;
        Serial.printf("Fire Sensor Triggered: %d\n", TimesFireSensorTriggered);

        SensorsMaintainer::GetInstance()->FireSensorInterruptHandler(); 
    }
    last_interrupt_time = interrupt_time; 
}


void SensorsMaintainer::VibrationSensorReaderISR()
{   
    //debounce
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > BUTTON_DEBOUNCE_INTERVAL_MS)
    {
        ++TimesVibrationSensorTriggered;
        Serial.printf("Vibration Sensor Triggered: %d\n", TimesVibrationSensorTriggered);

        SensorsMaintainer::GetInstance()->VibrationSensorInterruptHandler(); 
    }
    last_interrupt_time = interrupt_time; 
}


void SensorsMaintainer::MovementSensorReaderISR()
{   
    //debounce
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > BUTTON_DEBOUNCE_INTERVAL_MS)
    {
        ++TimesMovementSensorTriggered;
        Serial.printf("Movement Sensor Triggered: %d\n", TimesMovementSensorTriggered);

        SensorsMaintainer::GetInstance()->MovementSensorInterruptHandler(); 
    }
    last_interrupt_time = interrupt_time; 
}



bool SensorsMaintainer::CreateRTOSFireSensorInterruptCallbackTask()
{
    xTaskCreate(FireSensorInterruptCallbackTaskStart, "FireSensorTriggeredTask", 5000, this, tskNO_AFFINITY, NULL);
    return true;
}


bool SensorsMaintainer::CreateRTOSVibrationSensorInterruptCallbackTask()
{
    xTaskCreate(VibrationSensorInterruptCallbackTaskStart, "VibrationSensorTriggeredTask", 5000, this, tskNO_AFFINITY, NULL);
    return true;
}


bool SensorsMaintainer::CreateRTOSMovementSensorInterruptCallbackTask()
{
    xTaskCreate(MovementSensorInterruptCallbackTaskStart, "MovementSensorTriggeredTask", 5000, this, tskNO_AFFINITY, NULL);
    return true;
}


void SensorsMaintainer::FireSensorInterruptCallbackTaskStart(void *taskStartParameters)
{
    SensorsMaintainer *moduleObject = (SensorsMaintainer *)taskStartParameters;
    moduleObject->FireSensorCallBackTaskHandler();

    vTaskDelete(NULL);
}


void SensorsMaintainer::VibrationSensorInterruptCallbackTaskStart(void *taskStartParameters)
{
    SensorsMaintainer *moduleObject = (SensorsMaintainer *)taskStartParameters;
    moduleObject->VibrationSensorCallBackTaskHandler();

    vTaskDelete(NULL);
}


void SensorsMaintainer::MovementSensorInterruptCallbackTaskStart(void *taskStartParameters)
{
    SensorsMaintainer *moduleObject = (SensorsMaintainer *)taskStartParameters;
    moduleObject->MovementSensorCallBackTaskHandler();

    vTaskDelete(NULL);
}


void SensorsMaintainer::FireSensorCallBackTaskHandler()
{
    Serial.println("FireSensorCallBackTaskHandler!");
    //Some ASYNC Job can be called here.
    //TesterOfTheFeatures::GetInstance()->IncrementLedTestPhase();
    //DO FIRE SENSOR JOB HERE!
    uint32_t currentState = SmartDoorBell::GetInstance()->GetAdditionalSensorMaper();

    SmartDoorBell::GetInstance()->AttributeChangedProcessor
    (
        static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::ADDITIONAL_MAPPER_STATE), 
        (currentState | 0x02), AttributeChangeSource::SYSTEM_INTERNAL
    );
}


void SensorsMaintainer::VibrationSensorCallBackTaskHandler()
{
    Serial.println("VibrationSensorCallBackTaskHandler!");
    //Some ASYNC Job can be called here.
    //TesterOfTheFeatures::GetInstance()->IncrementLedTestPhase();
    //DO VIBRATION SENSOR JOB HERE!
    SmartDoorBell::GetInstance()->AttributeChangedProcessor
    (
        static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::BOX_VIBRATION_TRIGGERED), 
        1, AttributeChangeSource::SYSTEM_INTERNAL
    );
}



void SensorsMaintainer::MovementSensorCallBackTaskHandler()
{
    Serial.println("MovementSensorCallBackTaskHandler!");
    //Some ASYNC Job can be called here.
    //TesterOfTheFeatures::GetInstance()->IncrementLedTestPhase();
    //DO MOVEMENT SENSOR JOB HERE!
    uint32_t currentState = SmartDoorBell::GetInstance()->GetAdditionalSensorMaper();

    SmartDoorBell::GetInstance()->AttributeChangedProcessor
    (
        static_cast<uint32_t>(ZigBeeAttributeMapperDoorBell::ADDITIONAL_MAPPER_STATE), 
        (currentState | 0x01), AttributeChangeSource::SYSTEM_INTERNAL
    );
}


void SensorsMaintainer::FireSensorInterruptHandler()
{
    Serial.println("Entering FireSensorReaderISR Zone");
    CreateRTOSFireSensorInterruptCallbackTask();
}


void SensorsMaintainer::VibrationSensorInterruptHandler()
{
    Serial.println("Entering VibrationSensorReaderISR Zone");
    CreateRTOSVibrationSensorInterruptCallbackTask();
}


void SensorsMaintainer::MovementSensorInterruptHandler()
{
    Serial.println("Entering MovementSensorReaderISR Zone");
    CreateRTOSMovementSensorInterruptCallbackTask();
}


SensorsMaintainer::~SensorsMaintainer()
{
}


SensorsMaintainer *SensorsMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new SensorsMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


SensorsMaintainer *SensorsMaintainer::pinstance_{nullptr};
SemaphoreHandle_t SensorsMaintainer::mutex_{xSemaphoreCreateMutex()};
uint32_t SensorsMaintainer::TimesFireSensorTriggered{0};
uint32_t SensorsMaintainer::TimesVibrationSensorTriggered{0};
uint32_t SensorsMaintainer::TimesMovementSensorTriggered{0};