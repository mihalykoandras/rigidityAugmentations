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

std::shared_ptr<HyperEdge> M_compHyperGraph::makeNewHyperEdge(
            const std::vector<std::shared_ptr<Vertex> >& vert) {
    int newID = static_cast<int>(undirectedHyperEdges.size());
    std::shared_ptr<HyperEdge> newHyperEdge =
        std::make_shared<HyperEdge>(vert, newID);
    return newHyperEdge;
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


std::vector<bool> M_compHyperGraph::getSameComponentVector(std::shared_ptr<Vertex> v) {
    /*
        Running time is O(|V|) by Lemma 3.2
    */
    std::vector<bool> c_v(getNumberOfVertices(), false);
    for (std::shared_ptr<HyperEdge> undHyperEdge : undirectedHyperEdges) {
        if (undHyperEdge->isStillExistsing() && !isTrivial(undHyperEdge)) {
            // no need for already deleted or trivial M-components
            bool isVIn = false;
            for (std::shared_ptr<Vertex> vertex : undHyperEdge->getVertices()) {
                if (*vertex == *v) {
                    isVIn = true;
                    break;
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

bool M_compHyperGraph::DFS(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
            std::vector<bool>& vertexUsedInDFS, std::vector<bool>& hyperEdgeUsedInDFS) {
    std::queue<std::shared_ptr<Vertex> > Q;

    Q.push(v1);
    vertexUsedInDFS[v1->getId()] = true;

    Q.push(v2);
    vertexUsedInDFS[v2->getId()] = true;

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
                if (!hyperEdgeUsedInDFS[dirEdge->getHyperEdge()->getId()]) {
                     // This can be used for transverse back
                    hyperEdgeUsedInDFS[dirEdge->getHyperEdge()->getId()] = true;
                    std::vector<std::shared_ptr<Vertex> > edgeVertices = dirEdge->getHyperEdge()->getVertices();
                    for (std::shared_ptr<Vertex> newVert : edgeVertices) {
                        if (!vertexUsedInDFS[newVert->getId()]) {
                            vertexUsedInDFS[newVert->getId()] = true;
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


std::vector<std::shared_ptr<Vertex> > M_compHyperGraph::getTHyperEdges(
        std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, std::vector<bool>& hyperedgeUsedInDFS) {
    std::vector<std::shared_ptr<Vertex> > T(0);
    int maxSumDegree = 2 * k - ell - 1;

    std::vector<bool> vertexUsedInDFS;
    bool hasEdgeToChange;
    do {
        vertexUsedInDFS = std::vector<bool>(getNumberOfVertices(), false);
        hyperedgeUsedInDFS = std::vector<bool>(undirectedHyperEdges.size(), false);  // O(n)
        hasEdgeToChange = DFS(v1, v2, vertexUsedInDFS, hyperedgeUsedInDFS);
    } while ((getInDegree(v1) + getInDegree(v2) > maxSumDegree) && hasEdgeToChange);

    if (getInDegree(v1) + getInDegree(v2) <= maxSumDegree) {
        return T;
    } else {
        for (auto& v : vertices) {
            if (vertexUsedInDFS[v->getId()]) {
                T.push_back(v);
            }
        }
    }
    return T;
}


void M_compHyperGraph::MakeMCompHypergraph(SimpleGraph& G) {
    std::cout << "Vertices processed for M-comp hypergraph:" << std::endl;
    if (G.getNumberOfEdges() != 0) {
        SimpleGraph Gprime(G.getNumberOfNodes());  // graph of the already used edges
        int deletedHyperEdgeNumber = 0;
        double rearrangeRatio = 10;  // if more than this is non-existing edges, we delet those
        for (int i = 0; i < G.getNumberOfNodes(); i++) {

            // Progress bar
            std::cout << "[";
            int barWidth = 70;
            float progress = static_cast<float>(i) / G.getNumberOfNodes();
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int((progress + 0.003) * 100.0) << " %\r";
            std::cout.flush();

            //-------------------------------------
            // rearrange undirected hyperedges

            if (deletedHyperEdgeNumber > undirectedHyperEdges.size() * rearrangeRatio) {
                    std::vector<std::shared_ptr<HyperEdge> > newUndirected;
                    std::vector<bool> newTrivial;

                for (auto& hyperedge : undirectedHyperEdges) {
                    if (hyperedge->isStillExistsing()) {
                        newTrivial.push_back(isTrivial(hyperedge));
                        newUndirected.push_back(hyperedge);
                        newUndirected.back()->setId(newUndirected.size()-1);
                    }
                }
                trivial = newTrivial;
                undirectedHyperEdges = newUndirected;
                deletedHyperEdgeNumber = 0;
            }

            std::shared_ptr<Vertex> v = getVertex(i);
            std::vector<bool> inTheSameM_componentWith_i = getSameComponentVector(v);  // c_i in the paper
            std::vector<int> neighborIds = G.getNeighbors(v->getId());

            for (int neighborId : neighborIds) {
                std::shared_ptr<Vertex> neighbor = getVertex(neighborId);

            if (v < neighbor) {  // not to add two times
                // Gprime is jut a check, it could be deleted for faster run
                Gprime.addEdge(v->getId(), neighbor->getId());

                if (inTheSameM_componentWith_i[neighbor->getId()])
                    continue;  // in this case, no action is needed

                std::vector<bool> usedHyperEdge;
                std::vector<std::shared_ptr<Vertex> > T = getTHyperEdges(v, neighbor, usedHyperEdge);

                if (T.empty()) {  // you can add one edge
                    std::shared_ptr<HyperEdge> newHyperEdge =
                        makeNewHyperEdge(std::vector<std::shared_ptr<Vertex> >({v, neighbor}));
                        // this is a new edge, that is trivial
                    SpanningGraph.addEdge(v->getId(), neighbor->getId());
                    if (getInDegree(v) < getInDegree(neighbor)) {
                        addHyperEdge(newHyperEdge, v);
                    } else {
                        addHyperEdge(newHyperEdge, neighbor);
                    }
                    trivial.push_back(true);
                    continue;
                } else {
                    std::shared_ptr<HyperEdge> newHyperEdge = makeNewHyperEdge(T);
                    undirectedHyperEdges.push_back(newHyperEdge);
                    trivial.push_back(false);
                    // non-trivial new edge
                    for (auto& dirHyperEdge : directedHyperEdges) {
                        if (usedHyperEdge[dirHyperEdge->getHyperEdge()->getId()]) {
                            deletedHyperEdgeNumber++;
                            dirHyperEdge->changeUnderlyingEdge(newHyperEdge);
                        }
                    }
                }
            }
            }
        }
    }
}

