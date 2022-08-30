//==========================================================================================================
// globals.h - Declare globally accessible objects
//==========================================================================================================
#pragma once
#include "ppin_server.h"
#include "polling_thread.h"

struct global_t
{
    bool    init_success;
    bool    simulate;
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

// Thread that polls and reports on voltage state changes
extern CPollingThread PollingThread;
