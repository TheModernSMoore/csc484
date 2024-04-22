#pragma once
#include <vector> 
#include <SFML/Graphics.hpp>
#include "../Kinematic/kinematic.h"
#include "../Graph/graph.h"

class Task {
    public:
        Task();
        // Return on success (true) or failure (false)
        virtual bool run() = 0;
};

class CompositeTask : public Task {
    protected:
        std::vector<Task*> children;
    public:
        CompositeTask(std::vector<Task*> children);
        // Return on success (true) or failure (false)
        virtual bool run() = 0;
};

class Selector : public CompositeTask {
    public:
        Selector(std::vector<Task*> children);
        bool run();
};

class Sequence : public CompositeTask {
    public:
        Sequence(std::vector<Task*> children);
        bool run();
};

class NonDeterministicSelector : public CompositeTask {
    public:
        NonDeterministicSelector(std::vector<Task*> children);
        bool run();
};

class NonDeterministicSequence : public CompositeTask {
    public:
        NonDeterministicSequence(std::vector<Task*> children);
        bool run();
};

// Actionable Tasks

class CharacterInObjTask : public Task {
    private:
        WorldGraph* world;
        KinematicBody* character;
    public:
        CharacterInObjTask(WorldGraph* world, KinematicBody* character);
        bool run();
};

class BeelineTask : public Task {
    private:
        KinematicBody* character;
        KinematicBody* monster;
        float* deltaTime;
        int* current_edge;
    public:
        BeelineTask(KinematicBody* character, KinematicBody* monster, float* deltaTime, int* current_edge);
        bool run();
};

class WanderTask : public Task {
    private:
        KinematicBody* monster;
        float* deltaTime;
        int* current_edge;
    public:
        WanderTask(KinematicBody* monster, float* deltaTime, int* current_edge);
        bool run();
};

class PathfindTask : public Task {
    private:
        KinematicBody* monster;
        sf::Vector2f position;
        std::vector<Edge*>* path;
        WorldGraph* world;
        int* current_edge;
        float* deltaTime;
    public:
        PathfindTask(KinematicBody* monster, sf::Vector2f position, WorldGraph* world, std::vector<Edge*>* path, int* current_edge, float* deltaTime);
        void setPosition(sf::Vector2f position);
        bool run();
};