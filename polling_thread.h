//==========================================================================================================
// polling_thread.h - Defines the class that periodically polls the pilot pin voltage and reports
//                    state changes
//==========================================================================================================
#pragma once
#include "cthread.h"
#include <string>
using std::string;

//==========================================================================================================
// CPollingThread - Periodically polls the pilot pin voltage and reports changes
//==========================================================================================================
class CPollingThread : public CThread
{
public:

    // Indicates whether or not this thread has completed initialization
    void    wait_for_initialization();

    // Returns the current state of the pilot-pin
    string  get_state() {return m_current_state;}

protected:

    // When this thread is spawned, execution starts here
    void    main();

    // Mark this thread as uninitialized at spawn
    void    on_spawn() {m_is_initialized = false;}

    // Will be true if this thread has completed initialization
    bool    m_is_initialized;

    // This is the current state of the pilot-pin
    string  m_current_state;
};
//==========================================================================================================