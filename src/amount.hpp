#ifndef AMOUNT_HPP
#define AMOUNT_HPP

#include "order.hpp"

class Amount {
public:
  int amountPrice;
  int size;
  int totalVolume;
  bool buyOrSell;
  Amount *parent;
  Amount *leftChild;
  Amount *rightChild;

  // order linked lists
  Order *headOrder;
  Order *tailOrder;

public:
  Amount(int _amountPrice, bool _buyOrSell, int _size = 0,
         int _totalVolume = 0);
  ~Amount();

  int getAmountPrice() const;
  int getSize() const;
  int getTotalVolume() const;
  bool getBuyOrSell() const;

  void append(Order *_order);
};

#endif