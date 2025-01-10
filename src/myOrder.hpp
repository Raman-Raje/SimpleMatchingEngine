#include "order.hpp"
#include "iostream"

class MyOrder : public Order {
public:
    MyOrder(int orderId, bool isBuy, int shares, int price)
        : Order(orderId, isBuy, shares, price)
    {}

    void print() const override {
        std::cout << "Order ID: " << orderId
                  << (buyOrSell ? " (BUY)" : " (SELL)")
                  << ", Shares: " << shares
                  << ", Price: " << amount << "\n";
    }
};