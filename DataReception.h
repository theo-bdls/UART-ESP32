/* Written by Théo Bruneau de la Salle
 *
 * ENSEIRB-MATMECA student, France
 *
 * 2024
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Parameters.h"

class socketUDP
///// Class to receive data from a UDP server /////
/**
 * IP Client : IP of the ESP32 connected to the radar : to modify in Parameters.h
 * UDPPort : Port UDP of the server define in Arduino file : to modify in Parameters.h
 */
{
    public :

    ///// Constructor/Destructor /////
    socketUDP();  // Constructor
    ~socketUDP(); // Destructor 

    ///// Structure /////
    struct SocketParameters {
        int bufferSize;                 // <! Size of the receiver buffer
    };

    //////// Getters for private attributes ///////

    SocketParameters getSocketParameters() const;

    //////// Setters ///////
    void setBufferSocket(int bufferSize);

    /////// Function declaration //////

    int Socket_Initialization(struct sockaddr_in addr_sock, struct sockaddr_in client_addr);

    void Receive_Data(int sock, char* buffer);

    void Close_Socket(int sock);


    private :

    //// Attributs ////
    int m_bufferSize;                   // <! Size of the receiver buffer
};
