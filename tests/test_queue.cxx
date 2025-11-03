#include <iostream>
#include "queue.h"
#include "dyn_array.h"
#include "linked_list.h"
#include "test_util.h"

unsigned fibonacci(unsigned n)
{
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;
  return fibonacci(n-1) + fibonacci(n-2);
}

int main()
{
  std::cout << std::boolalpha;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "QUEUE TESTS" << std::endl;

  queue<unsigned> que;
  TEST_CHECK(que.is_empty(), "Queue should be initially empty");

  dyn_array<unsigned> gt;
  for (unsigned i = 0; i < 20; ++i) {
    que.enqueue(fibonacci(i));
    gt.add(fibonacci(i));
  }
  std::cout << "Enqueued 20 Fibonacci numbers: " << que << std::endl;
  TEST_EQUAL(que.size(), 20, "Queue should have 20 elements");
  TEST_EQUAL(que.is_empty(), false, "Queue should not be thinking it's empty");
  TEST_EQUAL(que.peek(), 0, "Peek should return the 1st Fibonacci number");

  std::cout << "Dequeue values until queue is empty: ";
  unsigned res[20];
  size_t ind = 0;
  while (!que.is_empty()) {
    res[ind] = que.dequeue();
    std::cout << res[ind] << ((que.is_empty()) ? "" : " ");
    ind++;
  }
  std::cout << std::endl;
  TEST_EQUAL(ind, 20, "Should have dequeued 20 elements");
  TEST_SEQ(gt, res, 20, "Dequeue order should be the insert order");
  TEST_EQUAL(que.size(), 0, "Queue should have size 0");

  std::cout << "----------------------------------------------" << std::endl;

  double basel_gt[10] = {0.01, 0.012345679, 0.015625, 0.020408163, 0.027777778,
                         0.04, 0.0625, 0.111111111, 0.25, 1};
  dyn_array<double> basel_deq;
  queue<double> basel = {1, 0.25, 0.111111111, 0.0625, 0.04, 0.027777778,
                         0.020408163, 0.015625, 0.012345679, 0.01};
  std::cout << "Constructed from initializer list:   " << basel << std::endl;
  std::cout << "Dequeuing values until queue is empty: ";
  while (!basel.is_empty()) {
    basel_deq.add(basel.dequeue());
    std::cout << basel_deq.last() << ((basel.is_empty()) ? "" : " ");
  }
  std::cout << std::endl;
  TEST_EQUAL(basel_deq.size(), 10, "Should have dequeued 10 elements");
  std::cout << basel_deq << std::endl;
  TEST_SEQ(basel_deq, basel_gt, 10, "Dequeue order should match init list");

  std::cout << "----------------------------------------------" << std::endl;

  dyn_array<unsigned> euclid_gt, eucl_pop;
  euclid_gt = {223092871, 9699691, 510511, 30031, 2311, 211, 31, 7, 3, 2};
  que = {2, 3, 7, 31, 211, 2311, 30031, 510511, 9699691, 223092871};
  std::cout << "Assigned from initializer list:        " << que << std::endl;
  std::cout << "Dequeuing values until queue is empty: ";
  while (!que.is_empty()) {
    eucl_pop.add(que.dequeue());
    std::cout << eucl_pop.last() << ((que.is_empty()) ? "" : " ");
  }
  std::cout << std::endl;
  TEST_EQUAL(eucl_pop.size(), 10, "Should have dequeued 10 elements");
  TEST_SEQ(eucl_pop, euclid_gt, 10, "Dequeue order should match init list");

  std::cout << "----------------------------------------------" << std::endl;

  que = {2, 3, 7, 31, 211, 2311, 30031, 510511, 9699691, 223092871};
  queue<unsigned> from_dyn;
  from_dyn = euclid_gt.reverse();
  std::cout << "Assigned from dyn_array: " << from_dyn << std::endl;
  TEST_CHECK(from_dyn == que, "Assignment should create same queue as before");

  linked_list<unsigned, true> ll;
  ll = {2, 3, 7, 31, 211, 2311, 30031, 510511, 9699691, 223092871};
  queue<unsigned> from_ll;
  from_ll = ll;
  std::cout << "Assigned from linked_list: " << from_ll << std::endl;
  TEST_CHECK(from_ll == que, "Assignment should create same queue as before");

  test_util::print_summary();
  return 0;
}