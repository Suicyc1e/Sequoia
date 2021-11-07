#ifndef SCALES_MAINTAINER_H
#define SCALES_MAINTAINER_H

#include <Arduino.h>
#include <HX711.h>

class ScalesMaintainer
{
private:

   //Singletoning)))
private:

    static ScalesMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;

protected:

    ScalesMaintainer(/* args */);
    ~ScalesMaintainer();

private:

    HX711 _scales;

    float _predefined_calibration_factor = 400661; // for me this value works just perfect 419640
    //400,353,982301
    //407,96460177
    //
    //float offset_factor = 4400;//

    bool _tareCheked = false;

private:   

    //IMPROVING:: Calibrate scales when the user calling (fro GUI) 
    void InitScales();
    void CalibrateScales();
    void SetScalesFactor(int32_t factor) { _scales.set_scale(factor); }

public:

    void Tare();

public:

    void AsyncCalibrateMeasure();
    void AsyncGetWeight();
    void AsyncTare();

    void AsyncSetScalesFactor(int32_t factor);

public:

    float DirectGetWeight(int numberOfApproximations = 1);

private:

    int32_t _currentCalibrationFactor;

//NON-Blocking tasks...
private:

    static void AsyncCalibrationTask(void *parameters);
    static void AsyncMeasurementTask(void *parameters);
    static void AsyncTareTask(void *parameters);
    static void AsyncSetScalesFactorTask(void *parameters);
    

//Direct measurements
//BLOCKING!
private:

    float CalibrationMeasure();
    float GetCurrentWeight(); //return in gramms!


public:

    ScalesMaintainer(ScalesMaintainer &other) = delete;
    void operator=(const ScalesMaintainer &) = delete;

    static ScalesMaintainer *GetInstance();
};





#endif