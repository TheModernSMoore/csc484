#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "SteeringBehavior/steeringBehavior.h"
#include "Graph/algorithm.h"
#include "DecisionTree/decisionNode.h"

// This is the dash velocity for the character action
const float DASH_VEL = 100;

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

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

    KinematicBody character(&window, texture, &breadcrumbs);
    character.setPosition(100, 100);
    character.linear_pos = sf::Vector2f(100, 100);

    std::vector<sf::Shape*> worldObjects;

    // Boundary shapes

    sf::RectangleShape up(sf::Vector2f(640.f, 3.f));
    up.setPosition(0, 0);
    up.setFillColor(sf::Color::Black);

    worldObjects.push_back(&up);

    sf::RectangleShape down(sf::Vector2f(640.f, 3.f));
    down.setPosition(0, 480 - 3);
    down.setFillColor(sf::Color::Black);

    worldObjects.push_back(&down);

    sf::RectangleShape left(sf::Vector2f(3.f, 480.f));
    left.setPosition(0, 0);
    left.setFillColor(sf::Color::Black);

    worldObjects.push_back(&left);

    sf::RectangleShape right(sf::Vector2f(3.f, 480.f));
    right.setPosition(640 - 3, 0);
    right.setFillColor(sf::Color::Black);

    worldObjects.push_back(&right);

    // Room walls

    sf::RectangleShape room1down(sf::Vector2f(427.f, 3.f));
    room1down.setPosition(0, 240 + 4);
    room1down.setFillColor(sf::Color::Black);

    worldObjects.push_back(&room1down);

    sf::RectangleShape room1right1(sf::Vector2f(3.f, 105.f));
    room1right1.setPosition(320 - 4, 0);
    room1right1.setFillColor(sf::Color::Black);

    worldObjects.push_back(&room1right1);

    sf::RectangleShape room1right2(sf::Vector2f(3.f, 90.f));
    room1right2.setPosition(320 - 4, 155.f);
    room1right2.setFillColor(sf::Color::Black);

    worldObjects.push_back(&room1right2);

    sf::RectangleShape room2down(sf::Vector2f(165.f, 3.f));
    room2down.setPosition(473, 240 + 4);
    room2down.setFillColor(sf::Color::Black);

    worldObjects.push_back(&room2down);

    // Room objects

    sf::RectangleShape box1(sf::Vector2f(33.f, 53.f));
    box1.setPosition(504, 104);
    box1.setFillColor(sf::Color::Black);

    worldObjects.push_back(&box1);

    sf::RectangleShape box2(sf::Vector2f(93.f, 33.f));
    box2.setPosition(304, 354);
    box2.setFillColor(sf::Color::Black);

    worldObjects.push_back(&box2);

    sf::CircleShape circle(53);
    circle.setPosition(154, 354);
    circle.setFillColor(sf::Color::Black);

    worldObjects.push_back(&circle);

    WorldGraph worldGraph(sf::Vector2f(640, 480), sf::Vector2f(10, 10), worldObjects);
    EuclideanHeuristic *heuristic = nullptr;
    WorldVertex *goal = nullptr;
    WorldVertex *start = nullptr;

    // Clock to gather elapsed time
    sf::Clock time;
    time.restart();

    // float elapsed_time = 0;

    std::vector<Edge*> edge_path;
    int current_edge = 0;

    // Building the Decision Tree

    DashAction dash;
    WanderAction wanderAct;
    PathfindAction topLeft(sf::Vector2f(50, 50));
    PathfindAction bottomLeft(sf::Vector2f(640 - 50, 480 - 50));

    RandomDecision randomDecision(&dash, &topLeft);
    InsideObjectDecision insideDecision(&wanderAct, &bottomLeft);

    NextToWallDecision root(&insideDecision, &randomDecision);
    

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
        }

        // Make decision

        DecisionTreeNode* action = root.makeDecision();

        SteeringBehavior actionSteering;
        if (instanceof<DashAction>(action)) {
            // Set velcity to dash velocity
            Kinematic dashKinematic;
            dashKinematic.linear_vel = DASH_VEL;
            VelMatch velmatch(character, dashKinematic);
            actionSteering = velmatch.getSteering();
        } else if (instanceof<WanderAction>(action)) {
            // Just run a wander action
            Wander wander(character);
            actionSteering = wander.getSteering();
        } else if (instanceof<PathfindAction>(action)) {
            // IF PATH EXXISTS PTH FLLOW IF NOT MAKE THA PATH
            
            // User code below to find pathfind to the position found in this action.
            PathfindAction* pfa = dynamic_cast<PathfindAction*>(action);
            // First get the position in the action.
            goal = worldGraph.quantizePosition(pfa->position);
            start = worldGraph.quantizePosition(character.getPosition());
            delete heuristic;
            heuristic = new EuclideanHeuristic(goal);
            edge_path = pathfindAStar(&worldGraph, start, goal, heuristic);
            current_edge = 0;
        }

        // Below needs to only be called if the path exists, else the character should just be updated with the acitonSteering.
        // Actually just put it in there buddy

        if (current_edge < edge_path.size()) {
            Edge* curr_edge = edge_path[current_edge];
            WorldVertex* curr_vert = (WorldVertex*)curr_edge->getTo();
            path_kinematic.linear_pos = curr_vert->localizePosition();
            path_kinematic.angular_pos = atan2(path_kinematic.linear_pos.y - character.linear_pos.y, path_kinematic.linear_pos.x - character.linear_pos.x);
            Arrive arrive(character, path_kinematic);
            Align align(character, path_kinematic);

            SteeringData steering;
            steering = arrive.getSteering();
            steering += align.getSteering();

            character.update(steering, delta_time);

            if (curr_vert == worldGraph.quantizePosition(character.getPosition())) {
                current_edge++;
            }
        } else {
            Arrive arrive(character, character);
            Align align(character, character);

            SteeringData steering;
            steering = arrive.getSteering();
            steering += align.getSteering();

            character.update(steering, delta_time);
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
        character.draw();

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;

}
