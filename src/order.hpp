#ifndef ORDER_HPP
#define ORDER_HPP


class Amount;

class Order {
protected:
  int orderId;
  int shares;
  int amount;
  bool buyOrSell; // True - Buy , False - Sell
  Order *nextOrder;
  Order *prevOrder;
  Amount *parentAmount;

public:
  Order(int _orderId, bool _buyOrSell, int _shares, int _amount);

  virtual ~Order() = default;

  int getShares() const;
  int getOrderId() const;
  int getAmount() const;
  bool isBuyOrder() const;
  void reduceShares(int qty);
  Amount *getParentAmount() const;

  void cancel();
  void execute();

  virtual void print() const = 0; // Make it a pure virtual function

  friend class Amount;
};

#endif