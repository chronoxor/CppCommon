//
// Created by Ivan Shynkarenka on 20.06.2017
//

#include "benchmark/cppbenchmark.h"

#include "memory/allocator_pool.h"

using namespace CppCommon;

enum Side
{
    BUY,
    SELL
};

struct

struct Order
{
    int id;
    int price;
    int volume;
    Order* prev;
    Order* next;

};

Order
  int idNumber;
  bool buyOrSell;
  int shares;
  int limit;
  int entryTime;
  int eventTime;
  Order *nextOrder;
  Order *prevOrder;
  Limit *parentLimit;

Limit  // representing a single limit price
  int limitPrice;
  int size;
  int totalVolume;
  Limit *parent;
  Limit *leftChild;
  Limit *rightChild;
  Order *headOrder;
  Order *tailOrder;

Book
  Limit *buyTree;
  Limit *sellTree;
  Limit *lowestSell;
  Limit *highestBuy;

BENCHMARK_MAIN()
