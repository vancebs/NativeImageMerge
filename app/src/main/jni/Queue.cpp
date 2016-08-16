//
// Created by vance on 2016/8/9.
//

#include "Queue.h"

#define TRASH_SIZE 5
#define DEFAULT_UNIT_SIZE 1024

template <typename T>
Queue<T>::Queue() {
    mpHeadNode = new Node<T>;
    mpHeadNode->pData = new T[DEFAULT_UNIT_SIZE];
    mpHeadNode->pHead = mpHeadNode->pData;
    mpHeadNode->pTail = mpHeadNode->pData + DEFAULT_UNIT_SIZE - 1;
    mpHeadNode->pPrev = NULL;
    mpHeadNode->pNext = NULL;

    mpTailNode = mpHeadNode;
    mpHead = mpHeadNode->pHead;
    mpTail = mpHead;

    // init trash
    mpTrash = new Node<T>*[TRASH_SIZE];
    memset(mpTrash, 0, TRASH_SIZE * sizeof(T));
    mpTrashHead = mpTrash;
    mpTrashTail = mpTrash + TRASH_SIZE - 1;
    mpTrashCurrent = NULL;
}

template <typename T>
Queue<T>::~Queue() {
    Node<T>* p = mpHeadNode;
    while (p != NULL) {
        delete p;
        p = p->pNext;
    }
    if (mpTrash != NULL) {
        delete mpTrash;
    }
}

template <typename T>
bool Queue<T>::isEmpty() {
    return mpHead == mpTail;
}

template <typename T>
void Queue<T>::inQueue(IN const T& item) {
    // insert item
    memcpy(mpTail, &item, sizeof(T));

    // update tail pointer
    if (mpTail == mpTailNode->pTail) {
        // try to reuse a node
        Node<T>* pNode = reuseFromTrash();

        // create one if no node to reuse
        if (pNode == NULL) {
            //pNode = createNode(mUnitSize, mpTailNode, NULL);
            pNode = new Node<T>;
            pNode->pData = new T[DEFAULT_UNIT_SIZE];
            pNode->pHead = pNode->pData;
            pNode->pTail = pNode->pData + DEFAULT_UNIT_SIZE - 1;
        }

        // insert not to tail
        pNode->pPrev = mpTailNode;
        pNode->pNext = NULL;
        mpTailNode->pNext = pNode;
        mpTailNode = pNode;

        // update tail
        mpTail = mpTailNode->pHead;
    } else {
        mpTail ++;
    }
}

template <typename T>
bool Queue<T>::outQueue(OUT T& item) {
    if (isEmpty()) {
        return false;
    }

    // get item
    memcpy(&item, mpHead, sizeof(T));

    // update head pointer
    if (mpHead == mpHeadNode->pTail) {
        Node<T>* pNode = mpHeadNode;

        // remove node
        mpHeadNode = mpHeadNode->pNext;
        mpHead = mpHeadNode->pHead;
        mpHeadNode->pPrev = NULL;

        // drop to trash
        dropToTrash(pNode);
    } else {
        mpHead ++;
    }

    return true;
}

template <typename T>
void Queue<T>::dropToTrash(Node<T>* pNode) {
    if (mpTrashCurrent == NULL) {
        mpTrashCurrent = mpTrashHead;
        *mpTrashCurrent = pNode;
    } else if (mpTrashCurrent == mpTrashTail) {
        // drop it, it already full
        delete pNode;
    } else {
        mpTrashCurrent ++;
        *mpTrashCurrent = pNode;
    }
}

template <typename T>
Node<T>* Queue<T>::reuseFromTrash() {
    if (mpTrashCurrent == NULL) {
        return NULL;
    } else {
        Node<T>* p = *mpTrashCurrent;
        if (mpTrashCurrent == mpTrashHead) {
            mpTrashCurrent = NULL;
        } else {
            mpTrashCurrent --;
        }
        return p;
    }
}