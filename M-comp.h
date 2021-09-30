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
    std::vector<EdgeList* > headOfHyperEdge;
    SimpleGraph SpanningGraph;

 public:
    M_comp_Hyper_Graph(size_t n, unsigned int k_, int ell_) {
        k = k_;
        ell = ell_;
        size = n;
        SpanningGraph = SimpleGraph(size);
        for (size_t i = 0; i < size; i++) {
            vertices.emplace_back(i);
        }
        vertexInHyperEdge = std::vector<std::list<HyperEdge*> >(size);
        headOfHyperEdge = std::vector<EdgeList* >(size);
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

    inline std::list<HyperEdge*> inHyperEdge(Vertex * v) const {return vertexInHyperEdge[v->getId()];}
    inline EdgeList* isHeadOf(Vertex * v) const {return headOfHyperEdge[v->getId()];}

    void addHyperEdge(const HyperEdge& edge, Vertex * head);
    void changeDirection(EdgeList* edge, Vertex * to);

    std::vector<bool> getSameComponentVector(Vertex * v);
    void MakeMCompHypergraph(const SimpleGraph& G);
    bool DFS(Vertex * v1, Vertex * v2);
};
#endif  // M_COMP_H_
