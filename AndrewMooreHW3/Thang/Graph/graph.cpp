#include "graph.h"

// Vertex implementation

Vetex::Vertex(int numb) {
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

// Edge implementation

Edge::Edge(Vertex *from, Vertex *to, float edgeWeight = 1) {
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
    vertices.push_back(new Vertex(vertices++));
}

bool Graph::addEdge(Vertex *from, Vertex *to, float weight = 1) {
    if (from == nullptr || to == nullptr) {
        return false;
    }
    new Edge(from, to, weight);
    return true;
}

std::vector<Edge*> getOutgoingEdges(Vertex *from) {
    return from.getOutgoingEdges();
}

std::vector<Vertex*> getGraphVertices() {
    return vertices;
}