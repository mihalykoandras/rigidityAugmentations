#include <queue>

#include "graph.h"
#include "M-comp.h"

void M_comp_Hyper_Graph::addHyperEdge(const HyperEdge& edge,  Vertex * head) {
    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.emplace_back(head, &undirectedHyperEdges.back());
    headOfHyperEdge[head->getId()]->push_front(&headOfHyperEdge[head->getId()], &directedHyperEdges.back());
    head->increaseInDegree();

    for (const auto& v : edge.getVertices()) {
            vertexInHyperEdge[v->getId()].push_back(&undirectedHyperEdges.back());
    }
}

void M_comp_Hyper_Graph::changeDirection(EdgeList* edge, Vertex * to) {
    DirectedHyperEdge * realEdge = edge->getEdge();
    Vertex * from = realEdge->getHead();
    from->decreaseInDegree();
    realEdge->setHead(to);
    headOfHyperEdge[to->getId()]->push_front(&headOfHyperEdge[to->getId()], realEdge);
    to->increaseInDegree();
    headOfHyperEdge[from->getId()]->deleteNode(&headOfHyperEdge[from->getId()], edge);//Ha egy pont van, akkor hogyan törlünk? 
}



std::vector<bool> M_comp_Hyper_Graph::getSameComponentVector(Vertex * v) {
    std::vector<bool> c_v(getNumberOfNodes(), 0);
    std::list<HyperEdge*> hyperEdges = inHyperEdge(v);
    for (HyperEdge* hyperedge : hyperEdges) {
        for (const auto& vertex : hyperedge->getVertices()) {
            c_v[vertex->getId()] = true;
        }
    }
    return c_v;
}

bool M_comp_Hyper_Graph::DFS(Vertex * v1, Vertex * v2) {
    /*
    Vertices that are used in this DFS form T(ij)
    */
    for (HyperEdge& h : undirectedHyperEdges) {  // O(n)
        h.setUsedInThisDFS(false);
    }
    for (Vertex& v : vertices) {  // O(n)
        v.setUsedInThisDFS(false);
    }
    std::queue<Vertex *> Q;

    Q.push(v1);
    v1->setUsedInThisDFS(true);

    
    Q.push(v2);
    v2->setUsedInThisDFS(true);
    

    while (!Q.empty()) {
        Vertex * actualVertex = Q.front();
        Q.pop();
        if (!(*actualVertex == *v1 || *actualVertex == *v2) &&
            actualVertex->getInDegree() < k) {
            // turn around
            // and return
            Vertex * v = actualVertex;
            do {
                EdgeList* comeFrom = actualVertex->getFrom();
                v = comeFrom->getEdge()->getHead();
                changeDirection(comeFrom, actualVertex);
                actualVertex = v;
            } while ( !((*actualVertex == *v1) || (*actualVertex == *v2)) );
            return true;
        } else {
            EdgeList * nodeList = isHeadOf(actualVertex);
            while (nodeList !=NULL) {
                DirectedHyperEdge* dirEdge = nodeList->getEdge();
                if (!dirEdge->getHyperEdge()->isUsedInThisDFS()) {
                     // This can be used for transverse back
                    std::vector<Vertex *> edgeVertices = dirEdge->getHyperEdge()->getVertices();
                    for (Vertex* newVert : edgeVertices) {
                        if (!newVert->isUsedInThisDFS()) {
                            Q.push(newVert);
                            newVert->setUsedInThisDFS(true);
                            newVert->setIncomingHyperedge(nodeList);
                        }
                    }
                }
                nodeList = nodeList->getNext();
            }
        }
    }
    return false;
}



void M_comp_Hyper_Graph::MakeMCompHypergraph(const SimpleGraph& G) {
    if (G.getNumberOfEdges() != 0) {
        SimpleGraph Gprime(G.getNumberOfNodes());  // graph of the already used edges
        for (int i = 0; i < G.getNumberOfNodes(); i++) {
            Vertex * v = getVertex(i);
            std::vector<bool> inTheSameM_componentWith_i = getSameComponentVector(v);  // c_i in the paper
            std::vector<int> neighborIds = G.getNeighbors(v->getId());

            for (int neighborId : neighborIds) {
                Vertex * neighbor = getVertex(neighborId);
                int maxSumDegree = 2 * k - ell - 1;

                if (v->getId() < neighbor->getId()) {  // not to add two times
                    Gprime.addEdge(v->getId(), neighbor->getId());
                }
                if (inTheSameM_componentWith_i[neighbor->getId()])
                    continue;  // in this case, no action is needed

                bool hasEdgeToChange = true;
                while (v->getInDegree() + neighbor-> getInDegree() > maxSumDegree & hasEdgeToChange) {
                    hasEdgeToChange = DFS(v, neighbor);
                }

                if (v->getInDegree() + neighbor-> getInDegree() <= maxSumDegree) {  // you can add one edge
                    HyperEdge * newHyperEgde = new HyperEdge({v, neighbor});
                    SpanningGraph.addEdge(v->getId(), neighbor->getId());
                    if (v->getInDegree() < neighbor->getInDegree()) {
                        addHyperEdge(*newHyperEgde, v);
                    } else {
                        addHyperEdge(*newHyperEgde, neighbor);
                    }
                    continue;
                } /*else {
                    // need to add new hyperedge by the seen vertices
                }  */
            }
        }
    }
}

int main() {
    SimpleGraph G;
    G.readFromInput();
    M_comp_Hyper_Graph HG(G.getNumberOfNodes(), 2, 3);
    HG.MakeMCompHypergraph(G);
    HG.print();
}
