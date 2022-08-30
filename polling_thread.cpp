//==========================================================================================================
// polling_thread.cpp - Implements a mechanism that periodically polls the pilot pin voltage and reports
//                      state changes
//==========================================================================================================
#include <unistd.h>
#include "globals.h"


static float sim_get_voltage()
{
    return 9.9;
}

static float get_voltage()
{
    return 8.8;

}


//==========================================================================================================
// get_state() - Returns the state of the pilot pin
//==========================================================================================================
static const char* get_state()
{
    // Fetch the voltage of the pilot pin
    float voltage = (global.simulate) ? sim_get_voltage() : get_voltage();

    if (voltage > 0 && voltage < 10) return "A";

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
