//==========================================================================================================
// pilot_adc.cpp - Implements the interface that reads the pilot-pin ADC and converts to voltages
//==========================================================================================================
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "pilot_adc.h"




//==========================================================================================================
// read_adjusted_voltage() - Returns the ADC value (in millivolts) with the DC-offset subtracted
//==========================================================================================================
int CPilotADC::read_adjusted_voltage(vchannel_t channel)
{
    int voltage;

    if (channel == POSV)
        voltage = read_raw_voltage(POSV) - conf.posv_dc_offset;
    else
        voltage = read_raw_voltage(NEGV) - conf.negv_dc_offset;

    return voltage;
}
//==========================================================================================================


//==========================================================================================================
// read_raw_voltage() - Reads an ASCII value and returns it as an integer (in millivolts)
//==========================================================================================================
int CPilotADC::read_raw_voltage(vchannel_t channel)
{
    char buffer[10];

    // Figure out which file descriptor to read
    int fd = (channel == POSV) ? m_sd_pos : m_sd_neg;

    // Rewind to the start of the file
    lseek(fd, 0, SEEK_SET);

    // Read a line from the file
    bitbucket = read(fd, buffer, sizeof buffer);

    // And hand the integer value to the caller
    return atoi(buffer);
}
//==========================================================================================================


//==========================================================================================================
// close() - Closes the device files
//==========================================================================================================
void CPilotADC::close()
{
    if (m_sd_neg >= 0) ::close(m_sd_neg);
    if (m_sd_pos >= 0) ::close(m_sd_pos);
    m_sd_neg = m_sd_pos = -1;
}
//==========================================================================================================



//==========================================================================================================
// init() - Opens the device files
//
// On Entry: conf.posv_device = The name of the device file for the positive voltage
//           conf.negv_device = The name of the device file for the negative voltage
//
// Returns:  true if both files could be opened, else false
//==========================================================================================================
bool CPilotADC::init()
{
   
    // Open the ADC device that reads the negative voltage
    m_sd_neg = ::open(conf.negv_device.c_str(), O_RDONLY);
    if (m_sd_neg == -1)
    {
        printf("Failed to open %s\n", conf.negv_device.c_str());
    }

    // Open the ADC device that reads the positive voltage
    m_sd_pos = ::open(conf.posv_device.c_str(), O_RDONLY);
    if (m_sd_pos == -1)
    {
        printf("Failed to open %s\n", conf.posv_device.c_str());
    }

    // If either of those open calls failed, close everything and tell the caller
    if (m_sd_neg == -1 || m_sd_pos == -1)
    {
        close();
        return false;
    }

    // Tell the caller that both device files are open
    return true;
}
//==========================================================================================================


//==========================================================================================================
// get_voltages() - Fetches the negative and positives voltages
//==========================================================================================================
void CPilotADC::get_voltages(float *posv, float* negv)
{
    // If init() failed, we will always return zeros
    if (m_sd_neg == -1 || m_sd_pos == -1)
    {
        *posv = *negv = 0.0;
        return;
    }

    // Fetch the adjusted (for DC offset) voltage from each channel
    int adj_posv = read_adjusted_voltage(POSV);
    int adj_negv = read_adjusted_voltage(NEGV);

    // Scale the two voltages
    *posv = adj_posv * conf.posv_gain + conf.posv_offset;
    *negv = adj_negv * conf.negv_gain + conf.negv_offset;
}
//==========================================================================================================
