// This callback file is created for your convenience. You may add application
// code to this file. If you regenerate this file over a previous version, the
// previous version will be overwritten and any code you have added will be
// lost.

#include "app/framework/include/af.h"
#include "hal/hal.h"
#include EMBER_AF_API_NETWORK_STEERING


int isInternalDrivenCommandChange = 0;

#define BOOL_COMMAND_MAX_VALUE 1


static void setPetKind(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Pet Kind value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PET_FEEDER_CONTROLLER_ID,
                        ZCL_PET_KIND_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setFoodPortionWeight(void)
{
  uint16_t value = (uint16_t)emberUnsignedCommandArgument(0);

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PET_FEEDER_CONTROLLER_ID,
                        ZCL_FOOD_PORTION_WEIGHT_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setFeedInterval(void)
{
  uint16_t value = (uint16_t)emberUnsignedCommandArgument(0);

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PET_FEEDER_CONTROLLER_ID,
                        ZCL_FEED_INTERVAL_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setFoodNotConsumedAlarm(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Food Not Consumed Alarm value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PET_FEEDER_CONTROLLER_ID,
                        ZCL_FOOD_NOT_CONSUMED_ALARM_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setFoodNotConsumedInterval(void)
{
  uint16_t value = (uint16_t)emberUnsignedCommandArgument(0);

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PET_FEEDER_CONTROLLER_ID,
                        ZCL_FOOD_CONSUME_INTERVAL_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setCurrentFoodWeight(void)
{
  uint16_t value = (uint16_t)emberUnsignedCommandArgument(0);

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PET_FEEDER_CONTROLLER_ID,
                        ZCL_CURRENT_FOOD_WEIGHT_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setBowlWeight(void)
{
  uint16_t value = (uint16_t)emberUnsignedCommandArgument(0);

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PET_FEEDER_CONTROLLER_ID,
                        ZCL_BOWL_WEIGHT_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setBowlMovedNotification(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Bowl Moved Notification value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_PET_FEEDER_CONTROLLER_ID,
                        ZCL_BOWL_MOVED_NOTIFICATION_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void connectToNetwork(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);

  emberAfSmartPetFeederControllerPrint("Checking Printing Levels. Start NetworkConnection. Parameter is: %d", value);

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK)
  {
      emberSerialPrintf(APP_SERIAL,"\r\n Do nothing. Already Joined! \r\n");
  }
  else
    {
      emberSerialPrintf(APP_SERIAL,"\r\n Start Joining... \r\n");
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

  emberCommandEntryAction("set-pet-kind",
                          setPetKind,
                          "u",
                          "Set Pet Kind."),

  emberCommandEntryAction("set-food-portion-weight",
                          setFoodPortionWeight,
                          "v",
                          "Set Food Portion Weight."),

  emberCommandEntryAction("set-feed-interval",
                          setFeedInterval,
                          "v",
                          "Set Feed Interval."),

  emberCommandEntryAction("set-food-not-consumed-alarm",
                          setFoodNotConsumedAlarm,
                          "u",
                          "Set Food Not consumed Alarm."),

  emberCommandEntryAction("set-food-not-consumed-interval",
                          setFoodNotConsumedInterval,
                          "v",
                          "Set Food Not Consumed Interval."),

  emberCommandEntryAction("set-current-food-weight",
                          setCurrentFoodWeight,
                          "v",
                          "Set Current Food Weight"),

  emberCommandEntryAction("set-bowl-weight",
                          setBowlWeight,
                          "v",
                          "Set Bowl Weight."),

  emberCommandEntryAction("set-bowl-moved-notification",
                          setBowlMovedNotification,
                          "u",
                          "Set No Water Alarm to the given state."),

  emberCommandEntryAction("connect-to-network",
                          connectToNetwork,
                          "u",
                          "Start Connection Procedure."),


  emberCommandEntryTerminator()
};

/** @brief Smart Flower Controller Cluster Cluster Set Pet Kind
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPetFeederControllerSetPetKindCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering ControllerSetPetKindCallback. PetKind: %d \r\n", argOne);

  emberAfSmartPetFeederControllerPrint("\r\n petKind_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Set Portion Weigh
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPetFeederControllerSetPortionWeighCallback(int16u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering ControllerSetPortionWeighCallback. Portion Weight: %d \r\n", argOne);

  emberAfSmartPetFeederControllerPrint("\r\n portionWeight_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Set Feed Interval
 *
 * 
 *
 * @param argTwo   Ver.: always
 */
boolean emberAfSmartPetFeederControllerSetFeedIntervalCallback(int16u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering ControllerSetFeedIntervalCallback. Feed Interval: %d \r\n", argOne);

  emberAfSmartPetFeederControllerPrint("\r\n feedInterval_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Feed The Pet
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPetFeederControllerFeedThePetCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering ControllerFeedThePetCallback. Feed The Pet Status: %d \r\n", argOne);

  emberAfSmartPetFeederControllerPrint("\r\n feedThePet_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Set Food Consumption Interval
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPetFeederControllerSetFoodConsumptionIntervalCallback(int16u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering ControllerSetFoodConsumptionIntervalCallback. Food Consumption Interval: %d \r\n", argOne);

  emberAfSmartPetFeederControllerPrint("\r\n foodConsumptionInterval_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Set Bowl Weight
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartPetFeederControllerSetBowlWeightCallback(int16u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering ControllerSetBowlWeightCallback. Bowl Weight: %d \r\n", argOne);

  emberAfSmartPetFeederControllerPrint("\r\n bowlWeight_%d\r\n", argOne);

  return FALSE;
}

/** @brief Smart Flower Controller Cluster Cluster Server Attribute Changed
 *
 * Server Attribute Changed
 *
 * @param endpoint Endpoint that is being initialized  Ver.: always
 * @param attributeId Attribute that changed  Ver.: always
 */
void emberAfSmartPetFeederControllerServerAttributeChangedCallback(int8u endpoint,
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
  emberAfCorePrintln("Network Steering Completed: %p (0x%X)",
                     (status == EMBER_SUCCESS ? "Join Success" : "FAILED"),
                     status);
  emberAfCorePrintln("Finishing state: 0x%X", finalState);
  emberAfCorePrintln("Beacons heard: %d\nJoin Attempts: %d", totalBeacons, joinAttempts);

  emberSerialPrintf(APP_SERIAL, "\r\n Entering emberAfPluginNetworkSteeringCompleteCallback. finalState: %d \r\n", finalState);

  if (status == EMBER_SUCCESS)
  {
      emberAfSmartPetFeederControllerPrint("\r\n nwksuccess_%d \r\n", finalState);
  }
  else
  {
      emberAfSmartPetFeederControllerPrint("\r\n nwkfailed_%d \r\n", status);
  }
}


