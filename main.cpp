#include "basics.h"

void tempReading (Values& dataSet) 
{
    while (running) 
    {
        lock_guard<mutex> lock(protectData);
        dataSet.temp = rand() % 30 + 5; // Random number for temp
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope.
        { 
            dataReady = true;
        }
        this_thread::sleep_for(milliseconds(500)); //tempReading sleeps for 0.5 seconds.
    }
};

void airMoistReading (Values& dataSet) 
{
    while (running) 
    {
        lock_guard<mutex> lock(protectData);
        dataSet.airMoist = rand() % 100; // Random number airMoist
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope. 
        { 
            dataReady = true;
        }
        this_thread::sleep_for(milliseconds(500)); //airMoistReading sleeps for 0.5 seconds.
    }
};

void windSpeedReading (Values& dataSet) 
{
    while (running) 
    {
        lock_guard<mutex> lock(protectData);
        dataSet.windSpeed = rand() % 20 + 1; // Random number for windSpeed
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope.
        {
            dataReady = true;
        }
        this_thread::sleep_for(milliseconds(500)); //windSpeedReading sleeps for 0.5 seconds.
    }
};

void resetTempValues(Values &dataSet) 
{
    dataSet.temp = 0.0;
    dataSet.airMoist = 0.0;
    dataSet.windSpeed = 0.0;
};


void collectData (vector<Values>& data, Values& dataSet) 
{
    while (running) 
    {
        if (dataReady) 
        {
            lock_guard<mutex> lock (protectData);
            if (data.size() >= 10) 
            {
                data.erase(data.begin()); // = data.erase(data[0]);
            }
            data.push_back(dataSet);
            resetTempValues(dataSet);
            dataReady = false;
        }
    }
};

//
//exempel på utskrift
void displayData(vector<Values>& data) 
{
    auto start_time = chrono::steady_clock::now();

    while (running) {
        auto now = chrono::steady_clock::now();
        auto timepassed = duration_cast<seconds>(now - start_time).count();

/*         // Beräkna hur lång tid som har gått
        this_thread::sleep_for(seconds(1));
        auto now = chrono::steady_clock::now();
        auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(now - start_time).count(); */

        // Visa senaste data varannan sekund
        if (/*elapsed_seconds*/ timepassed % 2 == 0) {
            lock_guard<mutex> lock (protectData);
            cout << "Senaste data (uppdateras var 2:a sekund): " << endl;
            this_thread::sleep_for(milliseconds(200)); // För att undvika dubbla utskrifter samma sekund
            cout << "------ Data ------" << endl;
            for (const auto& value : data) 
            {
                //data.end().temp
                cout << fixed << setprecision(2) << &data.front();

                   /*  << "Temp: " << value.temp << " C, "
                    << "Humidity: " << value.airMoist << "%, "
                    << "Wind Speed: " << value.windSpeed << " m/s" << endl; */
            }
            cout << "------------------" << endl;
        }

        // Visa statistik var 10:e sekund
        if (timepassed % 10 == 0) {
            lock_guard<mutex> lock (protectData);
            cout << "Statistik (visas var 10:e sekund): " << endl;
            this_thread::sleep_for(milliseconds(200)); // För att undvika flera utskrifter samma sekund
            cout << "------ Statistik ------" << endl;
            for (const auto& value : data) 
            {
                cout << fixed << setprecision(2)
                    << "Temp: " << value.temp << " C, "
                    << "Humidity: " << value.airMoist << "%, "
                    << "Wind Speed: " << value.windSpeed << " m/s" << endl;
            }
            cout << "------------------" << endl;
        }
        // beräkna genomsnitt
        // Pausa tråden lite så att CPU inte överbelastas
        this_thread::sleep_for(milliseconds(200));
    }

/*     while (running) 
    {
        //this_thread::sleep_for(seconds(10));
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
        this_thread::sleep_for(seconds(2));

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

    } */
};





int main () 
{
    srand(time(NULL));

    struct Values dataSet;
    vector<Values> data(10);

    thread temp(tempReading, ref(dataSet));
    thread airMoist(airMoistReading, ref(dataSet));
    thread windSpeed(windSpeedReading, ref(dataSet));
    thread collecting(collectData, ref(data), ref(dataSet));
    thread dataDisplay(displayData, ref(data));

    this_thread::sleep_for(seconds(30));
    running = false;
    
    //displayData(data);
    

    //Här bör vi sätta in en if else sats som kollar om de kan joinas, och om inte 
    //så skriver den ut något om att den inte kan hitta detta.
    temp.join();
    airMoist.join();
    windSpeed.join();
    collecting.join();
    dataDisplay.join();
            
    return 0;
};