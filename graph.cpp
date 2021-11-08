#include "graph.h"

void SimpleGraph::readFromInput() {
    std::cin >> numberOfVertices;
    int numbOfEdges;
    std::cin >> numbOfEdges;
    for (int i = 0; i < numbOfEdges; i++) {
        int a, b;
        std::cin >> a >> b;
        addEdge(a, b);
    }
}

void DirectedHyperGraph::print() const {
    std::cout << "Number of vertices: " << getNumberOfVertices() << std::endl;
    std::cout << "Number of hyperedges: " << getNumberOfEdges() << std::endl;
    std::cout << "Edges:" << std::endl;
    for (auto const& e : directedHyperEdges) {
        e.print();
    }
}

void DirectedHyperEdge::changeUnderlyingEdge(HyperEdge* newHyperEdge) {
    hyperEdge->setStillExistsing(false);
    newHyperEdge->setStillExistsing(true);
    hyperEdge = newHyperEdge;
}


void DirectedHyperGraph::addHyperEdge(HyperEdge* edge,  Vertex * head) {
    /* 
    Ads undirected hyperedge every time if it encounters for every "inMcomp"
    */
    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.emplace_back(head, undirectedHyperEdges.back());
    head->isHeadOf()->push_front(&directedHyperEdges.back());
    head->increaseInDegree();
}

void DirectedHyperGraph::addDirEdge(Vertex * head, Vertex * tail) {
    /* 
    Ads one directed edge as a hyperedge. No new non-trivial M-component appears by this
    */
    std::vector<Vertex *> vertices = {head, tail};
    undirectedHyperEdges.push_back(new HyperEdge(vertices, true));
    directedHyperEdges.emplace_back(head, undirectedHyperEdges.back());
    head->isHeadOf()->push_front(&directedHyperEdges.back());
    head->increaseInDegree();
}

void DirectedHyperGraph::changeDirection(DirectedHyperEdge& edge, Vertex * to) {
    edge.setHead(to);
    to->increaseInDegree();
}

void DirectedHyperGraph::readFromInput() {
    int m;
    std::cout << "Number of edges" << std::endl;
    std::cin >> m;

    std::cout << "Edges (head first, finish with -1)" << std::endl;
    for (int i = 0; i < m; i++) {
        std::vector<Vertex*> edgeVertices;
        int  h = -1;
        int v;
        std::cin >> v;

        while (v >= 0 && v < getNumberOfVertices()) {
            edgeVertices.push_back(vertices[v]);
            if (h == -1)  // Hack to store head
                h = v;
            std::cin >> v;
        }
        HyperEdge* newHyperEgde = new HyperEdge(edgeVertices);
        addHyperEdge(newHyperEgde, vertices[h]);
    }
}


