#include "basics.h"


void tempReading (Values& dataSet) 
{
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
void displayData(vector<Values>& data, Statistics &stats) 
{
    auto start_time = chrono::steady_clock::now();
    auto last_display_time = start_time;
    auto last_stats_time = start_time;

    while (running) 
    {
        this_thread::sleep_for(chrono::milliseconds(200));
        auto now = chrono::steady_clock::now();
        auto elapsed_display_seconds = chrono::duration_cast<chrono::seconds>(now - last_display_time).count();
        auto elapsed_stats_seconds = chrono::duration_cast<chrono::seconds>(now - last_stats_time).count();

        // Display the latest data every 2 seconds
        if (elapsed_display_seconds >= 2) 
        {
            lock_guard<mutex> lock(protectData);
            cout << "Senaste data (uppdateras var 2:a sekund): " << endl;
            cout << "------ Data ------" << endl;
            cout << "Tempature: " << data.back().temp << " C, "
                 << "Humidity: " << data.back().airMoist << "%, "
                 << "Wind Speed: " << data.back().windSpeed << " m/s" << endl;
            cout << "------------------" << endl << endl;
            last_display_time = now;
        }

        // Display statistics every 10 seconds
        if (elapsed_stats_seconds >= 10) 
        {
            lock_guard<mutex> lock(protectData);
            cout << "Statistik (visas var 10:e sekund): " << endl;
            cout << "------ Statistik ------" << endl;
            cout << "Max temp: "  << stats.maxTemp << endl;
            cout << "Min temp: " << stats.minTemp << endl;
            cout << "Average temp: " << stats.avgTemp << endl << endl;
            cout << "Max Humidity: " << stats.maxAirMoist << endl;
            cout << "Min Humidity: " << stats.minAirMoist << endl;
            cout << "Average Humidity: " << stats.avgAirMoist << endl << endl;
            cout << "Max Wind speed: " << stats.maxWindSpeed << endl;
            cout << "Min Wind speed: " << stats.minWindSpeed << endl;
            cout << "Average Wind speed: " << stats.avgWindSpeed << endl;
            cout << "------------------" << endl << endl;
            last_stats_time = now;
        }
    }
}

void calculateData (vector<Values> &data, Statistics &stats) 
{
    while (running) 
    {
        this_thread::sleep_for(seconds(5));
        lock_guard<mutex> lock(protectData);
        if (data.empty()) 
        {
            continue;
        }

        stats.minTemp = numeric_limits<double>::max();
        stats.maxTemp = numeric_limits<double>::lowest();
        double sumTemp = 0.0;

        stats.minAirMoist = numeric_limits<double>::max();
        stats.maxAirMoist = numeric_limits<double>::lowest();
        double sumAirMoist = 0.0;

        stats.minWindSpeed = numeric_limits<double>::max();
        stats.maxWindSpeed = numeric_limits<double>::lowest();
        double sumWindSpeed = 0.0;

        for (const auto& value : data) {
            // Calculate min and max for temp
            if (value.temp < stats.minTemp) stats.minTemp = value.temp;
            if (value.temp > stats.maxTemp) stats.maxTemp = value.temp;
            sumTemp += value.temp;

            // Calculate min and max for airMoist
            if (value.airMoist < stats.minAirMoist) stats.minAirMoist = value.airMoist;
            if (value.airMoist > stats.maxAirMoist) stats.maxAirMoist = value.airMoist;
            sumAirMoist += value.airMoist;

            // Calculate min and max for windSpeed
            if (value.windSpeed < stats.minWindSpeed) stats.minWindSpeed = value.windSpeed;
            if (value.windSpeed > stats.maxWindSpeed) stats.maxWindSpeed = value.windSpeed;
            sumWindSpeed += value.windSpeed;
        }

        stats.avgTemp = sumTemp / data.size();
        stats.avgAirMoist = sumAirMoist / data.size();
        stats.avgWindSpeed = sumWindSpeed / data.size();
    }
}


int main () 
{
    srand(time(NULL));

    struct Values dataSet;
    struct Statistics stats;
    vector<Values> data(10);

    thread temp(tempReading, ref(dataSet));
    thread airMoist(airMoistReading, ref(dataSet));
    thread windSpeed(windSpeedReading, ref(dataSet));
    thread collecting(collectData, ref(data), ref(dataSet));
    thread calculate(calculateData, ref(data), ref(stats));
    thread dataDisplay(displayData, ref(data), ref(stats));

    this_thread::sleep_for(seconds(30));
    running = false;
    
    //displayData(data);
    

    //Här bör vi sätta in en if else sats som kollar om de kan joinas, och om inte 
    //så skriver den ut något om att den inte kan hitta detta.
    temp.join();
    airMoist.join();
    windSpeed.join();
    collecting.join();
    calculate.join();
    dataDisplay.join();
            
    return 0;
};