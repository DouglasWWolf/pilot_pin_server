//==========================================================================================================
// pilot_adc.cpp - Implements the interface that reads the pilot-pin ADC and converts to voltages
//==========================================================================================================
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "globals.h"
#include "pilot_adc.h"

//==========================================================================================================
// read_adc() - Reads an ADC value and returns it as an integer
//==========================================================================================================
static int read_adc(int fd)
{
    char buffer[10];

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
    // Read the two voltages
    *posv = read_adc(m_sd_pos);
    *negv = read_adc(m_sd_neg);

    // Calibrate the two voltages
    *posv = *posv * conf.posv_gain + conf.posv_offset;
    *negv = *negv * conf.negv_gain + conf.negv_offset;
}
//==========================================================================================================
