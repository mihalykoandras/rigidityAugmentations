#include <queue>

#include "graph.h"
#include "M-comp.h"

void M_compHyperGraph::addHyperEdge(const HyperEdge& edge,  Vertex * head) {
    /* Ads undirected hyperedge every time if it encounters for every "inHypereedge"
    Might be too much, as in hyperedge will be d(G) for every v.
    */
    undirectedHyperEdges.push_back(edge);
    directedHyperEdges.emplace_back(head, &undirectedHyperEdges.back());
    List<DirectedHyperEdge*> head_of_list = *head->isHeadOf();
    head->isHeadOf()->push_front(&directedHyperEdges.back());
    head->increaseInDegree();

    for (const auto& v : edge.getVertices()) {
        v->inHyperEdge()->push_front(&undirectedHyperEdges.back());
    }
}

void M_compHyperGraph::changeDirection(Node<DirectedHyperEdge*> edge
/*comes from the list head->isHeadOf*/, Vertex * to) {
    DirectedHyperEdge* newEdge = edge.getData();
    Vertex * from = newEdge->getHead();
    newEdge->setHead(to);

    to->isHeadOf()->push_front(newEdge);
    to->increaseInDegree();

    from->decreaseInDegree();
    from->isHeadOf()->deleteNode(&edge);  //  Ha egy pont van, akkor hogyan törlünk?
}



std::vector<bool> M_compHyperGraph::getSameComponentVector(Vertex * v) {
    std::vector<bool> c_v(getNumberOfNodes(), 0);
    Node<HyperEdge*>* hyperedge = v->inHyperEdge()->getFirst();
    while (hyperedge != NULL) {  // TODO  Mivel az inHyperEdge többször is tartalmazhat u.a-t, ezért ez nem O(V^2)
        for (const auto& vertex : hyperedge->getData()->getVertices()) {
            c_v[vertex->getId()] = true;
        }
        hyperedge = hyperedge->getNext();
    }
    return c_v;
}

bool M_compHyperGraph::DFS(Vertex * v1, Vertex * v2) {
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
            actualVertex->getInDegree() < k) {  // actual vertex is correct, so
            // turn around and return
            Vertex * v = actualVertex;
            do {
                Node<DirectedHyperEdge*> comeFrom = actualVertex->getFrom();
                v = comeFrom.getData()->getHead();
                changeDirection(comeFrom, actualVertex);
                actualVertex = v;
            } while ( !((*actualVertex == *v1) || (*actualVertex == *v2)) );
            return true;
        } else {
            Node<DirectedHyperEdge*>* node = actualVertex->isHeadOf()->getFirst();
            while (node != NULL) {
                DirectedHyperEdge* dirEdge = node->getData();
                if (!dirEdge->getHyperEdge()->isUsedInThisDFS()) {
                     // This can be used for transverse back
                    std::vector<Vertex *> edgeVertices = dirEdge->getHyperEdge()->getVertices();
                    for (Vertex* newVert : edgeVertices) {
                        if (!newVert->isUsedInThisDFS()) {
                            newVert->setUsedInThisDFS(true);
                            newVert->setIncomingHyperedge(*node);
                            Q.push(newVert);
                        }
                    }
                }
                node = node->getNext();
            }
        }
    }
    return false;
}



void M_compHyperGraph::MakeMCompHypergraph(const SimpleGraph& G) {
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
    M_compHyperGraph HG(G.getNumberOfNodes(), 2, 3);
    HG.MakeMCompHypergraph(G);
    HG.print();
}
