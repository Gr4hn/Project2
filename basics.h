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

struct Statistics {
    double minTemp = numeric_limits<double>::max();
    double maxTemp = numeric_limits<double>::lowest();
    double avgTemp = 0.0;
    double minAirMoist = numeric_limits<double>::max();
    double maxAirMoist = numeric_limits<double>::lowest();
    double avgAirMoist = 0.0;
    double minWindSpeed = numeric_limits<double>::max();
    double maxWindSpeed = numeric_limits<double>::lowest();
    double avgWindSpeed = 0.0;
};

//Våra värden ska hela tiden kunna hålla 10 värden för att behållas som uppdaterade i realtid.
//Gamla värden ska raderas för att sedan uppdateras med nya värden.
//queue<Values> data;

vector<Values> data(10);



//Definitioner av funktioner
void tempReading (Values& dataSet); // Punkt 1 i uppgiften
void airMoistReading (Values& dataSet); // Punkt 1 i uppgiften
void windSpeedReading (Values& dataSet); // Punkt 1 i uppgiften
void collectData (vector<Values>& data, Values& dataSet); // Punkt 3 i uppgiften
void displayData (vector<Values>& data, Statistics &stats); // Punkt 4 i uppgiften
void resetTempValues(Values &dataSet);
void calculateData (vector<Values>& data, Statistics &stats);

/* data.push();
data.push(); */

//queue:
    //pop = Removes the first element
    //front = Access the first element
    //back = Access the last element





#endif

