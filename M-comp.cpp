#include "graph.h"

std::vector<bool> getSameComponentVector(int v, const DirectedHyperGraph& HG) {
    std::vector<bool> c_v(HG.getNumberOfNodes(), 0);
    std::list<HyperEdge*> hyperEdges = HG.inHyperEdge(v);
    for (HyperEdge* hyperedge : hyperEdges) {
        for (const auto& vertex : hyperedge->getVertices()) {
            c_v[vertex.getId()] = true;
        }
    }
    return c_v;
}

DirectedHyperGraph CreateMCompHypergraph(SimpleGraph& G, int k, int ell) {
    DirectedHyperGraph HGprime(G.getNumberOfNodes());

    if (G.getNumberOfEdges() == 0) {
        return HGprime;
    } else {
        SimpleGraph Gprime(G.getNumberOfNodes());
        for (int i = 0; i < G.getNumberOfNodes(); i++) {
            Vertex v = HGprime.getVertex(i);
            std::vector<bool> inTheSameM_componentWith_i=getSameComponentVector(v.getId(), HGprime);  // c_i in the paper
            std::vector<int> neighborIds = G.getNeighbors(v.getId());
            for (int neighborId : neighborIds) {
                Vertex neighbor = HGprime.getVertex(neighborId);

                if (v.getId() < neighbor.getId()) {  // not to add two times
                    Gprime.addEdge(v.getId(), neighbor.getId());
                }
                if (inTheSameM_componentWith_i[neighbor.getId()])
                    continue;  // in this case, no action is needed

                if (v.getInDegree() + neighbor.getInDegree() <= 2 * k - ell - 1) {
                    HyperEdge newHyperEgde({v, neighbor});
                    if (v.getInDegree() < neighbor.getInDegree()) {
                        HGprime.addHyperEdge(newHyperEgde, v);
                    } else {
                        HGprime.addHyperEdge(newHyperEgde, neighbor);
                    }

                    continue;
                }
            }
        }
        HGprime.print();

        return HGprime;
    }
}

int main() {
    SimpleGraph G;
    G.readFromInput();
    DirectedHyperGraph HG = CreateMCompHypergraph(G, 2, 3);
    HG.print();
}
