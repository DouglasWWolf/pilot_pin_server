//==========================================================================================================
// pilot_adc.h - Defines the interface that reads the pilot-pin ADC and converts to voltages
//==========================================================================================================
#pragma once

class CPilotADC
{
public:

    // Constructor, initializes the file descriptors
    CPilotADC() {m_sd_neg = m_sd_pos = -1;}

    // Destructor() - Closes the devices
    ~CPilotADC() {close();}

    // Opens the device files
    bool    init();

    // Closes the device files
    void    close();

    // Fetches the two voltages
    void    get_voltages(float *posv, float* negv);

protected:

    // File descriptors for the devices that read the negative and positive voltages
   int     m_sd_neg, m_sd_pos;

};
