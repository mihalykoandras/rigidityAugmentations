#include "graph.h"

DirectedHyperGraph CreateMCompHypergraph(SimpleGraph& G, int k, int ell) {
    if (G.getNumberOfEdges() == 0) {
        return DirectedHyperGraph(G.getNumberOfNodes());
    } else {
        SimpleGraph Gprime(G.getNumberOfNodes());
        DirectedHyperGraph HGprime(G.getNumberOfNodes());

        for (int i = 0; i < G.getNumberOfNodes(); i++) {
            std::vector<bool> inTheSameM_componentWith_i(false,G.getNumberOfNodes());  // c_i in the paper

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
