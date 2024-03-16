#include "graph.h"

// Vertex implementation

Vertex::Vertex(int numb) {
    number = numb;
}

std::vector<Edge*> Vertex::getOutgoingEdges() {
    return outgoingEdges;
}

bool Vertex::addOutgoingEdge(Edge *edge) {
    if (edge == nullptr) {
        return false;
    }
    outgoingEdges.push_back(edge);
    return true;
}

int Vertex::getID() {
    return number;
}

// Edge implementation

Edge::Edge(Vertex *from, Vertex *to, float edgeWeight) {
    fromVert = from;
    toVert = to;
    weight = edgeWeight;
    from->addOutgoingEdge(this);
}

Vertex* Edge::getFrom() {
    return fromVert;
}

Vertex* Edge::getTo() {
    return toVert;
}

float Edge::getWeight() {
    return weight;
}

// Graph implementation

Graph::Graph() {}

bool Graph::addVertex() {
    vertices.push_back(new Vertex(num_of_verts++));
    return true;
}

bool Graph::addEdge(Vertex *from, Vertex *to, float weight) {
    if (from == nullptr || to == nullptr) {
        return false;
    }
    new Edge(from, to, weight);
    return true;
}

bool Graph::addEdge(int from, int to, float weight) {
    new Edge(getVertex(from), getVertex(to), weight);
    return true;
}

Vertex* Graph::getVertex(int idx) {
    if (idx >= vertices.size() || idx < 0) {
        return nullptr;
    }
    return vertices[idx];
}

std::vector<Edge*> Graph::getOutgoingEdges(Vertex *from) {
    return from->getOutgoingEdges();
}

std::vector<Vertex*> Graph::getGraphVertices() {
    return vertices;
}