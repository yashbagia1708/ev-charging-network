#ifndef EVCHARGING_H_
#define EVCHARGING_H_

#include "Station.h"
#include "WeightedGraph.h"
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

using namespace std;

class EVCharging {
private:
    map<int, Station> locations;
    int numberOfLocations;
    WeightedGraph* weightedGraph;
public:
    EVCharging();
    ~EVCharging();
    void inputLocations();
    void printLocations();
    void printAdjacencyMatrix();
    void testMinimumSpanning();
    void updateStation(int index, bool availability, double price);
    void listAvailableStationsByPrice();
    void findNearestAvailableStation();
    void findCheapestChargingStation(); 
	void findPathWithMinimalCost();
    void findBestChargingPath();
};

EVCharging::EVCharging() {
    inputLocations();
    weightedGraph = new WeightedGraph(numberOfLocations);
}

EVCharging::~EVCharging() {
    delete weightedGraph;
}

void EVCharging::inputLocations() {
    ifstream infile("Locations.txt");

    if (!infile) {
        cout << "Cannot open input file." << endl;
        return;
    }

    int locationIndex = 0;

    while (!infile.eof()) {
        Station s;
        string charger;
        string price;
        while (!infile.eof()) {
            getline(infile, s.stationLocation, ',');
            getline(infile, charger, ',');
            getline(infile, price);
            s.available = (stoi(charger) == 1) ? true : false;
            s.chargingPrice = stod(price);
            s.index = locationIndex;
            locations[locationIndex] = s;
            locationIndex++;
        }
    }

    numberOfLocations = locationIndex;
}

void EVCharging::printLocations() {
    cout << "List of locations and charging information " << endl;
    cout << setw(8) << "Index" << setw(20) << "Station Location" << setw(20) << "Availability" << setw(20) << "Charging price" << endl;

    for (const auto& it : locations) {
        it.second.printLocation();
    }

    cout << endl;
}

void EVCharging::printAdjacencyMatrix() {
    cout << "Adjacency matrix (0 means no direct connection, non-zero value represents the distance of adjacent locations)\n" << endl;
    cout << setw(13) << " ";
    for (int i = 0; i < numberOfLocations; i++) {
        cout << setw(13) << locations[i].stationLocation;
    }
    cout << endl;
    for (int i = 0; i < numberOfLocations; i++) {
        cout << setw(13) << locations[i].stationLocation;
        for (int j = 0; j < numberOfLocations; j++) {
            cout << setw(13) << (weightedGraph->getWeight(i, j) == DBL_MAX ? 0.0 : weightedGraph->getWeight(i, j));
        }
        cout << endl;
    }
}

void EVCharging::testMinimumSpanning() {
    auto mstEdges = weightedGraph->minimumSpanning(5); 
    
    double totalLength = 0.0;
    cout << "Shortest power line from Parramatta to all other stations." << endl;
    cout << setw(20) << "From" << setw(20) << "To" << setw(15) << "Length" << endl;

    for (int i = 0; i < numberOfLocations; ++i) {
        if (i != 5) {
            cout << setw(20) << locations[mstEdges[i]].stationLocation;
            cout << setw(20) << locations[i].stationLocation;
            cout << setw(15) << weightedGraph->getWeight(mstEdges[i], i) << endl;
            totalLength += weightedGraph->getWeight(mstEdges[i], i);
        }
    }
    
    cout << "\nTotal length of the power line: " << totalLength << endl;
}

void EVCharging::updateStation(int index, bool availability, double price) {
    if (locations.find(index) != locations.end()) {
        locations[index].setAvailability(availability);
        if (availability) {
            locations[index].setChargingPrice(price);
        } else {
            locations[index].setChargingPrice(0.0);
        }
    } else {
        cout << "Station with index " << index << " not found." << endl;
    }
}

bool compareByPrice(const Station& a, const Station& b) {
    return a.chargingPrice < b.chargingPrice;
}

void EVCharging::listAvailableStationsByPrice() {
    vector<Station> availableStations;
    
    for (const auto& pair : locations) {
        if (pair.second.available) {
            availableStations.push_back(pair.second);
        }
    }

    sort(availableStations.begin(), availableStations.end(), compareByPrice);

    cout << "List all available charging stations in ascending order of charging price:" << endl;
    cout << setw(5) << "Index" << setw(20) << "Station Location" << setw(15) << "Availability" << setw(20) << "Charging price" << endl;
    int index = 0;
    for (const auto& station : availableStations) {
        cout << setw(5) << index++
                  << setw(20) << station.stationLocation
                  << setw(15) << (station.available ? "yes" : "no");
        if (station.chargingPrice == 0.0) {
            cout << setw(20) << "free of charge" << endl;
        } else {
            cout << setw(20) << "$" << setprecision(2) << fixed << station.chargingPrice << "/kWh" << endl;
        }
    }
}

void EVCharging::findNearestAvailableStation() {
    int startIndex;
    cout << "Enter the index of your starting location: ";
    cin >> startIndex;

    weightedGraph->dijkstra(startIndex);

    double minDistance = DBL_MAX;
    int nearestStationIndex = -1;

    for (const auto& location : locations) {
        if (location.second.available && location.first != startIndex) {
            double distance = weightedGraph->getDistance(location.first);
            if (distance < minDistance) {
                minDistance = distance;
                nearestStationIndex = location.first;
            }
        }
    }

    if (nearestStationIndex != -1) {
        cout << "The nearest available station is: " << locations[nearestStationIndex].stationLocation << endl;
        cout << "The distance to this station is: " << minDistance << " units." << endl;
    } else {
        cout << "No available stations found." << endl;
    }
}

void EVCharging::findCheapestChargingStation() {
    string startLocation;
    double chargeAmount;

    cout << "Enter the name of your starting location: ";
    cin.ignore();
    getline(cin, startLocation);

    int startIndex = -1;
    for (const auto& location : locations) {
        if (location.second.stationLocation == startLocation) {
            startIndex = location.first;
            break;
        }
    }

    if (startIndex == -1) {
        cout << "Starting location not found." << endl;
        return;
    }

    cout << "Charging amount (kWh): ";
    cin >> chargeAmount;

    weightedGraph->dijkstra(startIndex);

    double minTotalCost = DBL_MAX;
    int cheapestStationIndex = -1;

    for (const auto& location : locations) {
        if (location.second.available && location.first != startIndex) {
            double travelDistance = weightedGraph->getDistance(location.first);
            double travelCost = 2 * travelDistance * 0.08; 
            double chargingCost = chargeAmount * location.second.chargingPrice;
            double totalCost = travelCost + chargingCost;

            if (totalCost < minTotalCost) {
                minTotalCost = totalCost;
                cheapestStationIndex = location.first;
            }
        }
    }

    if (cheapestStationIndex != -1) {
        cout << "The cheapest other charging station is: " << locations[cheapestStationIndex].stationLocation << endl;
        cout << "Charging cost = $" << fixed << setprecision(2) << (chargeAmount * locations[cheapestStationIndex].chargingPrice) << endl;
        cout << "Travel cost = $" << fixed << setprecision(2) << (2 * weightedGraph->getDistance(cheapestStationIndex) * 0.08) << endl;
        cout << "Total cost = $" << fixed << setprecision(2) << minTotalCost << endl;
    } else {
        cout << "No available stations found." << endl;
    }
}


void EVCharging::findPathWithMinimalCost() {
    string originName, destinationName;
    double powerToCharge;

    cout << "Enter the origin station location: ";
    cin.ignore(); 
    getline(cin, originName);

    cout << "Enter the destination station location: ";
    getline(cin, destinationName);

    cout << "Enter the amount of power to charge (kWh): ";
    cin >> powerToCharge;

    int origin = -1, destination = -1;
    for (const auto& location : locations) {
        if (location.second.stationLocation == originName) {
            origin = location.first;
        }
        if (location.second.stationLocation == destinationName) {
            destination = location.first;
        }
    }

    if (origin == -1 || destination == -1) {
        cout << "One or both station locations not found." << endl;
        return;
    }

    weightedGraph->dijkstra(origin);
    vector<double> distancesFromOrigin(numberOfLocations);
    for (int i = 0; i < numberOfLocations; ++i) {
        distancesFromOrigin[i] = weightedGraph->getDistance(i);
    }

    double minCost = numeric_limits<double>::max();
    int minCostStation = -1;

    for (const auto& it : locations) {
        if (it.second.available) {
            weightedGraph->dijkstra(it.first);
            double distanceToDestination = weightedGraph->getDistance(destination);

            if (distancesFromOrigin[it.first] != numeric_limits<double>::max() &&
                distanceToDestination != numeric_limits<double>::max()) {
                double travelCost = (distancesFromOrigin[it.first] + distanceToDestination) * 0.08; 
                double chargingCost = it.second.chargingPrice * powerToCharge;
                double totalCost = travelCost + chargingCost;

                if (totalCost < minCost) {
                    minCost = totalCost;
                    minCostStation = it.first;
                }
            }
        }
    }

    if (minCostStation == -1) {
        cout << "No available paths found with the specified power to charge." << endl;
    } else {
        double travelCost = (distancesFromOrigin[minCostStation] + weightedGraph->getDistance(destination)) * 0.08;
        double chargingCost = powerToCharge * locations[minCostStation].chargingPrice;
        double totalCost = travelCost + chargingCost;

        cout << "The cheapest charging station is " << locations[minCostStation].stationLocation << "." << endl;
        cout << "Charging cost = $" << fixed << setprecision(2) << chargingCost << endl;
        cout << "Travel cost = $" << fixed << setprecision(3) << travelCost << endl;
        cout << "Total cost = $" << fixed << setprecision(3) << totalCost << endl;
        cout << "Travel path: " << originName << ", " << locations[minCostStation].stationLocation << endl;
    }
}

void EVCharging::findBestChargingPath() {
    string originName, destinationName;
    double powerToCharge;

    cout << "Enter the origin station location: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, originName);

    cout << "Enter the destination station location: ";
    getline(cin, destinationName);

    cout << "Enter the amount of power to charge (kWh): ";
    cin >> powerToCharge;

    // Find the indices for the origin and destination
    int origin = -1, destination = -1;
    for (const auto& location : locations) {
        if (location.second.stationLocation == originName) {
            origin = location.first;
        }
        if (location.second.stationLocation == destinationName) {
            destination = location.first;
        }
    }

    if (origin == -1 || destination == -1) {
        cout << "One or both station locations not found." << endl;
        return;
    }

    weightedGraph->dijkstra(origin);
    vector<double> distancesFromOrigin(numberOfLocations);
    vector<int> previousNode(numberOfLocations, -1);
    for (int i = 0; i < numberOfLocations; ++i) {
        distancesFromOrigin[i] = weightedGraph->getDistance(i);
    }

    if (powerToCharge > 30.0) {
        double minCost = numeric_limits<double>::max();
        int minCostStation = -1;

        for (const auto& it : locations) {
            if (it.second.available && it.first != origin) { 
                weightedGraph->dijkstra(it.first);
                double distanceToDestination = weightedGraph->getDistance(destination);

                if (distancesFromOrigin[it.first] != numeric_limits<double>::max() &&
                    distanceToDestination != numeric_limits<double>::max()) {
                    double travelCost = (distancesFromOrigin[it.first] + distanceToDestination) * 0.08; 
                    double chargingCost = 0.0; 
                    double totalCost = travelCost + chargingCost;

                    if (totalCost < minCost) {
                        minCost = totalCost;
                        minCostStation = it.first;
                    }
                }
            }
        }

        if (minCostStation != -1) {
            double travelCost = (distancesFromOrigin[minCostStation] + weightedGraph->getDistance(destination)) * 0.08;
            double totalCost = travelCost;
            cout << "The best way of charging is to charge at " << locations[minCostStation].stationLocation << "." << endl;
            cout << "Charging cost = Free" << endl;
            cout << "Travel cost = $" << fixed << setprecision(3) << travelCost << endl;
            cout << "Total cost = $" << fixed << setprecision(3) << totalCost << endl;
            cout << "Travel path: " << originName << ", " << locations[minCostStation].stationLocation << ", " << destinationName << endl;
            return;
        }
    }

    int currentLocation = destination;
    double minDistance = numeric_limits<double>::max();
    int nearestStationIndex = -1;
    for (const auto& location : locations) {
        if (location.second.available) {
            double distanceToStation = weightedGraph->getDistance(location.first);
            if (distanceToStation < minDistance) {
                minDistance = distanceToStation;
                nearestStationIndex = location.first;
            }
        }
    }

    double travelCost = distancesFromOrigin[destination];
    double chargingCost = powerToCharge * locations[nearestStationIndex].chargingPrice;
    double totalCost = travelCost + chargingCost;

    cout << "The best way of charging is to charge at " << locations[nearestStationIndex].stationLocation << "." << endl;
    cout << "Charging cost = $" << fixed << setprecision(2) << chargingCost << endl;
    cout << "Travel cost = $" << fixed << setprecision(3) << travelCost << endl;
    cout << "Total cost = $" << fixed << setprecision(3) << totalCost << endl;
    cout << "Travel path: " << originName << ", " << locations[nearestStationIndex].stationLocation << ", " << destinationName << endl;
}

#endif /* EVCHARGING_H_ */