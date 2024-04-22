#include "behavior.h"
#include <algorithm> 
#include <iostream> 
#include <ctime> 
#include <cstdlib>
#include "../SteeringBehavior/steeringBehavior.h"
#include "../Graph/algorithm.h"

// Task

Task::Task() {};

// CompositeTask

CompositeTask::CompositeTask(std::vector<Task*> children) {
    this->children = children;
}

// Selector

Selector::Selector(std::vector<Task*> children) : CompositeTask(children) {}

bool Selector::run() {
    for (Task* c : children) {
        if (c->run()) {
            return true;
        }
    }
    return false;
}

// Sequence

Sequence::Sequence(std::vector<Task*> children) : CompositeTask(children) {}

bool Sequence::run() {
    for (Task* c : children) {
        if (!c->run()) {
            return false;
        }
    }
    return true;
}

// NonDeterministicSelector

NonDeterministicSelector::NonDeterministicSelector(std::vector<Task*> children) : CompositeTask(children) {}

bool NonDeterministicSelector::run() {
    std::srand(std::time(nullptr));
    std::random_shuffle(children.begin(), children.end());
    for (Task* c : children) {
        if (c->run()) {
            return true;
        }
    }
    return false;
}

// NonDeterministicSequence

NonDeterministicSequence::NonDeterministicSequence(std::vector<Task*> children) : CompositeTask(children) {}

bool NonDeterministicSequence::run() {
    std::srand(std::time(nullptr));
    std::random_shuffle(children.begin(), children.end());
    for (Task* c : children) {
        if (!c->run()) {
            return false;
        }
    }
    return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Actionable Tasks

// Character In Object Condition Task

CharacterInObjTask::CharacterInObjTask(WorldGraph* world, KinematicBody* character) {
    this->world = world;
    this->character = character;
}

bool CharacterInObjTask::run() {
    WorldVertex* characterVert = world->quantizePosition(character->getPosition());
    return !characterVert->getValid();
}

// Beeline Task

BeelineTask::BeelineTask(KinematicBody* character, KinematicBody* monster, float* deltaTime, int* current_edge) {
    this->character = character;
    this->monster = monster;
    this->deltaTime = deltaTime;
    this->current_edge = current_edge;
}

bool BeelineTask::run() {
    Arrive arrive(*monster, *character);
    monster->update(arrive.getSteering(), *deltaTime);
    *current_edge = 1000;
    return true;
}

// Wander Task

WanderTask::WanderTask(KinematicBody* monster, float* deltaTime, int* current_edge) {
    this->monster = monster;
    this->deltaTime = deltaTime;
    this->current_edge = current_edge;
}

bool WanderTask::run() {
    Wander wander(*monster);
    monster->update(wander.getSteering(), *deltaTime);
    *current_edge = 1000;
    return true;
}

// Pathfinding Task

PathfindTask::PathfindTask(KinematicBody* monster, sf::Vector2f position, WorldGraph* world, std::vector<Edge*>* path, int* current_edge, float* deltaTime) {
    this->monster = monster;
    this->position = position;
    this->path = path;
    this->current_edge = current_edge;
    this->deltaTime = deltaTime;
    this->world = world;
}

void PathfindTask::setPosition(sf::Vector2f position) {
    this->position = position;
}

bool PathfindTask::run() {
    // This will merely just follow the current path or, if no path exists
    // find a new path and start following that
    if (*current_edge < path->size()) {
        Edge* curr_edge = (*path)[*current_edge];
        WorldVertex* curr_vert = (WorldVertex*)curr_edge->getTo();
        Kinematic path_kinematic;
        path_kinematic.linear_pos = curr_vert->localizePosition();
        path_kinematic.angular_pos = atan2(path_kinematic.linear_pos.y - monster->linear_pos.y, path_kinematic.linear_pos.x - monster->linear_pos.x);
        Arrive arrive(*monster, path_kinematic);
        Align align(*monster, path_kinematic);

        SteeringData pathSteering;
        pathSteering = arrive.getSteering();
        pathSteering += align.getSteering();

        monster->update(pathSteering, *deltaTime);

        if (curr_vert == world->quantizePosition(monster->getPosition())) {
            (*current_edge)++;
        }
    } else {
        // First get the position in the action.
        WorldVertex* goal = world->quantizePosition(position);
        WorldVertex* start = world->quantizePosition(monster->getPosition());
        Heuristic* heuristic = new EuclideanHeuristic(goal);
        // uhhhhhhhh does below break??????
        *path = pathfindAStar(world, start, goal, heuristic);
        *current_edge = 0;
    }
    return true;
}