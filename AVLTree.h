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
    void updateHeights(AVLTree<T>::Node*);
    int balanceFactor(AVLTree<T>::Node*);
    AVLTree<T>::Node* find(const T&);

  public:
    AVLTree<T>::Node* root;
    AVLTree() : root(nullptr){}
    ~AVLTree();
    void insert(const T&);
    bool has(const T&);
    void remove(const T&);
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

template <typename T>
bool AVLTree<T>::has(const T& data) {
    return find(data) != nullptr;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::find(const T& data) {
    AVLTree<T>::Node* curr = root;
    while(curr != nullptr) {
        if(curr->data == data)
            return curr;
        else if(curr->data > data)
            curr = curr->l;
        else
            curr = curr->r;
    }
    return nullptr;
}

template <typename T>
void AVLTree<T>::remove(const T& data) {
    AVLTree<T>::Node* curr = find(data);
    if(curr == nullptr)
        return;
    // node has no children
    if(curr->l == nullptr && curr->r == nullptr){
        if(curr->p == nullptr)
            root = nullptr;
        else if(curr->p->l == curr)
            curr->p->l = nullptr;
        else
            curr->p->r = nullptr;
    }
    // node has right child
    else if(curr->l == nullptr){
        if(curr->p == nullptr){
            root = curr->r;
            root->p = nullptr;
        }
        else if(curr->p->l == curr){
            curr->p->l = curr->r;
            curr->r->p = curr->p;
        }
        else{
            curr->p->r = curr->r;
            curr->r->p = curr->p;
        }
    }
    // node has left child
    else if(curr->r == nullptr){
        if(curr->p == nullptr){
            root = curr->l;
            root->p = nullptr;
        }
        else if(curr->p->l == curr){
            curr->p->l = curr->l;
            curr->l->p = curr->p;
        }
        else{
            curr->p->r = curr->l;
            curr->l->p = curr->p;
        }
    }
    // node has two children
    else{
        // swap curr with the smallest value in the right subtree
        AVLTree<T>::Node* temp = curr->r;
        while(temp->l != nullptr)
            temp = temp->l;
        curr->data = temp->data;
        if(temp->p->l == temp)
            temp->p->l = temp->r;
        else 
            temp->p->r = temp->r;
        curr = temp;
    }
    // update heights in path from curr to root
    updateHeights(curr->p);
    // rotate in path from curr to root
    AVLTree<T>::Node* temp = curr;
    while(temp != nullptr){
        rotate(temp);
        temp = temp->p;
    }
    delete curr;
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
                    updateHeights(curr->l);
                    break;
                }
                else
                    curr = curr->l;
            }
            else {
                if (curr->r == nullptr) {
                    curr->r = new AVLTree<T>::Node(data);
                    curr->r->p = curr;
                    updateHeights(curr->r);
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
    if(node == nullptr) return;
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
    updateHeights(y);
    updateHeights(x->l);
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
    updateHeights(x);
    updateHeights(y->r);
}

template <typename T>
void AVLTree<T>::updateHeights(AVLTree<T>::Node* node) {
    if (node == nullptr) {
        return;
    }
    else {
        node->h = 1 + std::max((node->l == nullptr ? -1 : node->l->h), (node->r == nullptr ? -1 : node->r->h));
        updateHeights(node->p);
    }
}