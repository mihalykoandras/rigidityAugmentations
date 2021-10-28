#include "graph.h"

void SimpleGraph::readFromInput() {
    std::cin >> numberOfVertices;
    neighborLists = std::vector<std::vector<int> >(numberOfVertices);
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
    for (const auto& v : hyperEdge->getVertices()) {
        // It is still O(|V|^2), as it is at most double the amount of hyperedges can occur
        v->inMcomp()->push_front(newHyperEdge);
    }
}


void DirectedHyperGraph::addHyperEdge(const HyperEdge& edge,  Vertex * head) {
    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.emplace_back(head, &undirectedHyperEdges.back());
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
            edgeVertices.push_back(&vertices[v]);
            if (h == -1)  // Hack to store head
                h = v;
            std::cin >> v;
        }
        HyperEdge newHyperEgde(edgeVertices);
        addHyperEdge(newHyperEgde, &vertices[h]);
    }
}


