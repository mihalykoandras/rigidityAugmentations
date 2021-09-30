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



std::vector<bool> getSameComponentVector(int v, const M_comp_Hyper_Graph& HG) {
    std::vector<bool> c_v(HG.getNumberOfNodes(), 0);
    std::list<HyperEdge*> hyperEdges = HG.inHyperEdge(v);
    for (HyperEdge* hyperedge : hyperEdges) {
        for (const auto& vertex : hyperedge->getVertices()) {
            c_v[vertex->getId()] = true;
        }
    }
    return c_v;
}




DirectedHyperGraph CreateMCompHypergraph(const SimpleGraph& G, int k, int ell) {
    DirectedHyperGraph * HGprime = new DirectedHyperGraph(G.getNumberOfNodes());
    SimpleGraph GStar(G.getNumberOfNodes());  // max spanning sparse subgraph
    if (G.getNumberOfEdges() == 0) {
        return *HGprime;
    } else {
        SimpleGraph Gprime(G.getNumberOfNodes());
        for (int i = 0; i < G.getNumberOfNodes(); i++) {
            Vertex * v = HGprime->getVertex(i);
            std::vector<bool> inTheSameM_componentWith_i = getSameComponentVector(v->getId(), *HGprime);  // c_i in the paper
            std::vector<int> neighborIds = G.getNeighbors(v->getId());

            for (int neighborId : neighborIds) {
                Vertex * neighbor = HGprime->getVertex(neighborId);
                int maxSumDegree = 2 * k - ell - 1;

                if (v->getId() < neighbor->getId()) {  // not to add two times
                    Gprime.addEdge(v->getId(), neighbor->getId());
                }
                if (inTheSameM_componentWith_i[neighbor->getId()])
                    continue;  // in this case, no action is needed

                while (v->getInDegree() + neighbor-> getInDegree() > maxSumDegree) {
                    // BFS
                    std::list<HyperEdge> * hyperEdges = HGprime->getUndirectedHyperEdges();
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
                            std::list<DirectedHyperEdge*> canPropagate = HGprime->isHeadOf(actualVertex->getId());
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
                }

                if (v->getInDegree() + neighbor-> getInDegree() <= maxSumDegree) {  // you can add one edge
                    HyperEdge * newHyperEgde = new HyperEdge({v, neighbor});
                    GStar.addEdge(v->getId(), neighbor->getId());
                    if (v->getInDegree() < neighbor->getInDegree()) {
                        HGprime->addHyperEdge(*newHyperEgde, v);
                    } else {
                        HGprime->addHyperEdge(*newHyperEgde, neighbor);
                    }
                    continue;
                } else {
                    // need to add new hyperedge by the seen vertices
                }

                
            }
        }
        return *HGprime;
    }
}

int main() {
    SimpleGraph G;
    G.readFromInput();
    M_comp_Hyper_Graph HG(G.getNumberOfNodes(), 2, 3);
    HG.CreateMCompHypergraph(G);
    HG.print();
}
