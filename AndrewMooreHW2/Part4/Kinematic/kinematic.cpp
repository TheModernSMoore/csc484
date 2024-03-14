#include "kinematic.h"
#include <iostream>
#include "../SteeringBehavior/steeringBehavior.h"

//Breadcrumb class
crumb::crumb(int id)
{
    //set initial position and size breadcrumbs   
    this->id = id;         
    this->setRadius(5.f);
    this->setFillColor(sf::Color::Green);
    this->setPosition(-100, -100);
}

//tell breadcrumb to render self, using current render window
void crumb::draw(sf::RenderWindow* window)
{
    window->draw(*this);
}

//set position of breadcrumb
void crumb::drop(float x, float y)
{
    this->setPosition(x, y);
}

//set position of breadcrumb
void crumb::drop(sf::Vector2f position)
{
    this->setPosition(position);
}

Kinematic::Kinematic() {
    linear_vel = sf::Vector2f(0, 0);
    angular_vel = 0;
    linear_pos = sf::Vector2f(0, 0);
    angular_pos = 0;
}

KinematicBody::KinematicBody(sf::RenderWindow* w, sf::Texture& tex, std::vector<crumb>* crumbs) : sf::Sprite(), Kinematic() {
    window = w;
    drop_timer = 100.f;
    crumb_idx = 0;
    breadcrumbs = crumbs;     
    this->setTexture(tex);
}

void KinematicBody::draw() {            
    window->draw(*this);
}

void KinematicBody::drawCrumbs() {            
    for (crumb cr : *breadcrumbs) {
        cr.draw(window);
    }
}  

void KinematicBody::update(SteeringData steering, float delta_time) {
    // Position update
    linear_pos += linear_vel * delta_time;

    setPosition(fmod(linear_pos.x, window->getSize().x), fmod(linear_pos.y, window->getSize().y));
    while (getPosition().x < 0) {
        move(window->getSize().x, 0);
    }
    while (getPosition().y < 0) {
        move(0, window->getSize().y);
    }

    angular_pos += angular_vel * delta_time;

    setRotation(angular_pos * 180 / M_PI);

    // Velocity update
    linear_vel += steering.linear * delta_time;

    angular_vel += steering.angular * delta_time;

    if (breadcrumbs) {
        //basic timer for leaving breadcrumbs
        if (drop_timer > 0)
        {
            drop_timer -= 0.1f;
        }
        else
        {
            drop_timer = 100.f;
            breadcrumbs->at(crumb_idx).drop(getPosition());

            if (crumb_idx < 9)
                crumb_idx++;
            else
                crumb_idx = 0;
        }
    }

}