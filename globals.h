//==========================================================================================================
// globals.h - Declare globally accessible objects
//==========================================================================================================
#pragma once
#include "pilot_server.h"
#include "sim_server.h"
#include "polling_thread.h"
#include "mgmt_server.h"
#include "pilot_adc.h"

struct global_t
{
    bool    simulate;
    float   sim_posv, sim_negv;
    float   posv, negv;
};

struct conf_t
{
    int     port;
    int     polling_period_ms;
    string  posv_device, negv_device;
    double  posv_gain, posv_offset;
    double  negv_gain, negv_offset;
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

// Manages the ADC that read voltages
extern CPilotADC PilotADC;

// A place to throw away values
extern volatile int bitbucket;