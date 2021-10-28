#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <list>
#include <iostream>

template<typename T>class Node{
 private:
    T data;
    Node<T>* prev;
    Node<T>* next;

 public:
    Node() {}
    Node<T>* getNext() {return next;}
    Node<T>* getPrevious() {return prev;}
    void setNext(Node<T>* next_) {next = next_;}
    void setPrevious(Node<T> * prev_) {prev = prev_;}
    void setData(T data_) {data = data_;}
    T getData() const {return data;}
    void setNode(T data_) {data = data_;}
};

template<typename T>class List{
 private:
    Node<T>* first;
    int numbOfData;

 public:
    List() {first = NULL; numbOfData = 0;}
    void push_front(T new_data);
    void deleteNode(Node<T>* nodeToDelete);
    bool isEmpty() const {return numbOfData == 0;}
    Node<T>* getFirst() {return first;}
};

template<typename T> void List<T>::push_front(T new_data) {
    Node<T>* new_node = new Node<T>();
    new_node->setData(new_data);
    new_node->setPrevious(NULL);
    new_node->setNext(first);
    if (first != NULL)
        first->setPrevious(new_node);
    first = new_node;
    numbOfData++;
}


template<typename T> void List<T>::deleteNode(Node<T>* nodeToDelete) {
    if (nodeToDelete == NULL)
        return;
    if (nodeToDelete->getData() == first->getData()) {  // delete the first
        first = nodeToDelete->getNext();
    }

    if (nodeToDelete->getNext() != NULL)
        nodeToDelete->getNext()->setPrevious(nodeToDelete->getPrevious());

    /* Change prev only if node to be
    deleted is NOT the first node */
    if (nodeToDelete->getPrevious() != NULL)
        nodeToDelete->getPrevious()->setNext(nodeToDelete->getNext());

    numbOfData--;
}


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


class Vertex {
    /* Highly speciliazed Vertex class. Contains the edges that it is a head of, 
    undirected hyperedges that contains it, and also the data for the DFS. 
     */
 private:
        int id;
        bool usedInDFS;
        int inDegree;
        Node<DirectedHyperEdge*> comeFrom;
        List<HyperEdge*> undHyperEdges;
        List <DirectedHyperEdge*> headOfHyperEdge;

 public:
        Vertex() {}
        explicit Vertex(int id_):id(id_) {inDegree = 0; usedInDFS = false;}
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
        inline Node<DirectedHyperEdge*> getFrom() {return comeFrom;}
        inline void setIncomingHyperedge(Node<DirectedHyperEdge*> from) {comeFrom = from;}
        inline List<HyperEdge*>* inMcomp() {return &undHyperEdges;}
        inline List<DirectedHyperEdge*>* isHeadOf() {return &headOfHyperEdge;}


        inline void print() const {
                std::cout << id;
        }
};

class HyperEdge {
 protected:
        std::vector<Vertex*> vertices;
        bool usedInThisDFS;
        bool stillExistsing;  // we don't delete the hyperedges that are now part of a bigger M-comp,
                              // just note that they are not useful any more
 public:
        explicit HyperEdge(const std::vector<Vertex*>& v_) : vertices(v_) {
            usedInThisDFS = false; stillExistsing = true;}

        ~HyperEdge() {}
        inline std::vector<Vertex*> getVertices() const {return vertices;}
        inline void print() const {
            for (const auto& v : vertices) {
                    v->print();
                    std::cout << " ";
                }
                std::cout << std::endl;
        }
        inline bool isStillExistsing() const {return stillExistsing;}
        inline void setStillExistsing(bool exists) {stillExistsing = exists;}
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
        void changeUnderlyingEdge(HyperEdge* hyperEdge_);

        inline void print() const {
            std::cout << "Head: ";
            head->print();
            std::cout << std::endl;
            std::cout << "Hyperedge: ";
            hyperEdge->print();
        }
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
        inline size_t getNumberOfVertices() const {return size;}
        inline size_t getNumberOfEdges() const {return directedHyperEdges.size();}


        inline Vertex * getVertex(int i) {return &vertices[i];}
        inline std::list<HyperEdge> * getUndirectedHyperEdges() {return &undirectedHyperEdges;}

        void addHyperEdge(const HyperEdge& edge, Vertex * head);
        void changeDirection(DirectedHyperEdge& edge, Vertex * to);


        void readFromInput();
        void print() const;
};
#endif  // GRAPH_H_
