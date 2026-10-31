//WeightedGraph.h

#ifndef WEIGHTEDGRAPH_H_
#define WEIGHTEDGRAPH_H_

#include <iostream>
#include <list>
#include <cfloat>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <limits> 
#include <utility> 


using namespace std;

class WeightedGraph {
protected:
    int gSize;
    list<int>* graph;
    
    int* edges;
    double* edgeWeights;

public:
    double** weights;
    WeightedGraph(int size = 0);
    ~WeightedGraph();
    int getGraphSize() const;
    list<int> getAdjacencyList(int index);
    double getWeight(int i, int j);
    void printAdjacencyList();
    void printAdjacencyMatrix();
	vector<int> minimumSpanning(int source);
	double getDistance(int destination);
    void dijkstra(int source);
};

WeightedGraph::WeightedGraph(int size) {
    gSize = 0;
    edges = new int[size];
    edgeWeights = new double[size];
    ifstream infile;
    char fileName[50] = "Distance.txt";

    infile.open(fileName);

    if (!infile) {
        cout << "Cannot open input file." << endl;
        return;
    }

    gSize = size;

    graph = new list<int>[gSize];
    weights = new double*[gSize];

    for (int i = 0; i < gSize; i++)
        weights[i] = new double[gSize];

    for (int i = 0; i < gSize; i++) {
        for (int j = 0; j < gSize; j++) {
            double value;
            infile >> value;
            if (value == 0)
                weights[i][j] = DBL_MAX; 
            else {
                weights[i][j] = value;
                graph[i].push_back(j);
            }
        }
    }
    infile.close();
}

WeightedGraph::~WeightedGraph() {
    for (int i = 0; i < gSize; i++)
        delete[] weights[i];

    delete[] weights;
    delete[] edges;
    delete[] edgeWeights;

    for (int index = 0; index < gSize; index++)
        graph[index].clear();

    delete[] graph;
}

void WeightedGraph::printAdjacencyMatrix() { 
    cout << "\nAdjacency Matrix" << endl;
    for (int i = 0; i < gSize; i++) {
        for (int j = 0; j < gSize; j++) {
            cout << setw(8) << (weights[i][j] == DBL_MAX ? 0.0 : weights[i][j]); 
        }
        cout << endl;
    }
}

void WeightedGraph::printAdjacencyList() { 
    cout << "\nAdjacency List" << endl;
    for (int index = 0; index < gSize; index++) {
        cout << index << ": ";
        for (int e : graph[index])
            cout << e << " ";
        cout << endl;
    }

    cout << endl;
}

vector<int> WeightedGraph::minimumSpanning(int source) {
    vector<int> parent(gSize, -1);
    vector<double> key(gSize, DBL_MAX);
    vector<bool> mstSet(gSize, false);
    vector<int> mstEdges(gSize);

    priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;
    pq.push(make_pair(0.0, source));
    key[source] = 0.0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        mstSet[u] = true;

        for (int v = 0; v < gSize; ++v) {
            if (weights[u][v] != 0 && !mstSet[v] && weights[u][v] < key[v]) {
                parent[v] = u;
                key[v] = weights[u][v];
                pq.push(make_pair(key[v], v));
            }
        }
    }

    for (int i = 0; i < gSize; ++i) {
        if (i != source) {
            mstEdges[i] = parent[i];
        }
    }

    return mstEdges;
}

double WeightedGraph::getWeight(int i, int j) {
    return weights[i][j];
}

double WeightedGraph::getDistance(int destination) {
    return edgeWeights[destination];
}

void WeightedGraph::dijkstra(int source) {
    priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;
    vector<double> dist(gSize, DBL_MAX);

    pq.push(make_pair(0.0, source));
    dist[source] = 0.0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (int v : graph[u]) {
            if (dist[v] > dist[u] + weights[u][v]) {
                dist[v] = dist[u] + weights[u][v];
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    for (int i = 0; i < gSize; ++i) {
        edgeWeights[i] = dist[i];
    }
}

int WeightedGraph::getGraphSize() const {
    return gSize;
}

#endif /* WEIGHTEDGRAPH_H_ */