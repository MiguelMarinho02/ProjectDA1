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

int Graph::bfs_for_max_flow(int source, int destination) {
    vertexSet[source]->setVisited(true);
    queue<pair<int,int>> q;
    q.push({source,INT_MAX});
    while(!q.empty()){
        int u = q.front().first;
        int path_flow = q.front().second;
        q.pop();
        for(Edge *e : vertexSet[u]->getAdj()){
            int v = e->getDest()->getId();
            int left_over_capacity = e->getWeight() - e->getFlow();
            if(!(vertexSet[v]->isVisited()) && left_over_capacity > 0){
                int new_flow = min(left_over_capacity,path_flow);
                e->setFlow(new_flow);
                vertexSet[v]->setPath(e);
                vertexSet[v]->setVisited(true);
                if(destination == v){
                    return new_flow;
                }
                q.push({v,new_flow});
            }
        }
    }
    return 0; //never reached the destination
}

int Graph::maxFlow(int source, int destination) {
    int totalFlow = 0;

    Graph rGraph = *this;
    while (true) {
        for(int i = 0; i < getNumVertex(); i++){
            rGraph.getVertexSet()[i]->setVisited(false);
            rGraph.getVertexSet()[i]->setPath(nullptr);
        }
        // find an augmenting path using BFS
        double path_flow = rGraph.bfs_for_max_flow(source, destination);
        // if no augmenting path found, break the loop
        if (path_flow == 0) {
            break;
        }

        for(Edge *e = rGraph.getVertexSet()[destination]->getPath(); e->getOrig()->getId() != source; e = e->getOrig()->getPath()){
            e->setFlow(e->getFlow()-path_flow);
        }
        // update total flow
        totalFlow += path_flow;
    }

    // return maximum flow
    return totalFlow;
}