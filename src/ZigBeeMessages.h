#ifndef ZIGBEE_MESSAGES_H
#define ZIGBEE_MESSAGES_H

#include "WString.h"

//Actually, 'ZigBeeMessage' is 'ZigBeeCommand' in backward Direction))
//So, it is 'ZigBeeCommand' from EFR32 to ESP32 :)

enum class ZigBeeMessageType
{
    ATTRIBUTE_CHANGE,
    NWK_SUCCESS,
    NWK_FAILED,
    UNKNOWN,
};


struct IZigBeeMessage
{
    virtual String GetRawMessage() const = 0;
    virtual String GetMessage() const = 0;
    virtual ZigBeeMessageType GetMessageType() const = 0;
};


class ZigBeeMessageBase : public virtual IZigBeeMessage
{

protected:

    static String _messagePrefix;
    String _rawMessage;
    ZigBeeMessageType _messageType;

public:

    ZigBeeMessageBase(String rawMessage, ZigBeeMessageType messageType) : _rawMessage(rawMessage), _messageType(messageType) 
    { };

    virtual String GetRawMessage() const override final
    { return _rawMessage; }

    virtual ZigBeeMessageType GetMessageType() const override final
    { return _messageType; }


    virtual String GetMessage() const = 0;
};

//IMPROVING: Make this as a template accepting different values, not only uint32 for further development.
class ZigBeeMessage_AttributeChanged : public virtual ZigBeeMessageBase
{
private:

    uint32_t _attributeId = 0;
    uint32_t _newAttributeValue = 0;
    String _resultMessageHumanReadable = "";    

public:

    ZigBeeMessage_AttributeChanged(uint32_t attrId, uint32_t attrValue, String rawMessage) : 
        ZigBeeMessageBase(rawMessage, ZigBeeMessageType::ATTRIBUTE_CHANGE),
        _attributeId(attrId),
        _newAttributeValue(attrValue)
    { 
        //_resultMessageHumanReadable = "Message Type: ZigBeeMessageType::ATTRIBUTE_CHANGE Attribute id: " + _attributeId + "New Attribute value: " + _newAttributeValue;
    };


    uint32_t GetAttributeId() const { return _attributeId; }
    uint32_t GetNewAttributeValue() const { return _newAttributeValue; }

    virtual String GetMessage() const override final
    { 
        return _resultMessageHumanReadable;
    };

    //IMPROVING: Add Static const string to be defined respective to Message class.
    //static const String MessagePrefix;
};

//Connection Succeded
class ZigBeeMessage_NwkSuccess : public virtual ZigBeeMessageBase
{
private:

    uint32_t _status = 0;
    String _resultMessageHumanReadable = "";    

public:

    ZigBeeMessage_NwkSuccess(uint32_t status, String rawMessage) : 
        ZigBeeMessageBase(rawMessage, ZigBeeMessageType::NWK_SUCCESS),
        _status(status)
    { 
        
    };

    uint32_t GetStatus() const { return _status; }

    virtual String GetMessage() const override final
    { 
        return _resultMessageHumanReadable;
    };

    //IMPROVING: Add Static const string to be defined respective to Message class.
    //static const String MessagePrefix;
};

//Connection Failed
class ZigBeeMessage_NwkFailed : public virtual ZigBeeMessageBase
{
private:

    uint32_t _status = 0;
    String _resultMessageHumanReadable = "";    

public:

    ZigBeeMessage_NwkFailed(uint32_t status, String rawMessage) : 
        ZigBeeMessageBase(rawMessage, ZigBeeMessageType::NWK_FAILED),
        _status(status)
    { 
        
    };

    uint32_t GetStatus() const { return _status; }

    virtual String GetMessage() const override final
    { 
        return _resultMessageHumanReadable;
    };

    //IMPROVING: Add Static const string to be defined respective to Message class.
    //static const String MessagePrefix;
};


class SmartPlantCarerCommand
{
private:

    String _rawCommand = "";

public:

    SmartPlantCarerCommand(const String& command):
        _rawCommand(command)
    { }

    String GetRawCommand() { return _rawCommand; }
};

#endif