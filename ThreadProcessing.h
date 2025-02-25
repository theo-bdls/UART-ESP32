/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/

#pragma once

#include "BasicTools.h"
#include "Parameters.h"
#include <thread>
#include <tbb/concurrent_queue.h>
#include <string>
#include <algorithm>
#include <vector>

class threadProcessing
{
public:
    ///// Constructor/Destructor /////
    threadProcessing();    // Constructor
    ~threadProcessing();  // Destructor

    ///// Function declaration //////
    void StartThreadProcessingRT(tbb::concurrent_bounded_queue<std::string> &dataQueue);
    void StartThreadProcessingRS(tbb::concurrent_bounded_queue <std::string> & dataQueue);
    void StartThreadProcessingRH(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRH, tbb::concurrent_bounded_queue<double> &tagRangeQueueRH);
    void StartThreadProcessingRN(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRN, tbb::concurrent_bounded_queue<double> &tagRangeQueueRN);
    void StopThreadProcessing();
    void ProcessDataRT(tbb::concurrent_bounded_queue <std::string> & dataQueue);
    void ProcessDataRS(tbb::concurrent_bounded_queue <std::string> & dataQueue);
    void ProcessDataRH(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRH, tbb::concurrent_bounded_queue<double> &tagRangeQueueRH);
    void ProcessDataRN(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRN, tbb::concurrent_bounded_queue<double> &tagRangeQueueRN);
    void RunThreadProcessingRT(tbb::concurrent_bounded_queue<std::string> &dataQueue);
    void RunThreadProcessingRS(tbb::concurrent_bounded_queue<std::string> &dataQueue);
    void RunThreadProcessingRH(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRH, tbb::concurrent_bounded_queue<double> &tagRangeQueueRH);
    void RunThreadProcessingRN(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRN, tbb::concurrent_bounded_queue<double> &tagRangeQueueRN);

private:
    std::thread m_threadProcessingRT;           // <! Handle the RT queue
    std::thread m_threadProcessingRS;           // <! Handle the RS queue 
    std::thread m_threadProcessingRH;           // <! Handle the RH queue 
    std::thread m_threadProcessingRN;           // <! Handle the RN queue
    std::vector<std::string> m_currentBlock;    // <! Block construction to process full block HR
    std::string m_currentID;                    // <! Current ID of the block HR
    std::vector<std::string> m_currentBlockNL;  // <! Block construction to process full block NL
    std::string m_currentIDNL;                  // <! Current ID of the block NL      
    int m_expectedIndex;                        // <! Index expected to check consecutivness of the sentence HR
    int m_expectedIndexNL;                      // <! Index expected to check consecutivness of the sentence NL
    int m_processCounter = 0;                   // <! Counter before pushing HR FFT Block for plot
    int m_processCounterNL = 0;                 // <! Counter before pushing NL FFT Block for plot
};
