#include <TesterOfTheFeatures.h>
#include <LEDMaintainer.h>
#include <GlobalPresets.h>

TesterOfTheFeatures::TesterOfTheFeatures()
{
    _currentTestPhase = LedTestPhase::START;
}

TesterOfTheFeatures::~TesterOfTheFeatures()
{

}



void TesterOfTheFeatures::IncrementLedTestPhase()
{
    //I am lazy and don't want to implement switch
    uint32_t currentPhaseId = static_cast<uint32_t>(_currentTestPhase);
    currentPhaseId++;

    if (currentPhaseId > static_cast<uint32_t>(LedTestPhase::TOGGLE_ORANGE))
    {
        currentPhaseId = static_cast<uint32_t>(LedTestPhase::START);
    }
        
    _currentTestPhase = static_cast<LedTestPhase>(currentPhaseId);

    switch (_currentTestPhase)
    {
    case LedTestPhase::START:
        Serial.println("LED Test Phase is START!");
        LEDMaintainer::GetInstance()->StopToggleGreenLED();
        LEDMaintainer::GetInstance()->StopToggleRedLED();
        LEDMaintainer::GetInstance()->SetGreenLEDState(LedControl::LED_DISABLED);
        LEDMaintainer::GetInstance()->SetRedLEDState(LedControl::LED_DISABLED);
        break;
    
    case LedTestPhase::ENABLE_GREEN:
        Serial.println("LED Test Phase is ENABLE_GREEN!");
        LEDMaintainer::GetInstance()->StopToggleGreenLED();
        LEDMaintainer::GetInstance()->StopToggleRedLED();
        LEDMaintainer::GetInstance()->SetGreenLEDState(LedControl::LED_ENABLED);
        LEDMaintainer::GetInstance()->SetRedLEDState(LedControl::LED_DISABLED);
        break;

    case LedTestPhase::ENABLE_RED:
        Serial.println("LED Test Phase is ENABLE_RED!");
        LEDMaintainer::GetInstance()->StopToggleGreenLED();
        LEDMaintainer::GetInstance()->StopToggleRedLED();
        LEDMaintainer::GetInstance()->SetGreenLEDState(LedControl::LED_DISABLED);
        LEDMaintainer::GetInstance()->SetRedLEDState(LedControl::LED_ENABLED);
        break;

    case LedTestPhase::ENABLE_ORANGE:
        Serial.println("LED Test Phase is ENABLE_ORANGE!");
        LEDMaintainer::GetInstance()->StopToggleGreenLED();
        LEDMaintainer::GetInstance()->StopToggleRedLED();
        LEDMaintainer::GetInstance()->SetGreenLEDState(LedControl::LED_ENABLED);
        LEDMaintainer::GetInstance()->SetRedLEDState(LedControl::LED_ENABLED);
        break;

    case LedTestPhase::TOGGLE_GREEN:
        Serial.println("LED Test Phase is TOGGLE_GREEN!");
        LEDMaintainer::GetInstance()->StopToggleGreenLED();
        LEDMaintainer::GetInstance()->StopToggleRedLED();
        LEDMaintainer::GetInstance()->ToggleGreenLED(LED_BLINKING_TIME_DURING_TEST_S);
        LEDMaintainer::GetInstance()->SetRedLEDState(LedControl::LED_DISABLED);
        break;

    case LedTestPhase::TOGGLE_RED:
        Serial.println("LED Test Phase is TOGGLE_RED!");
        LEDMaintainer::GetInstance()->StopToggleGreenLED();
        LEDMaintainer::GetInstance()->StopToggleRedLED();
        LEDMaintainer::GetInstance()->SetGreenLEDState(LedControl::LED_DISABLED);
        LEDMaintainer::GetInstance()->ToggleRedLED(LED_BLINKING_TIME_DURING_TEST_S);
        break;

    case LedTestPhase::TOGGLE_ORANGE:
        Serial.println("LED Test Phase is TOGGLE_ORANGE!");
        LEDMaintainer::GetInstance()->StopToggleGreenLED();
        LEDMaintainer::GetInstance()->StopToggleRedLED();
        LEDMaintainer::GetInstance()->ToggleGreenLED(LED_BLINKING_TIME_DURING_TEST_S);
        LEDMaintainer::GetInstance()->ToggleRedLED(LED_BLINKING_TIME_DURING_TEST_S);
        break;

    default:
        break;
    }
}



TesterOfTheFeatures *TesterOfTheFeatures::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new TesterOfTheFeatures();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}


TesterOfTheFeatures *TesterOfTheFeatures::pinstance_{nullptr};
SemaphoreHandle_t TesterOfTheFeatures::mutex_{xSemaphoreCreateMutex()};