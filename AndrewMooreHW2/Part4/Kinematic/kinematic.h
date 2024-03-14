#pragma once
#include <SFML/Graphics.hpp>

struct SteeringData;

//Breadcrumb class from Derek Martin
class crumb : sf::CircleShape
{
    public:
        crumb(int id);

        //tell breadcrumb to render self, using current render window
        void draw(sf::RenderWindow* window);

        //set position of breadcrumb
        void drop(float x, float y);

        //set position of breadcrumb
        void drop(sf::Vector2f position);

    private:
        int id;
};

class Kinematic {
    public:
        Kinematic();
        sf::Vector2f linear_vel;
        float angular_vel = 0;
        sf::Vector2f linear_pos;
        float angular_pos = 0;
};

class KinematicBody : public sf::Sprite, public Kinematic {
    // Modified boid variables and methods from Derek Martin
    private:
        //indice variables
        int crumb_idx;
        
        //float variables
        float drop_timer;
        
        sf::RenderWindow* window;    

        //point of breadcrumbs
        std::vector<crumb>* breadcrumbs;
    public:
        KinematicBody(sf::RenderWindow* w, sf::Texture& tex, std::vector<crumb>* crumbs);
        // update method
        void update(SteeringData steering, float delta_time);
        // draw method
        void draw();
        void drawCrumbs();
};