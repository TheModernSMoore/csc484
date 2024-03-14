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

    // Will modify fields based on clicked pos
    sf::Mouse mouse;
    Kinematic mouse_kinematic;
    mouse_kinematic.linear_pos = sf::Vector2f(320, 240);

    std::vector<crumb> breadcrumbs = std::vector<crumb>();
    for(int i = 0; i < 10; i++)
    {
        crumb c(i);
        breadcrumbs.push_back(c);
    }

    KinematicBody mouse_follower(&window, texture, &breadcrumbs);
    mouse_follower.setPosition(320, 240);
    mouse_follower.linear_pos = sf::Vector2f(320, 240);

    // Clock to gather elapsed time
    sf::Clock time;
    time.restart();

    float elapsed_time = 0;

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

        if (mouse.isButtonPressed(sf::Mouse::Left)) {
            mouse_kinematic.linear_pos = sf::Vector2f(mouse.getPosition(window));
            mouse_kinematic.angular_pos = atan2(mouse_kinematic.linear_pos.y - mouse_follower.linear_pos.y, mouse_kinematic.linear_pos.x - mouse_follower.linear_pos.x);

            // Just in case ig
            mouse_kinematic.linear_vel = sf::Vector2f(0, 0);
            mouse_kinematic.angular_vel = 0;
        }

        Arrive arrive(mouse_follower, mouse_kinematic);
        Align align(mouse_follower, mouse_kinematic);

        SteeringData steering;
        steering = arrive.getSteering();
        steering += align.getSteering();

        // Clear the whole window before rendering a new frame
        window.clear(sf::Color::White);

        mouse_follower.update(steering, delta_time);

        // Draw some graphical entities
        for(int i = 0; i < breadcrumbs.size(); i++) {
            breadcrumbs[i].draw(&window);
        }
        mouse_follower.draw();

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;

}
