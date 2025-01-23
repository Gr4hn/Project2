# Instructions
När programmet körs kommer en seed att sättas för att kunna beräkna tid enligt den nuvarande användares klocka. Programmet kommer därefter att skapa 1 struct (avsedd att hålla "mjuk" data) samt en vector (avsedd att hålla i "hård" data). 
Programmet skapar sedan (i nuläget kan behöva ändras i framtiden) 5 trådar som har olika uppgifter. Tråd 1 genererar en slumpmässig temperatur, tråd 2 genererar en slumpmässig luftfuktighet, tråd 3 generar en slumpmässig vindhastighet. När tråd 1,2 och 3 har data redo kommer kommer ett meddelande skickas ut att det finns data redo att hämtas. Tråd 4 som ständigt kollar efter detta kommer då att gå in och ta bort det äldsta värdet (bland de hårda värdena) innan det lägger in det nya värdet (det mjuka värdet i vectorn för de hårda värdena). Den sista tråden kommer därefter att varannan sekund att printa ut det senaste värdet som har lagts in i vectorn medans statistik (Max, min och medelvärde) printas varje 10:e sekund. I dessa trådar används en atomic<bool> för att säkerställa att alla uppgifter ska köras tills dess att förhållandet ändras. Efter detta kommer man till main-trådens "sleep_for(seconds(x))" där användaren själv kan ange hur länge programmet ska köras. Efter att den har kört klart sätts vår atomic till "false" för att meddela samtliga threads som använder detta att avsluta sin uppgift. Efter detta kommer programmet att vänta in samtliga threads innan programmet avslutas.

OBS! Allt sker samtidigt, men inte pararellt.

# Software Requirements Specification (SRS)
## 1. Introduction
#### 1.1 Purpose

Denna SRS beskriver kraven för ett system som simulerar en väderövervakningsstation. Systemet ska samla in, bearbeta och visa data från tre sensorer: temperatur, luftfuktighet och vindhastighet. Målet är att leverera ett realtidssystem som simulerar väderförhållanden i Sverige med periodiska uppdateringar och beräkning av statistik.

#### 1.2 Scope

Systemet kommer att:

*  Simulera sensorer med slumpmässigt genererade värden.
*  Säkerställa trådsäker datainsamling och bearbetning med mutex.
*  Beräkna statistik från insamlad data.
*  Visa realtidsdata och statistik via konsolen med definierade tidsintervaller.

Applikationen är avsedd för användning i utbildning och prototyputveckling av väderstationer.

#### 1.3 Definitions, Acronyms, and Abbreviations

*  SRS: Software Requirements Specification.
*  Mutex: Mutual Exclusion, används för att säkerställa trådsäkerhet.
*  Realtidsdata: Data som genereras och visas löpande.

#### 1.4 References

*  C++ Standard Library Documentation. 
*  C++ Reference Library Documentation

## 2. Overall Description
#### 2.1 System Overview

Systemet består av flera trådar som:

*  Simulerar tre sensorer som genererar väderdata.
*  Samlar in data i ett delat minnesområde.
*  Bearbetar data för att generera statistik.
*  Visar realtidsdata och statistik i konsolen.

#### 2.2 System Constraints

*  Applikationen ska köras på en dator med stöd för C++ och trådar (t.ex. POSIX eller std::thread).
*  Systemet måste hantera flera trådar och skydda delat minne från samtidig åtkomst.

#### 2.3 Assumptions and Dependencies

*  Sensordata antas vara slumpmässiga men realistiska inom definierade intervall.
*  Statistiken beräknas endast på data insamlad under de senaste fem sekunderna.

## 3. Functional Requirements
#### 3.1 Sensor Simulation

*  FR-1: Systemet ska simulera en temperaturgivare som genererar slumpmässiga värden mellan -10°C och 30°C var 500:e millisekund.
*  FR-2: Systemet ska simulera en luftfuktighetsgivare som genererar slumpmässiga värden mellan 0% och 100% var 500:e millisekund.
*  FR-3: Systemet ska simulera en vindhastighetsgivare som genererar slumpmässiga värden mellan 0 m/s och 30 m/s var 500:e millisekund.

#### 3.2 Data Collection

*  FR-4: Systemet ska använda ett delat minnesområde för att lagra de senaste värdena från varje sensor.
*  FR-5: Systemet ska använda mutex för att skydda delat minne från samtidig åtkomst av flera trådar.
*  FR-6: Systemet ska implementera flaggor för att signalera när ny data är tillgänglig.

#### 3.3 Data Processing

*  FR-7: Systemet ska beräkna statistik (medelvärde, minimum och maximum) för varje sensor var femte sekund.
*  FR-8: Statistiken ska baseras på data insamlad under de senaste fem sekunderna.

#### 3.4 Data Display

*  FR-9: Systemet ska visa de senaste sensordata i konsolen varannan sekund.
*  FR-10: Systemet ska visa statistik (medelvärde, minimum och maximum) för varje sensor i konsolen var tionde sekund.

## 4. Non-functional Requirements
#### 4.1 Performance Requirements

*  Systemet ska hantera tre sensorer som genererar data kontinuerligt utan märkbar fördröjning.
*  Statistikberäkningen ska vara färdig inom 500 millisekunder.

#### 4.2 Usability Requirements

*  Konsolutskrifterna ska vara lättlästa och visa sensordata och statistik i separata sektioner.

#### 4.3 Reliability Requirements

*  Systemet ska hantera samtidig åtkomst av flera trådar utan datakorruption.
*  Systemet ska kunna köras kontinuerligt i minst 24 timmar utan avbrott.

#### 4.4 Portability Requirements

*  Applikationen ska kunna köras på alla moderna operativsystem med stöd för C++ och trådar.

## 5. System Design (High-level)
#### 5.1 Architecture

*  __Sensortrådar:__ Varje sensor körs i en separat tråd som genererar data periodiskt.
*  __Delat minne:__ Ett gemensamt datalager skyddat med mutex lagrar de senaste värdena.
*  __Bearbetningstråd:__ En separat tråd ansvarar för att beräkna statistik.
*  __Visningstråd:__ En tråd hanterar periodisk konsolutskrift av data och statistik.

#### 5.2 Timing
| Uppgift                     | Tidsintervall      |  
|-----------------------------|--------------------|  
| Sensoruppdatering           | 500 millisekunder  |  
| Realtidsdata i konsolen     | 2 sekunder         |  
| Statistikberäkning          | 5 sekunder         |  
| Statistik i konsolen        | 10 sekunder        |

## 6. Appendices
#### 6.1 Example Output

__Realtidsdata (varannan sekund):__

__Tid:__ 12:00:02  
__Temperatur:__ 12.5°C  
__Luftfuktighet:__ 45%  
__Vindhastighet:__ 7.3 m/s  

Statistik (var tionde sekund):

__Tid:__ 12:00:10  
__Temperatur__ (Medel: 10.2°C, Min: -5.0°C, Max: 15.6°C)  
__Luftfuktighet__ (Medel: 48%, Min: 30%, Max: 60%)  
__Vindhastighet__ (Medel: 6.5 m/s, Min: 2.0 m/s, Max: 12.0 m/s)  

## 7. Approval

Denna specifikation är godkänd av projektledaren och utvecklingsteamet som grund för implementationen.

## Links
Documentation index: [Index](https://Gr4hn.github.io/Project2/gh-pages/index.html)
