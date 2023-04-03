#include <iostream>
#include "Station.h"
#include "Graph.h"
#include "VertexEdge.h"
#include <set>
#include "file_reader.h"

using namespace std;

///Function that displays the menu
///Complexity: O(1)
void menuDisplay() {
    cout << "\n<---------------------------Menu------------------------------>\nEnter your option:" << endl;
    cout << "1) Read data files" << endl;
    cout << "<----------------------Railway manager------------------------>\n";
    cout << "2) Find the maximum amount of trains between two stations \n";
    cout << "3) Find the pair of stations that require the most amount of trains when taking full advantage of the network \n";
    cout << "4) Budget information \n";
    cout << "5) Get maximum amount of trains arriving at a station simultaneously \n";
    cout << "6) Optimization analysis \n";
    cout << "<----------------Reliability and line failures---------------->\n";
    cout << "7) In reduced connectivity, get max amount of trains travelling between two stations \n";
    cout << "8) In segment failure, get most affected stations \n";
    cout << "0) Exit\n";
    cout << "Option:";
}

///Function that computes maximum num of trains between 2 given stations
///Complexity: O(VE^2)
int max_num_trains_two_stations(Graph g_st, Graph g_ap, string s1, string s2){
    Vertex* source_st = g_st.findVertex(Station(s1)), *source_ap = g_ap.findVertex(Station(s1));
    Vertex* destination_st = g_st.findVertex(Station(s2)), *destination_ap = g_ap.findVertex(Station(s2));

    if(destination_st == nullptr || destination_ap == nullptr || source_st == nullptr || source_ap == nullptr){
        cout << "\nInvalid station inserted" << endl;
        return -1;
    }

    int max_trains = g_st.maxFlow(source_st->getId(),destination_st->getId());
    max_trains += g_ap.maxFlow(source_ap->getId(),destination_ap->getId());

    return max_trains;
}

///Function that computes maximum num of trains between all pairs of stations
///Complexity: O(V^3E^2)
void max_amount_trains_capacity(Graph g_st, Graph g_ap){
    int count = 0, progress_bar_count = 0;
    vector<pair<Vertex*,Vertex*>> pairs;
    vector<int> capacities;
    cout << "Loading:\nWait until it reaches 100%"<< endl;
    int max_trains = g_st.getVertexSet()[0]->getAdj()[0]->getWeight();
    for(int i = 0; i < g_st.getNumVertex(); i++){
        if(count % 5 == 0){
            progress_bar_count++;
            if(progress_bar_count > 100){progress_bar_count = 100;}
            cout << "\r" << progress_bar_count << "%" << std::flush;
        }
        count++;
        for(int j = i+1; j < g_st.getNumVertex(); j++){
            int cap = max_num_trains_two_stations(g_st, g_ap, g_st.getVertexSet()[i]->getStation().name, g_st.getVertexSet()[j]->getStation().name);
            if(cap >= max_trains){
                max_trains = cap;
                pair<Vertex*,Vertex*> p(g_st.getVertexSet()[i],g_st.getVertexSet()[j]);
                pairs.push_back(p);
                capacities.push_back(cap);
            }

        }
    }
    cout << "\nHere are the station/s that require the most amount of trains\nCapacity: " << max_trains << endl;
    for(int i = 0; i < pairs.size(); i++){
        if(capacities[i] == max_trains){
            cout << pairs[i].first->getName() << " --> " << pairs[i].second->getName() << endl;
        }
    }

}
///Function that shows where management should assign larger budgets for the purchasing and maintenance of trains
///Complexity: O(nVE^2)
void budgetInformation(Graph g_st,Graph g_ap, set<string> set1){
    vector<pair<string,int>> top;
    int min = set1.size() - 1;

    cout << "\n" << "Introduce here the number of the top k:";
    int k;
    cin >> k;
    if(k > min){
        k = min;
    }

    for(string s : set1){
        //this should build a subgraph with only the districts/mun from the set and then calculate the max
    }
}

///Function that computes maximum num of trains that can arrive at a single station by selecting all the source stations and creating a super source to unite all of those sources. We then apply tha max flow
///algorithm and compute the max_flow.
///Complexity: O(VE^2)
int max_num_trains_arrive_at_a_station_simultaneously(Graph g_st,Graph g_ap, Vertex *origin){

    int total_trains = 0;

    vector<Vertex *> source_stations_st;
    for(Vertex *v : g_st.getVertexSet()){
        if(v->getId() == origin->getId()){
            continue;
        }
        if(v->getAdj().size() == 1){
            source_stations_st.push_back(v);
        }
    }

    vector<Vertex *> source_stations_ap;
    for(Vertex *v : g_ap.getVertexSet()){
        if(v->getId() == origin->getId()){
            continue;
        }
        if(v->getAdj().size() == 1){
            source_stations_ap.push_back(v);
        }
    }

    g_st.addVertex(-1,Station("super"));
    g_ap.addVertex(-1,Station("super"));

    for(Vertex *v : source_stations_st){
        g_st.addEdge(-1, v->getId(), INF, "STANDARD");
    }

    for(Vertex *v : source_stations_ap){
        g_ap.addEdge(-1, v->getId(), INF, "ALFA PENDULAR");
    }

    total_trains = g_st.maxFlow(-1,origin->getId());
    total_trains += g_ap.maxFlow(-1,origin->getId());

    Vertex *super_source = g_st.findVertex(-1);
    for(Vertex *v : source_stations_st){
        v->removeEdge(-1);
        v->removeEdge(-1);
        super_source->removeEdge(v->getId());
        super_source->removeEdge(v->getId());
    }

    super_source = g_ap.findVertex(-1);
    for(Vertex *v : source_stations_ap){
        v->removeEdge(-1);
        v->removeEdge(-1);
        super_source->removeEdge(v->getId());
        super_source->removeEdge(v->getId());
    }

    return total_trains;
}

///Function that computes maximum num of trains that can travel between 2 stations with minimal cost
///Complexity: O(VE^2)
void max_trains_min_cost(Graph g_st, Graph g_ap){

    string s1,s2;
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "\nPlease Input the name of the origin station:";
    getline(cin,s1);
    cout << "\nPlease Input the name of the destination station:";
    getline(cin,s2);


    Vertex* source_st = g_st.findVertex(Station(s1)), *source_ap = g_ap.findVertex(Station(s1));
    Vertex* destination_st = g_st.findVertex(Station(s2)), *destination_ap = g_ap.findVertex(Station(s2));

    if(destination_st == nullptr || destination_ap == nullptr || source_st == nullptr || source_ap == nullptr){
        cout << "\nInvalid station inserted" << endl;
        return;
    }


    int max_trains_alfa = g_ap.maxFlow_minCost(g_ap.findVertex(Station(s1))->getId(),g_ap.findVertex(Station(s2))->getId(), "ALFA PENDULAR");
    int max_trains_standard = g_st.maxFlow_minCost(g_st.findVertex(Station(s1))->getId(),g_st.findVertex(Station(s2))->getId(), "STANDARD");

    if(max_trains_alfa + max_trains_standard == 0){
        cout << "Found no connection between this stations\n";
    }
    else{
        cout << "Max num of trains with min cost:" << max_trains_alfa + max_trains_standard << endl;
    }
}

///Function that computes maximum num of trains that can travel between 2 stations in a restricted graph
///Complexity: O(VE^2)
int max_flow_segment_failure(vector<pair<string,string>> segments, vector<string> services, string source, string destination){
    Graph graph_st_failure, graph_ap_failure;


    create_stations_restricted(graph_st_failure,graph_ap_failure);
    create_networks_restricted(graph_st_failure,graph_ap_failure,segments,services);

    Vertex* source_st = graph_st_failure.findVertex(Station(source)), *source_ap = graph_ap_failure.findVertex(Station(source));
    Vertex* destination_st = graph_st_failure.findVertex(Station(destination)), *destination_ap = graph_ap_failure.findVertex(Station(destination));

    if(destination_st == nullptr || destination_ap == nullptr || source_st == nullptr || source_ap == nullptr){
        cout << "\nInvalid station inserted" << endl;
        return -1;
    }

    int max_flow = graph_st_failure.maxFlow(source_st->getId(),destination_st->getId());
    max_flow += graph_ap_failure.maxFlow(source_ap->getId(),destination_ap->getId());

    return max_flow;
}

///Same function as max_num_trains_arrive_at_a_station_simultaneously but with some restrictions
///algorithm and compute the max_flow.
///Complexity: O(VE^2)
int max_num_trains_arrive_at_a_station_simultaneously_for_failure(Graph g_st,Graph g_ap, Vertex *origin, vector<Vertex *> source_stations_st, vector<Vertex *> source_stations_ap){

    int total_trains = 0;

    g_st.addVertex(-1,Station("super"));
    g_ap.addVertex(-1,Station("super"));

    for(Vertex *v : source_stations_st){
        if(v->getId() == origin->getId()){
            continue;
        }
        g_st.addEdge(-1, v->getId(), INF, "STANDARD");
    }

    for(Vertex *v : source_stations_ap){
        if(v->getId() == origin->getId()){
            continue;
        }
        g_ap.addEdge(-1, v->getId(), INF, "ALFA PENDULAR");
    }

    total_trains = g_st.maxFlow(-1,origin->getId());
    total_trains += g_ap.maxFlow(-1,origin->getId());

    Vertex *super_source = g_st.findVertex(-1);
    for(Vertex *v : source_stations_st){
        v->removeEdge(-1);
        v->removeEdge(-1);
        super_source->removeEdge(v->getId());
        super_source->removeEdge(v->getId());
    }

    super_source = g_ap.findVertex(-1);
    for(Vertex *v : source_stations_ap){
        v->removeEdge(-1);
        v->removeEdge(-1);
        super_source->removeEdge(v->getId());
        super_source->removeEdge(v->getId());
    }

    return total_trains;
}

///Function that computes the top-k most affected stations caused by a segment failure
///Complexity: O(nV^2E^2)
void topk_most_affected_stations(vector<pair<string,string>> segments, vector<string> services, Graph original_st, Graph original_ap, int k){

    vector<Vertex *> source_stations_st;
    for(Vertex *v : original_st.getVertexSet()){
        if(v->getAdj().size() == 1){
            source_stations_st.push_back(v);
        }
    }

    vector<Vertex *> source_stations_ap;
    for(Vertex *v : original_ap.getVertexSet()){
        if(v->getAdj().size() == 1){
            source_stations_ap.push_back(v);
        }
    }

    Graph graph_st_failure, graph_ap_failure;
    create_stations_restricted(graph_st_failure,graph_ap_failure);
    create_networks(graph_st_failure,graph_ap_failure);

    for(int i = 0; i < segments.size(); i++){

        vector<pair<string,string>> segments_copy;
        vector<string> services_copy;

        segments_copy.push_back(segments[i]);
        services_copy.push_back(services[i]);


        Vertex *source_seg = graph_st_failure.findVertex(Station(segments_copy[0].first));
        Vertex *dest_seg = graph_st_failure.findVertex(Station(segments_copy[0].second));
        double w_st = 0;
        for(Edge *e : source_seg->getAdj()){
            if(e->getDest() == dest_seg){
                w_st = e->getWeight();
            }
        }
        dest_seg->removeEdge(source_seg->getId());
        source_seg->removeEdge(dest_seg->getId());

        source_seg = graph_ap_failure.findVertex(Station(segments_copy[0].first));
        dest_seg = graph_ap_failure.findVertex(Station(segments_copy[0].second));
        double w_ap = 0;
        for(Edge *e : source_seg->getAdj()){
            if(e->getDest() == dest_seg){
                w_ap = e->getWeight();
            }
        }
        dest_seg->removeEdge(source_seg->getId());
        source_seg->removeEdge(dest_seg->getId());

        vector<pair<int,string>> top;
        for(Vertex *v: original_st.getVertexSet()){
            int original_arrivals = max_num_trains_arrive_at_a_station_simultaneously(original_st,original_ap,v);
            int failure_arrivals = max_num_trains_arrive_at_a_station_simultaneously_for_failure(graph_st_failure,graph_ap_failure,v,source_stations_st,source_stations_ap);

            if(failure_arrivals < original_arrivals){
                pair<int,string> pair(original_arrivals-failure_arrivals,v->getStation().name);
                top.push_back(pair);
            }
        }

        graph_st_failure.addBidirectionalEdge(source_seg->getId(),dest_seg->getId(),w_st,"STANDARD");
        graph_ap_failure.addBidirectionalEdge(source_seg->getId(),dest_seg->getId(),w_ap,"ALFA PENDULAR");

        sort(top.begin(),top.end());
        if(k > top.size()){
            k = top.size();
        }
        cout << "\nSegment: " << segments[i].first << " - " << segments[i].second << " using " << services[i] << " service:" << endl;
        for(int j = k-1; j >= 0; j--){
            cout << top[j].second << " station lost " << top[j].first << " capacity" << endl;
        }
    }

}

int main() {
    cout << "Please build the graph before selecting the other options\n";
    Graph graph_st, graph_ap;
    set<string> districts;
    set<string> municipalities;
    char key = 1; //equals to 1 to get inside of loop
    while(key != '0'){
        menuDisplay();
        cin >> key;
        if(key == '1'){
            create_stations(graph_st,graph_ap, districts, municipalities);
            create_networks(graph_st,graph_ap);
            cout << "\nRailways built" << endl;
        }
        else if(key == '2'){
            string s1,s2;
            cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nPlease Input the name of the origin station:";
            getline(cin,s1);
            cout << "\nPlease Input the name of the destination station:";
            getline(cin,s2);
            int max_trains = max_num_trains_two_stations(graph_st,graph_ap, s1, s2);
            if(max_trains == 0){
                cout << "Found no connection between this stations\n";
            }
            else if(max_trains == -1){
                continue;
            }
            else{
                cout << "Max num of trains:" << max_trains << endl;
            }
        }
        else if(key == '3'){
            max_amount_trains_capacity(graph_st,graph_ap);
        }
        else if(key == '4'){
            int key_2 = 0;
            cout << "\nWould you like to see the top k:\n" << "1-Districts\n2-Municipalities: \nSelect here:";
            cin >> key_2;
            if(key_2 == 1){
                budgetInformation(graph_st, graph_ap, districts);
            }
            else if(key_2 == 2){
                budgetInformation(graph_st, graph_ap, municipalities);
            }
            else{
                cout << "Invalid option selected \n";
            }
        }
        else if(key == '5'){
            string input_string;
            cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nPlease Input the name of the origin station:";
            getline(cin,input_string);

            Vertex *origin = graph_st.findVertex(Station(input_string));
            if(origin == nullptr){
                cout << "\n" << "Could not find the given station" << endl;
            }
            double total_trains = max_num_trains_arrive_at_a_station_simultaneously(graph_st,graph_ap, origin);
            cout << "\n" << total_trains << " trains can arrive to " << input_string << " station" << endl;
        }
        else if(key == '6'){
            max_trains_min_cost(graph_st, graph_ap);
        }
        else if(key == '7'){
            string s1,s2;
            cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nPlease Input the name of the origin station:";
            getline(cin,s1);
            cout << "\nPlease Input the name of the destination station:";
            getline(cin,s2);

            bool more_segments = true;
            vector<pair<string,string>> segments;
            vector<string> services;
            while(more_segments){
                string s_origin, s_dest, service;
                cout << "\nPlease Input the name of the origin station of the segment:";
                getline(cin,s_origin);
                cout << "\nPlease Input the name of the destination station of the segment:";
                getline(cin,s_dest);
                cout << "\nPlease Input service of the segment:";
                getline(cin,service);

                pair<string,string> pair(s_origin,s_dest);
                segments.push_back(pair);
                services.push_back(service);

                char option;
                cout << "\nDo you wish to add more segments?\n1->Yes\nPress anything->No\nOption:";
                cin >> option;
                if(option == '1'){
                    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                else{
                    more_segments = false;
                }

            }

            int max_trains = max_flow_segment_failure(segments,services,s1,s2);
            if(max_trains == 0){
                cout << "Found no connection between this stations\n";
            }
            else if(max_trains == -1){
                continue;
            }
            else{
                cout << "Max num of trains:" << max_trains << endl;
            }
        }
        else if(key == '8'){
            string k;
            cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nPlease input the number of affected stations you wish to see (TOP-K):";
            getline(cin,k);
            bool more_segments = true;
            vector<pair<string,string>> segments;
            vector<string> services;
            while(more_segments) {
                string s_origin, s_dest, service;
                cout << "\nPlease Input the name of the origin station of the segment:";
                getline(cin, s_origin);
                cout << "\nPlease Input the name of the destination station of the segment:";
                getline(cin, s_dest);
                cout << "\nPlease Input service of the segment:";
                getline(cin, service);

                pair<string, string> pair(s_origin, s_dest);
                segments.push_back(pair);
                services.push_back(service);

                char option;
                cout << "\nDo you wish to add more segments?\n1->Yes\nPress anything->No\nOption:";
                cin >> option;
                if (option == '1') {
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                } else {
                    more_segments = false;
                }
            }
            topk_most_affected_stations(segments,services,graph_st,graph_ap,stoi(k));
        }
        else if(key == '0'){}
        else{
            cout << "\nInvalid option. Please insert a valid option." << endl;
        }
    }
    return 0;
}
