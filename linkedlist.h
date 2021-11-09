// Copyright 2021 András Mihálykó MIT License

#pragma once

/*
    Doubly linked list template . Used for its capability
    of deleting items in O(1) running time.
     
*/

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
    ~List() {
        Node<T>* next;
        while (first != NULL) {
            next = first->getNext();
            delete first;
            first = next;
        }
    }

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
    if (nodeToDelete == first) {  // delete the first
        first = nodeToDelete->getNext();
    }

    if (nodeToDelete->getNext() != NULL)
        nodeToDelete->getNext()->setPrevious(nodeToDelete->getPrevious());

    // Change prev only if node to be deleted is NOT the first node
    if (nodeToDelete->getPrevious() != NULL)
        nodeToDelete->getPrevious()->setNext(nodeToDelete->getNext());

    delete nodeToDelete;
    numbOfData--;
}
