#ifndef MATCHING_ENGINE_HPP
#define MATCHING_ENGINE_HPP

#include "avlTree.hpp"
#include "unordered_map"

// Mathcing Engine holds two BST - Buy and Sell
class MatchingEngine {
private:
  AVLTree buyTree;
  AVLTree sellTree;
  std::unordered_map<int, Order *> orderMap;

public:
  MatchingEngine();
  ~MatchingEngine();

  // Place a new order (creates the Order, inserts in tree, tries match)
  void placeOrder(int orderId, bool isBuy, int shares, int price);

  // Cancel an existing order
  void cancelOrder(int orderId);

  // Match an incoming order with the opposite side
  void matchOrder(Order *incomingOrder);

private:
  // Helper: find existing or create a new Amount node in the correct tree
  Amount *findOrCreateAmountNode(bool isBuy, int price);

  // If an Amount node has no Orders, remove it from the tree
  void removeAmountNodeIfEmpty(Amount *amountNode, bool isBuy);
};

#endif