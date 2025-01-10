#include "matchingEngine.hpp"
#include <iostream>

int main() {
    MatchingEngine engine;

    std::cout << "=== Place a BUY order (ID=1, shares=100, price=50) ===\n";
    engine.placeOrder(1, true /*BUY*/, 100, 50);

    std::cout << "\n=== Place a SELL order (ID=2, shares=50, price=49) => should cross/execute ===\n";
    engine.placeOrder(2, false /*SELL*/, 50, 49);

    std::cout << "\n=== Cancel a non-existent order (ID=999) ===\n";
    engine.cancelOrder(999);

    std::cout << "\n=== Cancel the remaining part of BUY order (ID=1) if partially filled ===\n";
    engine.cancelOrder(1);

    std::cout << "\n=== Place a SELL order (ID=3, shares=100, price=60) => no cross with best BUY ===\n";
    engine.placeOrder(3, false /*SELL*/, 100, 60);

    std::cout << "\n=== Place a BUY order (ID=4, shares=50, price=65) => should match with SELL@60 partially ===\n";
    engine.placeOrder(4, true /*BUY*/, 50, 65);

    std::cout << "\nDone.\n";
    return 0;
}
