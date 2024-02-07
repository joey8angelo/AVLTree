#pragma once
#include <string>
#include <fstream>

template <typename T>
class AVLTree{
  private:
    struct Node{
        T data;
        Node* l;
        Node* r;
        Node* p;
        int h;
        Node(T data) : data(data), l(nullptr), r(nullptr), p(nullptr), h(0){}
    };
    void removeAll(AVLTree<T>::Node*);
    void rotate(AVLTree<T>::Node*);
    void rotateRight(AVLTree<T>::Node*);
    void rotateLeft(AVLTree<T>::Node*);
    void updateHeights();
    int updateHeights(AVLTree<T>::Node*);
    int balanceFactor(AVLTree<T>::Node*);

  public:
    AVLTree<T>::Node* root;
    AVLTree() : root(nullptr){}
    ~AVLTree();
    void insert(const T&);
};

template <typename T>
AVLTree<T>::~AVLTree() {
    removeAll(root);
}

template <typename T>
void AVLTree<T>::removeAll(AVLTree<T>::Node* node) {
    if (node == nullptr) {
        return;
    }
    else {
        removeAll(node->l);
        removeAll(node->r);
        delete node;
    }
}

/*
    Insert a new element into the AVL tree
*/
template <typename T>
void AVLTree<T>::insert(const T& data) {
    if (root == nullptr) { // insert as root
        root = new AVLTree<T>::Node(data);
        root->p = nullptr;
        root->h = 0;
        return;
    }
    else {
        AVLTree<T>::Node* curr = root;
        while (curr != nullptr) { // find next available opening that keeps BST rules
            if (curr->data == data) { // no duplicates
                return;
            }
            if (curr->data > data) {
                if (curr->l == nullptr) {
                    curr->l = new AVLTree<T>::Node(data);
                    curr->l->p = curr;
                    updateHeights();
                    break;
                }
                else
                    curr = curr->l;
            }
            else {
                if (curr->r == nullptr) {
                    curr->r = new AVLTree<T>::Node(data);
                    curr->r->p = curr;
                    updateHeights();
                    break;
                }
                else
                    curr = curr->r;
            }
        }
        while (curr != nullptr) {
            rotate(curr);
            curr = curr->p;
        }
    }
}

/*
    Return the balance factor of a node
*/
template <typename T>
int AVLTree<T>::balanceFactor(AVLTree<T>::Node* node) {
    int l = -1;
    int r = -1;
    if (node->l != nullptr) {
        l = node->l->h;
    }
    if (node->r != nullptr) {
        r = node->r->h;
    }
    return l - r;
}

template <typename T>
void AVLTree<T>::rotate(AVLTree<T>::Node* node) {
  int bf = balanceFactor(node); // if the balance factor is |2| then a rotate is needed
    if (bf == -2) {
        if (balanceFactor(node->r) == 1) {
            rotateRight(node->r);
        }
        rotateLeft(node);
    }
    else if (bf == 2) {
        if (balanceFactor(node->l) == -1) {
            rotateLeft(node->l);
        }
        rotateRight(node);
    }
}

template <typename T>
void AVLTree<T>::rotateRight(AVLTree<T>::Node* y) {
    AVLTree::Node* x = y->l;
    AVLTree::Node* b = x->r;
    if (y->p != nullptr) {
        if (y->p->r == y) { // make y's parent x's parent
            y->p->r = x;
            x->p = y->p;
        }
        else {
            y->p->l = x;
            x->p = y->p;
        }
    }
    else {
        root = y->l;
        root->p = nullptr;
    }
    y->p = x; // x is y's parent
    x->r = y; // x->r is y
    if (b != nullptr) {
        y->l = b; // y->l is b
        b->p = y; // y is b's parent
    }
    else{
        y->l = nullptr;
    }
    updateHeights();
}

template <typename T>
void AVLTree<T>::rotateLeft(AVLTree<T>::Node* x) {
    AVLTree::Node* y = x->r;
    AVLTree::Node* b = y->l;
    if (x->p != nullptr) {
        if (x->p->r == x) { // make x's parent y's parent
            x->p->r = y;
            y->p = x->p;
        }
        else {
            x->p->l = y;
            y->p = x->p;
        }
    }
    else {
        root = x->r;
        root->p = nullptr;
    }
    x->p = y; // y is x's parent
    y->l = x; // y->l is x
    if (b != nullptr) {
      x->r = b; // x->r is b
      b->p = x; // x is b's parent
    }
    else{
        x->r = nullptr;
    }
    updateHeights();
}

template <typename T>
void AVLTree<T>::updateHeights() {
    updateHeights(root);
}

template <typename T>
int AVLTree<T>::updateHeights(AVLTree<T>::Node* node) {
    if (node == nullptr) {
        return -1;
    }
    else {
        int l = updateHeights(node->l);
        int r = updateHeights(node->r);
        if (l > r) {
            node->h = l + 1;
            return l + 1;
        }
        else {
            node->h = r + 1;
            return r + 1;
        }
    }
}