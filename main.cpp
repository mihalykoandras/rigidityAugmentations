#include "graph.h"
#include "M-comp.h"
#include "redund.h"


int main(int argc, char *argv[]) {
    unsigned int k = 2;
    int ell = 3;
    if (argc == 3) {
        if (atoi(argv[1]) > 0) {
            k = atoi(argv[1]);
            ell = atoi(argv[2]);
        } else {
            std::cerr << "k is not correct" << std::endl;
        }
    }

    SimpleGraph G;
    G.readFromInput();
    M_compHyperGraph HG(G.getNumberOfNodes(), k, ell);
    HG.MakeMCompHypergraph(G);
    HG.print();

    RedundHyperGraph R(HG);

    std::vector<Vertex *> P = R.findTransversal();
    std::cout << "Transversal of the MCT sets of the M-comp hypergraph\n";
    for (Vertex * v : P) {
         v->print();
         std::cout << " ";
    }
    std::cout << "\n";
    std::cout << "Optimal redund augmentation edges\n";
    std::vector<Edge> F = R.toRedund();
    for (Edge& f : F) {
        f.print();
    }
}
