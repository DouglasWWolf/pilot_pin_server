//==========================================================================================================
// globals.cpp - Instantiate globally accessible objects
//==========================================================================================================
#include "globals.h"

// Global variables
global_t    global;

// Values set by the configuration file
conf_t      conf;

// The server
CMainServer MainServer;

// Thread that polls and reports on voltage state changes
CPollingThread PollingThread;