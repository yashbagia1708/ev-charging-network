#ifndef STATION_H_
#define STATION_H_

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class Station {
public:
    int index;
    string stationLocation;
    bool available;
    double chargingPrice;

    Station() : index(-1), available(false), chargingPrice(0.0) {}

    void setAvailability(bool avail) { available = avail; }
    void setChargingPrice(double price) { chargingPrice = price; }
    
    void printLocation() const {
        cout << setw(8) << index 
                  << setw(20) << stationLocation 
                  << setw(20) << (available ? "yes" : "no");
        if (chargingPrice == 0.0) {
            cout << setw(20) << "free of charge" << endl;
        } else {
            cout << setw(20) << "$" << setprecision(2) << fixed << chargingPrice << "/kWh" << endl;
        }
    }
};

#endif /* STATION_H_ */
