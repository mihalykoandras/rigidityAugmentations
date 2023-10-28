// Copyright 2021 András Mihálykó MIT License
/*
    Driver program for M-component hypergraph creation,
    finding transversal vertices of the MCT sets and 
    adding minimal number of edges so that G becomes M-connected. 
    If G is (k,l)-tight, this latter adds the minimal number of edges to
    make G (k,l)-redundant.

    This is an accompanying program to the PhD dissertation titled 
    "Augmentation problems in count matroids and globally rigid graphs"
*/
#include <iostream>

#include "graph.h"
#include "M-comp.h"
#include "redund.h"


int main(int argc, char *argv[]) {
    unsigned int k = 2;
    int ell = 3;
    bool create_m_comp_hypergraph = true;
    try {
        if (argc >= 3) {
            if (atoi(argv[1]) > 0) {
                k = atoi(argv[1]);
                ell = atoi(argv[2]);
            } else {
                std::cerr << "k must be positive" << std::endl;
                throw 1;
            }
        }
        if (argc == 4) {
            create_m_comp_hypergraph = atoi(argv[3]);
        }
    } catch (int e) {
        std::cerr <<"Error code " << e << std::endl;
        return e;
    }

    SimpleGraph G;

    try {
        G.readFromInput();
    } catch (int e) {
        std::cerr <<"Error code " << e << std::endl;
        return e;
    }

    M_compHyperGraph HG;

    try {
        HG = M_compHyperGraph(G.getNumberOfNodes(), k, ell, !create_m_comp_hypergraph);
    } catch (int e) {
        std::cerr <<"Error code " << e << std::endl;
        return e;
    }
    HG.MakeMCompHypergraph(G);
    std::cout << " " << std::endl;
    HG.print();

    if (create_m_comp_hypergraph) {
        RedundHyperGraph R(HG);

        std::vector<std::shared_ptr<Vertex> > P;
        bool exceptionCaught = true;
        try {
            P = R.findTransversal();
            exceptionCaught = false;
        } catch (int e) {
            std::cerr <<"Error code " << e << std::endl;
        }
        if (!exceptionCaught) {
            std::cout << "Transversal of the MCT sets of the M-comp hypergraph"  << std::endl;
            for (std::shared_ptr<Vertex> v : P) {
                 v->print();
                 std::cout << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Optimal redund augmentation edges: " << std::endl;
        std::vector<Edge> F;
        try {
            F = R.toRedund();
        } catch (int e) {
            std::cerr <<"Error code " << e << std::endl;

            if (e == 31) {
                return e;
            }
        }
        for (Edge& f : F) {
            f.print();
        }
    }
}
