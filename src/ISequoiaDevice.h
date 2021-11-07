#ifndef I_SEQUOIA_DEVICE_H
#define I_SEQUOIA_DEVICE_H

#include <Arduino.h>


//I know English a little. I know, that "Carer" - there is no such a word.
//Nonetheless, this is an idea) I'd like to call it that way in the code. Since it's really take care of those green guys.

enum class SequoiaDeviceType
{
    DOOR_BELL,
    PET_FEEDER,
    PLANT_CARER
};

enum class SequoiaNetworkMode
{
    STAY_AS_ACCEESS_POINT,
    CONNECT_TO_WIFI
};

struct NetworkCredentials
{
    IPAddress m_address;
    String m_ssid;
    String m_pswd;
    SequoiaNetworkMode m_networkMode; 

    NetworkCredentials(IPAddress address, const String& ssid, const String& password, SequoiaNetworkMode mode = SequoiaNetworkMode::CONNECT_TO_WIFI):
        m_address(address),
        m_ssid(ssid),
        m_pswd(password),
        m_networkMode(mode)
    { }

    NetworkCredentials() { }
};


struct ISequoiaDevice
{
    virtual ~ISequoiaDevice() { }

    virtual void SetNetworkCredentials(const NetworkCredentials& credentials) = 0;
    virtual void StartLoopedServices() = 0;

};





#endif