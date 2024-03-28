#pragma once
#include "graph.h"

// This structure is used to keep track of the information we need
// for each node when pathfinding

struct VertexRecord {
    Vertex *vertex;
    Edge *edge;
    float weightSoFar;
    bool operator==(VertexRecord other);
};

class PathfindingList {
    private:
        std::vector<VertexRecord> list;
    public:
        PathfindingList();

        void addToList(VertexRecord record);

        void removeFromList(VertexRecord record);

        int size();

        bool contains(Vertex *vert);

        VertexRecord find(Vertex *vert);

        VertexRecord smallestElement();
};

std::vector<Edge*> pathfindDijkstra(Graph *graph, Vertex *start, Vertex *end);


// A* structures

struct VertexRecordStar {
    Vertex *vertex;
    Edge *edge;
    float weightSoFar;
    float estimatedTotalWeight;
    bool operator==(VertexRecordStar other);
};

class PathfindingListStar {
    private:
        std::vector<VertexRecordStar> list;
    public:
        PathfindingListStar();

        void addToList(VertexRecordStar record);

        void removeFromList(VertexRecordStar record);

        int size();

        bool contains(Vertex *vert);

        VertexRecordStar find(Vertex *vert);

        VertexRecordStar smallestElement();
};

class Heuristic {
    private:
        // Stores the goal node that this heuristic is estimating for
        Vertex *goalVert;
    public:
        Heuristic(Vertex *goal);
        // Estimated cost to reach the stored goal from the given node
        float estimate(Vertex *fromVert);
        // Any two nodes
        virtual float estimate(Vertex *fromVert, Vertex *toVert) = 0;
};

class EuclideanHeuristic : public Heuristic {
    public:
        EuclideanHeuristic(Vertex *goal);
        virtual float estimate(Vertex *fromVert, Vertex *toVert);
};

class ManhattanHeuristic : public Heuristic {
    public:
        ManhattanHeuristic(Vertex *goal);
        virtual float estimate(Vertex *fromVert, Vertex *toVert);
};

std::vector<Edge*> pathfindAStar(Graph *graph, Vertex *start, Vertex *end, Heuristic *heuristic);