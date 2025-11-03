#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "linked_list.h"
#include "test_util.h"

double factorial(size_t n)
{
  if (n < 2)
    return 1.0;
  return n*factorial(n-1);
}

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "SINGLY-LINKED NODE TESTS" << std::endl << std::endl;

  node<int, false>* head_s = node<int, false>::create(2);
  std::cout << "Linked list initialized with the value 2" << std::endl;
  TEST_EQUAL(head_s->val(), 2, "Head node value should be 2");

  head_s->append(4)->append(8);
  int gt1[3] = {2, 4, 8};
  std::cout << "Appended values 4 and 8 to tail: " << head_s << std::endl;
  TEST_EQUAL(head_s->size(), 3, "Linked list should have size 3");
  TEST_NODE_SEQ(*head_s, gt1, 3, "Linked list should have expected sequence");

  head_s = head_s->prepend(0)->prepend(-2);
  int gt2[5] = {-2, 0, 2, 4, 8};
  std::cout << "Appended values 0 and -2 to head: " << head_s << std::endl;
  TEST_EQUAL(head_s->size(), 5, "Linked list should have size 5");
  TEST_NODE_SEQ(*head_s, gt2, 5, "Linked list should have expected sequence");

  head_s = head_s->reverse();
  int gt2r[5] = {8, 4, 2, 0, -2};
  int min_s, max_s;
  std::cout << "Reversed linked list: " << head_s << std::endl;
  TEST_EQUAL(head_s->size(), 5, "Linked list size should be the same");
  TEST_NODE_SEQ(*head_s, gt2r, 5, "Linked list should have reversed sequence");
  TEST_EQUAL(head_s->min(min_s)->val(), 0, "Min should return preceding node");
  TEST_EQUAL(head_s->max(max_s), nullptr, "Max should return preceding node");
  TEST_EQUAL(min_s, -2, "Min value found should be -2");
  TEST_EQUAL(max_s, 8, "Max value found should be 8");
  TEST_CHECK(head_s->contains(4), "Linked list should have node with value 4");
  TEST_EQUAL(head_s->contains(12), 0, "Linked list shouldn't have value 12");
  TEST_EQUAL(head_s->contains(8), 0, "Nullptr expected from contains on head");

  head_s = head_s->delete_node(4);
  int gt3[4] = {8, 2, 0, -2};
  std::cout << "Deleted 4 by value: " << head_s << std::endl;
  TEST_EQUAL(head_s->size(), 4, "Linked list should have size 4");
  TEST_NODE_SEQ(*head_s, gt3, 4, "Linked list should have expected sequence");

  head_s->delete_tail();
  int gtdt[4] = {8, 2, 0};
  std::cout << "Deleted tail: " << head_s << std::endl;
  TEST_EQUAL(head_s->size(), 3, "Linked list should now have size 3");
  TEST_NODE_SEQ(*head_s, gtdt, 3, "Linked list should have expected sequence");

  node<int, false>* semiprimes = node<int, false>::create(0);
  *semiprimes = {4, 6, 9, 10, 14, 15, 21, 22, 25, 26};
  int gtsp[10] = {4, 6, 9, 10, 14, 15, 21, 22, 25, 26};
  std::cout << "Assigned semiprimes from initializer list to linked list:";
  std::cout << semiprimes << std::endl;
  TEST_EQUAL(semiprimes->size(), 10, "Linked list should have size 10");
  TEST_NODE_SEQ(*semiprimes, gtsp, 10, "Linked list seq should match array");
  node<int, false>* node_21 = semiprimes->search(21);
  TEST_EQUAL(node_21->val(), 21, "Search for 21 should find node with val 21");
  node<int, false>* node_15 = semiprimes->contains(node_21);
  TEST_CHECK(node_15, "Linked list should contain node_21");
  TEST_EQUAL(node_15->val(), 15, "Preceding node is expected from contains");
  TEST_EQUAL(head_s->contains(node_21), 0, "Old chain shouldn't have node_21");

  *head_s = *semiprimes;
  std::cout << "Assigned semiprimes to head_s node: " << head_s << std::endl;
  TEST_EQUAL(head_s->size(), 10, "Size of head_s should now be 10");
  TEST_NODE_SEQ(*head_s, gtsp, 10, "Sequence of head_s should be the same");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "DOUBLY-LINKED NODE TESTS" << std::endl << std::endl;

  node<float, true>* head_d = node<float, true>::create(2);
  std::cout << "Linked list initialized with the value 2" << std::endl;
  TEST_EQUAL(head_d->val(), 2, "Head node value should be 2");

  head_d->append(4)->append(8);
  std::cout << "Appended values 4 and 8 to tail: " << head_d << std::endl;
  TEST_EQUAL(head_d->size(), 3, "Linked list should have size 3");
  TEST_NODE_SEQ(*head_d, gt1, 3, "Linked list should have expected seq");

  std::cout << "Mid: " << head_d->next()->next()->prev()->val() << std::endl;
  TEST_EQUAL(head_d->next()->next()->prev()->val(), 4, "Mid val should be 4");

  head_d = head_d->prepend(0)->prepend(0.5);
  head_d = head_d->prepend(0.25)->prepend(0.125);
  float gt4[7] = {0.125, 0.25, 0.5, 0, 2, 4, 8};
  float min_d, max_d;
  std::cout << "In this order, chain-appended valuess 0, 0.5, 0.25 and 0.125 ";
  std::cout << "to head: " << head_d << std::endl;
  node<float, true>* found = head_d->next()->next()->next()->find_head();
  TEST_EQUAL(head_d->size(), 7, "Linked list should have size 7");
  TEST_NODE_SEQ(*head_d, gt4, 7, "Linked list should have expected sequence");
  TEST_CHECK(found == head_d, "Actual head should be returned by find_head");
  TEST_EQUAL(head_d->max(max_d)->val(), 4, "Max should return preceding node");
  TEST_EQUAL(max_d, 8, "Max value found should be 8");
  TEST_EQUAL(head_d->max(max_d)->min(min_d)->val(), 0.5, "Prec. node is 0.5");
  TEST_EQUAL(min_d, 0, "Min value found should be 0");
  found = head_d->find_tail();
  TEST_EQUAL(found->contains(head_d->next()), head_d, "Should return head");

  float mid = head_d->next()->next()->next()->next()->prev()->val();
  std::cout << "Middle node value: " << mid << std::endl;
  TEST_EQUAL(mid, 0, "Middle node's value should be 0");
  std::cout << "Size from next: " << head_d->next()->size() << std::endl;
  TEST_EQUAL(head_d->next()->size(), 7, "Size from next node should be same");
  std::cout << "Searched 4 | Found: " << head_d->search(4)->val() << std::endl;
  TEST_EQUAL(head_d->search(4)->val(), 4, "Should find node with val 4");

  node<float, true>* head_clone = head_d->next()->next()->next()->clone();
  std::cout << "Cloned list from node 0: " << head_clone << std::endl;
  TEST_EQUAL(head_clone->size(), 7, "Cloned list should also have size 7");
  TEST_NODE_SEQ(*head_clone, gt4, 7, "Cloned list should be identical");

  head_clone = head_clone->next()->next()->prepend(0.0625);
  float gtc1[8] = {0.0625, 0.125, 0.25, 0.5, 0, 2, 4, 8};
  std::cout << "Appended 0.0625 to head of clone from node 0.5: ";
  std::cout << head_clone << std::endl;
  TEST_EQUAL(head_clone->size(), 8, "Cloned list should now have size 8");
  TEST_NODE_SEQ(*head_clone, gtc1, 8, "Cloned list should match ground truth");

  float gtsw1[8] = {0.0625, 0.125, 0.25, 0.5, 4, 2, 0, 8};
  float gtsw2[8] = {0.0625, 0.125, 0.25, 4, 0.5, 2, 0, 8};
  float gtsw3[8] = {0.0625, 0.125, 0.25, 4, 0.5, 8, 0, 2};
  float gtsw4[8] = {0.0625, 0.125, 0.25, 4, 0.5, 0, 8, 2};
  float gtsw5[8] = {2, 0.125, 0.25, 4, 0.5, 0, 8, 0.0625};
  head_clone->search(0)->swap(head_clone->search(4));
  std::cout << "Swapped nodes 0 and 4: " << head_clone << std::endl;
  TEST_EQUAL(head_clone->size(), 8, "Cloned list should still have size 8");
  TEST_NODE_SEQ(*head_clone, gtsw1, 8, "Cloned list should reflect the swap");
  head_clone->search(4)->swap(head_clone->search(0.5));
  std::cout << "Swapped nodes 4 and 0.5: " << head_clone << std::endl;
  TEST_EQUAL(head_clone->size(), 8, "Cloned list should still have size 8");
  TEST_NODE_SEQ(*head_clone, gtsw2, 8, "Cloned list should reflect the swap");
  head_clone->search(2)->swap(head_clone->search(8));
  std::cout << "Swapped nodes 2 and 8: " << head_clone << std::endl;
  TEST_EQUAL(head_clone->size(), 8, "Cloned list should still have size 8");
  TEST_NODE_SEQ(*head_clone, gtsw3, 8, "Cloned list should reflect the swap");
  head_clone->search(8)->swap(head_clone->search(0));
  std::cout << "Swapped nodes 8 and 0: " << head_clone << std::endl;
  TEST_EQUAL(head_clone->size(), 8, "Cloned list should still have size 8");
  TEST_NODE_SEQ(*head_clone, gtsw4, 8, "Cloned list should reflect the swap");
  head_clone->swap(head_clone->search(2));
  std::cout << "Swapped nodes 0.0625 and 2: " << head_clone << std::endl;
  TEST_EQUAL(head_clone->size(), 8, "Cloned list should still have size 8");
  head_clone = head_clone->find_head();
  TEST_NODE_SEQ(*head_clone, gtsw5, 8, "Cloned list should reflect the swap");
  head_clone->search(0.0625)->swap(head_clone);
  std::cout << "Swapped nodes 2 and 0.0625: " << head_clone << std::endl;
  TEST_EQUAL(head_clone->size(), 8, "Cloned list should still have size 8");
  head_clone = head_clone->find_head();
  TEST_NODE_SEQ(*head_clone, gtsw4, 8, "Cloned list should reflect the swap");

  node<float, true>* back_clone = head_clone->next()->next()->clone_bwd();
  float gtc2[3] = {0.0625, 0.125, 0.25};
  std::cout << "Cloned the new clone backwards from node 0.25: ";
  std::cout << back_clone << std::endl;
  back_clone = back_clone->prev()->prev();
  TEST_EQUAL(back_clone->size(), 3, "Cloned list should now have size 3");
  TEST_NODE_SEQ(*back_clone, gtc2, 3, "Cloned list should match ground truth");
  TEST_EQUAL(head_d->contains(back_clone), 0, "List shouldn't contain clone");

  float result;
  head_d = head_d->reverse();
  float gt4r[7] = {8, 4, 2, 0, 0.5, 0.25, 0.125};
  std::cout << "Reversed original linked list from before all cloning: ";
  std::cout << head_d << std::endl;
  TEST_EQUAL(head_d->size(), 7, "Linked list size should be the same");
  TEST_NODE_SEQ(*head_d, gt4r, 7, "Linked list should have reversed sequence");
  std::cout << "Searched 4 from 0.25 | Found: ";
  std::cout << (result = head_d->search(0.25)->search(4)->val()) << std::endl;
  TEST_EQUAL(result, 4, "Search from 0.25 should still find node with val 4");
  TEST_EQUAL(head_d->find_tail()->val(), 0.125, "Tail value should be 0.125");

  head_d = head_d->delete_head();
  float gtdh1[6] = {4, 2, 0, 0.5, 0.25, 0.125};
  std::cout << "Deleted head from 8: " << head_d << std::endl;
  TEST_EQUAL(head_d->size(), 6, "Linked list should have size 6");
  TEST_NODE_SEQ(*head_d, gtdh1, 6, "Only head node should be removed");

  head_d = head_d->search(0.25)->delete_head();
  float gtdh2[5] = {2, 0, 0.5, 0.25, 0.125};
  std::cout << "Deleted head from 0.25: " << head_d << std::endl;
  TEST_EQUAL(head_d->size(), 5, "Linked list should have size 5");
  TEST_NODE_SEQ(*head_d, gtdh2, 5, "Actual head node should be removed");

  mid = head_d->next()->next()->next()->prev()->val();
  std::cout << "Middle node value: " << mid << std::endl;
  TEST_EQUAL(mid, 0.5, "Middle node's value should be 0.5 after deletions");

  head_d = head_d->delete_node(0)->delete_node(0.125);
  float gt5[3] = {2, 0.5, 0.25};
  std::cout << "Deleted 0.125 and 0 nodes by value: " << head_d << std::endl;
  TEST_EQUAL(head_d->size(), 3, "Linked list size should be 3 after deletes");
  TEST_NODE_SEQ(*head_d, gt5, 3, "Linked list should have expected sequence");

  mid = head_d->next()->next()->prev()->val();
  std::cout << "Middle node value: " << mid << std::endl;
  TEST_EQUAL(mid, 0.5, "Middle node's value should be 0.5 after deletions");

  head_d = head_d->delete_node(0.25);
  head_d = head_d->delete_node(0.5);
  head_d = head_d->delete_node(2);
  std::cout << "Deleted all 3 nodes: " << head_d << std::endl;
  TEST_EQUAL(head_d, nullptr, "Linked list should be empty after deletions");

  node<unsigned, true>* markov = node<unsigned, true>::create(0);
  *markov = {1, 2, 5, 13, 29, 34, 89, 169, 194};
  unsigned gtmr[9] = {1, 2, 5, 13, 29, 34, 89, 169, 194};
  std::cout << "Assigned Markov numbers from initializer list to linked list:";
  std::cout << markov << std::endl;
  TEST_EQUAL(markov->size(), 9, "Linked list should have size 9");
  TEST_NODE_SEQ(*markov, gtmr, 9, "Linked list sequence should match array");
  TEST_CHECK(markov->contains(34), "34 is a Markov number");
  TEST_EQUAL(markov->contains(31), 0, "31 is not a Markov number");

  delete head_s;
  delete head_d;
  delete head_clone;
  delete back_clone;
  delete semiprimes;
  delete markov;

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "NODE CHAIN SORTING TESTS" << std::endl << std::endl;
  std::cout << std::fixed << std::setprecision(15);

  node<double, false> *ns_bef, *ns_aft;
  node<double, true> *nd_bef, *nd_aft;
  node<double, false>* e_taylor = node<double, false>::create(0);
  node<double, true>* e_maclaurin = node<double, true>::create(0);

  dyn_array<double> e_terms(0, 18);
  for (size_t i = 0; i < e_terms.size(); ++i)
    e_terms[i] = 1 / factorial(i);

  std::cout << "Shuffled Taylor series of e as singly-linked list input: ";
  std::cout << std::endl << &(*e_taylor = e_terms.shuffle()) << std::endl;
  ns_bef = e_taylor->search(1.0);
  std::cout << "Ran bubble_sort in decreasing order: " << std::endl;
  std::cout << (e_taylor = e_taylor->bubble_sort(false)) << std::endl;
  ns_aft = e_taylor->search(1.0);
  e_terms.bubble_sort(false);
  TEST_NODE_SEQ(*e_taylor, e_terms, 18, "Should match dyn_array result");
  TEST_EQUAL(ns_bef->val(), 1.0, "Pre-sort search should find value 1.0");
  TEST_EQUAL(ns_aft->val(), 1.0, "Post-sort search should find value 1.0");
  TEST_CHECK(ns_bef == ns_aft, "Node chain bubble sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled Taylor series of e as doubly-linked list input: ";
  std::cout << std::endl << &(*e_maclaurin = e_terms.shuffle()) << std::endl;
  nd_bef = e_maclaurin->search(1.0);
  std::cout << "Mid-chain bubble_sort in increasing order: " << std::endl;
  e_maclaurin = e_maclaurin->next()->next()->next()->bubble_sort();
  std::cout << e_maclaurin << std::endl;
  nd_aft = e_maclaurin->search(1.0);
  e_terms.bubble_sort();
  TEST_NODE_SEQ(*e_maclaurin, e_terms, 18, "Should match dyn_array result");
  TEST_EQUAL(nd_bef->val(), 1.0, "Pre-sort search should find value 1.0");
  TEST_EQUAL(nd_aft->val(), 1.0, "Post-sort search should find value 1.0");
  TEST_CHECK(nd_bef == nd_aft, "Node chain bubble sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled Taylor series of e as singly-linked list input: ";
  std::cout << std::endl << &(*e_taylor = e_terms.shuffle()) << std::endl;
  ns_bef = e_taylor->search(1.0);
  std::cout << "Ran insertion_sort in increasing order: " << std::endl;
  std::cout << (e_taylor = e_taylor->insertion_sort()) << std::endl;
  ns_aft = e_taylor->search(1.0);
  e_terms.insertion_sort();
  TEST_NODE_SEQ(*e_taylor, e_terms, 18, "Should match dyn_array result");
  TEST_EQUAL(ns_bef->val(), 1.0, "Pre-sort search should find value 1.0");
  TEST_EQUAL(ns_aft->val(), 1.0, "Post-sort search should find value 1.0");
  TEST_CHECK(ns_bef == ns_aft, "Node chain insertion sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled Taylor series of e as doubly-linked list input: ";
  std::cout << std::endl << &(*e_maclaurin = e_terms.shuffle()) << std::endl;
  nd_bef = e_maclaurin->search(1.0);
  std::cout << "Mid-chain insertion_sort in decreasing order: " << std::endl;
  e_maclaurin = e_maclaurin->next()->prev()->next()->insertion_sort(false);
  std::cout << e_maclaurin << std::endl;
  nd_aft = e_maclaurin->search(1.0);
  e_terms.insertion_sort(false);
  TEST_NODE_SEQ(*e_maclaurin, e_terms, 18, "Should match dyn_array result");
  TEST_EQUAL(nd_bef->val(), 1.0, "Pre-sort search should find value 1.0");
  TEST_EQUAL(nd_aft->val(), 1.0, "Post-sort search should find value 1.0");
  TEST_CHECK(nd_bef == nd_aft, "Node chain insertion sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled Taylor series of e as singly-linked list input: ";
  std::cout << std::endl << &(*e_taylor = e_terms.shuffle()) << std::endl;
  ns_bef = e_taylor->search(1.0);
  std::cout << "Ran merge_sort in increasing order: " << std::endl;
  std::cout << (e_taylor = e_taylor->merge_sort()) << std::endl;
  ns_aft = e_taylor->search(1.0);
  e_terms.merge_sort();
  TEST_NODE_SEQ(*e_taylor, e_terms, 18, "Should match dyn_array result");
  TEST_EQUAL(ns_bef->val(), 1.0, "Pre-sort search should find value 1.0");
  TEST_EQUAL(ns_aft->val(), 1.0, "Post-sort search should find value 1.0");
  TEST_CHECK(ns_bef == ns_aft, "Node chain merge sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled Taylor series of e as singly-linked list input: ";
  std::cout << std::endl << &(*e_maclaurin = e_terms.shuffle()) << std::endl;
  nd_bef = e_maclaurin->search(1.0);
  std::cout << "Mid-chain merge_sort in decreasing order: " << std::endl;
  e_maclaurin = e_maclaurin->next()->next()->next()->next()->merge_sort(false);
  std::cout << e_maclaurin << std::endl;
  nd_aft = e_maclaurin->search(1.0);
  e_terms.merge_sort(false);
  TEST_NODE_SEQ(*e_maclaurin, e_terms, 18, "Should match dyn_array result");
  TEST_EQUAL(nd_bef->val(), 1.0, "Pre-sort search should find value 1.0");
  TEST_EQUAL(nd_aft->val(), 1.0, "Post-sort search should find value 1.0");
  TEST_CHECK(nd_bef == nd_aft, "Node chain merge sort should be stable");

  std::cout << "Shuffled Taylor series of e as singly-linked list input: ";
  std::cout << std::endl << &(*e_taylor = e_terms.shuffle()) << std::endl;
  ns_bef = e_taylor->search(1.0);
  std::cout << "Ran selection_sort in decreasing order: " << std::endl;
  std::cout << (e_taylor = e_taylor->selection_sort(false)) << std::endl;
  ns_aft = e_taylor->search(1.0);
  e_terms.selection_sort(false);
  TEST_NODE_SEQ(*e_taylor, e_terms, 18, "Should match dyn_array result");
  TEST_EQUAL(ns_bef->val(), 1.0, "Pre-sort search should find value 1.0");
  TEST_EQUAL(ns_aft->val(), 1.0, "Post-sort search should find value 1.0");
  TEST_CHECK(ns_bef == ns_aft, "Node chain selection sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled Taylor series of e as singly-linked list input: ";
  std::cout << std::endl << &(*e_maclaurin = e_terms.shuffle()) << std::endl;
  nd_bef = e_maclaurin->search(1.0);
  std::cout << "Mid-chain selection_sort in increasing order: " << std::endl;
  e_maclaurin = e_maclaurin->next()->next()->next()->next()->selection_sort();
  std::cout << e_maclaurin << std::endl;
  nd_aft = e_maclaurin->search(1.0);
  e_terms.selection_sort();
  TEST_NODE_SEQ(*e_maclaurin, e_terms, 18, "Should match dyn_array result");
  TEST_EQUAL(nd_bef->val(), 1.0, "Pre-sort search should find value 1.0");
  TEST_EQUAL(nd_aft->val(), 1.0, "Post-sort search should find value 1.0");
  TEST_CHECK(nd_bef == nd_aft, "Node chain selection sort should be stable");

  delete e_taylor;
  delete e_maclaurin;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "LINKED LIST WRAPPER TESTS" << std::endl << std::endl;
  std::cout << std::fixed << std::setprecision(4);

  linked_list<float, true> ll(2);
  std::cout << "Doubly linked list initialized with 2: " << ll << std::endl;
  TEST_EQUAL(ll.size(), 1, "Initial size for linked list should be 1");
  TEST_EQUAL(ll.head()->val(), 2, "Head node value should be 2");
  TEST_EQUAL(ll.tail()->val(), 2, "Tail node value should be 2");
  TEST_EQUAL(ll.head(), ll.tail(), "Head and tail nodes should be the same");

  ll.append(4).append(8);
  std::cout << "Appended values 4 and 8 to tail: " << ll << std::endl;
  std::cout << "Forward chain starting from tail: " << ll.tail() << std::endl;
  std::cout << "Forward chain starting from head: " << ll.head() << std::endl;
  TEST_EQUAL(ll.size(), 3, "Size should be 3 after appending");
  TEST_LIST_SEQ(ll, gt1, 3, "Linked list should have expected sequence");
  TEST_EQUAL(ll.tail()->next(), nullptr, "Tail should not have next node");

  mid = ll.head()->next()->next()->prev()->val();
  std::cout << "Middle node value: " << mid << std::endl;
  TEST_EQUAL(mid, 4, "Middle node's value should be 4 after deletions");

  ll.head() = ll.head()->prepend(0)->prepend(0.5);
  ll.head() = ll.head()->prepend(0.25)->prepend(0.125);
  std::cout << "In this order, chain-appended values 0, 0.5, 0.25 and 0.125 ";
  std::cout << " to head: " << ll << std::endl;
  TEST_EQUAL(ll.size(), 7, "Linked list should have size 7");
  TEST_EQUAL(ll.head()->size(), 7, "Linked list head node should have size 7");
  TEST_EQUAL(ll.tail()->size(), 7, "Doubly linked tail should also count 7");
  TEST_LIST_SEQ(ll, gt4, 7, "Linked list should have expected sequence");

  mid = ll.head()->next()->next()->next()->next()->prev()->val();
  std::cout << "Middle node value: " << mid << std::endl;
  TEST_EQUAL(mid, 0, "Middle node's value should be 0 after appending");

  std::cout << "Searched 4 | Found: " << ll.search(4)->val() << std::endl;
  TEST_EQUAL(ll.search(4)->val(), 4, "Search should find node with val 4");

  ll.delete_node(0);
  ll.delete_node(0.125);
  float gt6[5] = {0.25, 0.5, 2, 4, 8};
  std::cout << "Deleted 0.125 and 0 nodes by value: " << ll << std::endl;
  TEST_EQUAL(ll.size(), 5, "Linked list size should be 5 after deletions");
  TEST_LIST_SEQ(ll, gt6, 5, "Linked list should have expected sequence");

  mid = ll.head()->next()->next()->next()->prev()->val();
  std::cout << "Middle node value: " << mid << std::endl;
  TEST_EQUAL(mid, 2, "Middle node's value should be 2 after deletion");

  ll.delete_node(8);
  ll.delete_node(4);
  std::cout << "Deleted 4 and 8 nodes by value: " << ll << std::endl;
  TEST_EQUAL(ll.size(), 3, "Linked list size should be 3 after deletions");
  TEST_LIST_SEQ(ll.reverse(), gt5, 3, "Linked list sequence should match gt");

  mid = ll.head()->next()->next()->prev()->val();
  std::cout << "Middle node value: " << mid << std::endl;
  TEST_EQUAL(mid, 0.5, "Middle node's value should be 0.5 after deletion");

  ll.delete_node(0.25);
  float gt7[2] = {0.5, 2};
  std::cout << "Deleted 0.25 node by value: " << ll << std::endl;
  TEST_EQUAL(ll.size(), 2, "Linked list size should be 2 after deletions");
  TEST_LIST_SEQ(ll.reverse(), gt7, 2, "Linked list sequence should match gt");

  ll.delete_node(0.5);
  float gt8[1] = {2};
  std::cout << "Deleted 0.5 node by value: " << ll << std::endl;
  TEST_EQUAL(ll.size(), 1, "Linked list size should be 1 after deletions");
  TEST_LIST_SEQ(ll, gt8, 1, "Linked list should have expected sequence");

  ll.delete_node(2);
  std::cout << "Deleted 2 node by value, should be last: " << ll << std::endl;
  TEST_EQUAL(ll.size(), 0, "Linked list size should be 0 after deletions");
  TEST_CHECK(ll.is_empty(), "Linked list should be empty after deletions");
  TEST_EQUAL(ll.head(), nullptr, "Linked list head should be nullptr");
  TEST_EQUAL(ll.tail(), nullptr, "Linked list tail should be nullptr");

  std::cout << "Attempting a delete on an empty list" << std::endl;
  ll.delete_node(0);

  int unt_gt[10] = {188, 162, 146, 124, 120, 96, 88, 52, 5, 2};
  linked_list<int, true> untouchable({  2,   5,  52,  88,  96,
                                      120, 124, 146, 162, 188});
  std::cout << "Constructed untouchable numbers from initializer list: ";
  std::cout << untouchable.reverse() << std::endl;
  TEST_EQUAL(untouchable.size(), 10, "Linked list should have size 10");
  TEST_LIST_SEQ(untouchable, unt_gt, 10, "Linked list seq should match");
  TEST_CHECK(untouchable.contains(120), "120 is an untouchable number");
  TEST_EQUAL(untouchable.contains(200), 0, "200 is not untouchable");
  int minpr = untouchable.min(min_s)->val();
  TEST_EQUAL(minpr, 5, "Min should return preceding node");
  TEST_EQUAL(min_s, 2, "Min value should be 2");

  untouchable.clear() = {2, 5, 52, 88, 96, 120, 124, 146, 162, 188};
  unsigned gtun[10] = {2, 5, 52, 88, 96, 120, 124, 146, 162, 188};
  std::cout << "Assigned untouchable numbers from initializer list: ";
  std::cout << untouchable << std::endl;
  TEST_EQUAL(untouchable.size(), 10, "Linked list should have size 10");
  TEST_LIST_SEQ(untouchable, gtun, 10, "Linked list sequence should match");
  TEST_CHECK(untouchable.contains(52), "52 is an untouchable number");
  TEST_EQUAL(untouchable.contains(42), 0, "42 is not untouchable");
  int maxpr = untouchable.max(max_s)->val();
  TEST_EQUAL(maxpr, 162, "Max should return preceding node");
  TEST_EQUAL(max_s, 188, "Max should return preceding node");

  untouchable.clear();
  node<int, true>* untouch_node = node<int, true>::create(0);
  *untouch_node = {2, 5, 52, 88, 96, 120, 124, 146, 162, 188};
  untouchable = *(untouch_node->next()->next());
  std::cout << "Assigned untouchable numbers from node chain: ";
  std::cout << untouchable << std::endl;
  TEST_EQUAL(untouchable.size(), 10, "Linked list should have size 10");
  TEST_LIST_SEQ(untouchable, gtun, 10, "Linked list sequence should match");
  TEST_CHECK(untouchable.contains(5), "2 is an untouchable number");
  TEST_EQUAL(untouchable.contains(3), 0, "3 is not untouchable");

  dyn_array<int> untouch_arr = {5, 52, 88, 96, 120, 124, 146, 162, 188};
  untouchable.delete_head();
  std::cout << "Deleted head from linked list: " << untouchable << std::endl;
  TEST_EQUAL(untouchable.size(), 9, "Linked list should have size 9");
  TEST_LIST_SEQ(untouchable, untouch_arr, 9, "Linked list seq should match");

  untouch_arr = {5, 52, 88, 96, 120, 124, 146, 162};
  untouchable.delete_tail();
  std::cout << "Deleted head from linked list: " << untouchable << std::endl;
  TEST_EQUAL(untouchable.size(), 8, "Linked list should have size 8");
  TEST_LIST_SEQ(untouchable, untouch_arr, 8, "Linked list seq should match");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "TAIL-LOOPING SINGLY-LINKED LIST TESTS" << std::endl;
  std::cout << std::endl;

  linked_list<std::string, false> robertfrost;
  const char* robertfrost1 = "The woods are lovely, dark and deep";
  const char* robertfrost2 = "But I have promises to keep";
  const char* robertfrost3 = "And miles to go before I sleep";

  std::cout << robertfrost1 << std::endl;
  std::cout << robertfrost2 << std::endl;
  std::cout << robertfrost3 << std::endl << std::endl;

  // Invert the word order of the first line to append words to head later.
  std::string buf;
  std::string robertfrost1_arr[7];
  std::stringstream ss(robertfrost1);
  for (unsigned i = 7; i > 0; --i) {
    ss >> robertfrost1_arr[i-1];
  }
  ss.str(robertfrost2);
  ss.clear();
  while (ss >> buf)
    robertfrost.append(buf);
  std::string gt9[6] = {"But", "I", "have", "promises", "to", "keep"};
  std::cout << "Appended 2nd line to tail of empty linked list" << std::endl;
  std::cout << robertfrost << std::endl;
  TEST_EQUAL(robertfrost.size(), 6, "Size should match 2nd line's word count");
  TEST_LIST_SEQ(robertfrost, gt9, 6, "Linked list should match 2nd line");
  std::cout << std::endl;

  ss.str(robertfrost3);
  ss.clear();
  ss >> buf;
  robertfrost.append(buf);
  ss >> buf;
  robertfrost.append(buf);
  std::string gt10[8] = {"But", "I", "have", "promises", "to", "keep", "And",
                         "miles"};
  std::cout << "Appended first 2 words of the 3rd line to tail" << std::endl;
  std::cout << robertfrost << std::endl;
  TEST_EQUAL(robertfrost.size(), 8, "Size should be 8 after appending");
  TEST_LIST_SEQ(robertfrost, gt10, 8, "Word sequence should match linked list");
  std::cout << std::endl;

  // Create loops using node pointers to avoid double-deletion problems.
  // Once a node is linked to linked_list class, it doesn't need to be deleted.
  ss >> buf;
  node<std::string, false>* partial = node<std::string, false>::create(buf);
  while (ss >> buf)
    partial->append(buf);
  std::string gt11[5] = {"to", "go", "before", "I", "sleep"};
  std::cout << "Appended rest of 3rd line to a new node chain" << std::endl;
  std::cout << partial << std::endl;
  TEST_EQUAL(partial->size(), 5, "Size should match the appended word count");
  TEST_NODE_SEQ(*partial, gt11, 5, "Word sequence should match node chain");
  std::cout << std::endl;

  partial->append_node(robertfrost.search(std::string("And")));
  std::string gt12[7] = {"to", "go", "before", "I", "sleep", "And", "miles"};
  std::cout << "Appended \"And\" by ptr to new node chain tail" << std::endl;
  std::cout << partial << std::endl;
  TEST_EQUAL(partial->size(), 7, "Size should match the current word count");
  TEST_NODE_SEQ(*partial, gt12, 7, "Word sequence should match node chain");
  std::cout << std::endl;

  robertfrost.append_node(partial);
  std::string gt13[13] = {"But", "I", "have", "promises", "to", "keep", "And",
                          "miles", "to", "go", "before", "I", "sleep"};
  std::cout << "Appended new linked list to other's tail" << std::endl;
  std::cout << robertfrost << std::endl;
  TEST_EQUAL(robertfrost.size(), 13, "Size should match current word count");
  TEST_LIST_SEQ(robertfrost, gt13, 13, "Word seq should match linked list");
  TEST_CHECK(robertfrost.detect_tail_loop(), "Tail loop should be detected");
  std::cout << std::endl;

  for (unsigned i = 0; i < 7; ++i)
    robertfrost.prepend(robertfrost1_arr[i]);
  std::string gt14[20] = {"The", "woods", "are", "lovely,", "dark", "and",
                          "deep", "But", "I", "have", "promises", "to", "keep",
                          "And", "miles", "to", "go", "before", "I", "sleep"};
  std::cout << "Appended 1st line to head in reverse-order" << std::endl;
  std::cout << robertfrost << std::endl;
  TEST_EQUAL(robertfrost.size(), 20, "Linked list size should be 20");
  TEST_LIST_SEQ(robertfrost, gt14, 20, "Word seq should match linked list");
  std::cout << "prev: " << robertfrost.min(buf)->val() << " | min: " << buf;
  std::cout << std::endl;
  std::cout << "prev: " << robertfrost.max(buf)->val() << " | max: " << buf;
  std::cout << std::endl;
  TEST_EQUAL(robertfrost.min(buf)->val(), "keep", "\"keep\" should be prev");
  TEST_EQUAL(buf, "And", "Should find lexicographical minimum");
  TEST_EQUAL(robertfrost.max(buf)->val(), "The", "\"The\" should be prev");
  TEST_EQUAL(buf, "woods", "Should find lexicographical maximum");
  std::cout << std::endl;

  std::string fnd = robertfrost.search("sleep")->val();
  std::cout << "Searched \"sleep\" | Found: " << fnd << std::endl;
  TEST_EQUAL(fnd, "sleep", "Search should find node with value \"sleep\"");
  TEST_CHECK(robertfrost.contains("sleep"), "Should have \"sleep\"");
  TEST_EQUAL(robertfrost.contains("weep"), 0, "Shouldn't have \"weep\"");
  fnd = robertfrost.search("miles")->val();
  std::cout << "Searched \"miles\" | Found: " << fnd << std::endl;
  TEST_EQUAL(fnd, "miles", "Search should find node with value \"miles\"");
  TEST_CHECK(robertfrost.contains("miles"), "Should have \"miles\"");
  TEST_EQUAL(robertfrost.contains("smiles"), 0, "Shouldn't have \"smiles\"");
  fnd = robertfrost.search("keep")->val();
  std::cout << "Searched \"keep\" | Found: " << fnd << std::endl;
  TEST_EQUAL(fnd, "keep", "Search should find node with value \"keep\"");
  TEST_CHECK(robertfrost.contains("keep"), "Should have \"keep\"");
  TEST_EQUAL(robertfrost.contains("seep"), 0, "Shouldn't have \"seep\"");
  std::cout << std::endl;

  linked_list<std::string, false> rfcopy1 = robertfrost;
  std::cout << "Copied using copy constructor: " << rfcopy1 << std::endl;
  TEST_CHECK(rfcopy1 == robertfrost, "Copy with loops should be equal");
  rfcopy1.tail()->next() = nullptr;
  std::cout << "Broke tail loop on the copy: " << rfcopy1 << std::endl;
  TEST_CHECK(rfcopy1 != robertfrost, "Copy without loops should not be equal");
  std::cout << std::endl;

  linked_list<std::string, false> rfcopy2;
  rfcopy2 = robertfrost;
  std::cout << "Copied using assignment operator: " << rfcopy2 << std::endl;
  TEST_CHECK(rfcopy2 == robertfrost, "Copy with loops should be equal");
  rfcopy2.tail()->next() = nullptr;
  std::cout << "Broke tail loop on the copy: " << rfcopy2 << std::endl;
  TEST_CHECK(rfcopy2 != robertfrost, "Copy without loops should not be equal");
  std::cout << std::endl;

  linked_list<std::string, false> rfcopy3 = robertfrost;
  rfcopy3.delete_tail().delete_head();
  std::string gttl[18] = {"woods", "are", "lovely,", "dark", "and", "deep",
                          "But", "I", "have", "promises", "to", "keep",
                          "And", "miles", "to", "go", "before", "I"};
  std::cout << "Deleted head and tail on a new copy: " << rfcopy3 << std::endl;
  TEST_EQUAL(rfcopy3.size(), 18, "Linked list copy should have size 18");
  TEST_LIST_SEQ(rfcopy3, gttl, 18, "Word seq should match linked list");
  std::cout << std::endl;

  linked_list<std::string, false> rfcopy4 = robertfrost;
  rfcopy4.reverse();
  std::string gtrv[20] = {"sleep", "I", "before", "go", "to", "miles", "And",
                          "keep", "to", "promises", "have", "I", "But", "deep",
                          "and", "dark", "lovely,", "are", "woods", "The"};
  std::cout << "Reversed a new copy: " << rfcopy4 << std::endl;
  TEST_EQUAL(rfcopy4.size(), 20, "Reversed copy should have size 20");
  TEST_LIST_SEQ(rfcopy4, gtrv, 20, "Reversed seq should match linked list");
  TEST_CHECK(!rfcopy4.detect_tail_loop(), "Reverse should not have tail loop");

  robertfrost.delete_node("promises");
  std::cout << "Deleted \"promises\":" << std::endl;
  std::cout << robertfrost << std::endl;
  robertfrost.delete_node("The");
  std::cout << "Deleted \"The\":" << std::endl;
  std::cout << robertfrost << std::endl;
  robertfrost.delete_node("to");
  std::cout << "Deleted \"to\":" << std::endl;
  std::cout << robertfrost << std::endl;
  robertfrost.delete_node("And");
  std::cout << "Deleted \"And\":" << std::endl;
  std::cout << robertfrost << std::endl << std::endl;

  // Re-create the loop to test looping tail deletion
  robertfrost.append_node(robertfrost.search(std::string("miles")));
  std::cout << "Re-created another loop at \"miles\":" << std::endl;
  std::cout << robertfrost << std::endl;
  TEST_CHECK(robertfrost.detect_tail_loop(), "Tail loop should be detected");
  TEST_EQUAL(robertfrost.detect_head_loop(), nullptr, "No head loop expected");
  std::cout << std::endl;

  robertfrost.delete_node("sleep");
  std::cout << "Deleted \"sleep\":" << std::endl;
  std::cout << robertfrost << std::endl;
  TEST_EQUAL(robertfrost.detect_tail_loop(), nullptr, "No tail loop expected");
  TEST_EQUAL(robertfrost.detect_head_loop(), nullptr, "No head loop expected");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "TAIL-LOOPING SINGLY-LINKED LIST SORTING TESTS" << std::endl;
  std::cout << std::endl;

  linked_list<std::string, false> rf;
  node<std::string, false> *ntbef, *ntaft;
  dyn_array<std::string> gtrf;
  gtrf = {"The", "woods", "are", "lovely,", "dark", "and", "deep",
          "But", "I", "have", "promises", "to", "keep",
          "And", "miles", "to", "go", "before", "I", "sleep"};

  std::cout << "Robert Frost: " << (rf = rfcopy4) << std::endl;
  ntbef = rf.search("to");
  std::cout << "Ran bubble_sort in increasing order: ";
  std::cout << rf.bubble_sort() << std::endl;
  ntaft = rf.search("to");
  gtrf.bubble_sort();
  TEST_LIST_SEQ(rf, gtrf, 20, "Should match dyn_array result");
  TEST_EQUAL(ntbef->val(), "to", "Pre-sort search should find \"to\"");
  TEST_EQUAL(ntaft->val(), "to", "Post-sort search should find \"to\"");
  TEST_CHECK(ntbef == ntaft, "Linked list bubble sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled input: " << (rf = rfcopy4) << std::endl;
  ntbef = rf.search("to");
  std::cout << "Ran insertion_sort in increasing order: ";
  std::cout << rf.insertion_sort() << std::endl;
  ntaft = rf.search("to");
  gtrf.insertion_sort();
  TEST_LIST_SEQ(rf, gtrf, 20, "Should match dyn_array result");
  TEST_EQUAL(ntbef->val(), "to", "Pre-sort search should find \"to\"");
  TEST_EQUAL(ntaft->val(), "to", "Post-sort search should find \"to\"");
  TEST_CHECK(ntbef == ntaft, "Linked list insetion sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled input: " << (rf = rfcopy4) << std::endl;
  ntbef = rf.search("to");
  std::cout << "Ran merge_sort in decreasing order: ";
  std::cout << rf.merge_sort(false) << std::endl;
  ntaft = rf.search("to");
  gtrf.merge_sort(false);
  TEST_LIST_SEQ(rf, gtrf, 20, "Should match dyn_array result");
  TEST_EQUAL(ntbef->val(), "to", "Pre-sort search should find \"to\"");
  TEST_EQUAL(ntaft->val(), "to", "Post-sort search should find \"to\"");
  TEST_CHECK(ntbef == ntaft, "Linked list merge sort should be stable");
  std::cout << std::endl;

  std::cout << "Shuffled input: " << (rf = rfcopy4) << std::endl;
  ntbef = rf.search("to");
  std::cout << "Ran selection_sort in decreasing order: ";
  std::cout << rf.selection_sort(false) << std::endl;
  ntaft = rf.search("to");
  gtrf.selection_sort(false);
  TEST_LIST_SEQ(rf, gtrf, 20, "Should match dyn_array result");
  TEST_EQUAL(ntbef->val(), "to", "Pre-sort search should find \"to\"");
  TEST_EQUAL(ntaft->val(), "to", "Post-sort search should find \"to\"");
  TEST_CHECK(ntbef == ntaft, "Linked list selection sort should be stable");
  std::cout << std::endl;

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HEAD-LOOPING DOUBLY-LINKED LIST TESTS" << std::endl;
  std::cout << std::endl;

  linked_list<std::string, true> shakespeare;
  const char* shakespeare1 = "tomorrow,";
  const char* shakespeare2 = "and tomorrow,\n"
                             "creeps in this petty pace from day to day,\n"
                             "to the last syllable of recorded time;\n"
                             "and all our yesterdays have lighted fools\n"
                             "the way to dusty death. Out, out, brief candle!\n"
                             "Life's but a walking shadow, a poor player,\n"
                             "that struts and frets his hour upon the stage,\n"
                             "and then is heard no more. It is a tale\n"
                             "told by an idiot, full of sound and fury,\n"
                             "signifying nothing.";

  std::cout << shakespeare1 << " and tomorrow, " << shakespeare2 << std::endl;
  std::cout << std::endl;

  dyn_array<std::string> gtsh;
  std::string gt15[72];
  size_t ind = 0;
  ss.str(shakespeare2);
  ss.clear();
  while (ss >> buf) {
    gt15[ind++] = buf;
    shakespeare.append(buf);
    gtsh.add(buf);
  }
  ss.str(shakespeare1);
  ss.clear();

  ss >> buf;
  gtsh.add(buf);
  node<std::string, true>* beginning = node<std::string, true>::create(buf);
  beginning->prev() = shakespeare.search("and");
  shakespeare.prepend_node(beginning);
  std::cout << "Added head loop" << std::endl;
  std::cout << shakespeare << std::endl;
  TEST_EQUAL(shakespeare.size(), 73, "Linked list size should be 73");
  TEST_EQUAL(shakespeare.head()->val(), "tomorrow,", "Head is \"tomorrow,\"");
  TEST_NODE_SEQ(*shakespeare.head()->next(), gt15, 72, "Match should work");
  TEST_CHECK(shakespeare.detect_head_loop(), "Head loop should be detected");
  TEST_EQUAL(shakespeare.detect_tail_loop(), nullptr, "No tail loop expected");
  std::cout << "prev: " << shakespeare.min(buf)->val() << " | min: " << buf;
  std::cout << std::endl;
  std::cout << "prev: " << shakespeare.max(buf)->val() << " | max: " << buf;
  std::cout << std::endl;
  TEST_EQUAL(shakespeare.min(buf)->val(), "more.", "\"more.\" should be prev");
  TEST_EQUAL(buf, "It", "Should find lexicographical minimum");
  TEST_EQUAL(shakespeare.max(buf)->val(), "our", "\"our\" should be prev");
  TEST_EQUAL(buf, "yesterdays", "Should find lexicographical maximum");
  std::cout << std::endl;

  fnd = shakespeare.search("syllable")->val();
  std::cout << "Searched \"syllable\" | Found: " << fnd << std::endl;
  TEST_EQUAL(fnd, "syllable", "Search should find node \"syllable\"");
  TEST_CHECK(shakespeare.contains("and"), "Should have \"and\"");
  TEST_EQUAL(shakespeare.contains("be"), 0, "Shouldn't have \"be\"");

  linked_list<std::string, true> shcopy1 = shakespeare;
  std::cout << "Copied using copy constructor: " << shcopy1 << std::endl;
  TEST_CHECK(shcopy1 == shakespeare, "Copy with loops should be equal");
  shcopy1.head()->prev() = nullptr;
  std::cout << "Broke head loop on the copy: " << shcopy1 << std::endl;
  TEST_CHECK(shcopy1 != shakespeare, "Copy without loops should not be equal");
  std::cout << std::endl;

  linked_list<std::string, true> shcopy2;
  shcopy2 = shakespeare;
  std::cout << "Copied using assignment operator: " << shcopy2 << std::endl;
  TEST_CHECK(shcopy2 == shakespeare, "Copy with loops should be equal");
  shcopy2.head()->prev() = nullptr;
  std::cout << "Broke head loop on the copy: " << shcopy2 << std::endl;
  TEST_CHECK(shcopy2 != shakespeare, "Copy without loops should not be equal");
  std::cout << std::endl;

  linked_list<std::string, true> shcopy3 = shakespeare;
  std::cout << "Created a reversed copy: " << shcopy3.reverse() << std::endl;
  TEST_CHECK(shcopy3.detect_tail_loop(), "Head loop should become tail loop");
  std::cout << std::endl;

  linked_list<std::string, true> shcopy4 = shakespeare;
  shcopy4.delete_head().delete_tail();
  std::string gthd[71] = {"and", "tomorrow,", "creeps", "in", "this", "petty",
                          "pace", "from", "day", "to", "day,", "to", "the",
                          "last", "syllable", "of", "recorded", "time;", "and",
                          "all", "our", "yesterdays", "have", "lighted",
                          "fools", "the", "way", "to", "dusty", "death.",
                          "Out,", "out,", "brief", "candle!", "Life's", "but",
                          "a", "walking", "shadow,", "a", "poor", "player,",
                          "that", "struts", "and", "frets", "his", "hour",
                          "upon", "the", "stage,", "and", "then", "is",
                          "heard", "no", "more.", "It", "is", "a", "tale",
                          "told", "by", "an", "idiot,", "full", "of", "sound",
                          "and", "fury,", "signifying"};
  std::cout << "Deleted head and tail on a new copy: " << shcopy4 << std::endl;
  TEST_EQUAL(shcopy4.size(), 71, "Linked list copy should have size 71");
  TEST_LIST_SEQ(shcopy4, gthd, 71, "Word seq should match linked list");

  std::cout << "Deleted \"tomorrow,\":" << std::endl;
  shakespeare.delete_node("tomorrow,");
  std::cout << shakespeare << std::endl;
  TEST_EQUAL(shakespeare.size(), 72, "Linked list size should be 72");
  std::cout << std::endl;

  ss.str(shakespeare1);
  ss.clear();
  while (ss >> buf)
    shakespeare.prepend(buf);
  shakespeare.prepend_node(shakespeare.search("and"));
  std::cout << "Restored original:" << std::endl;
  std::cout << shakespeare << std::endl << std::endl;
  shakespeare.delete_node("and");
  std::cout << "Deleted \"and\":" << std::endl;
  std::cout << shakespeare << std::endl << std::endl;
  std::cout << "Deleted \"creeps\":" << std::endl;
  shakespeare.delete_node("creeps");
  std::cout << shakespeare << std::endl << std::endl;
  TEST_EQUAL(shakespeare.size(), 71, "Linked list size should be 71");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HEAD-LOOPING DOUBLY-LINKED LIST SORTING TESTS" << std::endl;
  std::cout << std::endl;

  shcopy3.reverse();
  linked_list<std::string, true> sh;
  node<std::string, true> *nhbef, *nhaft;

  std::cout << "William Shakespeare: " << (sh = shcopy3) << std::endl;
  nhbef = sh.search("tomorrow,");
  std::cout << "Ran bubble_sort in decreasing order: ";
  std::cout << sh.bubble_sort(false) << std::endl;
  nhaft = sh.search("tomorrow,");
  gtsh.bubble_sort(false);
  TEST_LIST_SEQ(sh, gtsh, 73, "Should match dyn_array result");
  TEST_EQUAL(nhbef->val(), "tomorrow,", "Pre-sort should find \"tomorrow,\"");
  TEST_EQUAL(nhaft->val(), "tomorrow,", "Post-sort should find \"tomorrow,\"");
  TEST_CHECK(nhbef == nhaft, "Linked list bubble sort should be stable");
  std::cout << std::endl;

  std::cout << "William Shakespeare: " << (sh = shcopy3) << std::endl;
  nhbef = sh.search("tomorrow,");
  std::cout << "Ran insertion_sort in decreasing order: ";
  std::cout << sh.insertion_sort(false) << std::endl;
  nhaft = sh.search("tomorrow,");
  gtsh.insertion_sort(false);
  TEST_LIST_SEQ(sh, gtsh, 73, "Should match dyn_array result");
  TEST_EQUAL(nhbef->val(), "tomorrow,", "Pre-sort should find \"tomorrow,\"");
  TEST_EQUAL(nhaft->val(), "tomorrow,", "Post-sort should find \"tomorrow,\"");
  TEST_CHECK(nhbef == nhaft, "Linked list insertion sort should be stable");
  std::cout << std::endl;

  std::cout << "William Shakespeare: " << (sh = shcopy3) << std::endl;
  nhbef = sh.search("tomorrow,");
  std::cout << "Ran merge_sort in increasing order: ";
  std::cout << sh.merge_sort() << std::endl;
  nhaft = sh.search("tomorrow,");
  gtsh.merge_sort();
  TEST_LIST_SEQ(sh, gtsh, 73, "Should match dyn_array result");
  TEST_EQUAL(nhbef->val(), "tomorrow,", "Pre-sort should find \"tomorrow,\"");
  TEST_EQUAL(nhaft->val(), "tomorrow,", "Post-sort should find \"tomorrow,\"");
  TEST_CHECK(nhbef == nhaft, "Linked list merge sort should be stable");
  std::cout << std::endl;

  std::cout << "William Shakespeare: " << (sh = shcopy3) << std::endl;
  nhbef = sh.search("tomorrow,");
  std::cout << "Ran selection_sort in increasing order: ";
  std::cout << sh.selection_sort() << std::endl;
  nhaft = sh.search("tomorrow,");
  gtsh.selection_sort();
  TEST_LIST_SEQ(sh, gtsh, 73, "Should match dyn_array result");
  TEST_EQUAL(nhbef->val(), "tomorrow,", "Pre-sort should find \"tomorrow,\"");
  TEST_EQUAL(nhaft->val(), "tomorrow,", "Post-sort should find \"tomorrow,\"");
  TEST_CHECK(nhbef == nhaft, "Linked list selection sort should be stable");
  std::cout << std::endl;

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HEAD-AND-TAIL-LOOPING LINKED LIST TESTS" << std::endl;

  linked_list<std::string, true> josephconrad;
  std::string gt16[2] = {"The", "horror!"};
  josephconrad.append(std::string("The"));
  josephconrad.append(std::string("horror!"));
  std::cout << josephconrad << std::endl;
  TEST_EQUAL(josephconrad.size(), 2, "Linked list size should be 2");
  TEST_LIST_SEQ(josephconrad, gt16, 2, "Both words should match");
  std::cout << std::endl;

  josephconrad.append_node(josephconrad.search("The"));
  std::cout << josephconrad << std::endl;
  TEST_EQUAL(josephconrad.size(), 2, "Linked list size should be 2");
  TEST_LIST_SEQ(josephconrad, gt16, 2, "Both words should match");
  TEST_CHECK(josephconrad.detect_head_loop(), "Head loop should be detected");
  TEST_CHECK(josephconrad.detect_tail_loop(), "Tail loop should be detected");
  josephconrad.min(buf);
  TEST_EQUAL(buf, "The", "Head should be the lexicographical minimum");
  josephconrad.max(buf);
  TEST_EQUAL(buf, "horror!", "Tail should be the lexicographical maximum");
  std::cout << std::endl;

  fnd = josephconrad.search("The")->val();
  std::cout << "Searched \"The\" | Found: " << fnd << std::endl;
  TEST_EQUAL(fnd, "The", "Search should find node \"The\"");
  TEST_CHECK(josephconrad.contains("horror!"), "Should have \"horror!\"");
  TEST_EQUAL(josephconrad.contains("horror"), 0, "Shouldn't have \"horror\"");

  linked_list<std::string, true> jccopy1 = josephconrad;
  std::cout << "Copied using copy constructor: " << jccopy1 << std::endl;
  TEST_CHECK(jccopy1 == josephconrad, "Copy with loops should be equal");
  jccopy1.head()->prev() = nullptr;
  std::cout << "Broke head loop on the copy: " << jccopy1 << std::endl;
  TEST_CHECK(jccopy1 != josephconrad, "Copy without loops should not be equal");
  std::cout << std::endl;

  linked_list<std::string, true> jccopy2;
  jccopy2 = josephconrad;
  std::cout << "Copied using assignment operator: " << jccopy2 << std::endl;
  TEST_CHECK(jccopy2 == josephconrad, "Copy with loops should be equal");
  jccopy2.tail()->next() = nullptr;
  std::cout << "Broke tail loop on the copy: " << jccopy2 << std::endl;
  TEST_CHECK(jccopy2 != josephconrad, "Copy without loops should not be equal");
  std::cout << std::endl;

  linked_list<std::string, true> jccopy3 = josephconrad;
  jccopy3.delete_head().delete_tail();
  std::cout << "Deleted head and tail on a new copy: " << jccopy3 << std::endl;
  TEST_CHECK(jccopy3.is_empty(), "Copy should be empty after deletions");
  std::cout << std::endl;

  linked_list<std::string, true> jccopy4 = josephconrad;
  std::cout << "Created a reversed copy: " << jccopy4.reverse() << std::endl;
  TEST_CHECK(jccopy4.detect_tail_loop(), "Head loop should become tail loop");
  TEST_CHECK(jccopy4.detect_head_loop(), "Tail loop should become head loop");
  std::cout << std::endl;

  josephconrad.delete_node("The");
  std::cout << josephconrad << std::endl;
  TEST_EQUAL(josephconrad.detect_head_loop(), nullptr, "Head loop impossible");
  TEST_EQUAL(josephconrad.detect_tail_loop(), nullptr, "Tail loop impossible");
  std::cout << std::endl;

  linked_list<std::string, true> notes;
  notes.append(std::string("do"));
  notes.append(std::string("re"));
  notes.append(std::string("mi"));
  notes.append(std::string("fa"));
  notes.append(std::string("sol"));
  notes.append(std::string("la"));
  notes.append(std::string("si"));
  notes.prepend_node(notes.search("si"));
  std::string gt17[7] = {"do", "re", "mi", "fa", "sol", "la", "si"};
  std::cout << "Notes in Italian: " << notes << std::endl;
  TEST_EQUAL(notes.size(), 7, "There are 7 natural (white key) notes");
  TEST_LIST_SEQ(notes, gt17, 7, "Order should be: do-re-mi-fa-sol-la-si");
  TEST_CHECK(notes.detect_head_loop(), "Head loop expected - previous octave");
  TEST_CHECK(notes.detect_tail_loop(), "Tail loop expected - next octave");
  std::cout << std::endl;

  linked_list<std::string, true> ntcopy1 = notes;
  std::cout << "Copied using copy constructor: " << ntcopy1 << std::endl;
  TEST_CHECK(ntcopy1 == notes, "Copy with loops should be equal");
  ntcopy1.tail()->next() = nullptr;
  std::cout << "Broke head loop on the copy: " << ntcopy1 << std::endl;
  TEST_CHECK(ntcopy1 != notes, "Copy without loops should not be equal");
  std::cout << std::endl;

  linked_list<std::string, true> ntcopy2;
  ntcopy2 = notes;
  std::cout << "Copied using assignment operator: " << ntcopy2 << std::endl;
  TEST_CHECK(ntcopy2 == notes, "Copy with loops should be equal");
  ntcopy2.head()->prev() = nullptr;
  std::cout << "Broke tail loop on the copy: " << ntcopy2 << std::endl;
  TEST_CHECK(ntcopy2 != notes, "Copy without loops should not be equal");
  std::cout << std::endl;

  linked_list<std::string, true> ntcopy3 = notes;
  ntcopy3.delete_tail().delete_head();
  std::string gt18[5] = {"re", "mi", "fa", "sol", "la"};
  std::cout << "Deleted head and tail on a new copy: " << ntcopy3 << std::endl;
  TEST_EQUAL(ntcopy3.size(), 5, "Linked list copy should have size 71");
  TEST_LIST_SEQ(ntcopy3, gt18, 5, "Word seq should match linked list");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HEAD-AND-TAIL-LOOPING LINKED LIST SORTING TESTS" << std::endl;
  std::cout << std::endl;

  jccopy4.reverse();
  linked_list<std::string, true> jc, it;
  dyn_array<std::string> gtjc = {"The", "horror!"};
  dyn_array<std::string> gtit = {"do",  "re", "mi", "fa", "sol", "la", "si"};

  std::cout << "Joseph Conrad: " << (jc = jccopy4) << std::endl;
  std::cout << "Ran bubble_sort in decreasing order: ";
  std::cout << jc.bubble_sort(false) << std::endl;
  gtjc.bubble_sort(false);
  TEST_LIST_SEQ(jc, gtjc, 2, "Should match dyn_array result");
  std::cout << std::endl;

  std::cout << "Notes in Italian: " << (it = notes) << std::endl;
  std::cout << "Ran bubble_sort in increasing order: ";
  std::cout << it.bubble_sort() << std::endl;
  gtit.bubble_sort();
  TEST_LIST_SEQ(it, gtit, 7, "Should match dyn_array result");
  std::cout << std::endl;

  std::cout << "Joseph Conrad: " << (jc = jccopy4) << std::endl;
  std::cout << "Ran insertion_sort in increasing order: ";
  std::cout << jc.insertion_sort() << std::endl;
  gtjc.insertion_sort();
  TEST_LIST_SEQ(jc, gtjc, 2, "Should match dyn_array result");
  std::cout << std::endl;

  std::cout << "Notes in Italian: " << (it = notes) << std::endl;
  std::cout << "Ran insertion_sort in decreasing order: ";
  std::cout << it.insertion_sort(false) << std::endl;
  gtit.insertion_sort(false);
  TEST_LIST_SEQ(it, gtit, 7, "Should match dyn_array result");
  std::cout << std::endl;

  std::cout << "Joseph Conrad: " << (jc = jccopy4) << std::endl;
  std::cout << "Ran merge_sort in decreasing order: ";
  std::cout << jc.merge_sort(false) << std::endl;
  gtjc.merge_sort(false);
  TEST_LIST_SEQ(jc, gtjc, 2, "Should match dyn_array result");
  std::cout << std::endl;

  std::cout << "Notes in Italian: " << (it = notes) << std::endl;
  std::cout << "Ran merge_sort in increasing order: ";
  std::cout << it.merge_sort() << std::endl;
  gtit.merge_sort();
  TEST_LIST_SEQ(it, gtit, 7, "Should match dyn_array result");
  std::cout << std::endl;

  std::cout << "Joseph Conrad: " << (jc = jccopy4) << std::endl;
  std::cout << "Ran selection_sort in increasing order: ";
  std::cout << jc.selection_sort() << std::endl;
  gtjc.selection_sort();
  TEST_LIST_SEQ(jc, gtjc, 2, "Should match dyn_array result");
  std::cout << std::endl;

  std::cout << "Notes in Italian: " << (it = notes) << std::endl;
  std::cout << "Ran selection_sort in decreasing order: ";
  std::cout << it.selection_sort(false) << std::endl;
  gtit.selection_sort(false);
  TEST_LIST_SEQ(it, gtit, 7, "Should match dyn_array result");

  test_util::print_summary();
  return 0;
}