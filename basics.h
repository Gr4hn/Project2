/**
 * @file basics.h
 * @author Eric Grahn (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef BASICDEFINITIONS
#define BASICDEFINITIONS
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <atomic>
#include <iomanip>

/**
 * @brief Testar att skriva en kommentar
 * 
 */

//Namespace standards
using namespace std;
using namespace std::literals::chrono_literals;
using namespace chrono;

//Define a mutex
mutex protectData;

//Define atomic
atomic<bool> running(true);
atomic<bool> dataReady(false);

//temperatur Celsius double, Luftfuktighet % double, vindhastighet = m/s double
struct Values {
    double temp = 0.0;
    double humidity = 0.0;
    double windSpeed = 0.0;
};

//Definitioner av funktioner
void tempReading (Values& dataSet); // Punkt 1 i uppgiften
void humidityReading (Values& dataSet); // Punkt 1 i uppgiften
void windSpeedReading (Values& dataSet); // Punkt 1 i uppgiften
void collectData (vector<Values>& data, Values& dataSet); // Punkt 3 i uppgiften
void displayData (vector<Values>& data); // Punkt 4 i uppgiften
void displayStatistics(vector<Values>& data);
void resetTempValues(Values &dataSet);

#endif

