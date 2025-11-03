#include <iostream>
#include "test_util.h"
#include "dyn_array.h"
#include "linked_list.h"
#include "hash_table.h"
#include "stack.h"
#include "queue.h"
#include "graph.h"
#include "binary_tree.h"
#include "trie.h"

int main()
{
  bool contains_all;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH dyn_array<int> KEYS" << std::endl;

  dyn_array<int> primes = {2, 3, 5, 7, 11};
  dyn_array<dyn_array<int> > perms = primes.permutations();

  hash_table<dyn_array<int>, char> ht4(47);
  for (size_t i = 0; i < perms.size(); ++i)
    ht4.insert(perms[i], '\0');
  std::cout << "Hashed all permutations of: " << primes << std::endl;
  std::cout << "Hash contents: " << std::endl << ht4 << std::endl;
  TEST_EQUAL(ht4.keys().size(), 120, "Hash should have 120 keys");
  contains_all = true;
  for (size_t i = 0; i < perms.size(); ++i) {
    if (!ht4.contains(perms[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all permutations as keys");

  ht4.insert(perms[0], 'x');
  std::cout << "Inserted duplicate key marked with value 'x'" << std::endl;
  TEST_EQUAL(ht4[perms[0]], 'x', "Duplicate key's new value should be 'x'");

  ht4.remove(perms[0]);
  std::cout << "Removed the same key marked with value 'x'" << std::endl;
  TEST_EQUAL(ht4.contains(perms[0]), 0, "Removed key should be gone");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH node<int, false> KEYS" << std::endl;

  dyn_array<node<int, false>*> plists(nullptr, perms.size());
  for (size_t i = 0; i < perms.size(); ++i) {
    for (size_t p = 0; p < perms[i].size(); ++p) {
      if (plists[i])
        plists[i]->append(perms[i][p]);
      else
        plists[i] = node<int, false>::create(perms[i][p]);
    }
  }
  std::cout << "Created singly-linked node chains from perm arr" << std::endl;
  TEST_NODE_SEQ(*plists[0], perms[0], 5, "Node chains should match arrays");
  TEST_NODE_SEQ(*plists[25], perms[25], 5, "Node chains should match arrays");
  TEST_NODE_SEQ(*plists[119], perms[119], 5, "Node chains should match arrays");

  hash_table<node<int, false>, char> ht5(47);
  for (size_t i = 0; i < plists.size(); ++i)
    ht5.insert(*plists[i], '\0');
  std::cout << "Hashed all permutations of: " << plists[0] << std::endl;
  std::cout << "Hash contents: " << std::endl << ht5 << std::endl;
  TEST_EQUAL(ht5.keys().size(), 120, "Hash should have 120 keys");
  contains_all = true;
  for (size_t i = 0; i < plists.size(); ++i) {
    if (!ht5.contains(*plists[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all permutations as keys");

  ht5.insert(*plists[119], 'x');
  std::cout << "Inserted duplicate key marked with value 'x'" << std::endl;
  TEST_EQUAL(ht5[*plists[119]], 'x', "Duplicate key's new value should be 'x'");

  ht5.remove(*plists[0]);
  std::cout << "Removed the first key on the permutations list" << std::endl;
  TEST_EQUAL(ht5.contains(*plists[0]), 0, "Removed key should be gone");

  for (size_t i = 0; i < plists.size(); ++i)
    delete plists[i];
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH linked_list<int, true> KEYS" << std::endl;

  size_t lls = perms.size();
  dyn_array<linked_list<int, true> > perm_ll(linked_list<int, true>(), lls);
  for (size_t i = 0; i < perms.size(); ++i)
    perm_ll[i] = perms[i];
  std::cout << "Created doubly-linked lists from perm arr" << std::endl;
  TEST_LIST_SEQ(perm_ll[32], perms[32], 5, "Linked lists should match arrays");
  TEST_LIST_SEQ(perm_ll[64], perms[64], 5, "Linked lists should match arrays");
  TEST_LIST_SEQ(perm_ll[72], perms[72], 5, "Linked lists should match arrays");

  hash_table<linked_list<int, true>, char> ht6(47);
  for (size_t i = 0; i < perm_ll.size(); ++i)
    ht6.insert(perm_ll[i], '\0');
  std::cout << "Hashed all permutations of: " << perm_ll[0] << std::endl;
  std::cout << "Hash contents: " << std::endl << ht6 << std::endl;
  TEST_EQUAL(ht6.keys().size(), 120, "Hash should have 120 keys");
  contains_all = true;
  for (size_t i = 0; i < perm_ll.size(); ++i) {
    if (!ht6.contains(perm_ll[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all permutations as keys");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH stack<int> KEYS" << std::endl;

  dyn_array<stack<int> > pstacks(stack<int>(), perms.size());
  for (size_t i = 0; i < perms.size(); ++i)
    pstacks[i] = perms[i];
  std::cout << "Created stacks from permutation arrays" << std::endl;

  hash_table<stack<int>, char> ht7(47);
  for (size_t i = 0; i < pstacks.size(); ++i)
    ht7.insert(pstacks[i], '\0');
  std::cout << "Hashed all permutations of: " << pstacks[0] << std::endl;
  std::cout << "Hash contents: " << std::endl << ht7 << std::endl;
  TEST_EQUAL(ht7.keys().size(), 120, "Hash should have 120 keys");
  contains_all = true;
  for (size_t i = 0; i < pstacks.size(); ++i) {
    if (!ht7.contains(pstacks[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all permutations as keys");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH queue<int> KEYS" << std::endl;

  dyn_array<queue<int> > pqueues(queue<int>(), perms.size());
  for (size_t i = 0; i < perms.size(); ++i)
    for (size_t j = 0; j < perms[i].size(); ++j)
      pqueues[i].enqueue(perms[i][j]);
  std::cout << "Created queues from permutation arrays" << std::endl;

  hash_table<queue<int>, char> ht8(47);
  for (size_t i = 0; i < pqueues.size(); ++i)
    ht8.insert(pqueues[i], '\0');
  std::cout << "Hashed all permutations of: " << pqueues[0] << std::endl;
  std::cout << "Hash contents: " << std::endl << ht8 << std::endl;
  TEST_EQUAL(ht8.keys().size(), 120, "Hash should have 120 keys");
  contains_all = true;
  for (size_t i = 0; i < pqueues.size(); ++i) {
    if (!ht8.contains(pqueues[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all permutations as keys");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH graph_node<int> KEYS" << std::endl;

  // Create a node with each prime number in the original array.
  dyn_array<graph_node<int> > gnodes;
  for (size_t i = 0; i < primes.size(); ++i)
    gnodes.add(graph_node<int>(primes[i], 4));
  // Compute permutations of the graph_node array.
  dyn_array<dyn_array<graph_node<int> > > gn_perms = gnodes.permutations();
  // In each permutation, make the other nodes neighbors of the first.
  for (size_t i = 0; i < gn_perms.size(); ++i)
    for (size_t n = 1; n < 5; ++n)
      gn_perms[i][0][n-1] = &gn_perms[i][n];
  std::cout << "Created graph nodes from permutation arrays:" << std::endl;
  // Since permutations of identical neighbor values are considered equal if
  // node values and indices also match, inserting all 120 of these graph nodes
  // should result in only 5 keys, duplicates replacing existing values.
  hash_table<graph_node<int>, char> ht9(47);
  for (size_t i = 0; i < gn_perms.size(); ++i)
    ht9.insert(gn_perms[i][0], '\0');
  std::cout << "Hashed all permutations of: " << gn_perms[0][0] << std::endl;
  TEST_EQUAL(ht9.keys().size(), 5, "Hash should have 5 keys");
  TEST_CHECK(ht9.contains(gn_perms[0][0]),  "Hash should have 2:{11,7,5,3}");
  TEST_CHECK(ht9.contains(gn_perms[24][0]), "Hash should have 3:{11,7,5,2}");
  TEST_CHECK(ht9.contains(gn_perms[48][0]), "Hash should have 5:{11,7,3,2}");
  TEST_CHECK(ht9.contains(gn_perms[72][0]), "Hash should have 7:{11,5,3,2}");
  TEST_CHECK(ht9.contains(gn_perms[96][0]), "Hash should have 11:{7,5,3,2}");

  // Now go through all the nodes and change the last neighbor of each one
  // to unique value.
  int unique_val = 12;
  for (size_t i = 0; i < gn_perms.size(); ++i)
    gn_perms[i][0][3]->set_val(unique_val++);
  std::cout << "Created unique graph nodes:" << std::endl;
  // Inserting these unique 120 graph nodes should result in 125 total keys.
  for (size_t i = 0; i < gn_perms.size(); ++i)
    ht9.insert(gn_perms[i][0], '\0');
  std::cout << "Hashed all unique graph nodes" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht9 << std::endl;
  TEST_EQUAL(ht9.keys().size(), 125, "Hash should have 125 keys");
  contains_all = true;
  for (size_t i = 0; i < gn_perms.size(); ++i) {
    if (!ht9.contains(gn_perms[i][0])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all unique nodes as keys");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH graph<int> KEYS" << std::endl;

  // Recompute permutations of the graph_node array.
  dyn_array<dyn_array<graph_node<int> > > node_perms = gnodes.permutations();
  dyn_array<graph<int> > graphs(graph<int>(), node_perms.size());
  // In each permutation, make the other nodes neighbors of the first.
  for (size_t i = 0; i < node_perms.size(); ++i) {
    for (size_t n = 1; n < 5; ++n)
      node_perms[i][0][n-1] = &node_perms[i][n];
    // Create a graph out of the connected graph nodes.
    graphs[i] = &node_perms[i][0];
  }
  std::cout << "Created graphs from permutation arrays:" << std::endl;

  hash_table<graph<int>, char> ht10(47);
  for (size_t i = 0; i < graphs.size(); ++i)
    ht10.insert(graphs[i], '\0');
  std::cout << "Hashed all permutations of: " << gn_perms[0][0] << std::endl;
  TEST_EQUAL(ht10.keys().size(), 5, "Hash should have 5 keys");
  TEST_CHECK(ht10.contains(graphs[3]),  "Contains 2{3,5,7,11}3{}5{}7{}11{}");
  TEST_CHECK(ht10.contains(graphs[27]), "Contains 2{}3{2,5,7,11}5{}7{}11{}");
  TEST_CHECK(ht10.contains(graphs[51]), "Contains 2{}3{}5{2,3,7,11}7{}11{}");
  TEST_CHECK(ht10.contains(graphs[75]), "Contains 2{}3{}5{}7{2,3,5,11}11{}");
  TEST_CHECK(ht10.contains(graphs[99]), "Contains 2{}3{}5{}7{}11{2,3,5,7}");

  // Now create and hash unique graphs.
  dyn_array<graph<int> > gunique(graph<int>(), node_perms.size());
  unique_val = 12;
  for (size_t i = 0; i < node_perms.size(); ++i) {
    node_perms[i][0][3]->set_val(unique_val++);
    gunique[i] = &node_perms[i][0];
  }
  std::cout << "Created unique graph nodes:" << std::endl;
  // Inserting these unique 120 graph nodes should result in 125 total keys.
  for (size_t i = 0; i < gunique.size(); ++i)
    ht10.insert(gunique[i], '\0');
  std::cout << "Hashed all unique graph nodes" << std::endl;
  TEST_EQUAL(ht10.keys().size(), 125, "Hash should have 125 keys");
  contains_all = true;
  for (size_t i = 0; i < gunique.size(); ++i) {
    if (!ht10.contains(gunique[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all unique nodes as keys");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH binary_tree<int, false> KEYS" << std::endl;

  dyn_array<binary_tree<int, false>*> btnodes(nullptr, perms.size());
  for (size_t i = 0; i < perms.size(); ++i) {
    // Create a binary tree root node with the first value in perms[i].
    btnodes[i] = binary_tree<int, false>::STANDARD(perms[i][0]);
    // Add the remaining values in level-order, creating similar trees.
    for (size_t j = 1; j < perms[i].size(); ++j)
      btnodes[i]->insert(perms[i][j]);
  }

  hash_table<binary_tree<int, false>, size_t> ht11(107);
  for (size_t i = 0; i < btnodes.size(); ++i)
    ht11.insert(*btnodes[i], i);
  std::cout << "Hashed all binary tree permutations" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht11 << std::endl;
  TEST_EQUAL(ht11.keys().size(), 120, "Hash should have 120 keys");
  contains_all = true;
  for (size_t i = 0; i < btnodes.size(); ++i) {
    if (!ht11.contains(*btnodes[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all permutations as keys");

  dyn_array<binary_tree<int, false>*> exnodes(nullptr, perms.size());
  for (size_t i = 0; i < perms.size(); ++i) {
    exnodes[i] = binary_tree<int, false>::create(perms[i][0]);
    exnodes[i]->left() = binary_tree<int, false>::create(perms[i][1]);
    exnodes[i]->left()->left() = binary_tree<int, false>::create(perms[i][3]);
    exnodes[i]->right() = binary_tree<int, false>::create(perms[i][2]);
    exnodes[i]->right()->left() = binary_tree<int, false>::create(perms[i][4]);
  }
  std::cout << "Built trees with slightly different topologies" << std::endl;
  bool unique_keys = true;
  for (size_t i = 0; i < exnodes.size(); ++i) {
    if (ht11.contains(*exnodes[i]))
      unique_keys = false;
    ht11.insert(*exnodes[i], i+1000);
  }
  TEST_CHECK(unique_keys, "Despite similarities, all keys should be unique");
  TEST_EQUAL(ht11.keys().size(), 240, "Hash should have 240 keys");
  contains_all = true;
  for (size_t i = 0; i < exnodes.size(); ++i) {
    if (!ht11.contains(*exnodes[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all 240 trees");

  for (size_t i = 0; i < btnodes.size(); ++i)
    delete btnodes[i];
  for (size_t i = 0; i < exnodes.size(); ++i)
    delete exnodes[i];
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH binary_search_tree<int, 1> KEYS" << std::endl;

  dyn_array<binary_search_tree<int, 1>*> bstnodes(nullptr, perms.size());
  // Creating binary search trees out of all the permutations of the 5 prime
  // numbers without balancing will result in 42 distinct keys instead of 120
  // due to the structural properties of a binary search tree.
  for (size_t i = 0; i < perms.size(); ++i) {
    // Create a binary search tree root node with the first value in perms[i].
    bstnodes[i] = binary_tree<int, 1>::SEARCH(perms[i][0]);
    // Insert remaining values according to search tree constraints.
    for (size_t j = 1; j < perms[i].size(); ++j)
      bstnodes[i]->insert(perms[i][j]);
  }

  // If a key exists, append its value instead of replacing the existing one.
  hash_table<binary_search_tree<int, 1>, dyn_array<size_t> > ht12(47);
  for (size_t i = 0; i < bstnodes.size(); ++i) {
    if (ht12.contains(*bstnodes[i]))
      ht12.insert(*bstnodes[i], ht12[*bstnodes[i]].add(i));
    else
      ht12.insert(*bstnodes[i], dyn_array<size_t>(i));
  }
  std::cout << "Hashed all binary search tree permutations" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht12 << std::endl;
  TEST_EQUAL(ht12.keys().size(), 42, "Hash should have 42 keys");
  contains_all = true;
  for (size_t i = 0; i < bstnodes.size(); ++i) {
    if (!ht12.contains(*bstnodes[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all 240 trees");

  // Count the number of unique keys and min/max number of identical trees
  // per root node value.
  unsigned r2_unique, r3_unique, r5_unique, r7_unique, r11_unique;
  size_t r2_min, r3_min, r5_min, r7_min, r11_min;
  size_t r2_max, r3_max, r5_max, r7_max, r11_max;
  r2_unique = r3_unique = r5_unique = r7_unique = r11_unique = 0;
  r2_min = r3_min = r5_min = r7_min = r11_min = 100;
  r2_max = r3_max = r5_max = r7_max = r11_max = 0;
  dyn_array<bool> flags(false, bstnodes.size());
  dyn_array<size_t> vals;
  size_t val_cnt;
  for (size_t i = 0; i < bstnodes.size(); ++i) {
    if (flags[i])
      continue;
    if (i < 24) {
      r2_unique++;
      vals = ht12[*bstnodes[i]];
      for (size_t j = 0; j < vals.size(); ++j)
        flags[vals[j]] = true;
      if (r2_min > vals.size()) r2_min = vals.size();
      if (r2_max < vals.size()) r2_max = vals.size();
    }
    else if (i < 48) {
      r3_unique++;
      vals = ht12[*bstnodes[i]];
      for (size_t j = 0; j < vals.size(); ++j)
        flags[vals[j]] = true;
      if (r3_min > vals.size()) r3_min = vals.size();
      if (r3_max < vals.size()) r3_max = vals.size();
    }
    else if (i < 72) {
      r5_unique++;
      vals = ht12[*bstnodes[i]];
      for (size_t j = 0; j < vals.size(); ++j)
        flags[vals[j]] = true;
      if (r5_min > vals.size()) r5_min = vals.size();
      if (r5_max < vals.size()) r5_max = vals.size();
    }
    else if (i < 96) {
      r7_unique++;
      vals = ht12[*bstnodes[i]];
      for (size_t j = 0; j < vals.size(); ++j)
        flags[vals[j]] = true;
      if (r7_min > vals.size()) r7_min = vals.size();
      if (r7_max < vals.size()) r7_max = vals.size();
    }
    else {
      r11_unique++;
      vals = ht12[*bstnodes[i]];
      for (size_t j = 0; j < vals.size(); ++j)
        flags[vals[j]] = true;
      if (r11_min > vals.size()) r11_min = vals.size();
      if (r11_max < vals.size()) r11_max = vals.size();
    }
  }
  TEST_EQUAL(r2_unique, 14, "Root 2 should have 14 unique keys");
  TEST_EQUAL(r2_min, 1, "Min identical key count for root 2 should be 1");
  TEST_EQUAL(r2_max, 3, "Max identical key count for root 2 should be 3");
  TEST_EQUAL(r3_unique, 5, "Root 3 should have 5 unique keys");
  TEST_EQUAL(r3_min, 4, "Min identical key count for root 3 should be 4");
  TEST_EQUAL(r3_max, 8, "Max identical key count for root 3 should be 8");
  TEST_EQUAL(r5_unique, 4, "Root 3 should have 4 unique keys");
  TEST_EQUAL(r5_min, 6, "Min identical key count for root 5 should be 6");
  TEST_EQUAL(r5_max, 6, "Max identical key count for root 5 should be 6");
  TEST_EQUAL(r7_unique, 5, "Root 3 should have 5 unique keys");
  TEST_EQUAL(r7_min, 4, "Min identical key count for root 7 should be 4");
  TEST_EQUAL(r7_max, 8, "Max identical key count for root 7 should be 8");
  TEST_EQUAL(r11_unique, 14, "Root 3 should have 14 unique keys");
  TEST_EQUAL(r11_min, 1, "Min identical key count for root 11 should be 1");
  TEST_EQUAL(r11_max, 3, "Max identical key count for root 11 should be 3");
  val_cnt = ht12[*bstnodes[22]].size();
  TEST_EQUAL(val_cnt, 1, "Key of {2 11 7 3 5} should be unique");
  val_cnt = ht12[*bstnodes[2]].size();
  TEST_EQUAL(val_cnt, 2, "Key of {2 3 7 5 11} should match 1 other key");
  val_cnt = ht12[*bstnodes[26]].size();
  TEST_EQUAL(val_cnt, 8, "Key of {3 2 7 5 11} should match 7 other keys");
  val_cnt = ht12[*bstnodes[66]].size();
  TEST_EQUAL(val_cnt, 6, "Key of {5 11 2 3 7} should match 5 other keys");
  val_cnt = ht12[*bstnodes[87]].size();
  TEST_EQUAL(val_cnt, 4, "Key of {7 5 3 11 2} should match 3 other keys");
  val_cnt = ht12[*bstnodes[103]].size();
  TEST_EQUAL(val_cnt, 3, "Key of {11 3 2 7 5} should match 2 other keys");
  val_cnt = ht12[*bstnodes[115]].size();
  TEST_EQUAL(val_cnt, 1, "Key of {11 7 2 5 3} should be unique");

  for (size_t i = 0; i < bstnodes.size(); ++i)
    delete bstnodes[i];
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH heap_tree<int, 0, false> KEYS" << std::endl;

  dyn_array<heap_tree<int, 0, false>*> hptnodes(nullptr, perms.size());
  // Creating min heap trees out of all the permutations of the 5 prime
  // numbers without balancing will result in only 8 distinct keys instead of
  // 120 due to the structural properties of a heap tree.
  for (size_t i = 0; i < perms.size(); ++i) {
    // Create a min heap tree root node with the first value in perms[i]
    hptnodes[i] = binary_tree<int, 0>::MIN_HEAP(perms[i][0]);
    // Insert remaining values according to min heap tree constraints.
    for (size_t j = 1; j < perms[i].size(); ++j)
      hptnodes[i]->insert(perms[i][j]);
  }

  // If a key exists, append its value instead of replacing the existing one.
  hash_table<heap_tree<int, 0, false>, dyn_array<size_t> > ht13(47);
  for (size_t i = 0; i < hptnodes.size(); ++i) {
    if (ht13.contains(*hptnodes[i]))
      ht13.insert(*hptnodes[i], ht13[*hptnodes[i]].add(i));
    else
      ht13.insert(*hptnodes[i], dyn_array<size_t>(i));
  }
  std::cout << "Hashed all heap tree permutations" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht13 << std::endl;
  TEST_EQUAL(ht13.keys().size(), 8, "Hash should have 8 keys");
  TEST_CHECK(ht13.contains(*hptnodes[0]),   "Contains 2~+3~|+7~|>11~>5~");
  TEST_CHECK(ht13.contains(*hptnodes[2]),   "Contains 2~+3~|+5~|>11~>7~");
  TEST_CHECK(ht13.contains(*hptnodes[30]),  "Contains 2~+5~|+7~|>11~>3~");
  TEST_CHECK(ht13.contains(*hptnodes[34]),  "Contains 2~+3~|+5~|>7~>11~");
  TEST_CHECK(ht13.contains(*hptnodes[42]),  "Contains 2~+5~|+11~|>7~>3~");
  TEST_CHECK(ht13.contains(*hptnodes[82]),  "Contains 2~+3~|+7~|>5~>11~");
  TEST_CHECK(ht13.contains(*hptnodes[95]),  "Contains 2~+3~|+11~|>5~>7~");
  TEST_CHECK(ht13.contains(*hptnodes[111]), "Contains 2~+3~|+11~|>7~>5~");

  for (size_t i = 0; i < hptnodes.size(); ++i)
    delete hptnodes[i];
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH avl_tree<int, true> KEYS" << std::endl;

  dyn_array<avl_tree<int, true>*> avlnodes(nullptr, perms.size());
  // Creating AVL trees out of all the permutations of the 5 prime numbers
  // without manual balancing will result in only 8 distinct keys instead of
  // the 42 that binary search trees generated because self-balancing routine
  // in an AVL tree creates a set with many more identical tree structures.
  for (size_t i = 0; i < perms.size(); ++i) {
    // Create an AVL tree root node with the first value in perms[i].
    avlnodes[i] = binary_tree<int, true>::AVL(perms[i][0]);
    // Insert remaining values according to AVL tree constraints.
    for (size_t j = 1; j < perms[i].size(); ++j)
      avlnodes[i] = avlnodes[i]->insert(perms[i][j]);
  }
  // If a key exists, append its value instead of replacing the existing one.
  hash_table<avl_tree<int, true>, dyn_array<size_t> > ht14(47);
  for (size_t i = 0; i < avlnodes.size(); ++i) {
    if (ht14.contains(*avlnodes[i]))
      ht14.insert(*avlnodes[i], ht14[*avlnodes[i]].add(i));
    else
      ht14.insert(*avlnodes[i], dyn_array<size_t>(i));
  }
  std::cout << "Hashed all AVL tree permutations" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht14 << std::endl;
  TEST_EQUAL(ht14.keys().size(), 6, "Hash should have 6 keys");
  TEST_CHECK(ht14.contains(*avlnodes[1]),    "Should contain unique key #1");
  TEST_CHECK(ht14.contains(*avlnodes[9]),    "Should contain unique key #2");
  TEST_CHECK(ht14.contains(*avlnodes[45]),   "Should contain unique key #3");
  TEST_CHECK(ht14.contains(*avlnodes[66]),   "Should contain unique key #4");
  TEST_CHECK(ht14.contains(*avlnodes[81]),   "Should contain unique key #5");
  TEST_CHECK(ht14.contains(*avlnodes[100]),  "Should contain unique key #6");

  for (size_t i = 0; i < avlnodes.size(); ++i)
    delete avlnodes[i];
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH red_black_tree<int> KEYS" << std::endl;

  dyn_array<red_black_tree<int>*> rbnodes(nullptr, perms.size());
  // Creating red-black trees out of all the permutations of the 5 prime
  // numbers without manual balancing will result in only 6 distinct keys,
  // same number as AVL trees. Despite the looser self-balancing and colorized
  // nodes creating additional distinctions to trees, on very small trees both
  // self-balancing trees generate the same number of duplicate structures.
  for (size_t i = 0; i < perms.size(); ++i) {
    // Create a red-black tree root node with the first value in perms[i].
    rbnodes[i] = binary_tree<int, true>::RED_BLACK(perms[i][0]);
    // Insert remaining values according to red-black tree constraints.
    for (size_t j = 1; j < perms[i].size(); ++j)
      rbnodes[i] = rbnodes[i]->insert(perms[i][j]);
  }

  // If a key exists, append its value instead of replacing the existing one.
  hash_table<red_black_tree<int>, dyn_array<size_t> > ht15(47);
  for (size_t i = 0; i < rbnodes.size(); ++i) {
    if (ht15.contains(*rbnodes[i]))
      ht15.insert(*rbnodes[i], ht15[*rbnodes[i]].add(i));
    else
      ht15.insert(*rbnodes[i], dyn_array<size_t>(i));
  }
  std::cout << "Hashed all red-black tree permutations" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht15 << std::endl;
  TEST_EQUAL(ht15.keys().size(), 6, "Hash should have 6 keys");
  TEST_CHECK(ht15.contains(*rbnodes[8]),    "Should contain unique key #1");
  TEST_CHECK(ht15.contains(*rbnodes[17]),   "Should contain unique key #2");
  TEST_CHECK(ht15.contains(*rbnodes[53]),   "Should contain unique key #3");
  TEST_CHECK(ht15.contains(*rbnodes[59]),   "Should contain unique key #4");
  TEST_CHECK(ht15.contains(*rbnodes[102]),  "Should contain unique key #5");
  TEST_CHECK(ht15.contains(*rbnodes[107]),  "Should contain unique key #6");

  for (size_t i = 0; i < rbnodes.size(); ++i)
    delete rbnodes[i];
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH heap<int> KEYS" << std::endl;

  // Creating array-based heaps out of all the permutations of the 5 prime
  // numbers will result in only 8 distinct keys instead of the 120 that
  // dyn_array hashing generated because of heap properties. Notice that the
  // number of keys matches the number distinct keys created by heap_tree,
  // even though the distribution of hash keys in two cases are different.
  dyn_array<heap<int> > pheaps(heap<int>(), perms.size());
  for (size_t i = 0; i < perms.size(); ++i)
    pheaps[i] = perms[i];
  std::cout << "Created heaps from permutation arrays" << std::endl;

  heap<int> key;
  hash_table<heap<int>, dyn_array<size_t> > ht16(47);
  for (size_t i = 0; i < pheaps.size(); ++i) {
    if (ht16.contains(pheaps[i]))
      ht16.insert(pheaps[i], ht16[pheaps[i]].add(i));
    else
      ht16.insert(pheaps[i], dyn_array<size_t>(i));
  }
  std::cout << "Hashed all permutations of: " << pheaps[0] << std::endl;
  std::cout << "Hash contents: " << std::endl << ht16 << std::endl;
  bool check = ht13.keys().size() == ht16.keys().size();
  TEST_CHECK(check, "Hash should have the same number of keys as heap_tree");
  key = {2, 3, 7, 5, 11};
  TEST_CHECK(ht16.contains(key), "Hash should contain key {2 3 7 5 11}");
  key = {2, 3, 11, 7, 5};
  TEST_CHECK(ht16.contains(key), "Hash should contain key {2 3 11 7 5}");
  key = {2, 3, 5, 7, 11};
  TEST_CHECK(ht16.contains(key), "Hash should contain key {2 3 5 7 11}");
  key = {2, 5, 3, 7, 11};
  TEST_CHECK(ht16.contains(key), "Hash should contain key {2 5 3 7 11}");
  key = {2, 3, 7, 11, 5};
  TEST_CHECK(ht16.contains(key), "Hash should contain key {2 3 7 11 5}");
  key = {2, 3, 11, 5, 7};
  TEST_CHECK(ht16.contains(key), "Hash should contain key {2 3 11 5 7}");
  key = {2, 3, 5, 11, 7};
  TEST_CHECK(ht16.contains(key), "Hash should contain key {2 3 5 11 7}");
  key = {2, 5, 3, 11, 7};
  TEST_CHECK(ht16.contains(key), "Hash should contain key {2 5 3 11 7}");
  heap<int> wrong_key(true);
  wrong_key = {2, 3, 11, 7, 5};
  TEST_CHECK(!ht16.contains(wrong_key), "Hash should not contain wrong keys");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH trie KEYS" << std::endl;

  trie* ts_eliot = trie::create();
  dyn_array<trie*> stanzas(nullptr, 20);
  hash_table<trie, unsigned short> ht17(17);

  *ts_eliot = "We are the hollow men "
              "We are the stuffed men "
              "Leaning together "
              "Headpiece filled with straw. Alas! "
              "Our dried voices, when "
              "We whisper together "
              "Are quiet and meaningless "
              "As wind in dry grass "
              "Or rats' feet over broken glass "
              "In our dry cellar";
  ht17.insert(*ts_eliot, 1);
  stanzas[0] = ts_eliot->clone();

  *ts_eliot = "Shape without form, shade without colour. "
              "Paralysed force, gesture without motion;";
  ht17.insert(*ts_eliot, 1);
  stanzas[1] = ts_eliot->clone();

  *ts_eliot = "Those who have crossed "
              "With direct eyes, to death's other Kingdom "
              "Remember us-if at all-not as lost "
              "Violent souls, but only "
              "As the hollow men";
  ht17.insert(*ts_eliot, 1);
  stanzas[2] = ts_eliot->clone();

  *ts_eliot = "Eyes I dare not meet in dreams "
              "In death's dream kingdom "
              "These do not appear: "
              "There, the eyes are "
              "Sunlight on a broken column "
              "There, is a tree swinging "
              "And voices are "
              "In the wind's singing "
              "More distant and more solemn "
              "Than a fading star.";
  ht17.insert(*ts_eliot, 2);
  stanzas[3] = ts_eliot->clone();

  *ts_eliot = "Let me be no nearer "
              "In death's dream kingdom "
              "Let me also wear "
              "Such deliberate disguises "
              "Rat's coat, crowskin, crossed staves "
              "In a field "
              "Behaving as the wind behaves "
              "No nearer-";
  ht17.insert(*ts_eliot, 2);
  stanzas[4] = ts_eliot->clone();

  *ts_eliot = "Not that final meeting "
              "In the twilight kingdom";
  ht17.insert(*ts_eliot, 2);
  stanzas[5] = ts_eliot->clone();

  *ts_eliot = "This is the dead land "
              "This is cactus land "
              "Here the stone images "
              "Are raised, here they receive "
              "The supplication of a dead man's hand "
              "Under the twinkle of a fading star.";
  ht17.insert(*ts_eliot, 3);
  stanzas[6] = ts_eliot->clone();

  *ts_eliot = "Is it like this "
              "In death's other kingdom "
              "Waking alone "
              "At the hour when we are "
              "Trembling with tenderness "
              "Lips that would kiss "
              "Form prayers to broken stone.";
  ht17.insert(*ts_eliot, 3);
  stanzas[7] = ts_eliot->clone();

  *ts_eliot = "The eyes are not here "
              "There are no eyes here "
              "In this valley of dying stars "
              "In this hollow valley "
              "This broken jaw of our lost kingdoms";
  ht17.insert(*ts_eliot, 4);
  stanzas[8] = ts_eliot->clone();

  *ts_eliot = "In this last of meeting places "
              "We grope together "
              "And avoid speech "
              "Gathered on this beach of the tumid river";
  ht17.insert(*ts_eliot, 4);
  stanzas[9] = ts_eliot->clone();

  *ts_eliot = "Sightless, unless "
              "The eyes reappear "
              "As the perpetual star "
              "Multifoliate rose "
              "Of death's twilight kingdom "
              "The hope only "
              "Of empty men.";
  ht17.insert(*ts_eliot, 4);
  stanzas[10] = ts_eliot->clone();

  *ts_eliot = "Here we go round the prickly pear "
              "Prickly pear prickly pear "
              "Here we go round the prickly pear "
              "At five o'clock in the morning.";
  ht17.insert(*ts_eliot, 5);
  stanzas[11] = ts_eliot->clone();

  *ts_eliot = "Between the idea "
              "And the reality "
              "Between the motion "
              "And the act "
              "Falls the Shadow";
  ht17.insert(*ts_eliot, 5);
  stanzas[12] = ts_eliot->clone();

  *ts_eliot = "For Thine is the Kingdom";
  ht17.insert(*ts_eliot, 5);
  stanzas[13] = ts_eliot->clone();

  *ts_eliot = "Between the conception "
              "And the creation "
              "Between the emotion "
              "And the response "
              "Falls the Shadow";
  ht17.insert(*ts_eliot, 5);
  stanzas[14] = ts_eliot->clone();

  *ts_eliot = "Life is very long";
  ht17.insert(*ts_eliot, 5);
  stanzas[15] = ts_eliot->clone();

  *ts_eliot = "Between the desire "
              "And the spasm "
              "Between the potency "
              "And the existence "
              "Between the essence "
              "And the descent "
              "Falls the Shadow";
  ht17.insert(*ts_eliot, 5);
  stanzas[16] = ts_eliot->clone();

  *ts_eliot = "For Thine is the Kingdom";
  ht17.insert(*ts_eliot, 5);
  stanzas[17] = ts_eliot->clone();

  *ts_eliot = "For Thine is "
              "Life is "
              "For Thine is the";
  ht17.insert(*ts_eliot, 5);
  stanzas[18] = ts_eliot->clone();

  *ts_eliot = "This is the way the world ends "
              "This is the way the world ends "
              "This is the way the world ends "
              "Not with a bang but a whimper.";
  ht17.insert(*ts_eliot, 5);
  stanzas[19] = ts_eliot->clone();

  std::cout << "Hashed The Hollow Men as 20 tries with sections as values";
  std::cout << std::endl;
  std::cout << "Hash contents: " << std::endl << ht17 << std::endl;
  TEST_EQUAL(ht17.keys().size(), 19, "Hash should have 19 keys");
  contains_all = true;
  for (size_t i = 0; i < stanzas.size(); ++i) {
    if (!ht17.contains(*stanzas[i])) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain all stanzas as keys");

  delete ts_eliot;
  for (size_t i = 0; i < stanzas.size(); ++i)
    delete stanzas[i];
  test_util::print_summary();
  return 0;
}