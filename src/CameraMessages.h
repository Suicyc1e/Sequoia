#ifndef CAMERA_MESSAGES
#define CAMERA_MESSAGES

#include <WString.h>
#include <WiFi.h>

enum class CameraMessageType
{
    CAMERA_READY,
    IP_SET
};

struct ICameraMessage
{
    virtual String GetRawMessage() const = 0;
    virtual String GetMessage() const = 0;
    virtual CameraMessageType GetMessageType() const = 0;
};


class CameraMessageBase : public virtual ICameraMessage
{

protected:

    static String _prefix;
    String _rawMessage;
    CameraMessageType _type;

public:

    CameraMessageBase(String rawMessage, CameraMessageType messageType) : _rawMessage(rawMessage), _type(messageType) 
    { };

    virtual String GetRawMessage() const override final
    { return _rawMessage; }

    virtual CameraMessageType GetMessageType() const override final
    { return _type; }


    virtual String GetMessage() const = 0;

};


class CameraMessage_Ready : public virtual CameraMessageBase
{
private:

    bool _ready;
    String _resultMessageHumanReadable = "";    

public:

    CameraMessage_Ready(bool ready, String rawMessage) : 
        CameraMessageBase(rawMessage, CameraMessageType::CAMERA_READY),
        _ready(ready)
    { 
        
    };

    uint32_t GetReady() const { return _ready; }

    virtual String GetMessage() const override final
    { 
        return _resultMessageHumanReadable;
    };
};




class CameraMessage_IPSet : public virtual CameraMessageBase
{
private:

    IPAddress _ipAddress;
    String _resultMessageHumanReadable = "";    

public:

    CameraMessage_IPSet(IPAddress address, String rawMessage) : 
        CameraMessageBase(rawMessage, CameraMessageType::IP_SET),
        _ipAddress(address)
    { 
        
    };

    uint32_t GetIPAddress() const { return _ipAddress; }

    virtual String GetMessage() const override final
    { 
        return _resultMessageHumanReadable;
    };
};

#endif