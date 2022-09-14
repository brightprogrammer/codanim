#ifndef STACK_H_
#define STACK_H_

#include "typedefs.h"
#include <cstdlib>
#include <cstdio>

template <typename T>
struct Node{
    Node* next;
    T data;

    Node<T>* pop(T& data);
    Node<T>* push(const T& data);

    static Node* create(const T& data);
    void destroy();
};

/**
 * @brief Create a new node
 * */
template<typename T>
Node<T>* Node<T>::create(const T& data){
    Node<T>* n = (Node<T>*)malloc(sizeof(Node<T>));
    n->next = nullptr;
    n->data = data;

    return n;
}

/**
 * @brief Destroy given node.
 *
 * @param n Node to be destroyed.
 * */
template<typename T>
void Node<T>::destroy(){
    free((void*)this);
}

/**
 * @brief Add node to top of linked list
 *
 * @param top Current top node.
 * @param next Next top element to add.
 * @return New top element.
 * */
template<typename T>
Node<T>* Node<T>::push(const T& data){
    Node<T>* new_top = Node<T>::create(data);
    new_top->next = this;
    return new_top;
}

/**
 * @brief Remove top node and return node below that.
 *
 * @param top Current top element
 * @return New top element.
 * */
template<typename T>
Node<T>* Node<T>::pop(T& pdata){
    // get data
    pdata = data;

    // remove top node
    Node<T>* new_top = (Node<T>*)this->next;
    this->destroy();

    // return new top node
    return new_top;
}

template<typename T>
void destroyStack(Node<T>* top){
    if(top == nullptr) return;

    T data;
    while(top != nullptr){
        Node<T>* temp = top;
        top = top->next;
        free(temp);
    }

    // destroy final bottom node
    free(top);
}

#endif // STACK_H_
