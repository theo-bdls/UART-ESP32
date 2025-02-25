/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/

#include "ThreadReception.h"

using namespace std;


/////// Constructor ///////
threadReception::threadReception()
{
}


////// Destructor /////
threadReception::~threadReception()
{
}

/**
 * Start the thread  for the reception 
 * 
 * IN : 
 * 
 * socket : created in the main
 * fullPath : created in the main 
 * 
 * dataQueue : 4 differents type of queue handle by a library to push and pop easily
 */
void threadReception::StartThreadReception(int socket, const char *fullPath, tbb::concurrent_bounded_queue<std::string> &dataQueueRT, tbb::concurrent_bounded_queue<std::string> &dataQueueRS, tbb::concurrent_bounded_queue<std::string> &dataQueueRH, tbb::concurrent_bounded_queue<std::string> &dataQueueRN)
{
    m_threadReception = std::thread(&threadReception::RunThreadReception, this, socket, fullPath, std::ref(dataQueueRT), std::ref(dataQueueRS), std::ref(dataQueueRH), std::ref(dataQueueRN));
    printf("Thread Reception started\n");
}

/**
 * Stop the thread and join 
 */
void threadReception::StopThreadReception()
{
    if(m_threadReception.joinable())
    {
        m_threadReception.join();
        printf("Thread Reception stopped\n");
    }
}

/**
 * Writing to a file and sorting each sentence in the appropriate queue
 *
 * IN :
 *
 * socket : sock ID : int
 * fullPath : full path of the file to write
 * buffer : buffer to store the data
 * dataQueue : 4 differents type of queue handle by a library to push and pop easily
 *
 * OUT :
 *
 * 4 queues
 *
 */
void threadReception::ReceiveStorageData(int socket, char *buffer, const char *fullPath, tbb::concurrent_bounded_queue<std::string> &dataQueueRT, tbb::concurrent_bounded_queue<std::string> &dataQueueRS, tbb::concurrent_bounded_queue<std::string> &dataQueueRH, tbb::concurrent_bounded_queue<std::string> &dataQueueRN)
{
    m_udp.Receive_Data(socket, buffer);
    m_storage.Write_Text_File(fullPath, (char*)buffer);

#if defined(RT_THREAD) || defined(RS_THREAD) || defined(RH_THREAD) || defined(RN_THREAD) //if one of the thread processing is define
    std::istringstream iss(buffer);

    static const std::string RTString = "$RT";
    static const std::string RSString = "$RS";
    static const std::string RHString = "$RH";
    static const std::string RNString = "$RN";

    std::string line;
    std::string completeLine;

    std::string content = iss.str();
    size_t size = content.size();
    size_t i = 0;

    char c;

    for (i; i<size ;i++)
    {
        iss.get(c);
        line+=c;
        if (line.back()=='\n')
        {
            if(!m_incompleteLine.empty())
            {
                completeLine = m_incompleteLine + line;
                m_incompleteLine.clear();
            }
            else 
            {
                completeLine = line;
            }

            //std::cout <<"Complete Line Extracted : " << completeLine << std::endl;

#ifdef RT_THREAD
            if (completeLine.find(RTString) != std::string::npos)
            {
                dataQueueRT.push(completeLine);
            }
#endif //RT_THREAD
#ifdef RS_THREAD
            if (completeLine.find(RSString) != std::string::npos)
            {
                dataQueueRS.push(completeLine);
            }
#endif //RS_THREAD
#ifdef RH_THREAD
            if (completeLine.find(RHString) != std::string::npos)
            {
                dataQueueRH.push(completeLine);
            }
#endif //RH_THREAD
#ifdef RN_THREAD
            if (completeLine.find(RNString) != std::string::npos)
            {
                dataQueueRN.push(completeLine);
            }
#endif //RN_THREAD
            
            line.clear();
        }
    }

    if (!line.empty() && line.back()!='\n')
    {
        m_incompleteLine += line;
    }
#endif //THREAD_PROCESS
}

/**
 * Run the function, define the buffer and protected the loop
 */
void threadReception::RunThreadReception(int socket, const char *fullPath, tbb::concurrent_bounded_queue<std::string> &dataQueueRT, tbb::concurrent_bounded_queue<std::string> &dataQueueRS, tbb::concurrent_bounded_queue<std::string> &dataQueueRH, tbb::concurrent_bounded_queue<std::string> &dataQueueRN)
{
    while (true) 
    {
        char buffer[m_udp.getSocketParameters().bufferSize];
        ReceiveStorageData(socket, buffer, fullPath, dataQueueRT, dataQueueRS, dataQueueRH, dataQueueRN);
        std::this_thread::yield();
    }

}
