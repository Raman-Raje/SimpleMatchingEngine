#include "avlTree.hpp"
#include <iostream>

//=== Constructor / Destructor
AVLTree::AVLTree()
    : root(nullptr)
{}

AVLTree::~AVLTree() {
    clear(root);
}

void AVLTree::clear(Node* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node; // calls ~Node(), which calls ~Amount() automatically?
}

//=== Public interface methods ===

void AVLTree::insert(Amount* data) {
    root = insertAVL(root, data);
}

void AVLTree::remove(Amount* data) {
    if (!data) return;
    int price = data->amountPrice;
    root = removeAVL(root, price);
}

Amount* AVLTree::search(int price) const {
    Node* res = searchAVL(root, price);
    return (res ? res->data : nullptr);
}

Amount* AVLTree::findMin() const {
    Node* minNode = findMinNode(root);
    return (minNode ? minNode->data : nullptr);
}

Amount* AVLTree::findMax() const {
    Node* maxNode = findMaxNode(root);
    return (maxNode ? maxNode->data : nullptr);
}

//=== Private / Internal Helpers ===

// Return the height of a node, or 0 if null
int AVLTree::getHeight(Node* node) const {
    return (node ? node->height : 0);
}

// Recompute the node's height based on children's heights
void AVLTree::updateHeight(Node* node) {
    if (node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
}

// Balance factor = height(left) - height(right)
int AVLTree::getBalance(Node* node) const {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

//=== Rotations ===

/**
 * Right rotation:
 *    y        x
 *   / \      / \
 *  x   T3   T1  y
 * / \          / \
 *T1 T2        T2 T3
 */
AVLTree::Node* AVLTree::rotateRight(Node* y) {
    Node* x  = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left  = T2;

    // Update heights
    updateHeight(y);
    updateHeight(x);

    // x is new root
    return x;
}

/**
 * Left rotation:
 *    x         y
 *   / \       / \
 *  T1  y     x  T3
 *     / \    / \
 *    T2 T3  T1 T2
 */
AVLTree::Node* AVLTree::rotateLeft(Node* x) {
    Node* y  = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    updateHeight(x);
    updateHeight(y);

    // y is new root
    return y;
}

//=== Rebalance after insertion or deletion
AVLTree::Node* AVLTree::rebalance(Node* node) {
    if (!node) return nullptr;

    updateHeight(node);
    int balance = getBalance(node);

    // Case 1: Left Left
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    // Case 2: Left Right
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // Case 3: Right Right
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    // Case 4: Right Left
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

//=== AVL insertion ===
AVLTree::Node* AVLTree::insertAVL(Node* node, Amount* data) {
    if (!node) {
        return new Node(data);
    }
    int price = data->amountPrice;

    if (price < node->data->amountPrice) {
        node->left = insertAVL(node->left, data);
    }
    else if (price > node->data->amountPrice) {
        node->right = insertAVL(node->right, data);
    }
    else {
        // price == node->data->amountPrice
        // In a real order book, you'd just append the Order to that existing node.
        // For simplicity, do nothing or handle duplicates as you see fit.
        // We'll do nothing here.
        return node;
    }

    // Rebalance on the way back up
    return rebalance(node);
}

//=== AVL removal ===
AVLTree::Node* AVLTree::removeAVL(Node* node, int price) {
    if (!node) return nullptr;

    if (price < node->data->amountPrice) {
        node->left = removeAVL(node->left, price);
    }
    else if (price > node->data->amountPrice) {
        node->right = removeAVL(node->right, price);
    }
    else {
        // Found the node to remove
        if (!node->left || !node->right) {
            // 0 or 1 child
            Node* temp = (node->left) ? node->left : node->right;
            // If no children, temp = nullptr
            delete node; // calls ~Amount(), which deletes Orders
            return temp;
        } else {
            // 2 children: get min in right subtree
            Node* minRight = findMinNode(node->right);
            // Copy the data from minRight to node
            node->data->amountPrice = minRight->data->amountPrice;
            node->data->buyOrSell   = minRight->data->buyOrSell;
            node->data->size        = minRight->data->size;
            node->data->totalVolume = minRight->data->totalVolume;
            node->data->headOrder   = minRight->data->headOrder;
            node->data->tailOrder   = minRight->data->tailOrder;

            // Remove the minRight node
            node->right = removeAVL(node->right, minRight->data->amountPrice);
        }
    }
    // Rebalance on the way back up
    return rebalance(node);
}

//=== Searching ===
AVLTree::Node* AVLTree::searchAVL(Node* node, int price) const {
    if (!node) return nullptr;
    if (price == node->data->amountPrice) {
        return node;
    }
    else if (price < node->data->amountPrice) {
        return searchAVL(node->left, price);
    }
    else {
        return searchAVL(node->right, price);
    }
}

//=== Finding min/max node ===
AVLTree::Node* AVLTree::findMinNode(Node* node) const {
    if (!node) return nullptr;
    while (node->left) {
        node = node->left;
    }
    return node;
}

AVLTree::Node* AVLTree::findMaxNode(Node* node) const {
    if (!node) return nullptr;
    while (node->right) {
        node = node->right;
    }
    return node;
}
