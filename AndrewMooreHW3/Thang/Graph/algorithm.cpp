#include "algorithm.h"
#include <limits>
#include <iostream>

// Path-finding algorithm implementaion

bool VertexRecord::operator==(VertexRecord other) {
    if (vertex == other.vertex &&
        edge == other.edge &&
        weightSoFar == other.weightSoFar) {
        return true;
    }
    return false;
}

PathfindingList::PathfindingList() {}

void PathfindingList::addToList(VertexRecord record) {
    list.push_back(record);
}

void PathfindingList::removeFromList(VertexRecord record) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == record) {
            list.erase(list.begin() + i);
        }
    }
}

int PathfindingList::size() {
    return list.size();
}

bool PathfindingList::contains(Vertex *vert) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i].vertex == vert) {
            return true;
        }
    }
    return false;
}

VertexRecord PathfindingList::find(Vertex *vert) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i].vertex == vert) {
            return list[i];
        }
    }
    return VertexRecord();
}

VertexRecord PathfindingList::smallestElement() {
    int smallest_idx;
    float smallest_weight = std::numeric_limits<float>::max();
    for (int i = 0; i < list.size(); i++) {
        if (list[i].weightSoFar < smallest_weight) {
            smallest_weight = list[i].weightSoFar;
            smallest_idx = i;
        }
    }
    return list[smallest_idx];
}

// This algorithm is taken from the class textbook
std::vector<Edge*> pathfindDijkstra(Graph *graph, Vertex *start, Vertex *end) {
    VertexRecord startRecord;
    startRecord.vertex = start;
    startRecord.edge = nullptr;
    startRecord.weightSoFar = 0;

    // Initialize the open and closed lists.
    PathfindingList open;
    open.addToList(startRecord);
    PathfindingList closed;

    VertexRecord current;

    // Iterate through processing each node.
    while (open.size() > 0) {
        // Find the smallest element in the open list.
        current = open.smallestElement();

        // If it is the goal node, then terminate.
        if (current.vertex == end) {
            break;
        }

        // Otherwise get its outgoing connections.
        std::vector<Edge*> edges = graph->getOutgoingEdges(current.vertex);

        // Loop through each edge in turn
        for (Edge* e : edges) {
            // Get the weight estimate for the end vertex
            Vertex *endVert = e->getTo();
            float endVertWeight = current.weightSoFar + e->getWeight();
            
            VertexRecord endVertRecord;
            // Skip if the node is closed.
            if (closed.contains(endVert)) {
                continue;
            } else if (open.contains(endVert)) { // or if it is open and we've found a worse route.
                // Here we find the record in the open list
                // corresponding to the endNode
                endVertRecord = open.find(endVert);
                if (endVertRecord.weightSoFar <= endVertWeight) {
                    continue;
                }
            } else {
                // Otherwise we know we've got an unvisited node, so
                // make a record for it
                endVertRecord.vertex = endVert;
            }

            // We're here if we need to update the node.
            // Update the cost and connection.
            endVertRecord.weightSoFar = endVertWeight;
            endVertRecord.edge = e;

            // And add it to the open list.
            if (!open.contains(endVert)) {
                open.addToList(endVertRecord);
            }
        }

        // We've finished looking at the connections for the current
        // node, so add it to the closed list and remove it from the
        // open list
        open.removeFromList(current);
        closed.addToList(current);
    }
    // We're here if we've either found the goal, or if we've no more
    // nodes to search, find which
    if (current.vertex != end) {
        // We've run out of nodes without finding the goal, so there's no solution
        return std::vector<Edge*>();
    } else {
        // Compile the list of connections in the path
        std::vector<Edge*> reverse_path;

        // Work back along the path, accumulating connections.
        while (current.vertex != start) {
            reverse_path.push_back(current.edge);
            current = closed.find(current.edge->getFrom());
        }

        std::vector<Edge*> path;

        // Reverse the path, and return it.
        for (int i = reverse_path.size() - 1; i >= 0; i--) {
            path.push_back(reverse_path[i]);
        }
        return path;
    }
}

// A* structure and algorithm implementations

// Base Heuristic implementation

Heuristic::Heuristic(Vertex *goal) {
    goalVert = goal;
}

float Heuristic::estimate(Vertex *fromVert) {
    return estimate(fromVert, goalVert);
}

bool VertexRecordStar::operator==(VertexRecordStar other) {
    if (vertex == other.vertex &&
        edge == other.edge &&
        weightSoFar == other.weightSoFar &&
        estimatedTotalWeight == other.estimatedTotalWeight) {
        return true;
    }
    return false;
}

PathfindingListStar::PathfindingListStar() {}

void PathfindingListStar::addToList(VertexRecordStar record) {
    list.push_back(record);
}

void PathfindingListStar::removeFromList(VertexRecordStar record) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == record) {
            list.erase(list.begin() + i);
        }
    }
}

int PathfindingListStar::size() {
    return list.size();
}

bool PathfindingListStar::contains(Vertex *vert) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i].vertex == vert) {
            return true;
        }
    }
    return false;
}

VertexRecordStar PathfindingListStar::find(Vertex *vert) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i].vertex == vert) {
            return list[i];
        }
    }
    return VertexRecordStar();
}

// Looks at total estimated weight instead of weight so far
VertexRecordStar PathfindingListStar::smallestElement() {
    int smallest_idx;
    float smallest_weight = std::numeric_limits<float>::max();
    for (int i = 0; i < list.size(); i++) {
        if (list[i].estimatedTotalWeight < smallest_weight) {
            smallest_weight = list[i].estimatedTotalWeight;
            smallest_idx = i;
        }
    }
    return list[smallest_idx];
}

// Algorithm implementation

std::vector<Edge*> pathfindAStar(Graph *graph, Vertex *start, Vertex *end, Heuristic *heuristic) {
    // Initialize the record for the start vertex.
    VertexRecordStar startRecord;
    startRecord.vertex = start;
    startRecord.edge = nullptr;
    startRecord.weightSoFar = 0;
    startRecord.estimatedTotalWeight = heuristic->estimate(start);

    // Initialize the open and close lists.
    PathfindingListStar open;
    open.addToList(startRecord);
    PathfindingListStar closed;

    VertexRecordStar current;

    // Iterate through processing each node.
    while (open.size() > 0) {
        // Find the smallest element in the open list
        // (using the estimatedTotalWeight)
        current = open.smallestElement();

        // If it is the goal node, then terminate
        if (current.vertex == end) {
            break;
        }

        // Otherwise get its outgoing connections
        std::vector<Edge*> edges = graph->getOutgoingEdges(current.vertex);

        // Loop through each connection in turn.
        for (Edge* edge : edges) {
            // Get the cost estimate for the end vertex.
            Vertex *endVert = edge->getTo();
            float endVertWeight = current.weightSoFar + edge->getWeight();

            VertexRecordStar endVertRecord;
            float endVertHeuristic;

            // If the vert is closed we may have to skip, or remove it
            // from the closed list.
            if (closed.contains(endVert)) {
                // Here we find the record in the closed list
                // corresponding to the endVert
                endVertRecord = closed.find(endVert);

                // If we didn't find a shorter route, skip
                if (endVertRecord.weightSoFar <= endVertWeight) {
                    continue;
                }

                // Otherwise remove it form the closed list
                closed.removeFromList(endVertRecord);

                // We can use the node's old weight values to calculate
                // its heuristic without calling the possibly expensive
                // heuristic function
                endVertHeuristic = endVertRecord.estimatedTotalWeight - endVertRecord.weightSoFar;
            } else if (open.contains(endVert)) {
                // Skip if the node is open and we've not found a better route

                // Here we find the record in the open list 
                // corresponding to the endVert
                endVertRecord = open.find(endVert);

                // If our route is no better, then skip
                if (endVertRecord.weightSoFar <= endVertWeight) {
                    continue;
                }

                // Again, we can calculate its heuristic
                endVertHeuristic = endVertRecord.estimatedTotalWeight - endVertRecord.weightSoFar;
            } else {
                // Otherwise we know we've got an unvisited vert, so make
                // a record for it
                endVertRecord.vertex = endVert;

                // We'll need to calculate the heurisitc value using
                // the function, since we don't have an existing record
                // to use
                endVertHeuristic = heuristic->estimate(endVert);
            }

            // We're here if we need to update the vert.  Update the weight,
            // estimate and edge
            endVertRecord.weightSoFar = endVertWeight;
            endVertRecord.edge = edge;
            endVertRecord.estimatedTotalWeight = endVertWeight + endVertHeuristic;


            // And add it to the open list
            if (!open.contains(endVert)) {
                open.addToList(endVertRecord);
            }
        }

        // We've finished looking at the connections for the current vert
        // so add it to the closed list and remove it fomr the open list
        open.removeFromList(current);
        closed.addToList(current);
    }
    // We're here if we've either found the goal, or if we've no more
    // vertices to search, find out which
    if (current.vertex != end) {
        // We've run out of nodes without finding the goal, so there's
        // no solution
        return std::vector<Edge*>();
    } else {
        // Compile the list of edges in the path
        std::vector<Edge*> reverse_path;

        // Work back along the path, accumulating edges.
        while (current.vertex != start) {
            reverse_path.push_back(current.edge);
            current = closed.find(current.edge->getFrom());
        }

        std::vector<Edge*> path;

        // Reverse the path, and return it.
        for (int i = reverse_path.size() - 1; i >= 0; i--) {
            path.push_back(reverse_path[i]);
        }

        return path;
    }
}