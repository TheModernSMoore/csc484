#include "graph.h"
#include "../Vector/vecStuff.h"
#include <cstdlib>
#include <iostream>

// Vertex implementation

Vertex::Vertex(sf::Vector2f pos, int numb) {
    position = pos;
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

sf::Vector2f Vertex::getPos() {
    return position;
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

Graph::Graph(int vertices, int edges) {
    if (edges >= vertices) {
        int edgesMade = 0;
        // Add all vertices
        for (int i = 0; i < vertices; i++) {
            addVertex(sf::Vector2f(rand() % 100, rand() % 100));
        }
        // Make a cycle
        for (int i = 0; i < vertices; i++) {
            addEdge(i, (i+1) % vertices);
            edgesMade++;
        }
        while (edgesMade < edges) {
            addEdge(rand() % vertices, rand() % vertices);
            edgesMade++;
        }
    }
}

bool Graph::addVertex(sf::Vector2f pos) {
    vertices.push_back(new Vertex(pos, num_of_verts++));
    return true;
}

bool Graph::addEdge(Vertex *from, Vertex *to) {
    if (from == nullptr || to == nullptr) {
        return false;
    }
    new Edge(from, to, length(from->getPos() - to->getPos()));
    return true;
}

bool Graph::addEdge(int from, int to) {
    return addEdge(getVertex(from), getVertex(to));
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

//  WorldVertex and WorldGraph implementations

// WorldVertex

WorldVertex::WorldVertex(sf::Vector2f pos, sf::Vector2f size, int numb) : Vertex(pos, numb) {
    area = sf::RectangleShape(size);
    area.setOrigin(size/2.f);
    area.setPosition(pos);
}

void WorldVertex::setValid(std::vector<sf::Shape*> worldObjects) {
    valid = true;
    sf::FloatRect area_rect = area.getGlobalBounds();
    for (sf::Shape* object : worldObjects) {
        sf::FloatRect object_rect = object->getGlobalBounds();
        if (area_rect.intersects(object_rect)) {
            valid = false;
            break;
        }
    }
}

bool WorldVertex::getValid() {
    return valid;
}

sf::RectangleShape WorldVertex::getShape() {
    return area;
}

sf::Vector2f WorldVertex::localizePosition() {
    return area.getPosition();
}

// WorldGraph

WorldGraph::WorldGraph(sf::Vector2f screen_size, sf::Vector2f vertex_size, std::vector<sf::Shape*> worldObjects) {
    rows = screen_size.y / vertex_size.y;
    columns = screen_size.x / vertex_size.x;
    vert_size = vertex_size;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            addVertex(sf::Vector2f(j * vertex_size.x + vertex_size.x/2, i * vertex_size.y + vertex_size.y/2), vertex_size);
        }
    }
    
    for (WorldVertex* vert : world_vertices) {
        vert->setValid(worldObjects);
    }

    for (int i = 0; i < world_vertices.size(); i++) {
        if (world_vertices[i]->getValid()) {
            if (i >= columns && world_vertices[i - columns]->getValid()) {
                addEdge(i, i - columns);
            }
            if (i % columns > 0 && world_vertices[i - 1]->getValid()) {
                addEdge(i, i - 1);
            }
            if (i <= (rows - 1) * columns - 1 && world_vertices[i + columns]->getValid()) {
                addEdge(i, i + columns);
            }
            if (i % columns < columns - 1 && world_vertices[i + 1]->getValid()) {
                addEdge(i, i + 1);
            }
        }
    }
}

WorldVertex* WorldGraph::getWorldVertex(int idx) {
    return world_vertices[idx];
}

std::vector<WorldVertex*> WorldGraph::getWorldGraphVertices() {
    return world_vertices;
}

WorldVertex* WorldGraph::quantizePosition(sf::Vector2f position) {
    int row = position.y / vert_size.y;
    int column = position.x / vert_size.x;
    return world_vertices[columns * row + column];
}

bool WorldGraph::addVertex(sf::Vector2f pos, sf::Vector2f size) {
    world_vertices.push_back(new WorldVertex(pos, size, vert_amount++));
    return true;
}