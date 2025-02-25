/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/


#include "BasicTools.h"

// Constructor
basicTools::basicTools()
{
}

// Destructor
basicTools::~basicTools()
{
}

//////// Function ////////
/**
 * Dec To Hex function
 * 
 * IN :
 * 
 * decNumber : decimal number input
 * 
 * OUT : 
 * 
 * hexNumber : string format 
 */
std::string basicTools::DecToHex(uint8_t decNumber)
{
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(decNumber);
    return ss.str();
}

/**
 * Hex to Int16 function for a vector 
 * 
 * IN : 
 * 
 * fullBlockDataCorrected : hexadecimal number vector : size 1 x n 
 * 
 * Convert string to int16 after adding the digit for the sign for each elements
 * 
 * OUT :
 * 
 * vector of int16_t : size 1 x n
 */
std::vector<int16_t> basicTools::HexToInt16(std::vector<std::string> fullBlockDataCorrected)
{

    std::vector<int16_t> fullBlockDataInt16(fullBlockDataCorrected.size());
    for (int i = 0; i < fullBlockDataCorrected.size(); i++)
    {
        fullBlockDataInt16[i] = std::stoi(fullBlockDataCorrected[i], nullptr, 16);
    }
    return fullBlockDataInt16;
}

/**
 * Dec to Hex function
 * hexNumber : hexadecimal number
 * 
 * IN : 
 * 
 * hexNumber : hexNumber in string format
 * 
 * OUT : 
 * 
 * decimal : int
 */
int basicTools::HexToDec(std::string hexNumber)
{
    uint16_t result;
    std::stringstream ss;
    ss << std::hex << hexNumber;
    ss >> result;
    return static_cast<int>(result);
}

/**
 * Check vector = vector null
 * 
 * IN :
 * 
 * vector : vector of double 1 x n
 * 
 * OUT : 
 * 
 * 1 if vector nul
 * 
 * 0 if vector non nul
 */
bool basicTools::isZeroVector(std::vector<double> vector)
{
    return std::all_of(vector.begin(), vector.end(), [](double value){return value == 0.0; });
}

/**
 * Check that the sentence is not corrupted
 * 
 * CheckSum : last two digits of a sentence radar data
 * Cumulative xor of the data of a sentence
 * 
 * IN : 
 * 
 * sentence : data sentence after split = vector of 1 x n size 
 * 
 * OUT : 
 * 
 * 1 if checksum ok 
 * 
 * 0 if checksum failed 
 * 
 * BE AWARE THAT THERE IS EXTRA CHARACTERS AT THE END OF THE SENTENCE 
 * '\r', '\n', '\0' are some examples 
 * If checksum failed verify the strpsplit delimiters
 */
int basicTools::CheckSum(std::vector<std::string> sentence)
{
    // Obtain the data fields in a sentence, convert to uint8 and perform
    // cumulative XOR and match with the CheckSum of the sentence

    // Calculate the length of the input string
    std::string D1str;
    for (const auto& str : sentence) {
        D1str = D1str + str.c_str();
    }
    size_t D1Len= D1str.length();

    // Compute the checksum
    uint8_t CumXor = (uint8_t)(D1str[1]);

    for (size_t ii = 2; ii < D1Len - 2; ++ii)
    {
        uint8_t D1uint8 = (uint8_t)(D1str[ii]);
        CumXor = CumXor ^ D1uint8;
    }

    // Convert the XOR result into hex
    std::string chkSumComputed = DecToHex(CumXor);

    // Compare with the checksum
    std::string expectedChecksum = D1str.substr(D1Len - 2, 2);
    if (chkSumComputed == expectedChecksum)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Check the sign of the data (4:13) for the index in RN and RH sentence
 * 
 * IN : 
 * 
 * fullBlockData : vector of hex digit : string format 1 x n
 * 
 * When we have 25 sentences of RH or 13 sentences of RN
 * Add an extra digit for the sign 
 * 
 * OUT : 
 * 
 * vector of hex digit + digit for sign : string format 1 x n 
 */
std::vector<std::string> basicTools::CheckSign(std::vector<std::string> fullBlockData)
{
    std::vector<std::string> fullBlockDataCorrected(fullBlockData.size());
    for (int i = 0; i < fullBlockData.size(); i++ )
    {
        if (HexToDec(fullBlockData[i]) > HexToDec("7FF"))
        {
            fullBlockDataCorrected[i] = "F" + fullBlockData[i];
        }
        else 
        {
            fullBlockDataCorrected[i] = "0" + fullBlockData[i];
        }
    }
    return fullBlockDataCorrected;
}

/**
 * Return a vector with cells corresponding to the delimiters
 * Example : "$RH,126,02" -> ["RH","126","02"] 1 x 3 vector 
 * 
 * Need to use regex because it's thread safe
 * 
 * IN : 
 * 
 * sentence : RH sentence or RN sentence popped from the queue 
 * 
 * OUT : 
 * 
 * vector of string : 1 x 14 for RH and RN because we have 14 cells 
 */

std::vector<std::string> basicTools::StrSplitSentence(std::string sentence)
{
    std::vector<std::string> tokens;
    std::regex regex("[^,*\\r\\n]+");
    std::sregex_token_iterator it(sentence.begin(), sentence.end(), regex);
    std::sregex_token_iterator end;
    while (it != end)
    {
        tokens.push_back(*it++);
    }
    return tokens;
}

/**
 * Add zero at the end of a vector to reach a pow of 2 
 * 
 * IN : 
 * 
 * vec : vector of int16_t : 1 x n 
 * 
 * OUT : 
 * 
 * vector of int16_t : 1 x m 
 * 
 * m = n + number of zero need 
 * 
 * Example : RH data : IN : 1x250 -> OUT : 1x256
 *           RN data : IN : 1:130 -> OUT : 1x256
 */
std::vector<int16_t> basicTools::PadZeroData(std::vector<int16_t> vec)
{
    size_t currentSize = vec.size();
    double n = std::ceil(std::log2(currentSize));
    size_t newSize = std::pow(2, n);
    vec.resize(newSize, 0);
    return vec;
}

/**
 * Creation of hanning window depending of the size of the input
 * Just need the size of the vector in this function
 * 
 * IN : 
 * 
 * dataBlockPad : vector of int16_t : 1 x n with n power of 2 
 * dataBlockPad is the data block with int16 signed
 * 
 * OUT : 
 * 
 * vector of double : 1 x 256 for RH and RN 
 * Constant vector : hanning are constant value
 * 
 * Hanning window because error is lower when using Agrez's method and 
 * a hanning window 
 */
std::vector<double> basicTools::WinHanning(std::vector<int16_t> dataBlockPad)
{
    std::vector<double> winHanningWindow(dataBlockPad.size() + 1);
    for (double i=1; i <= dataBlockPad.size(); i++)
    {
        winHanningWindow[i] = 0.5*(1 - std::cos(2.0 * M_PI * i /(dataBlockPad.size()+1)));
    }
    std::vector<double> winHanningWithoutZero(winHanningWindow.begin() + 1, winHanningWindow.end());
    return winHanningWithoutZero;
}

/**
 * Process the hanning window on a vector and remove offset if necessary
 * 
 * IN : 
 * 
 * dataBlockPad : vector of int16_t : 1 x n with n pow of 2 
 * RH : vector size 1 x 256
 * 
 * typeOfBlock : string contains the first element of strsplit function on a radar sentence
 * "$RH" or "$RN"
 */
std::vector<double> basicTools::WinRadarDataBlockPad(std::vector<int16_t> dataBlockPad, std::string typeOfBlock)
{
    size_t dataBlkSize =  dataBlockPad.size();
    std::vector<double> hanningWindow = WinHanning(dataBlockPad);
    std::vector<double> winRadarDataBlockPad(dataBlkSize);
    for (size_t i=0; i<dataBlkSize; i++)
    {
        winRadarDataBlockPad[i] = static_cast<double>(dataBlockPad[i]) * hanningWindow [i];
    }

    if (typeOfBlock == "$RH")
    {
        double DCOffset = std::accumulate(winRadarDataBlockPad.begin(), winRadarDataBlockPad.end(), 0.0)/dataBlkSize;
        for (double& value : winRadarDataBlockPad)
        {
            value -= DCOffset;
        }
        return winRadarDataBlockPad;
    }
    else
    {
        return winRadarDataBlockPad;
    }
}

/**
 * Perform the FFT and Abs
 * 
 * IN :
 * 
 * winRadarDataBlock : 1 x 256
 * Data Block after applying the hanning window on all the coefficient
 * 
 * OUT : 
 * 
 * vector of double : abs value of the fft : 1 x n 
 * 
 * FFT symmetric : only 0:n/2 -1 will be used (InterpolatedFFT)
 */
std::vector<double> basicTools::computeFFTHR(std::vector<double> winRadarDataBlock)
{
    size_t N = winRadarDataBlock.size();

    fftw_complex *inHR = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex *freqOutHR = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

    for (size_t i = 0; i < N; ++i)
    {
        inHR[i][0] = winRadarDataBlock[i]; 
        inHR[i][1] = 0.0;    
    }

    fftw_plan pHR = fftw_plan_dft_1d(N, inHR, freqOutHR, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(pHR);

    std::vector<std::complex<double>> fftResultHR(N);
    for (size_t i = 0; i < N; ++i)
    {
        fftResultHR[i] = std::complex<double>(freqOutHR[i][0], freqOutHR[i][1]);
    }

    std::vector<double> fftAbsHR(N);
    for (size_t i = 0; i<N; ++i)
    {
        fftAbsHR[i] = std::abs(fftResultHR[i]);
    }

    fftw_destroy_plan(pHR);
    fftw_free(inHR);
    fftw_free(freqOutHR);

    return fftAbsHR;

}

/**
 * Perform the FFT and Abs
 *
 * IN :
 *
 * winRadarDataBlock : 1 x 256
 * Data Block after applying the hanning window on all the coefficient
 *
 * OUT :
 *
 * vector of double : abs value of the fft : 1 x n
 *
 * FFT symmetric : only 0:n/2 -1 will be used (InterpolatedFFT)
 */
std::vector<double> basicTools::computeFFTNL(std::vector<double> winRadarDataBlock)
{
    size_t N = winRadarDataBlock.size();

    fftw_complex *inNL = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex *freqOutNL = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

    for (size_t i = 0; i < N; ++i)
    {
        inNL[i][0] = winRadarDataBlock[i];
        inNL[i][1] = 0.0;
    }

    fftw_plan pNL = fftw_plan_dft_1d(N, inNL, freqOutNL, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(pNL);

    std::vector<std::complex<double>> fftResultNL(N);
    for (size_t i = 0; i < N; ++i)
    {
        fftResultNL[i] = std::complex<double>(freqOutNL[i][0], freqOutNL[i][1]);
    }

    std::vector<double> fftAbsNL(N);
    for (size_t i = 0; i < N; ++i)
    {
        fftAbsNL[i] = std::abs(fftResultNL[i]);
    }

    fftw_destroy_plan(pNL);
    fftw_free(inNL);
    fftw_free(freqOutNL);

    return fftAbsNL;
}

/**
 * Moving average on a vector harmonic data
 * VECTOR_AVG_HR : set in Parameters.h
 *
 * Waiting for having VECTOR_AVG_HR vectors before doing the mean
 * column by column.
 * Each time we are adding a new vectors and erasing the oldest one
 *
 * ////////////////////////////////////////////
 * Example : absFFTVector(t) = [1 2]
 *           absFFTVector(t+1) = [1 3]
 *           absFFTVector(t+2) = [3 4]
 *
 * VECTOR_AVG_HR = 2
 *
 * 1st call :
 * m_arrayMean = [absFFTVector(t) absFFTVector(t+1)]
 *
 * meanFFT = [1 2.5]
 *
 * 2nd call :
 * m_arrayMean = [absFFTVector(t+1) absFFTVector(t+2)]
 * 
 * meanFFT = [2 3.5]
 *
 * /////////////////////////////////////////////
 * IN :
 *
 * absFFTVector : vector of double : 1 x n
 *
 * m_arrayMean : member of the class : VECTOR_AVG_HR x n size
 * 
 * For the moment : m_arrayMean : 10 x 256
 *
 * OUT :
 *
 * vector of double : 1 x n
 */
std::vector<double> basicTools::MoveMeanVectorHR(std::vector<double> absFFTVector)
{
    size_t vectorSize = absFFTVector.size();
    std::vector<double> meanFFT(absFFTVector.size());

    if (m_arrayMeanHR.size() >= VECTOR_AVG_HR)
    {
        m_arrayMeanHR.erase(m_arrayMeanHR.begin());
    }

    m_arrayMeanHR.push_back(absFFTVector);

    if (m_arrayMeanHR.size() == VECTOR_AVG_HR)
    {
        for (const auto& vec : m_arrayMeanHR)
        {
            for (size_t i = 0; i < vectorSize; i++)
            {
                meanFFT[i] += vec[i];
            }
        }
        
        for (size_t i = 0; i < vectorSize; ++i)
        {
            meanFFT[i] /= VECTOR_AVG_HR;
        }
    }
    return meanFFT;
}

/**
 * Moving average on a vector non linear data
 * VECTOR_AVG : set in Parameters.h
 *
 * Waiting for having VECTOR_AVG vectors before doing the mean
 * column by column.
 * Each time we are adding a new vectors and erasing the oldest one
 *
 * ////////////////////////////////////////////
 * Example : absFFTVector(t) = [1 2]
 *           absFFTVector(t+1) = [1 3]
 *           absFFTVector(t+2) = [3 4]
 *
 * VECTOR_AVG = 2
 *
 * 1st call :
 * m_arrayMean = [absFFTVector(t) absFFTVector(t+1)]
 *
 * meanFFT = [1 2.5]
 *
 * 2nd call :
 * m_arrayMean = [absFFTVector(t+1) absFFTVector(t+2)]
 *
 * meanFFT = [2 3.5]
 *
 * /////////////////////////////////////////////
 * IN :
 *
 * absFFTVector : vector of double : 1 x n
 *
 * m_arrayMean : member of the class : VECTOR_AVG x n size
 *
 * For the moment : m_arrayMean : 10 x 256
 *
 * OUT :
 *
 * vector of double : 1 x n
 */
std::vector<double> basicTools::MoveMeanVectorNL(std::vector<double> absFFTVector)
{
    size_t vectorSize = absFFTVector.size();
    std::vector<double> meanFFT(absFFTVector.size());

    if (m_arrayMeanNL.size() >= VECTOR_AVG_NL)
    {
        m_arrayMeanNL.erase(m_arrayMeanNL.begin());
    }

    m_arrayMeanNL.push_back(absFFTVector);

    if (m_arrayMeanNL.size() == VECTOR_AVG_NL)
    {
        for (const auto &vec : m_arrayMeanNL)
        {
            for (size_t i = 0; i < vectorSize; i++)
            {
                meanFFT[i] += vec[i];
            }
        }

        for (size_t i = 0; i < vectorSize; ++i)
        {
            meanFFT[i] /= VECTOR_AVG_NL;
        }
    }
    return meanFFT;
}

/**
 * Function to filter the FFT 
 * In this case eliminate the low frequencies 
 * 
 * IN : 
 * 
 * filterParameter : number of coefficient to put to zero
 * 
 * meanFFT : vector of double : 1 x n that contains the mean absolute FFT of 10 blocks 
 * 
 * OUT : 
 * 
 * vector of double : 1 x n 
 */
std::vector<double> basicTools::FilterFFT(std::vector<double> meanFFT, int filterParameter)
{
    std::vector<double> filtMeanFFT = meanFFT;
    for (int i = 0; i < filterParameter; ++i)
    {
        filtMeanFFT[i] = 0;
    }
    return filtMeanFFT;
}

/**
 * Generation of the FreqAxis for positive frequency of the FFT (FFT center on zero)
 * 
 * IN : 
 * 
 * freqRes : resolution of the FFT : Parameters.h
 * 
 * sampleFreq : sampling frequency : Parameters.h
 * 
 * OUT : 
 * 
 * vector of double : 1 x (1/2)*sampleFreq/freqRes : 1 x 128 for RH and RN 
 * 
 * But for RN sampleFreq/2 and freqRes/2
 * 
 */
std::vector<double> basicTools::generateFreqAxis(double freqRes, double sampleFreq)
{
    std::vector<double> freqAxis;
    for (double freq = 0; freq < sampleFreq /2; freq += freqRes)
    {
        freqAxis.push_back(freq);
    }
    return freqAxis;
}

/**
 * Agrez's methode : interpolation of DFT to find the real frequency of the maximum
 * 
 * Determination of a delta using 3 points interpolation 
 * Delta = offset between actual frequency of the max and real one 
 * 
 * IN : 
 * 
 * filtFFTMMoveMean : vector of double : 1 x 256 with 10 block averaging for each coefficient 
 * 
 * freqRes : Resolution of the FFT 
 * 
 * freqAxis : vector of double : freq axis 
 * 
 * OUT : 
 * 
 * struct PeakVal : Max amplitude peak for the FFT
 *        PeakFreq : Freq with offset to find the real frequency of the maximum peak
 */
InterPolFFT basicTools::InterpolatedFFT(std::vector<double> filtFFTMoveMean, double freqRes, std::vector<double> freqAxis)
{
    double GPeak = filtFFTMoveMean[0];
    size_t sizeFiltFFT = filtFFTMoveMean.size();
    double indexGPeak;

    for (size_t i = 0; i < sizeFiltFFT/2; ++i) // spectrum symmetric
    {
        if (filtFFTMoveMean[i] > GPeak)
        {
            GPeak = filtFFTMoveMean[i];
            indexGPeak = i;
        }
        else 
        {
            GPeak = GPeak;
        }
    }
    double num = 2 * (std::abs(filtFFTMoveMean[indexGPeak +1]) - std::abs(filtFFTMoveMean[indexGPeak-1]));
    double den = std::abs(filtFFTMoveMean[indexGPeak + 1]) + 2 * (std::abs(filtFFTMoveMean[indexGPeak])) + std::abs(filtFFTMoveMean[indexGPeak - 1]);
    double delta = num/den;

    InterPolFFT Out;

    Out.PeakVal = GPeak; // TO DO : use Agrez's method to calculate Gpeak
    Out.PeakFreq = freqAxis[indexGPeak] + delta*freqRes;

    return Out;
}


