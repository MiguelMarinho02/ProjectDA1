#include <iostream>
#include "Station.h"
#include "Graph.h"
#include "VertexEdge.h"
#include <fstream>
#include <sstream>

using namespace std;

///Function that displays the menu
///Complexity: O(1)
void menuDisplay() {
    cout << "Menu \nEnter your option:" << endl;
    cout << "1) Read data files" << endl;
    cout << "Railway manager: \n";
    cout << "2) Find the maximum amount of trains between two stations \n";
    cout << "3) Find the pair of stations that require the most amount of trains when taking full advantage of the network \n";
    cout << "4) Budget information \n";
    cout << "5) Get maximum amount of trains arriving at a station simultaneously \n";
    cout << "6) Optimization analysis \n";
    cout << "Reliability and line failures: \n";
    cout << "7) In reduced connectivity, get max amount of trains travelling between two stations \n";
    cout << "8) In segment failure, get most affected stations \n";
    cout << "0) Exit\n";
    cout << "Option:";
}


///Function that builds the graph's nodes
///Complexity: O(N^2)
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
///Function that builds the edges and avoids repetitions
///Complexity: O(N^2)
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
            if(e->getDest()->getName() == v2->getName() && e->getService() == service){
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

///Function that computes maximum num of trains between 2 given stations
///Complexity: O(VE^2)
void max_num_trains_two_stations(Graph graph){
    string s1,s2;
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "\nPlease Input the name of the origin station:";
    getline(cin,s1);
    cout << "\nPlease Input the name of the destination station:";
    getline(cin,s2);
    int max_trains = graph.maxFlow(graph.findVertex(Station(s1))->getId(),graph.findVertex(Station(s2))->getId());
    if(max_trains == 0){
        cout << "Found no connection between this stations\n";
    }
    else{
        cout << "Max num of trains:" << max_trains << endl;
    }
}

void max_amount_trains_capacity(Graph graph){
    vector<pair<Vertex*,Vertex*>> pairs;
    vector<int> capacities;
    cout << "Loading:\nIt is not stuck, it just takes a long time to finish"<< endl;
    int max_trains = graph.getVertexSet()[0]->getAdj()[0]->getWeight();
    for(int i = 0; i < graph.getNumVertex(); i++){
        for(int j = i+1; j < graph.getNumVertex(); j++){
            cout << i << " " << j << endl;
            int cap = graph.maxFlow(graph.getVertexSet()[i]->getId(),graph.getVertexSet()[j]->getId());
            if(cap >= max_trains){
                max_trains = cap;
                pair<Vertex*,Vertex*> p(graph.getVertexSet()[i],graph.getVertexSet()[j]);
                pairs.push_back(p);
                capacities.push_back(cap);
            }

            for(int i1 = 0; i1 < graph.getNumVertex(); i1++){
                for(int j1 = 0; j1 < graph.getVertexSet()[i1]->getAdj().size(); j1++){
                    graph.getVertexSet()[i1]->setVisited(false);
                    graph.getVertexSet()[i1]->getAdj()[j1]->setFlow(0);
                }
            }
        }
    }
    cout << "\nHere are the station/s that require the most ammount of trains\nCapacity: " << max_trains << endl;
    for(int i = 0; i < pairs.size(); i++){
        if(capacities[i] == max_trains){
            cout << pairs[i].first->getName() << " --> " << pairs[i].second->getName() << endl;
        }
    }

}

int main() {
    cout << "Please build the graph before selecting the other options\n";
    Graph graph;
    int key = 1; //equals to 1 to get inside of loop
    while(key){
        menuDisplay();
        cin >> key;
        if(key == 1){
            create_stations(graph);
            create_networks(graph);
            cout << "\nRailways built" << endl;
        }
        else if(key == 2){
            max_num_trains_two_stations(graph);
        }
        else if(key == 3){
            max_amount_trains_capacity(graph);
        }

        for(int i = 0; i < graph.getNumVertex(); i++){
            for(int j = 0; j < graph.getVertexSet()[i]->getAdj().size(); j++){
                graph.getVertexSet()[i]->setVisited(false);
                graph.getVertexSet()[i]->getAdj()[j]->setFlow(0);
            }
        }
    }

    return 0;
}
