#include <utility>


#include "graph.h"
#include "M-comp.h"
#include "redund.h"



void RedundHyperGraph::markOneTight(Vertex * head, Vertex * j) {
    std::vector<Vertex *> Tij = getT(head, j);
    for (Vertex* v : Tij) {
        setMark(v, true);
        setUsedForStar(v, false);
    }
    setUsedForStar(j, true);
}


std::vector<Vertex *> RedundHyperGraph::StarSearch(Vertex * i, std::vector<Vertex *> L = std::vector<Vertex *>()) {
    //    Algorithm 4.4 in the paper
    //    O(|V|^2)

    for (std::pair<const int, Vertex* > & v : vertices) {
        setMark(v.second, false);
        setUsedForStar(v.second, false);
    }

    setMark(i, true);
    for (Vertex* j : L) {
        if (!isMarked(j)) {
            markOneTight(i, j);
        }
    }

    for (std::pair<const int, Vertex* > & v : vertices) {
        if (!isMarked(v.second)) {
            markOneTight(i, v.second);
        }
    }
    std::vector<Vertex *> P(0);
    for (std::pair<const int, Vertex* > & v : vertices) {
        if (isUsedForStar(v.second)) {
            P.push_back(v.second);
        }
    }
    return P;
}

Vertex * RedundHyperGraph::findLowDegreeVertex() {
    std::vector<int> degree(getNumberOfVertices(), 0);
    for (auto& edge : SpanningGraph.getEdges()) {
        degree[edge.getEdge()[0]]++;
        degree[edge.getEdge()[1]]++;
    }

    int i = 0;
    while (degree[i] >= 2 * k) {i++;}
    return getVertex(i);
}


std::vector<Vertex *> RedundHyperGraph::findTransversal(std::vector<Vertex *> L) {
    if (isRigid()) {
    Vertex * i = findLowDegreeVertex();
    std::vector<Vertex *> ViL = StarSearch(i, L);
    if (ViL.size() == 1) {
        ViL.push_back(i);
        return ViL;
    }
    if (ViL.size() == 2) {
        for (std::pair<const int, Vertex* > & v : vertices) {
            if (isWholeSized(getT(ViL[0], v.second))) {
                return std::vector<Vertex *>{ViL[0], v.second};
            }
            if (isWholeSized(getT(ViL[1], v.second))) {
                return std::vector<Vertex *>{ViL[1], v.second};
            }
        }
    }
    std::vector<Vertex *> P = StarSearch(ViL[0], L);
    P.push_back(ViL[0]);
    if (P.size() < 2) {
        std::cerr << "Too small transversal" << std::endl;
    }
    return P;
    } else {
        std::cerr << "G is not rigid" <<std::endl;
        return  std::vector<Vertex *>();
    }
}

bool RedundHyperGraph::threeInTwo(
    const std::vector<Vertex* >& T1, const std::vector<Vertex* >& T2, const std::vector<Vertex* >& T3,
    const std::vector<Vertex* >& L1, const std::vector<Vertex* >& L2) const {  // O(|V|)
        std::vector<bool> isIn(getNumberOfVertices(), false);
        for (Vertex * v : L1) {
            isIn[v->getId()] = true;
        }
        for (Vertex * v : L2) {
            isIn[v->getId()] = true;
        }

        for (Vertex * v : T1) {
            if (!isIn[v->getId()])
                return false;
        }
        for (Vertex * v : T2) {
            if (!isIn[v->getId()])
                return false;
        }
        for (Vertex * v : T3) {
            if (!isIn[v->getId()])
                return false;
        }
        return true;
}

std::vector<Edge> RedundHyperGraph::toRedund() {
    if (getNumberOfVertices() < 4) {
        std::cerr << "Too few vertices" << std::endl;
        return std::vector<Edge>();
    }
    if (isRigid()) {
    std::vector<Vertex *> P = findTransversal();
    if (P.size() < 2) {
        return std::vector<Edge>();
    }
    std::vector<Edge> F;
    while (P.size() >= 4) {
        Vertex * i_1 = P[0];
        Vertex * i_h = P[P.size()-1];
        Vertex * i_h1 = P[P.size()-2];
        Vertex * i_h2 = P[P.size()-3];
        std::vector<Vertex* > T_1_h2 = getT(i_1, i_h2);
        std::vector<Vertex* > T_1_h1 = getT(i_1, i_h1);
        std::vector<Vertex* > T_1_h = getT(i_1, i_h);
        std::vector<Vertex* > T_h_h1 = getT(i_h, i_h1);

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
        std::cerr << "G is not rigid" <<std::endl;
        return std::vector<Edge>();
    }
}