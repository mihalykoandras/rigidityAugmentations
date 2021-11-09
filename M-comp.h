// Copyright 2021 András Mihálykó MIT License

#pragma once

#include <vector>
#include <list>
#include <map>
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
    std::map<int, bool> vertexUsedInDFS;  // key is the id of the vertex
    std::map<int, Node<std::shared_ptr<DirectedHyperEdge> >* > comeFrom;
        // contains the list of hyperedges needed to get to this node
    std::map<std::shared_ptr<HyperEdge>, bool> hyperedgeUsedInDFS;
    std::map<std::shared_ptr<HyperEdge>, bool> trivial;
        // if it is underlying hyperedge for a non-trivial M-component or not

    inline bool isUsedInThisDFS(int id) {return vertexUsedInDFS[id];}
    inline bool isUsedInThisDFS(const Vertex& v) {return isUsedInThisDFS(v.getId());}
    inline bool isUsedInThisDFS(const std::shared_ptr<Vertex> v) {return isUsedInThisDFS(v->getId());}

    inline void setUsedInThisDFS(int id, bool used) {vertexUsedInDFS[id] = used;}
    inline void setUsedInThisDFS(const std::shared_ptr<Vertex>  v, bool used) {setUsedInThisDFS(v->getId(), used);}
    inline void setUsedInThisDFS(const Vertex& v, bool used) {setUsedInThisDFS(v.getId(), used);}

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

    inline bool isUsedInThisDFS(std::shared_ptr<HyperEdge> edge) {return hyperedgeUsedInDFS[edge];}
    inline void setUsedInThisDFS(std::shared_ptr<HyperEdge> edge, bool used) {hyperedgeUsedInDFS[edge] = used;}

    inline bool isTrivial(std::shared_ptr<HyperEdge> edge) {return trivial[edge];}
    inline void setTrivial(std::shared_ptr<HyperEdge> edge, bool used) {trivial[edge] = used;}

    std::map<int, bool> getSameComponentVector(std::shared_ptr<Vertex>  v);
    bool DFS(std::shared_ptr<Vertex>  v1, std::shared_ptr<Vertex>  v2);

 public:
    M_compHyperGraph() {}

    M_compHyperGraph(size_t n, unsigned int k_, int ell_) {
        // 0 < k and 0 < ell <2k
        if (k_<= 0 || ell_ <=0 || ell_ >= 2 * k_) {
            std::cerr << "Bad constraints for k and ell, it works only if 0 < k and 0 < ell < 2k" << std::endl;
            throw 20;
        }

        k = k_;
        ell = ell_;
        size = n;
        SpanningGraph = SimpleGraph(size);
        for (size_t i = 0; i < size; i++) {
            insertNewVertex(i);
        }
    }

    M_compHyperGraph(const M_compHyperGraph& HG) : DirectedHyperGraph(HG) {
        k = HG.k;
        ell = HG.ell;
        SpanningGraph = HG.SpanningGraph;
        vertexUsedInDFS = HG.vertexUsedInDFS;
        comeFrom = HG.comeFrom;
        trivial = HG.trivial;
        hyperedgeUsedInDFS = HG.hyperedgeUsedInDFS;
    }

    ~M_compHyperGraph() {}

    void changeDirection(Node<std::shared_ptr<DirectedHyperEdge> >* edge, std::shared_ptr<Vertex>  to);

    bool isRigid() const {return SpanningGraph.getEdges().size() == k* getNumberOfVertices() - ell;}

    std::vector<std::shared_ptr<Vertex> > getT(std::shared_ptr<Vertex>  v1, std::shared_ptr<Vertex>  v2);
    std::vector<std::shared_ptr<Vertex> > getT(int i, int j) {return getT(vertices[i], vertices[j]);}

    void MakeMCompHypergraph(SimpleGraph& G);

    void print() const;
};
