#ifndef PROJECTDA1_FILE_READER_H
#define PROJECTDA1_FILE_READER_H

#include <iostream>
#include "Station.h"
#include "Graph.h"
#include "VertexEdge.h"
#include <fstream>
#include <sstream>
#include <set>


void create_stations(Graph &g_st, Graph &g_ap, set<string> &d, set<string> &m);
void create_networks(Graph &g_st,Graph &g_ap);
void create_stations_restricted(Graph &g_st, Graph &g_ap);
void create_networks_restricted(Graph &g_st,Graph &g_ap,vector<pair<string,string>> segments,vector<string> services);

#endif //PROJECTDA1_FILE_READER_H
