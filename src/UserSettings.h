#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <Arduino.h>

struct UserSettings
{
    String m_network_ssid;
    String m_network_password;
    bool m_saved;

    //do not save device type. Since hardware side it can't be 

    UserSettings() { }

    UserSettings(const String& ssid, const String& password, bool saved) :
        m_network_ssid(ssid),
        m_network_password(password),
        m_saved(saved)
    { }

};



#endif