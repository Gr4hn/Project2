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

//Namespace standards
using namespace std;
using namespace std::literals::chrono_literals;
using namespace chrono;

//Defince a mutex
mutex protectData;

//temperatur Celsius double, Luftfuktighet % double, vindhastighet = m/s double
struct Values {
    double temp;
    double airMoist;
    double windSpeed;
};

//Våra värden ska hela tiden kunna hålla 10 värden för att behållas som uppdaterade i realtid.
//Gamla värden ska raderas för att sedan uppdateras med nya värden.
queue<Values> data;

//Definitioner av funktioner
void tempReading (); // Punkt 1 i uppgiften
void airMoistReading (); // Punkt 1 i uppgiften
void windSpeedReading (); // Punkt 1 i uppgiften
void popValues (queue<Values>& data); // Punkt 2 i uppgiften
void addValues (queue<Values>& data); // Punkt 2 i uppgiften
void collectData (queue<Values>& data); // Punkt 3 i uppgiften
void displayData (queue<Values>& data); // Punkt 4 i uppgiften

/* data.push();
data.push(); */

//queue:
    //pop = Removes the first element
    //front = Access the first element
    //back = Access the last element





#endif

