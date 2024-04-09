#pragma once

// Implementation was guided with the class textbook

class DecisionTreeNode {
    public:
        DecisionTreeNode();
        // Recursively walk through the tree.
        virtual DecisionTreeNode* makeDecision() = 0;  
};

class Action : public DecisionTreeNode {
    public:
        Action();
        DecisionTreeNode* makeDecision();
};

class Decision : public DecisionTreeNode {
    private:
        DecisionTreeNode* trueNode;
        DecisionTreeNode* falseNode;
    
    public:
        Decision();
        // Defined in subclasses with the appropriate type.
        virtual auto testValue() = 0;
        // Perform the test.
        virtual DecisionTreeNode* getBranch() = 0;
        // Recursively walk through the tree.
        DecisionTreeNode* makeDecision() = 0;
}