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

    // Clock to gather elapsed time
    sf::Clock time;
    time.restart();

    // Hand making this graph :(
    
    hyrule.addVertex(sf::Vector2f(-3621, 1800));
    hyrule.addVertex(sf::Vector2f(-1655, 2572));
    hyrule.addVertex(sf::Vector2f(1657, 2440));
    hyrule.addVertex(sf::Vector2f(4231, 2746));
    hyrule.addVertex(sf::Vector2f(419, 2140));
    hyrule.addVertex(sf::Vector2f(-2932, 548));
    hyrule.addVertex(sf::Vector2f(-1361, 724));
    hyrule.addVertex(sf::Vector2f(1062, 1135));
    hyrule.addVertex(sf::Vector2f(2613, 1144));
    hyrule.addVertex(sf::Vector2f(3149, 1691));
    hyrule.addVertex(sf::Vector2f(3966, 1611));
    hyrule.addVertex(sf::Vector2f(-249, 52));
    hyrule.addVertex(sf::Vector2f(3331, 545));
    hyrule.addVertex(sf::Vector2f(888, -174));
    hyrule.addVertex(sf::Vector2f(-1449, -1269));
    hyrule.addVertex(sf::Vector2f(1817, -988));
    hyrule.addVertex(sf::Vector2f(-2804, -2226));
    hyrule.addVertex(sf::Vector2f(1761, -1925));
    hyrule.addVertex(sf::Vector2f(3545, -2090));
    hyrule.addVertex(sf::Vector2f(-3845, -2926));
    hyrule.addVertex(sf::Vector2f(530, -3451));
    hyrule.addVertex(sf::Vector2f(1552, -3538));
    hyrule.addVertex(sf::Vector2f(3007, -3474));

    hyrule.addEdge(0, 1);

    hyrule.addEdge(1, 0);
    hyrule.addEdge(1, 6);

    hyrule.addEdge(2, 1);
    hyrule.addEdge(2, 7);
    hyrule.addEdge(2, 9);

    hyrule.addEdge(3, 9);

    hyrule.addEdge(4, 7);

    hyrule.addEdge(5, 0);
    hyrule.addEdge(5, 14);

    hyrule.addEdge(6, 5);
    hyrule.addEdge(6, 11);

    hyrule.addEdge(7, 4);
    hyrule.addEdge(7, 8);
    hyrule.addEdge(7, 11);

    hyrule.addEdge(8, 9);
    hyrule.addEdge(8, 10);
    hyrule.addEdge(8, 12);

    hyrule.addEdge(9, 2);
    hyrule.addEdge(9, 3);
    hyrule.addEdge(9, 8);
    hyrule.addEdge(9, 10);

    hyrule.addEdge(10, 3);
    hyrule.addEdge(10, 9);

    hyrule.addEdge(11, 6);
    hyrule.addEdge(11, 7);
    hyrule.addEdge(11, 13);
    hyrule.addEdge(11, 14);
    hyrule.addEdge(11, 20);

    hyrule.addEdge(12, 8);
    hyrule.addEdge(12, 15);

    hyrule.addEdge(13, 11);
    hyrule.addEdge(13, 15);

    hyrule.addEdge(14, 6);
    hyrule.addEdge(14, 11);
    hyrule.addEdge(14, 16);
    hyrule.addEdge(14, 20);

    hyrule.addEdge(15, 17);
    
    hyrule.addEdge(16, 14);
    hyrule.addEdge(16, 19);

    hyrule.addEdge(17, 14);
    hyrule.addEdge(17, 18);
    
    hyrule.addEdge(18, 17);
    
    hyrule.addEdge(19, 16);

    hyrule.addEdge(20, 11);
    hyrule.addEdge(20, 14);
    hyrule.addEdge(20, 17);
    hyrule.addEdge(20, 21);

    hyrule.addEdge(21, 20);
    hyrule.addEdge(21, 22);

    hyrule.addEdge(22, 18);
    hyrule.addEdge(22, 21);

    // Get the elapsed_time since the last frame
    float graph_time;

    time.restart().asSeconds();
    printPath(pathfindDijkstra(&hyrule, hyrule.getVertex(0), hyrule.getVertex(15)));
    graph_time = time.restart().asSeconds();
    std::cout << "Small Dijkstra = " << graph_time << std::endl << std::endl;
    
    time.restart().asSeconds();
    printPath(pathfindAStar(&hyrule, hyrule.getVertex(0), hyrule.getVertex(15), new EuclideanHeuristic(hyrule.getVertex(15))));
    graph_time = time.restart().asSeconds();
    std::cout << "Small AStar Euclidean = " << graph_time << std::endl << std::endl;

    time.restart().asSeconds();
    printPath(pathfindAStar(&hyrule, hyrule.getVertex(0), hyrule.getVertex(15), new ManhattanHeuristic(hyrule.getVertex(15))));
    graph_time = time.restart().asSeconds();
    std::cout << "Small AStar Manhattan = " << graph_time << std::endl << std::endl;

    Graph bigGraph(100000, 500000);

    time.restart().asSeconds();
    printPath(pathfindDijkstra(&bigGraph, bigGraph.getVertex(0), bigGraph.getVertex(2568)));
    graph_time = time.restart().asSeconds();
    std::cout << "Big Dijkstra = " << graph_time << std::endl << std::endl;

    time.restart().asSeconds();
    printPath(pathfindAStar(&bigGraph, bigGraph.getVertex(0), bigGraph.getVertex(2568), new EuclideanHeuristic(bigGraph.getVertex(2568))));
    graph_time = time.restart().asSeconds();
    std::cout << "Big AStar Euclidean = " << graph_time << std::endl << std::endl;

    time.restart().asSeconds();
    printPath(pathfindAStar(&bigGraph, bigGraph.getVertex(0), bigGraph.getVertex(2568), new ManhattanHeuristic(bigGraph.getVertex(2568))));
    graph_time = time.restart().asSeconds();
    std::cout << "Small AStar Manhattan = " << graph_time << std::endl << std::endl;
}
