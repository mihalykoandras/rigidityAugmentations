// Copyright 2021 András Mihálykó MIT License

#include <queue>
#include <utility>
#include <iostream>

#include "graph.h"
#include "M-comp.h"

void M_compHyperGraph::print() const {
    std::cout << "(k, ell) = (" << k <<", " << ell << ")" << std::endl;
    DirectedHyperGraph::printUndirectedHyperedges();
}


void M_compHyperGraph::changeDirection(Node<std::shared_ptr<DirectedHyperEdge> >* edge
/*comes from the list head->isHeadOf*/, std::shared_ptr<Vertex> to) {
    std::shared_ptr<DirectedHyperEdge> newEdge = edge->getData();
    std::shared_ptr<Vertex> from = newEdge->getHead();
    newEdge->setHead(to);

    headOf(to)->push_front(newEdge);
    increaseInDegree(to);

    decreaseInDegree(from);
    headOf(from)->deleteNode(edge);
}


std::unordered_map<int, bool> M_compHyperGraph::getSameComponentVector(std::shared_ptr<Vertex> v) {
    /*
        Running time is O(|V|) by Lemma 3.1
    */
    std::unordered_map<int, bool> c_v;
    for (std::shared_ptr<HyperEdge> undHyperEdge : undirectedHyperEdges) {
        if (undHyperEdge->isStillExistsing() && !isTrivial(undHyperEdge)) {
            // no need for already deleted or trivial M-components
            bool isVIn = false;
            for (std::shared_ptr<Vertex> vertex : undHyperEdge->getVertices()) {
                if (*vertex == *v) {
                    isVIn = true;
                }
            }
            if (isVIn) {
                for (std::shared_ptr<Vertex> vertex : undHyperEdge->getVertices()) {
                    c_v[vertex->getId()] = true;
                }
            }
        }
    }
    return c_v;
}

bool M_compHyperGraph::DFS(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) {
    for (std::shared_ptr<HyperEdge> h : undirectedHyperEdges) {  // O(n)
        setUsedInThisDFS(h, false);
    }
    for (std::pair<const int, std::shared_ptr<Vertex> > & v : vertices) {  // O(n)
        setUsedInThisDFS(v.second, false);
    }
    std::queue<std::shared_ptr<Vertex> > Q;

    Q.push(v1);
    setUsedInThisDFS(v1, true);

    Q.push(v2);
    setUsedInThisDFS(v2, true);

    while (!Q.empty()) {
        std::shared_ptr<Vertex> actualVertex = Q.front();
        Q.pop();
        if (!(*actualVertex == *v1 || *actualVertex == *v2) &&
            getInDegree(actualVertex) < k) {  // actual vertex is correct, so
            // turn around and return
            std::shared_ptr<Vertex> v = actualVertex;
            do {
                Node<std::shared_ptr<DirectedHyperEdge> >* comeFrom = getIncomingHyperedge(actualVertex);
                v = comeFrom->getData()->getHead();
                changeDirection(comeFrom, actualVertex);
                actualVertex = v;
            } while ( !((*actualVertex == *v1) || (*actualVertex == *v2)) );
            return true;
        } else {
            Node<std::shared_ptr<DirectedHyperEdge> >* node = headOf(actualVertex)->getFirst();
            while (node != NULL) {
                std::shared_ptr<DirectedHyperEdge> dirEdge = node->getData();
                if (!isUsedInThisDFS(dirEdge->getHyperEdge())) {
                     // This can be used for transverse back
                    setUsedInThisDFS(dirEdge->getHyperEdge(), true);
                    std::vector<std::shared_ptr<Vertex> > edgeVertices = dirEdge->getHyperEdge()->getVertices();
                    for (std::shared_ptr<Vertex> newVert : edgeVertices) {
                        if (!isUsedInThisDFS(newVert)) {
                            setUsedInThisDFS(newVert, true);
                            setIncomingHyperedge(newVert, node);
                            Q.push(newVert);
                        }
                    }
                }
                node = node->getNext();
            }
        }
    }
    return false;
}

std::vector<std::shared_ptr<Vertex> > M_compHyperGraph::getT(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) {
    std::vector<std::shared_ptr<Vertex> > T(0);
    int maxSumDegree = 2 * k - ell - 1;

    bool hasEdgeToChange;
    do {
        hasEdgeToChange = DFS(v1, v2);
    } while ((getInDegree(v1) + getInDegree(v2) > maxSumDegree) && hasEdgeToChange);

    if (getInDegree(v1) + getInDegree(v2) <= maxSumDegree) {
        return T;
    } else {
        for (std::pair<const int, std::shared_ptr<Vertex> > & v : vertices) {
            if (isUsedInThisDFS(v.second)) {
                T.push_back(v.second);
            }
        }
    }
    return T;
}



void M_compHyperGraph::MakeMCompHypergraph(SimpleGraph& G) {
    if (G.getNumberOfEdges() != 0) {
        SimpleGraph Gprime(G.getNumberOfNodes());  // graph of the already used edges
        for (int i = 0; i < G.getNumberOfNodes(); i++) {
            std::shared_ptr<Vertex> v = getVertex(i);
            std::unordered_map<int, bool> inTheSameM_componentWith_i = getSameComponentVector(v);  // c_i in the paper
            std::vector<int> neighborIds = G.getNeighbors(v->getId());

            for (int neighborId : neighborIds) {
                std::shared_ptr<Vertex> neighbor = getVertex(neighborId);

            if (v < neighbor) {  // not to add two times
                // Gprime is jut a check, it could be deleted for faster run
                Gprime.addEdge(v->getId(), neighbor->getId());

                if (inTheSameM_componentWith_i[neighbor->getId()])
                    continue;  // in this case, no action is needed

                std::vector<std::shared_ptr<Vertex> > T = getT(v, neighbor);

                if (T.empty()) {  // you can add one edge
                    std::shared_ptr<HyperEdge> newHyperEdge =
                        std::make_shared<HyperEdge>(std::vector<std::shared_ptr<Vertex> >({v, neighbor}));
                    setTrivial(newHyperEdge, true);  // this is a new edge, that is trivial
                    SpanningGraph.addEdge(v->getId(), neighbor->getId());
                    if (getInDegree(v) < getInDegree(neighbor)) {
                        addHyperEdge(newHyperEdge, v);
                    } else {
                        addHyperEdge(newHyperEdge, neighbor);
                    }
                    continue;
                } else {
                    std::shared_ptr<HyperEdge> newHyperEdge = std::make_shared<HyperEdge>(T);
                    undirectedHyperEdges.push_back(newHyperEdge);
                    setTrivial(newHyperEdge, false);  // this is not trivial hyperegde
                    for (auto& hyperEdge : directedHyperEdges) {
                        if (isUsedInThisDFS(hyperEdge->getHyperEdge())) {
                            hyperEdge->changeUnderlyingEdge(newHyperEdge);
                        }
                    }
                }
            }
            }
        }
    }
}

