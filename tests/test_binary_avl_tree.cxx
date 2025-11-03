#include <iostream>
#include <random>
#include <algorithm>
#include "binary_tree.h"
#include "test_util.h"

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "AVL TREE WITH RANDOM PRIME NUMBERS" << std::endl << std::endl;

  int primes[50] = {  2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                     41,  43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,
                     97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
                    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
                    227, 229};

  // Random number generator to pick from the first 50 primes in random order
  // Once a prime is inserted, it won't be inserted again.
  std::random_device rd;
  std::mt19937 eng(rd());
  // Shuffle the array of primes
  std::shuffle(primes, primes + 50, eng);

  avl_tree<int, 0>* avl = binary_tree<int, 0>::AVL(primes[0]);
  dyn_array<int> avl_gt(primes[0]);
  bt_ptrs<int, 0> in_path;
  bool valid, sorted, search, balanced, check;
  size_t old_nc, old_sz, exp_sz;
  dyn_array<int> unq_gt;
  int bf;

  valid = balanced = search = sorted = true;
  for (int i = 1; i < 50; ++i) {
    avl = avl->insert(primes[i]);
    avl_gt.add(primes[i]);
    if (!avl->is_search())
      search = false;
    if (!avl->is_balanced())
      balanced = false;
    bf = avl->balance_factor();
    if (bf < -1 || bf > 1)
      valid = false;
    in_path = avl->traverse(IN_ORDER);
    unq_gt = avl_gt.unique();
    unq_gt.quick_sort();
    for (size_t i = 0; i < in_path.size(); ++i) {
      if (in_path[i]->val() != unq_gt[i]) {
        sorted = false;
        break;
      }
    }
  }
  std::cout << "Inserted first 50 primes in random order:" << std::endl;
  TEST_CHECK(search, "AVL tree should have always remained a search tree");
  TEST_CHECK(balanced, "AVL tree should have always remained a balanced tree");
  TEST_CHECK(valid, "Balance factor should have always been in range [-1, 1]");
  TEST_CHECK(sorted, "In-order traversal visits should be in sorted order");
  TEST_EQUAL(avl->search(71)->val(), 71, "Search for 71 should find 71");
  TEST_EQUAL(avl->search(500), nullptr, "Search for 500 should not find it");
  TEST_EQUAL(avl->find_closest(500), 229, "Closest input prime to 500 is 229");
  TEST_EQUAL(avl->find_closest(104), 103, "Closest input prime to 104 is 103");
  TEST_EQUAL(avl->find_closest(31), 31, "Closest input prime to 31 is 31");
  TEST_EQUAL(avl->find_closest(-5), 2, "Closest input prime to -5 is 2");
  std::cout << "Resulting AVL tree:" << std::endl << avl << std::endl;

  dyn_array<int> small_test;
  small_test = {199, 2, 47, 97, 59, 73, 11, 13, 127, 31, 3, 5, 7, 229, 227};
  old_nc = avl->num_nodes();
  exp_sz = avl->size() + small_test.size();
  for (int i = 0; i < small_test.size(); ++i) {
    avl = avl->insert(small_test[i]);
    avl_gt.add(small_test[i]);
    if (!avl->is_search())
      search = false;
    if (!avl->is_balanced())
      balanced = false;
    bf = avl->balance_factor();
    if (bf < -1 || bf > 1)
      valid = false;
    in_path = avl->traverse(IN_ORDER);
    unq_gt = avl_gt.unique();
    unq_gt.quick_sort();
    for (size_t i = 0; i < in_path.size(); ++i) {
      if (in_path[i]->val() != unq_gt[i]) {
        sorted = false;
        break;
      }
    }
  }
  std::cout << "Inserted duplicates from the small test:" << std::endl;
  TEST_EQUAL(avl->size(), exp_sz, "Each duplicate should increase size by 1");
  TEST_EQUAL(avl->num_nodes(), old_nc, "Node count should not change");
  TEST_CHECK(search, "AVL tree should have always remained a search tree");
  TEST_CHECK(balanced, "AVL tree should have always remained a balanced tree");
  TEST_CHECK(valid, "Balance factor should have always been in range [-1, 1]");
  TEST_CHECK(sorted, "In-order traversal visits should be in sorted order");
  std::cout << "Resulting AVL tree:" << std::endl << avl << std::endl;

  std::cout << "Balanced the AVL tree:" << std::endl << avl->balance();
  std::cout << "Checking properties after balancing:" << std::endl;
  TEST_CHECK(avl->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avl->is_complete(), "AVL tree should be complete");
  TEST_CHECK(avl->is_search(), "AVL tree should be a binary search tree");
  check = avl->balance_factor() >= -1 && avl->balance_factor() <= 1;
  TEST_CHECK(check, "AVL tree balance factor should be in range [-1, 1]");
  std::cout << std::endl;

  old_sz = avl->size();
  old_nc = avl->num_nodes();
  const avl_tree<int, 0>* rm_avl = avl->right_ch()->right_ch()->right_ch();
  avl = avl->remove(rm_avl);
  small_test.remove();
  rm_avl = avl->right_ch()->right_ch()->right_ch();
  avl = avl->remove(rm_avl);
  small_test.remove();
  rm_avl = avl->left_ch()->left_ch()->left_ch();
  avl = avl->remove(rm_avl);
  small_test.remove();
  std::cout << "Removed 3 nodes with duplicate vals by pointer:" << std::endl;
  TEST_CHECK(avl->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avl->is_search(), "AVL tree should be a binary search tree");
  check = avl->balance_factor() >= -1 && avl->balance_factor() <= 1;
  TEST_CHECK(check, "AVL tree balance factor should be in range [-1, 1]");
  TEST_EQUAL(avl->size(), old_sz-6, "AVL tree size should shrink by 6");
  TEST_EQUAL(avl->num_nodes(), old_nc-3, "Node count should be reduced by 3");
  std::cout << "Resulting AVL tree:" << std::endl << avl << std::endl;

  std::shuffle(primes, primes + 50, eng);
  balanced = search = valid = true;
  std::cout << "Removing duplicate elements in random order:" << std::endl;
  for (size_t i = 0; i < small_test.size(); ++i) {
    avl = avl->remove(small_test[i]);
    if (avl) {
      if (!avl->is_search())
        search = false;
      if (!avl->is_balanced())
        balanced = false;
      bf = avl->balance_factor();
      if (bf < -1 || bf > 1)
        valid = false;
    }
  }
  TEST_CHECK(search, "AVL tree should have always remained a search tree");
  TEST_CHECK(balanced, "AVL tree should have always remained a balanced tree");
  TEST_CHECK(valid, "Balance factor should have always beed in range [-1, 1]");
  std::cout << "Resulting AVL tree:" << std::endl << avl << std::endl;

  std::cout << "Balanced the AVL tree:" << std::endl << avl->balance();
  TEST_CHECK(avl->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avl->is_complete(), "AVL tree should be complete");
  TEST_CHECK(avl->is_search(), "AVL tree should be a binary search tree");
  check = avl->balance_factor() >= -1 && avl->balance_factor() <= 1;
  TEST_CHECK(check, "AVL tree balance factor should be in range [-1, 1]");
  std::cout << std::endl;

  balanced = search = valid = true;
  avl = avl->insert(7);
  avl = avl->insert(229);
  avl = avl->insert(227);
  size_t i = 0;
  while (avl) {
    avl = avl->remove(primes[i++]);
    if (avl) {
      if (!avl->is_search())
        search = false;
      if (!avl->is_balanced())
        balanced = false;
      bf = avl->balance_factor();
      if (bf < -1 || bf > 1)
        valid = false;
    }
  }
  std::cout << "Removed remaining elements in random order: " << std::endl;
  TEST_CHECK(search, "AVL tree should have always remained a search tree");
  TEST_CHECK(balanced, "AVL tree should have always remained a balanced tree");
  TEST_CHECK(valid, "Balance factor should have always beed in range [-1, 1]");
  TEST_EQUAL(avl, nullptr, "AVL tree should be empty after removals");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "AVL TREE ASSIGNMENT TESTS" << std::endl << std::endl;

  // Initialize all binary tree types with a 0 root node, then assign Recaman.
  dyn_array<int> ulam, recaman, trav;
  ulam = {1, 2, 3, 4, 6, 8, 11, 13, 16, 18, 26, 28, 36, 38, 47, 48, 53, 57};
  recaman = { 0,  1,   3,  6,   2,  7, 13, 20, 12, 21, 11, 22, 10, 23,  9, 24,
              8, 25,  43, 62,  42, 63, 41, 18, 42, 17, 43, 16, 44, 15, 45, 14,
             46, 79, 113, 78, 114, 77, 39, 78, 38, 79, 37, 80, 36, 81, 35, 82,
             34, 83,  33, 84,  32, 85, 31, 86, 30, 87, 29, 88, 28, 89, 27, 90};

  binary_tree<int, 1> *bt, *btc;
  binary_search_tree<int, 1> *bst;
  avl_tree<int, 1> *avlp, *avl_dyn, *avlc, *avlcl;
  red_black_tree<int> *rb;

  bt      = binary_tree<int, true>::STANDARD(0);
  btc  = binary_tree<int, true>::STANDARD(0);
  bst     = binary_tree<int, true>::SEARCH(0);
  avlp    = binary_tree<int, true>::AVL(0);
  avlc    = binary_tree<int, true>::AVL(0);
  avl_dyn = binary_tree<int, true>::AVL(0);
  rb      = binary_tree<int, true>::RED_BLACK(0);

  *btc  = recaman;
  *bst  = recaman;

  *avlp = { 0,  1,   3,  6,   2,  7, 13, 20, 12, 21, 11, 22, 10, 23,  9, 24,
            8, 25,  43, 62,  42, 63, 41, 18, 42, 17, 43, 16, 44, 15, 45, 14,
           46, 79, 113, 78, 114, 77, 39, 78, 38, 79, 37, 80, 36, 81, 35, 82,
           34, 83,  33, 84,  32, 85, 31, 86, 30, 87, 29, 88, 28, 89, 27, 90};
  *avl_dyn = recaman;
  std::cout << "Recaman's seq: init list -> avl tree:" << std::endl;
  TEST_CHECK(*avlp == *bst, "Init list trees of avl and bst should match");
  TEST_CHECK(avlp->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avlp->is_complete(), "AVL tree should be complete");
  TEST_CHECK(avlp->is_search(), "AVL tree should be search tree");
  TEST_EQUAL(avlp->height(), 5, "AVL tree height should be 5");
  std::cout << "Recaman's seq: dyn_array -> avl tree:" << std::endl;
  TEST_CHECK(*avlp == *avl_dyn, "Init list and dyn_array trees should match");
  TEST_CHECK(avl_dyn->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avl_dyn->is_complete(), "AVL tree should be complete");
  TEST_CHECK(avl_dyn->is_search(), "AVL tree should be search tree");
  TEST_EQUAL(avl_dyn->height(), 5, "AVL tree height should be 5");
  std::cout << "Resulting AVL tree:" << std::endl << avlp << std::endl;

  *avlp = ulam.shuffle();
  *bt = *avlp;
  std::cout << "Ulam numbers: avl tree -> binary tree:" << std::endl;
  TEST_CHECK(*bt == *avlp, "Structures of bt and avlp should match");
  TEST_CHECK(bt->height() == avlp->height(), "Heights should also match");
  TEST_EQUAL(bt->size(), avlp->size(), "Sizes of bt and avlp should match");
  check = bt->left()->type() != avlp->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bt and avlp should differ");
  TEST_CHECK(bt->is_balanced(), "Binary tree should be balanced");
  TEST_CHECK(bt->is_complete(), "Binary tree should be complete");
  TEST_CHECK(bt->is_search(), "Binary tree should be a search tree");
  TEST_CHECK(*btc != *avlp, "Structures of btc and avlp should differ");
  TEST_EQUAL(btc->height(), 0, "Height of btc should not be computed");
  TEST_EQUAL(avlp->height(), 4, "AVL tree height should be 4");
  TEST_CHECK(*bst != *avlp, "Structures of bst and avlp should differ");
  bf = avlp->search(13)->parent()->val();
  TEST_EQUAL(bf, 16, "Parent of 13in the AVL tree should initially be 16");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  *avlp = *btc;
  std::cout << "Recaman's seq: binary tree -> avl tree:" << std::endl;
  TEST_CHECK(*avlp != *btc, "Structures of avlp and btc should differ");
  TEST_CHECK(*avlp == *bst, "Structures of avlp and bst should now match");
  check = btc->size() == avlp->size();
  TEST_CHECK(check, "Sizes of btc and avlp should match");
  check = avlp->left_ch()->type() != btc->left()->type();
  TEST_CHECK(check, "Dynamic types of avlp and btc should differ");
  TEST_CHECK(avlp->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avlp->is_complete(), "AVL tree should be complete");
  TEST_CHECK(avlp->is_search(), "AVL tree should be search tree");
  TEST_EQUAL(avlp->height(), 5, "AVL tree height should now be 5");
  TEST_EQUAL(bst->height(), 0, "Height of bst should not yet be computed");
  bf = avlp->search(13)->parent()->val();
  TEST_EQUAL(bf, 9, "Parent of 13in the AVL tree should now be 9");
  std::cout << "Resulting AVL tree:" << std::endl << avlp << std::endl;

  *avlp = ulam.shuffle();
  *bst = *avlp;
  std::cout << "Ulam numbers: avl tree -> binary search tree:" << std::endl;
  TEST_CHECK(*bst == *avlp, "Structures of bst and avlp should match");
  TEST_CHECK(bst->height() == avlp->height(), "Heights should also match");
  TEST_CHECK(bst->size() == avlp->size(), "Sizes should also match");
  check = bst->left_ch()->type() != avlp->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bst and avlp should differ");
  TEST_CHECK(bst->is_balanced(), "Binary search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Binary search tree should be complete");
  TEST_CHECK(avlp->is_complete(), "AVL tree should also be complete");
  TEST_CHECK(bst->is_search(), "Binary search tree should be a search tree");
  TEST_EQUAL(avlp->height(), 4, "AVL tree height should be 4");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  recaman.quick_sort();
  bst->clear()->set_val(recaman[0]);
  for (size_t i = 1; i < recaman.size(); ++i)
    bst->insert(recaman[i]);
  *avlp = *bst;
  std::cout << "Recaman's seq: binary search tree -> avl tree:" << std::endl;
  TEST_CHECK(*avlp != *bst, "Structures of avlp and bst should differ");
  TEST_CHECK(avlp->height() != bst->height(), "Heights should also differ");
  TEST_CHECK(avlp->size() == bst->size(), "But sizes should match");
  TEST_CHECK(*(bst->balance()) == *avlp, "Balanced structures should match");
  TEST_CHECK(bst->height() != avlp->height(), "Balanced heights should differ");
  check = avlp->left_ch()->type() != bst->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of avlp and bst should differ");
  TEST_EQUAL(avlp->height(), 5, "AVL tree height should now be 5");
  TEST_CHECK(avlp->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avlp->is_complete(), "AVL tree should be complete");
  TEST_CHECK(avlp->is_search(), "AVL tree should be search tree");
  TEST_EQUAL(avlc->height(), 0, "Height of avlc should be 0");
  std::cout << "Resulting AVL tree:" << std::endl << avlp << std::endl;

  *avlc = *avlp;
  avlcl = avlc->clone();
  std::cout << "Recaman's seq: avl tree -> avl tree:" << std::endl;
  TEST_CHECK(*avlp == *avlc, "Structures of avlp and avlc should match");
  TEST_CHECK(*avlp == *avlcl, "Structures of avlp and avlcl should match");
  check = avlp->size() == avlc->size();
  TEST_CHECK(check, "Sizes of avlp and avlc should match");
  check = avlp->size() == avlcl->size();
  TEST_CHECK(check, "Sizes of avlp and avlcl should match");
  check = avlp->height() == avlc->height();
  TEST_CHECK(check, "Heights of avlp and avlc should also match");
  check = avlp->height() == avlcl->height();
  TEST_CHECK(check, "Heights of avlp and avlcl should also match");
  check = avlp->left_ch()->type() == avlc->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of avlp and avlc should match");
  check = avlp->left_ch()->type() == avlcl->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of avlp and avlcl should match");
  check = avlc->is_balanced() && avlcl->is_balanced();
  TEST_CHECK(check, "Both AVL trees should be balanced");
  check = avlc->is_complete() && avlcl->is_complete();
  TEST_CHECK(check, "Both AVL trees should be complete");
  check = avlc->is_search() && avlcl->is_search();
  TEST_CHECK(check, "Both AVL trees should be binary search trees");
  std::cout << std::endl;

  *rb = ulam.shuffle();
  *avlp = *rb;
  std::cout << "Ulam numbers: red-black tree -> avl tree:" << std::endl;
  TEST_CHECK(*rb == *avlp, "Structures of rb and avlp should match");
  TEST_CHECK(avlp->height() == rb->height(), "Heights should also match");
  TEST_CHECK(avlp->size() == rb->size(), "Sizes should also match");
  TEST_EQUAL(avlp->height(), 4, "Tree heights should be 4");
  check = avlp->left_ch()->type() != rb->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of avlp and rb should differ");
  TEST_CHECK(avlp->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avlp->is_complete(), "AVL tree should be complete");
  TEST_CHECK(avlp->is_search(), "AVL tree should be search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  TEST_CHECK(*rb != *avlc, "Structures of rb and avlc should differ");
  std::cout << "Resulting AVL tree:" << std::endl << avlp << std::endl;

  *rb = *avlc;
  std::cout << "Recaman's seq: avl tree tree -> red-black tree:" << std::endl;
  TEST_CHECK(*avlc == *rb, "Structures of avlc and rb should now match");
  TEST_CHECK(avlc->height() == rb->height(), "Heights should also match");
  TEST_CHECK(avlc->size() == rb->size(), "Sizes should also match");
  TEST_EQUAL(rb->height(), 5, "Tree heights should be 5");
  check = rb->left_ch()->type() != avlc->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of rb and avlc should differ");
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_complete(), "Red-black tree should be complete");
  TEST_CHECK(rb->is_search(), "Red-black tree should be search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  delete bt;
  delete btc;
  delete bst;
  delete avlp;
  delete avlc;
  delete avlcl;
  delete avl_dyn;
  delete rb;
  test_util::print_summary();
  return 0;
}