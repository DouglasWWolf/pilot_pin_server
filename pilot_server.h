//==========================================================================================================
// pilot_server.h - Defines the main TCP command-line server
//==========================================================================================================
#pragma once
#include "cmd_server_base.h"



class CMainServer : public CCmdServerBase
{
public:

    // Default constructor
    CMainServer() : CCmdServerBase() {}

    // This gets called when a client connects to the server socket
    void    on_client_connect();

protected:

    // Whenever a command arrives, this will be called to handle it
    void    handle_command();

    // These are the handlers for each specific command
    bool    handle_voltageq();
};