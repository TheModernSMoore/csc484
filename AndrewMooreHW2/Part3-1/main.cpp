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

    std::vector<crumb> breadcrumbs = std::vector<crumb>();
    for(int i = 0; i < 10; i++)
    {
        crumb c(i);
        breadcrumbs.push_back(c);
    }

    KinematicBody wanderer(&window, texture, &breadcrumbs);
    wanderer.setPosition(320, 240);
    wanderer.linear_pos = sf::Vector2f(320, 240);

    // Clock to gather elapsed time
    sf::Clock time;
    time.restart();

    float elapsed_time = 0;
    bool newSteer = true;
    SteeringData steering;

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

        if (newSteer) {
            newSteer = false;
            Wander wander(wanderer);
            steering = wander.getSteering();
        } else {
            elapsed_time += delta_time;
            if (elapsed_time >= 0.25) {
                elapsed_time = 0;
                newSteer = true;
            }
        }

        // Clear the whole window before rendering a new frame
        window.clear(sf::Color::White);

        wanderer.update(steering, delta_time);

        // Draw some graphical entities
        for(int i = 0; i < breadcrumbs.size(); i++) {
            breadcrumbs[i].draw(&window);
        }
        wanderer.draw();

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;

}
