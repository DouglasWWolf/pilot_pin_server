
//==========================================================================================================
// sim_server.cpp - TCP Server for the simulator
//==========================================================================================================
#include "globals.h"
using namespace std;


//==========================================================================================================
// handle_command() - This gets called every time a command arrives on the server
//==========================================================================================================
void CSimServer::handle_command()
{
    // Fetch the name of the command
    string cmd = m_line.get_first();

    if (cmd == "voltage" ) handle_voltage();
    else fail_syntax();
}
//==========================================================================================================


//==========================================================================================================
// handle_voltage() - Specifies the simulated voltage
//==========================================================================================================
bool CSimServer::handle_voltage()
{
    double v1, v2;

    // Fetch the command parameters as floating point values
    if (!m_line.get_next(&v1)) return fail_syntax();
    if (!m_line.get_next(&v2)) return fail_syntax();

    // Fill in the simulated voltages
    global.sim_voltage1 = (float) v1;
    global.sim_voltage2 = (float) v2;

    // Tell the caller that all is well
    return pass();
}
//==========================================================================================================

