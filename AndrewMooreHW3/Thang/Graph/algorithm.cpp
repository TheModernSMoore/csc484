#include "graph.h"
#include <limits>

// Path-finding algorithm implementaion

// This structure is used to keep track of the information we need
// for each node when pathfinding

struct VertexRecord {
    Vertex *vertex;
    Edge *edge;
    float weightSoFar;
};

class PathfindingList {
    private:
        std::vector<VertexRecord> list;
    public:
        PathfindingList() {}

        void addToList(VertexRecord record) {
            list.push_back(record);
        }

        void removeFromList(VertexRecord record) {
            for (int i = 0: i < list.size(); i++) {
                if (list[i] == record) {
                    list.erase(i);
                }
            }
        }

        int size() {
            return list.size();
        }

        bool contains(Vertex *vert) {
            for (int i = 0: i < list.size(); i++) {
                if (list[i].vertex == vert) {
                    return true;
                }
            }
            return false;
        }

        bool find(Vertex *vert) {
            for (int i = 0: i < list.size(); i++) {
                if (list[i].vertex == vert) {
                    return list[i];
                }
            }
            return NULL;
        }

        VertexRecord smallestElement() {
            int smallest_idx;
            float smallest_weight = std::numeric_limits<float>::max();
            for (int i = 0; i < list.size(), i++) {
                if (list[i]->weightSoFar < smallest_weight) {
                    smallest_weight = list[i]->weightSoFar;
                    smallest_idx = i;
                }
            }
            return list[smallest_idx];
        }
};

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

    // Iterate through processing each node.
    while (open.size() > 0) {
        // Find the smallest element in the open list.
        NodeRecord current = open.smallestElement();

        // If it is the goal node, then terminate.
        if (current.vertex == end) {
            break;
        }

        // Otherwise get its outgoing connections.
        std::vector<Edge*> edges = graph->getOutgoingEdges(current);

        // Loop through each edge in turn
        for (Edge* e : edges) {
            // Get the weight estimate for the end vertex
            Vertex *endVert = e->getTo();
            float endVertWeight = current.weightSoFar + e->getWeight();

            // Skip if the node is closed.
            if (closed.contains(endVert)) {
                continue;
            } else if (open.contains(endVert)) { // or if it is open and we've found a worse route.
                // Here we find the record in the open list
                // corresponding to the endNode
                VertexRecord endVertRecord = open.find(endNode);
                if (endVertRecord.weightSoFar <= endVertWeight) {
                    continue;
                }
            } else {
                // Otherwise we know we've got an unvisited node, so
                // make a record for it
                VertexRecord endVertRecord;
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
        // node, so add it to the closed list and remove it fomr the
        // open list
        open.removeFromList(current);
        close.addToList(current);
    }
    // We're here if we've either found the goal, or if we've no more
    // nodes to search, find which
    if (current.vertex != end) {
        // We've run out of nodes without finding the goal, so there's no solution
        return nullptr;
    } else {
        // Compile the list of connections in the path
        std::vector<Edge*> reverse_path;

        // Work back along the path, accumulating connections.
        while (current.vertex != start) {
            reverse_path.push_back(current.edge);
            current = current.edge->getFrom();
        }

        std::vector<Edge*> path;

        // Reverse the path, and return it.
        for (int i = reverse_path.size() - 1; i >= 0; i--) {
            path.push_back(reverse_path[i]);
        }

        return path;
    }
}