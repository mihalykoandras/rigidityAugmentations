#include <queue>
#include <utility>


#include "graph.h"
#include "M-comp.h"

void M_compHyperGraph::print() const {
    std::cout << "(k, ell) = (" << k <<", " << ell << ")" << std::endl;
    DirectedHyperGraph::print();
}


void M_compHyperGraph::changeDirection(Node<DirectedHyperEdge*> edge
/*comes from the list head->isHeadOf*/, Vertex * to) {
    DirectedHyperEdge* newEdge = edge.getData();
    Vertex * from = newEdge->getHead();
    newEdge->setHead(to);

    to->isHeadOf()->push_front(newEdge);
    to->increaseInDegree();

    from->decreaseInDegree();
    from->isHeadOf()->deleteNode(&edge);
}


std::map<int, bool> M_compHyperGraph::getSameComponentVector(Vertex * v) {
    /*
        Running time is O(|V|) by Lemma 3.1
    */
    std::map<int, bool> c_v;
    for (HyperEdge* undHyperEdge : undirectedHyperEdges) {
        if (undHyperEdge->isStillExistsing() && !undHyperEdge->isTrivial()) {
            // no need for already deleted or trivial M-components
            bool isVIn = false;
            for (Vertex* vertex : undHyperEdge->getVertices()) {
                if (*vertex == *v) {
                    isVIn = true;
                }
            }
            if (isVIn) {
                for (Vertex* vertex : undHyperEdge->getVertices()) {
                    c_v[vertex->getId()] = true;
                }
            }
        }
    }
    return c_v;
}

bool M_compHyperGraph::DFS(Vertex * v1, Vertex * v2) {
    for (HyperEdge* h : undirectedHyperEdges) {  // O(n)
        h->setUsedInThisDFS(false);
    }
    for (std::pair<const int, Vertex* > & v : vertices) {  // O(n)
        setUsedInThisDFS(v.second, false);
    }
    std::queue<Vertex *> Q;

    Q.push(v1);
    setUsedInThisDFS(v1, true);

    Q.push(v2);
    setUsedInThisDFS(v2, true);

    while (!Q.empty()) {
        Vertex * actualVertex = Q.front();
        Q.pop();
        if (!(*actualVertex == *v1 || *actualVertex == *v2) &&
            actualVertex->getInDegree() < k) {  // actual vertex is correct, so
            // turn around and return
            Vertex * v = actualVertex;
            do {
                Node<DirectedHyperEdge*> comeFrom = getIncomingHyperedge(actualVertex);
                v = comeFrom.getData()->getHead();
                changeDirection(comeFrom, actualVertex);
                actualVertex = v;
            } while ( !((*actualVertex == *v1) || (*actualVertex == *v2)) );
            return true;
        } else {
            Node<DirectedHyperEdge*>* node = actualVertex->isHeadOf()->getFirst();
            while (node != NULL) {
                DirectedHyperEdge* dirEdge = node->getData();
                if (!dirEdge->getHyperEdge()->isUsedInThisDFS()) {
                     // This can be used for transverse back
                    dirEdge->getHyperEdge()->setUsedInThisDFS(true);
                    std::vector<Vertex *> edgeVertices = dirEdge->getHyperEdge()->getVertices();
                    for (Vertex* newVert : edgeVertices) {
                        if (!isUsedInThisDFS(newVert)) {
                            setUsedInThisDFS(newVert, true);
                            setIncomingHyperedge(newVert, *node);
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

std::vector<Vertex *> M_compHyperGraph::getT(Vertex * v1, Vertex * v2) {
    std::vector<Vertex *> T(0);
    int maxSumDegree = 2 * k - ell - 1;

    bool hasEdgeToChange;
    do {
        hasEdgeToChange = DFS(v1, v2);
    } while ((v1->getInDegree() + v2-> getInDegree() > maxSumDegree) && hasEdgeToChange);

    if (v1->getInDegree() + v2-> getInDegree() <= maxSumDegree) {
        return T;
    } else {
        for (std::pair<const int, Vertex* > & v : vertices) {
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
            Vertex * v = getVertex(i);
            std::map<int, bool> inTheSameM_componentWith_i = getSameComponentVector(v);  // c_i in the paper
            std::vector<int> neighborIds = G.getNeighbors(v->getId());

            for (int neighborId : neighborIds) {
                Vertex * neighbor = getVertex(neighborId);

                if (v < neighbor) {  // not to add two times
                // Gprime is jut a check, it could be deleted for faster run
                    Gprime.addEdge(v->getId(), neighbor->getId());
                }
                if (inTheSameM_componentWith_i[neighbor->getId()])
                    continue;  // in this case, no action is needed

                std::vector<Vertex *> T = getT(v, neighbor);

                if (T.empty()) {  // you can add one edge
                    HyperEdge * newHyperEgde = new HyperEdge({v, neighbor});
                    SpanningGraph.addEdge(v->getId(), neighbor->getId());
                    if (v->getInDegree() < neighbor->getInDegree()) {
                        addDirEdge(v, neighbor);
                    } else {
                        addDirEdge(neighbor, v);
                    }
                    continue;
                } else {
                    undirectedHyperEdges.push_back(new HyperEdge(T, false));
                    HyperEdge * newHyperEgde = undirectedHyperEdges.back();
                    for (DirectedHyperEdge& hyperEdge : directedHyperEdges) {
                        if (hyperEdge.getHyperEdge()->isUsedInThisDFS()) {
                            hyperEdge.changeUnderlyingEdge(newHyperEgde);
                        }
                    }
                }
            }
        }
    }
}

