#include "basics.h"





void tempReading (Values& dataSet) 
{
    while (running) 
    {
        lock_guard<mutex> lock(protectData);
        dataSet.temp = rand() % 30 + 5; //Vi vill ha en random value: rand() % 30 + 5
        //lägg till i vektorn
        //Vi vill att den ska läggas in i en databas/i en struktur.
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) {
            dataReady = true;
        }
        this_thread::sleep_for(milliseconds(500));
    }
};

void airMoistReading (Values& dataSet) 
{
    while (running) 
    {
        lock_guard<mutex> lock(protectData);
        dataSet.airMoist = rand() % 100; //Vi vill ha en random value: rand() % 30 + 5
        //lägg till i vektorn
        //Vi vill att den ska läggas in i en databas/i en struktur.
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) {
            dataReady = true;
        }
        this_thread::sleep_for(milliseconds(500));
    }
};

void windSpeedReading (Values& dataSet) 
{
    while (running) 
    {
        lock_guard<mutex> lock(protectData);
        dataSet.windSpeed = rand() % 20 + 1; //Vi vill ha en random value: rand() % 30 + 5
        //lägg till i vektorn
        //Vi vill att den ska läggas in i en databas/i en struktur.
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) 
        {
            dataReady = true;
        }
        this_thread::sleep_for(milliseconds(500));
    }
};

void resetTempValues(Values &dataSet) 
{
    dataSet.temp = 0.0;
    dataSet.airMoist = 0.0;
    dataSet.windSpeed = 0.0;
};

/* void displayData (vector<Values>& data) {
    for (const auto &value : data) {
        cout << setprecision(3) << "Temp: " << value.temp << " C" << "\n";
    }
    cout << "\n";
    for (const auto &value : data) {
        cout << setprecision(3) << "Humidity: " << value.airMoist << "%" << "\n";
    }
    cout << "\n";
    for (const auto &value : data) {
        cout << setprecision(3) << "Wind Speed: " << value.windSpeed << " m/s" << "\n";
    }
}; */

void collectData (vector<Values>& data, Values& dataSet) 
{
    while (running) 
    {
        if (dataReady) 
        {
            lock_guard<mutex> lock (protectData);
            if (data.size() >= 10) {
                data.erase(data.begin()); // = data.erase(data[0]);
            }
            data.push_back(dataSet);
            resetTempValues(dataSet);
            dataReady = false;
        }
    }
};

//exempel på utskrift
void displayData(vector<Values>& data) 
{
    while (running) 
    {
        this_thread::sleep_for(seconds(10));
        lock_guard<mutex> lock(protectData);

        cout << "------ Data ------" << endl;
        for (const auto& value : data) 
        {
            cout << fixed << setprecision(2)
                 << "Temp: " << value.temp << " C, "
                 << "Humidity: " << value.airMoist << "%, "
                 << "Wind Speed: " << value.windSpeed << " m/s" << endl;
        }
        cout << "------------------" << endl;
        this_thread::sleep_for(seconds(10));
    }
};


void popValues (vector<Values>& data) {
    
};

void addValues (vector<Values>& data) {

};


int main () {
    struct Values dataSet;
    
    vector<Values> data(10);
    srand(time(NULL));

    thread temp(tempReading, ref(dataSet));
    thread airMoist(airMoistReading, ref(dataSet));
    thread windSpeed(windSpeedReading, ref(dataSet));
    thread collecting(collectData, ref(data), ref(dataSet));

    this_thread::sleep_for(seconds(10));
    running = false;
    
    displayData(data);
    collecting.join();
    temp.join();
    airMoist.join();
    windSpeed.join();
            
    return 0;
};