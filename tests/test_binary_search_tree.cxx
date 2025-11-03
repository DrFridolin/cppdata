#include <iostream>
#include <random>
#include <algorithm>
#include "binary_tree.h"
#include "test_util.h"

template <class B>
void test_trav(B* bt, int* pre, int* in, int* post, int* lvl, int* df, int* bf)
{
  std::cout << "Pre-order traversal:" << std::endl;
  auto trpath = bt->traverse(PRE_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(trpath, pre, trpath.size(), "Pre-order path should match gt");

  std::cout << "In-order traversal:" << std::endl;
  trpath = bt->traverse(IN_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(trpath, in, trpath.size(), "In-order path should match gt");

  std::cout << "Post-order traversal:" << std::endl;
  trpath = bt->traverse(POST_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(trpath, post, trpath.size(), "Post-order path should match gt");

  std::cout << "Level-order traversal:" << std::endl;
  trpath = bt->traverse(LEVEL_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(trpath, lvl, trpath.size(), "Level-order path should match gt");

  std::cout << "Depth-first traversal: " << std::endl;
  gn_ptrs<int> grpath = bt->depth_first_traverse(&std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(grpath, df, grpath.size(), "Depth-1st path should match gt");

  std::cout << "Breadth-first traversal:" << std::endl;
  grpath = bt->breadth_first_traverse(&std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(grpath, bf, grpath.size(), "Breadth-1st path should match gt");

  return;
}

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "BINARY SEARCH TREE WITH SORTED PRIME NUMBERS" << std::endl;
  std::cout << std::endl;
  int primes[50] = {  2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                     41,  43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,
                     97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
                    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
                    227, 229};

  binary_search_tree<int, 1>* bst = binary_tree<int, 1>::SEARCH(primes[10]);
  const binary_search_tree<int, 1>* buf;
  std::cout << "Initialized bst with 11th prime: " << primes[10] << std::endl;
  std::cout << "Checking root node value:" << std::endl;
  TEST_EQUAL(bst->val(), 31, "Root node value should be 31");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  bst->insert(primes[11]);
  std::cout << "Inserted the 12th prime: " << primes[11] << std::endl;
  std::cout << "Verifying insertion location:" << std::endl;
  buf = bst->right_ch();
  TEST_EQUAL(buf->val(), 37, "Val should be at 31.right");
  TEST_CHECK(buf->parent() == bst, "Root should be linked as parent");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  bst->insert(primes[12]);
  std::cout << "Inserted the 13th prime: " << primes[12] << std::endl;
  std::cout << "Verifying insertion location:" << std::endl;
  TEST_EQUAL(bst->right_ch()->right_ch()->val(), 41, "Should be at 31.right");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  std::cout << "Inserted seven more primes: ";
  for (int i = 13; i < 20; ++i) {
    std::cout << primes[i] << " ";
    bst->insert(primes[i]);
  }
  std::cout << std::endl;
  std::cout << "Checking properties of the search tree:" << std::endl;
  TEST_EQUAL(bst->is_full(), 0, "Search tree should not be full");
  TEST_EQUAL(bst->is_balanced(), 0, "Search tree should not be balanced");
  TEST_EQUAL(bst->is_complete(), 0, "Search tree should not be complete");
  TEST_EQUAL(bst->is_perfect(), 0, "Search tree should not be perfect");
  TEST_CHECK(bst->is_search(), "Search tree should be a search tree");
  int pre1[] = {31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
  int in1[]  = {31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
  int pos1[] = {71, 67, 61, 59, 53, 47, 43, 41, 37, 31};
  int lvl1[] = {31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
  int df1[]  = {31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
  int bf1[]  = {31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
  test_trav(bst, pre1, in1, pos1, lvl1, df1, bf1);
  bst->compute_heights();
  TEST_EQUAL(bst->height(), 9, "Tree height should be 9");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  std::cout << "Balanced the search tree:" << std::endl << bst->balance();
  std::cout << "Checking properties after balancing:" << std::endl;
  TEST_EQUAL(bst->is_full(), 0, "Search tree should not be full");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  TEST_EQUAL(bst->is_perfect(), 0, "Search tree should not be perfect");
  TEST_CHECK(bst->is_search(), "Search tree should be a search tree");
  TEST_EQUAL(bst->is_heap(0), 0, "Tree should not be a min heap tree");
  TEST_EQUAL(bst->is_heap(1), 0, "Tree should not be a max heap tree");
  buf = bst->left_ch()->right_ch();
  TEST_CHECK(buf->parent() == bst->left_ch(), "Parent should be linked");
  TEST_EQUAL(bst->parent(), 0, "Root should have no parent");
  int pre2[] = {59, 43, 37, 31, 41, 53, 47, 67, 61, 71};
  int in2[]  = {31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
  int pos2[] = {31, 41, 37, 47, 53, 43, 61, 71, 67, 59};
  int lvl2[] = {59, 43, 67, 37, 53, 61, 71, 31, 41, 47};
  int df2[]  = {59, 43, 37, 31, 41, 53, 47, 67, 61, 71};
  int bf2[]  = {59, 43, 67, 37, 53, 61, 71, 31, 41, 47};
  test_trav(bst, pre2, in2, pos2, lvl2, df2, bf2);
  bst->compute_heights();
  TEST_EQUAL(bst->height(), 3, "Balanced tree height should be 3");
  std::cout << std::endl;

  std::cout << "Inserted 10 primes from the end: ";
  for (int i = 49; i > 39; --i) {
    std::cout << primes[i] << " ";
    bst->insert(primes[i]);
  }
  std::cout << std::endl;
  std::cout << "Inserted 10 primes from the beginning: ";
  for (int i = 0; i < 10; ++i) {
    std::cout << primes[i] << " ";
    bst->insert(primes[i]);
  }
  std::cout << std::endl;
  std::cout << "Checking properties of the search tree:" << std::endl;
  TEST_EQUAL(bst->is_full(), 0, "Search tree should not be full");
  TEST_EQUAL(bst->is_balanced(), 0, "Search tree should not be balanced");
  TEST_EQUAL(bst->is_complete(), 0, "Search tree should not be complete");
  TEST_EQUAL(bst->is_perfect(), 0, "Search tree should not be perfect");
  TEST_CHECK(bst->is_search(), "Search tree should be a search tree");
  buf = bst->search(41);
  TEST_EQUAL(buf->parent()->val(), 37, "41's parent should be 37");
  std::cout << "Checking if in-order traversal vals are sorted:" << std::endl;
  bt_ptrs<int, 1> path = bst->traverse(IN_ORDER);
  bool sorted = true;
  for (size_t i = 1; i < path.size(); ++i) {
    if (path[i]->val() < path[i-1]->val()) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "In-order traversal path should have sorted values");
  bst->compute_heights();
  TEST_EQUAL(bst->height(), 13, "Tree height should be 13");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  std::cout << "Balanced the search tree:" << std::endl << bst->balance();
  std::cout << "Checking properties after balancing:" << std::endl;
  TEST_EQUAL(bst->is_full(), 0, "Search tree should not be full");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  TEST_EQUAL(bst->is_perfect(), 0, "Search tree should not be perfect");
  TEST_CHECK(bst->is_search(), "Search tree should be a search tree");
  std::cout << "Checking if in-order traversal vals are sorted:" << std::endl;
  path = bst->traverse(IN_ORDER);
  sorted = true;
  for (size_t i = 1; i < path.size(); ++i) {
    if (path[i]->val() < path[i-1]->val()) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "In-order traversal path should have sorted values");
  bst->compute_heights();
  TEST_EQUAL(bst->height(), 4, "Balanced tree height should be 4");
  std::cout << std::endl;

  int nval;
  bool search, balanced, check;
  size_t old_nc, exp_nc, old_sz, exp_sz, cnt1, cnt2, cnt3;
  dyn_array<int> dups = {41, 41, 47, 41, 43, 47, 53, 71};
  old_nc = bst->num_nodes();
  old_sz = bst->size();
  exp_sz = old_sz + dups.size();
  for (size_t i = 0; i < dups.size(); ++i)
    bst->insert(dups[i]);
  std::cout << "Inserted duplicate values to binary search tree:" << std::endl;
  TEST_EQUAL(bst->num_nodes(), old_nc, "Number of nodes should not change");
  TEST_EQUAL(bst->size(), exp_sz, "Size should grow by num values inserted");
  TEST_EQUAL(bst->count(), 2, "Root's duplicate value count should be 2");
  cnt1 = bst->search(41)->count();
  TEST_EQUAL(cnt1, 4, "Node 41's duplicate value count should be 4");
  cnt2 = bst->search(47)->count();
  TEST_EQUAL(cnt2, 3, "Node 47's duplicate value count should be 3");
  cnt3 = bst->search(53)->count();
  TEST_EQUAL(cnt3, 2, "Node 53's duplicate value count should be 2");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  nval = bst->find_parent(41)->val();
  TEST_EQUAL(nval, 43, "41's parent should be 43");
  nval = bst->find_parent(bst->right_ch()->left_ch()->left_ch())->val();
  TEST_EQUAL(nval, 71, "61's parent should be 71");
  nval = bst->find_closest(42);
  check = (nval == 41) || (nval == 43);
  TEST_CHECK(check, "Closest prime to 42 in tree should either be 41 or 43");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  dyn_array<int> uniq = {7, 23, 193, 199, 179, 37, 31};
  old_nc = bst->num_nodes();
  old_sz = bst->size();
  exp_nc = old_nc - uniq.size();
  exp_sz = old_sz - uniq.size();
  search = true;
  for (size_t i = 0; i < uniq.size(); ++i) {
    bst = bst->remove(uniq[i]);
    if (!bst->is_search())
      search = false;
  }
  std::cout << "Removed some values without duplicates" << std::endl;
  TEST_EQUAL(bst->num_nodes(), exp_nc, "Each unique val should remove a node");
  TEST_EQUAL(bst->size(), exp_sz, "Each unique val should reduce size by 1");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(!bst->is_complete(), "Search tree should not be complete");
  TEST_CHECK(search, "Search tree should have always remained a search tree");
  nval = bst->left_ch()->right_ch()->val();
  TEST_EQUAL(nval, 41, "Node 41 should move up to root->left()->right()");
  TEST_EQUAL(bst->search(41)->count(), cnt1, "41's count shouldn't change");
  TEST_EQUAL(bst->search(47)->count(), cnt2, "47's count shouldn't change");
  TEST_EQUAL(bst->search(53)->count(), cnt3, "53's count shouldn't change");
  buf = bst->left_ch()->left_ch();
  TEST_CHECK(bst->search(3)->parent() == buf, "3's new parent should be 19");
  TEST_EQUAL(bst->search(7), nullptr, "Tree shouldn't contain value 7");
  TEST_EQUAL(bst->search(199), nullptr, "Tree shouldn't contain value 199");
  TEST_EQUAL(bst->min(), 2, "Minimum value in the tree should be 2");
  TEST_EQUAL(bst->max(), 229, "Maximum value in the tree should be 229");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  old_nc = bst->num_nodes();
  old_sz = bst->size();
  std::cout << "Balanced the search tree:" << std::endl << bst->balance();
  std::cout << "Checking properties after balancing:" << std::endl;
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should now be complete");
  TEST_CHECK(bst->is_search(), "Search tree should be a valid search tree");
  TEST_EQUAL(bst->num_nodes(), old_nc, "Number of nodes should not change");
  TEST_EQUAL(bst->size(), old_sz, "Size should not change");
  TEST_EQUAL(bst->search(41)->count(), cnt1, "41's count shouldn't change");
  TEST_EQUAL(bst->search(47)->count(), cnt2, "47's count shouldn't change");
  TEST_EQUAL(bst->search(53)->count(), cnt3, "53's count shouldn't change");
  TEST_EQUAL(bst->val(), 71, "71 should be the new root");
  buf = bst->search(71)->parent();
  TEST_EQUAL(buf, 0, "As new root, 71 should have no parent");
  TEST_EQUAL(bst->count(), 2, "71's count shouldn't change");
  std::cout << std::endl;

  dyn_array<int> dups_unique = dups.unique();
  old_nc = bst->num_nodes();
  old_sz = bst->size();
  exp_sz = old_sz - dups_unique.size();
  search = true;
  for (size_t i = 0; i < dups_unique.size(); ++i) {
    bst = bst->remove(dups_unique[i]);
    if (!bst->is_search())
      search = false;
  }
  std::cout << "Removed one of each duplicate value" << std::endl;
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  TEST_CHECK(search, "Search tree should have always remained a search tree");
  TEST_EQUAL(bst->num_nodes(), old_nc, "Number of nodes should not change");
  TEST_EQUAL(bst->size(), exp_sz, "Size should shrink by num values removed");
  TEST_EQUAL(bst->search(41)->count(), cnt1-1, "41's count should decrement");
  TEST_EQUAL(bst->search(47)->count(), cnt2-1, "47's count should decrement");
  TEST_EQUAL(bst->search(53)->count(), cnt3-1, "53's count should decrement");
  buf = bst->left_ch()->right_ch()->left_ch()->parent();
  TEST_EQUAL(buf->val(), 53, "43's parent should be 53");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  old_nc = bst->num_nodes();
  old_sz = bst->size();
  bst->remove(71)->remove(53)->insert(211)->insert(211)->insert(211);
  std::cout << "Removed 71 and 53, inserted 3 duplicates of 211:" << std::endl;
  TEST_CHECK(bst->is_search(), "Search tree should be a valid search tree");
  TEST_EQUAL(bst->val(), 181, "181 should be the new root");
  TEST_EQUAL(bst->search(53), 0, "53 should no longer exist");
  TEST_EQUAL(bst->search(71), 0, "71 should no longer exist");
  TEST_EQUAL(bst->size(), old_sz+1, "Size should grow by 1");
  TEST_EQUAL(bst->num_nodes(), old_nc-2, "Node count should be reduced by 2");
  buf = bst->left_ch()->right_ch()->left_ch()->parent();
  TEST_EQUAL(buf->val(), 59, "43's parent should now be 59");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  old_nc = bst->num_nodes();
  old_sz = bst->size();
  bst->insert(181)->insert(181)->remove(bst->right_ch());
  std::cout << "Inserted 2x 181, removed root->right by pointer:" << std::endl;
  TEST_CHECK(bst->is_search(), "Search tree should be a valid search tree");
  TEST_EQUAL(bst->size(), old_sz-2, "Size should shrink by 2");
  TEST_EQUAL(bst->num_nodes(), old_nc-1, "Node count should be reduced by 1");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  old_nc = bst->num_nodes();
  old_sz = bst->size();
  bst->remove(bst)->remove(bst->search(41));
  std::cout << "Removed root (181) and node 41 by pointer:" << std::endl;
  TEST_CHECK(bst->is_search(), "Search tree should be a valid search tree");
  TEST_EQUAL(bst->val(), 191, "191 should be the new root");
  TEST_EQUAL(bst->size(), old_sz-6, "Size should shrink by 6");
  TEST_EQUAL(bst->num_nodes(), old_nc-2, "Node count should be reduced by 2");
  buf = bst->right_ch()->parent();
  TEST_CHECK(buf == bst, "223 should maintain its parent link to root");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  search = true;
  std::cout << "Removing 8 values in order from binary search tree: ";
  for (size_t i = 0; i < 8; ++i) {
    std::cout << bst->remove() << ((i < 12) ? " " : "");
    if (!bst->is_search())
      search = false;
  }
  std::cout << std::endl;
  TEST_EQUAL(bst->size(), 11, "Size of binary search tree should be 11");
  TEST_EQUAL(bst->num_nodes(), 11, "Node count should also be 11");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  TEST_CHECK(search, "Search tree should have always remained a search tree");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  search = balanced = true;
  std::cout << "Removing remaining elements until bst is root-only: ";
  while (!bst->root_only()) {
    std::cout << bst->remove() << ((bst->root_only()) ? "" : " ");
    if (!bst->is_search())
      search = false;
    if (!bst->is_balanced())
      balanced = false;
  }
  TEST_CHECK(bst->root_only(), "Binary search tree should be root-only");
  TEST_CHECK(search, "Search tree should have always remained a search tree");
  TEST_CHECK(balanced, "Search tree should have always remained balanced");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "BINARY SEARCH TREE WITH RANDOM PRIME NUMBERS" << std::endl;
  std::cout << std::endl;
  // Random number generator to pick from the first 50 primes in random order
  // Once a prime is inserted, it won't be inserted again.
  std::random_device rd;
  std::mt19937 eng(rd());
  // Shuffle the array of primes.
  std::shuffle(primes, primes + 50, eng);

  bst->clear();
  std::cout << "Cleared and inserted 10 random primes from [2, 229]: ";
  bst->set_val(primes[0]);
  for (int i = 1; i < 10; ++i) {
    std::cout << primes[i] << " ";
    bst->insert(primes[i]);
  }
  std::cout << std::endl;
  path = bst->traverse(IN_ORDER);
  sorted = true;
  for (size_t i = 1; i < path.size(); ++i) {
    if (path[i]->val() < path[i-1]->val()) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "In-order traversal path should have sorted values");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  std::cout << "Balanced the search tree:" << std::endl << bst->balance();
  std::cout << "Checking properties after balancing:" << std::endl;
  TEST_EQUAL(bst->is_full(), 0, "Search tree should not be full");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  TEST_EQUAL(bst->is_perfect(), 0, "Search tree should not be perfect");
  TEST_CHECK(bst->is_search(), "Search tree should be a search tree");
  bst->compute_heights();
  TEST_EQUAL(bst->height(), 3, "Balanced tree height should be 3");
  buf = bst->left_ch()->right_ch()->left_ch()->parent();
  TEST_CHECK(buf == bst->left_ch()->right_ch(), "Parents should be linked");
  std::cout << std::endl;

  std::cout << "Inserted 20 more random primes remaining in [2, 229]: ";
  for (int i = 10; i < 30; ++i) {
    std::cout << primes[i] << " ";
    bst->insert(primes[i]);
  }
  std::cout << std::endl;
  path = bst->traverse(IN_ORDER);
  sorted = true;
  for (size_t i = 1; i < path.size(); ++i) {
    if (path[i]->val() < path[i-1]->val()) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "In-order traversal path should have sorted values");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  std::cout << "Balanced the search tree:" << std::endl << bst->balance();
  std::cout << "Checking properties after balancing:" << std::endl;
  TEST_EQUAL(bst->is_full(), 0, "Search tree should not be full");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  TEST_EQUAL(bst->is_perfect(), 0, "Search tree should not be perfect");
  TEST_CHECK(bst->is_search(), "Search tree should be a search tree");
  bst->compute_heights();
  TEST_EQUAL(bst->height(), 4, "Balanced tree height should be 4");
  std::cout << std::endl;

  std::cout << "Inserted last 20 random primes remaining in [2, 229]: ";
  for (int i = 30; i < 50; ++i) {
    std::cout << primes[i] << " ";
    bst->insert(primes[i]);
  }
  std::cout << std::endl;
  path = bst->traverse(IN_ORDER);
  sorted = true;
  for (size_t i = 1; i < path.size(); ++i) {
    if (path[i]->val() < path[i-1]->val()) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "In-order traversal path should have sorted values");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  std::cout << "Balanced the search tree:" << std::endl << bst->balance();
  std::cout << "Checking properties after balancing:" << std::endl;
  TEST_EQUAL(bst->is_full(), 0, "Search tree should not be full");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  TEST_EQUAL(bst->is_perfect(), 0, "Search tree should not be perfect");
  TEST_CHECK(bst->is_search(), "Search tree should be a search tree");
  TEST_EQUAL(bst->is_heap(0), 0, "Search tree should not be a min heap tree");
  TEST_EQUAL(bst->is_heap(1), 0, "Search tree should not be a max heap tree");
  bst->compute_heights();
  TEST_EQUAL(bst->height(), 5, "Balanced tree height should be 5");
  std::cout << "Testing the search method:" << std::endl;
  TEST_EQUAL(bst->search(181)->val(), 181, "Searching for 181 should find it");
  TEST_EQUAL(bst->search(97)->val(), 97, "Searching for 97 should find it");
  TEST_EQUAL(bst->search(19)->val(), 19, "Searching for 19 should find it");
  std::cout << "Finding closest primes in interval [2, 229]:" << std::endl;
  TEST_EQUAL(bst->find_closest(2), 2, "Closest prime to 2 in intr: 2");
  TEST_EQUAL(bst->find_closest(100), 101, "Closest prime to 100 in intr: 101");
  TEST_EQUAL(bst->find_closest(300), 229, "Closest prime to 300 in intr: 229");
  std::cout << std::endl;

  bst = bst->remove(43);
  std::cout << "Removed the leaf node 43." << std::endl;
  bst = bst->remove(197);
  std::cout << "Removed the single-child node 197." << std::endl;
  bst = bst->remove(13);
  std::cout << "Removed the double-child node 13." << std::endl;
  bst = bst->remove(17)->remove(19);
  std::cout << "Removed the double-child nodes 17 and 19." << std::endl;
  path = bst->traverse(IN_ORDER);
  sorted = true;
  for (size_t i = 1; i < path.size(); ++i) {
    if (path[i]->val() < path[i-1]->val()) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "In-order traversal path should have sorted values");
  std::cout << std::endl;

  bst->balance();
  std::cout << "Balanced the search tree:" << std::endl;
  std::cout << "Checking properties after balancing:" << std::endl;
  TEST_CHECK(bst->is_full(), "Search tree should be full");
  TEST_CHECK(bst->is_balanced(), "Search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Search tree should be complete");
  TEST_EQUAL(bst->is_perfect(), 0, "Search tree should not be perfect");
  TEST_CHECK(bst->is_search(), "Search tree should be a search tree");
  bst->compute_heights();
  TEST_EQUAL(bst->height(), 5, "Balanced tree height should still be 5");
  path = bst->traverse(IN_ORDER);
  sorted = true;
  for (size_t i = 1; i < path.size(); ++i) {
    if (path[i]->val() < path[i-1]->val()) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "In-order traversal path should have sorted values");
  binary_search_tree<int, 1>* parent = bst->find_parent(151);
  TEST_EQUAL(parent->val(), 163, "Parent of 151 should be 163");
  const binary_search_tree<int, 1>* query;
  query = bst->left_ch()->left_ch()->right_ch()->right_ch();
  parent = bst->find_parent(query);
  TEST_EQUAL(parent->val(), 53, "Parent of 61 should be 53");
  std::cout << std::endl;

  std::cout << "Removed " << parent->val() << " by pointer: " << std::endl;
  bst->remove(parent);
  TEST_EQUAL(bst->search(53), nullptr, "Search for 53 should return nullptr");
  TEST_EQUAL(bst->min(), 2, "Min value in search tree should be 2");
  TEST_EQUAL(bst->max(), 229, "Max value in search tree should be 229");
  std::cout << std::endl;

  std::cout << "Removed the root node 103 by value: " << std::endl;
  bst->remove(103);
  path = bst->traverse(IN_ORDER);
  sorted = true;
  for (size_t i = 1; i < path.size(); ++i) {
    if (path[i]->val() < path[i-1]->val()) {
      sorted = false;
      break;
    }
  }
  TEST_CHECK(sorted, "In-order traversal path should have sorted values");
  std::cout << std::endl;

  balanced = search = true;
  std::cout << "Removed rest of the elements except the last: ";
  while (!bst->root_only()) {
    std::cout << bst->remove() << ((bst->root_only()) ? "" : " ");
    if (!bst->is_balanced())
      balanced = false;
    if (!bst->is_search())
      search = false;
  }
  std::cout << std::endl;
  TEST_CHECK(bst->root_only(), "Tree should be root-only");
  TEST_CHECK(balanced, "Tree should have always remained balanced");
  TEST_CHECK(search, "Tree should have always remained a search tree");
  std::cout << std::endl;

  bst->clear();
  bst->set_val(1);
  for (int i = 2; i < 10; ++i)
    bst->insert(i);
  bst->balance();
  std::cout << "Built a new balanced bst with 9 positive ints:" << std::endl;
  bst->remove(1)->remove(2);
  std::cout << "Chain-removed 2 elements: " << std::endl;
  TEST_CHECK(bst->is_search(), "Tree should still be a search tree");
  std::cout << std::endl;

  balanced = search = true;
  std::cout << "Removed rest of the elements except the last: ";
  while (!bst->root_only()) {
    std::cout << bst->remove() << ((bst->root_only()) ? "" : " ");
    if (!bst->is_balanced())
      balanced = false;
    if (!bst->is_search())
      search = false;
  }
  std::cout << std::endl;
  TEST_CHECK(bst->root_only(), "Tree should be root-only");
  TEST_CHECK(balanced, "Tree should have always remained balanced");
  TEST_CHECK(search, "Tree should have always remained a search tree");
  std::cout << std::endl;

  std::cout << "Removed the last element by value:" << std::endl;
  bst = bst->remove(bst->val());
  TEST_EQUAL(bst, nullptr, "Binary search tree should be nullptr if emptied");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "BINARY SEARCH TREE ASSIGNMENT TESTS" << std::endl << std::endl;

  // Initialize all binary tree types with a 0 root node, then assign Recaman.
  dyn_array<int> ulam, recaman, trav;
  ulam = {1, 2, 3, 4, 6, 8, 11, 13, 16, 18, 26, 28, 36, 38, 47, 48, 53, 57};
  recaman = { 0,  1,   3,  6,   2,  7, 13, 20, 12, 21, 11, 22, 10, 23,  9, 24,
              8, 25,  43, 62,  42, 63, 41, 18, 42, 17, 43, 16, 44, 15, 45, 14,
             46, 79, 113, 78, 114, 77, 39, 78, 38, 79, 37, 80, 36, 81, 35, 82,
             34, 83,  33, 84,  32, 85, 31, 86, 30, 87, 29, 88, 28, 89, 27, 90};

  binary_tree<int, 1> *bt, *bt_ins, *bt_dyn;
  binary_search_tree<int, 1> *bst_ins, *bst_dyn, *bstc, *bstcl;
  bt_ptrs<int, 1> bst_path;

  bt      = binary_tree<int, true>::STANDARD(0);
  bt_ins  = binary_tree<int, true>::STANDARD(0);
  bt_dyn  = binary_tree<int, true>::STANDARD(0);
  bst     = binary_tree<int, true>::SEARCH(0);
  bstc    = binary_tree<int, true>::SEARCH(0);
  bst_ins = binary_tree<int, true>::SEARCH(0);
  bst_dyn = binary_tree<int, true>::SEARCH(0);

  *bt = { 0,  1,   3,  6,   2,  7, 13, 20, 12, 21, 11, 22, 10, 23,  9, 24,
          8, 25,  43, 62,  42, 63, 41, 18, 42, 17, 43, 16, 44, 15, 45, 14,
         46, 79, 113, 78, 114, 77, 39, 78, 38, 79, 37, 80, 36, 81, 35, 82,
         34, 83,  33, 84,  32, 85, 31, 86, 30, 87, 29, 88, 28, 89, 27, 90};
  for (size_t i = 1; i < recaman.size(); ++i)
    bt_ins->insert(recaman[i]);
  *bt_dyn = recaman;
  std::cout << "Recaman's seq: init list -> binary tree:" << std::endl;
  TEST_CHECK(*bt == *bt_ins, "Insert and init list trees should match");
  std::cout << "Recaman's seq: dyn_array -> binary tree:" << std::endl;
  TEST_CHECK(*bt == *bt_dyn, "Insert and dyn_array trees should match");
  TEST_EQUAL(bt_dyn->height(), 0, "Height of bt_dyn should not be computed");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  *bst = { 0,  1,   3,  6,   2,  7, 13, 20, 12, 21, 11, 22, 10, 23,  9, 24,
           8, 25,  43, 62,  42, 63, 41, 18, 42, 17, 43, 16, 44, 15, 45, 14,
          46, 79, 113, 78, 114, 77, 39, 78, 38, 79, 37, 80, 36, 81, 35, 82,
          34, 83,  33, 84,  32, 85, 31, 86, 30, 87, 29, 88, 28, 89, 27, 90};
  for (size_t i = 1; i < recaman.size(); ++i)
    bst_ins->insert(recaman[i]);
  bst_ins->balance();
  *bst_dyn = recaman;
  std::cout << "Recaman's seq: init list -> binary search tree:" << std::endl;
  TEST_CHECK(*bst == *bst_ins, "Insert and init list trees should match");
  TEST_CHECK(bst->is_balanced(), "Binary search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Binary search tree should be complete");
  TEST_CHECK(bst->is_search(), "Binary search tree should be search tree");
  std::cout << "Recaman's seq: dyn_array -> binary search tree:" << std::endl;
  TEST_CHECK(*bst == *bst_dyn, "Insert and dyn_array trees should match");
  TEST_CHECK(bst_dyn->is_balanced(), "Binary search tree should be balanced");
  TEST_CHECK(bst_dyn->is_complete(), "Binary search tree should be complete");
  TEST_CHECK(bst_dyn->is_search(), "Binary search tree should be search tree");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  *bt = ulam.shuffle();
  std::cout << "Ulam numbers: dyn_array -> binary tree:" << std::endl;
  TEST_EQUAL(bt->size(), 18, "Size of binary tree should now be 18");
  TEST_CHECK(*bt != *bst, "Structures of bt and bst should now differ");
  TEST_EQUAL(bt->height(), 0, "Height of bt should not yet be computed");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  *bst = *bt;
  std::cout << "Ulam numbers: binary tree -> binary search tree:" << std::endl;
  TEST_EQUAL(bt->size(), bst->size(), "Sizes of bt and bst should match");
  TEST_CHECK(*bt != *bst, "Structures of bt and bst should differ");
  check = bt->left()->type() != bst->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bt and bst should differ");
  TEST_CHECK(bst->is_balanced(), "Binary search tree should be balanced");
  TEST_CHECK(bst->is_complete(), "Binary search tree should be complete");
  TEST_CHECK(bst->is_search(), "Binary search tree should be a search tree");
  bst_path = bst->traverse(IN_ORDER);
  trav.clear();
  for (size_t i = 0; i < bst_path.size(); ++i)
    trav.add(bst_path[i]->val());
  ulam.merge_sort();
  TEST_SEQ(trav, ulam, 18, "In-order traversal values should be sorted");
  std::cout << "Resulting search tree:" << std::endl << bst << std::endl;

  *bst = recaman.shuffle();
  *bt = *bst;
  std::cout << "Recaman's seq: binary search tree -> binary tree:";
  std::cout << std::endl;
  TEST_CHECK(*bt == *bst, "Structures of bt and bst should match");
  check = bt->size() == bst->num_nodes();
  TEST_CHECK(check, "Node counts of bt and bst should match");
  TEST_CHECK(bt->size() != bst->size(), "Sizes of bt and bst should differ");
  check = bt->left()->type() != bst->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bt and bst should differ");
  TEST_EQUAL(bt->height(), 5, "Height of bt should be 5");
  TEST_CHECK(bt->is_balanced(), "Binary tree should be balanced");
  TEST_CHECK(bt->is_complete(), "Binary tree should be complete");
  TEST_CHECK(bt->is_search(), "Binary tree should be a search tree");
  std::cout << std::endl;

  *bstc = *bst;
  bstcl = bstc->clone();
  std::cout << "Recaman's seq: binary search tree -> binary search tree:";
  std::cout << std::endl;
  TEST_CHECK(*bst == *bstc, "Structures of bst and bstc should match");
  TEST_CHECK(*bst == *bstcl, "Structures of bst and bstcl should match");
  check = bst->size() == bstc->size();
  TEST_CHECK(check, "Sizes of bst and bstc should match");
  check = bst->size() == bstcl->size();
  TEST_CHECK(check, "Sizes of bst and bstcl should match");
  check = bst->left_ch()->type() == bstc->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bst and bstc should match");
  check = bst->left_ch()->type() == bstcl->left_ch()->type();
  TEST_CHECK(check, "Dynamic types of bst and bstcl should match");
  check = bstc->is_balanced() && bstcl->is_balanced();
  TEST_CHECK(check, "Both binary search trees should be balanced");
  check = bstc->is_complete() && bstcl->is_complete();
  TEST_CHECK(check, "Both binary search trees should be complete");
  check = bstc->is_search() && bstcl->is_search();
  TEST_CHECK(check, "Both binary search trees should be search trees");

  delete bt;
  delete bt_ins;
  delete bt_dyn;
  delete bst;
  delete bstc;
  delete bstcl;
  delete bst_ins;
  delete bst_dyn;
  test_util::print_summary();
  return 0;
}