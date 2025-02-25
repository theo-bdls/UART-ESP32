/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/


#include "ThreadReception.h"
#include "ThreadProcessing.h"
#include "DataReception.h"
#include "matplotlibcpp.h"
#include "BasicTools.h"
#include "Storage.h"
#include "Parameters.h"
#include <tbb/concurrent_queue.h>

namespace plt = matplotlibcpp;

int sock; // <! creation of the socket to bind to the UDP server


// All the queue to push the data receive in ThreadReception and pop the data receive in ThreadProcessing
tbb::concurrent_bounded_queue<std::string> dataQueueRT; 
tbb::concurrent_bounded_queue<std::string> dataQueueRS;
tbb::concurrent_bounded_queue<std::string> dataQueueRH;
tbb::concurrent_bounded_queue<std::string> dataQueueRN;

// Queue to receive the data from the RH processing thread
#ifdef RH_THREAD 
tbb::concurrent_bounded_queue<std::vector<double>> FFTQueueRH; // <! 1 x 128 block received with absolute FFT value each NB_PLOT blocks
tbb::concurrent_bounded_queue<double> tagRangeQueueRH;         // <! Queue with the tagRange value reception each NB_PLOT blocks
#endif //RH_THREAD

#ifdef RN_THREAD
// Queue to receive the data from the RN processing thread
tbb::concurrent_bounded_queue<std::vector<double>> FFTQueueRN; // <! 1 x 128 block received with absolute FFT value each NB_PLOT blocks
tbb::concurrent_bounded_queue<double> tagRangeQueueRN;         // <! Queue with the tagRange value reception each NB_PLOT blocks
#endif //RN_THREAD

// Client and host info
static struct sockaddr_in addr_sock;
static struct sockaddr_in client_addr;

// Class declaration 
socketUDP UDP;                      // <! reception from the UDP server
storageData Storage;                // <! writting to a file all the data
threadReception ThreadReception;    // <! reception of the data and push to the 4 queues 
threadProcessing ThreadProcessing;  // <! process each type of data : RH and RN 
basicTools ToolsMain;               // <! main tools 

int main(){

    sock = UDP.Socket_Initialization(addr_sock, client_addr);  // <! Open and bind to the socket
    Storage.setFileParameters(FULLPATH);
    Storage.Open_Text_File(Storage.getFullPath()); // <! Open the text file to write on it

    double freqRes = SAMPLING_FREQ / FFT_VECTOR_SIZE; // <! calculate the resolution of the fft

    ThreadReception.StartThreadReception(sock, Storage.getFullPath(), dataQueueRT, dataQueueRS, dataQueueRH, dataQueueRN);

#ifdef RT_THREAD
    ThreadProcessing.StartThreadProcessingRT(dataQueueRT);
#endif //RT_THREAD
#ifdef RS_THREAD
    ThreadProcessing.StartThreadProcessingRS(dataQueueRS);
#endif //RS_THREAD

// Multiprocessing issues because of the FFT
#ifdef RH_THREAD
    ThreadProcessing.StartThreadProcessingRH(dataQueueRH, FFTQueueRH, tagRangeQueueRH);
    std::vector<double> FFTDataRH; // <! storage of the data received from RH queue thread
    double tagRangeEst;            // <! range received from RH queue thread

//////////////// PLOT //////////////
// The Plot can slow everything //
// Changing the pause can improve the speed of the plot //
// Plot should be used as a diagnosis tool to see if the setup is good // 
// Then I advise to use only the storage and processing tools //

#ifdef RH_PLOT
    std::vector<double> freqAxis = ToolsMain.generateFreqAxis(freqRes, SAMPLING_FREQ); // <! define the freqAxis for Harmonic plot
    while (true)
    {
        if (FFTQueueRH.try_pop(FFTDataRH) && tagRangeQueueRH.try_pop(tagRangeEst))
        {
        
            std::string tagRangeString = "Tag Range : " + (std::to_string(tagRangeEst)).substr(0,4) + "m";

            plt::clf();
            plt::title("Magnitude vs Freq HR");
            plt::semilogy(freqAxis, FFTDataRH);
            plt::xlabel("Frequency (Hz)");
            plt::xlim(3000, 100000);
            plt::ylim(10e-3, 10e4);
            plt::text(80000.0, 2e-3, tagRangeString);
            plt::draw();
            plt::pause(0.01);
        }

    }

#endif // PLOT_HR
#endif // RH_THREAD

#ifdef RN_THREAD
    ThreadProcessing.StartThreadProcessingRN(dataQueueRN, FFTQueueRN, tagRangeQueueRN);
    std::vector<double> FFTDataRN; // <! storage of the data received from RN queue thread
    double tagRangeEstRN;          // <! range received from RN queue thread

#ifdef RN_PLOT
    std::vector<double> freqAxisRN = ToolsMain.generateFreqAxis(freqRes / 2, SAMPLING_FREQ / 2); // <! define the freqAxis for NonLinear plot
    while (true)
    {
        if (FFTQueueRN.try_pop(FFTDataRN) && tagRangeQueueRN.try_pop(tagRangeEstRN))
        {

            std::string tagRangeStringRN = "Tag Range : " + (std::to_string(tagRangeEstRN)).substr(0, 4) + "m";

            plt::clf();
            plt::title("Magnitude vs Freq NL");
            plt::semilogy(freqAxisRN, FFTDataRN);
            plt::xlabel("Frequency (Hz)");
            plt::xlim(2000, 60000);
            plt::ylim(10e1, 10e4);
            plt::text(2200.0, 10e4, tagRangeStringRN);
            plt::draw();
            plt::pause(0.01);
        }
    }

#endif // PLOT_NL
#endif // RN_THREAD

    ThreadReception.StopThreadReception();      // <! Stop the reception
    ThreadProcessing.StopThreadProcessing();    // <! Stop all the processing 

    UDP.Close_Socket(sock); // <! close the socket
    return 0;
}