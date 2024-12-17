#include "basics.h"

void tempReading (Values& dataSet) 
{
    cout << "Temp reading started:\n";
    while (running) 
    {   
        {
        lock_guard<mutex> lock(protectData);
        dataSet.temp = rand() % 30 + 5; // Random number for temp
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope.
        { 
            dataReady = true;
        }
        }
        this_thread::sleep_for(milliseconds(500)); //tempReading sleeps for 0.5 seconds.
    }
};

void airMoistReading (Values& dataSet) 
{
    cout << "Moist reading started.\n";
    while (running) 
    {   
        {
        lock_guard<mutex> lock(protectData);
        dataSet.airMoist = rand() % 100; // Random number airMoist
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope. 
        { 
            dataReady = true;
        }
        }
        this_thread::sleep_for(milliseconds(500)); //airMoistReading sleeps for 0.5 seconds.
    }
};

void windSpeedReading (Values& dataSet) 
{
    cout << "Wind reading started.\n";
    while (running) 
    {   
        {
        lock_guard<mutex> lock(protectData);
        dataSet.windSpeed = rand() % 20 + 1; // Random number for windSpeed
        if (dataSet.temp != 0.0 && dataSet.airMoist != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope.
        {
            dataReady = true;
        }
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
    cout << "Data collection started\n";
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
    cout << "displaydata started\n";

    while (running) {
        
        //this_thread::sleep_for(seconds(2));
        // Visa senaste data varannan sekund
        if (data.empty() == false) {
            lock_guard<mutex> lock (protectData);
            cout << "Senaste data (uppdateras var 2:a sekund): " << endl;
            
            cout << "------ Data ------" << endl;

            cout << fixed << setprecision(2)
                 << "Temp: " << data.back().temp << " C, "
                 << "Humidity: " << data.back().airMoist << "%, "
                 << "Wind Speed: " << data.back().windSpeed << " m/s" << endl
                 << "------------------" << endl;

            
        }
    this_thread::sleep_for(seconds(2));
    }
};

// Visa statistik var 10:e sekund
void displayStatistics(vector<Values>& data) {
    cout << "Displaystatistics started\n";
    //this_thread::sleep_for(seconds(10));
    while (running) {
    if (data.empty() == false) {
            

            struct Values average = {0.0, 0.0, 0.0};
            struct Values min = {1000.0, 1000.0, 1000.0};
            struct Values max = {0.0, 0.0, 0.0};
            
            lock_guard<mutex> lock (protectData);
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
        this_thread::sleep_for(seconds(10));
    }
}



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
    thread statisticsDisplay(displayStatistics, ref(data));

    this_thread::sleep_for(seconds(60));
    running = false;
    

    //Här bör vi sätta in en if else sats som kollar om de kan joinas, och om inte 
    //så skriver den ut något om att den inte kan hitta detta.
    temp.join();
    airMoist.join();
    windSpeed.join();
    collecting.join();
    dataDisplay.join();
    statisticsDisplay.join();
            
    return 0;
};