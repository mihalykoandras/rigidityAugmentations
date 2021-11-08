#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <list>
#include <iostream>
#include <map>

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
        void print() const {
            std::cout << epi << " " << epj <<std::endl;
        }
};

class SimpleGraph {
// the very simple basic graph implementation, nodes indexed by ints
 private:
    int numberOfVertices;
    std::vector<Edge> edgeList;
    std::map<int, std::vector<int> > neighborLists;

 public:
    SimpleGraph() {}
    explicit SimpleGraph(int n) : numberOfVertices(n) {
        neighborLists = std::map<int, std::vector<int> >();}
    SimpleGraph(int n, const std::vector<int>& I, const std::vector<int>& J) {
        numberOfVertices = n;
        neighborLists = std::map<int, std::vector<int> >();

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
    std::vector<int> getNeighbors(int i) {return neighborLists[i];}

    void readFromInput();
};


class HyperEdge;
class DirectedHyperEdge;


class Vertex {
    /* 
    Directed hypergraph vertex. Keeps the list of hyperedges it is head of.
     */
 private:
        int id;
        int inDegree;
        List <DirectedHyperEdge*> headOfHyperEdge;

 public:
        Vertex() {}
        explicit Vertex(int id_):id(id_) {
            inDegree = 0;
        }

        ~Vertex() {}

        inline int getId() const {return id;}

        inline bool operator==(const Vertex &v) const {return id == v.getId();}
        inline bool operator<(const Vertex &v) const {return id < v.getId();}
        inline bool operator>(const Vertex &v) const {return id > v.getId();}

        inline int getInDegree() const {return inDegree;}
        inline void decreaseInDegree() {
            if (inDegree > 0)
                inDegree--;  // TODO else exception
            }
        inline void increaseInDegree() {inDegree++;}
        inline List<DirectedHyperEdge*>* isHeadOf() {return &headOfHyperEdge;}
        inline void print() const {
                std::cout << id;
        }
};

class HyperEdge {
 protected:
        std::vector<Vertex*> vertices;
        bool stillExistsing;  // we don't delete the hyperedges that are now part of a bigger M-comp,
                              // just note that they are not useful any more

 public:
        explicit HyperEdge(const std::vector<Vertex*>& v_) : vertices(v_) { stillExistsing = true;}

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
        Class for directed hypergraph. Every hyperedge has one head and multiple tail.
        It does not implement any funcionality over the basics.
        Funconalities regarding (k,l)-sparsity concern the M_compHyperGraph class
    */
 protected:
        std::map<int, Vertex*> vertices;  // key is the ID of the vertex
        std::list<DirectedHyperEdge> directedHyperEdges;
        // this does not need to be pointer, as it contains just two pointers itself
        std::list<HyperEdge*> undirectedHyperEdges;
        size_t size;

 public:
        DirectedHyperGraph() {}
        explicit DirectedHyperGraph(size_t n) {
            size = n;
            for (size_t i = 0; i < size; i++) {
                insertNewVertex(i);
            }
        }

        explicit DirectedHyperGraph(const SimpleGraph& G) {
            size = G.getNumberOfNodes();

            std::vector<Edge> graphEdges = G.getEdges();

            for (const auto& e : graphEdges) {
                std::vector<int> edge = e.getEdge();
                insertNewVertex(edge[0]);
                insertNewVertex(edge[1]);
                addDirEdge(vertices[edge[0]], vertices[edge[1]]);
            }
        }

        DirectedHyperGraph(const DirectedHyperGraph& HG) {
            vertices = HG.vertices;
            directedHyperEdges = HG.directedHyperEdges;
            undirectedHyperEdges = HG.undirectedHyperEdges;
            size = HG.size;
        }

        ~DirectedHyperGraph() {
            vertices.clear();
            undirectedHyperEdges.clear();
        }

        void inline insertNewVertex(int id) {  // insert new vertex only if id is not contained
            if (vertices.find(id) == vertices.end()) {
                    vertices[id] = new Vertex(id);
                }
        }

        inline size_t getNumberOfVertices() const {return size;}
        inline size_t getNumberOfEdges() const {return directedHyperEdges.size();}


        inline Vertex * getVertex(int i) {return vertices[i];}
        inline std::list<HyperEdge*>* getUndirectedHyperEdges() {return &undirectedHyperEdges;}

        void addHyperEdge(HyperEdge* edge, Vertex * head);
        void addDirEdge(Vertex * head, Vertex * tail);
        void changeDirection(DirectedHyperEdge& edge, Vertex * to);

        void readFromInput();
        void print() const;
};
#endif  // GRAPH_H_
