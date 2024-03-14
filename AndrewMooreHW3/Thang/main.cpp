#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "SteeringBehavior/steeringBehavior.h"

int main(int argc, char const *argv[])
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "My Window");

    // Limit the framerate to 60 frames per second (this step is optional)

    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");

    std::vector<KinematicBody*> daBoids = std::vector<KinematicBody*>();
    std::vector<SteeringData> steering = std::vector<SteeringData>();
    
    std::srand(std::time(nullptr));

    for (int i = 0; i < 30; i++)
    {
        SteeringData dummy;
        steering.push_back(dummy);
        std::vector<crumb> *someCrumbs = new std::vector<crumb>();
        for (int i = 0; i < 10; i++) {
            crumb c(i);
            someCrumbs->push_back(c);
        }
        KinematicBody *boid = new KinematicBody(&window, texture, someCrumbs);
        boid->setPosition(std::rand() / (RAND_MAX/100) + 270, std::rand() / (RAND_MAX/100) + 190);
        boid->linear_pos = sf::Vector2f(boid->getPosition());
        daBoids.push_back(boid);
    }
    

    // Clock to gather elapsed time
    sf::Clock time;
    time.restart();

    float elapsed_time = 0;
    bool newSteer = true;

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

        // Clear the whole window before rendering a new frame
        window.clear(sf::Color::White);

        for (KinematicBody* boid : daBoids) {
            boid->drawCrumbs();
        }

        int idx = 0;
        if (newSteer) {
            newSteer = false;
            for (KinematicBody* boid : daBoids) {
                Flock flock(*boid, daBoids);
                steering.at(idx) = flock.getSteering();
                boid->update(steering.at(idx++), delta_time);
                boid->draw();
            }
        } else {
            elapsed_time += delta_time;
            if (elapsed_time >= 0.25) {
                elapsed_time = 0;
                newSteer = true;
            }
            for (KinematicBody* boid : daBoids) {
                boid->update(steering.at(idx++), delta_time);
                boid->draw();
            }
        }


        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;

}
