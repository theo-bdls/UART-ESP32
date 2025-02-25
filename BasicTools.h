/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/

#pragma once 

#include "Parameters.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <vector>
#include <numeric>
#include <fftw3.h>
#include <complex>
#include <regex>


struct InterPolFFT{
double PeakFreq;
double PeakVal;
}
;


class basicTools
{

public:
    ///// Constructor/Destructor /////
    basicTools();  // Constructor
    ~basicTools(); // Destructor

    ///// Function declaration //////
    std::string DecToHex(uint8_t decNumber);
    std::vector<int16_t> HexToInt16(std::vector<std::string> fullBlockDataCorrected);
    int HexToDec(std::string hexNumber);
    bool isZeroVector(std::vector<double> vector);

    int CheckSum(std::vector<std::string> sentence);    
    std::vector<std::string> CheckSign(std::vector<std::string> fullBlockData);

    std::vector<std::string> StrSplitSentence(std::string sentence);

    std::vector<int16_t> PadZeroData(std::vector<int16_t> vec);
    std::vector<double> WinHanning(std::vector<int16_t> dataBlockPad);
    std::vector<double> WinRadarDataBlockPad(std::vector<int16_t> dataBlockPad, std::string typeOfBlock);

    std::vector<double> computeFFTHR(std::vector<double> winRadarDataBlock);
    std::vector<double> computeFFTNL(std::vector<double> winRadarDataBlock);
    std::vector<double> MoveMeanVectorHR(std::vector<double> absFFTVector);
    std::vector<double> MoveMeanVectorNL(std::vector<double> absFFTVector);
    std::vector<double> FilterFFT(std::vector<double> meanFFT, int filterParameter);
    std::vector<double> generateFreqAxis(double freqRes, double sampleFreq);
    InterPolFFT InterpolatedFFT(std::vector<double> filtFFTMoveMean, double freqRes, std::vector<double> freqAxis);

private:
    std::vector<std::vector<double>> m_arrayMeanHR; // <! array to do the averaging
    std::vector<std::vector<double>> m_arrayMeanNL; // <! array to do the averaging

};
