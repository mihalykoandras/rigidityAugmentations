#include <queue>

#include "graph.h"
#include "M-comp.h"

void M_comp_Hyper_Graph::addHyperEdge(const HyperEdge& edge,  Vertex * head) {
    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.emplace_back(head, &undirectedHyperEdges.back());
    headOfHyperEdge[head->getId()].push_back(&directedHyperEdges.back());
    head->increaseInDegree();

    for (const auto& v : edge.getVertices()) {
            vertexInHyperEdge[v->getId()].push_back(&undirectedHyperEdges.back());
    }
}

void M_comp_Hyper_Graph::changeDirection(DirectedHyperEdge& edge, Vertex * to) {
    edge.setHead(to);
    headOfHyperEdge[to->getId()].push_back(&edge);
    to->increaseInDegree();
    // from-ból ki kell szedni valahogy.. Ez majd akkor kell, amikor bejárunk, mert tényleg csak ott kell
}



std::vector<bool> M_comp_Hyper_Graph::getSameComponentVector(Vertex * v) {
    std::vector<bool> c_v(getNumberOfNodes(), 0);
    std::list<HyperEdge*> hyperEdges = inHyperEdge(v);
    for (HyperEdge* hyperedge : hyperEdges) {
        for (const auto& vertex : hyperedge->getVertices()) {
            c_v[vertex->getId()] = true;
        }
    }
    return c_v;
}




void M_comp_Hyper_Graph::MakeMCompHypergraph(const SimpleGraph& G) {
    if (G.getNumberOfEdges() != 0) {
        SimpleGraph Gprime(G.getNumberOfNodes());  // graph of the already used edges
        for (int i = 0; i < G.getNumberOfNodes(); i++) {
            Vertex * v = getVertex(i);
            std::vector<bool> inTheSameM_componentWith_i = getSameComponentVector(v);  // c_i in the paper
            std::vector<int> neighborIds = G.getNeighbors(v->getId());

            for (int neighborId : neighborIds) {
                Vertex * neighbor = getVertex(neighborId);
                int maxSumDegree = 2 * k - ell - 1;

                if (v->getId() < neighbor->getId()) {  // not to add two times
                    Gprime.addEdge(v->getId(), neighbor->getId());
                }
                if (inTheSameM_componentWith_i[neighbor->getId()])
                    continue;  // in this case, no action is needed

                /*while (v->getInDegree() + neighbor-> getInDegree() > maxSumDegree) {
                    // BFS
                    std::list<HyperEdge> * hyperEdges = getUndirectedHyperEdges();
                    for (HyperEdge h : *hyperEdges) {
                        h.setUsedInThisDFS(false);
                    }
                    std::queue<Vertex *> Q;
                    Q.push(v);
                    Q.push(neighbor);
                    while (!Q.empty()) {
                        Vertex * actualVertex = Q.front();
                        Q.pop();
                        if (actualVertex->getInDegree() < k) {
                            
                        } else {
                            std::list<DirectedHyperEdge*> canPropagate = isHeadOf(actualVertex->getId());
                             for (DirectedHyperEdge* dirEdge : canPropagate) {
                                 if (!(dirEdge->getHyperEdge()->isUsedInThisDFS())) {
                                     // This can be used for transverse back
                                     std::vector<Vertex *> edgeVertices = dirEdge->getHyperEdge()->getVertices();
                                     for (Vertex* newVert : edgeVertices) {
                                         Q.push(newVert);
                                     }
                                 }
                            }
                        }
                    }
                }*/

                if (v->getInDegree() + neighbor-> getInDegree() <= maxSumDegree) {  // you can add one edge
                    HyperEdge * newHyperEgde = new HyperEdge({v, neighbor});
                    SpanningGraph.addEdge(v->getId(), neighbor->getId());
                    if (v->getInDegree() < neighbor->getInDegree()) {
                        addHyperEdge(*newHyperEgde, v);
                    } else {
                        addHyperEdge(*newHyperEgde, neighbor);
                    }
                    continue;
                } /*else {
                    // need to add new hyperedge by the seen vertices
                }  */
            }
        }
    }
}

int main() {
    SimpleGraph G;
    G.readFromInput();
    M_comp_Hyper_Graph HG(G.getNumberOfNodes(), 2, 3);
    HG.MakeMCompHypergraph(G);
    HG.print();
}
