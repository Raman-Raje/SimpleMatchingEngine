#include "matchingEngine.hpp"
#include "myOrder.hpp"
#include <iostream>
#include <algorithm>

class MyOrder;

MatchingEngine::MatchingEngine() {}
MatchingEngine::~MatchingEngine() {}

// Helper to find or create a price node in the appropriate tree
Amount* MatchingEngine::findOrCreateAmountNode(bool isBuy, int price) {
    AVLTree& tree = (isBuy ? buyTree : sellTree);
    Amount* found = tree.search(price);
    if (found) {
        return found;
    }
    Amount* newNode = new Amount(price, isBuy);
    tree.insert(newNode);
    return newNode;
}

// Place a new order, add it to the book, then attempt to match immediately.
void MatchingEngine::placeOrder(int orderId, bool isBuy, int shares, int price) {
    // Create a concrete order
    MyOrder* newOrder = new MyOrder(orderId, isBuy, shares, price);

    // Insert it into the appropriate price node
    Amount* node = findOrCreateAmountNode(isBuy, price);
    node->append(newOrder);

    // Track in orderMap
    orderMap[orderId] = newOrder;

    // Attempt to match it right away (continuous matching)
    matchOrder(newOrder);
}

/**
 * Cancel an existing order by ID:
 * - Find in map
 * - Unlink from parent's linked list
 * - If parent's list is empty, remove the price node from the tree
 * - Remove from map, delete the order
 */
void MatchingEngine::cancelOrder(int orderId) {
    auto it = orderMap.find(orderId);
    if (it == orderMap.end()) {
        std::cout << "Order " << orderId << " not found!\n";
        return;
    }
    Order* orderToCancel = it->second;
    Amount* parent = orderToCancel->getParentAmount();

    // Unlink
    orderToCancel->cancel();

    // If that price node is now empty, remove it
    if (parent) {
        removeAmountNodeIfEmpty(parent, parent->buyOrSell);
    }

    // Remove from map
    orderMap.erase(it);

    // Deallocate
    delete orderToCancel;
}

/**
 * matchOrder():
 * - We look at the best opposite side (min from sell if we are buy, max from buy if we are sell)
 * - Check if prices cross
 * - If they do, trade up to the min(shares)
 * - Remove orders that become fully filled
 * - Stop if incomingOrder is filled or if there's no crossing price
 */
void MatchingEngine::matchOrder(Order* incomingOrder) {
    if (!incomingOrder) return;
    if (incomingOrder->getShares() <= 0) return;

    bool isBuy = incomingOrder->isBuyOrder();
    AVLTree& oppTree = (isBuy ? sellTree : buyTree);

    // Keep matching while the incoming order has shares
    while (incomingOrder->getShares() > 0) {
        // Opposite side's best price
        Amount* bestOpposite = (isBuy ? oppTree.findMin() : oppTree.findMax());
        if (!bestOpposite) {
            // No orders on the other side
            break;
        }

        // Check if they cross
        // (Buy: incoming price >= bestOpposite price -> cross)
        // (Sell: incoming price <= bestOpposite price -> cross)
        if (( isBuy && incomingOrder->getAmount() >= bestOpposite->getAmountPrice()) ||
            (!isBuy && incomingOrder->getAmount() <= bestOpposite->getAmountPrice()))
        {
            // There's a match potential
            Order* topOppOrder = bestOpposite->headOrder;
            if (!topOppOrder) {
                break; 
            }

            // How many shares can we match?
            int matchedQty = std::min(incomingOrder->getShares(), topOppOrder->getShares());

            // Reduce shares on both
            incomingOrder->reduceShares(matchedQty);
            topOppOrder->reduceShares(matchedQty);

            // If topOppOrder is fully filled:
            if (topOppOrder->getShares() == 0) {
                // Mark it executed
                topOppOrder->execute();
                int finishedId = topOppOrder->getOrderId();

                // Remove from the map
                orderMap.erase(finishedId);

                // Fully remove it from memory
                delete topOppOrder;

                // Possibly remove the now-empty Amount node
                removeAmountNodeIfEmpty(bestOpposite, bestOpposite->buyOrSell);
            }
            else {
                // Partially filled.
            }

            // If incomingOrder is now fully filled
            if (incomingOrder->getShares() == 0) {
                // We can stop matching (we do NOT delete incomingOrder here if it's a limit resting order).
                // In many real systems, if there's leftover quantity, the incoming order remains on the book.
                break;
            }
        }
        else {
            // No price cross; stop matching
            break;
        }
    }
}

/**
 * Remove the Amount node from the tree if it no longer contains any orders.
 */
void MatchingEngine::removeAmountNodeIfEmpty(Amount* amountNode, bool isBuy) {
    if (!amountNode) return;
    if (amountNode->getSize() == 0) {
        // Remove from the tree
        if (isBuy) {
            buyTree.remove(amountNode);
        } else {
            sellTree.remove(amountNode);
        }
        // NOTE: Our naive removeBST() already does "delete curr;", 
        // which calls ~Amount(), which deletes all Orders in that node.
        // So `amountNode` is now a dangling pointer here. We do NOT call `delete` again.
    }
}
