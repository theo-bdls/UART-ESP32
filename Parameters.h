/* Written by Théo Bruneau de la Salle
 *
 * ENSEIRB-MATMECA student, France
 *
 * 2024
 */

#pragma once

#ifndef FULLPATH
#define FULLPATH "/Users/theobruneaudelasalle/Desktop/TestWithCpp/SaveData/DataSample_default.txt"
#endif

#ifndef IPCLIENT
#define IPCLIENT "192.168.0.2" //By default, central node IP address
#endif

#ifndef UDPPORT
#define UDPPORT 4001 //By default, central node UDP port
#endif

// Length of RT (Timestamp) data
constexpr int RTLENGTH = 26;

// Length of RS (Radar Signal) data
constexpr int RSLENGTH = 24;

// Number of RH (Radar Header) sentences
constexpr int NB_SENTENCE_RH = 25;

// Number of RN (Radar Navigation) sentences
constexpr int NB_SENTENCE_RN = 13;

// Length of radar data
constexpr int RADAR_DATA_LENGTH = 53;

// Number of vector average movemean
constexpr int VECTOR_AVG_HR = 10; // <! Impact the speed of the calcul

// Number of vector average movemean
constexpr int VECTOR_AVG_NL = 7; // <! Impact the speed of the calcul

// Number of column to put at zero low-freq filter
constexpr int CBIN = 8; // <! Depending on the experimantal conditions

// Sampling frequency in Hz
constexpr double SAMPLING_FREQ = 250e3;

// Size of the FFT vector
constexpr double FFT_VECTOR_SIZE = 256;

// Sweep time in seconds
constexpr double TSWEEP = 1e-3;

// Speed of light in meters per second
constexpr double C = 3e8;

// Bandwidth in Hz
constexpr double BW = 80e6;

// High-resolution cable delay in seconds
constexpr double HRCABLEDELAY = 0.78;

// Normal-low cable delay in seconds
constexpr double NLCABLEDELAY = 0.4;

// Number of block to wait for a plot 
constexpr int NB_PLOT = 15; // <! if NB_PLOT increase, speed of the plot decrease but may avoid segmentation fault
