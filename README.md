# Simple Matching Engine

This is a **simple limit-order book** (matching engine) in C++. It demonstrates:
- **Linked lists** of orders per price level (`Amount`)
- A **balanced BST** (AVL) for storing price levels
- A **HashMap** for fast order lookups (e.g., cancellation by ID)

## Overview

1. **Order**  
   - Represents a single buy/sell order, linked in a doubly-linked list at its price level.  
   - Supports partial fills (`reduceShares`), full execution (`execute`), and canceling (`cancel`).

2. **Amount (Price Node)**  
   - Holds all orders at a single price in a doubly-linked list.  
   - Tracks total orders (`size`) and total shares (`totalVolume`).

3. **AVLTree**  
   - Self-balancing BST keyed by `amountPrice` (the price).  
   - Provides `insert`, `remove`, `search`, `findMin` (best ask), and `findMax` (best bid).

4. **MatchingEngine**  
   - Manages two AVL trees: `buyTree` and `sellTree`.  
   - Stores `Order*` in a HashMap for O(1) average-time cancels.  
   - **Operations**:
     - **placeOrder**: inserts an order into the correct tree, then tries to match it.  
     - **cancelOrder**: finds the order in HashMap, unlinks it, removes from the tree if empty, and deletes it.  
     - **matchOrder**: continuously matches new orders with opposite best price until filled or no crossing.

## Building & Running

1. Clone or download the repository.
2. Compile (example using g++ on Linux):
   ```bash
   g++ -std=c++11 -o matching_engine \
       src/order.cpp \
       src/amount.cpp \
       src/avltree.cpp \
       src/matchingEngine.cpp \
       src/main.cpp
