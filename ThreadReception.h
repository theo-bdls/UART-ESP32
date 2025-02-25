/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/

#pragma once

#include "BasicTools.h"
#include "DataReception.h"
#include "Parameters.h"
#include "Storage.h"
#include <thread>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <tbb/concurrent_queue.h>
#include <cstdint>
#include <cstring>

class threadReception
{
public:
    ///// Constructor/Destructor /////
    threadReception(); // Constructor
    ~threadReception(); // Destructor

    ///// Function declaration //////
    void StartThreadReception(int socket, const char *fullPath, tbb::concurrent_bounded_queue<std::string> &dataQueueRT, tbb::concurrent_bounded_queue<std::string> &dataQueueRS, tbb::concurrent_bounded_queue<std::string> &dataQueueRH, tbb::concurrent_bounded_queue<std::string> &dataQueueRN);
    void StopThreadReception();
    void ReceiveStorageData(int socket, char *buffer, const char *fullPath, tbb::concurrent_bounded_queue<std::string> &dataQueueRT, tbb::concurrent_bounded_queue<std::string> &dataQueueRS, tbb::concurrent_bounded_queue<std::string> &dataQueueRH, tbb::concurrent_bounded_queue<std::string> &dataQueueRN);
    void RunThreadReception(int socket, const char *fullPath, tbb::concurrent_bounded_queue<std::string> &dataQueueRT, tbb::concurrent_bounded_queue<std::string> &dataQueueRS, tbb::concurrent_bounded_queue<std::string> &dataQueueRH, tbb::concurrent_bounded_queue<std::string> &dataQueueRN);

private:
    socketUDP m_udp;                      // <! Start and Receive        
    storageData m_storage;                // <! To have access to the method
    std::thread m_threadReception;        // <! Handle the thread
    std::string m_incompleteLine;         // <! Incomplete line from a buffer read
};

