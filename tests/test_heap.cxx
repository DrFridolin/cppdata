#include <iostream>
#include <random>
#include <algorithm>
#include "binary_tree.h"
#include "heap.h"
#include "test_util.h"

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "MIN HEAP TREE WITH RANDOM PRIME NUMBERS" << std::endl;
  std::cout << std::endl;
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
  bool valid = true;

  dyn_array<int> ins;
  std::cout << "Inserted 10 random primes from [2, 229] to min heap tree: ";
  heap_tree<int, 1, false>* min_hpt = binary_tree<int, 1>::MIN_HEAP(primes[0]);
  const heap_tree<int, 1, false>* cmin_hpt;
  ins.add(primes[0]);
  std::cout << primes[0] << " ";
  for (int i = 1; i < 10; ++i) {
    ins.add(primes[i]);
    std::cout << primes[i] << ((i < 9) ? " " : "");
    min_hpt->insert(primes[i]);
    if (!min_hpt->is_heap(0))
      valid = false;
  }
  std::cout << std::endl;
  TEST_EQUAL(min_hpt->val(), ins.min(), "Root node should have min val");
  TEST_CHECK(valid, "Tree should always remain a min heap tree");
  cmin_hpt = min_hpt->left_ch()->right_ch()->parent();
  TEST_CHECK(cmin_hpt == min_hpt->left_ch(), "Parents should be linked");
  std::cout << std::endl << min_hpt << std::endl;

  std::cout << "Checking properties of min heap tree:" << std::endl;
  TEST_EQUAL(min_hpt->is_full(), 0, "Heap tree should not be full");
  TEST_CHECK(min_hpt->is_balanced(), "Heap tree should be balanced");
  TEST_CHECK(min_hpt->is_complete(), "Heap tree should be complete");
  TEST_EQUAL(min_hpt->is_perfect(), 0, "Heap tree should not be perfect");
  TEST_EQUAL(min_hpt->is_search(), 0, "Heap tree should not be a search tree");
  std::cout << std::endl;

  std::cout << "Removed: ";
  dyn_array<int> rem;
  valid = true;
  for (int i = 0; i < 9; ++i) {
    rem.add(min_hpt->remove());
    std::cout << rem.last() << " ";
    if (!min_hpt->is_heap(0))
      valid = false;
  }
  std::cout << min_hpt->val() << std::endl;
  rem.add(min_hpt->remove());
  std::cout << min_hpt << std::endl;
  std::cout << "Checking the tree size after removals:" << std::endl;
  TEST_EQUAL(min_hpt->size(), 1, "Heap tree should be root-only");
  TEST_CHECK(valid, "Tree should always remain a min heap tree");
  std::cout << std::endl;

  std::cout << "Checking if removed vals are sorted min to max:" << std::endl;
  bool sorted = true;
  for (size_t i = 1; i < rem.size(); ++i) {
    if (rem[i] < rem[i-1]) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "Vals removed from min heap tree should be sorted");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "MAX HEAP TREE WITH RANDOM PRIME NUMBERS" << std::endl;
  std::cout << std::endl;
  // Shuffle the array of primes
  std::shuffle(primes, primes + 50, eng);

  ins.clear();
  std::cout << "Inserted 10 random primes from [2, 229] to max heap: ";
  binary_tree<int, 0>* bt = binary_tree<int, 0>::create(primes[0], TreeType<int, 0>::MAX_HEAP);
  heap_tree<int, 0, true>* max_hpt;
  max_hpt = dynamic_cast<heap_tree<int, 0, true>*>(bt);
  valid = true;
  ins.add(primes[0]);
  std::cout << primes[0] << " ";
  for (int i = 1; i < 10; ++i) {
    ins.add(primes[i]);
    std::cout << primes[i] << " ";
    max_hpt->insert(primes[i]);
    if (!max_hpt->is_heap(1))
      valid = false;
  }
  std::cout << std::endl << max_hpt << std::endl;
  std::cout << "Checking the heap properties:" << std::endl;
  TEST_EQUAL(max_hpt->val(), ins.max(), "Root node should have max val");
  TEST_CHECK(valid, "Tree should always remain a max heap tree");
  std::cout << "Checking other properties of max heap tree:" << std::endl;
  TEST_EQUAL(max_hpt->is_full(), 0, "Heap tree should not be full");
  TEST_CHECK(max_hpt->is_balanced(), "Heap tree should be balanced");
  TEST_CHECK(max_hpt->is_complete(), "Heap tree should be complete");
  TEST_EQUAL(max_hpt->is_perfect(), 0, "Heap tree should not be perfect");
  TEST_EQUAL(max_hpt->is_search(), 0, "Heap tree should not be a search tree");
  std::cout << std::endl;

  rem.clear();
  std::cout << "Removed: ";
  for (int i = 0; i < 9; ++i) {
    rem.add(max_hpt->remove());
    std::cout << rem.last() << " ";
  }
  std::cout << max_hpt->val() << std::endl;
  rem.add(max_hpt->remove());
  std::cout << max_hpt << std::endl;
  std::cout << "Checking the tree size after removals:" << std::endl;
  TEST_EQUAL(max_hpt->size(), 1, "Heap tree should be root-only");
  std::cout << "Checking if removed vals are sorted max to min:" << std::endl;
  sorted = true;
  for (size_t i = 1; i < rem.size(); ++i) {
    if (rem[i] > rem[i-1]) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "Vals removed from max heap tree should be sorted");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "MIN HEAP WITH RANDOM PRIME NUMBERS" << std::endl;
  std::cout << std::endl;

  // Shuffle the array of primes
  std::shuffle(primes, primes + 50, eng);

  heap<int> min_hp(false);
  std::cout << "Created an empty min heap" << std::endl;
  TEST_CHECK(min_hp.is_empty(), "Min heap should initially be empty");
  std::cout << min_hp << std::endl;

  ins.clear();
  valid = true;
  std::cout << "Inserted 10 random primes from [2, 229] to min heap: ";
  for (int i = 0; i < 10; ++i) {
    ins.add(primes[i]);
    std::cout << primes[i] << ((i < 9) ? " " : "");
    min_hp.insert(primes[i]);
    if (!min_hpt->is_heap(0))
      valid = false;
  }
  std::cout << std::endl << "Min heap contents: " << min_hp << std::endl;
  TEST_EQUAL(min_hp.peek(), ins.min(), "Root node should have min val");
  TEST_CHECK(valid, "Tree should always remain a min heap tree");
  std::cout << std::endl;

  rem.clear();
  std::cout << "Removed: ";
  for (int i = 0; i < 10; ++i) {
    rem.add(min_hp.remove());
    std::cout << rem.last() << ((i < 9) ? " " : "");
  }
  std::cout << std::endl << "Min heap contents: " << min_hp << std::endl;
  TEST_CHECK(min_hp.is_empty(), "Min heap should be empty");
  TEST_EQUAL(min_hp.size(), 0, "Min heap should have a size of 0");
  std::cout << std::endl;

  std::cout << "Checking if removed vals are sorted min to max:" << std::endl;
  sorted = true;
  for (size_t i = 1; i < rem.size(); ++i) {
    if (rem[i] < rem[i-1]) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "Vals removed from min heap should be sorted");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "MAX HEAP WITH RANDOM PRIME NUMBERS" << std::endl;
  std::cout << std::endl;

  // Shuffle the array of primes
  std::shuffle(primes, primes + 50, eng);

  heap<int> max_hp(true);
  std::cout << "Created an empty max heap" << std::endl;
  TEST_CHECK(max_hp.is_empty(), "Max heap should initially be empty");
  std::cout << max_hp << std::endl;

  ins.clear();
  valid = true;
  std::cout << "Inserted 20 random primes from [2, 229] to max heap: ";
  for (int i = 0; i < 20; ++i) {
    ins.add(primes[i]);
    std::cout << primes[i] << ((i < 19) ? " " : "");
    max_hp.insert(primes[i]);
    if (!min_hpt->is_heap(0))
      valid = false;
  }
  std::cout << std::endl << "Max heap contents: " << max_hp << std::endl;
  TEST_EQUAL(max_hp.peek(), ins.max(), "Root node should have max val");
  TEST_CHECK(valid, "Tree should always remain a min heap tree");
  std::cout << std::endl;

  rem.clear();
  std::cout << "Removed: ";
  for (int i = 0; i < 20; ++i) {
    rem.add(max_hp.remove());
    std::cout << rem.last() << ((i < 19) ? " " : "");
  }
  std::cout << std::endl << "Max heap contents: " << max_hp << std::endl;
  TEST_CHECK(max_hp.is_empty(), "Max heap should be empty");
  TEST_EQUAL(max_hp.size(), 0, "Max heap should have a size of 0");
  std::cout << std::endl;

  std::cout << "Checking if removed vals are sorted max to min:" << std::endl;
  sorted = true;
  for (size_t i = 1; i < rem.size(); ++i) {
    if (rem[i] > rem[i-1]) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "Vals removed from min heap should be sorted");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HEAP TO/FROM HEAP TREE ASSIGNMENTS" << std::endl << std::endl;

  // Shuffle the array of primes
  std::shuffle(primes, primes + 50, eng);

  std::cout << "Built a min heap_tree with 20 random primes from [2, 229]: ";
  min_hpt->set_val(primes[0]);
  std::cout << primes[0] << " ";
  for (int i = 1; i < 20; ++i) {
    std::cout << primes[i] << ((i < 19) ? " " : "");
    min_hpt->insert(primes[i]);
  }
  std::cout << std::endl << min_hpt << std::endl;

  max_hp = min_hpt;
  std::cout << "Assigned min heap_tree to an empty max heap" << std::endl;
  std::cout << "Contents of heap: " << max_hp << std::endl;
  TEST_EQUAL(max_hp.is_max(), 0, "Max heap should now be a min heap");
  TEST_EQUAL(max_hp.size(), 20, "Heap should have 20 elements");
  std::cout << std::endl;

  dyn_array<int> rem1;
  std::cout << "Removed all elements from min heap_tree: ";
  for (int i = 0; i < 19; ++i) {
    rem1.add(min_hpt->remove());
    std::cout << rem1.last() << " ";
  }
  std::cout << min_hpt->val() << std::endl;
  rem1.add(min_hpt->remove());
  dyn_array<int> rem2;
  std::cout << "Removed all elements from min heap     : ";
  for (int i = 0; i < 20; ++i) {
    rem2.add(max_hp.remove());
    std::cout << rem2.last() << ((i < 19) ? " " : "");
  }
  std::cout << std::endl;
  TEST_SEQ(rem1, rem2, 20, "Removal seq of heap_tree and heap should match");
  std::cout << "Checking if removed vals are sorted min to max:" << std::endl;
  sorted = true;
  for (size_t i = 1; i < rem2.size(); ++i) {
    if (rem2[i] < rem2[i-1]) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "Vals removed from min heap should be sorted");
  std::cout << std::endl;

  //---------------------

  std::cout << "Built a max heap_tree with 20 random primes from [2, 229]: ";
  min_hpt->set_val(primes[20]);
  std::cout << primes[20] << " ";
  for (int i = 21; i < 40; ++i) {
    std::cout << primes[i] << ((i < 39) ? " " : "");
    max_hpt->insert(primes[i]);
  }
  std::cout << std::endl << max_hpt << std::endl;

  min_hp = max_hpt;
  std::cout << "Assigned max heap_tree to an empty min heap" << std::endl;
  std::cout << "Contents of heap: " << min_hp << std::endl;
  TEST_CHECK(min_hp.is_max(), "Min heap should now be a max heap");
  TEST_EQUAL(min_hp.size(), 20, "Heap should have 20 elements");
  std::cout << std::endl;

  rem1.clear();
  std::cout << "Removed all elements from max heap_tree: ";
  for (int i = 0; i < 19; ++i) {
    rem1.add(max_hpt->remove());
    std::cout << rem1.last() << " ";
  }
  std::cout << max_hpt->val() << std::endl;
  rem1.add(max_hpt->remove());
  rem2.clear();
  std::cout << "Removed all elements from max heap     : ";
  for (int i = 0; i < 20; ++i) {
    rem2.add(min_hp.remove());
    std::cout << rem2.last() << ((i < 19) ? " " : "");
  }
  std::cout << std::endl;
  TEST_SEQ(rem1, rem2, 20, "Removal seq of heap_tree and heap should match");
  std::cout << "Checking if removed vals are sorted max to min:" << std::endl;
  sorted = true;
  for (size_t i = 1; i < rem2.size(); ++i) {
    if (rem2[i] > rem2[i-1]) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "Vals removed from min heap should be sorted");
  std::cout << std::endl;

  //---------------------

  // Set the min and max heap types to be consistent with variable names again
  min_hp = heap<int>(false);
  max_hp = heap<int>(true);
  std::cout << "Built a min heap with 20 random primes from [2, 229]: ";
  for (int i = 30; i < 50; ++i) {
    std::cout << primes[i] << ((i < 49) ? " " : "");
    min_hp.insert(primes[i]);
  }
  std::cout << std::endl << "Min heap contents: " << min_hp << std::endl;

  *min_hpt = min_hp;
  std::cout << "Assigned min heap to a min heap_tree" << std::endl;
  std::cout << min_hpt << std::endl;

  std::cout << "Checking min heap_tree size:" << std::endl;
  TEST_EQUAL(min_hpt->size(), 20, "Heap tree should have 20 elements");
  std::cout << std::endl;

  rem1.clear();
  std::cout << "Removed all elements from min heap_tree: ";
  for (int i = 0; i < 19; ++i) {
    rem1.add(min_hpt->remove());
    std::cout << rem1.last() << " ";
  }
  std::cout << min_hpt->val() << std::endl;
  rem1.add(min_hpt->remove());
  rem2.clear();
  std::cout << "Removed all elements from min heap     : ";
  for (int i = 0; i < 20; ++i) {
    rem2.add(min_hp.remove());
    std::cout << rem2.last() << ((i < 19) ? " " : "");
  }
  std::cout << std::endl;
  TEST_SEQ(rem1, rem2, 20, "Removal seq of heap_tree and heap should match");
  std::cout << std::endl;

  std::cout << "Checking if removed vals are sorted min to max:" << std::endl;
  sorted = true;
  for (size_t i = 1; i < rem2.size(); ++i) {
    if (rem2[i] < rem2[i-1]) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "Vals removed from min heap should be sorted");
  std::cout << std::endl;

  //---------------------

  std::cout << "Built a max heap with 20 random primes from [2, 229]: ";
  for (int i = 10; i < 30; ++i) {
    std::cout << primes[i] << ((i < 29) ? " " : "");
    max_hp.insert(primes[i]);
  }
  std::cout << std::endl << "Max heap contents: " << max_hp << std::endl;

  *max_hpt = max_hp;
  std::cout << "Assigned max heap to a max heap_tree" << std::endl;
  std::cout << max_hpt << std::endl;

  std::cout << "Checking max heap_tree size:" << std::endl;
  TEST_EQUAL(max_hpt->size(), 20, "Heap tree should have 20 elements");
  std::cout << std::endl;

  rem1.clear();
  std::cout << "Removed all elements from max heap_tree: ";
  for (int i = 0; i < 19; ++i) {
    rem1.add(max_hpt->remove());
    std::cout << rem1.last() << " ";
  }
  std::cout << max_hpt->val() << std::endl;
  rem1.add(max_hpt->remove());
  rem2.clear();
  std::cout << "Removed all elements from max heap     : ";
  for (int i = 0; i < 20; ++i) {
    rem2.add(max_hp.remove());
    std::cout << rem2.last() << ((i < 19) ? " " : "");
  }
  std::cout << std::endl;
  TEST_SEQ(rem1, rem2, 20, "Removal seq of heap_tree and heap should match");
  std::cout << std::endl;
  std::cout << "Checking if removed vals are sorted max to min:" << std::endl;
  sorted = true;
  for (size_t i = 1; i < rem2.size(); ++i) {
    if (rem2[i] > rem2[i-1]) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "Vals removed from min heap should be sorted");

  delete min_hpt;
  delete max_hpt;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HEAPIFICATION FROM dyn_array<T>" << std::endl << std::endl;

  dyn_array<int> sum_of_divisors;
  sum_of_divisors = {  1,   3,   4,   7,   6,  12,   8,  15,  13,  18,  12,
                      28,  14,  24,  24,  31,  18,  39,  20,  42,  32,  36,
                      24,  60,  31,  42,  40,  56,  30,  72,  32,  63,  48,
                      54,  48,  91,  38,  60,  56,  90,  42,  96,  44,  84,
                      78,  72,  48, 124,  57,  93,  72,  98,  54, 120,  72,
                     120,  80,  90,  60, 168,  62,  96, 104, 127,  84, 144,
                      68, 126,  96, 144};
  dyn_array<int> min_gt;
  min_gt = {  1,   3,   4,   6,   7,   8,  12,  12,  13,  14,  15,  18,  18,
             20,  24,  24,  24,  28,  30,  31,  31,  32,  32,  36,  38,  39,
             40,  42,  42,  42,  44,  48,  48,  48,  54,  54,  56,  56,  57,
             60,  60,  60,  62,  63,  68,  72,  72,  72,  72,  78,  80,  84,
             84,  90,  90,  91,  93,  96,  96,  96,  98, 104, 120, 120, 124,
            126, 127, 144, 144, 168};
  dyn_array<int> max_gt;
  max_gt = {168, 144, 144, 127, 126, 124, 120, 120, 104,  98,  96,  96,  96,
             93,  91,  90,  90,  84,  84,  80,  78,  72,  72,  72,  72,  68,
             63,  62,  60,  60,  60,  57,  56,  56,  54,  54,  48,  48,  48,
             44,  42,  42,  42,  40,  39,  38,  36,  32,  32,  31,  31,  30,
             28,  24,  24,  24,  20,  18,  18,  15,  14,  13,  12,  12,   8,
              7,   6,   4,   3,   1};

  dyn_array<int> buf;
  std::cout << "Input: " << (buf = sum_of_divisors).shuffle() << std::endl;
  min_hp = buf;
  std::cout << "Min-heapified shuffled input array: " << min_hp << std::endl;
  TEST_EQUAL(min_hp.size(), 70, "Min heap should have 70 elements");
  std::cout << std::endl;

  rem1.clear();
  std::cout << "Removed all elements from min heap: ";
  for (int i = 0; i < buf.size(); ++i) {
    rem1.add(min_hp.remove());
    std::cout << rem1.last() << ((i < buf.size()-1) ? ", " : "");
  }
  std::cout << std::endl;
  TEST_CHECK(min_hp.is_empty(), "Min heap should now be empty");
  TEST_SEQ(rem1, min_gt, 70, "Removals should be sorted in increasing order");
  std::cout << std::endl;

  std::cout << "Input: " << (buf = sum_of_divisors).shuffle() << std::endl;
  max_hp = buf;
  std::cout << "Max-heapified shuffled input array: " << max_hp << std::endl;
  TEST_EQUAL(max_hp.size(), 70, "Max heap should have 70 elements");
  std::cout << std::endl;

  rem2.clear();
  std::cout << "Removed all elements from max heap: ";
  for (int i = 0; i < buf.size(); ++i) {
    rem2.add(max_hp.remove());
    std::cout << rem2.last() << ((i < buf.size()-1) ? ", " : "");
  }
  std::cout << std::endl;
  TEST_CHECK(max_hp.is_empty(), "Max heap should now be empty");
  TEST_SEQ(rem2, max_gt, 70, "Removals should be sorted in decreasing order");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HEAP TREE ASSIGNMENT TESTS" << std::endl << std::endl;

  bool check;
  heap_tree<unsigned short, 1, false> *htmn, *htmnc, *htmnd;
  heap_tree<unsigned short, 1, true> *htmx, *htmxc, *htmxi, *htmxd;
  binary_tree<unsigned short, 1>* btu;
  binary_search_tree<unsigned short, 1>* bst;
  avl_tree<unsigned short, 1>* avl;
  red_black_tree<unsigned short>* rb;
  bt_ptrs<unsigned short, 1> path;
  dyn_array<unsigned short> perrin, gould, trav;
  perrin = {    3,     0,     2,     3,     2,     5,     5,     7,    10,
               12,    17,    22,    29,    39,    51,    68,    90,   119,
              158,   209,   277,   367,   486,   644,   853,  1130,  1497,
             1983,  2627,  3480,  4610,  6107,  8090, 10717, 14197, 18807,
            24914, 33004, 43721, 57918};
  gould  = { 1,  2,  2,  4,  2,  4,  4,  8,  2,  4,  4,  8,  4,  8,  8, 16,
             2,  4,  4,  8,  4,  8,  8, 16,  4,  8,  8, 16,  8, 16, 16, 32,
             2,  4,  4,  8,  4,  8,  8, 16,  4,  8,  8, 16,  8, 16, 16, 32,
             4,  8,  8, 16,  8, 16, 16, 32,  8, 16, 16, 32, 16, 32, 32, 64,
             2,  4,  4,  8,  4,  8,  8, 16,  4,  8,  8, 16,  8, 16, 16, 32,
             4,  8,  8, 16,  8, 16, 16, 32};
  dyn_array<unsigned short> p_unq = perrin.unique().merge_sort();
  dyn_array<unsigned short> g_unq  = gould.unique().merge_sort();

  htmn  = binary_tree<unsigned short, 1>::MIN_HEAP(1);
  htmnc = binary_tree<unsigned short, 1>::MIN_HEAP(1);
  htmnd = binary_tree<unsigned short, 1>::MIN_HEAP(1);

  htmx  = binary_tree<unsigned short, 1>::MAX_HEAP(3);
  htmxc = binary_tree<unsigned short, 1>::MAX_HEAP(3);
  htmxi = binary_tree<unsigned short, 1>::MAX_HEAP(3);
  htmxd = binary_tree<unsigned short, 1>::MAX_HEAP(3);

  btu   = binary_tree<unsigned short, 1>::STANDARD(1);
  bst   = binary_tree<unsigned short, 1>::SEARCH(1);
  avl   = binary_tree<unsigned short, 1>::AVL(1);
  rb    = binary_tree<unsigned short, 1>::RED_BLACK(1);

  *htmn  = { 1,  2,  2,  4,  2,  4,  4,  8,  2,  4,  4,  8,  4,  8,  8, 16,
             2,  4,  4,  8,  4,  8,  8, 16,  4,  8,  8, 16,  8, 16, 16, 32,
             2,  4,  4,  8,  4,  8,  8, 16,  4,  8,  8, 16,  8, 16, 16, 32,
             4,  8,  8, 16,  8, 16, 16, 32,  8, 16, 16, 32, 16, 32, 32, 64,
             2,  4,  4,  8,  4,  8,  8, 16,  4,  8,  8, 16,  8, 16, 16, 32,
             4,  8,  8, 16,  8, 16, 16, 32};
  *htmnd = gould;
  std::cout << "Gould's sequence: init list -> min heap tree:" << std::endl;
  std::cout << "Gould's sequence: dyn_array -> min heap tree:" << std::endl;
  TEST_CHECK(*htmn == *htmnd, "Init list and dyn_array trees should match");
  check = htmn->is_heap(0) && htmnd->is_heap(0);
  TEST_CHECK(check, "Both min heap trees should be valid min heaps");
  check = htmn->is_heap(1) || htmnd->is_heap(1);
  TEST_CHECK(!check, "There should be no min/max heap_tree mixup");
  check = htmn->is_balanced() && htmnd->is_balanced();
  TEST_CHECK(check, "Both min heap trees should be balanced");
  check = htmn->is_complete() && htmnd->is_complete();
  TEST_CHECK(check, "Both min heap trees should be complete");
  TEST_EQUAL(htmn->height(), 0, "Heap insertions should not compute heights");

  *htmx  = {    3,     0,     2,     3,     2,     5,     5,     7,    10,
               12,    17,    22,    29,    39,    51,    68,    90,   119,
              158,   209,   277,   367,   486,   644,   853,  1130,  1497,
             1983,  2627,  3480,  4610,  6107,  8090, 10717, 14197, 18807,
            24914, 33004, 43721, 57918};
  *htmxd = perrin;
  std::cout << "Perrin numbers: init list -> max heap tree:" << std::endl;
  std::cout << "Perrin numbers: dyn_array -> max heap tree:" << std::endl;
  TEST_CHECK(*htmx == *htmxd, "Init list and dyn_array trees should match");
  check = htmx->is_heap(1) && htmxd->is_heap(1);
  TEST_CHECK(check, "Both max heap trees should be valid max heaps");
  check = htmx->is_heap(0) || htmxd->is_heap(0);
  TEST_CHECK(!check, "There should be no min/max heap_tree mixup");
  check = htmx->is_balanced() && htmxd->is_balanced();
  TEST_CHECK(check, "Both max heap trees should be balanced");
  check = htmx->is_complete() && htmxd->is_complete();
  TEST_CHECK(check, "Both min heap trees should be complete");
  TEST_EQUAL(htmx->height(), 0, "Heap insertions should not compute heights");

  path = htmx->traverse(LEVEL_ORDER);
  btu->set_val(path[0]->val());
  for (size_t i = 1; i < path.size(); ++i)
    btu->insert(path[i]->val());
  std::cout << "Created a binary_tree with identical structure:" << std::endl;
  TEST_CHECK(*btu == *htmx, "Structures of btu and htmx should match");
  check = btu->left()->type() != htmx->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of btu and htmx should be different");
  TEST_CHECK(btu->is_heap(1), "Binary tree should be a valid max heap");
  TEST_CHECK(btu->is_balanced(), "Binary tree should be balanced");
  TEST_CHECK(btu->is_complete(), "Binary tree should be complete");
  TEST_EQUAL(btu->max(), btu->val(), "Binary tree root should hold max");

  perrin.shuffle();
  btu->clear()->set_val(perrin[0]);
  binary_tree<unsigned short, 1>* btu_ptr = btu;
  for (size_t i = 1; i < perrin.size(); ++i) {
    btu_ptr->left() = binary_tree<unsigned short, 1>::STANDARD(perrin[i]);
    btu_ptr = btu_ptr->left();
  }
  std::cout << "Perrin numbers: Left-skewed and degenerate binary tree:";
  std::cout << std::endl;
  TEST_EQUAL(btu->size(), 40, "Size of binary tree should be 40");
  TEST_EQUAL(btu->right(), 0, "Root should have no right child");
  TEST_CHECK(*btu != *htmn, "Structures of btu and htmn should differ");
  TEST_CHECK(*btu != *htmx, "Structures of btu and htmx should differ");
  TEST_CHECK(!btu->is_heap(0), "Binary tree should not be a min heap tree");
  TEST_CHECK(!btu->is_heap(1), "Binary tree should not be a max heap tree");

  *htmn = *btu;
  std::cout << "Perrin numbers: binary tree -> min heap tree:" << std::endl;
  TEST_EQUAL(btu->size(), htmn->size(), "Sizes of btu and htmn should match");
  TEST_CHECK(*btu != *htmn, "Structures of btu and htmn should still differ");
  check = btu->left()->type() != htmn->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of btu and htmn should differ");
  TEST_CHECK(htmn->is_heap(0), "Min heap tree should be a valid min heap");
  TEST_CHECK(htmn->is_balanced(), "Min heap tree should be balanced");
  TEST_CHECK(htmn->is_complete(), "Min heap tree should be complete");
  TEST_EQUAL(htmn->min(), htmn->val(), "Min heap tree root should hold min");
  TEST_EQUAL(htmn->height(), 0, "Assignment should not compute heights");
  rem.clear();
  for (size_t i = 0; i < perrin.size(); ++i)
    rem.add(htmn->remove());
  perrin.merge_sort(true);
  TEST_SEQ(rem, perrin, 40, "Removals should be sorted in increasing order");

  *htmx = gould.shuffle();
  *btu = *htmx;
  std::cout << "Gould's seq: max heap tree -> binary tree:" << std::endl;
  TEST_CHECK(*btu == *htmx, "Structures of ntu and htmx should now match");
  check = btu->left()->type() != htmx->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of btu and htmx should still differ");
  TEST_CHECK(btu->is_heap(1), "Binary tree should be a valid max heap");
  TEST_CHECK(btu->is_balanced(), "Binary tree should be balanced");
  TEST_CHECK(btu->is_complete(), "Binary tree should be complete");
  TEST_CHECK(!btu->is_heap(0), "Binary tree should not be a min heap");
  TEST_EQUAL(btu->max(), btu->val(), "Binary tree root should hold max");
  TEST_EQUAL(htmx->height(), 0, "Assignment should not compute heights");

  *htmn = gould.shuffle();
  *htmnc = *htmn;
  std::cout << "Gould's seq: min heap tree -> min heap tree:" << std::endl;
  TEST_CHECK(*htmn == *htmnc, "Structures of htmn and htmnc should match");
  check = htmn->left_ch()->type() == htmnc->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of htmn and htmnc should also match");
  check = htmn->height() != htmnc->height();
  TEST_CHECK(check, "Heights of htmn and htmnc should differ");
  TEST_EQUAL(htmnc->height(), 6, "Should compute height 6 during assignment");
  TEST_CHECK(htmnc->is_heap(0), "Min heap tree should be a valid min heap");
  TEST_CHECK(htmnc->is_balanced(), "Min heap tree should be balanced");
  TEST_CHECK(htmnc->is_complete(), "Min heap tree should be complete");
  TEST_EQUAL(htmnc->min(), htmnc->val(), "Root val of htmnc should be min");

  *htmx = perrin.shuffle();
  htmxc = htmx->clone();
  std::cout << "Perrin numbers: max heap tree -> max heap tree:" << std::endl;
  TEST_CHECK(*htmx == *htmxc, "Structures of htmx and htmxc should match");
  check = htmx->left_ch()->type() == htmxc->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of htmx and htmxc should also match");
  check = htmx->height() != htmxc->height();
  TEST_CHECK(check, "Heights of htmx and htmxc should differ");
  TEST_EQUAL(htmxc->height(), 5, "Height of htmxc should be 5");
  TEST_CHECK(htmxc->is_heap(1), "Max heap tree should be a valid max heap");
  TEST_CHECK(htmxc->is_balanced(), "Max heap tree should be balanced");
  TEST_CHECK(htmxc->is_complete(), "Max heap tree should be complete");
  TEST_EQUAL(htmxc->max(), htmxc->val(), "Root val of htmxc should be max");

  *bst = gould.shuffle();
  *htmx = *bst;
  std::cout << "Gould's seq: binary search tree -> max heap tree:";
  std::cout << std::endl;
  TEST_EQUAL(bst->size(), htmx->size(), "Sizes of bst and htmx should match");
  TEST_CHECK(*bst != *htmx, "Structures of bst and htmx should differ");
  check = bst->num_nodes() == g_unq.size();
  TEST_CHECK(check, "Binary search tree should have 1 node per unique value");
  check = bst->left_ch()->type() != htmx->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bst and htmx should differ");
  TEST_CHECK(htmx->is_balanced(), "Max heap tree should be balanced");
  TEST_CHECK(htmx->is_complete(), "Max heap tree should be complete");
  TEST_CHECK(htmx->is_heap(1), "Max heap tree should be a valid max heap");
  TEST_EQUAL(htmx->max(), htmx->val(), "Root val of htmx should be max");
  TEST_EQUAL(htmx->height(), 0, "Assignment should not compute heights");
  rem.clear();
  for (size_t i = 0; i < gould.size(); ++i)
    rem.add(htmx->remove());
  gould.merge_sort(false);
  TEST_SEQ(rem, gould, 88, "Removals should be sorted in decreasing order");

  *htmnc = perrin.shuffle();
  *bst = *htmnc;
  std::cout << "Perrin numbers: min heap tree -> binary search tree:";
  std::cout << std::endl;
  check = htmnc->size() == bst->size();
  TEST_CHECK(check, "Sizes of heap tree and binary search tree should match");
  TEST_CHECK(*bst != *htmnc, "Structures of bst and htmnc should differ");
  check = bst->left_ch()->type() != htmnc->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bst and htmnc should also differ");
  check = (htmnc->size() - bst->num_nodes()) == (perrin.size() - p_unq.size());
  TEST_CHECK(check, "Duplicates should give binary search tree 3 less nodes");
  TEST_CHECK(bst->is_balanced(), "Binary search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Binary search tree should be complete");
  TEST_CHECK(bst->is_search(), "Binary search tree should be search tree");
  TEST_CHECK(!bst->is_heap(), "Binary search tree shouldn't be min heap tree");
  path = bst->traverse(IN_ORDER);
  trav.clear();
  for (size_t i = 0; i < path.size(); ++i)
    trav.add(path[i]->val());
  TEST_SEQ(trav, p_unq, 37, "In-order traversal values should be sorted");
  TEST_EQUAL(htmn->height(), 0, "Min heap tree should still have no heights");
  TEST_EQUAL(bst->height(), 0, "Assignment should not compute heights");

  *avl = *htmn;
  std::cout << "Gould's seq: min heap tree -> avl tree:" << std::endl;
  TEST_EQUAL(avl->size(), htmn->size(), "Sizes of avl and htmn should match");
  TEST_CHECK(avl->height() != htmn->height(), "Heights should not match");
  TEST_CHECK(*avl != *htmn, "Structures of avl and htmn should differ");
  check = avl->left_ch()->type() != htmn->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of avl and htmn should differ");
  check = (htmn->size() - avl->num_nodes()) == (gould.size() - g_unq.size());
  TEST_CHECK(check, "Duplicates should give AVL tree 81 less nodes");
  TEST_EQUAL(avl->num_nodes(), g_unq.size(), "1 node per unique val expected");
  TEST_CHECK(avl->is_balanced(), "AVL tree should be balanced");
  TEST_CHECK(avl->is_complete(), "AVL tree should be complete");
  TEST_CHECK(avl->is_search(), "AVL tree should be search tree");
  TEST_CHECK(!avl->is_heap(), "AVL tree shouldn't be a valid min heap");
  TEST_EQUAL(avl->height(), 2, "AVL tree height should be 2");
  check = (avl->balance_factor() >= -1 && avl->balance_factor() <= 1);
  TEST_CHECK(check, "AVL tree balance factor should be in range [-1, 1]");
  path = avl->traverse(IN_ORDER);
  trav.clear();
  for (size_t i = 0; i < path.size(); ++i)
    trav.add(path[i]->val());
  TEST_SEQ(trav, g_unq, 7, "In-order traversal values should be sorted");

  *htmx = *avl;
  std::cout << "Gould's seq: avl tree -> max heap tree:" << std::endl;
  TEST_EQUAL(avl->size(), htmx->size(), "Sizes of avl and htmx should match");
  TEST_CHECK(*avl != *htmx, "Structures of avl and ht should differ");
  check = (htmx->size() - avl->num_nodes()) == (gould.size() - g_unq.size());
  TEST_CHECK(check, "Duplicates should give max heap tree 81 more nodes");
  check = avl->left_ch()->type() != htmx->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of avl and htmx should differ");
  TEST_CHECK(htmx->is_balanced(), "Max heap tree should be balanced");
  TEST_CHECK(htmx->is_complete(), "Max heap tree should be complete");
  TEST_CHECK(htmx->is_heap(1), "Max heap tree should be a valid max heap");
  TEST_EQUAL(htmx->max(), htmx->val(), "Root val of htmx should be max");
  TEST_EQUAL(htmx->height(), 0, "Assignment should not compute heights");
  rem.clear();
  for (size_t i = 0; i < gould.size(); ++i)
    rem.add(htmx->remove());
  gould.merge_sort(false);
  TEST_SEQ(rem, gould, 88, "Removals should be sorted in decreasing order");

  *rb = perrin.shuffle();
  *htmn = *rb;
  std::cout << "Perrin numbers: red-black tree -> min heap tree:" << std::endl;
  TEST_EQUAL(rb->size(), htmn->size(), "Sizes of rb and htmn should match");
  TEST_CHECK(*rb != *htmn, "Structures of rb and htmn should differ");
  check = rb->left_ch()->type() != htmn->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of rb and htmn should differ");
  TEST_CHECK(htmn->is_balanced(), "Min heap tree should be balanced");
  TEST_CHECK(htmn->is_complete(), "Min heap tree should be complete");
  TEST_CHECK(htmn->is_heap(0), "Min heap tree should be a valid min heap");
  TEST_EQUAL(htmn->min(), htmn->val(), "Root val of htmn should be min");
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_complete(), "Red-black tree should be complete");
  TEST_CHECK(rb->is_search(), "Red-black tree should be search tree");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  TEST_EQUAL(rb->height(), 5, "Red-black tree height should be 5");
  for (size_t i = 0; i < perrin.size(); ++i)
    htmn = htmn->remove(perrin[i]);
  TEST_EQUAL(htmn, nullptr, "Targeted removal should clear min heap tree");

  *htmx = gould.shuffle();
  *rb = *htmx;
  std::cout << "Gould's seq: max heap tree -> red-black tree:" << std::endl;
  TEST_EQUAL(rb->size(), htmx->size(), "Sizes of rb and htmx should match");
  TEST_CHECK(*rb != *htmx, "Structures of rb and htmx should differ");
  check = rb->left_ch()->type() != htmx->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of rb and htmx should differ");
  check = (htmx->size() - rb->num_nodes()) == (gould.size() - g_unq.size());
  TEST_CHECK(check, "Duplicates should give red-black tree 81 less nodes");
  TEST_EQUAL(rb->num_nodes(), g_unq.size(), "1 node per unique val expected");
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be balanced");
  TEST_CHECK(rb->is_balanced(), "Red-black tree should be complete");
  TEST_CHECK(rb->is_search(), "Red-black tree should be search tree");
  TEST_CHECK(!rb->is_heap(1), "Red-black tree shouldn't be a valid max heap");
  TEST_CHECK(rb->is_valid(), "Red-black tree should not have any violations");
  TEST_EQUAL(rb->height(), 2, "Red-black tree height should be 2");
  path = rb->traverse(IN_ORDER);
  trav.clear();
  for (size_t i = 0; i < path.size(); ++i)
    trav.add(path[i]->val());
  TEST_SEQ(trav, g_unq, 7, "In-order traversal values should be sorted");

  delete htmn;
  delete htmnd;
  delete htmx;
  delete htmxi;
  delete htmxd;
  delete btu;
  delete bst;
  delete avl;
  delete rb;
  test_util::print_summary();
  return 0;
}