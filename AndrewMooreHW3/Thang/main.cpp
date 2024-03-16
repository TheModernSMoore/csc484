#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "Graph/algorithm.h"

void printPath(std::vector<Edge*> path) {
    for (Edge* edge : path) {
        std::cout << edge->getFrom()->getID() << "-" << edge->getWeight() << "-" << edge->getTo()->getID() << ";";
    }
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    Graph hyrule;

    // Hand making this graph :(
    
    for (int i = 0; i < 23; i++) {
        hyrule.addVertex();
    }

    hyrule.addEdge(0, 1, 5);

    hyrule.addEdge(1, 0, 5);
    hyrule.addEdge(1, 6, 6);

    hyrule.addEdge(2, 1, 9);
    hyrule.addEdge(2, 7, 5);
    hyrule.addEdge(2, 9, 6);

    hyrule.addEdge(3, 9, 5);

    hyrule.addEdge(4, 7, 2);

    hyrule.addEdge(5, 0, 5);
    hyrule.addEdge(5, 14, 8);

    hyrule.addEdge(6, 5, 4);
    hyrule.addEdge(6, 11, 5);

    hyrule.addEdge(7, 4, 2);
    hyrule.addEdge(7, 8, 7);
    hyrule.addEdge(7, 11, 6);

    hyrule.addEdge(8, 9, 2);
    hyrule.addEdge(8, 10, 5);
    hyrule.addEdge(8, 12, 3);

    hyrule.addEdge(9, 2, 6);
    hyrule.addEdge(9, 3, 5);
    hyrule.addEdge(9, 8, 2);
    hyrule.addEdge(9, 10, 2);

    hyrule.addEdge(10, 3, 5);
    hyrule.addEdge(10, 9, 2);

    hyrule.addEdge(11, 6, 5);
    hyrule.addEdge(11, 7, 6);
    hyrule.addEdge(11, 13, 3);
    hyrule.addEdge(11, 14, 6);
    hyrule.addEdge(11, 20, 12);

    hyrule.addEdge(12, 8, 3);
    hyrule.addEdge(12, 15, 7);

    hyrule.addEdge(13, 11, 3);
    hyrule.addEdge(13, 15, 3);

    hyrule.addEdge(14, 6, 7);
    hyrule.addEdge(14, 11, 6);
    hyrule.addEdge(14, 16, 5);
    hyrule.addEdge(14, 20, 8);

    hyrule.addEdge(15, 17, 2);
    
    hyrule.addEdge(16, 14, 5);
    hyrule.addEdge(16, 19, 3);

    hyrule.addEdge(17, 14, 9);
    hyrule.addEdge(17, 18, 4);
    
    hyrule.addEdge(18, 17, 4);
    
    hyrule.addEdge(19, 16, 3);

    hyrule.addEdge(20, 11, 12);
    hyrule.addEdge(20, 14, 8);
    hyrule.addEdge(20, 17, 4);
    hyrule.addEdge(20, 21, 2);

    hyrule.addEdge(21, 20, 2);
    hyrule.addEdge(21, 22, 2);

    hyrule.addEdge(22, 18, 3);
    hyrule.addEdge(22, 21, 2);

    printPath(pathfindDijkstra(&hyrule, hyrule.getVertex(0), hyrule.getVertex(15)));
}
