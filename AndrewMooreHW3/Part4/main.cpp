#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "SteeringBehavior/steeringBehavior.h"
#include "Graph/algorithm.h"

int main(int argc, char const *argv[])
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "My Window");

    // Limit the framerate to 60 frames per second (this step is optional)

    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");

    // Will modify fields based on clicked pos
    Kinematic path_kinematic;
    path_kinematic.linear_pos = sf::Vector2f(320, 240);

    std::vector<crumb> breadcrumbs = std::vector<crumb>();
    for(int i = 0; i < 10; i++)
    {
        crumb c(i);
        breadcrumbs.push_back(c);
    }

    KinematicBody path_follower(&window, texture, &breadcrumbs);
    path_follower.setPosition(320, 240);
    path_follower.linear_pos = sf::Vector2f(320, 240);

    std::vector<sf::Shape*> worldObjects;

    sf::RectangleShape box1(sf::Vector2f(35.f, 50.f));
    box1.setPosition(200, 300);
    box1.setFillColor(sf::Color::Black);

    worldObjects.push_back(&box1);

    sf::RectangleShape box2(sf::Vector2f(100.f, 30.f));
    box2.setPosition(300, 100);
    box2.setFillColor(sf::Color::Black);

    worldObjects.push_back(&box2);

    WorldGraph worldGraph(sf::Vector2f(640, 480), sf::Vector2f(5, 5), worldObjects);
    EuclideanHeuristic *heuristic = nullptr;
    WorldVertex *goal = nullptr;
    WorldVertex *start = nullptr;

    // Clock to gather elapsed time
    sf::Clock time;
    time.restart();

    // float elapsed_time = 0;

    std::vector<Edge*> edge_path;
    int current_edge = 0;

    

    // The main loop - ends as soon as the window is closed
    while (window.isOpen()) {

        // Get the elapsed_time since the last frame
        float delta_time = time.restart().asSeconds();

        // Event processing
        sf::Event event;
        while (window.pollEvent(event)) {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                std::cout << "borther" << std::endl;
                goal = worldGraph.quantizePosition(sf::Vector2f((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y));
                std::cout << goal->getID() << std::endl;
                std::cout << goal->getValid() << std::endl;
                start = worldGraph.quantizePosition(path_follower.getPosition());
                delete heuristic;
                heuristic = new EuclideanHeuristic(goal);
                edge_path = pathfindAStar(&worldGraph, start, goal, heuristic);
                std::cout << edge_path.size() << std::endl;
                current_edge = 0;
            }
        }

        if (current_edge < edge_path.size()) {
            WorldVertex *curr_edge = (WorldVertex*)edge_path[current_edge];
            path_kinematic.linear_pos = curr_edge->localizePosition();
            path_kinematic.angular_pos = atan2(path_kinematic.linear_pos.y - path_follower.linear_pos.y, path_kinematic.linear_pos.x - path_follower.linear_pos.x);
            Arrive arrive(path_follower, path_kinematic);
            Align align(path_follower, path_kinematic);

            SteeringData steering;
            steering = arrive.getSteering();
            steering += align.getSteering();

            path_follower.update(steering, delta_time);

            if (edge_path[current_edge]->getTo() == worldGraph.quantizePosition(path_follower.getPosition())) {
                current_edge++;
            }
        }
        

        // Clear the whole window before rendering a new frame
        window.clear(sf::Color::White);

        // Draw some graphical entities
        for(int i = 0; i < breadcrumbs.size(); i++) {
            breadcrumbs[i].draw(&window);
        }
        // Draw world objects
        for(int i = 0; i < worldObjects.size(); i++) {
            window.draw(*(worldObjects[i]));
        }
        path_follower.draw();

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;

}
