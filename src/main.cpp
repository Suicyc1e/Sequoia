#include <WiFi.h>
#include <ZigBeeCommands.h>
#include <SmartDoorBell.h>
#include <SmartPlantCarer.h>
#include <SmartPetFeeder.h>

#include <SequoiaWiFiStartUpManager.h>
#include <AsyncWebServerMaintainer.h>


#include <GlobalPresets.h>




// #define DOOR_BELL

// #ifndef DOOR_BELL
// #define PLANT_CARER
// #endif


// #define HOME_SWEET_HOME
// #define HOME
// //#define ALEX_POLINA

// #ifndef HOME_SWEET_HOME
// #define HOME
// #else
// #undef HOME
// #endif


#ifndef ALEX_POLINA
  #ifndef HOME
  #define WORK
  #endif
#endif


// Replace with your network credentials

// !!! Your WiFi HERE !!!
//======================================
#ifdef HOME
  const char* ssid = "MaxDen";
  const char* password = "uvarenkov1995";
#elif defined HOME_SWEET_HOME
  const char* ssid = "Beeline_2G_F17333";
  const char* password = "FKgHb9px";
#elif defined WORK
  const char* ssid = "GS-Labs";
  const char* password = "NetWork55!";
#elif defined ALEX_POLINA
  const char* ssid = "Gaika";
  const char* password = "987456PSh";
#endif
//=======================================




// #ifdef DOOR_BELL
// SmartDoorBell *MyDevice = NULL;
// #elif defined PLANT_CARER
// SmartPlantCarer *MyDevice = NULL;
// #endif



void setup() 
{ 
  //Main Console
  Serial.begin(115200);
  SequoiaWiFiStartUpManager::GetInstance();
  //Go-go-go
  SequoiaWiFiStartUpManager::GetInstance()->Begin(); //And create device immidiately... Why to wait?

}

void loop()
{

  #ifdef BUILD_FOR_DOOR_BELL
  SmartDoorBell::GetInstance()->StartLoopedServices();
  #endif
  #ifdef BUILD_FOR_PLANT_CARER
  SmartPlantCarer::GetInstance()->StartLoopedServices();
  #endif
  #ifdef BUILD_FOR_PET_FEEDER
  SmartPetFeeder::GetInstance()->StartLoopedServices();
  #endif

  //StartUpManager.StartDevice();
  {
    //Nothing to do in the Main Loop.
  }

}