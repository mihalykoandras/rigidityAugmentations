#ifndef M_COMP_H_
#define M_COMP_H_

#include <vector>
#include <list>
#include <map>

#include "graph.h"


class M_compHyperGraph : public DirectedHyperGraph {
 protected:
    unsigned int k;
    int ell;

    SimpleGraph SpanningGraph;


 private:
    std::map<int, bool> vertexUsedInDFS;  // key is the id of the vertex
    std::map<int, Node<DirectedHyperEdge*> > comeFrom;  // contains the list of hyperedges needed to get to this node
    std::map<HyperEdge*, bool> hyperedgeUsedInDFS;
    std::map<HyperEdge*, bool> trivial;  // if it is underlying hyperedge for a non-trivial M-component or not



    inline bool isUsedInThisDFS(int id) {return vertexUsedInDFS[id];}
    inline bool isUsedInThisDFS(const Vertex& v) {return isUsedInThisDFS(v.getId());}
    inline bool isUsedInThisDFS(const std::shared_ptr<Vertex> v) {return isUsedInThisDFS(v->getId());}

    inline void setUsedInThisDFS(int id, bool used) {vertexUsedInDFS[id] = used;}
    inline void setUsedInThisDFS(const std::shared_ptr<Vertex>  v, bool used) {setUsedInThisDFS(v->getId(), used);}
    inline void setUsedInThisDFS(const Vertex& v, bool used) {setUsedInThisDFS(v.getId(), used);}

    inline Node<DirectedHyperEdge*> getIncomingHyperedge(int id) {return comeFrom[id];}
    inline Node<DirectedHyperEdge*> getIncomingHyperedge(const Vertex& v) {return getIncomingHyperedge(v.getId());}
    inline Node<DirectedHyperEdge*> getIncomingHyperedge(const std::shared_ptr<Vertex> v) {return getIncomingHyperedge(v->getId());}

    inline void setIncomingHyperedge(int id, Node<DirectedHyperEdge*> from) {comeFrom[id] = from;}
    inline void setIncomingHyperedge(const Vertex& v, Node<DirectedHyperEdge*> from) {
        setIncomingHyperedge(v.getId(), from);
    }
    inline void setIncomingHyperedge(const std::shared_ptr<Vertex>  v, Node<DirectedHyperEdge*> from) {
        setIncomingHyperedge(v->getId(), from);
    }

    inline bool isUsedInThisDFS(HyperEdge* edge) {return hyperedgeUsedInDFS[edge];}
    inline void setUsedInThisDFS(HyperEdge* edge, bool used) {hyperedgeUsedInDFS[edge] = used;}

    inline bool isTrivial(HyperEdge* edge) {return trivial[edge];}
    inline void setTrivial(HyperEdge* edge, bool used) {trivial[edge] = used;}

    std::map<int, bool> getSameComponentVector(std::shared_ptr<Vertex>  v);
    bool DFS(std::shared_ptr<Vertex>  v1, std::shared_ptr<Vertex>  v2);

 public:
    M_compHyperGraph(size_t n, unsigned int k_, int ell_) {
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

    void changeDirection(Node<DirectedHyperEdge*> edge, std::shared_ptr<Vertex>  to);

    bool isRigid() const {return SpanningGraph.getEdges().size() == k* getNumberOfVertices() - ell;}

    std::vector<std::shared_ptr<Vertex> > getT(std::shared_ptr<Vertex>  v1, std::shared_ptr<Vertex>  v2);
    std::vector<std::shared_ptr<Vertex> > getT(int i, int j) {return getT(vertices[i], vertices[j]);}

    void MakeMCompHypergraph(SimpleGraph& G);

    void print() const;
};
#endif  // M_COMP_H_
