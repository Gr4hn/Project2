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


//exempel på utskrift
void displayData(vector<Values>& data) 
{
    auto start_time = chrono::steady_clock::now();

    while (running) {
        auto now = chrono::steady_clock::now();
        auto timepassed = duration_cast<seconds>(now - start_time).count();

        // Visa senaste data varannan sekund
        if (timepassed % 2 == 0 && data.empty() == false) {
            lock_guard<mutex> lock (protectData);
            cout << "Senaste data (uppdateras var 2:a sekund): " << endl;
            this_thread::sleep_for(milliseconds(150)); // För att undvika dubbla utskrifter samma sekund
            cout << "------ Data ------" << endl;

            cout << fixed << setprecision(2)
                 << "Temp: " << data.back().temp << " C, "
                 << "Humidity: " << data.back().airMoist << "%, "
                 << "Wind Speed: " << data.back().windSpeed << " m/s" << endl
                 << "------------------" << endl;
        }

        // Visa statistik var 10:e sekund
        if (timepassed % 10 == 0 && data.empty() == false) {
            lock_guard<mutex> lock (protectData);

            struct Values average;
            struct Values min;
            struct Values max;

            for (auto& MAM : data) {
                //samla in alla instanser för de olika värdena
                average.temp += MAM.temp;
                average.airMoist += MAM.airMoist;
                average.windSpeed += MAM.windSpeed;

                //Får fram högsta värderna i data.
                if (MAM.temp > max.temp) {
                    max.temp = MAM.temp;
                }
                if (MAM.airMoist > max.airMoist) {
                    max.airMoist = MAM.airMoist;
                }
                if (MAM.windSpeed > max.windSpeed) {
                    max.windSpeed = MAM.windSpeed;
                }

                //Får fram minsta värderna i data.
                if (MAM.temp < min.temp) {
                    min.temp = MAM.temp;
                }
                if (MAM.airMoist < min.airMoist) {
                    min.airMoist = MAM.airMoist;
                }
                if (MAM.windSpeed < min.windSpeed) {
                    min.windSpeed = MAM.windSpeed;
                }
            }
            
            //Få från genomsnittliga värderna
            average.temp = average.temp / data.size();
            average.airMoist = average.airMoist / data.size();
            average.windSpeed = average.windSpeed / data.size();

            cout << "Statistik (visas var 10:e sekund): " << endl;
            this_thread::sleep_for(milliseconds(150)); // För att undvika flera utskrifter samma sekund
            cout << "------ Statistik ------" << endl;
            cout << fixed << setprecision(2)

                 << "Average Temp: " << average.temp << " C, "
                 << "Average Humidity: " << average.airMoist << "%, "
                 << "Average Wind Speed: " << average.windSpeed << " m/s" << endl
                 << "------------------" << endl

                 << "Highest Temp: " << max.temp << " C, "
                 << "Highest Humidity: " << max.airMoist << "%, "
                 << "Highest Wind Speed: " << max.windSpeed << " m/s" << endl
                 << "------------------" << endl

                 << "Lowest Temp: " << min.temp << " C, "
                 << "Lowest Humidity: " << min.airMoist << "%, "
                 << "Lowest Wind Speed: " << min.windSpeed << " m/s" << endl
                 << "------------------" << endl;
        }
        // beräkna genomsnitt
        // Pausa tråden lite så att CPU inte överbelastas
        this_thread::sleep_for(milliseconds(200));
    }
};





int main () 
{
    srand(time(NULL));

    struct Values dataSet;
    vector<Values> data;

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