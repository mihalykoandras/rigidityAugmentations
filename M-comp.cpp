#include <queue>

#include "graph.h"
#include "M-comp.h"

void M_compHyperGraph::addHyperEdge(const HyperEdge& edge,  Vertex * head) {
    /* 
    Ads undirected hyperedge every time if it encounters for every "inMcomp"
    */

    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.emplace_back(head, &undirectedHyperEdges.back());
    head->isHeadOf()->push_front(&directedHyperEdges.back());
    head->increaseInDegree();

    for (const auto& v : edge.getVertices()) {
        v->inMcomp()->push_front(&undirectedHyperEdges.back());
    }
}

void M_compHyperGraph::addDirEdge(Vertex * head, Vertex * tail) {
    /* 
    Ads one directed edge as a hyperedge. No new non-trivial M-component appears by this
    */
    std::vector<Vertex *> vertices = {head, tail};
    undirectedHyperEdges.emplace_back(vertices);
    directedHyperEdges.emplace_back(head, &undirectedHyperEdges.back());
    head->isHeadOf()->push_front(&directedHyperEdges.back());
    head->increaseInDegree();
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



std::vector<bool> M_compHyperGraph::getSameComponentVector(Vertex * v) {
    std::vector<bool> c_v(getNumberOfVertices(), 0);
    Node<HyperEdge*>* hyperedge = v->inMcomp()->getFirst();
    while (hyperedge != NULL) {
        if (hyperedge->getData()->isStillExistsing()) {
            for (const auto& vertex : hyperedge->getData()->getVertices()) {
                c_v[vertex->getId()] = true;
            }
        }
        hyperedge = hyperedge->getNext();
    }
    return c_v;
}

bool M_compHyperGraph::DFS(Vertex * v1, Vertex * v2) {
    for (HyperEdge& h : undirectedHyperEdges) {  // O(n)
        h.setUsedInThisDFS(false);
    }
    for (Vertex& v : vertices) {  // O(n)
        v.setUsedInThisDFS(false);
    }
    std::queue<Vertex *> Q;

    Q.push(v1);
    v1->setUsedInThisDFS(true);

    Q.push(v2);
    v2->setUsedInThisDFS(true);

    while (!Q.empty()) {
        Vertex * actualVertex = Q.front();
        Q.pop();
        if (!(*actualVertex == *v1 || *actualVertex == *v2) &&
            actualVertex->getInDegree() < k) {  // actual vertex is correct, so
            // turn around and return
            Vertex * v = actualVertex;
            do {
                Node<DirectedHyperEdge*> comeFrom = actualVertex->getFrom();
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
                        if (!newVert->isUsedInThisDFS()) {
                            newVert->setUsedInThisDFS(true);
                            newVert->setIncomingHyperedge(*node);
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
        for (Vertex& v : vertices) {
            if (v.isUsedInThisDFS()) {
                T.push_back(&v);
            }
        }
    }
    return T;
}



void M_compHyperGraph::MakeMCompHypergraph(const SimpleGraph& G) {
    if (G.getNumberOfEdges() != 0) {
        SimpleGraph Gprime(G.getNumberOfNodes());  // graph of the already used edges
        for (int i = 0; i < G.getNumberOfNodes(); i++) {
            Vertex * v = getVertex(i);
            std::vector<bool> inTheSameM_componentWith_i = getSameComponentVector(v);  // c_i in the paper
            std::vector<int> neighborIds = G.getNeighbors(v->getId());

            for (int neighborId : neighborIds) {
                Vertex * neighbor = getVertex(neighborId);

                if (v->getId() < neighbor->getId()) {  // not to add two times
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
                    undirectedHyperEdges.emplace_back(T);
                    HyperEdge * newHyperEgde = &undirectedHyperEdges.back();
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

void M_compHyperGraph::markOneTight(Vertex * head, Vertex * j) {
    std::vector<Vertex *> Tij = getT(head, j);
    for (Vertex* v : Tij) {
        v->setMark(true);
        v->setUsedForStar(false);
    }
    j->setUsedForStar(true);
}


std::vector<Vertex *> M_compHyperGraph::StarSearch(Vertex * i, std::vector<Vertex *> L = std::vector<Vertex *>()) {
    /*
        Algorithm 4.4 in the paper
        O(|V|^2)
    */
    for (Vertex& v : vertices) {
        v.setMark(false);
        v.setUsedForStar(false);
    }

    i->setMark(true);
    for (Vertex* j : L) {
        if (!j->isMarked()) {
            markOneTight(i, j);
        }
    }

    for (Vertex& v : vertices) {
        if (!v.isMarked()) {
            markOneTight(i, &v);
        }
    }
    std::vector<Vertex *> P(0);
     for (Vertex& v : vertices) {
        if (v.isUsedForStar()) {
            P.push_back(&v);
        }
    }
    return P;
}

Vertex * M_compHyperGraph::findLowDegreeVertex() {
    std::vector<int> degree(getNumberOfVertices(), 0);
    for (auto& edge : SpanningGraph.getEdges()) {
        degree[edge.getEdge()[0]]++;
        degree[edge.getEdge()[1]]++;
    }

    int i = 0;
    while (degree[i] >= 2 * k) {i++;}
    return getVertex(i);
}


std::vector<Vertex *> M_compHyperGraph::findTransversal(std::vector<Vertex *> L = std::vector<Vertex *>()) {
    Vertex * i = findLowDegreeVertex();
    std::vector<Vertex *> ViL = StarSearch(i, L);
    if (ViL.size() == 1) {
        ViL.push_back(i);
        return ViL;
    }
    if (ViL.size() == 2) {
        for (Vertex& v : vertices) {
            if (isWholeSized(getT(ViL[0], &v))) {
                return std::vector<Vertex *>{ViL[0], &v};
            }
            if (isWholeSized(getT(ViL[1], &v))) {
                return std::vector<Vertex *>{ViL[1], &v};
            }
        }
    }
    std::vector<Vertex *> P = StarSearch(ViL[0], L);
    P.push_back(ViL[0]);
    return P;
}



int main() {
    SimpleGraph G;
    G.readFromInput();
    M_compHyperGraph HG(G.getNumberOfNodes(), 2, 3);
    HG.MakeMCompHypergraph(G);
    HG.print();
    std::vector<Vertex *> P = HG.findTransversal();
    std::cout << "Vertices in P\n";
    for (Vertex * v : P) {
         v->print();
         std::cout << " ";
    }
    std::cout << "\n";
}
