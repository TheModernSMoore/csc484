#pragma once
#include <vector> 

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
        CompositeTask();
        // Return on success (true) or failure (false)
        virtual bool run() = 0;
};

class Selector : public CompositeTask {
    public:
        Selector();
        bool run();
};

class Sequence : public CompositeTask {
    public:
        Sequence();
        bool run();
};

class NonDeterministicSelector : public CompositeTask {
    public:
        NonDeterministicSelector();
        bool run();
};

class NonDeterministicSequence : public CompositeTask {
    public:
        NonDeterministicSequence();
        bool run();
};