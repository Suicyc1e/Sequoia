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

//THIS 'ATTRIBUTE' is virtual and NOT Stored within EFR32 device.
#define WEB_INTERFACE_ATTR_ID 8


//WE ARE EMBEDDED! :)
//
const char *bit_rep[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

static void printUnsignedInt(uint32_t value)
{
  emberAfSmartDoorBellControllerPrint("\r\n Flag Value is: %s%s%s%s%s%s%s%s \r\n",
            bit_rep[value >> 28],
            bit_rep[value >> 24 & 0x0F],
            bit_rep[value >> 20 & 0x0F],
            bit_rep[value >> 16 & 0x0F],
            bit_rep[value >> 12 & 0x0F],
            bit_rep[value >> 8 & 0x0F],
            bit_rep[value >> 4 & 0x0F],
            bit_rep[value & 0x0F]
            );
}


static void setAutoAnswerControl(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid AutoAnswer value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_AUTO_ANSWER_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setSoundDetectControl(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid SoundDetect value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_MICROPHONE_SOUND_DETECTION_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setPhotoSavingControl(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid PhotoSaving value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_PHOTO_SAVING_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setSoundSavingControl(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid SoundSaving value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_SOUND_SAVING_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setButtonTriggeredAlarm(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Button Triggered Alarm value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_BUTTON_TRIGGERED_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setMicrophoneTriggeredAlarm(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Microphone Triggered Alarm value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_MICROPHONE_TRIGGERED_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


//AntiVandalism Detection
static void setBoxVibrationTriggeredAlarm(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);
  if (value > BOOL_COMMAND_MAX_VALUE)
  {
    emberAfCorePrintln("Warning: Invalid Microphone Triggered Alarm value: %d will be treated as 1", value);
  }

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_BOX_VIBRATION_TRIGGERED_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);
}


static void setAdditionalSensorMapperState(void)
{
  uint32_t value = (uint32_t)emberUnsignedCommandArgument(0);

  emberAfSmartDoorBellControllerPrint("Checking Printing Levels. Set AdditionalSensorsMapper State to the: %d", value);

  isInternalDrivenCommandChange = 1;

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_ADDITIONAL_SENSORS_MAPPER_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&value,
                        ZCL_INT32U_ATTRIBUTE_TYPE);

  //Check AdditionalSensorsMapper
  printUnsignedInt(value);
}


static void connectToNetwork(void)
{
  uint8_t value = (uint8_t)emberUnsignedCommandArgument(0);

  emberAfSmartDoorBellControllerPrint("Checking Printing Levels. Start NetworkConnection. Parameter is: %d", value);

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


EmberCommandEntry emberAfCustomCommands[] =
{
  emberCommandEntryAction("set-auto-answer",
                          setAutoAnswerControl,
                          "u",
                          "Set AutoAnswer Control to the given state."),

  emberCommandEntryAction("set-sound-detect",
                          setSoundDetectControl,
                          "u",
                          "Set Sound Detect control to the given state."),

  emberCommandEntryAction("set-photo-saving",
                          setPhotoSavingControl,
                          "u",
                          "Set Photo Saving control to the given state."),

  emberCommandEntryAction("set-sound-saving",
                          setSoundSavingControl,
                          "u",
                          "Set Sound Saving control to the given state."),

  emberCommandEntryAction("set-button-triggered",
                          setButtonTriggeredAlarm,
                          "u",
                          "Set Button Triggered Alarm to the given state."),

  emberCommandEntryAction("set-microphone-triggered",
                          setMicrophoneTriggeredAlarm,
                          "u",
                          "Set Microphone Triggered Alarm to the given state."),

  emberCommandEntryAction("set-box-vibration-triggered",
                          setBoxVibrationTriggeredAlarm,
                          "u",
                          "Set Box Vibration Triggered Alarm to the given state."),

  emberCommandEntryAction("set-additionalSensorMapper-state",
                          setAdditionalSensorMapperState,
                          "w",
                          "Set Additional Sensors Mapper to the given state."),

  emberCommandEntryAction("connect-to-network",
                          connectToNetwork,
                          "u",
                          "Start Connection Procedure."),

  emberCommandEntryTerminator()
};



/** @brief Called when Button 0 is pressed long
 *
 * @param timePressedMs Amount of time button 0 was pressed.
 * @param pressedAtReset Was the button pressed at startup.
 */
void emberAfPluginButtonInterfaceButton0PressedLongCallback(uint16_t timePressedMs,
                                                            bool pressedAtReset)
{
  emberSerialPrintf(APP_SERIAL,"\r\n Entering Button0PressedLongCallback.\r\n");

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK)
  {
      emberSerialPrintf(APP_SERIAL,"\r\n Start Binding... \r\n");
      emberAfPluginFindAndBindTargetStart(1);
  }
  else
    {
      emberSerialPrintf(APP_SERIAL,"\r\n Start Joining... \r\n");
      GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 1);
      GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1);
      emberAfPluginNetworkSteeringStart();
    }
}

/** @brief On/off Cluster Toggle
 *
 *
 *
 */
boolean emberAfOnOffClusterToggleCallback(void)
{
  flag_LED1 = !flag_LED1;

  emberSerialPrintf(APP_SERIAL, "\r\n Entering OnOffClusterToggleCallback\r\n");

  if (flag_LED1 == 1)
    {
      GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1);
    }
  else
    {
      GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 0);
    }

  return FALSE;
}

/** @brief Called when Button 1 is pressed long
 *
 * @param timePressedMs Amount of time button 1 was pressed.
 * @param pressedAtReset Was the button pressed at startup.
 */
void emberAfPluginButtonInterfaceButton1PressedLongCallback(uint16_t timePressedMs,
                                                            bool pressedAtReset)
{
  flag_LED0 = !flag_LED0;

  emberSerialPrintf(APP_SERIAL, "\r\n Entering Button1PressedLongCallback \r\n");

  if (flag_LED0 == 1)
    {
      GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 1);
    }
  else
    {
      GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 0);
    }
}

/** @brief Smart Bell Settings Cluster Cluster Enable Auto Answer
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartDoorBellControllerEnableAutoAnswerCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering EnableAutoAnswerCallback. Value: %d \r\n", argOne);

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_AUTO_ANSWER_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&argOne,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);

  emberAfSmartDoorBellControllerPrint("\r\n change_%d %d \r\n", ZCL_AUTO_ANSWER_ATTRIBUTE_ID, argOne);

  return FALSE;
}

/** @brief Smart Bell Settings Cluster Cluster Enable Sound Detection
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartDoorBellControllerEnableSoundDetectionCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering EnableSoundDetectionCallback. Value: %d \r\n", argOne);

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_MICROPHONE_SOUND_DETECTION_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&argOne,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);

  emberAfSmartDoorBellControllerPrint("\r\n change_%d %d \r\n", ZCL_MICROPHONE_SOUND_DETECTION_ATTRIBUTE_ID, argOne);

  return FALSE;
}

/** @brief Smart Bell Settings Cluster Cluster Enable Photo Saving
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartDoorBellControllerEnablePhotoSavingCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering EnablePhotoSavingCallback. Value: %d \r\n", argOne);

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_PHOTO_SAVING_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&argOne,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);

  emberAfSmartDoorBellControllerPrint("\r\n change_%d %d \r\n", ZCL_PHOTO_SAVING_ATTRIBUTE_ID, argOne);

  return FALSE;
}

/** @brief Smart Bell Settings Cluster Cluster Enable Sound Saving
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartDoorBellControllerEnableSoundSavingCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering EnableSoundSavingCallback. Value: %d \r\n", argOne);

  emberAfWriteAttribute(emberAfPrimaryEndpoint(),
                        ZCL_SMART_DOOR_BELL_CONTROLLER_ID,
                        ZCL_SOUND_SAVING_ATTRIBUTE_ID,
                        CLUSTER_MASK_SERVER,
                        (uint8_t *)&argOne,
                        ZCL_BOOLEAN_ATTRIBUTE_TYPE);

  emberAfSmartDoorBellControllerPrint("\r\n change_%d %d \r\n", ZCL_SOUND_SAVING_ATTRIBUTE_ID, argOne);

  return FALSE;
}

/** @brief Smart Bell Settings Cluster Cluster Enable Web Interface
 *
 * 
 *
 * @param argOne   Ver.: always
 */
boolean emberAfSmartDoorBellControllerEnableWebInterfaceCallback(int8u argOne)
{
  emberSerialPrintf(APP_SERIAL, "\r\n Entering EnableWebInterfaceCallback. Value: %d \r\n", argOne);

  //The is NO Attribute responsible for the WebInterface holder.
  emberAfSmartDoorBellControllerPrint("\r\n change_%d %d \r\n", WEB_INTERFACE_ATTR_ID, argOne);

  return FALSE;
}

/** @brief Smart Bell Settings Cluster Cluster Server Attribute Changed
 *
 * Server Attribute Changed
 *
 * @param endpoint Endpoint that is being initialized  Ver.: always
 * @param attributeId Attribute that changed  Ver.: always
 */
void emberAfSmartDoorBellControllerServerAttributeChangedCallback(int8u endpoint,
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
  emberSerialPrintf(APP_SERIAL, "\r\n Entering emberAfPluginNetworkSteeringCompleteCallback. finalState: %d \r\n", finalState);

  if (status == EMBER_SUCCESS)
  {
    emberAfSmartDoorBellControllerPrint("\r\n nwksuccess_%d \r\n", finalState);
  }
  else
  {
    emberAfSmartDoorBellControllerPrint("\r\n nwkfailed_%d \r\n", status);
  }

}

