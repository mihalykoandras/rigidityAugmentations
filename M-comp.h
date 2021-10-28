#ifndef M_COMP_H_
#define M_COMP_H_

#include <vector>
#include <list>

#include "graph.h"



class M_compHyperGraph : public DirectedHyperGraph {
 private:
    unsigned int k;
    int ell;
    SimpleGraph SpanningGraph;

 public:
    M_compHyperGraph(size_t n, unsigned int k_, int ell_) {
        k = k_;
        ell = ell_;
        size = n;
        SpanningGraph = SimpleGraph(size);
        for (size_t i = 0; i < size; i++) {
            vertices.emplace_back(i);
        }
    }

    M_compHyperGraph(const M_compHyperGraph& HG) {
        k = HG.k;
        ell = HG.ell;
        vertices = HG.vertices;
        directedHyperEdges = HG.directedHyperEdges;
        undirectedHyperEdges = HG. undirectedHyperEdges;
        size = HG.size;
    }

    ~M_compHyperGraph() {}


    void addHyperEdge(const HyperEdge& edge, Vertex * head);  // never used
    void addDirEdge(Vertex * head, Vertex * tail);

    void changeDirection(Node<DirectedHyperEdge*> edge, Vertex * to);

    std::vector<bool> getSameComponentVector(Vertex * v);
    void MakeMCompHypergraph(const SimpleGraph& G);
    bool DFS(Vertex * v1, Vertex * v2);
    std::vector<Vertex*> getT(Vertex * v1, Vertex * v2);
};
#endif  // M_COMP_H_
