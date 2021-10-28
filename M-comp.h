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

    std::vector<bool> getSameComponentVector(Vertex * v);
    bool DFS(Vertex * v1, Vertex * v2);
    Vertex * findLowDegreeVertex();
    void markOneTight(Vertex * head, Vertex * j);
    bool isWholeSized(const std::vector<Vertex* >& V) const {return V.size() == size;}
    std::vector<Vertex*> StarSearch(Vertex * i, std::vector<Vertex*> L);
    bool threeInTwo(const std::vector<Vertex* >& T1, const std::vector<Vertex* >& T2, const std::vector<Vertex* >& T3,
    const std::vector<Vertex* >& L1, const std::vector<Vertex* >& L2) const;

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

    bool isRigid() const {return SpanningGraph.getEdges().size() == k* getNumberOfVertices() - ell;}
    std::vector<Vertex*> getT(Vertex * v1, Vertex * v2);
    void MakeMCompHypergraph(const SimpleGraph& G);
    std::vector<Vertex *> findTransversal(std::vector<Vertex *> L);
    std::vector<Edge> toRedund();

    void print() const;
};
#endif  // M_COMP_H_
