#include "decisionNode.h"

// DecisionTreeNode

DecisionTreeNode::DecisionTreeNode() {}

// Decision Start ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Decision::Decision(DecisionTreeNode* trueNode, DecisionTreeNode* falseNode) {
    this->trueNode = trueNode;
    this->falseNode = falseNode;
}

DecisionTreeNode* Decision::makeDecision() {
    // Make the decision and recurse based on the result
    DecisionTreeNode* branch = getBranch();
    return branch->makeDecision();
}

// Wall Distance Decision

NextToWallDecision::NextToWallDecision(DecisionTreeNode* trueNode, DecisionTreeNode* falseNode, WorldGraph* world, KinematicBody* character) : Decision(trueNode, falseNode) {
    this->world = world;
    this->character = character;
}

DecisionTreeNode* NextToWallDecision::getBranch() {
    WorldVertex* characterVert = world->quantizePosition(character->getPosition());
    std::vector<WorldVertex*> aroundVerts = world->getWorldVerticesAround(characterVert);
    if (aroundVerts.size() == 4) {
        for (WorldVertex* vert : aroundVerts) {
            if (!vert->getValid()) {
                return trueNode;
            }
        }
        return falseNode;
    }
    return trueNode;
}

// Inside Object Decision

InsideObjectDecision::InsideObjectDecision(DecisionTreeNode* trueNode, DecisionTreeNode* falseNode, WorldGraph* world, KinematicBody* character) : Decision(trueNode, falseNode) {
    this->world = world;
    this->character = character;
}

DecisionTreeNode* InsideObjectDecision::getBranch() {
    WorldVertex* characterVert = world->quantizePosition(character->getPosition());
    return characterVert->getValid() ? falseNode : trueNode;
}

// Random Decision

RandomDecision::RandomDecision(DecisionTreeNode* trueNode, DecisionTreeNode* falseNode) : Decision(trueNode, falseNode) {}

DecisionTreeNode* RandomDecision::getBranch() {
    std::srand(std::time(nullptr));
    float percent = (float)std::rand()/RAND_MAX;
    return percent > 0.95 ? trueNode : falseNode;
}

// Decision end ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Action start ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Action::Action() {}

DecisionTreeNode* Action::makeDecision() {
    return this;
}

DashAction::DashAction() {}

WanderAction::WanderAction() {}

PathfindAction::PathfindAction(sf::Vector2f position) {
    this->position = position;
}