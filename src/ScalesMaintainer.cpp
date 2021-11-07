#include <ScalesMaintainer.h>
#include <PIOMapper.h>



ScalesMaintainer::ScalesMaintainer()
{
    //CalibrateScales();
    Serial.println("Starting Scales Init...");
    InitScales();
    Serial.println("Scales Init Done!");
}

ScalesMaintainer::~ScalesMaintainer()
{
}


 void ScalesMaintainer::InitScales()
 {
    _scales.begin(USER_SCALES_I2C_SDA, USER_SCALES_I2C_SCL);
    _scales.set_scale(_predefined_calibration_factor);
    _scales.tare();
 }


 void ScalesMaintainer::CalibrateScales()
 {
    Serial.println("Starting Scales Calibration...");
    _scales.begin(USER_SCALES_I2C_SDA, USER_SCALES_I2C_SCL);
    _scales.set_scale();
    _scales.tare();
    Serial.println("Scales Calibration Done!");
 }


//2256
//2177
//2163
//2114
//2143 == 5.65 грамма
//2262




float ScalesMaintainer::CalibrationMeasure()
{
    if (!_tareCheked)
    {
        _tareCheked = true;
        _scales.tare();
    }
    return _scales.get_units(10);
}


void ScalesMaintainer::AsyncCalibrationTask(void *parameters)
{
    Serial.printf("CALIBRATE MEASURE: %.6f", ScalesMaintainer::GetInstance()->CalibrationMeasure());
    vTaskDelete(NULL);
}


void ScalesMaintainer::AsyncCalibrateMeasure()
{
    //Serial.printf("CALIBRATE MEASURE: %.6f", _scales.get_units(10));

    Serial.println("Create Calibration measure task.");
    xTaskCreate(
        AsyncCalibrationTask,          /* Task function. */
        "AsyncCalibrationTask",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        this,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
}


float ScalesMaintainer::GetCurrentWeight()
{
    return _scales.get_units(5); //BLOCKING!
}


void ScalesMaintainer::AsyncMeasurementTask(void *parameters)
{
    //TODO: Callback here!!!
    float value = ScalesMaintainer::GetInstance()->GetCurrentWeight();
    Serial.printf("Current Weight: %.6f", value);

    //CALBACK HERE!!!!

    vTaskDelete(NULL);
}


void ScalesMaintainer::Tare()
{
    _scales.tare();
}

void ScalesMaintainer::AsyncTareTask(void *parameters)
{
    //TODO: Callback here!!!
    ScalesMaintainer::GetInstance()->Tare();
    Serial.printf("Tare done!\n");

    //CALBACK HERE!!!!

    vTaskDelete(NULL);
}


void ScalesMaintainer::AsyncSetScalesFactorTask(void *parameters)
{
    ScalesMaintainer *m_ptr = (ScalesMaintainer*)(parameters);
    
    ScalesMaintainer::GetInstance()->SetScalesFactor(m_ptr->_currentCalibrationFactor);

    vTaskDelete(NULL);
}


void ScalesMaintainer::AsyncGetWeight()
{
    Serial.println("Create Weight measure task.");
    xTaskCreate(
        AsyncMeasurementTask,          /* Task function. */
        "AsyncMeasurementTask",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        this,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
}

void ScalesMaintainer::AsyncTare()
{
    Serial.println("Create Tare task.");
    xTaskCreate(
        AsyncTareTask,          /* Task function. */
        "AsyncTareTask",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        this,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
}


void ScalesMaintainer::AsyncSetScalesFactor(int32_t factor)
{
    _currentCalibrationFactor = factor;
        xTaskCreate(
        AsyncSetScalesFactorTask,          /* Task function. */
        "AsyncSetScalesFactorTask",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        this,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
}


float ScalesMaintainer::DirectGetWeight(int numberOfApproximations)
{
    return _scales.get_units(numberOfApproximations);
}



ScalesMaintainer *ScalesMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new ScalesMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


ScalesMaintainer *ScalesMaintainer::pinstance_{nullptr};
SemaphoreHandle_t ScalesMaintainer::mutex_{xSemaphoreCreateMutex()};