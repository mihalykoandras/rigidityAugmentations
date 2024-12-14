// Copyright 2021 András Mihálykó MIT License

#include "graph.h"

#include <ogdf/basic/Graph.h>
#include <ogdf/decomposition/DynamicSPQRTree.h>

#include <iostream>

void SimpleGraph::readFromInput() {
    std::cin >> numberOfVertices;
    int numbOfEdges;
    std::cin >> numbOfEdges;
    for (int i = 0; i < numbOfEdges; i++) {
        int a, b;
        std::cin >> a >> b;
        if (a == b) {
            std::cerr << "No loop edges are allowed." << std::endl;
            throw 12;
        }
        addEdge(a, b);
    }
}

bool SimpleGraph::is3Connected() const {
    // Convert SimpleGraph to ogdf::Graph
    ogdf::Graph ogdfGraph;
    for (size_t v = 0; v < getNumberOfNodes(); v++) {
        ogdfGraph.newNode();
    }
    // Create nodes
    std::vector<ogdf::node> nodes;
    for (int i = 0; i < 4; ++i) {
        nodes.push_back(ogdfGraph.newNode());
    }

    // Add edges
    for (const auto& edge : getEdges()) {
        ogdfGraph.newEdge(nodes[edge.getEdge()[0]], nodes[edge.getEdge()[1]]);
    }

    // Create a SPQR tree decomposition
    ogdf::DynamicSPQRTree spqr(ogdfGraph);

    // Check if the graph is 3-connected
    return spqr.spqrTree().numberOfNodes() == 1;
}

void DirectedHyperGraph::print() const {
    std::cout << "Number of vertices: " << getNumberOfVertices() << std::endl;
    std::cout << "Number of hyperedges: " << getNumberOfEdges() << std::endl;
    std::cout << "Edges:" << std::endl;
    for (auto const& e : directedHyperEdges) {
        e->print();
    }
}

void DirectedHyperGraph::printUndirectedHyperedges() const {
    std::cout << "Number of vertices: " << getNumberOfVertices() << std::endl;
    std::cout << "Number of hyperedges: " << getNumberOfEdges() << std::endl;
    std::cout << "Undirected Hyper Edges:" << std::endl;
    for (auto const& e : undirectedHyperEdges) {
        if (e->isStillExistsing())
            e->print();
    }
}

void DirectedHyperEdge::changeUnderlyingEdge(std::shared_ptr<HyperEdge> newHyperEdge) {
    hyperEdge->deleteHyperedge();
    hyperEdge = newHyperEdge;
}

std::shared_ptr<Vertex> DirectedHyperGraph::getVertex(int id) const {
    if (vertices.size() > id) {
        return vertices[id];
    }
    throw std::runtime_error("No vertex found with index " + std::to_string(id));
    return std::shared_ptr<Vertex>();
}

int DirectedHyperGraph::getInDegree(int id) const {
    if (inDegree.size() > id) {
        return inDegree[id];
    }
    throw std::runtime_error("No vertex found with index " + std::to_string(id));
    return -1;
}

void DirectedHyperGraph::addHyperEdge(std::shared_ptr<HyperEdge> edge,
                                      const std::shared_ptr<Vertex>& head) {
    /*
    Adds undirected hyperedge every time if it encounters for every "inMcomp"
    */
    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.push_back(std::make_shared<DirectedHyperEdge>(head, edge));
    headOf(head)->push_front(directedHyperEdges.back());
    increaseInDegree(head);
}

void DirectedHyperGraph::addDirEdge(const std::shared_ptr<Vertex>& head,
                                    const std::shared_ptr<Vertex>& tail) {
    /*
    Adds one directed edge as a hyperedge. No new non-trivial M-component
    appears by this
    */
    std::vector<std::shared_ptr<Vertex>> vertices = {head, tail};
    std::shared_ptr<HyperEdge> edge =
        std::make_shared<HyperEdge>(vertices, undirectedHyperEdges.size());
    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.push_back(std::make_shared<DirectedHyperEdge>(head, edge));
    headOf(head)->push_front(directedHyperEdges.back());
    increaseInDegree(head);
}

void DirectedHyperGraph::changeDirection(DirectedHyperEdge& edge,
                                         const std::shared_ptr<Vertex>& to) {
    edge.setHead(to);
    increaseInDegree(to);
}

void DirectedHyperGraph::readFromInput() {
    int m;
    std::cout << "Number of edges" << std::endl;
    std::cin >> m;

    std::cout << "Edges (head first, finish with -1)" << std::endl;
    for (int i = 0; i < m; i++) {
        std::vector<std::shared_ptr<Vertex>> edgeVertices;
        int h = -1;
        int v;
        std::cin >> v;

        while (v >= 0 && v < getNumberOfVertices()) {
            edgeVertices.push_back(vertices[v]);
            if (h == -1)  // Hack to store head
                h = v;
            std::cin >> v;
        }
        std::shared_ptr<HyperEdge> newHyperEgde =
            std::make_shared<HyperEdge>(edgeVertices, undirectedHyperEdges.size());
        addHyperEdge(newHyperEgde, vertices[h]);
    }
}
