#include "graph.h"
std::vector<bool> getSameComponentVector(int v, const DirectedHyperGraph& HG) {
    std::vector<bool> c_v(false, HG.getNumberOfNodes());
    std::list<HyperEdge*> hyperEdges = HG.inHyperEdge(v);
    for (HyperEdge* hyperedge : hyperEdges) {
        for (const auto& vertex : hyperedge->getVertices()) {
            c_v[vertex->getId()] = true;
        }
    }
    return c_v;
}

DirectedHyperGraph CreateMCompHypergraph(SimpleGraph& G, int k, int ell) {
    if (G.getNumberOfEdges() == 0) {
        return DirectedHyperGraph(G.getNumberOfNodes());
    } else {
        SimpleGraph Gprime(G.getNumberOfNodes());
        DirectedHyperGraph HGprime(G.getNumberOfNodes());

        for (int i = 0; i < G.getNumberOfNodes(); i++) {
            std::vector<bool> inTheSameM_componentWith_i=getSameComponentVector(i, HGprime);  // c_i in the paper

            std::vector<int> neighbors = G.getNeighbors(i);
            for (int neighbor : neighbors) {
                if (i < neighbor) {  // not to add two times
                    Gprime.addEdge(i, neighbor);
                }
            }
        }
        return HGprime;
    }
}

int main() {
    SimpleGraph G;
    G.readFromInput();
    DirectedHyperGraph HG = CreateMCompHypergraph(G, 2, 3);
    HG.print();
}
