#ifndef M_COMP_H_
#define M_COMP_H_

#include <vector>
#include <list>

#include "graph.h"


class M_comp_Hyper_Graph :public DirectedHyperGraph {
 private:
    unsigned int k;
    int ell;
    std::vector<std::list<HyperEdge*> > vertexInHyperEdge;
    std::vector<std::list<DirectedHyperEdge*> > headOfHyperEdge;

 public:
    M_comp_Hyper_Graph(size_t n, unsigned int k_, int ell_) {
        k = k_;
        ell = ell_;
        size = n;
        for (size_t i = 0; i < size; i++) {
            vertices.emplace_back(i);
        }
        vertexInHyperEdge = std::vector<std::list<HyperEdge*> >(size);
        headOfHyperEdge = std::vector<std::list<DirectedHyperEdge*> >(size);
    }

    M_comp_Hyper_Graph(const M_comp_Hyper_Graph& HG) {
        k = HG.k;
        ell = HG.ell;
        vertices = HG.vertices;
        directedHyperEdges = HG.directedHyperEdges;
        undirectedHyperEdges = HG. undirectedHyperEdges;
        size = HG.size;
        vertexInHyperEdge = HG.vertexInHyperEdge;
        headOfHyperEdge = HG.headOfHyperEdge;
    }

    ~M_comp_Hyper_Graph() {}

    inline std::list<HyperEdge*> inHyperEdge(int v) const {return vertexInHyperEdge[v];}
    inline std::list<DirectedHyperEdge*> isHeadOf(int v) const {return headOfHyperEdge[v];}

    void addHyperEdge(const HyperEdge& edge, Vertex * head);
    void changeDirection(DirectedHyperEdge& edge, Vertex * to);
};
#endif  // M_COMP_H_
