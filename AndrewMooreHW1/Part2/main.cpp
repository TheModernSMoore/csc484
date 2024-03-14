#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, char const *argv[])
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "My Window");

    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");

    sf::Sprite sm_boid;
    sm_boid.setTexture(texture);

    int frame = 0;

    // The main loop - ends as soon as the window is closed
    while (window.isOpen()) {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event)) {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sm_boid.move(5, 0);

        if (sm_boid.getPosition().x + 10 >= 640) {
            sm_boid.setPosition(0, 0);
        } 

        // Clear the whole window before rendering a new frame
        window.clear(sf::Color::White);

        // Draw some graphical entities
        window.draw(sm_boid);

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;

}
