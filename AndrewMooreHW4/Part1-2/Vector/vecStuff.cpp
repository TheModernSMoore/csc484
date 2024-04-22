#include "vecStuff.h"
#include <cmath>

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