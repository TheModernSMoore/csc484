#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "../Kinematic/kinematic.h"

struct SteeringData {
    // Linear acceleration
    sf::Vector2f linear;
    // Angular acceleration
    float angular = 0;

    SteeringData operator+=(SteeringData other);
    SteeringData operator+(SteeringData other);
    SteeringData operator*(float constant);
};

class SteeringBehavior {
    public:
        SteeringBehavior();
        virtual SteeringData getSteering() = 0;
};

class Arrive : public SteeringBehavior {
    private:
        Kinematic character;
        Kinematic target;

        float maxAccel = 100;
        float maxSpeed = 25;

        float targetRadius = 3;
        float slowRadius = 5;

        float timeToTarget = 0.1;
    public:
        Arrive(Kinematic character, Kinematic target);
        SteeringData getSteering();
        
};

class Align : public SteeringBehavior {
    private:
        Kinematic character;
        Kinematic target;

        float maxAngularAccel = 2*M_PI;
        float maxRotation = M_PI / 2;

        float targetRadius = 0.0000001;
        float slowRadius = M_PI/10;

        float timeToTarget = 0.1;
    public:
        Align(Kinematic character, Kinematic target);
        SteeringData getSteering();
    
};

class VelMatch : public SteeringBehavior {
    private:
        Kinematic character;
        Kinematic target;

        float maxAccel = 100000;

        float timeToTarget = 0.1;

    public:
        VelMatch(Kinematic character, Kinematic target);
        SteeringData getSteering();
        
};

class RotMatch : public SteeringBehavior {
    private:
        Kinematic character;
        Kinematic target;

        float maxAngularAccel;

        float timeToTarget = 0.1;

    public:
        RotMatch(Kinematic character, Kinematic target);
        SteeringData getSteering();
        
};

class Wander : public SteeringBehavior {
    private:
        Kinematic character;

        float wanderRate = 10;
        float wanderOffset = 100;
        float wanderRadius = 10;

    public:
        Wander(Kinematic character);
        SteeringData getSteering();
};

class Separation : public SteeringBehavior {
    private:
        Kinematic character;
        std::vector<KinematicBody*> targets;

        float maxAccel = 19;
        float threshold = 10;
        float decayCoefficient = 5;

    public:
        Separation(Kinematic character, std::vector<KinematicBody*> targets);
        SteeringData getSteering();
};

class Flock : public SteeringBehavior {
    private:
        Kinematic character;
        std::vector<KinematicBody*> targets;

        float neighborRadius = 50;
        float maxAccel = 50;
        float maxAngularAccel = 2*M_PI;

    public:
        Flock(Kinematic character, std::vector<KinematicBody*> targets);
        SteeringData getSteering();
};