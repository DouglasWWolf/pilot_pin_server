//==========================================================================================================
// polling_thread.cpp - Implements a mechanism that periodically polls the pilot pin voltage and reports
//                      state changes
//==========================================================================================================
#include <unistd.h>
#include "globals.h"


static void get_voltages()
{
    // If we're in simulation mode, return the simulated voltage
    if (global.simulate)
    {
        global.voltage1 = global.sim_voltage1;
        global.voltage2 = global.sim_voltage2;
        return;
    }

    global.voltage1 = 1.1;
    global.voltage2 = 2.2;
}


//==========================================================================================================
// get_state() - Returns the state of the pilot pin
//==========================================================================================================
static const char* get_state()
{
    // Fetch the pilot pin voltage and pilot pin minimum voltage
    get_voltages();
    
    // save voltages we just fetched in high and low variables
    float voltage_high = global.voltage1;
    float voltage_low  = global.voltage2;

    // define a state for the pilot minimum voltage. 
    // true indicates a good low voltage reading, false indicates a bad low voltage reading
    bool pilot_low = (voltage_low >= -12.60 && voltage_low <= -9.56);

    // check pilot high voltage and set the pilot state accordingly
    if (voltage_high >= 11.40 && voltage_high <= 12.60) return pilot_low ? "A2" : "A1";
    if (voltage_high >=  8.36 && voltage_high <=  9.56) return pilot_low ? "B2" : "B1";
    if (voltage_high >=  5.48 && voltage_high <=  6.49) return pilot_low ? "C2" : "C1";
    if (voltage_high >=  2.62 && voltage_high <=  3.25) return pilot_low ? "D2" : "D1";
    if (voltage_high >=  0    && voltage_high <=  0.25) return pilot_low ? "F"  : "E";
    
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
