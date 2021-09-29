#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <list>
#include <iostream>

class HyperEdge;
class DirectedHyperEdge;

class Vertex {
 private:
        int id;
        int inDegree;
 public:
        Vertex() {}
        explicit Vertex(int id_):id(id_) {inDegree = 0;}
        ~Vertex() {}

        inline int getId() const {return id;}

        inline bool operator==(const Vertex &v) const {return id == v.getId();}
        inline bool operator<(const Vertex &v) const {return id < v.getId();}
        inline bool operator>(const Vertex &v) const {return id > v.getId();}

        inline int getInDegree() const {return inDegree;}
        inline void decreaseInDegree() {inDegree--;}  // TODO check if in-degree is >0
        inline void increaseInDegree() {inDegree++;}

        inline void print() const {std::cout << id;}
};

class HyperEdge {
 protected:
        std::vector<Vertex> vertices;

 public:
        explicit HyperEdge(const std::vector<Vertex>& v_) : vertices(v_) {}

        ~HyperEdge() {/*vertices.clear();*/}
        inline std::vector<Vertex> getVertices() const {return vertices;}
        inline void print() const {
            for (const auto& v : vertices) {
                    v.print();
                    std::cout << " ";
                }
                std::cout << std::endl;
        }
};

class DirectedHyperEdge {
 private:
        HyperEdge *hyperEdge;
 public:
            Vertex head;

        DirectedHyperEdge() {}
        DirectedHyperEdge(Vertex head_, HyperEdge* hyperedge_)
         : head(head_), hyperEdge(hyperedge_) {}
        ~DirectedHyperEdge() {}
        inline Vertex getHead()const {return head;}
        inline void setHead(const Vertex& v) {head = v;}
        inline void print() const {
            std::cout << "Head: ";
            head.print();
            std::cout << std::endl;
            std::cout << "Hyperedge: ";
            hyperEdge->print();
        }
};

struct Edge {
 private:
        int epi, epj;

 public:
        Edge(int i, int j) : epi(i), epj(j) {}
        std::vector<int> getEdge() const {
            std::vector<int> v = {epi, epj};
            return v;
        }
};

class SimpleGraph {
// the very simple basic graph implementation, nodes indexed by ints
 private:
    int numberOfVertices;
    std::vector<Edge> edgeList;
    std::vector<std::vector<int> > neighborLists;

 public:
    SimpleGraph() {}
    explicit SimpleGraph(int n) : numberOfVertices(n) {
        neighborLists = std::vector<std::vector<int> >(numberOfVertices);}
    SimpleGraph(int n, const std::vector<int>& I, const std::vector<int>& J) {
        numberOfVertices = n;
        neighborLists = std::vector<std::vector<int> >(numberOfVertices);

        if (I.size() == J.size()) {
            for (int i = 0; i < I.size(); i++ ){
                edgeList.emplace_back(I[i], J[i]);
                neighborLists[I[i]].push_back(J[i]);
                neighborLists[J[i]].push_back(I[i]);
            }
        } else {
            // EXCEPTION
            std::cout << "Two list not the same length" << std::endl;
        }
    }

    inline int getNumberOfNodes() const {return numberOfVertices;}
    inline void addEdge(int i, int j) {
        edgeList.emplace_back(i, j);
        neighborLists[i].push_back(j);
        neighborLists[j].push_back(i);
    }
    inline size_t getNumberOfEdges() const {return edgeList.size();}
    std::vector<Edge> getEdges() const {return edgeList;}
    std::vector<int> getNeighbors(int i) const {return neighborLists[i];}

    void readFromInput();
};

class DirectedHyperGraph {
 protected:
        std::vector<Vertex> vertices;
        std::list<DirectedHyperEdge> directedHyperEdges;
        std::list<HyperEdge> undirectedHyperEdges;
        size_t size;

        std::vector<std::list<HyperEdge*> > vertexInHyperEdge;
        std::vector<std::list<DirectedHyperEdge*> > headOfHyperEdge;

 public:
        DirectedHyperGraph() {}
        explicit DirectedHyperGraph(size_t n) {
            size = n;
            for (size_t i = 0; i < size; i++) {
                vertices.emplace_back(i);
            }
            vertexInHyperEdge = std::vector<std::list<HyperEdge*> >(size);
            headOfHyperEdge = std::vector<std::list<DirectedHyperEdge*> >(size);
        }

        explicit DirectedHyperGraph(const SimpleGraph& G) {
            size = G.getNumberOfNodes();
            for (size_t i = 0; i < size; i++) {
                vertices.emplace_back(i);
            }
            vertexInHyperEdge = std::vector<std::list<HyperEdge*> >(size);
            headOfHyperEdge = std::vector<std::list<DirectedHyperEdge*> >(size);

            std::vector<Edge> graphEdges = G.getEdges();

            for (const auto& e : graphEdges) {
                std::vector<int> edge = e.getEdge();
                std::vector<Vertex> edgeVertices = {&vertices[edge[0]], &vertices[edge[1]]};

                HyperEdge newHyperEgde(edgeVertices);

                addHyperEdge(newHyperEgde, vertices[edge[0]]);
            }
        }

        ~DirectedHyperGraph() {}
        inline size_t getNumberOfNodes() const {return size;}
        inline std::list<HyperEdge*> inHyperEdge(int v) const {return vertexInHyperEdge[v];}
        inline std::list<DirectedHyperEdge*> isHeadOf(int v) const {return headOfHyperEdge[v];}
        inline Vertex getVertex(int i) const {return vertices[i];}

        void addHyperEdge(HyperEdge& edge, Vertex head);
        void changeDirection(DirectedHyperEdge& edge, Vertex to);


        void readFromInput();
        void print() const;

};
#endif  // GRAPH_H_
