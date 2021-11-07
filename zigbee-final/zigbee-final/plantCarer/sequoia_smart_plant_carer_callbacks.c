// This callback file is created for your convenience. You may add application
// code to this file. If you regenerate this file over a previous version, the
// previous version will be overwritten and any code you have added will be
// lost.

#include "app/framework/include/af.h"
#include "hal/hal.h"
#include EMBER_AF_API_NETWORK_STEERING



int flag_LED0 = 0;
int flag_LED1 = 0;



int isInternalDrivenCommandChange = 0;

#define BOOL_COMMAND_MAX_VALUE 1



static void setDeviceEnableControl(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Device Enable value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_DEVICE_STATUS_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setDeviceFunctionalModeControl(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Device Functional Mode value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_FUNCTIONAL_MODE_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setWaterValveMapperControl(void)
{
  uint32_t value = (uint32_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Water Valve Mapper value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_WATERING_MAP_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint32_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setSoilHumidityMapperControl(void)
{
  uint32_t value = (uint32_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Soil Humidity Mapper value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_SENSOR_MAP_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint32_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}



static void setLowerPlantMapperControl(void)
{
  uint32_t value = (uint32_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Lower Plant Mapper value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_LOWER_PLANT_ID_MAP_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint32_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}



static void setMiddlePlantMapperControl(void)
{
  uint32_t value = (uint32_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Middle Plant Mapper value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_MIDDLE_PLANT_ID_MAP_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint32_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setUpperPlantMapperControl(void)
{
  uint32_t value = (uint32_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Upper Plant Mapper value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_UPPER_PLANT_ID_MAP_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint32_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}



static void setNoWaterAlarmControl(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: No Water Alarm value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_NO_WATER_ALARM_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void connectToNetwork(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);

  emberAfSmartPlantCarerControllerPrint("Checking Printing Levels. Start NetworkConnection. Parameter is: %d", value);

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK)
  {
      emberSerialPrintf(APP_SERIAL,"\r\n Do nothing. Already Joined! \r\n");
  }
  else
    {
      emberSerialPrintf(APP_SERIAL,"\r\n Start Joining... \r\n");
      GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 1);
      GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1);
      emberAfPluginNetworkSteeringStart();
    }
}


/** 
 * Custom CLI.  This command tree is executed by typing "custom <command>"
 * See app/util/serial/command-interpreter2.h for more detail on writing commands.
**/
/*  Example sub-menu */
//  extern void doSomethingFunction(void);
//  static EmberCommandEntry customSubMenu[] = {
//    emberCommandEntryAction("do-something", doSomethingFunction, "", "Do something description"),
//    emberCommandEntryTerminator()
//  };

//  extern void actionFunction(void);
EmberCommandEntry emberAfCustomCommands[] =
{
  /* Sample Custom CLI commands */
  // emberCommandEntrySubMenu("sub-menu", customSubMenu, "Sub menu of custom commands"),
  // emberCommandEntryAction("action", actionFunction, "", "Description of action command."),



    emberCommandEntryAction("set-device-enable",
                            setDeviceEnableControl,
                            "u",
                            "Set Device Enable Control to the given state."),

    emberCommandEntryAction("set-functional-mode",
                            setDeviceFunctionalModeControl,
                            "u",
                            "Set Functional Mode control to the given state."),

    emberCommandEntryAction("set-water-valves-mapper",
                            setWaterValveMapperControl,
                            "w",
                            "Set Water Valves Mapper control to the given state."),

    emberCommandEntryAction("set-soil-humidity-mapper",
                            setSoilHumidityMapperControl,
                            "w",
                            "Set Soil Humidity Mapper control to the given state."),

    emberCommandEntryAction("set-lower-plant-mapper",
                            setLowerPlantMapperControl,
                            "w",
                            "Set Lower Plant Mapper to the given state."),

    emberCommandEntryAction("set-middle-plant-mapper",
                            setMiddlePlantMapperControl,
                            "w",
                            "Set Middle Plant Mapper to the given state."),

    emberCommandEntryAction("set-upper-plant-mapper",
                            setUpperPlantMapperControl,
                            "w",
                            "Set Upper Plant Mapper to the given state."),

    emberCommandEntryAction("set-no-water-alarm",
                            setNoWaterAlarmControl,
                            "u",
                            "Set No Water Alarm to the given state."),

    emberCommandEntryAction("connect-to-network",
                            connectToNetwork,
                            "u",
                            "Start Connection Procedure."),


  emberCommandEntryTerminator()
};

/** @brief Smart Flower Controller Cluster Cluster Enable Device
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerEnableDeviceCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering EnableDeviceCallback. Value: %d \r\n", argOne);

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_DEVICE_STATUS_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&argOne,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);

  emberAfSmartPlantCarerControllerPrint("\r\n change_%d %d \r\n", ZCL_DEVICE_STATUS_ATTRIBUTE_ID, argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Set Work Mode
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerSetWorkModeCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering SetWorkModeCallback. Value: %d \r\n", argOne);

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PLANT_CARER_CONTROLLER_ID,
                        ZCL_FUNCTIONAL_MODE_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&argOne,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);

  emberAfSmartPlantCarerControllerPrint("\r\n change_%d %d \r\n", ZCL_FUNCTIONAL_MODE_ATTRIBUTE_ID, argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Water The Plant
 *
 * 
 *
 * @param argOne   Ver.: always
 * @param argTwo   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerWaterThePlantCallback(int8u argOne,
                                                              int16u argTwo)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering WaterThePlantCallback. Flower ID: %d Time for watering: %d\r\n", argOne, argTwo);

  emberAfSmartPlantCarerControllerPrint("\r\n water_the_plant_%d %d \r\n", argOne, argTwo);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Set Watering Interval For Plant
 *
 * 
 *
 * @param argOne   Ver.: always
 * @param argTwo   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerSetWateringIntervalForPlantCallback(int8u argOne,
                                                                            int16u argTwo)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering WateringIntervalForPlantCallback. Flower ID: %d Time for watering: %d\r\n", argOne, argTwo);

  emberAfSmartPlantCarerControllerPrint("\r\n set_watering_interval_%d %d \r\n", argOne, argTwo);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Get Soil Humidity By Id
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerGetSoilHumidityByIdCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering GetSoilHumidityByIdCallback. Flower ID: %d \r\n", argOne);

  emberAfSmartPlantCarerControllerPrint("\r\n get_soil_humidity_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Get Plant Light By Id
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerGetPlantLightByIdCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering GetPlantLightByIdCallback. Flower ID: %d \r\n", argOne);

  emberAfSmartPlantCarerControllerPrint("\r\n get_plant_light_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Get Plant Air Humidity By Id
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerGetPlantAirHumidityByIdCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering GetPlantAirHumidityByIdCallback. Flower ID: %d \r\n", argOne);

  emberAfSmartPlantCarerControllerPrint("\r\n get_plant_air_humidity_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Get Plant Air Temperature By Id
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerGetPlantAirTemperatureByIdCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering GetPlantAirHumidityByIdCallback. Flower ID: %d \r\n", argOne);

  emberAfSmartPlantCarerControllerPrint("\r\n get_plant_air_temperature_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Enable Light
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPlantCarerControllerEnableLightCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering ControllerEnableLightCallback. Flower ID: %d \r\n", argOne);

  emberAfSmartPlantCarerControllerPrint("\r\n enable_ligth_%d\r\n", argOne);

  return FALSE;
}



/** @brief Complete
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt. Ver.: always
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID. Ver.: always
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network. Ver.: always
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete. Ver.: always
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering emberAfPluginNetworkSteeringCompleteCallback. finalState: %d \r\n", finalState);

  if (status == EMBER_SUCCESS)
  {
      emberAfSmartPlantCarerControllerPrint("\r\n nwksuccess_%d \r\n", finalState);
  }
  else
  {
      emberAfSmartPlantCarerControllerPrint("\r\n nwkfailed_%d \r\n", status);
  }

}



/** @brief Smart Bell Settings Cluster Cluster Server Attribute Changed
 *
 * Server Attribute Changed
 *
 * @param endpoint Endpoint that is being initialized  Ver.: always
 * @param attributeId Attribute that changed  Ver.: always
 */
void emberAfSmartPlantCarerControllerServerAttributeChangedCallback(int8u endpoint,
                                                                    EmberAfAttributeId attributeId)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering AttributeChangedCallback. Attribute: %d \r\n", attributeId);

  if (isInternalDrivenCommandChange)
  {
      emberSerialPrintf(APP_SERIAL, "\r\n Change is ignored since it is InternalDriven. Attribute: %d \r\n", attributeId);
      isInternalDrivenCommandChange = false;
      return;
  }

}
