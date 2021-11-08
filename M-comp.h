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

    inline bool isUsedInThisDFS(int id) {return vertexUsedInDFS[id];}
    inline bool isUsedInThisDFS(const Vertex& v) {return isUsedInThisDFS(v.getId());}
    inline bool isUsedInThisDFS(const Vertex* v) {return isUsedInThisDFS(v->getId());}

    inline void setUsedInThisDFS(int id, bool used) {vertexUsedInDFS[id] = used;}
    inline void setUsedInThisDFS(const Vertex * v, bool used) {setUsedInThisDFS(v->getId(), used);}
    inline void setUsedInThisDFS(const Vertex& v, bool used) {setUsedInThisDFS(v.getId(), used);}

    inline Node<DirectedHyperEdge*> getIncomingHyperedge(int id) {return comeFrom[id];}
    inline Node<DirectedHyperEdge*> getIncomingHyperedge(const Vertex& v) {return getIncomingHyperedge(v.getId());}
    inline Node<DirectedHyperEdge*> getIncomingHyperedge(const Vertex* v) {return getIncomingHyperedge(v->getId());}

    inline void setIncomingHyperedge(int id, Node<DirectedHyperEdge*> from) {comeFrom[id] = from;}
    inline void setIncomingHyperedge(const Vertex& v, Node<DirectedHyperEdge*> from) {
        setIncomingHyperedge(v.getId(), from);
    }
    inline void setIncomingHyperedge(const Vertex * v, Node<DirectedHyperEdge*> from) {
        setIncomingHyperedge(v->getId(), from);
    }

    std::map<int, bool> getSameComponentVector(Vertex * v);
    bool DFS(Vertex * v1, Vertex * v2);

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
    }

    ~M_compHyperGraph() {}

    void changeDirection(Node<DirectedHyperEdge*> edge, Vertex * to);

    bool isRigid() const {return SpanningGraph.getEdges().size() == k* getNumberOfVertices() - ell;}

    std::vector<Vertex*> getT(Vertex * v1, Vertex * v2);
    std::vector<Vertex*> getT(int i, int j) {return getT(vertices[i], vertices[j]);}

    void MakeMCompHypergraph(SimpleGraph& G);

    void print() const;
};
#endif  // M_COMP_H_
