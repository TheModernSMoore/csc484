#pragma once
#include "../Graph/graph.h"
#include "../Kinematic/kinematic.h"
#include "SFML/Graphics.hpp"
// Implementation was guided with the class textbook

class DecisionTreeNode {
    public:
        DecisionTreeNode();
        // Recursively walk through the tree.
        virtual DecisionTreeNode* makeDecision() = 0;  
};

// Decision Start ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Decision : public DecisionTreeNode {
    protected:
        DecisionTreeNode* trueNode;
        DecisionTreeNode* falseNode;
    
    public:
        Decision(DecisionTreeNode* trueNode, DecisionTreeNode* falseNode);
        // Perform the test.
        virtual DecisionTreeNode* getBranch() = 0;
        // Recursively walk through the tree.
        DecisionTreeNode* makeDecision();
};

class NextToWallDecision : public Decision {
    private:
        WorldGraph* world;
        KinematicBody* character;
    
    public:
        NextToWallDecision(DecisionTreeNode* trueNode, DecisionTreeNode* falseNode, WorldGraph* world, KinematicBody* character);
        // Perform the test.
        DecisionTreeNode* getBranch();
};

class InsideObjectDecision : public Decision {
    private:
        WorldGraph* world;
        KinematicBody* character;
    
    public:
        InsideObjectDecision(DecisionTreeNode* trueNode, DecisionTreeNode* falseNode, WorldGraph* world, KinematicBody* character);
        // Perform the test.
        DecisionTreeNode* getBranch();
};

class RandomDecision : public Decision {
    public:
        RandomDecision(DecisionTreeNode* trueNode, DecisionTreeNode* falseNode);
        // Perform the test.
        DecisionTreeNode* getBranch();
};


// Action Start ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Action : public DecisionTreeNode {
    public:
        Action();
        DecisionTreeNode* makeDecision();
};

class DashAction : public Action {
    public:
        DashAction();
};

class WanderAction : public Action {
    public:
        WanderAction();
};

class PathfindAction : public Action {
    public:
        sf::Vector2f position;
        PathfindAction(sf::Vector2f position);
};