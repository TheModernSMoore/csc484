#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Edge;

class Vertex {
    private:
        int number;
        std::vector<Edge*> outgoingEdges;
        sf::Vector2f position;
    
    public:
        Vertex(sf::Vector2f pos, int numb);
        std::vector<Edge*> getOutgoingEdges();
        bool addOutgoingEdge(Edge *edge);
        sf::Vector2f getPos();
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
        Edge(Vertex *from, Vertex *to, float edgeWeight);
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
        Graph(int vertices, int edges);
        bool addVertex(sf::Vector2f pos);
        bool addEdge(Vertex *from, Vertex *to);
        bool addEdge(int from, int to);
        Vertex* getVertex(int idx);
        std::vector<Edge*> getOutgoingEdges(Vertex *from);
        std::vector<Vertex*> getGraphVertices();

};

// SFML based Vertex and Graph declarations

class WorldVertex : public Vertex {
    private:
        sf::RectangleShape area;
        bool valid;
    public:
        WorldVertex(sf::Vector2f pos, sf::Vector2f size, int numb);
        void setValid(std::vector<sf::Shape*> worldObjects);
        bool getValid();
        sf::RectangleShape getShape();
        sf::Vector2f localizePosition();

};

class WorldGraph : public Graph {
    private:
        sf::Vector2f vert_size;
        int rows;
        int columns;
        std::vector<WorldVertex*> world_vertices;
        int vert_amount = 0;
    public:
        WorldGraph(sf::Vector2f screen_size, sf::Vector2f vertex_size, std::vector<sf::Shape*> worldObjects);
        WorldVertex* quantizePosition(sf::Vector2f position);
        virtual bool addVertex(sf::Vector2f pos, sf::Vector2f size);
        virtual bool addEdge(Vertex *from, Vertex *to);
        virtual bool addEdge(int from, int to);
        WorldVertex* getWorldVertex(int idx);
        std::vector<WorldVertex*> getWorldGraphVertices();
        std::vector<WorldVertex*> getWorldVerticesAround(WorldVertex* middle);
};