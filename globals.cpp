//==========================================================================================================
// globals.cpp - Instantiate globally accessible objects
//==========================================================================================================
#include "globals.h"

// Global variables
global_t global;

// Values set by the configuration file
conf_t conf;

// A place to throw away values
volatile int bitbucket;

// The server
CMainServer MainServer;

// Server for simulation commands
CSimServer  SimServer;

// Thread that polls and reports on voltage state changes
CPollingThread PollingThread;

// Process management server
CMgmtServer Manager;

// Manages the ADC that read voltages
CPilotADC PilotADC;
