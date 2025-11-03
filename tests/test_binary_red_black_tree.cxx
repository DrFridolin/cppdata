#include <iostream>
#include <random>
#include <algorithm>
#include "binary_tree.h"
#include "test_util.h"

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "RED-BLACK TREE WITH SMALL SET OF PRIMES" << std::endl;
  std::cout << std::endl;

  bool valid, sorted, search, check;
  size_t old_nc, old_sz;
  dyn_array<int> unq_gt, small_test;
  small_test = {199, 2, 47, 97, 59, 73, 11, 13, 127, 31, 3, 5, 7, 229, 227};

  red_black_tree<int>* rb = binary_tree<int, 1>::RED_BLACK(small_test[0]);
  dyn_array<int> path_gt;
  bt_ptrs<int, 1> pre, post, lvl, rb_path;
  dyn_array<int> rb_gt(small_test[0]);
  valid = search = sorted = true;
  for (int i = 1; i < small_test.size(); ++i) {
    rb = rb->insert(small_test[i]);
    rb_gt.add(small_test[i]);
    if (!rb->is_search())
      search = false;
    if (!rb->is_valid())
      valid = false;
    rb_path = rb->traverse(IN_ORDER);
    rb_gt.quick_sort();
    for (size_t i = 0; i < rb_path.size(); ++i) {
      if (rb_path[i]->val() != rb_gt[i]) {
        sorted = false;
        break;
      }
    }
  }
  std::cout << "Inserted 10 primes from small test set:" << std::endl;
  TEST_CHECK(search, "Red-black tree should always remain a search tree");
  TEST_CHECK(valid, "Red-black tree should never have any violations");
  TEST_CHECK(sorted, "In-order traversal visits should be in sorted order");
  path_gt = {47, 11, 3, 2, 5, 7, 13, 31, 97, 59, 73, 199, 127, 229, 227};
  std::cout << "Pre-order traversal path: ";
  pre = rb->traverse(PRE_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(pre, path_gt, 15, "Pre-order path should be correct");
  path_gt = {2, 7, 5, 3, 31, 13, 11, 73, 59, 127, 227, 229, 199, 97, 47};
  std::cout << "Post-order traversal path: ";
  post = rb->traverse(POST_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(post, path_gt, 15, "Post-order path should be correct");
  path_gt = {47, 11, 97, 3, 13, 59, 199, 2, 5, 31, 73, 127, 229, 7, 227};
  std::cout << "Level-order traversal path: ";
  lvl = rb->traverse(LEVEL_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(lvl, path_gt, 15, "Level-order path should be correct");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(rb->right_ch());
  std::cout << "Remove 97 (1 black 1 red child). Cases tested:" << std::endl;
  std::cout << "Case E: Black successor with black/null child" << std::endl;
  std::cout << "Case 3a: S black, NR (and maybe NL) red, N left" << std::endl;
  std::cout << "Case 3b: S black, NL red, N left" << std::endl;
  TEST_CHECK(rb->is_search(), "Red-black tree should be a binary search tree");
  path_gt = {47, 11, 3, 2, 5, 7, 13, 31, 127, 59, 73, 227, 199, 229};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {2, 7, 5, 3, 31, 13, 11, 73, 59, 199, 229, 227, 127, 47};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {47, 11, 127, 3, 13, 59, 227, 2, 5, 31, 73, 199, 229, 7};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(227);
  std::cout << "Remove 227 (2 black children). Cases tested:" << std::endl;
  std::cout << "Case E: Black successor with black/null child" << std::endl;
  std::cout << "Case 2: S black, NL and NR black" << std::endl;
  TEST_CHECK(rb->is_search(), "Red-black tree should be a binary search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  path_gt = {47, 11, 3, 2, 5, 7, 13, 31, 127, 59, 73, 229, 199};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {2, 7, 5, 3, 31, 13, 11, 73, 59, 199, 229, 127, 47};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {47, 11, 127, 3, 13, 59, 229, 2, 5, 31, 73, 199, 7};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(199);
  std::cout << "Remove 199 (0 children). Cases tested:" << std::endl;
  std::cout << "Case B: Red leaf node" << std::endl;
  TEST_CHECK(rb->is_search(), "Red-black tree should be a binary search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(127);
  std::cout << "Remove (127) (2 black children). Cases tested:" << std::endl;
  std::cout << "Case E: Black successor with black/null child" << std::endl;
  std::cout << "Case 4a: S black, NL (and maybe NR) red, N right" << std::endl;
  std::cout << "Case 4b: S black, NR red, N right" << std::endl;
  TEST_CHECK(rb->is_search(), "Red-black tree should be a binary search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  path_gt = {47, 11, 3, 2, 5, 7, 13, 31, 73, 59, 229};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {2, 7, 5, 3, 31, 13, 11, 59, 229, 73, 47};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {47, 11, 73, 3, 13, 59, 229, 2, 5, 31, 7};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting tree: " << std::endl << rb << std::endl;

  rb = rb->remove(31);
  std::cout << "Remove 31 (0 children). Cases tested:" << std::endl;
  std::cout << "Case B: Red leaf node" << std::endl;
  TEST_CHECK(rb->is_search(), "Red-black tree should be a binary search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(13);
  std::cout << "Remove (13) (0 children). Cases tested:" << std::endl;
  std::cout << "Case A: Black leaf node" << std::endl;
  std::cout << "Case 1b: S red, N right" << std::endl;
  std::cout << "Case 4a: S black, NL (and maybe NR) red, N right" << std::endl;
  std::cout << "Case 4b: S black, NR red, N right" << std::endl;
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  path_gt = {47, 3, 2, 7, 5, 11, 73, 59, 229};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {2, 5, 11, 7, 3, 59, 229, 73, 47};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {47, 3, 73, 2, 7, 59, 229, 5, 11};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(2);
  std::cout << "Remove (2) (0 children). Cases tested:" << std::endl;
  std::cout << "Case A: Black leaf node" << std::endl;
  std::cout << "Case 1a: S red, N left" << std::endl;
  std::cout << "Case 2: S black, NL and NR black" << std::endl;
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  path_gt = {47, 7, 3, 5, 11, 73, 59, 229};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {5, 3, 11, 7, 59, 229, 73, 47};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {47, 7, 73, 3, 11, 59, 229, 5};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(rb->left_ch());
  std::cout << "Remove (7) (2 black children). Cases tested:" << std::endl;
  std::cout << "Case E: Black successor with black/null child" << std::endl;
  std::cout << "Case 4a: S red, N left" << std::endl;
  std::cout << "Case 4b: S black, NL and NR black" << std::endl;
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  path_gt = {47, 5, 3, 11, 73, 59, 229};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {3, 11, 5, 59, 229, 73, 47};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {47, 5, 73, 3, 11, 59, 229};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(59);
  std::cout << "Remove (59) (0 children). Cases tested:" << std::endl;
  std::cout << "Case A: Black leaf node" << std::endl;
  std::cout << "Case 2: S black, NL and NR black" << std::endl;
  rb = rb->remove(rb);
  std::cout << "Remove (47) (1 black 1 red child). Cases tested:" << std::endl;
  std::cout << "Case F: At least 1 red among successor and child" << std::endl;
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  path_gt = {73, 5, 3, 11, 229};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {3, 11, 5, 229, 73};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {73, 5, 229, 3, 11};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(73);
  std::cout << "Remove (73) (1 black 1 red child). Cases tested:" << std::endl;
  std::cout << "Case E: Black successor with black/null child" << std::endl;
  std::cout << "Case 1b: S red, N right" << std::endl;
  std::cout << "Case 2: S black, NL and NR black" << std::endl;
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  path_gt = {5, 3, 229, 11};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {3, 11, 229, 5};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {5, 3, 229, 11};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(11);
  std::cout << "Remove <11> (0 children). Cases tested:" << std::endl;
  std::cout << "Case B: Red leaf node" << std::endl;
  rb = rb->remove(3);
  std::cout << "Remove (3) (0 children). Cases tested:" << std::endl;
  std::cout << "Case A: Black leaf node" << std::endl;
  std::cout << "Case 2: S black, NL and NR black" << std::endl;
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  path_gt = {5, 229};
  pre = rb->traverse(PRE_ORDER);
  TEST_PTR_SEQ(pre, path_gt, pre.size(), "Pre-order path should match gt");
  path_gt = {229, 5};
  post = rb->traverse(POST_ORDER);
  TEST_PTR_SEQ(post, path_gt, post.size(), "Post-order path should match gt");
  path_gt = {5, 229};
  lvl = rb->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(lvl, path_gt, lvl.size(), "Level-order path should match gt");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(5);
  std::cout << "Remove (5) (1 red child). Cases tested:" << std::endl;
  std::cout << "Case D: At least 1 red among node and child" << std::endl;
  TEST_CHECK(rb->root_only(), "Red black tree should be root-only");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  rb = rb->remove(229);
  std::cout << "Remove (229) (0 children), the final node." << std::endl;
  TEST_EQUAL(rb, nullptr, "Red black tree should be empty after removals");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "RED-BLACK TREE WITH RANDOM PRIMES" << std::endl << std::endl;

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

  rb = binary_tree<int, 1>::RED_BLACK(primes[0]);
  rb_gt.clear();
  rb_gt.add(primes[0]);
  valid = search = sorted = true;
  for (size_t i = 1; i < 50; ++i) {
    rb = rb->insert(primes[i]);
    rb_gt.add(primes[i]);
    if (!rb->is_search())
      search = false;
    if (!rb->is_valid())
      valid = false;
    rb_path = rb->traverse(IN_ORDER);
    rb_gt.quick_sort();
    for (size_t i = 0; i < rb_path.size(); ++i) {
      if (rb_path[i]->val() != rb_gt[i]) {
        sorted = false;
        break;
      }
    }
  }
  std::cout << "Inserted first 50 primes in random order:" << std::endl;
  TEST_CHECK(search, "Red-black tree should always remain a search tree");
  TEST_CHECK(valid, "Red-black tree should never have any violations");
  TEST_CHECK(sorted, "In-order traversal visits should be in sorted order");
  std::cout << "Resulting red-black tree:" << std::endl << rb << std::endl;

  small_test.shuffle();
  for (size_t i = 0; i < small_test.size(); ++i) {
    rb = rb->insert(small_test[i]);
    rb_gt.add(small_test[i]);
    if (!rb->is_search())
      search = false;
    if (!rb->is_valid())
      valid = false;
    rb_path = rb->traverse(IN_ORDER);
    unq_gt = rb_gt.unique();
    unq_gt.quick_sort();
    for (size_t i = 0; i < rb_path.size(); ++i) {
      if (rb_path[i]->val() != unq_gt[i]) {
        sorted = false;
        break;
      }
    }
  }
  std::cout << "Inserted duplicates from the small test:" << std::endl;
  TEST_CHECK(search, "Red-black tree should always remain a search tree");
  TEST_CHECK(valid, "Red-black tree should never have any violations");
  TEST_CHECK(sorted, "In-order traversal visits should be in sorted order");
  TEST_EQUAL(rb->search(11)->val(), 11, "Search for 11 should find 11");
  TEST_EQUAL(rb->search(81), nullptr, "Search for 81 should not find it");
  TEST_EQUAL(rb->find_closest(300), 229, "Closest input prime to 300 is 229");
  int c = rb->find_closest(81);
  TEST_CHECK((c == 79) || (c == 83), "Closest input prime to 81 is 79 or 83");
  TEST_EQUAL(rb->find_closest(199), 199, "Closest input prime to 199 is 199");
  TEST_EQUAL(rb->find_closest(-5), 2, "Closest input prime to -5 is 2");
  std::cout << "Resulting red-black tree: " << std::endl << rb << std::endl;

  std::cout << "Balanced the red-black tree:" << std::endl << rb->balance();
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_complete(), "Red-black tree should be complete");
  TEST_CHECK(rb->is_search(), "Red-black tree should be a binary search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << std::endl;

  old_sz = rb->size();
  old_nc = rb->num_nodes();
  const red_black_tree<int>* rem_rb = rb->left_ch()->left_ch()->left_ch();
  rb = rb->remove(rem_rb);
  rem_rb = rb->right_ch()->right_ch()->left_ch()->right_ch();
  rb = rb->remove(rem_rb);
  rem_rb = rb->left_ch()->left_ch()->right_ch()->left_ch()->right_ch();
  rb = rb->remove(rem_rb);
  std::cout << "Removed 3 nodes with duplicate vals by pointer:" << std::endl;
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_search(), "Red-black tree should be a binary search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  TEST_EQUAL(rb->size(), old_sz-6, "Red-black tree size should shrink by 6");
  TEST_EQUAL(rb->num_nodes(), old_nc-3, "Node count should be reduced by 3");
  std::cout << std::endl;

  std::shuffle(primes, primes + 50, eng);
  small_test = {2, 47, 97, 59, 73, 11, 13, 127, 3, 5, 229, 227};
  small_test.shuffle();
  valid = search = sorted = true;
  for (size_t i = 0; i < small_test.size(); ++i) {
    rb = rb->remove(small_test[i]);
    if (rb) {
      if (!rb->is_search())
        search = false;
      if (!rb->is_valid())
        valid = false;
    }
  }
  std::cout << "Removed duplicate elements in random order:" << std::endl;
  TEST_CHECK(search, "Red-black tree should always remain a search tree");
  TEST_CHECK(valid, "Red-black tree should never have any violations");
  std::cout << "Resulting red-black tree: " << std::endl << rb << std::endl;

  std::cout << "Balanced the red-black tree:" << std::endl << rb->balance();
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_complete(), "Red-black tree should be complete");
  TEST_CHECK(rb->is_search(), "Red-black tree should be a binary search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << std::endl;

  rb = rb->insert(7);
  rb = rb->insert(31);
  rb = rb->insert(199);
  int i = 0;
  std::cout << "Removing remaining elements in random order:" << std::endl;
  while (rb) {
    rb = rb->remove(primes[i++]);
    if (rb) {
      if (!rb->is_search())
        search = false;
      if (!rb->is_valid())
        valid = false;
    }
  }
  TEST_CHECK(search, "Red-black tree should always remain a search tree");
  TEST_CHECK(valid, "Red-black tree should never have any violations");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "RED-BLACK TREE ASSIGNMENT TESTS" << std::endl << std::endl;

  // Initialize all binary tree types with a 0 root node, then assign Recaman.
  dyn_array<int> ulam, recaman, trav;
  ulam = {1, 2, 3, 4, 6, 8, 11, 13, 16, 18, 26, 28, 36, 38, 47, 48, 53, 57};
  recaman = { 0,  1,   3,  6,   2,  7, 13, 20, 12, 21, 11, 22, 10, 23,  9, 24,
              8, 25,  43, 62,  42, 63, 41, 18, 42, 17, 43, 16, 44, 15, 45, 14,
             46, 79, 113, 78, 114, 77, 39, 78, 38, 79, 37, 80, 36, 81, 35, 82,
             34, 83,  33, 84,  32, 85, 31, 86, 30, 87, 29, 88, 28, 89, 27, 90};

  binary_tree<int, 1> *bt, *btc;
  binary_search_tree<int, 1> *bst;
  avl_tree<int, 1> *avlp;
  red_black_tree<int> *rb_dyn, *rbc, *rbcl;

  bt      = binary_tree<int, true>::STANDARD(0);
  btc     = binary_tree<int, true>::STANDARD(0);
  bst     = binary_tree<int, true>::SEARCH(0);
  avlp    = binary_tree<int, true>::AVL(0);
  rb      = binary_tree<int, true>::RED_BLACK(0);
  rbc     = binary_tree<int, true>::RED_BLACK(0);
  rb_dyn  = binary_tree<int, true>::RED_BLACK(0);

  *bt = recaman;
  *btc = recaman;
  *bst = recaman;
  *avlp = recaman;

  *rb = { 0,  1,   3,  6,   2,  7, 13, 20, 12, 21, 11, 22, 10, 23,  9, 24,
          8, 25,  43, 62,  42, 63, 41, 18, 42, 17, 43, 16, 44, 15, 45, 14,
         46, 79, 113, 78, 114, 77, 39, 78, 38, 79, 37, 80, 36, 81, 35, 82,
         34, 83,  33, 84,  32, 85, 31, 86, 30, 87, 29, 88, 28, 89, 27, 90};
  *rb_dyn = recaman;
  std::cout << "Recaman's seq: init list -> red-black tree:" << std::endl;
  TEST_CHECK(*rb == *bst, "bst insert and init list trees should match");
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_complete(), "Red-black tree should be complete");
  TEST_CHECK(rb->is_search(), "Red-black tree should be search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  TEST_EQUAL(rb->height(), 5, "Red-black tree height should be 5");
  std::cout << "Recaman's seq: dyn_array -> red-black tree:" << std::endl;
  TEST_CHECK(*rb_dyn == *bst, "bst insert and init list trees should match");
  TEST_CHECK(rb_dyn->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb_dyn->is_complete(), "Red-black tree should be complete");
  TEST_CHECK(rb_dyn->is_search(), "Red-black tree should be search tree");
  TEST_CHECK(rb_dyn->is_valid(), "Red-black tree should not have violations");
  TEST_EQUAL(rb_dyn->height(), 5, "Red-black tree height should be 5");
  std::cout << "Resulting red-black tree: " << std::endl << rb << std::endl;

  *rb = ulam.shuffle();
  *bt = *rb;
  std::cout << "Ulam numbers: red-black_tree -> binary tree:" << std::endl;
  TEST_CHECK(*bt == *rb, "Structures of bt and rb should match");
  TEST_CHECK(bt->size() == rb->size(), "Sizes should also match");
  TEST_CHECK(bt->height() == rb->height(), "Heights should also match");
  TEST_EQUAL(bt->height(), 4, "Tree heights should be 4");
  check = bt->left()->type() != rb->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bt and rb should differ");
  TEST_CHECK(bt->is_balanced(), "Binary tree should be balanced");
  TEST_CHECK(bt->is_complete(), "Binary tree should be complete");
  TEST_CHECK(bt->is_search(), "Binary tree should be a search tree");
  TEST_CHECK(*btc != *rb, "Structures of btc and rb should not match");
  TEST_EQUAL(btc->height(), 0, "Height of btc should not be computed");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  *rb = *btc;
  std::cout << "Recaman's seq: binary tree -> red-black tree:" << std::endl;
  TEST_CHECK(*rb != *btc, "Structures of rb and btc should differ");
  TEST_CHECK(rb->size() == btc->size(), "Sizes should match");
  TEST_CHECK(*rb == *bst, "Structures of rb and bst should match");
  check = rb->left_ch()->type() != bst->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of rb and bst should differ");
  TEST_EQUAL(rb->height(), 5, "Red-black tree height should be 5");
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_complete(), "Red-black tree should be complete");
  TEST_CHECK(rb->is_search(), "Red-black tree should be search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << "Resulting red-black tree: " << std::endl << rb << std::endl;

  *rb = ulam.shuffle();
  *bst = *rb;
  std::cout << "Ulam numbers: red-black tree -> binary search tree:";
  std::cout << std::endl;
  TEST_CHECK(*bst == *rb, "Structures of bst and rb should match");
  TEST_CHECK(bst->height() == rb->height(), "Heights should also match");
  TEST_CHECK(bst->size() == rb->size(), "Sizes should also match");
  TEST_EQUAL(rb->height(), 4, "Tree heights should be 4");
  check = bst->left_ch()->type() != rb->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bst and rb should differ");
  TEST_CHECK(bst->is_balanced(), "Binary search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Binary search tree should be complete");
  TEST_CHECK(rb->is_complete(), "Red-black tree should also be complete");
  TEST_CHECK(bst->is_search(), "Binary search tree should be a search tree");
  std::cout << rb << std::endl;
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  recaman.quick_sort();
  bst->clear()->set_val(recaman[0]);
  for (size_t i = 1; i < recaman.size(); ++i)
    bst->insert(recaman[i]);
  *rb = *bst;
  std::cout << "Recaman's seq: binary search tree -> red-black tree:";
  std::cout << std::endl;
  TEST_CHECK(*bst != *rb, "Structures of bst and rb should not match");
  TEST_CHECK(bst->size() == rb->size(), "Sizes should also match");
  TEST_CHECK(bst->height() != rb->height(), "Heights should not match");
  TEST_CHECK(*(bst->balance()) == *rb, "Balanced structures should match");
  TEST_CHECK(bst->height() != rb->height(), "Balanced heights should differ");
  check = rb->left_ch()->type() != bst->left_ch()->type();
  TEST_CHECK(rb->type() != bst->type(), "Types of rb and bst should differ");
  TEST_EQUAL(rb->height(), 5, "Red-black tree height should be 5");
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_complete(), "Red-black tree should be complete");
  TEST_CHECK(rb->is_search(), "Red-black tree should be search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  std::cout << "Resulting red-black tree: " << std::endl << rb << std::endl;

  *rb = recaman.shuffle();
  *rbc = *rb;
  rbcl = rbc->clone();
  std::cout << "Recaman's seq: red-black tree -> red-black tree:" << std::endl;
  TEST_CHECK(*rb == *rbc, "Structures of rb and rbc should match");
  TEST_CHECK(*rb == *rbcl, "Structures of rb and rbcl should match");
  check = rb->size() == rbc->size();
  TEST_CHECK(check, "Sizes of rb and rbc should match");
  check = rb->size() == rbcl->size();
  TEST_CHECK(check, "Sizes of rb and rbcl should match");
  check = rb->height() == rbc->height();
  TEST_CHECK(check, "Heights of rb and rbc should also match");
  check = rb->height() == rbcl->height();
  TEST_CHECK(check, "Heights of rb and rbcl should also match");
  TEST_EQUAL(rbc->height(), 5, "Both tree heights should be 5");
  check = rb->left_ch()->type() == rbc->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of rb and rbc should match");
  check = rb->left_ch()->type() == rbcl->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of rb and rbcl should match");
  check = rbc->is_balanced() && rbcl->is_balanced();
  TEST_CHECK(check, "Both red-black trees should be balanced");
  check = rbc->is_complete() && rbcl->is_complete();
  TEST_CHECK(check, "Both red-black trees should be complete");
  check = rbc->is_search() && rbcl->is_search();
  TEST_CHECK(check, "Both red-black trees should be binary search trees");
  check = rbc->is_valid() && rbcl->is_valid();
  TEST_CHECK(check, "None of the red-black trees should have any violations");

  delete bt;
  delete btc;
  delete bst;
  delete avlp;
  delete rb;
  delete rbc;
  delete rbcl;
  delete rb_dyn;
  test_util::print_summary();
  return 0;
}