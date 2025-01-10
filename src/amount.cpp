#include "order.hpp"
#include "amount.hpp"


Amount::Amount(int _amountPrice, bool _buyOrSell, int _size, int _totalVolume)
    : amountPrice(_amountPrice), buyOrSell(_buyOrSell), size(_size), totalVolume(_totalVolume),
    parent(nullptr), leftChild(nullptr), rightChild(nullptr),
    headOrder(nullptr), tailOrder(nullptr) {}

Amount::~Amount() {
    // Clean up the orders in this price node
    Order* current = headOrder;
    while (current) {
        Order* tmp = current->nextOrder;
        delete current;  // Freed memory of each order
        current = tmp;
    }
}

int Amount::getAmountPrice() const
{
    return amountPrice;
}

int Amount::getSize() const
{
    return size;
}

int Amount::getTotalVolume() const
{
    return totalVolume;
}

// Add an order to the limit
void Amount::append(Order *order)
{
        if (headOrder == nullptr) {
            headOrder = tailOrder = order;
        } else {
            tailOrder->nextOrder = order;
            order->prevOrder = tailOrder;
            order->nextOrder = nullptr;
            tailOrder = order;
        }
        size += 1;
        totalVolume += order->getShares();
        order->parentAmount = this;
}