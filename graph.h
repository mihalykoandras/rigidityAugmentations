#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <list>
#include <iostream>


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
            for (int i = 0; i < I.size(); i++) {
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


class HyperEdge;
class DirectedHyperEdge;
class EdgeList;

class Vertex {
 private:
        int id;
        int inDegree;
        bool printWithDegree;
        bool usedInDFS;
        EdgeList* comeFrom;

 public:
        Vertex() {}
        explicit Vertex(int id_):id(id_) {inDegree = 0; printWithDegree = false; usedInDFS = false;}
        ~Vertex() {}

        inline int getId() const {return id;}

        inline bool operator==(const Vertex &v) const {return id == v.getId();}
        inline bool operator<(const Vertex &v) const {return id < v.getId();}
        inline bool operator>(const Vertex &v) const {return id > v.getId();}

        inline int getInDegree() const {return inDegree;}
        inline void decreaseInDegree() {inDegree--;}  // TODO check if in-degree is >0
        inline void increaseInDegree() {inDegree++;}
        inline bool isUsedInThisDFS() const {return usedInDFS;}
        inline void setUsedInThisDFS(bool used) {usedInDFS = used;}
        inline EdgeList* getFrom() {return comeFrom;}
        inline void setIncomingHyperedge(EdgeList* from) {comeFrom = from;}


        inline void print() const {
            if (printWithDegree)
                std::cout << "Id: " << id << " d: " << inDegree;
            else
                std::cout << id;
            }
};

class HyperEdge {
 protected:
        std::vector<Vertex*> vertices;
        bool usedInThisDFS;
 public:
        explicit HyperEdge(const std::vector<Vertex*>& v_) : vertices(v_) {usedInThisDFS = false;}

        ~HyperEdge() {}
        inline std::vector<Vertex*> getVertices() const {return vertices;}
        inline void print() const {
            for (const auto& v : vertices) {
                    v->print();
                    std::cout << " ";
                }
                std::cout << std::endl;
        }
        inline bool isUsedInThisDFS() const {return usedInThisDFS;}
        inline void setUsedInThisDFS(bool used) {usedInThisDFS = used;}
};

class DirectedHyperEdge {
 private:
        HyperEdge *hyperEdge;
        Vertex * head;

 public:
        DirectedHyperEdge() {}
        DirectedHyperEdge(Vertex * head_, HyperEdge* hyperedge_)
         : head(head_), hyperEdge(hyperedge_) {}
        ~DirectedHyperEdge() {}
        inline Vertex * getHead() const {return head;}
        inline void setHead(Vertex* v) {head = v;}
        inline HyperEdge * getHyperEdge() {return hyperEdge;}

        inline void print() const {
            std::cout << "Head: ";
            head->print();
            std::cout << std::endl;
            std::cout << "Hyperedge: ";
            hyperEdge->print();
        }
};

class EdgeList{
 private:
    DirectedHyperEdge* edge;
    EdgeList* prev;
    EdgeList* next;

 public:
    EdgeList() {}
    void push_front(EdgeList** headRef, DirectedHyperEdge* new_data);
    void deleteNode(EdgeList* nodeToDelete);
    DirectedHyperEdge* getEdge() const {return edge;}
    void setEdge(DirectedHyperEdge* edge_) {edge = edge_;}
};

class DirectedHyperGraph {
    /*
        This class contains all data: vertices, hyperedges, directions, directions
        It does not implement any funcionality over the basics, that is for the M_comp_Hyper_Graph class
    */
 protected:
        std::vector<Vertex> vertices;
        std::list<DirectedHyperEdge> directedHyperEdges;
        std::list<HyperEdge> undirectedHyperEdges;
        size_t size;

 public:
        DirectedHyperGraph() {}
        explicit DirectedHyperGraph(size_t n) {
            size = n;
            for (size_t i = 0; i < size; i++) {
                vertices.emplace_back(i);
            }
        }

        explicit DirectedHyperGraph(const SimpleGraph& G) {
            size = G.getNumberOfNodes();
            for (size_t i = 0; i < size; i++) {
                vertices.emplace_back(i);
            }

            std::vector<Edge> graphEdges = G.getEdges();

            for (const auto& e : graphEdges) {
                std::vector<int> edge = e.getEdge();
                std::vector<Vertex*> edgeVertices = {&vertices[edge[0]], &vertices[edge[1]]};

                HyperEdge newHyperEgde(edgeVertices);

                addHyperEdge(newHyperEgde, &vertices[edge[0]]);
            }
        }

        DirectedHyperGraph(const DirectedHyperGraph& HG) {
            vertices = HG.vertices;
            directedHyperEdges = HG.directedHyperEdges;
            undirectedHyperEdges = HG. undirectedHyperEdges;
            size = HG.size;
        }

        ~DirectedHyperGraph() {}
        inline size_t getNumberOfNodes() const {return size;}

        inline Vertex * getVertex(int i) {return &vertices[i];}
        inline std::list<HyperEdge> * getUndirectedHyperEdges() {return &undirectedHyperEdges;}

        void addHyperEdge(const HyperEdge& edge, Vertex * head);
        void changeDirection(DirectedHyperEdge& edge, Vertex * to);


        void readFromInput();
        void print() const;
};
#endif  // GRAPH_H_
