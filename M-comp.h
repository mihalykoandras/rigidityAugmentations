// Copyright 2021 András Mihálykó MIT License

#pragma once

#include <vector>
#include <list>
#include <memory>

#include "linkedlist.h"
#include "graph.h"



class M_compHyperGraph : public DirectedHyperGraph {
/*
    Class of directed graph that will find an optimal direction and
    with that a maximal (k,l)-sparse subgraph. This class can return the
    M-comp hypergraph of G. 
*/
 protected:
    unsigned int k;
    int ell;

    SimpleGraph SpanningGraph;


 private:
    // if create only the tight graph and not the M copm hypergraph. 
    // Important for testing purposes
    bool m_create_only_tight_graph;
    std::vector<Node<std::shared_ptr<DirectedHyperEdge> >* > comeFrom;
        // contains the list of hyperedges needed to get to this node
    std::vector<bool> trivial;
        // if it is underlying hyperedge for a non-trivial M-component or not

    inline Node<std::shared_ptr<DirectedHyperEdge> >* getIncomingHyperedge(int id) {return comeFrom[id];}
    inline Node<std::shared_ptr<DirectedHyperEdge> >* getIncomingHyperedge(const Vertex& v) {
        return getIncomingHyperedge(v.getId());
    }
    inline Node<std::shared_ptr<DirectedHyperEdge> >* getIncomingHyperedge(const std::shared_ptr<Vertex> v) {
        return getIncomingHyperedge(v->getId());
    }

    inline void setIncomingHyperedge(int id, Node<std::shared_ptr<DirectedHyperEdge> >* from) {
        comeFrom[id] = from;
    }
    inline void setIncomingHyperedge(const Vertex& v, Node<std::shared_ptr<DirectedHyperEdge> >* from) {
        setIncomingHyperedge(v.getId(), from);
    }
    inline void setIncomingHyperedge(const std::shared_ptr<Vertex>  v,
        Node<std::shared_ptr<DirectedHyperEdge> >* from) {
        setIncomingHyperedge(v->getId(), from);
    }

    inline bool isTrivial(int id) {return trivial[id];}
    inline bool isTrivial(std::shared_ptr<HyperEdge> edge) {return isTrivial(edge->getId());}
    inline void setTrivial(int id, bool used) {trivial[id] = used;}
    inline void setTrivial(std::shared_ptr<HyperEdge> edge, bool used) {setTrivial(edge->getId(), used);}

    std::vector<bool> getSameComponentVector(std::shared_ptr<Vertex>  v);
    bool DFS(std::shared_ptr<Vertex>  v1, std::shared_ptr<Vertex>  v2,
            std::vector<bool>& vertexUsedInDFS, std::vector<bool>& hyperEdgeUsedInDFS);

    void changeDirection(Node<std::shared_ptr<DirectedHyperEdge> >* edge, std::shared_ptr<Vertex>  to);


 public:
    M_compHyperGraph() {}

    M_compHyperGraph(size_t n, unsigned int k_, int ell_, bool create_only_tight_graph = false) : 
                k(k_), ell(ell_),  SpanningGraph(SimpleGraph(n)), m_create_only_tight_graph(create_only_tight_graph) {
        // 0 < k and 0 < ell < 2k
        if (k_<= 0 || ell_ <=0 || ell_ >= 2 * k_) {
            std::cerr << "Bad constraints for k and ell, it works only if 0 < k and 0 < ell < 2k" << std::endl;
            throw 20;
        }

        size = n;
        for (size_t i = 0; i < size; i++) {
            insertNewVertex(i);
        }
        comeFrom = std::vector<Node<std::shared_ptr<DirectedHyperEdge> >* >(n);
    }

    M_compHyperGraph(const M_compHyperGraph& HG) : DirectedHyperGraph(HG) {
        k = HG.k;
        ell = HG.ell;
        SpanningGraph = HG.SpanningGraph;
        comeFrom = HG.comeFrom;
        trivial = HG.trivial;
        m_create_only_tight_graph = HG.m_create_only_tight_graph;
    }

    ~M_compHyperGraph() {}

    std::shared_ptr<HyperEdge> makeNewHyperEdge(const std::vector<std::shared_ptr<Vertex> >& v_);

    bool isRigid() const {return SpanningGraph.getEdges().size() == k* getNumberOfVertices() - ell;}

    std::vector<std::shared_ptr<Vertex> > getTHyperEdges(std::shared_ptr<Vertex>  v1, std::shared_ptr<Vertex>  v2,
            std::vector<bool>& usedHyperEdges);
    std::vector<std::shared_ptr<Vertex> > getTHyperEdges(int i, int j, std::vector<bool>& usedHyperEdges){
        return getTHyperEdges(vertices[i], vertices[j], usedHyperEdges);
    }

    std::vector<std::shared_ptr<Vertex> > getT(std::shared_ptr<Vertex>  v1, std::shared_ptr<Vertex>  v2) {
        std::vector<bool> hyperedgeUsedInDFS;
        return getTHyperEdges(v1, v2, hyperedgeUsedInDFS);
    }

    std::vector<std::shared_ptr<Vertex> > getT(int i, int j) {return getT(vertices[i], vertices[j]);}

    void MakeMCompHypergraph(SimpleGraph& G);

    SimpleGraph& getSpanningGraph();

    void print() const;
};
