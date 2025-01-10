#pragma once
#include "amount.hpp"
#include <algorithm>


class AVLTree {
private:
    //=== AVL Node definition ===
    struct Node {
        Amount* data;  // pointer to your Amount object
        Node*   left;
        Node*   right;
        int     height;

        Node(Amount* d)
            : data(d), left(nullptr), right(nullptr), height(1)
        {}
    };

    Node* root; // root of the AVL tree

public:
    AVLTree();
    ~AVLTree();

    //=== Public interface, similar to your RBTree ===
    void    insert(Amount* data);
    void    remove(Amount* data);
    Amount* search(int price) const;
    Amount* findMin() const;
    Amount* findMax() const;

private:
    //=== Internal helpers ===
    void  clear(Node* node);

    // Basic AVL utilities
    int   getHeight(Node* node) const;
    void  updateHeight(Node* node);
    int   getBalance(Node* node) const;

    // Rotations
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);

    // Rebalance a node after insertion or removal
    Node* rebalance(Node* node);

    // BST-like insert & remove, then rebalance
    Node* insertAVL(Node* node, Amount* data);
    Node* removeAVL(Node* node, int price);

    // Search
    Node* searchAVL(Node* node, int price) const;

    // Min / Max
    Node* findMinNode(Node* node) const;
    Node* findMaxNode(Node* node) const;
};
