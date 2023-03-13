#include <iostream>
#include "Station.h"
#include "Graph.h"
#include "VertexEdge.h"
#include <fstream>
#include <sstream>

using namespace std;
///Function that builds the vertexs
///Complexity: O(N)
void create_stations(Graph &g){
    ifstream file("../files/stations.csv");
    string string1;
    string name, district ,municipality ,township ,line;
    bool first_it = true;
    int count = 0;
    while(getline(file,string1)){
        if(first_it){
            first_it = false;
            continue;
        }
        istringstream ss(string1);
        getline(ss,name,',');
        getline(ss,district,',');
        getline(ss,municipality,',');
        getline(ss,township,',');
        getline(ss,line,',');
        Station station(name,district,municipality,township,line);
        if(g.findVertex(station) != nullptr){
            continue;
        }
        g.addVertex(count,station);
        count++;
    }
}

void create_networks(Graph &g){

    ifstream file("../files/network.csv");

    string line, station_A,station_B,capacity,service;
    bool first_it = true;

    while(getline(file, line)){
        stringstream ss(line);

        getline(ss, station_A, ',');
        getline(ss, station_B, ',');
        getline(ss, capacity, ',');
        getline(ss, service, ',');

        if(first_it == true){
            first_it = false;
            continue;
        }

        Vertex *v1 = g.findVertex(Station(station_A));
        Vertex *v2 = g.findVertex(Station(station_B));
        bool edge_exists = false;
        for(auto e : v1->getAdj()){
            if(e->getDest()->getName() == v2->getName()){
                edge_exists = true;
                break;
            }
        }

        if(edge_exists){
            continue;
        }

        g.addBidirectionalEdge(v1->getId(), v2->getId(), stoi(capacity), service);
    }

}
int main() {
    Graph graph;
    create_stations(graph);
    create_networks(graph);



    return 0;
}
