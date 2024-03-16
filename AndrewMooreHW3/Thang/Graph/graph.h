#pragma once
#include <vector>

class Edge;

class Vertex {
    private:
        int number;
        std::vector<Edge*> outgoingEdges;
    
    public:
        Vertex(int numb);
        std::vector<Edge*> getOutgoingEdges();
        bool addOutgoingEdge(Edge *edge);
        int getID();
};

class Edge {
    private:
        Vertex *fromVert;
        Vertex *toVert;
        float weight;

    public:
        // Constructor will assign basic fields as well as add themselves
        // To the outgoinEdges list in the "from" Vertex
        Edge(Vertex *from, Vertex *to, float edgeWeight = 1);
        Vertex* getFrom();
        Vertex* getTo();
        float getWeight();
};

class Graph {
    private:
        int num_of_verts = 0;
        std::vector<Vertex*> vertices;
    public:
        Graph();
        bool addVertex();
        bool addEdge(Vertex *from, Vertex *to, float weight = 1);
        bool addEdge(int from, int to, float weight = 1);
        Vertex* getVertex(int idx);
        std::vector<Edge*> getOutgoingEdges(Vertex *from);
        std::vector<Vertex*> getGraphVertices();

};