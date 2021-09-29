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
    std::cout << "Number of Nodes: " << getNumberOfNodes() << std::endl;
    std::cout << "Edges:" << std::endl;
    for (auto const& e : directedHyperEdges) {
        e.print();
    }
}

void DirectedHyperGraph::addHyperEdge(const HyperEdge& edge,  Vertex * head) {
    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.emplace_back(head, &undirectedHyperEdges.back());
    headOfHyperEdge[head->getId()].push_back(&directedHyperEdges.back());
    head->increaseInDegree();

    for (const auto& v : edge.getVertices()) {
            vertexInHyperEdge[v->getId()].push_back(&undirectedHyperEdges.back());
    }
}

void DirectedHyperGraph::changeDirection(DirectedHyperEdge& edge, Vertex * to) {
    edge.setHead(to);
    headOfHyperEdge[to->getId()].push_back(&edge);
    to->increaseInDegree();
    // from-ból ki kell szedni valahogy.. Ez majd akkor kell, amikor bejárunk, mert tényleg csak ott kell
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

        while (v >= 0 && v < getNumberOfNodes()) {
            edgeVertices.push_back(&vertices[v]);
            if (h == -1)  // Hack to store head
                h = v;
            std::cin >> v;
        }
        HyperEdge newHyperEgde(edgeVertices);
        addHyperEdge(newHyperEgde, &vertices[h]);
    }
}

