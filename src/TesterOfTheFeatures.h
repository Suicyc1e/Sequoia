#ifndef TESTER_OF_THE_FEATURES_H
#define TESTER_OF_THE_FEATURES_H

#include <Arduino.h>

enum class LedTestPhase
{
    START = 0,
    ENABLE_GREEN = 1,
    ENABLE_RED = 2,
    ENABLE_ORANGE = 3,
    TOGGLE_GREEN = 4,
    TOGGLE_RED = 5,
    TOGGLE_ORANGE = 6
};


class TesterOfTheFeatures
{
//Singletoning)))
private:

    static TesterOfTheFeatures *pinstance_;
    static SemaphoreHandle_t mutex_;

protected:

    TesterOfTheFeatures(/* args */);
    ~TesterOfTheFeatures();

public:

    TesterOfTheFeatures(TesterOfTheFeatures &other) = delete;
    void operator=(const TesterOfTheFeatures &) = delete;

    static TesterOfTheFeatures *GetInstance();

private:

    LedTestPhase _currentTestPhase;

public:

    void IncrementLedTestPhase();
    

};




#endif