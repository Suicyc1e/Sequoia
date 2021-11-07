#ifndef CAMERA_COMMANDS
#define CAMERA_COMMANDS

#include "WString.h"


enum class CameraCommandType
{
    SET_SSID,
    SET_PSWD
};



struct ICameraCommand
{
    virtual String GetCommand() = 0;
};


class CameraCommandBase : public virtual ICameraCommand
{
protected:

    String _prefix;
    String _command;
    CameraCommandType _commandType;

public:

    CameraCommandBase(const String& prefix, const String& command, CameraCommandType type):
        _prefix(prefix),
        _command(command),
        _commandType(type)
    { }

    virtual String GetCommand() = 0;
};


class CameraCommand_SetSSID : public virtual CameraCommandBase
{

public:

    CameraCommand_SetSSID(const String& ssid):
        CameraCommandBase("setSSID_", ssid, CameraCommandType::SET_SSID)
        { }

public:

    virtual String GetCommand() override final 
    {
        return _prefix + _command;
    } 
};


class CameraCommand_SetPSWD : public virtual CameraCommandBase
{

public:

    CameraCommand_SetPSWD(const String& pswd):
        CameraCommandBase("setPSWD_", pswd, CameraCommandType::SET_PSWD)
        { }

public:

    virtual String GetCommand() override final 
    {
        return _prefix + _command;
    } 
};


#endif