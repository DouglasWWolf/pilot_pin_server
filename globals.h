//==========================================================================================================
// globals.h - Declare globally accessible objects
//==========================================================================================================
#pragma once
#include "pilot_server.h"
#include "sim_server.h"
#include "polling_thread.h"
#include "mgmt_server.h"

struct global_t
{
    bool    init_success;
    bool    simulate;
    float   sim_voltage1, sim_voltage2;
    float   voltage1, voltage2;
};

struct conf_t
{
    int     port;
    int     polling_period_ms;
};


// Values from the config file
extern conf_t  conf;

// Global variables
extern global_t global;

// The main command server
extern CMainServer MainServer;

// Server for simulation related function
extern CSimServer SimServer;

// Thread that polls and reports on voltage state changes
extern CPollingThread PollingThread;

// Process management server
extern CMgmtServer Manager;