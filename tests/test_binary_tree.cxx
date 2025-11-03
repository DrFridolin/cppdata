#include <iostream>
#include <random>
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
  std::cout << "MANUALLY CONSTRUCTED UNBALANCED BINARY TREE" << std::endl;

  dyn_array<int> leaf_gt;
  bt_ptrs<int, true> leaves;
  dyn_array<int> lpgt;
  dyn_array<bt_ptrs<int, true> > leaf_paths;

  binary_tree<int, true>* bt = binary_tree<int, true>::create(0);
  std::cout << "Initialized the bt (root) node with 0" << std::endl;
  TEST_EQUAL(bt->val(), 0, "Root node value should be 0");
  TEST_EQUAL(bt->height(), 0, "Root node height should be 0");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  std::cout << "Checking properties of the root-only tree:" << std::endl;
  TEST_CHECK(bt->is_full(), "Root-only tree should be full");
  TEST_CHECK(bt->is_balanced(),"Root-only tree should be balanced");
  TEST_CHECK(bt->is_complete(), "Root-only tree should be complete");
  TEST_CHECK(bt->is_perfect(), "Root-only tree should be perfect");
  TEST_CHECK(bt->is_search(), "Root-only tree should be a search tree");
  TEST_EQUAL(bt->height(), 0, "Root-only tree's root should have height 0");
  std::cout << std::endl;

  binary_tree<int, true>* buf;
  binary_tree<int, true>* child = binary_tree<int, true>::create(1);
  bt->left() = child;
  std::cout << "Added 1 to the left of 0 (root)" << std::endl;
  TEST_EQUAL(bt->left()->val(), 1, "Left child value should be 1");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  std::cout << "Checking the binding: left() <-> adj_[0]" << std::endl;
  TEST_EQUAL(bt->left()->val(), (*bt)[0]->val(), "Vals should be equal");
  TEST_EQUAL(bt->left(), (*bt)[0], "Pointers should be equal");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  bt->right() = child->clone();
  std::cout << "Cloned 1 to the right of 0 (root)" << std::endl;
  TEST_EQUAL(bt->right()->val(), 1, "Right child value should be 1");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  std::cout << "Check the binding: right() <-> adj_[1]" << std::endl;
  TEST_EQUAL(bt->right()->val(), (*bt)[1]->val(), "Vals should be equal");
  TEST_EQUAL(bt->right(), (*bt)[1], "Pointers should be equal");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  bt->left()->set_val(12);
  std::cout << "Changed the value of 0.left to 12" << std::endl;
  TEST_EQUAL(bt->left()->val(), 12, "Left child value should now be 12");
  TEST_EQUAL((*bt)[0]->val(), 12, "Value of adj_[0] should be 12");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  bt->left()->left() = binary_tree<int, true>::create(13);
  bt->left()->right() = binary_tree<int, true>::create(20);
  std::cout << "12.left = 13 | 12.right = 20" << std::endl;
  TEST_EQUAL(bt->left()->left()->val(), 13, "12.left = 13");
  TEST_EQUAL(bt->left()->right()->val(), 20, "12.right = 20");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  bt->left()->left()->left() = binary_tree<int, true>::create(14);
  bt->left()->left()->right() = binary_tree<int, true>::create(18);
  std::cout << "13.left = 14 | 13.right = 18" << std::endl;
  TEST_EQUAL(bt->left()->left()->left()->val(), 14, "13.left = 14");
  TEST_EQUAL(bt->left()->left()->right()->val(), 18, "13.right = 18");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  bt->left()->left()->left()->left() = binary_tree<int, true>::create(15);
  bt->left()->left()->right()->right() = binary_tree<int, true>::create(19);
  std::cout << "14.left = 15 | 18.right = 19" << std::endl;
  TEST_EQUAL(bt->left()->left()->left()->left()->val(), 15, "14.left = 15");
  TEST_EQUAL(bt->left()->left()->right()->right()->val(), 19, "18.right = 19");
  leaf_gt = {15, 19, 20, 1};
  leaves = bt->find_leaves();
  TEST_PTR_SEQ(leaves, leaf_gt, leaf_gt.size(), "All leaves should be found");
  leaf_paths = bt->generate_leaf_paths();
  lpgt = {0, 12, 13, 14, 15};
  TEST_PTR_SEQ(leaf_paths[0], lpgt, 5, "Path to leaf 15: {0 12 13 14 15}");
  lpgt = {0, 12, 13, 18, 19};
  TEST_PTR_SEQ(leaf_paths[1], lpgt, 5, "Path to leaf 19: {0 12 13 18 19}");
  lpgt = {0, 12, 20};
  TEST_PTR_SEQ(leaf_paths[2], lpgt, 3, "Path to leaf 20: {0 12 20}");
  lpgt = {0, 1};
  TEST_PTR_SEQ(leaf_paths[3], lpgt, 2, "Path to leaf  1: {0 1}");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  buf = binary_tree<int, true>::create(16);
  bt->left()->left()->left()->left()->left() = buf;
  buf = binary_tree<int, true>::create(17);
  bt->left()->left()->left()->left()->right() = buf;
  std::cout << "15.left = 16 | 15.right = 17" << std::endl;
  binary_tree<int, true>* node15 = bt->left()->left()->left()->left();
  TEST_EQUAL(node15->left()->val(), 16, "15.left = 16");
  TEST_EQUAL(node15->right()->val(), 17, "15.right = 17");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  bt->right()->right() = binary_tree<int, true>::create(2);
  bt->right()->right()->left() = binary_tree<int, true>::create(11);
  bt->right()->right()->right() = binary_tree<int, true>::create(3);
  bt->right()->right()->right()->right() = binary_tree<int, true>::create(4);
  std::cout << "1.right = 2 | 2.left  = 11" << std::endl;
  std::cout << "2.right = 3 | 3.right =  4" << std::endl;
  TEST_EQUAL(bt->right()->right()->val(), 2, "1.right = 2");
  TEST_EQUAL(bt->right()->right()->left()->val(), 11, "2.left = 11");
  TEST_EQUAL(bt->right()->right()->right()->val(), 3, "2.right = 3");
  TEST_EQUAL(bt->right()->right()->right()->right()->val(), 4, "3.right = 4");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  binary_tree<int, true>* node4 = bt->right()->right()->right()->right();
  node4->left() = binary_tree<int, true>::create(5);
  node4->right() = binary_tree<int, true>::create(10);
  node4->left()->right() = binary_tree<int, true>::create(6);
  node4->left()->right()->left() = binary_tree<int, true>::create(7);
  node4->left()->right()->left()->left() = binary_tree<int, true>::create(8);
  node4->left()->right()->left()->right() = binary_tree<int, true>::create(9);
  std::cout << "4.left  = 5 | 4.right = 10" << std::endl;
  std::cout << "5.right = 6 | 6.left  = 7" << std::endl;
  std::cout << "7.left  = 8 | 7.right = 9" << std::endl;
  TEST_EQUAL(node4->left()->val(), 5, "4.left = 5");
  TEST_EQUAL(node4->right()->val(), 10, "4.right = 10");
  TEST_EQUAL(node4->left()->right()->val(), 6, "5.right = 6");
  TEST_EQUAL(node4->left()->right()->left()->val(), 7, "6.left = 7");
  TEST_EQUAL(node4->left()->right()->left()->left()->val(), 8, "7.left = 8");
  TEST_EQUAL(node4->left()->right()->left()->right()->val(), 9, "7.right = 9");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  std::cout << "Verifying tree node heights: " << bt->height() << std::endl;
  bt->compute_heights();
  size_t height0 = node4->left()->right()->left()->right()->height();
  size_t height1 = node4->left()->right()->left()->height();
  size_t height2 = node4->left()->right()->height();
  size_t height3 = node4->right()->height();
  size_t height4 = bt->left()->left()->right()->right()->height();
  size_t height5 = bt->right()->right()->right()->height();
  size_t height6 = bt->left()->left()->height();
  size_t height7 = bt->right()->height();
  size_t height8 = bt->height();
  TEST_EQUAL(height0, 0, "Node 9  should have height 0");
  TEST_EQUAL(height1, 1, "Node 7  should have height 1");
  TEST_EQUAL(height2, 2, "Node 6  should have height 2");
  TEST_EQUAL(height3, 0, "Node 10 should have height 0");
  TEST_EQUAL(height4, 0, "Node 19 should have height 0");
  TEST_EQUAL(height5, 5, "Node 3  should have height 5");
  TEST_EQUAL(height6, 3, "Node 13 should have height 3");
  TEST_EQUAL(height7, 7, "Node 1  should have height 7");
  TEST_EQUAL(height8, 8, "Node 0  should have height 8");
  std::cout << std::endl;

  std::cout << "Checking properties of the binary tree:" << std::endl;
  TEST_EQUAL(bt->is_full(), 0, "Tree should not be full");
  TEST_EQUAL(bt->is_balanced(), 0, "Tree should not be balanced");
  TEST_EQUAL(bt->is_complete(), 0, "Tree should not be complete");
  TEST_EQUAL(bt->is_perfect(), 0, "Tree should not be perfect");
  TEST_EQUAL(bt->is_search(), 0, "Tree should not be a search tree");
  std::cout << std::endl;

  int pre1[] = {0, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 2, 11, 3, 4, 5,
                6, 7, 8, 9, 10};
  int in1[]  = {16, 15, 17, 14, 13, 18, 19, 12, 20, 0, 1, 11, 2, 3, 5, 8,
                7, 9, 6, 4, 10};
  int pos1[] = {16, 17, 15, 14, 19, 18, 13, 20, 12, 11, 8, 9, 7, 6, 5, 10,
                4, 3, 2, 1, 0};
  int lvl1[] = {0, 12, 1, 13, 20, 2, 14, 18, 11, 3, 15, 19, 4, 16, 17, 5,
                10, 6, 7, 8, 9};
  int df1[]  = {0, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 2, 11, 3, 4, 5,
                6, 7, 8, 9, 10};
  int bf1[]  = {0, 12, 1, 13, 20, 2, 14, 18, 11, 3, 15, 19, 4, 16, 17, 5,
                10, 6, 7, 8, 9};
  test_trav(bt, pre1, in1, pos1, lvl1, df1, bf1);
  TEST_EQUAL(bt->height(), 8, "Tree height should be 8");
  leaf_gt = {16, 17, 19, 20, 11, 8, 9, 10};
  leaves = bt->find_leaves();
  TEST_PTR_SEQ(leaves, leaf_gt, leaf_gt.size(), "All leaves should be found");
  leaf_paths = bt->generate_leaf_paths();
  lpgt = {0, 12, 13, 14, 15, 16};
  TEST_PTR_SEQ(leaf_paths[0], lpgt, 6, "Path to leaf 16: {0 12 13 14 15 16}");
  lpgt = {0, 12, 13, 14, 15, 17};
  TEST_PTR_SEQ(leaf_paths[1], lpgt, 6, "Path to leaf 17: {0 12 13 14 15 17}");
  lpgt = {0, 12, 13, 18, 19};
  TEST_PTR_SEQ(leaf_paths[2], lpgt, 5, "Path to leaf 19: {0 12 13 18 19}");
  lpgt = {0, 12, 20};
  TEST_PTR_SEQ(leaf_paths[3], lpgt, 3, "Path to leaf 20: {0 12 20}");
  lpgt = {0, 1, 2, 11};
  TEST_PTR_SEQ(leaf_paths[4], lpgt, 4, "Path to leaf 11: {0 1 2 11}");
  lpgt = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  TEST_PTR_SEQ(leaf_paths[5], lpgt, 9, "Path to leaf 8: {0 1 2 3 4 5 6 7 8}");
  lpgt = {0, 1, 2, 3, 4, 5, 6, 7, 9};
  TEST_PTR_SEQ(leaf_paths[6], lpgt, 9, "Path to leaf 9: {0 1 2 3 4 5 6 7 9}");
  lpgt = {0, 1, 2, 3, 4, 10};
  TEST_PTR_SEQ(leaf_paths[7], lpgt, 6, "Path to leaf 10: {0 1 2 3 4 10}");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "MANUALLY CONSTRUCTED FULL BINARY TREE" << std::endl;

  binary_tree<int, true>* node_5;
  bt->left()->left()->left()->right() = bt->left()->left()->right()->right();
  bt->left()->left()->right()->right() = nullptr;
  std::cout << "14.right = 19 | 18.right = **" << std::endl;
  bt->right()->left() = binary_tree<int, true>::create(21);
  bt->right()->right()->right()->left() = binary_tree<int, true>::create(22);
  std::cout << "1.left   = 21 | 3.left   = 22" << std::endl;
  node_5 = bt->right()->right()->right()->right()->left();
  node_5->left() = binary_tree<int, true>::create(23);
  node_5->right()->right() = binary_tree<int, true>::create(24);
  std::cout << "5.left   = 23 | 6.right  = 24" << std::endl;
  TEST_EQUAL(bt->left()->left()->left()->right()->val(), 19, "14.right = 19");
  TEST_EQUAL(bt->left()->left()->right()->right(), nullptr, "18.right = **");
  TEST_EQUAL(bt->right()->left()->val(), 21, "1.left = 21");
  TEST_EQUAL(bt->right()->right()->right()->left()->val(), 22, "3.left = 22");
  TEST_EQUAL(node_5->left()->val(), 23, "5.left = 23");
  TEST_EQUAL(node_5->right()->right()->val(), 24, "6.right = 24");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  int prnt;
  binary_tree<int, true>* parent;
  binary_tree<int, true>* unlinked = binary_tree<int, true>::create(200);
  std::cout << "Checking properties of the binary tree:" << std::endl;
  TEST_CHECK(bt->is_full(), "Tree should be full");
  TEST_EQUAL(bt->is_balanced(), 0, "Tree should not be balanced");
  TEST_EQUAL(bt->is_complete(), 0, "Tree should not be complete");
  TEST_EQUAL(bt->is_perfect(), 0, "Tree should not be perfect");
  TEST_EQUAL(bt->is_search(), 0, "Tree should not be a search tree");
  TEST_EQUAL(bt->search(9)->val(), 9, "Search for 9 should find 9");
  TEST_EQUAL(bt->search(26), nullptr, "Search for 26 should return null");
  prnt = bt->find_parent(10)->val();
  TEST_EQUAL(prnt, 4, "Search for 10's parent should find 4");
  parent = bt->find_parent(-3);
  TEST_EQUAL(parent, nullptr, "Search for -3's parent should return null");
  prnt = bt->find_parent(bt->left()->left()->left()->left()->right())->val();
  TEST_EQUAL(prnt, 15, "Search for node 17's parent should find 15");
  parent = bt->find_parent(bt);
  TEST_EQUAL(parent, nullptr, "Search for root's parent should return null");
  std::cout << std::endl;

  int pre2[] = {0, 12, 13, 14, 15, 16, 17, 19, 18, 20, 1, 21, 2, 11, 3, 22, 4,
                5, 23, 6, 7, 8, 9, 24, 10};
  int in2[]  = {16, 15, 17, 14, 19, 13, 18, 12, 20, 0, 21, 1, 11, 2, 22, 3, 23,
                5, 8, 7, 9, 6, 24, 4, 10};
  int pos2[] = {16, 17, 15, 19, 14, 18, 13, 20, 12, 21, 11, 22, 23, 8, 9, 7,
                24, 6, 5, 10, 4, 3, 2, 1, 0};
  int lvl2[] = {0, 12, 1, 13, 20, 21, 2, 14, 18, 11, 3, 15, 19, 22, 4, 16, 17,
                5, 10, 23, 6, 7, 24, 8, 9};
  int df2[]  = {0, 12, 13, 14, 15, 16, 17, 19, 18, 20, 1, 21, 2, 11, 3, 22, 4,
                5, 23, 6, 7, 8, 9, 24, 10};
  int bf2[]  = {0, 12, 1, 13, 20, 21, 2, 14, 18, 11, 3, 15, 19, 22, 4, 16, 17,
                5, 10, 23, 6, 7, 24, 8, 9};
  test_trav(bt, pre2, in2, pos2, lvl2, df2, bf2);
  TEST_EQUAL(bt->height(), 8, "Tree height should be 8");
  leaf_gt = {16, 17, 19, 18, 20, 21, 11, 22, 23, 8, 9, 24, 10};
  leaves = bt->find_leaves();
  TEST_PTR_SEQ(leaves, leaf_gt, leaf_gt.size(), "All leaves should be found");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "MANUALLY CONSTRUCTED BALANCED BINARY TREE" << std::endl;

  binary_tree<int, true> *bt_rr, *bt_rl;
  bt->left()->left()->right()->right() = binary_tree<int, true>::create(30);
  bt->left()->right()->right() = binary_tree<int, true>::create(28);
  bt->left()->right()->right()->right() = binary_tree<int, true>::create(24);
  bt->left()->right()->right()->left() = binary_tree<int, true>::create(25);
  bt->left()->right()->left() = binary_tree<int, true>::create(26);
  bt->left()->right()->left()->left() = binary_tree<int, true>::create(27);
  std::cout << "18.right = 30 | 20.right = 28" << std::endl;
  std::cout << "28.right = 24 | 28.left  = 25" << std::endl;
  std::cout << "20.left  = 26 | 26.right = 27" << std::endl;
  node_5 = bt->right()->right()->right()->right()->left();
  delete node_5->right()->left()->right();
  node_5->right()->left()->right() = nullptr;
  delete node_5->right()->left()->left();
  node_5->right()->left()->left() = nullptr;
  delete node_5->right()->left();
  node_5->right()->left() = nullptr;
  delete node_5->right()->right();
  node_5->right()->right() = nullptr;
  std::cout << "7.left   = ** | 7.right  = **" << std::endl;
  std::cout << "6.left   = ** | 6.right  = **" << std::endl;
  bt_rr = bt->right()->right();
  bt_rr->right()->left()->right() = binary_tree<int, true>::create(8);
  bt_rr->left()->right() = binary_tree<int, true>::create(29);
  bt_rr->left()->left() = binary_tree<int, true>::create(31);
  bt_rr->left()->right()->right() = binary_tree<int, true>::create(7);
  std::cout << "22.right = 8  | 11.right = 29" << std::endl;
  std::cout << "11.left  = 31 | 29.left  = 7" << std::endl;
  bt->right()->left()->left() = binary_tree<int, true>::create(32);
  bt->right()->left()->right() = binary_tree<int, true>::create(33);
  std::cout << "21.left  = 32 | 21.right = 33" << std::endl;
  bt->right()->left()->right()->left() = binary_tree<int, true>::create(34);
  bt->right()->left()->right()->right() = binary_tree<int, true>::create(35);
  std::cout << "33.left  = 34 | 33.right = 35" << std::endl;
  bt_rl = bt->right()->left();
  bt_rl->right()->right()->right() = binary_tree<int, true>::create(9);
  bt_rl->left()->left() = binary_tree<int, true>::create(36);
  std::cout << "35.right = 9  | 32.left  = 36" << std::endl;
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  std::cout << "Checking properties of the binary tree:" << std::endl;
  TEST_EQUAL(bt->is_full(), 0, "Tree should not be full");
  TEST_CHECK(bt->is_balanced(), "Tree should be balanced");
  TEST_EQUAL(bt->is_complete(), 0, "Tree should not be complete");
  TEST_EQUAL(bt->is_perfect(), 0, "Tree should not be perfect");
  TEST_EQUAL(bt->is_search(), 0, "Tree should not be a search tree");
  TEST_EQUAL(bt->is_heap(0), 0, "Tree should not be a min heap tree");
  TEST_EQUAL(bt->is_heap(1), 0, "Tree should not be a max heap tree");
  TEST_EQUAL(bt->search(18)->val(), 18, "Search for 18 should find 18");
  TEST_EQUAL(bt->search(-18), nullptr, "Search for -18 should return null");
  prnt = bt->find_parent(1)->val();
  TEST_EQUAL(prnt, 0, "Search for 1's parent should find 0");
  parent = bt->find_parent(0);
  TEST_EQUAL(parent, nullptr, "Search for 0's parent should return null");
  prnt = bt->find_parent(bt->right()->left()->left()->left())->val();
  TEST_EQUAL(prnt, 32, "Search for node 36's parent should find 32");
  parent = bt->find_parent(unlinked);
  TEST_EQUAL(parent, nullptr, "Search for unlinked node should return null");
  TEST_EQUAL(bt_rl->prnt(), 0, "Parents should not be linked yet");
  std::cout << std::endl;

  bt->link_parents();
  std::cout << "Linked parents of all nodes in binary tree" << std::endl;
  TEST_CHECK(bt_rl->prnt() == bt->right(), "21's parent should now be 1");
  parent = bt->find_parent(9);
  TEST_CHECK(parent->right()->prnt() == parent, "9's parent should now be 35");
  TEST_CHECK(parent->find_root() == bt, "Should find correct root from 35");
  std::cout << std::endl;

  int pre3[] = {0, 12, 13, 14, 15, 16, 17, 19, 18, 30, 20, 26, 27, 28, 25, 24,
    1, 21, 32, 36, 33, 34, 35, 9, 2, 11, 31, 29, 7, 3, 22, 8, 4, 5, 23, 6, 10};
  int in3[]  = {16, 15, 17, 14, 19, 13, 18, 30, 12, 27, 26, 20, 25, 28, 24, 0,
    36, 32, 21, 34, 33, 35, 9, 1, 31, 11, 29, 7, 2, 22, 8, 3, 23, 5, 6, 4, 10};
  int pos3[] = {16, 17, 15, 19, 14, 30, 18, 13, 27, 26, 25, 24, 28, 20, 12, 36,
    32, 34, 9, 35, 33, 21, 31, 7, 29, 11, 8, 22, 23, 6, 5, 10, 4, 3, 2, 1, 0};
  int lvl3[] = {0, 12, 1, 13, 20, 21, 2, 14, 18, 26, 28, 32, 33, 11, 3, 15, 19,
    30, 27, 25, 24, 36, 34, 35, 31, 29, 22, 4, 16, 17, 9, 7, 8, 5, 10, 23, 6};
  int df3[]  = {0, 12, 13, 14, 15, 16, 17, 19, 18, 30, 20, 26, 27, 28, 25, 24,
    1, 21, 32, 36, 33, 34, 35, 9, 2, 11, 31, 29, 7, 3, 22, 8, 4, 5, 23, 6, 10};
  int bf3[]  = {0, 12, 1, 13, 20, 21, 2, 14, 18, 26, 28, 32, 33, 11, 3, 15, 19,
    30, 27, 25, 24, 36, 34, 35, 31, 29, 22, 4, 16, 17, 9, 7, 8, 5, 10, 23, 6};
  test_trav(bt, pre3, in3, pos3, lvl3, df3, bf3);
  bt->compute_heights();
  TEST_EQUAL(bt->height(), 6, "Tree height should be 6");
  leaf_gt = {16, 17, 19, 30, 27, 25, 24, 36, 34, 9, 31, 7, 8, 23, 6, 10};
  leaves = bt->find_leaves();
  TEST_PTR_SEQ(leaves, leaf_gt, leaf_gt.size(), "All leaves should be found");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "INSERTION-FINISHED COMPLETE BINARY TREE" << std::endl;
  // Insert elements in level-order until tree becomes complete
  for (int i = 37; i < 125; ++i)
    bt->insert(i);
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  std::cout << "Checking properties of the binary tree:" << std::endl;
  TEST_CHECK(bt->is_full(), "Tree should be full");
  TEST_CHECK(bt->is_balanced(), "Tree should be balanced");
  TEST_CHECK(bt->is_complete(), "Tree should be complete");
  TEST_EQUAL(bt->is_perfect(), 0, "Tree should not be perfect");
  TEST_EQUAL(bt->is_search(), 0, "Tree should not be a search tree");
  std::cout << std::endl;

  int pre4[] = {0, 12, 13, 14, 15, 16, 65, 66, 17, 67, 68, 19, 40, 69, 70, 41,
   71, 72, 18, 37, 42, 73, 74, 43, 75, 76, 30, 44, 77, 78, 45, 79, 80, 20, 26,
   27, 46, 81, 82, 47, 83, 84, 38, 48, 85, 86, 49, 87, 88, 28, 25, 50, 89, 90,
   51, 91, 92, 24, 52, 93, 94, 53, 95, 96, 1, 21, 32, 36, 54, 97, 98, 55, 99,
   100, 39, 56, 101, 102, 57, 103, 104, 33, 34, 58, 105, 106, 59, 107, 108, 35,
   60, 109, 110, 9, 111, 112, 2, 11, 31, 61, 113, 114, 62, 115, 116, 29, 63,
   117, 118, 7, 119, 120, 3, 22, 64, 121, 122, 8, 123, 124, 4, 5, 23, 6, 10};
  int in4[]  = {65, 16, 66, 15, 67, 17, 68, 14, 69, 40, 70, 19, 71, 41, 72, 13,
   73, 42, 74, 37, 75, 43, 76, 18, 77, 44, 78, 30, 79, 45, 80, 12, 81, 46, 82,
   27, 83, 47, 84, 26, 85, 48, 86, 38, 87, 49, 88, 20, 89, 50, 90, 25, 91, 51,
   92, 28, 93, 52, 94, 24, 95, 53, 96, 0, 97, 54, 98, 36, 99, 55, 100, 32, 101,
   56, 102, 39, 103, 57, 104, 21, 105, 58, 106, 34, 107, 59, 108, 33, 109, 60,
   110, 35, 111, 9, 112, 1, 113, 61, 114, 31, 115, 62, 116, 11, 117, 63, 118,
   29, 119, 7, 120, 2, 121, 64, 122, 22, 123, 8, 124, 3, 23, 5, 6, 4, 10};
  int pos4[] = {65, 66, 16, 67, 68, 17, 15, 69, 70, 40, 71, 72, 41, 19, 14, 73,
   74, 42, 75, 76, 43, 37, 77, 78, 44, 79, 80, 45, 30, 18, 13, 81, 82, 46, 83,
   84, 47, 27, 85, 86, 48, 87, 88, 49, 38, 26, 89, 90, 50, 91, 92, 51, 25, 93,
   94, 52, 95, 96, 53, 24, 28, 20, 12, 97, 98, 54, 99, 100, 55, 36, 101, 102,
   56, 103, 104, 57, 39, 32, 105, 106, 58, 107, 108, 59, 34, 109, 110, 60, 111,
   112, 9, 35, 33, 21, 113, 114, 61, 115, 116, 62, 31, 117, 118, 63, 119, 120,
   7, 29, 11, 121, 122, 64, 123, 124, 8, 22, 23, 6, 5, 10, 4, 3, 2, 1, 0};
  int lvl4[] = {0, 12, 1, 13, 20, 21, 2, 14, 18, 26, 28, 32, 33, 11, 3, 15, 19,
   37, 30, 27, 38, 25, 24, 36, 39, 34, 35, 31, 29, 22, 4, 16, 17, 40, 41, 42,
   43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 9,
   61, 62, 63, 7, 64, 8, 5, 10, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
   77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
   96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
   112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 23, 6};
  int df4[]  = {0, 12, 13, 14, 15, 16, 65, 66, 17, 67, 68, 19, 40, 69, 70, 41,
   71, 72, 18, 37, 42, 73, 74, 43, 75, 76, 30, 44, 77, 78, 45, 79, 80, 20, 26,
   27, 46, 81, 82, 47, 83, 84, 38, 48, 85, 86, 49, 87, 88, 28, 25, 50, 89, 90,
   51, 91, 92, 24, 52, 93, 94, 53, 95, 96, 1, 21, 32, 36, 54, 97, 98, 55, 99,
   100, 39, 56, 101, 102, 57, 103, 104, 33, 34, 58, 105, 106, 59, 107, 108, 35,
   60, 109, 110, 9, 111, 112, 2, 11, 31, 61, 113, 114, 62, 115, 116, 29, 63,
   117, 118, 7, 119, 120, 3, 22, 64, 121, 122, 8, 123, 124, 4, 5, 23, 6, 10};
  int bf4[]  = {0, 12, 1, 13, 20, 21, 2, 14, 18, 26, 28, 32, 33, 11, 3, 15, 19,
   37, 30, 27, 38, 25, 24, 36, 39, 34, 35, 31, 29, 22, 4, 16, 17, 40, 41, 42,
   43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 9,
   61, 62, 63, 7, 64, 8, 5, 10, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
   77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
   96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
   112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 23, 6};
  test_trav(bt, pre4, in4, pos4, lvl4, df4, bf4);
  TEST_EQUAL(bt->height(), 6, "Tree height should be 6");
  leaf_gt = {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
             81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
             97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
             111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
             124, 23, 6, 10};
  leaves = bt->find_leaves();
  TEST_PTR_SEQ(leaves, leaf_gt, leaf_gt.size(), "All leaves should be found");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "INSERTION-FINISHED PERFECT BINARY TREE" << std::endl;

  std::cout << std::endl;
  std::cout << "Two more nodes should make the tree perfect" << std::endl;
  bt->insert(125);
  std::cout << "Inserted first node with value 125" << std::endl;
  TEST_EQUAL(bt->is_full(), 0, "Tree should not be full yet");
  TEST_CHECK(bt->is_balanced(), "Tree should be balanced");
  TEST_CHECK(bt->is_complete(), "Tree should be complete");
  TEST_EQUAL(bt->is_perfect(), 0, "Tree should not yet be perfect");
  TEST_EQUAL(bt->is_search(), 0, "Tree should not be a search tree");
  std::cout << std::endl;

  bt->insert(126);
  std::cout << "Inserted second node with value 126" << std::endl;
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;
  std::cout << "Checking tree properties after second insertion:" << std::endl;
  TEST_CHECK(bt->is_full(), "Tree should be full");
  TEST_CHECK(bt->is_balanced(), "Tree should be balanced");
  TEST_CHECK(bt->is_complete(), "Tree should be complete");
  TEST_CHECK(bt->is_perfect(), "Tree should be perfect");
  TEST_EQUAL(bt->is_search(), 0, "Tree should not be a search tree");
  TEST_EQUAL(bt->is_heap(0), 0, "Tree should not be a min heap tree");
  TEST_EQUAL(bt->is_heap(1), 0, "Tree should not be a max heap tree");
  TEST_EQUAL(bt->search(42)->val(), 42, "Search for 42 should find 42");
  TEST_EQUAL(bt->search(200), nullptr, "Search for 200 should return null");
  prnt = bt->find_parent(64)->val();
  TEST_EQUAL(prnt, 22, "Search for 64's parent should find 22");
  parent = bt->find_parent(-5);
  TEST_EQUAL(parent, nullptr, "Search for -5's parent should return null");
  prnt = bt->find_parent(bt->left()->right()->left()->right())->val();
  TEST_EQUAL(prnt, 26, "Search for node 38's parent should find 26");
  parent = bt->find_parent(unlinked);
  TEST_EQUAL(parent, nullptr, "Search for unlinked node should return null");
  leaf_gt.remove();
  leaf_gt.add(125).add(126);
  leaves = bt->find_leaves();
  TEST_PTR_SEQ(leaves, leaf_gt, leaf_gt.size(), "All leaves should be found");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "SEQUENTIAL AND TARGETED DELETIONS ON BINARY TREE" << std::endl;

  bool check;
  int rep1, rep2, rep3, rep4;
  binary_tree<int, true> *node1, *node2, *node3;
  std::cout << "Removed nodes: ";
  for (int i = 0; i < 72; ++i)
    std::cout << bt->remove() << " ";
  std::cout << std::endl;
  TEST_CHECK(bt->is_full(), "Tree should be full");
  TEST_CHECK(bt->is_balanced(), "Tree should be balanced");
  TEST_CHECK(bt->is_complete(), "Tree should be complete");
  TEST_EQUAL(bt->is_perfect(), 0, "Tree should no longer be perfect");
  TEST_EQUAL(bt->is_search(), 0, "Tree should not be a search tree");
  std::cout << "Resulting binary tree:" << std::endl << bt << std::endl;

  size_t sz = bt->size();
  std::cout << "Removed 32 by value:" << std::endl;
  parent = bt->right()->left();
  rep1 = parent->left()->left()->val();
  bt = bt->remove(parent->left()->val());
  TEST_EQUAL(parent->left()->val(), rep1, "32.left should move up");
  TEST_EQUAL(bt->size(), --sz, "Only one node should be removed");
  std::cout << std::endl;

  rep1 = parent->left()->left()->val();
  rep2 = parent->left()->left()->right()->val();
  bt = bt->remove(parent->left()->val());
  std::cout << "Removed 36 by value:" << std::endl;
  TEST_EQUAL(parent->left()->val(), rep1, "36.left should move up");
  TEST_EQUAL(parent->left()->left()->val(), rep2, "54.right should move up");
  TEST_EQUAL(bt->size(), --sz, "Only one node should be removed");
  std::cout << std::endl;

  rep1 = parent->left()->left()->val();
  rep2 = parent->left()->right()->val();
  bt = bt->remove(parent->left()->val());
  std::cout << "Removed 54 by value:" << std::endl;
  TEST_EQUAL(parent->left()->val(), rep1, "54.left should move up");
  TEST_EQUAL(parent->left()->left(), nullptr, "55.left should not exist");
  TEST_EQUAL(parent->left()->right()->val(), rep2, "39 should not move");
  TEST_EQUAL(bt->size(), --sz, "Only one node should be removed");
  std::cout << std::endl;

  parent = bt->search(14);
  rep1 = parent->left()->left()->val();
  bt = bt->remove(parent->left()->val());
  std::cout << "Removed 15 by value:" << std::endl;
  TEST_EQUAL(parent->left()->val(), rep1, "15.left should move up");
  TEST_EQUAL(bt->size(), --sz, "Only one node should be removed");
  std::cout << std::endl;

  rep1 = parent->left()->right()->val();
  bt = bt->remove(parent->left()->val());
  std::cout << "Removed 16 by value:" << std::endl;
  TEST_EQUAL(parent->left()->val(), rep1, "16.right should move up");
  TEST_EQUAL(bt->size(), --sz, "Only one node should be removed");
  std::cout << std::endl;

  rep1 = parent->right()->val();
  bt = bt->remove(parent->left()->val());
  std::cout << "Removed 17 by value:" << std::endl;
  TEST_EQUAL(parent->left(), nullptr, "14.left should not exist");
  TEST_EQUAL(parent->right()->val(), rep1, "19 should not move");
  TEST_EQUAL(bt->size(), --sz, "Only one node should be removed");
  std::cout << std::endl;

  rep1 = bt->left()->val();
  rep2 = bt->right()->val();
  rep3 = parent->val();
  rep4 = parent->right()->val();
  bt = bt->remove(0);
  std::cout << "Removed 0 by value:" << std::endl;
  check = parent->right() == bt->left()->left()->right();
  TEST_EQUAL(bt->val(), rep1, "12 should be the new root");
  TEST_EQUAL(bt->right()->val(), rep2, "1 should not move");
  TEST_EQUAL(bt->left()->left()->val(), rep3, "14 should move up");
  TEST_CHECK(check, "18 should now be 14.right");
  TEST_EQUAL(parent->left()->val(), rep4, "14.right should now be 14.left");
  TEST_EQUAL(bt->size(), --sz, "Only one node should be removed");
  std::cout << std::endl;

  node1 = bt->right()->left();
  node2 = bt->right()->left()->left();
  node3 = bt->right()->left()->left()->right();
  node4 = bt->right()->left()->right();
  bt = bt->remove(bt->right());
  std::cout << "Removed root's right child by pointer:" << std::endl;
  TEST_EQUAL(bt->right(), node1, "1.left should move up");
  TEST_EQUAL(bt->right()->left(), node2, "21.left should move up");
  check = bt->right()->left()->left() == node3;
  TEST_CHECK(check, "55.right should now be 55.left");
  TEST_CHECK(bt->right()->left()->right() == node4, "33 should not move");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "BINARY TREE ASSIGNMENT TESTS" << std::endl << std::endl;

  bt->clear()->set_val(0);
  dyn_array<int> seq = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  bt_ptrs<int, true> path;

  *bt = seq;
  std::cout << "Integer sequence: dyn_array -> binary_tree:" << std::endl;
  path = bt->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(path, seq, 15, "Should have inserted elements in level-order");

  seq.reverse();
  *bt = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  std::cout << "Integer sequence: init list -> binary_tree:" << std::endl;
  path = bt->traverse(LEVEL_ORDER);
  TEST_PTR_SEQ(path, seq, 15, "Should have inserted elements in level-order");

  *unlinked = *bt;
  std::cout << "Assigned binary tree to unlinked tree:" << std::endl;
  TEST_CHECK(*bt == *unlinked, "Trees should now be identical");

  delete unlinked;
  delete bt;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "BINARY TREE SUBTREE TESTS" << std::endl << std::endl;

  // Create the subtree
  binary_tree<char, 0>* t2 = binary_tree<char, 0>::STANDARD('a');
  t2->insert('b');
  t2->insert('c');
  t2->insert('d');
  t2->insert('e');
  std::cout << "t2_in:" << std::endl << t2;

  dyn_array<char> in_gt = {'d', 'b', 'e', 'a', 'c'};
  dyn_array<char> pre_gt = {'a', 'b', 'd', 'e', 'c'};
  dyn_array<char> post_gt = {'d', 'e', 'b', 'c', 'a'};

  std::cout << "In-order traversal: ";
  bt_ptrs<char, 0> in = t2->traverse(IN_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(in, in_gt, 5, "In-order traversal path should match gt");

  std::cout << "Pre-order traversal: ";
  bt_ptrs<char, 0> pre = t2->traverse(PRE_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(pre, pre_gt, 5, "Pre-order traversal path should match gt");

  std::cout << "Post-order traversal: ";
  bt_ptrs<char, 0> post = t2->traverse(POST_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(post, post_gt, 5, "Post-order traversal path should match gt");
  std::cout << std::endl;

  // Create another tree with the same in-order traversal as t2
  binary_tree<char, 0>* t2_in = binary_tree<char, 0>::STANDARD('a');
  t2_in->insert('d')->right() = binary_tree<char, 0>::STANDARD('e');
  t2_in->insert('c');
  t2_in->left()->right()->left() = binary_tree<char, 0>::STANDARD('b');
  std::cout << "t2_in:" << std::endl << t2_in;

  std::cout << "In-order traversal: ";
  in = t2_in->traverse(IN_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(in, in_gt, 5, "In-order path should be same as t2");
  std::cout << std::endl;

  // Create another tree with the same pre-order traversal as t2
  binary_tree<char, 0>* t2_pre = binary_tree<char, 0>::STANDARD('a');
  t2_pre->insert('b')->right() = binary_tree<char, 0>::STANDARD('d');
  t2_pre->insert('e')->right() = binary_tree<char, 0>::STANDARD('c');
  std::cout << "t2_pre:" << std::endl << t2_pre;

  std::cout << "Pre-order traversal: ";
  pre = t2_pre->traverse(PRE_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(pre, pre_gt, 5, "Pre-order path should be same as t2");
  std::cout << std::endl;

  // Create another tree with the same post-order traversal as t2
  binary_tree<char, 0>* t2_post = binary_tree<char, 0>::STANDARD('a');
  t2_post->insert('e')->left() = binary_tree<char, 0>::STANDARD('d');
  t2_post->insert('c')->right() = binary_tree<char, 0>::STANDARD('b');
  std::cout << "t2_post:" << std::endl << t2_post;

  std::cout << "Post-order traversal: ";
  post = t2_post->traverse(POST_ORDER, &std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(post, post_gt, 5, "Post-order path should be same as t2");
  std::cout << std::endl;

  // Create a clone of t2
  binary_tree<char, 0>* t2_clone = t2->clone();
  std::cout << "Cloned t2:" << std::endl;
  TEST_CHECK(*t2 == *t2_clone, "Cloned trees should be equal");
  std::cout << std::endl;

  // Create a large tree and append the t2 clone to one of its leaf nodes
  char charset[11] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'};
  // Random index generator to randomly pick from the charset
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> picker(0, 10);

  binary_tree<char, 0>* t1 = binary_tree<char, 0>::STANDARD('a');
  for (size_t i = 0; i < 99; ++i)
    t1->insert(charset[picker(eng)]);
  std::cout << "Built larger tree t1 with random val insertions" << std::endl;
  TEST_EQUAL(t1->size(), 100, "Tree size should be equal to number of nodes");
  std::cout << std::endl;

  binary_tree<char, 0>* t1_clone = t1->clone();
  binary_tree<char, 0>* leaf = t1_clone->right()->right();
  while (leaf->left())
    leaf = leaf->left();
  binary_tree<char, 0>* t2_leaf = t2->clone();
  leaf->right() = t2_leaf;
  std::cout << "Cloned t1 and appended t2_clone to a leaf node" << std::endl;
  TEST_CHECK(t2->is_subtree(t1_clone), "t2 is a subtree of t1_clone");
  TEST_CHECK(t1_clone->has_subtree(t2), "t1_clone has t2 as subtree");
  std::cout << std::endl;

  binary_tree<char, 0>* t1_in = t1->clone();
  leaf = t1_in->right();
  while (leaf->left())
    leaf = leaf->left();
  t2_leaf = t2_in->clone();
  leaf->right() = t2_leaf;
  std::cout << "Cloned t1 and appended t2_in to a leaf node" << std::endl;
  TEST_CHECK(t2_in->is_subtree(t1_in), "t2_in is a subtree of t1_in");
  TEST_CHECK(t1_in->has_subtree(t2_in), "t1_in has t2_in as subtree");
  std::cout << std::endl;

  binary_tree<char, 0>* t1_pre = t1->clone();
  leaf = t1_pre->left()->left();
  while (leaf->right())
    leaf = leaf->right();
  t2_leaf = t2_pre->clone();
  leaf->left() = t2_leaf;
  std::cout << "Cloned t1 and appended t2_pre to a leaf node" << std::endl;
  TEST_CHECK(t2_pre->is_subtree(t1_pre), "t2_pre is a subtree of t1_pre");
  TEST_CHECK(t1_pre->has_subtree(t2_pre), "t1_pre has t2_pre as subtree");
  std::cout << std::endl;

  binary_tree<char, 0>* t1_post = t1->clone();
  leaf = t1_post->left();
  while (leaf->left())
    leaf = leaf->left();
  t2_leaf = t2_post->clone();
  leaf->left() = t2_leaf;
  std::cout << "Cloned t1 and appended t2_post to a leaf node" << std::endl;
  TEST_CHECK(t2_post->is_subtree(t1_post), "t2_post is a subtree of t1_post");
  TEST_CHECK(t1_post->has_subtree(t2_post), "t1_post has t2_post as subtree");
  std::cout << std::endl;

  std::cout << "Testing is_subtree and has_subtree results:" << std::endl;
  TEST_CHECK(t2->is_subtree(t2_clone), "t2 is t2_clone sub");
  TEST_CHECK(t2_clone->has_subtree(t2), "t2_clone has t2 as sub");
  TEST_EQUAL(t2->is_subtree(t1), 0, "t2 isn't t1 sub");
  TEST_EQUAL(t1->has_subtree(t2), 0, "t1 has no t2 sub");
  TEST_CHECK(t2->is_subtree(t1_clone), "t2 is t1_clone sub");
  TEST_CHECK(t1_clone->has_subtree(t2), "t1_clone has t2 as sub");
  TEST_EQUAL(t2_in->is_subtree(t1_clone), 0, "t2_in isn't t1_clone sub");
  TEST_EQUAL(t1_clone->has_subtree(t2_in), 0, "t1_clone has no t2_in sub");
  TEST_EQUAL(t2_pre->is_subtree(t1_clone), 0, "t2_pre isn't t1_clone sub");
  TEST_EQUAL(t1_clone->has_subtree(t2_pre), 0, "t1_clone has no t2_pre sub");
  TEST_EQUAL(t2_post->is_subtree(t1_clone), 0, "t2_post isn't t1_clone sub");
  TEST_EQUAL(t1_clone->has_subtree(t2_post), 0, "t1_clone has no t2_post sub");
  TEST_EQUAL(t2->is_subtree(t1_in), 0, "t2 isn't t1_in sub");
  TEST_EQUAL(t1_in->has_subtree(t2), 0, "t1_in has no t2 sub");
  TEST_EQUAL(t2->is_subtree(t1_pre), 0, "t2 isn't t1_pre sub");
  TEST_EQUAL(t1_pre->has_subtree(t2), 0, "t1_pre has no t2 sub");
  TEST_EQUAL(t2->is_subtree(t1_post), 0, "t2 isn't t1_post sub");
  TEST_EQUAL(t1_post->has_subtree(t2), 0, "t1_post has no t2 sub");

  delete t1;
  delete t1_clone;
  delete t1_in;
  delete t1_pre;
  delete t1_post;
  delete t2;
  delete t2_clone;
  delete t2_in;
  delete t2_pre;
  delete t2_post;
  test_util::print_summary();
  return 0;
}