#ifndef MENU_H_
#define MENU_H_

#include "EVCharging.h"

const double maxPrice = 1.00;

void displayMenu() {
    cout << "Menu:" << endl;
    cout << "1. Print location information" << endl;
    cout << "2. Print adjacency matrix" << endl;
    cout << "3. Change station availability" << endl;
    cout << "4. Generate minimum spanning tree" << endl;
    cout << "5. List available charging stations by price" << endl;
    cout << "6. Find the nearest available charging station" << endl;
    cout << "7. Find the cheapest charging station (considering travel costs)" << endl;
    cout << "8. Find path with minimal total cost" << endl;
    cout << "9. Find the best charging path (multiple charging)" << endl; 
    cout << "Enter your choice: ";
}

void handleMenuChoice(EVCharging& charging, int choice) {
    switch (choice) {
        case 1:
            charging.printLocations();
            break;
        case 2:
            charging.printAdjacencyMatrix();
            break;
        case 3: {
            int stationIndex;
            bool newAvailability;
            double newPrice = 0.0;
            char availResponse;

            cout << "Enter the index of the station you want to update: ";
            cin >> stationIndex;

            cout << "Change availability (y for available, n for unavailable): ";
            cin >> availResponse;
            newAvailability = (availResponse == 'y' || availResponse == 'Y');

            if (newAvailability) {
                cout << "Enter the new price for the station (max $1.00/kWh): ";
                cin >> newPrice;
                if (newPrice > maxPrice) {
                    newPrice = maxPrice;
                }
            }

            charging.updateStation(stationIndex, newAvailability, newPrice);
            cout << "Updated station information:\n";
            charging.printLocations();
            break;
        }
        case 4:
            charging.testMinimumSpanning();
            break;
        case 5:
            charging.listAvailableStationsByPrice();
            break;
        case 6:
            charging.findNearestAvailableStation();
            break;
        case 7:
            charging.findCheapestChargingStation();
            break;
        case 8:
            charging.findPathWithMinimalCost();
            break;
        case 9:
            charging.findBestChargingPath();
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
}

#endif /* MENU_H_ */