#include "steeringBehavior.h"
#include <cmath>
#include <iostream>
// #include <algorithm>

float length(sf::Vector2f vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f normalize(sf::Vector2f vec) {
    float len = length(vec);
    if (len == 0) {
        return sf::Vector2f(0, 0);
    }
    return sf::Vector2f(vec.x / len, vec.y / len);
}

SteeringBehavior::SteeringBehavior() {};

SteeringData SteeringData::operator+=(SteeringData other) {
    linear += other.linear;
    angular += other.angular;
    return *this;
}

SteeringData SteeringData::operator+(SteeringData other) {
    SteeringData result;
    result.linear = linear + other.linear;
    result.angular = angular + other.angular;
    return result;
}

SteeringData SteeringData::operator*(float constant) {
    SteeringData result;
    result.linear = linear * constant;
    result.angular = angular * constant;
    return result;
}

// Arrive implementation

Arrive::Arrive(Kinematic character, Kinematic target) {
    this->character = character;
    this->target = target;
}

// Below algorithm taken from class textbook
SteeringData Arrive::getSteering() {
    SteeringData result;
    
    // dir to target
    sf::Vector2f direction(target.linear_pos - character.linear_pos);
    float distance = length(direction);

    // If we are outside the slowRadius, then move at max speed
    float targetSpeed;
    if (distance > slowRadius) {
        targetSpeed = maxSpeed;
    } else { // Otherwise make a scaled speed
        targetSpeed = maxSpeed * distance / slowRadius;
    }

    // The target velocity combines speed and direction

    sf::Vector2f targetVelocity = normalize(direction);

    targetVelocity *= targetSpeed;

    // Check if we are there, return no steering if true
    if (distance < targetRadius) {
        targetVelocity = sf::Vector2f(0, 0);
    }

    // Acceleration tries to get to the target velocity
    result.linear = targetVelocity - character.linear_vel;
    result.linear /= timeToTarget;

    // Check if the acceleration is too fast.
    if (length(result.linear) > maxAccel) {
        result.linear = normalize(result.linear);
        result.linear *= maxAccel;
    }

    result.angular = 0;
    return result;
}

// Align Implementation

Align::Align(Kinematic character, Kinematic target) {
    this->character = character;
    this->target = target;
}

float mapToRange(float rotation) {
    float radian = std::fmod(rotation, 2*M_PI);
    if (radian > M_PI) {
        radian -= 2*M_PI;
    }
    if (radian < -M_PI) {
        radian += 2*M_PI;
    }
    return radian;
}

// Below algorithm taken from class textbook
SteeringData Align::getSteering() {
    SteeringData result;

    // Get the naive (non-normalized?) direction to the target
    float rotation = target.angular_pos - character.angular_pos;

    // Map the result to the (-pi, pi) interval
    rotation = mapToRange(rotation);
    float rotationSize = abs(rotation);

    float targetRotation;
    // If we are outside the slowRadius, then use maximum rotation
    if (rotationSize > slowRadius) {
        targetRotation = maxRotation;
    } else { // Otherwise calculate a scaled rotation.
        targetRotation = maxRotation * rotationSize / slowRadius;
    }

    // The final target rotation combines speed (already in the variable)
    // and direction
    targetRotation *= rotation / rotationSize;

    // Check if we are there, return no steering
    if (rotationSize < targetRadius) {
        targetRotation = 0;
    }

    // Acceleration tries to get to the target rotation
    result.angular = targetRotation - character.angular_vel;
    result.angular /= timeToTarget;

    // Check if the acceleration is too great
    float angularAccel = abs(result.angular);
    if (angularAccel > maxAngularAccel) {
        result.angular /= angularAccel;
        result.angular *= maxAngularAccel;
    }

    result.linear = sf::Vector2f(0, 0);
    return result;
}

// VelMatch implementation

VelMatch::VelMatch(Kinematic character, Kinematic target) {
    this->character = character;
    this->target = target;
}

// Below algorithm taken from class textbook
SteeringData VelMatch::getSteering() {
    SteeringData result;

    // Acceleration tries to get to the target velocity
    result.linear = target.linear_vel - character.linear_vel;
    result.linear /= timeToTarget;

    // Check if the acceleration is too fast
    if (length(result.linear) > maxAccel) {
        result.linear = normalize(result.linear);
        result.linear *= maxAccel;
    }

    result.angular = 0;
    return result;
}

// RotMatch implementation

RotMatch::RotMatch(Kinematic character, Kinematic target) {
    this->character = character;
    this->target = target;
}

// Below algorithm taken from class textbook
SteeringData RotMatch::getSteering() {
    SteeringData result;

    // Get the naive (non-normalized?) direction to the target
    float rotation = target.angular_pos - character.angular_pos;

    // Map the result to the (-pi, pi) interval
    result.angular = mapToRange(rotation);
    result.angular /= timeToTarget;
    float angularAccel = abs(result.angular);

    // Check if the acceleration is too fast
    if (angularAccel> maxAngularAccel) {
        result.angular /= angularAccel;
        result.angular *= maxAngularAccel;
    }

    result.angular = 0;
    return result;
}

// Wander implementation

sf::Vector2f angleAsVector(float angle) {
    return sf::Vector2f(cos(angle), sin(angle));
}

Wander::Wander(Kinematic character) {
    this->character = character;
    std::srand(std::time(nullptr));
}

// Below algorithm taken from the one made in class
SteeringData Wander::getSteering() {
    float wander_orientation = ((float)std::rand()/RAND_MAX - (float)std::rand()/RAND_MAX) * wanderRate;
    float target_orientation = wander_orientation + character.angular_pos;

    // new target to apply things to
    Kinematic target;

    // Find circle center
    target.linear_pos = character.linear_pos + wanderOffset * angleAsVector(character.angular_pos);
    target.linear_pos += wanderRadius * angleAsVector(target_orientation);
    target.angular_pos = target_orientation;

    Align align(character, target);
    Arrive arrive(character, target);
        
    SteeringData alignData = align.getSteering();
    SteeringData arriveData = arrive.getSteering();
    
    return alignData + arriveData;
}

// Seperation implementation

Separation::Separation(Kinematic character, std::vector<KinematicBody*> targets) {
    this->character = character;
    this->targets = targets;
}

// Below algorithm taken from class textbook
SteeringData Separation::getSteering() {
    SteeringData result;

    // Loop through each target
    for (KinematicBody* target : targets) {
        // Check if the target is close
        sf::Vector2f direction = target->linear_pos - character.linear_pos;
        float distance = length(direction);

        if (distance < threshold && distance != 0) {
            // Calculate the strength of repulsion
            // (using the inverse square law)
            float strength = std::min(decayCoefficient / (distance * distance), maxAccel);
            // Add the acceleration
            direction = normalize(direction);
            result.linear += strength * direction;
        }
    }

    result.angular = 0;
    return result;
}

// Flock implementation

Flock::Flock(Kinematic character, std::vector<KinematicBody*> targets) {
    this->character = character;
    this->targets = targets;
}

SteeringData Flock::getSteering() {
    Separation separation(character, targets);
    SteeringData sep = separation.getSteering();
    
    // Find center of mass (given a radius)
    Kinematic center;
    float boidsInRadius = 0;
    for (KinematicBody* target: targets) {
        sf::Vector2f direction = target->linear_pos - character.linear_pos;
        float distance = length(direction);

        if (distance < neighborRadius) {
            boidsInRadius++;
            center.linear_pos += target->linear_pos;
            center.linear_vel += target->linear_vel;
        }
    }
    if (boidsInRadius > 0) {
        center.linear_pos /= boidsInRadius;
        center.linear_vel /= boidsInRadius;
    }

    center.angular_pos = atan2(center.linear_vel.y, center.linear_vel.x);

    VelMatch velMatch(character, center);
    Arrive arrive(character, center);
    Align align(character, center);

    SteeringData velM = velMatch.getSteering();
    SteeringData arr = arrive.getSteering();
    SteeringData ali = align.getSteering();

    // Blending
    SteeringData result = sep * 20 + velM * 10 + arr * 4 + ali;

    // Crop the result and return
    float accelMag = std::min(length(result.linear), maxAccel);
    result.linear = normalize(result.linear) * accelMag;
    if (result.angular > 0) {
        result.angular = std::min(result.angular, maxAngularAccel);
    } else {
        result.angular = std::max(result.angular, -maxAngularAccel);
    }
    
    return result;
}