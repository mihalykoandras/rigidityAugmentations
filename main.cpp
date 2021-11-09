// Copyright 2021 András Mihálykó MIT License
/*
    Driver program for M-component hypergraph creation,
    finding transversal vertices of the MCT sets and 
    adding minimal number of edges so that G becomes M-connected. 
    If G is (k,l)-tight, this latter adds the minimal number of edges to
    make G (k,l)-redundant.
*/
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

    std::vector<std::shared_ptr<Vertex> > P = R.findTransversal();
    std::cout << "Transversal of the MCT sets of the M-comp hypergraph\n";
    for (std::shared_ptr<Vertex> v : P) {
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
