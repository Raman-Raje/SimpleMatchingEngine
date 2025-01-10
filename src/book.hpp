#ifndef BOOK_HPP
#define BOOK_HPP

#include "amount.hpp"
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class Amount;
class Order;

class Book {
private:
  Amount *buyTree;
  Amount *sellTree;

  std::unordered_map<int, Order *> orderMap;
  std::unordered_map<int, Amount *> stopMap;

  void addAmount(int limitPrice, bool buyOrSell);
  void deleteAmount(Amount *limit);
  void deleteFromOrderMap(int orderId);
  void deleteFromStopMap(int StopPrice);
  void marketOrderHelper(int orderId, bool buyOrSell, int shares);

public:
  Book();
  ~Book();

  // Counts used in order book perforamce visualisations
  int executedOrdersCount = 0;
  int AVLTreeBalanceCount = 0;

  // Getter and setter functions
  Amount *getBuyTree() const;
  Amount *getSellTree() const;
  Amount *getLowestSell() const;
  Amount *getHighestBuy() const;

  void marketOrder(int orderId, bool buyOrSell, int shares);
  void addAmountOrder(int orderId, bool buyOrSell, int shares, int limitPrice);
  void cancelAmountOrder(int orderId);

  void printAmount(int limitPrice, bool buyOrSell) const;
  void printOrder(int orderId) const;
};

#endif