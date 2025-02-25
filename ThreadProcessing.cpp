/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/

#include "ThreadProcessing.h"

basicTools Tools;

// Constructor
threadProcessing::threadProcessing()
{
}

// Destructor
threadProcessing::~threadProcessing()
{
}

/**
 * Start the thread for processing data RT
 * 
 * IN : 
 * 
 * dataQueue : Queue to store the RT sentence
 */
void threadProcessing::StartThreadProcessingRT(tbb::concurrent_bounded_queue <std::string> & dataQueue)
{
    m_threadProcessingRT = std::thread(&threadProcessing::RunThreadProcessingRT, this, std::ref(dataQueue));
    printf("Thread Processing started RT\n");
}

/**
 * Start the thread for processing data RS
 *
 * IN :
 *
 * dataQueue : Queue to store the RS sentence
 */
void threadProcessing::StartThreadProcessingRS(tbb::concurrent_bounded_queue <std::string> & dataQueue)
{
    m_threadProcessingRS = std::thread(&threadProcessing::RunThreadProcessingRS, this, std::ref(dataQueue));
    printf("Thread Processing started RS\n");
}

/**
 * Start the thread for processing data RH
 *
 * IN :
 *
 * dataQueue : Queue to store the RH sentence
 *
 * FFTQueueRH : pushing 1x128 vector that contains FFT positive value
 *
 * tagRangeQueueRH : pushing tagRange estimation after the all processing
 *
 * Those two queues are used in the main and define in the main
 */
void threadProcessing::StartThreadProcessingRH(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRH, tbb::concurrent_bounded_queue<double> &tagRangeQueueRH)
{
    m_threadProcessingRH = std::thread(&threadProcessing::RunThreadProcessingRH, this, std::ref(dataQueue), std::ref(FFTQueueRH), std::ref(tagRangeQueueRH));
    printf("Thread Processing started RH\n");
}

/**
 * Start the thread for processing data RN
 *
 * IN :
 *
 * dataQueue : Queue contains the RN sentence
 * 
 * FFTQueueRN : pushing 1x128 vector that contains FFT positive value 
 * 
 * tagRangeQueueRN : pushing tagRange estimation after the all processing
 * 
 * Those two queues are used in the main and define in the main
 * 
 * 
 */
void threadProcessing::StartThreadProcessingRN(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRN, tbb::concurrent_bounded_queue<double> &tagRangeQueueRN)
{
    m_threadProcessingRN = std::thread(&threadProcessing::RunThreadProcessingRN, this, std::ref(dataQueue), std::ref(FFTQueueRN), std::ref(tagRangeQueueRN));
    printf("Thread Processing started RN\n");
}

/**
 * Stop the thread and join
 * 
 * Kill the thread
 * 
 * Calling this function AFTER plot and AFTER kill reception thread
 */
void threadProcessing::StopThreadProcessing()
{
    if (m_threadProcessingRT.joinable())
    {
        m_threadProcessingRT.join();
        printf("Thread Processing RT stopped\n");
    }
    if (m_threadProcessingRS.joinable())
    {
        m_threadProcessingRS.join();
        printf("Thread Processing RS stopped\n");
    }
    if (m_threadProcessingRH.joinable())
    {
        m_threadProcessingRH.join();
        printf("Thread Processing RH stopped\n");
    }
    if (m_threadProcessingRN.joinable())
    {
        m_threadProcessingRN.join();
        printf("Thread Processing RN stopped\n");
    }
}

/**
 * Process the data from the RT queue
 * 
 * For the moment, not using RT sentence. 
 * Should be helpful for angular estimation
 */
void threadProcessing::ProcessDataRT(tbb::concurrent_bounded_queue<std::string> & dataQueue)
{
    std::string RTSentence;
    dataQueue.pop(RTSentence);
    
}

/**
 * Process the data from the RS queue
 * 
 * For the moment, not using the RS sentence. 
 */
void threadProcessing::ProcessDataRS(tbb::concurrent_bounded_queue <std::string> & dataQueue)
{
    std::string RSSentence;
    dataQueue.pop(RSSentence);
    
}
/**
 * Process the data from the RH queue
 *
 * This function extract 10 blocks of data in a RH sentence. Make all the
 * checks to be sure that we have a full block with consecutive sentences.
 * It processes a full block as a 1 x n vector by performing the following steps:
 * - abs(FFT)
 * - movemean(FFT)
 * - InterpolatedFFT
 * - Estimate the tag range
 * It pushes the movemean(FFT) block into a queue (1 x 128 vector) every NB_PLOT blocks for plotting in the main.
 * It also pushes the tag range into a queue (double value) every NB_PLOT blocks for plotting in the main.
 *
 * IN :
 *
 * dataQueue : contains RH sentence format string
 *
 * FFTQueueRH : pushing 1x128 vector that contains FFT positive value
 *
 * tagRangeQueueRH : pushing tagRange estimation after the all processing
 *
 */
void threadProcessing::ProcessDataRH(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRH, tbb::concurrent_bounded_queue<double> &tagRangeQueueRH)
{
    std::string RHSentence;
    // Store the sentence one by one in a string
    dataQueue.pop(RHSentence);

    
    std::vector<std::string> RHSplitSentence;       // <! Vectors 1 x 14 
    std::vector<std::string> RHDataCorrected;       // <! Vectors 1 x 250 with sign corrected (one block of data)
    std::vector<int16_t> HarmonicRadDataInt;        // <! Vector 1 x 250 in int16 format
    std::vector<int16_t> HarmonicRadDataIntPad;     // <! Vector 1 x 256 in int16 format
    std::vector<double> WinRadarDataBlock;          // <! Vector 1 x 256 hanning window applied

    double freqRes = SAMPLING_FREQ / FFT_VECTOR_SIZE;  // <! Resolution of the fft
    std::vector<double> freqAxis = Tools.generateFreqAxis(freqRes, SAMPLING_FREQ); // <! Freq axis for Harmonic data, 1 x 128 vector

    std::vector<double> fftAbs;             // <! absolute value of the FFT, 1 x 256 vector
    std::vector<double> fftMoveMean;        // <! movemean of the fft apply to X data block, 1 x 256
    std::vector<double> filtFFTMoveMean;    // <! filter apply to FFT after movemean, 1 x256
    InterPolFFT freqData;                   // <! Struct that contains the maximum peak and the freq of this peak
    double tagRange;                        // <! estimation of the range of the tag

    RHSplitSentence = Tools.StrSplitSentence(RHSentence); // <! 1x14 vector of string

    std::string typeOfBlock = RHSplitSentence[0];         // <! Should return : "$RH" 

    int result = Tools.CheckSum(RHSplitSentence);
    if (result == 1)
    {
        std::string id = RHSplitSentence[1];        // <! Example : "303" block 303
        int index = std::stoi(RHSplitSentence[2]);  // <! Example : "01" sentence 01

        if (m_currentBlock.empty())
        {
            if (index == 1)
            {
                m_currentID = id;
                m_expectedIndex = 1;
            }
            //printf("NEW BLOCK\n"); //////// DEBUG MESSAGE
        }

        if (id == m_currentID && index == m_expectedIndex)
        {
            for (int i = 3; i < 13; i++)
            {
                m_currentBlock.push_back(RHSplitSentence[i]);
            }
            m_expectedIndex++;
            //printf("Block ID : %s Expected index : %d\n", m_currentID.c_str(), m_expectedIndex); //////// DEBUG MESSAGE

            if (m_expectedIndex > NB_SENTENCE_RH) // if ok : m_currentBlock 1 x 250
            {
                //printf("Processing RH\n");
                RHDataCorrected = Tools.CheckSign(m_currentBlock);                                  // <! 1 x 250 vector
                HarmonicRadDataInt = Tools.HexToInt16(RHDataCorrected);                             // <! 1 x 250 vector
                HarmonicRadDataIntPad = Tools.PadZeroData(HarmonicRadDataInt);                      // <! 1 x 256 vector 
                WinRadarDataBlock = Tools.WinRadarDataBlockPad(HarmonicRadDataIntPad, typeOfBlock); // <! 1 x 256 with hanning
                fftAbs = Tools.computeFFTHR(WinRadarDataBlock);                                       // <! 1 x 256 vector with abs fft value
                fftMoveMean = Tools.MoveMeanVectorHR(fftAbs);                                         // <! 1 x 256 vector : 10 blocks for the moving average

                if (!Tools.isZeroVector(fftMoveMean)) // <! Befor having the first 10 sentences, fftMoveMean == 0
                {
                    filtFFTMoveMean = Tools.FilterFFT(fftMoveMean, CBIN);                 // <!put the first CBIN value to zero 1 x 256 vector
                    freqData = Tools.InterpolatedFFT(filtFFTMoveMean, freqRes, freqAxis); // <! 1 x 256 vector 
                    tagRange = (freqData.PeakFreq * TSWEEP * C) / (4 * BW);               // <! Range estimation : need to add cable delay

                    // IF YOU WANT TO SEE THE VALUE IN THE TERMINAL UNCOMMENT THE NEXT LINE 
                    printf("The estimated frequency is %0.4f KHz & range is %.04f m\n", freqData.PeakFreq / 1000, tagRange);

#ifdef RH_PLOT
                    m_processCounter++;
                    if (m_processCounter >= NB_PLOT)
                    {
                        std::vector<double> symetricFFT(fftMoveMean.begin(), fftMoveMean.begin() + 128); // <! process for plot only half fft data
                        FFTQueueRH.push(symetricFFT);           // <! Push to the queue for the plot (only in the main)
                        tagRangeQueueRH.push(tagRange);         // <! Push to the queue for the plot (only in the main)
                        m_processCounter = 0;                   // <! Ready for the next NB_PLOT lines
                    }
#endif //RH_PLOT
                }
                m_expectedIndex = 1;    // <! A full block has been proceed
                m_currentBlock.clear(); // <! Clear for the next block 
            }
        }
    }
}

/**
 * Process the data from the RN queue
 *
 * This function extracts 10 blocks of data in an RN sentence. It performs
 * all necessary checks to ensure that we have a full block with consecutive sentences.
 * It processes a full block as a 1 x n vector by performing the following steps:
 * - abs(FFT)
 * - movemean(FFT)
 * - InterpolatedFFT
 * - Estimate the tag range
 * It pushes the movemean(FFT) block into a queue (1 x 128 vector) every NB_PLOT blocks for plotting in the main.
 * It also pushes the tag range into a queue (double value) every NB_PLOT blocks for plotting in the main.
 *
 * IN :
 *
 * dataQueue : contains RN sentence format string
 *
 * FFTQueueRN : queue to push 1 x 128 vector containing FFT positive values
 *
 * tagRangeQueueRN : queue to push tag range estimation after all processing
 *
 */
void threadProcessing::ProcessDataRN(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRN, tbb::concurrent_bounded_queue<double> &tagRangeQueueRN)
{
    std::string RNSentence;
    // Store the sentence one by one in a string
    dataQueue.pop(RNSentence);

    std::vector<std::string> RNSplitSentence;       // <! 1x14 vector of string
    std::vector<std::string> RNDataCorrected;       // <! 1x130 vector with corrected sign
    std::vector<int16_t> NonLinearRadDataInt;       // <! 1x130 vector in int16 format
    std::vector<int16_t> NonLinearRadDataIntPad;    // <! 1x256 vector in int16 format
    std::vector<double> WinNonLinearRadarDataBlock; // <! 1x256 vector with hanning window applied

    double freqResNL = SAMPLING_FREQ / FFT_VECTOR_SIZE;                                        // <! Resolution of the FFT
    std::vector<double> freqAxisNL = Tools.generateFreqAxis(freqResNL / 2, SAMPLING_FREQ / 2); // <! Frequency axis for non-linear data, 1x128 vector

    std::vector<double> NLfftAbs;          // <! Absolute value of the FFT, 1x256 vector
    std::vector<double> NLfftMoveMean;     // <! Movemean of the FFT applied to X data block, 1x256 vector
    std::vector<double> NLfiltFFTMoveMean; // <! Filter applied to FFT after movemean, 1x256 vector
    InterPolFFT freqDataNL;                // <! Struct containing the maximum peak and the frequency of this peak
    double tagRangeNL;                     // <! Estimation of the tag range

    RNSplitSentence = Tools.StrSplitSentence(RNSentence); // <! 1x14 vector of string

    std::string typeOfBlock = RNSplitSentence[0]; // <! Should return: "$RN"

    int result = Tools.CheckSum(RNSplitSentence);
    if (result == 1)
    {
        std::string idNL = RNSplitSentence[1];       // <! Example: "303" block 303
        int indexNL = std::stoi(RNSplitSentence[2]); // <! Example: "13" sentence 13

        if (m_currentBlockNL.empty())
        {
            if (indexNL == 1)
            {
                m_currentIDNL = idNL;
                m_expectedIndexNL = 1;
            }
            // printf("NEW BLOCK\n"); //////// DEBUG MESSAGE
        }

        if (idNL == m_currentIDNL && indexNL == m_expectedIndexNL)
        {
            for (int i = 3; i < 13; i++)
            {
                m_currentBlockNL.push_back(RNSplitSentence[i]);
            }
            m_expectedIndexNL++;
            // printf("Block ID : %s Expected index : %d\n", m_currentIDNL.c_str(), m_expectedIndexNL); //////// DEBUG MESSAGE

            if (m_expectedIndexNL > NB_SENTENCE_RN) // if ok: m_currentBlockNL 1x130
            {
                // printf("Processing RN\n");
                RNDataCorrected = Tools.CheckSign(m_currentBlockNL);                                          // <! 1x130 vector
                NonLinearRadDataInt = Tools.HexToInt16(RNDataCorrected);                                      // <! 1x130 vector
                NonLinearRadDataIntPad = Tools.PadZeroData(NonLinearRadDataInt);                              // <! 1x256 vector
                WinNonLinearRadarDataBlock = Tools.WinRadarDataBlockPad(NonLinearRadDataIntPad, typeOfBlock); // <! 1x256 with hanning
                NLfftAbs = Tools.computeFFTNL(WinNonLinearRadarDataBlock);                                      // <! 1x256 vector with abs FFT value
                NLfftMoveMean = Tools.MoveMeanVectorNL(NLfftAbs);                                               // <! 1x256 vector : 10 blocks for the moving average

                if (!Tools.isZeroVector(NLfftMoveMean)) // Proceed only when we have the first 10 sentences
                {
                    NLfiltFFTMoveMean = Tools.FilterFFT(NLfftMoveMean, CBIN);                     // <! Put the first CBIN value to zero, 1x256 vector
                    freqDataNL = Tools.InterpolatedFFT(NLfiltFFTMoveMean, freqResNL, freqAxisNL); // <! 1x256 vector
                    tagRangeNL = (freqDataNL.PeakFreq * TSWEEP * C) / (2 * BW);                   // <! Range estimation: need to add cable delay

                    // IF YOU WANT TO SEE THE VALUE IN THE TERMINAL UNCOMMENT THE NEXT LINE
                    printf("The estimated frequency is %0.4f KHz & range is %.04f m\n", freqDataNL.PeakFreq / 1000, tagRangeNL);
                
#ifdef RN_PLOT
                    m_processCounterNL++;
                    if (m_processCounterNL >= NB_PLOT)
                    {
                        std::vector<double> NLsymetricFFT(NLfftMoveMean.begin(), NLfftMoveMean.begin() + 128); // <! Process for plot only half FFT data
                        FFTQueueRN.push(NLsymetricFFT);                                                        // <! Push to the queue for the plot (only in the main)
                        tagRangeQueueRN.push(tagRangeNL);                                                      // <! Push to the queue for the plot (only in the main)
                        m_processCounterNL = 0;                                                                // <! Ready for the next NB_PLOT lines
                    }
#endif //RN_PLOT
                }

                m_expectedIndexNL = 1;    // <! A full block has been processed
                m_currentBlockNL.clear(); // <! Clear for the next block
            }
        }
    }
}

/**
 * Run the function in a loop to process data
 */
void threadProcessing::RunThreadProcessingRT(tbb::concurrent_bounded_queue<std::string> & dataQueue)
{
    while (true)
    {
        ProcessDataRT(dataQueue);
        std::this_thread::yield();
    }
}

/**
 * Run the function in a loop to process data
 */
void threadProcessing::RunThreadProcessingRS(tbb::concurrent_bounded_queue<std::string> & dataQueue)
{
    while (true)
    {
        ProcessDataRS(dataQueue);
        std::this_thread::yield();
    }
}

/**
 * Run the function in a loop to process data
 */
void threadProcessing::RunThreadProcessingRH(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRH, tbb::concurrent_bounded_queue<double> &tagRangeQueueRH)
{
    while (true)
    {
        ProcessDataRH(dataQueue, FFTQueueRH, tagRangeQueueRH);
        std::this_thread::yield();
    }
}

/**
 * Run the function in a loop to process data
 */
void threadProcessing::RunThreadProcessingRN(tbb::concurrent_bounded_queue<std::string> &dataQueue, tbb::concurrent_bounded_queue<std::vector<double>> &FFTQueueRN, tbb::concurrent_bounded_queue<double> &tagRangeQueueRN)
{
    while (true)
    {
        ProcessDataRN(dataQueue, FFTQueueRN, tagRangeQueueRN);
        std::this_thread::yield();
    }
}
