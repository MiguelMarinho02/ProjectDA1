#include "Graph.h"

int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
Vertex * Graph::findVertex(const int &id) const {
    for (auto v : vertexSet){
        if (v->getId() == id) {
            return v;
        }
    }
    return nullptr;
}

Vertex * Graph::findVertex(Station s) {
    for (auto v : vertexSet) {
        if (v->getStation().name == s.name){
            return v;
        }
    }
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
int Graph::findVertexIdx(const int &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const int &id, Station station) {
    if (findVertex(id) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id, station));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addEdge(const int &sourc, const int &dest, double w, string service) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w, service);
    return true;
}

bool Graph::addBidirectionalEdge(const int &sourc, const int &dest, double w, string service) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w, service);
    auto e2 = v2->addEdge(v1, w, service);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

Graph::~Graph() {
}

///BFS
///Complexity: O(V+E)
bool Graph::bfs_for_max_flow(Vertex *s, Vertex *t) {
    for(auto v : vertexSet){
        v->setVisited(false);
    }
    s->setVisited(true);
    std::queue<Vertex *> q;
    q.push(s);

    while(!q.empty() && !t->isVisited()){
        auto u = q.front();
        q.pop();
        for(auto e : u->getAdj()){
            if((!e->getDest()->isVisited() && e->getWeight() - e->getFlow() > 0)){
                e->getDest()->setVisited(true);
                e->getDest()->setPath(e);
                q.push(e->getDest());
            }
        }
    }
    return t->isVisited();
}

///Edmond´s Karp
///Complexity: O(VE^2)
int Graph::maxFlow(int source, int target) {
    Vertex *s = findVertex(source);
    Vertex *t = findVertex(target);

    for(int i = 0; i < getNumVertex(); i++){
        for(int j = 0; j < vertexSet[i]->getAdj().size(); j++){
            vertexSet[i]->getAdj()[j]->setFlow(0);
        }
    }

    int total_flow = 0;
    while (bfs_for_max_flow(s, t)) {
        double path_flow = INF;
        for(auto v = t; v != s; ){
            auto e = v->getPath();
            if (e->getDest() == v){
                path_flow = std::min(path_flow,e->getWeight() - e->getFlow());
                v = e->getOrig();
            }
            else{
                path_flow = std::min(path_flow,e->getFlow());
                v = e->getDest();
            }
        }

        for(auto v = t; v != s; ){
            auto e = v->getPath();
            double flow = e->getFlow();
            if (e->getDest() == v){
                e->setFlow(flow + path_flow);
                v = e->getOrig();
            }
            else{
                e->setFlow(flow - path_flow);
                v = e->getDest();
            }
        }
        total_flow += path_flow;
    }
    return total_flow;
}

///Modified Edmond´s Karp
///Complexity: O(VE^2)
int Graph::maxFlow_minCost(int source , int target, string service){
    Vertex *s = findVertex(source);
    Vertex *t = findVertex(target);
    double d;
    if(service == "ALFA PENDULAR"){
        d = 4.0;
    }
    else if(service == "STANDARD"){
        d = 2.0;
    }

    for(int i = 0; i < getNumVertex(); i++){
        for(int j = 0; j < vertexSet[i]->getAdj().size(); j++){
            vertexSet[i]->getAdj()[j]->setFlow(0);
        }
    }
    std::vector<double> aux;
    double total_flow = 0;
    while (bfs_for_max_flow(s, t)) {
        double path_flow = INF;
        for(auto v = t; v != s; ){
            auto e = v->getPath();
            if (e->getDest() == v){
                path_flow = std::min(path_flow,e->getWeight() - e->getFlow());
                v = e->getOrig();
            }
            else{
                path_flow = std::min(path_flow,e->getFlow());
                v = e->getDest();
            }
        }

        for(auto v = t; v != s; ){
            auto e = v->getPath();
            double flow = e->getFlow();
            if (e->getDest() == v){
                e->setFlow(flow + path_flow);
                v = e->getOrig();
            }
            else{
                e->setFlow(flow - path_flow);
                v = e->getDest();
            }
        }

        aux.push_back(path_flow);
    }

    if(aux.size() == 0){
        return 0;
    }

    double max_ratio = 0;
    int index = 0;
    for(int i = 0; i < aux.size(); i++){
        if(max_ratio < double(aux[i] / d)){
            index = i;
            max_ratio = double(aux[i] / d);
        }
    }

    return (int) aux[index];
}