//
// Created by vance on 2016/8/9.
//

#ifndef NATIVEIMAGEMERGE_QUEUE_H
#define NATIVEIMAGEMERGE_QUEUE_H

#include "inc.h"

template <typename T>
struct Node {
    T* pData;
    T* pHead;
    T* pTail;
    Node<T>* pPrev;
    Node<T>* pNext;
};

template <typename T>
class Queue {
private:
    Node<T>* mpHeadNode;
    Node<T>* mpTailNode;
    T* mpHead;
    T* mpTail;

    Node<T>** mpTrash;
    Node<T>** mpTrashHead;
    Node<T>** mpTrashTail;
    Node<T>** mpTrashCurrent;

public:
    Queue();
    virtual ~Queue();

    bool isEmpty();
    void inQueue(IN const T& item);
    bool outQueue(OUT T& item);

private:
    void dropToTrash(Node<T>* pNode);
    Node<T>* reuseFromTrash();
};


#endif //NATIVEIMAGEMERGE_QUEUE_H
