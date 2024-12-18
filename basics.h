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
    double airMoist = 0.0;
    double windSpeed = 0.0;
};

//Definitioner av funktioner
void tempReading (Values& dataSet); // Punkt 1 i uppgiften
void airMoistReading (Values& dataSet); // Punkt 1 i uppgiften
void windSpeedReading (Values& dataSet); // Punkt 1 i uppgiften
void collectData (vector<Values>& data, Values& dataSet); // Punkt 3 i uppgiften
void displayData (vector<Values>& data); // Punkt 4 i uppgiften
void displayStatistics(vector<Values>& data);
void resetTempValues(Values &dataSet);

#endif

