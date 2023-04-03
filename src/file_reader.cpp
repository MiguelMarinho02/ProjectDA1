#include "file_reader.h"

///Function that builds the graph's nodes and adds districts and municipalities to according sets
///Complexity: O(N^2)
void create_stations(Graph &g_st, Graph &g_ap, set<string> &d, set<string> &m){
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
        if(municipality[0] == '"'){
            string rest;
            getline(ss,rest,'"');
            municipality += rest;
            municipality = municipality.substr(1,municipality.size()-1);
            getline(ss,rest,',');
        }
        getline(ss,township,',');
        if(township[0] == '"'){
            string rest;
            getline(ss,rest,'"');
            township += rest;
            township = township.substr(1, township.size()-1);
            getline(ss,rest,',');
        }
        getline(ss,line,',');
        d.insert(district);
        m.insert(municipality);
        Station station(name,district,municipality,township,line);
        if(g_st.findVertex(station) != nullptr){
            continue;
        }
        g_st.addVertex(count,station);
        g_ap.addVertex(count,station);
        count++;
    }
}

///Function that builds the edges and avoids repetitions
///Complexity: O(N^2)
void create_networks(Graph &g_st,Graph &g_ap){

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

        if(service == "STANDARD") { //creates edges for standard service
            Vertex *v1 = g_st.findVertex(Station(station_A));
            Vertex *v2 = g_st.findVertex(Station(station_B));
            bool edge_exists = false;
            for (auto e: v1->getAdj()) {
                if (e->getDest()->getName() == v2->getName() && e->getService() == service) {
                    edge_exists = true;
                    break;
                }
            }

            if (edge_exists) {
                continue;
            }

            g_st.addBidirectionalEdge(v1->getId(), v2->getId(), stoi(capacity), service);
        }
        else if(service == "ALFA PENDULAR"){ //creates edges for alfa pendular service
            Vertex *v1 = g_ap.findVertex(Station(station_A));
            Vertex *v2 = g_ap.findVertex(Station(station_B));
            bool edge_exists = false;
            for (auto e: v1->getAdj()) {
                if (e->getDest()->getName() == v2->getName() && e->getService() == service) {
                    edge_exists = true;
                    break;
                }
            }

            if (edge_exists) {
                continue;
            }

            g_ap.addBidirectionalEdge(v1->getId(), v2->getId(), stoi(capacity), service);
        }
    }

}

///Function that builds the graph's nodes
///Complexity: O(N^2)
void create_stations_restricted(Graph &g_st, Graph &g_ap){
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
        if(municipality[0] == '"'){
            string rest;
            getline(ss,rest,'"');
            municipality += rest;
            municipality = municipality.substr(1,municipality.size()-1);
            getline(ss,rest,',');
        }
        getline(ss,township,',');
        if(township[0] == '"'){
            string rest;
            getline(ss,rest,'"');
            township += rest;
            township = township.substr(1, township.size()-1);
            getline(ss,rest,',');
        }
        getline(ss,line,',');
        Station station(name,district,municipality,township,line);
        if(g_st.findVertex(station) != nullptr){
            continue;
        }
        g_st.addVertex(count,station);
        g_ap.addVertex(count,station);
        count++;
    }
}

///Function that builds the edges and avoids repetitions with some restrictions
///Complexity: O(N^2)
void create_networks_restricted(Graph &g_st,Graph &g_ap,vector<pair<string,string>> segments, vector<string> services){
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

        bool segment_found = false;
        for(int i = 0; i < segments.size(); i++){
            if(((segments[i].first == station_A && segments[i].second == station_B) || (segments[i].second == station_A && segments[i].first == station_B)) && services[i] == service){
                segment_found = true;
                break;
            }
        }

        if(segment_found){
            continue;
        }

        if(service == "STANDARD") { //creates edges for standard service
            Vertex *v1 = g_st.findVertex(Station(station_A));
            Vertex *v2 = g_st.findVertex(Station(station_B));
            bool edge_exists = false;
            for (auto e: v1->getAdj()) {
                if (e->getDest()->getName() == v2->getName() && e->getService() == service) {
                    edge_exists = true;
                    break;
                }
            }

            if (edge_exists) {
                continue;
            }

            g_st.addBidirectionalEdge(v1->getId(), v2->getId(), stoi(capacity), service);
        }
        else if(service == "ALFA PENDULAR"){ //creates edges for alfa pendular service
            Vertex *v1 = g_ap.findVertex(Station(station_A));
            Vertex *v2 = g_ap.findVertex(Station(station_B));
            bool edge_exists = false;
            for (auto e: v1->getAdj()) {
                if (e->getDest()->getName() == v2->getName() && e->getService() == service) {
                    edge_exists = true;
                    break;
                }
            }

            if (edge_exists) {
                continue;
            }

            g_ap.addBidirectionalEdge(v1->getId(), v2->getId(), stoi(capacity), service);
        }
    }
}
