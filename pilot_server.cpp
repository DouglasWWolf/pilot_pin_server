//==========================================================================================================
// pilot_server.cpp - Implements a TCP command-line server
//==========================================================================================================
#include "globals.h"
using namespace std;


//==========================================================================================================
// This gets called when a client connects
//==========================================================================================================
void CMainServer::on_client_connect()
{
    // Find the current state of the pilot pin
    string current_state = PollingThread.get_state();

    // Tell the newly connected client the state of the pilot-pin
    sendf(".state %s\n", current_state.c_str());
}
//==========================================================================================================



//==========================================================================================================
// handle_command() - This gets called every time a command arrives on the server
//==========================================================================================================
void CMainServer::handle_command()
{
    // Fetch the name of the command
    string cmd = m_line.get_first();

    if      (cmd == "voltage?") handle_voltageq();
    else sendf(".syntax\n");
}
//==========================================================================================================


//==========================================================================================================
// handle_voltageq() - Posts an event with the most recent voltage readings
//==========================================================================================================
bool CMainServer::handle_voltageq()
{
    sendf(".voltage %1.2f %1.2f\n", global.voltage1, global.voltage2);
    return true;
}
//==========================================================================================================

