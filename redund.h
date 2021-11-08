#ifndef REDUND_H_
#define REDUND_H_

#include <vector>
#include <map>

#include "graph.h"
#include "M-comp.h"

class RedundHyperGraph : public M_compHyperGraph {
/*
    class of an already M-component directed hypergraph.
    Can be used to find a transversal of the MCT sets, or
    even an optimal redund augmenting edge set.  
*/
 private:
    std::map<int, bool> marked;
    std::map<int, bool> usedForStar;

    std::shared_ptr<Vertex> findLowDegreeVertex();
    void markOneTight(std::shared_ptr<Vertex> head, std::shared_ptr<Vertex> j);
    bool isWholeSized(const std::vector<std::shared_ptr<Vertex> >& V) const {return V.size() == size;}
    std::vector<std::shared_ptr<Vertex> > StarSearch(std::shared_ptr<Vertex> i, std::vector<std::shared_ptr<Vertex> > L);
    bool threeInTwo(const std::vector<std::shared_ptr<Vertex> >& T1, const std::vector<std::shared_ptr<Vertex> >& T2, const std::vector<std::shared_ptr<Vertex> >& T3,
    const std::vector<std::shared_ptr<Vertex> >& L1, const std::vector<std::shared_ptr<Vertex> >& L2) const;


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
        marked = std::map<int, bool>();
        usedForStar = std::map<int, bool>();
    }


    ~RedundHyperGraph() {}
    std::vector<std::shared_ptr<Vertex> > findTransversal(std::vector<std::shared_ptr<Vertex> > L = std::vector<std::shared_ptr<Vertex> >());
    std::vector<Edge> toRedund();
};

#endif  // REDUND_H_
