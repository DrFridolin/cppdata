#include <iostream>
#include "stack.h"
#include "dyn_array.h"
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
  std::cout << "STACK TESTS" << std::endl;

  stack<unsigned> stk;
  TEST_CHECK(stk.is_empty(), "Stack should be initially empty");

  dyn_array<unsigned> gt;
  for (unsigned i = 0; i < 20; ++i) {
    stk.push(fibonacci(i));
    gt.add(fibonacci(i));
  }
  gt.reverse();
  std::cout << "Pushed 20 Fibonacci numbers: " << stk << std::endl;
  TEST_EQUAL(stk.size(), 20, "Stack should have 20 elements");
  TEST_EQUAL(stk.is_empty(), false, "Stack should not be thinking it's empty");
  TEST_EQUAL(stk.peek(), 4181, "Peek should return the 20th Fibonacci number");

  std::cout << "Popping values until stack is empty: ";
  unsigned res[20];
  size_t ind = 0;
  while (!stk.is_empty()) {
    res[ind] = stk.pop();
    std::cout << res[ind] << ((stk.is_empty()) ? "" : " ");
    ind++;
  }
  std::cout << std::endl;
  TEST_EQUAL(ind, 20, "Should have popped 20 elements");
  TEST_SEQ(gt, res, 20, "Pop order should be the inverse of insert order");
  TEST_EQUAL(stk.size(), 0, "Stack should have size 0");

  std::cout << "----------------------------------------------" << std::endl;

  double basel_gt[10] = {1, 0.25, 0.111111111, 0.0625, 0.04, 0.027777778,
                         0.020408163, 0.015625, 0.012345679, 0.01};
  dyn_array<double> basel_pop;
  stack<double> basel = {1, 0.25, 0.111111111, 0.0625, 0.04, 0.027777778,
                         0.020408163, 0.015625, 0.012345679, 0.01};
  std::cout << "Constructed from initializer list:   " << basel << std::endl;
  std::cout << "Popping values until stack is empty: ";
  while (!basel.is_empty()) {
    basel_pop.add(basel.pop());
    std::cout << basel_pop.last() << ((basel.is_empty()) ? "" : " ");
  }
  std::cout << std::endl;
  TEST_EQUAL(basel_pop.size(), 10, "Should have popped 10 elements");
  TEST_SEQ(basel_pop, basel_gt, 10, "Pop order should match initializer list");

  std::cout << "----------------------------------------------" << std::endl;

  dyn_array<unsigned> euclid_gt, eucl_pop;
  euclid_gt = {2, 3, 7, 31, 211, 2311, 30031, 510511, 9699691, 223092871};
  stk = {2, 3, 7, 31, 211, 2311, 30031, 510511, 9699691, 223092871};
  std::cout << "Assigned from initializer list:      " << stk << std::endl;
  std::cout << "Popping values until stack is empty: ";
  while (!stk.is_empty()) {
    eucl_pop.add(stk.pop());
    std::cout << eucl_pop.last() << ((stk.is_empty()) ? "" : " ");
  }
  std::cout << std::endl;
  TEST_EQUAL(eucl_pop.size(), 10, "Should have popped 10 elements");
  TEST_SEQ(eucl_pop, euclid_gt, 10, "Pop order should match initializer list");

  std::cout << "----------------------------------------------" << std::endl;

  stk = {2, 3, 7, 31, 211, 2311, 30031, 510511, 9699691, 223092871};
  stack<unsigned> from_dyn;
  from_dyn = euclid_gt;
  std::cout << "Assigned from dyn_array: " << from_dyn << std::endl;
  TEST_CHECK(from_dyn == stk, "Assignment should create same queue as before");

  node<unsigned, false>* nodes = node<unsigned, false>::create(0);
  *nodes = {2, 3, 7, 31, 211, 2311, 30031, 510511, 9699691, 223092871};
  stack<unsigned> from_node;
  from_node = nodes;
  std::cout << "Assigned from linked_list: " << from_node << std::endl;
  TEST_CHECK(from_node == stk, "Assignment should create identical queue");

  test_util::print_summary();
  delete nodes;
  return 0;
}