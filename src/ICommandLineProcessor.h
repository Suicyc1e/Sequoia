#ifndef I_COMMAND_LINE_RPOCESSOR
#define I_COMMAND_LINE_PROCESSOR

#include <Arduino.h>

struct ICommandLineProcessor
{
    /* data */
public:

    virtual void ExecuteStartUpProcedures() = 0; //Find, init, connect, test...
    virtual bool IsConnectionAlive() = 0;
    virtual String GetHardwareDescription() = 0; //ex. UART port 5, 115200, etc...
    virtual void CommandsTest() = 0;

protected:

    virtual  void ProcessIncomingData(String esp32Data) = 0; 
};


#endif