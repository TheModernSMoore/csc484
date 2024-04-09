#include "behavior.h"
#include <algorithm> 
#include <iostream> 
#include <ctime> 
#include <cstdlib>

// Task

Task::Task() {};

// CompositeTask

CompositeTask::CompositeTask() {};

// Selector

Selector::Selector() {};

bool Selector::run() {
    for (Task* c : children) {
        if (c->run()) {
            return true;
        }
    }
    return false;
}

// Sequence

Sequence::Sequence() {};

bool Sequence::run() {
    for (Task* c : children) {
        if (!c->run()) {
            return false;
        }
    }
    return true;
}

// NonDeterministicSelector

NonDeterministicSelector::NonDeterministicSelector() {};

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

NonDeterministicSequence::NonDeterministicSequence() {};

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