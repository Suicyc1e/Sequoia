#ifndef I_SEQUOIA_STARTUP_MANAGER
#define I_SEQUOIA_STARTUP_MANAGER

#include <ISequoiaDevice.h>



//There is an Interface, sine extentions of it are:
//"Bluetooth startup manager..."
//"ZigBee startup manager..."
//"PIO startup manager..."
//This tiny thing has lots of interfaces


struct ISequoiaStartUpManager
{
    virtual ~ISequoiaStartUpManager() { }

    virtual void LoadConfiguration() = 0;
    virtual void StartInitialUserInterface() = 0;
    virtual void SaveUserDefinedConfiguration() = 0;
    virtual bool StartUpUserDefinedInterface() = 0;
    virtual bool CreateDevice() = 0; //device is a Singletone!
    virtual void StartDevice() = 0;

};



#endif