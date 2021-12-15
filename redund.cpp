// Copyright 2021 András Mihálykó MIT License

#include <utility>

#include "graph.h"
#include "M-comp.h"
#include "redund.h"



void RedundHyperGraph::markOneTight(std::shared_ptr<Vertex> head, std::shared_ptr<Vertex> j) {
    std::vector<std::shared_ptr<Vertex> > Tij = getT(head, j);
    for (std::shared_ptr<Vertex> v : Tij) {
        setMark(v, true);
        setUsedForStar(v, false);
    }
    setUsedForStar(j, true);
}


std::vector<std::shared_ptr<Vertex> > RedundHyperGraph::StarSearch(
    std::shared_ptr<Vertex> i, std::vector<std::shared_ptr<Vertex> > L = std::vector<std::shared_ptr<Vertex> >()) {
    // Running time: O(|V|^2)

    for (std::shared_ptr<Vertex>  & v : vertices) {
        setMark(v, false);
        setUsedForStar(v, false);
    }

    setMark(i, true);
    for (std::shared_ptr<Vertex> j : L) {
        if (!isMarked(j)) {
            markOneTight(i, j);
        }
    }

    for (std::shared_ptr<Vertex> & v : vertices) {
        if (!isMarked(v)) {
            markOneTight(i, v);
        }
    }
    std::vector<std::shared_ptr<Vertex> > P(0);
    for (std::shared_ptr<Vertex> & v : vertices) {
        if (isUsedForStar(v)) {
            P.push_back(v);
        }
    }
    return P;
}

std::shared_ptr<Vertex> RedundHyperGraph::findLowDegreeVertex() {
    std::vector<int> degree(getNumberOfVertices(), 0);
    for (auto& edge : SpanningGraph.getEdges()) {
        degree[edge.getEdge()[0]]++;
        degree[edge.getEdge()[1]]++;
    }

    int i = 0;
    while (degree[i] >= 2 * k) {i++;}
    return getVertex(i);
}


std::vector<std::shared_ptr<Vertex> > RedundHyperGraph::findTransversal(std::vector<std::shared_ptr<Vertex> > L) {
    if (isRigid()) {
    std::shared_ptr<Vertex> i = findLowDegreeVertex();
    std::vector<std::shared_ptr<Vertex> > ViL = StarSearch(i, L);
    if (ViL.size() == 1) {
        ViL.push_back(i);
        return ViL;
    }
    if (ViL.size() == 2) {
        for (std::shared_ptr<Vertex> & v : vertices) {
            if (isWholeSized(getT(ViL[0], v))) {
                return std::vector<std::shared_ptr<Vertex> >{ViL[0], v};
            }
            if (isWholeSized(getT(ViL[1], v))) {
                return std::vector<std::shared_ptr<Vertex> >{ViL[1], v};
            }
        }
    }
    std::vector<std::shared_ptr<Vertex> > P = StarSearch(ViL[0], L);
    P.push_back(ViL[0]);

    return P;
    } else {
        std::cerr << "G is not rigid - can't compute transversal of MCT sets." <<std::endl;
        throw 32;
    }
}

bool RedundHyperGraph::threeInTwo(
    const std::vector<std::shared_ptr<Vertex> >& T1, const std::vector<std::shared_ptr<Vertex> >& T2,
    const std::vector<std::shared_ptr<Vertex> >& T3, const std::vector<std::shared_ptr<Vertex> >& L1,
    const std::vector<std::shared_ptr<Vertex> >& L2) const {
        // Running time: O(|V|)
        std::vector<bool> isIn(getNumberOfVertices(), false);
        for (std::shared_ptr<Vertex> v : L1) {
            isIn[v->getId()] = true;
        }
        for (std::shared_ptr<Vertex> v : L2) {
            isIn[v->getId()] = true;
        }

        for (std::shared_ptr<Vertex> v : T1) {
            if (!isIn[v->getId()])
                return false;
        }
        for (std::shared_ptr<Vertex> v : T2) {
            if (!isIn[v->getId()])
                return false;
        }
        for (std::shared_ptr<Vertex> v : T3) {
            if (!isIn[v->getId()])
                return false;
        }
        return true;
}

std::vector<Edge> RedundHyperGraph::toRedund() {
    if (getNumberOfVertices() < 4) {
        std::cerr << "Too few vertices in the graph to augment to comute its optimal augmentation." << std::endl;
        throw 30;
    }
    if (isRigid()) {
        std::vector<std::shared_ptr<Vertex> > P = findTransversal();
        if (P.size() < 2) {
            std::cerr << "Too small transversal" << std::endl;
            throw 31;
        }
        if (P.size() < 2) {
            return std::vector<Edge>();
        }
        std::vector<Edge> F;
        while (P.size() >= 4) {
            std::shared_ptr<Vertex> i_1 = P[0];
            std::shared_ptr<Vertex> i_h = P[P.size()-1];
            std::shared_ptr<Vertex> i_h1 = P[P.size()-2];
            std::shared_ptr<Vertex> i_h2 = P[P.size()-3];
            std::vector<std::shared_ptr<Vertex> > T_1_h2 = getT(i_1, i_h2);
            std::vector<std::shared_ptr<Vertex> > T_1_h1 = getT(i_1, i_h1);
            std::vector<std::shared_ptr<Vertex> > T_1_h = getT(i_1, i_h);
            std::vector<std::shared_ptr<Vertex> > T_h_h1 = getT(i_h, i_h1);

            if (threeInTwo(T_1_h2, T_1_h1, T_1_h, T_1_h2, T_h_h1)) {
                F.emplace_back(i_h1->getId(), i_h->getId());
            } else {
                F.emplace_back(i_h2->getId(), i_h->getId());
                P[P.size()-3] = P[P.size()-2];
            }
            P.pop_back();
            P.pop_back();
        }
        if (P.size() == 2) {
            F.emplace_back(P[0]->getId(), P[1]->getId());
        } else {  // == 3
            F.emplace_back(P[0]->getId(), P[1]->getId());
            F.emplace_back(P[0]->getId(), P[2]->getId());
        }
        return F;
    } else {
        std::cerr << "G is not rigid - can't compute optimal augmentation." <<std::endl;
        throw 32;
    }
}
