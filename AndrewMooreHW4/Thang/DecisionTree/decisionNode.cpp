#include "decisionNode.h"

// DecisionTreeNode

DecisionTreeNode::DecisionTreeNode() {};

// Action

Action::Action() {};

DecisionTreeNode* Action::makeDecision() {
    return this;
}

// Decision

Decision::Decision() {};

DecisionTreeNode* Decision::makeDecision() {
    // Make the decision and recurse based on the result
    DecisionTreeNode* branch = getBranch();
    return branch->makeDecision();
}