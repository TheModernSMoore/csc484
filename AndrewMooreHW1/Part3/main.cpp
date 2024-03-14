#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

int main(int argc, char const *argv[])
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "My Window");

    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");

    std::vector<sf::Sprite> sprites;

    sf::Sprite first_sprite;
    first_sprite.setTexture(texture);

    sprites.push_back(first_sprite);


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

        int sprite_num = 0;
        bool create = false;
        bool destroy = false;
        for (auto & sprite : sprites) {
            sprite_num++;
            switch ((int) sprite.getRotation())
            {
            case 0:
                sprite.move(4, 0);
                if (sprite.getPosition().x + 10 >= 640) {
                    sprite.rotate(90);
                    if (sprites.size() < 4) {
                        create = true;
                    }
                }
                break;
            
            case 90:
                sprite.move(0, 3);
                if (sprite.getPosition().y + 7 >= 480) {
                    sprite.rotate(90);
                }
                break;

            case 180:
                sprite.move(-4, 0);
                if (sprite.getPosition().x <= 0) {
                    sprite.rotate(90);
                }
                break;
            
            case 270:
                sprite.move(0, -3);
                if (sprite.getPosition().y <= 0) {
                    destroy = true;
                }
                break;
            }
        }

        if (create) {
            sf::Sprite new_sprite;
            new_sprite.setTexture(texture);

            sprites.push_back(new_sprite);
        }

        if (destroy) {
            sprites.erase(sprites.begin());
            if (sprites.size() == 0) {
                sf::Sprite new_sprite;
                new_sprite.setTexture(texture);

                sprites.push_back(new_sprite);
            }
        }

        // Clear the whole window before rendering a new frame
        window.clear(sf::Color::White);

        // Draw some graphical entities
        for (auto & sprite : sprites) {
            window.draw(sprite);
        }

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;

}
