#include "basics.h"

//Genererar data för temp och skickar tillbaka det till objektet.
void tempReading (Values& dataSet) 
{
    cout << "Temp reading started:\n"; //Debugging utskrift
    while (running) 
    {   
        {
        lock_guard<mutex> lock(protectData);
        dataSet.temp = rand() % 30 + 5; // Random number for temp
        if (dataSet.temp != 0.0 && dataSet.humidity != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope.
        { 
            dataReady = true; //Om alla element finns, signalera att det finns data att hämta.
        }
        }
        this_thread::sleep_for(milliseconds(500)); //tempReading sleeps for 0.5 seconds.
    }
};

//Genererar data för luftfuktighet och skickar tillbaka det till objektet.
void humidityReading (Values& dataSet) 
{
    cout << "Humidity reading started.\n"; //Debugging utskrift
    while (running) 
    {   
        {
        lock_guard<mutex> lock(protectData);
        dataSet.humidity = rand() % 100; // Random number humidity
        if (dataSet.temp != 0.0 && dataSet.humidity != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope. 
        { 
            dataReady = true; //Om alla element finns, signalera att det finns data att hämta.
        }
        }
        this_thread::sleep_for(milliseconds(500)); //humidityReading sleeps for 0.5 seconds.
    }
};

//Genererar data för vindhastighet och skickar tillbaka det till objektet.
void windSpeedReading (Values& dataSet) 
{
    cout << "Wind reading started.\n"; //Debugging utskrift
    while (running) 
    {   
        {
        lock_guard<mutex> lock(protectData);
        dataSet.windSpeed = rand() % 20 + 1; // Random number for windSpeed
        if (dataSet.temp != 0.0 && dataSet.humidity != 0.0 && dataSet.windSpeed != 0.0) // If all elements doesn't have a value skip this scope.
        {
            dataReady = true; //Om alla element finns, signalera att det finns data att hämta.
        }
        }
        this_thread::sleep_for(milliseconds(500)); //windSpeedReading sleeps for 0.5 seconds.
    }
};

//Nollställer värderna i objektet dataset
void resetTempValues(Values &dataSet) 
{
    dataSet.temp = 0.0;
    dataSet.humidity = 0.0;
    dataSet.windSpeed = 0.0;
};

void collectData(vector<Values>& data, Values& dataSet) {
    cout << "Data collection started\n"; // Debugging output
    while (running) {
        if (dataReady) {
            {
                lock_guard<mutex> lock(protectData);
                if (data.size() >= 10) {
                    data.erase(data.begin()); // Remove oldest entry
                }
                data.push_back(dataSet); // Add current dataSet
            }

            string fileTransfer = "Dataset: \n";
            fileTransfer += "Temp: " + to_string(dataSet.temp);
            fileTransfer += " Humidity: " + to_string(dataSet.humidity);
            fileTransfer += " Wind Speed: " + to_string(dataSet.windSpeed) + "\n\n";

            {
                lock_guard<mutex> fileLock(fileMutex); // Protect file writing
                ofstream outfile("./sensordata.txt", ios::app);
                if (!outfile.is_open()) {
                    cerr << "Error: Could not open sensordata.txt for writing.\n";
                    return;
                }
                outfile << fileTransfer;
                outfile.flush();
                outfile.close();
            }

            resetTempValues(dataSet); // Reset dataSet
            dataReady = false; // Reset dataReady flag
        }
    }
}

// Visa senaste data varannan sekund
void displayData(vector<Values>& data) 
{
    cout << "displaydata started\n"; //Debugging utskrift

    while (running) {

        //Ser till att funktionen inte körs om vectorn är tom.
        if (data.empty() == false) {
            lock_guard<mutex> lock (protectData);
            cout << "Senaste data (uppdateras var 2:a sekund): " << endl;
            
            cout << "------ Data ------" << endl;

            cout << fixed << setprecision(2)
                 << "Temp: " << data.back().temp << " C, "
                 << "Humidity: " << data.back().humidity << "%, "
                 << "Wind Speed: " << data.back().windSpeed << " m/s" << endl
                 << "------------------" << endl;

            
        }
    this_thread::sleep_for(seconds(2));
    }
};

// Visa statistik var 10:e sekund
void displayStatistics(vector<Values>& data) {
    
    cout << "Displaystatistics started\n";
    
    while (running) {
    
    //Ser till att funktionen inte körs om vectorn är tom.
    if (data.empty() == false) {
            
            //Object för att lagra värderna för respektive statistik
            struct Values average = {0.0, 0.0, 0.0};
            struct Values min = {1000.0, 1000.0, 1000.0};
            struct Values max = {0.0, 0.0, 0.0};
            
            lock_guard<mutex> lock (protectData);

            //MAM = Min Average Max
            for (auto& MAM : data) {

                //samlar in alla instanser för de olika värdena
                average.temp += MAM.temp;
                average.humidity += MAM.humidity;
                average.windSpeed += MAM.windSpeed;

                //Får fram högsta värderna i data.
                if (MAM.temp > max.temp) {
                    max.temp = MAM.temp;
                }
                if (MAM.humidity > max.humidity) {
                    max.humidity = MAM.humidity;
                }
                if (MAM.windSpeed > max.windSpeed) {
                    max.windSpeed = MAM.windSpeed;
                }

                //Får fram minsta värderna i data.
                if (MAM.temp < min.temp) {
                    min.temp = MAM.temp;
                }
                if (MAM.humidity < min.humidity) {
                    min.humidity = MAM.humidity;
                }
                if (MAM.windSpeed < min.windSpeed) {
                    min.windSpeed = MAM.windSpeed;
                }
            }
            
            //Få fram genomsnittliga värderna
            average.temp = average.temp / data.size();
            average.humidity = average.humidity / data.size();
            average.windSpeed = average.windSpeed / data.size();

            cout << "Statistik (visas var 10:e sekund): " << endl;
            
            //Skriver ut statistik genom att använda sig av objekten ovan.
            cout << "------ Statistik ------" << endl;
            cout << fixed << setprecision(2)

                 << "Average Temp: " << average.temp << " C, "
                 << "Average Humidity: " << average.humidity << "%, "
                 << "Average Wind Speed: " << average.windSpeed << " m/s" << endl
                 << "------------------" << endl

                 << "Highest Temp: " << max.temp << " C, "
                 << "Highest Humidity: " << max.humidity << "%, "
                 << "Highest Wind Speed: " << max.windSpeed << " m/s" << endl
                 << "------------------" << endl

                 << "Lowest Temp: " << min.temp << " C, "
                 << "Lowest Humidity: " << min.humidity << "%, "
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
    thread humidity(humidityReading, ref(dataSet));
    thread windSpeed(windSpeedReading, ref(dataSet));
    thread collecting(collectData, ref(data), ref(dataSet));
    thread dataDisplay(displayData, ref(data));
    thread statisticsDisplay(displayStatistics, ref(data));

    this_thread::sleep_for(seconds(60));
    running = false;
    

    //if satser för att avgöra om det går att utföra join för varje thread.
    //Placerar ut en join om joinable returnerar sant.
    //Skriver ut ett errormeddelande om det ej går att placera join.
    if (temp.joinable()) {
        temp.join();
    }
    else {
        cout << "Error, kan ej join thread.\n";
    }

    if (humidity.joinable()) {
        humidity.join();
    }
    else {
        cout << "Error, kan ej join thread.\n";
    } 

    if (windSpeed.joinable()) {
        windSpeed.join();
    }
    else {
        cout << "Error, kan ej join thread.\n";
    }

    if (collecting.joinable()) {
        collecting.join();
    }
    else {
        cout << "Error, kan ej join thread.\n";
    }

    if (dataDisplay.joinable()) {
        dataDisplay.join();
    }
    else {
        cout << "Error, kan ej join thread.\n";
    }

    if (statisticsDisplay.joinable()) {
        statisticsDisplay.join();
    }
    else {
        cout << "Error, kan ej join thread.\n";
    }
            
    return 0;
};