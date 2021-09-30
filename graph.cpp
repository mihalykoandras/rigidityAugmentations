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

/*void EdgeList::push_front(EdgeList** headRef, DirectedHyperEdge* new_data) {
        EdgeList* new_node = new EdgeList();
        new_node->edge = new_data;
        new_node->next = (*headRef);
        (*headRef) = new_node;
}*/

void EdgeList::push_front(EdgeList** headRef, DirectedHyperEdge* new_data) {
    EdgeList* new_node = new EdgeList();
    new_node->edge = new_data;
    new_node->prev = NULL;
    new_node->next = this;
    if ((*headRef) != NULL)
        (*headRef)->prev = new_node;
    (*headRef) = new_node;
}

void EdgeList::deleteNode(EdgeList* nodeToDelete) {
    if (nodeToDelete == NULL)
        return;

    /* Change next only if node to be
    deleted is NOT the last node */
    if (nodeToDelete->next != NULL)
        nodeToDelete->next->prev = nodeToDelete->prev;

    /* Change prev only if node to be
    deleted is NOT the first node */
    if (nodeToDelete->prev != NULL)
        nodeToDelete->prev->next = nodeToDelete->next;
    return;
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

