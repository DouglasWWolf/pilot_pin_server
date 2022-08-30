
//==========================================================================================================
// sim_server.h - Command line server for simulation functionality
//==========================================================================================================
#pragma once
#include "cmd_server_base.h"


class CSimServer : public CCmdServerBase
{
public:

    // Default constructor
    CSimServer() : CCmdServerBase() {}

protected:

    // Whenever a command arrives, this will be called to handle it
    void    handle_command();

    // These are the handlers for each specific command
    bool    handle_voltage();
};