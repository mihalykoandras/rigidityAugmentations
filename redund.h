// Copyright 2021 András Mihálykó MIT License

#pragma once

#include <vector>
#include <memory>


#include "graph.h"
#include "M-comp.h"

class RedundHyperGraph : public M_compHyperGraph {
/*
    class of an already M-component directed hypergraph.
    Can be used to find a transversal of the MCT sets, or
    even an optimal redund augmenting edge set.  
*/
 private:
    std::vector<bool> marked;
    std::vector<bool> usedForStar;

    std::shared_ptr<Vertex> findLowDegreeVertex();
    void markOneTight(std::shared_ptr<Vertex> head, std::shared_ptr<Vertex> j);
    bool isWholeSized(const std::vector<std::shared_ptr<Vertex> >& V) const {return V.size() == size;}
    std::vector<std::shared_ptr<Vertex> > StarSearch(
        std::shared_ptr<Vertex> i, std::vector<std::shared_ptr<Vertex> > L);
        // Algorithm 4.6
    bool threeInTwo(const std::vector<std::shared_ptr<Vertex> >& T1,
        const std::vector<std::shared_ptr<Vertex> >& T2, const std::vector<std::shared_ptr<Vertex> >& T3,
        const std::vector<std::shared_ptr<Vertex> >& L1, const std::vector<std::shared_ptr<Vertex> >& L2) const;
        // utility function for Algorithm 4.14


    inline bool isMarked(int id) {return marked[id];}
    inline bool isMarked(const Vertex& v) {return isMarked(v.getId());}
    inline bool isMarked(const std::shared_ptr<Vertex> v) {return isMarked(v->getId());}

    inline void setMark(int id, bool flag) {marked[id] = flag;}
    inline void setMark(const std::shared_ptr<Vertex> v, bool flag) {setMark(v->getId(), flag);}
    inline void setMark(const Vertex& v, bool flag) {setMark(v.getId(), flag);}

    inline bool isUsedForStar(int id) {return usedForStar[id];}
    inline bool isUsedForStar(const Vertex& v) {return isUsedForStar(v.getId());}
    inline bool isUsedForStar(const std::shared_ptr<Vertex> v) {return isUsedForStar(v->getId());}

    inline void setUsedForStar(int id, bool used) {usedForStar[id] = used;}
    inline void setUsedForStar(const std::shared_ptr<Vertex> v, bool used) {setUsedForStar(v->getId(), used);}
    inline void setUsedForStar(const Vertex& v, bool used) {setUsedForStar(v.getId(), used);}

 public:
    RedundHyperGraph(const RedundHyperGraph& R) : M_compHyperGraph(R) {
        marked = R.marked;
        usedForStar = R.usedForStar;
    }

    explicit RedundHyperGraph(const M_compHyperGraph& HG) : M_compHyperGraph(HG) {
        marked = std::vector<bool>(HG.getNumberOfVertices(), false);
        usedForStar = std::vector<bool>(HG.getNumberOfVertices(), false);
    }


    ~RedundHyperGraph() {}
    std::vector<std::shared_ptr<Vertex> > findTransversal(
        std::vector<std::shared_ptr<Vertex> > L = std::vector<std::shared_ptr<Vertex> >());
        // Algorithm 4.12
    std::vector<Edge> toRedund();  // Algorithm 4.14
};
