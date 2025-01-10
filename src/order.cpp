#include "order.hpp"
#include "amount.hpp"

Order::Order(int _orderId, bool _buyOrSell, int _shares, int _amount)
    :orderId(_orderId), buyOrSell(_buyOrSell), shares(_shares), amount(_amount),
    nextOrder(nullptr), prevOrder(nullptr), parentAmount(nullptr) {}

int Order::getShares() const
{
    return shares;
}

int Order::getOrderId() const
{
    return orderId;
}

int Order::getAmount() const
{
    return amount;
}

bool Order::isBuyOrder() const {
    return buyOrSell;
}

Amount* Order::getParentAmount() const {
    return parentAmount;
}

void Order::reduceShares(int qty) {
    shares -= qty;
    if (shares < 0) {
        shares = 0;
    }
}

// Remove order from its parent amount
void Order::cancel()
{
    // If the order is head of list
    if (prevOrder == nullptr)
    {
        parentAmount->headOrder = nextOrder;
    } else
    {
        prevOrder->nextOrder = nextOrder;
    }
    // if order is tail of list
    if (nextOrder == nullptr)
    {
        parentAmount->tailOrder = prevOrder;
    } else
    {
        nextOrder->prevOrder = prevOrder;
    }

    // update parent stats
    parentAmount->totalVolume -= shares;
    parentAmount->size -= 1;
}

// Execute head order
void Order::execute()
{
    parentAmount->headOrder = nextOrder;
    if (nextOrder == nullptr)
    {
        parentAmount->tailOrder = nullptr;
    } else
    {
        nextOrder->prevOrder = nullptr;
    }
    nextOrder = nullptr;
    prevOrder = nullptr;

    parentAmount->totalVolume -= shares;
    parentAmount->size -= 1;
}
