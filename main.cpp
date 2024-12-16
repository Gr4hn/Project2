#include "basics.h"

/* void tempReading (struct Values& value) {
    value->temp = rand() % 30 + 5;
}

void collectData (queue<Values>& data, struct Values value) {
    data.push(value);
}*/

using namespace std;

void tempReading () {
    lock_guard<mutex> lock(protectData);
    
};

void airMoistReading () {
    lock_guard<mutex> lock(protectData);

};

void windSpeedReading () {
    lock_guard<mutex> lock(protectData);

    this_thread::sleep_for(milliseconds(500));
};

/* void popValues (queue<Values>& data) {}; 
void addValues (queue<Values>& data) {}; 
void collectData (queue<Values>& data) {}; 
void displayData (queue<Values>& data) {};  */

int main () {
    srand(time(NULL));
    thread temp(tempReading());
    thread airMoist(airMoistReading());
    thread windSpeed(windSpeedReading());
    thread data ();

    this_thread::sleep_for(seconds(20));
    
/*     tempThread.join();
    displayThread.join(); */           
    return 0;
}