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



template<typename T> void List<T>::push_front(T new_data) {
    Node<T>* new_node = new Node<T>();
    new_node->setData(new_data);
    new_node->setPrevious(NULL);
    new_node->setNext(first);
    if (first != NULL)
        first->setPrevious(new_node);
    first = new_node;
    numbOfData++;
}

template<typename T> void List<T>::deleteNode(Node<T>* nodeToDelete) {
    if (nodeToDelete == NULL)
        return;
    if (first == nodeToDelete) {
        first = nodeToDelete->getNext();
    }

    if (nodeToDelete->getNext() != NULL)
        nodeToDelete->getNext()->setPrevious(nodeToDelete->getPrevious());

    /* Change prev only if node to be
    deleted is NOT the first node */
    if (nodeToDelete->getPrevious() != NULL)
        nodeToDelete->getPrevious()->setNext(nodeToDelete->getNext());

    delete nodeToDelete;
    numbOfData--;
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

template class List<DirectedHyperEdge*>;
template class Node<DirectedHyperEdge*>;
template class List<HyperEdge*>;
template class Node<HyperEdge*>;
