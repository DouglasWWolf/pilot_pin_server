//==========================================================================================================
// cppserver - A demonstration of the CCmdServerBase class
//==========================================================================================================
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmd_line.h"
#include "globals.h"
#include "config_file.h"

using namespace std;

// A command line parser
CCmdLine    CmdLine;


bool init_hardware() {return true;}
bool init_simulation() {return true;}

void fetch_config();

//==========================================================================================================
// main() - Execution starts here
//==========================================================================================================
int main(int argc, char** argv)
{
    cmd_server_t server_params;

    // Read the configuration file
    fetch_config();

    // Set up some default server parameters
    server_params.verbose = false;
    server_params.port    = conf.port;

    // Declare the valid command line switches
    CmdLine.declare_switch("-verbose", CLP_NONE);
    CmdLine.declare_switch("-sim",     CLP_NONE);

    // Parse the command line
    if (!CmdLine.parse(argc, argv))
    {
        fprintf(stderr, "%s: %s\n", argv[0], CmdLine.error().c_str());
        exit(1);
    }

    // Determine whether we're in simulation mode
    global.simulate = CmdLine.has_switch("-sim");

    // Find out if the user wants to be in verbose mode
    server_params.verbose = CmdLine.has_switch("-verbose");

    // Initialize either the simulated hardware or the real hardware
    if (global.simulate)
        global.init_success = init_simulation();
    else
        global.init_success = init_hardware();

    // Spawn the thread that reports changes in pilot-pin state
    PollingThread.spawn();

    // Wait for the polling thread to discover the current state of the pilot-pin
    PollingThread.wait_for_initialization();

    // If we're in simulation mode, spawn the SimServer
    if (global.simulate)
    {
        cmd_server_t sim_params;
        sim_params.port = conf.port + 1;
        sim_params.verbose = false;
        SimServer.start(&sim_params);
    }

    // Start the command server
    MainServer.start(&server_params);

    // We have nothing else to do, so we'll just wait on the server thread
    MainServer.join();
}
//==========================================================================================================




//==========================================================================================================
// fetch_config() - Reads in the configuration file
//==========================================================================================================
void fetch_config()
{
    CConfigFile config;

    // Read the configuration file and drop dead if we can't
    if (!config.read("pilot_pin_server.conf")) exit(1);

    // Fetch the port number of the main server
    config.get("port", &conf.port);

    // Fetch the number of millisecond between voltage readings
    config.get("polling_period_ms", &conf.polling_period_ms);
}
//==========================================================================================================