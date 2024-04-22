#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "SteeringBehavior/steeringBehavior.h"
#include "Graph/algorithm.h"
#include "DecisionTree/decisionNode.h"
#include "Vector/vecStuff.h"
#include "BehaviorTree/behavior.h"

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
    character.setPosition(0, 0);
    character.linear_pos = sf::Vector2f(0, 0);

    std::vector<crumb> crumbbreads = std::vector<crumb>();
    for(int i = 0; i < 10; i++)
    {
        crumb c(i);
        c.setFillColor(sf::Color::Red);
        crumbbreads.push_back(c);
    }

    KinematicBody monster(&window, texture, &crumbbreads);
    monster.setPosition(300, 300);
    monster.linear_pos = sf::Vector2f(300, 300);

    std::vector<sf::Shape*> worldObjects;

    // Boundary shapes

    sf::RectangleShape up(sf::Vector2f(640.f, 3.f));
    up.setPosition(0, 0);
    up.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&up);

    sf::RectangleShape down(sf::Vector2f(640.f, 3.f));
    down.setPosition(0, 480 - 3);
    down.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&down);

    sf::RectangleShape left(sf::Vector2f(3.f, 480.f));
    left.setPosition(0, 0);
    left.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&left);

    sf::RectangleShape right(sf::Vector2f(3.f, 480.f));
    right.setPosition(640 - 3, 0);
    right.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&right);

    // Room walls

    sf::RectangleShape room1down(sf::Vector2f(427.f, 3.f));
    room1down.setPosition(0, 240 + 4);
    room1down.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&room1down);

    sf::RectangleShape room1right1(sf::Vector2f(3.f, 105.f));
    room1right1.setPosition(320 - 4, 0);
    room1right1.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&room1right1);

    sf::RectangleShape room1right2(sf::Vector2f(3.f, 90.f));
    room1right2.setPosition(320 - 4, 155.f);
    room1right2.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&room1right2);

    sf::RectangleShape room2down(sf::Vector2f(165.f, 3.f));
    room2down.setPosition(473, 240 + 4);
    room2down.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&room2down);

    // Room objects

    sf::RectangleShape box1(sf::Vector2f(33.f, 53.f));
    box1.setPosition(504, 104);
    box1.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&box1);

    sf::RectangleShape box2(sf::Vector2f(93.f, 33.f));
    box2.setPosition(304, 354);
    box2.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&box2);

    sf::CircleShape circle(53);
    circle.setPosition(154, 354);
    circle.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&circle);

    sf::RectangleShape box3(sf::Vector2f(53.f, 53.f));
    box3.setPosition(104, 104);
    box3.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&box3);

    sf::CircleShape circle2(27);
    circle2.setPosition(354, 154);
    circle2.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&circle2);

    sf::CircleShape circle3(27);
    circle3.setPosition(154, 154);
    circle3.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&circle3);

    sf::CircleShape circle4(47);
    circle4.setPosition(404, 354);
    circle4.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&circle4);

    sf::CircleShape circle5(27);
    circle5.setPosition(354, 254);
    circle5.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&circle5);

    sf::CircleShape circle6(27);
    circle6.setPosition(204, 254);
    circle6.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&circle6);

    sf::RectangleShape box4(sf::Vector2f(53.f, 53.f));
    box4.setPosition(454, 54);
    box4.setFillColor(sf::Color::Blue);

    worldObjects.push_back(&box4);

    WorldGraph worldGraph(sf::Vector2f(640, 480), sf::Vector2f(10, 10), worldObjects);
    EuclideanHeuristic *heuristic = nullptr;
    WorldVertex *goal = nullptr;
    WorldVertex *start = nullptr;

    // Clock to gather elapsed time
    sf::Clock time;
    time.restart();
    float delta_time = 0;

    // float elapsed_time = 0;
    
    // Character path
    std::vector<Edge*> edge_path;
    int current_edge = 0;

    // Monster path
    std::vector<Edge*> monster_path;
    int current_monster_edge = 0;

    // Building the Decision Tree

    DashAction dash;
    WanderAction wanderAct;
    PathfindAction topLeft(sf::Vector2f(50, 50));
    PathfindAction bottomLeft(sf::Vector2f(50, 480 - 50));

    RandomDecision randomDecision(&dash, &topLeft);
    InsideObjectDecision insideDecision(&wanderAct, &bottomLeft, &worldGraph, &character);

    NextToWallDecision root(&insideDecision, &randomDecision, &worldGraph, &character);

    
    // Building the Behavior Tree

    CharacterInObjTask charInObj(&worldGraph, &character);
    BeelineTask beeline(&character, &monster, &delta_time, &current_monster_edge);
    CharacterInObjTask monstInObj(&worldGraph, &monster);
    WanderTask wanderTask(&monster, &delta_time, &current_monster_edge);
    PathfindTask topRight(&monster, sf::Vector2f(600, 50), &worldGraph, &monster_path, &current_monster_edge, &delta_time);
    PathfindTask bottomRight(&monster, sf::Vector2f(600, 480 - 50), &worldGraph, &monster_path, &current_monster_edge, &delta_time);
    // MAKE SURE TO UPDATE THIS TASK EACH LOOP
    PathfindTask characterPathTask(&monster, character.getPosition(), &worldGraph, &monster_path, &current_monster_edge, &delta_time);

    std::vector<Task*> beeliners;
    beeliners.push_back(&charInObj);
    beeliners.push_back(&beeline);
    Sequence beelineSeq(beeliners);

    std::vector<Task*> wanderers;
    wanderers.push_back(&monstInObj);
    wanderers.push_back(&wanderTask);
    Sequence wanderSeq(wanderers);

    std::vector<Task*> pathFinders;
    pathFinders.push_back(&topRight);
    pathFinders.push_back(&bottomRight);
    pathFinders.push_back(&characterPathTask);
    NonDeterministicSelector pathSel(pathFinders);

    std::vector<Task*> composites;
    composites.push_back(&beelineSeq);
    composites.push_back(&wanderSeq);
    composites.push_back(&pathSel);
    Selector behaviorRoot(composites);
    

    // The main loop - ends as soon as the window is closed
    while (window.isOpen()) {

        // Get the elapsed_time since the last frame
        delta_time = time.restart().asSeconds();

        // Event processing
        sf::Event event;
        while (window.pollEvent(event)) {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Make decision for character

        DecisionTreeNode* action = root.makeDecision();

        SteeringData actionSteering;

        if (instanceof<DashAction>(action)) {
            // Set velcity to dash velocity
            Kinematic dashKinematic;
            dashKinematic.linear_vel = normalize(character.linear_vel) * DASH_VEL;
            VelMatch velmatch(character, dashKinematic);
            actionSteering = velmatch.getSteering();
            // Reset the pathfinding algorithm since we've dashed away from the path
            current_edge = 1000;
        } else if (instanceof<WanderAction>(action)) {
            // Just run a wander action
            Wander wander(character);
            actionSteering = wander.getSteering();
        } else if (instanceof<PathfindAction>(action)) {
            // IF PATH EXXISTS PTH FLLOW IF NOT MAKE THA PATH
            if (current_edge < edge_path.size()) {
                Edge* curr_edge = edge_path[current_edge];
                WorldVertex* curr_vert = (WorldVertex*)curr_edge->getTo();
                path_kinematic.linear_pos = curr_vert->localizePosition();
                path_kinematic.angular_pos = atan2(path_kinematic.linear_pos.y - character.linear_pos.y, path_kinematic.linear_pos.x - character.linear_pos.x);
                Arrive arrive(character, path_kinematic);
                Align align(character, path_kinematic);

                actionSteering = arrive.getSteering();
                actionSteering += align.getSteering();

                if (curr_vert == worldGraph.quantizePosition(character.getPosition())) {
                    current_edge++;
                }
            } else {
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
        }

        character.update(actionSteering, delta_time);

        characterPathTask.setPosition(character.getPosition());


        // Run Behavior Tree for monster
        
        behaviorRoot.run();


        // Check if the monster and character collide
        // If so just reset positions
        sf::FloatRect character_rect = character.getGlobalBounds();
        sf::FloatRect monster_rect = monster.getGlobalBounds();
        if (character_rect.intersects(monster_rect)) {
            character.setPosition(0, 0);
            character.linear_pos = sf::Vector2f(0, 0);
            monster.setPosition(300, 300);
            monster.linear_pos = sf::Vector2f(300, 300);
            current_edge = 1000;
        }



        // Clear the whole window before rendering a new frame
        window.clear(sf::Color::White);

        // Draw world objects
        for(int i = 0; i < worldObjects.size(); i++) {
            window.draw(*(worldObjects[i]));
        }
        // Draw breadcrumbs
        for(int i = 0; i < breadcrumbs.size(); i++) {
            breadcrumbs[i].draw(&window);
        }
        character.draw();
        // Draw crumbbreads
        for(int i = 0; i < crumbbreads.size(); i++) {
            crumbbreads[i].draw(&window);
        }
        monster.draw();

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;

}
