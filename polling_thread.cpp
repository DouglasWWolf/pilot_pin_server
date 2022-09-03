//==========================================================================================================
// polling_thread.cpp - Implements a mechanism that periodically polls the pilot pin voltage and reports
//                      state changes
//==========================================================================================================
#include <unistd.h>
#include "globals.h"


//==========================================================================================================
// get_voltages() - Fetches the positive and negative voltages 
//
// On Exit: Voltages are in global.posv and global.negv
//==========================================================================================================
static void get_voltages()
{
    // If we're in simulation mode, return the simulated voltage
    if (global.simulate)
    {
        global.posv = global.sim_posv;
        global.negv = global.sim_negv;
        return;
    }

    // Fetch the real voltages from the hardware
    PilotADC.get_voltages(&global.posv, &global.negv);
}
//==========================================================================================================



//==========================================================================================================
// get_state() - Returns the state of the pilot pin
//==========================================================================================================
static const char* get_state()
{
    // Fetch the pilot pin positive and negative voltages
    get_voltages();
    
    // Save voltages we just fetched
    float posv = global.posv;
    float negv = global.negv;

    // define a state for the pilot minimum voltage. 
    // true indicates a good negative voltage reading, false indicates a bad negative voltage reading
    bool negv_ok = (negv >= -12.60 && negv <= -9.56);

    // check pilot high voltage and set the pilot state accordingly
    if (posv >= 11.40 && posv <= 12.60) return negv_ok ? "A2" : "A1";
    if (posv >=  8.36 && posv <=  9.56) return negv_ok ? "B2" : "B1";
    if (posv >=  5.48 && posv <=  6.49) return negv_ok ? "C2" : "C1";
    if (posv >=  2.62 && posv <=  3.25) return negv_ok ? "D2" : "D1";
    if (posv >=  0    && posv <=  0.25) return negv_ok ? "F"  : "E";
    
    // if we reach here, something is very wrong, return state F
    return "F";
}
//==========================================================================================================



//==========================================================================================================
// main() - Execution begins here
//==========================================================================================================
void CPollingThread::main()
{
    // Fetch the current pilot-pin state
    m_current_state = ::get_state();

    // Tell the outside world we've initialized
    m_is_initialized = true;

    // Fetch the polling period in microseconds
    int polling_period_us = conf.polling_period_ms * 1000;
        
    // We'll sit in a loop forever polling the pin state
    while (true)
    {
        // Do nothing for a bit
        usleep(polling_period_us);

        // Fetch the current pilot-pin state
        const char* state = ::get_state();

        // If the state of the pilot pin has changed...
        if (state != m_current_state)
        {
            // Save the new state of the pilot pin
            m_current_state = state;

            // Report the change of state to the client
            MainServer.sendf(".state %s\n", state);
        }
    }
}
//==========================================================================================================



//==========================================================================================================
// wait_for_initialization() - Returns after the pilot-pin knows what state it's in
//==========================================================================================================
void CPollingThread::wait_for_initialization()
{
    while (!m_is_initialized) usleep(100000);
}
//==========================================================================================================
