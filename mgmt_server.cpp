//==========================================================================================================
// mgmt_server.cpp - Implements a process-management UDP server
//==========================================================================================================
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "mgmt_server.h"
#include "udpsock.h"

enum
{
    CMD_PING = 1,
    RSP_PING = 2,
    CMD_DOWN = 3
};

// Message structures for the message we understand
struct cmd_ping_t {uint16_t cmd; uint16_t port;};
struct rsp_ping_t {uint16_t cmd; uint16_t port;};

//==========================================================================================================
// main() - Performs process management
//==========================================================================================================
void CMgmtServer::main()
{
    int reply_port;

    union 
    {
        char        buffer[4];
        uint16_t    cmd;
        cmd_ping_t  ping_cmd;
        rsp_ping_t  ping_rsp;
    };

    // The UDP client and server socket
    UDPSock client, server;

    // The UDP port we want to listen on is in the first spawn parameter
    uint16_t port = *(int*)(m_spawn_param[0]);

    // Create a UDP server
    if (!server.create_server(port, "", AF_INET))
    {
        printf("Can't create management server on socket %d\n", port);
        while(true) sleep(999999999);
    }

    // Wait for process management messages to arrive
    while (server.receive(buffer, sizeof buffer))
    {
        // If the command was "ping", send a ping response
        if (cmd == CMD_PING)
        {
            // Fetch the port number to send our reply to
            reply_port = ping_cmd.port;
            ping_rsp = {RSP_PING, port};
            ping_rsp.port = port;
            client.create_sender(reply_port, "localhost", AF_INET);
            client.send(&ping_rsp, sizeof ping_rsp);
            client.close();
            continue;
        }        

        // If the command was "down", exit the program
        if (cmd == CMD_DOWN) exit(0);
    }
};
//==========================================================================================================
