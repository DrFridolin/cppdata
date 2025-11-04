// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "binary_tree.h"
#include "stack.h"
#include "queue.h"

//======================================================================//
//                                                                      //
//                      binary_tree<T, P> methods                       //
//                                                                      //
//======================================================================//

template <class T, bool P>
template <bool B>
typename std::enable_if<!B, binary_tree<T, P>*>::type
binary_tree<T, P>::create(const T& v, BinaryTreeType type)
{
  switch (type)
  {
    case BinaryTreeType::STANDARD:  return new binary_tree<T, P>(v);
    case BinaryTreeType::SEARCH:    return new binary_search_tree<T, P>(v);
    case BinaryTreeType::MIN_HEAP:  return new heap_tree<T, P, false>(v);
    case BinaryTreeType::MAX_HEAP:  return new heap_tree<T, P, true>(v);
    case BinaryTreeType::AVL:       return new avl_tree<T, P>(v);
    default:                        return nullptr;
  }
}

template <class T, bool P>
template <bool B>
typename std::enable_if<B, binary_tree<T, P>*>::type
binary_tree<T, P>::create(const T& v, BinaryTreeType type)
{
  switch (type)
  {
    case BinaryTreeType::STANDARD:  return new binary_tree<T, P>(v);
    case BinaryTreeType::SEARCH:    return new binary_search_tree<T, P>(v);
    case BinaryTreeType::MIN_HEAP:  return new heap_tree<T, P, false>(v);
    case BinaryTreeType::MAX_HEAP:  return new heap_tree<T, P, true>(v);
    case BinaryTreeType::AVL:       return new avl_tree<T, P>(v);
    case BinaryTreeType::RED_BLACK: return new red_black_tree<T>(v);
    default:                        return nullptr;
  }
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::STANDARD(const T& v)
{
  return binary_tree<T, P>::create(v, BinaryTreeType::STANDARD);
}

template <class T, bool P>
binary_search_tree<T, P>* binary_tree<T, P>::SEARCH(const T& v)
{
  binary_tree<T, P>* buf;
  buf = binary_tree<T, P>::create(v, BinaryTreeType::SEARCH);
  return dynamic_cast<binary_search_tree<T, P>*>(buf);
}

template <class T, bool P>
heap_tree<T, P, false>* binary_tree<T, P>::MIN_HEAP(const T& v)
{
  binary_tree<T, P>* buf;
  buf = binary_tree<T, P>::create(v, BinaryTreeType::MIN_HEAP);
  return dynamic_cast<heap_tree<T, P, false>*>(buf);
}

template <class T, bool P>
heap_tree<T, P, true>* binary_tree<T, P>::MAX_HEAP(const T& v)
{
  binary_tree<T, P>* buf;
  buf = binary_tree<T, P>::create(v, BinaryTreeType::MAX_HEAP);
  return dynamic_cast<heap_tree<T, P, true>*>(buf);
}

template <class T, bool P>
avl_tree<T, P>* binary_tree<T, P>::AVL(const T& v)
{
  binary_tree<T, P>* buf;
  buf = binary_tree<T, P>::create(v, BinaryTreeType::AVL);
  return dynamic_cast<avl_tree<T, P>*>(buf);
}

template <class T, bool P>
template <bool B>
rb_ok<T, B> binary_tree<T, P>::RED_BLACK(const T& v)
{
  binary_tree<T, true>* buf;
  buf = binary_tree<T, true>::create(v, BinaryTreeType::RED_BLACK);
  return dynamic_cast<red_black_tree<T>*>(buf);
}

template <class T, bool P>
binary_tree<T, P>::~binary_tree()
{
  // If left and/or right children exist, trigger their destructors
  if (this->left())
    delete this->left();
  this->left() = nullptr;
  if (this->right())
    delete this->right();
  this->right() = nullptr;
  if (P)
    this->prnt() = nullptr;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::clone() const
{
  return this->clone(this->type());
}

template <class T, bool P>
binary_tree<T, P>*& binary_tree<T, P>::left()
{
  return reinterpret_cast<binary_tree<T, P>*&>(this->adj_[0]);
}

template <class T, bool P>
binary_tree<T, P>*& binary_tree<T, P>::right()
{
  return reinterpret_cast<binary_tree<T, P>*&>(this->adj_[1]);
}

template <class T, bool P>
binary_tree<T, P>*& binary_tree<T, P>::prnt()
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning pointer to left child." << std::endl;
    return reinterpret_cast<binary_tree<T, P>*&>(this->adj_[0]);
  }
  return reinterpret_cast<binary_tree<T, P>*&>(this->adj_[2]);
}

template <class T, bool P>
const binary_tree<T, P>* binary_tree<T, P>::left() const
{
  return static_cast<const binary_tree<T, P>*>(this->adj_[0]);
}

template <class T, bool P>
const binary_tree<T, P>* binary_tree<T, P>::right() const
{
  return static_cast<const binary_tree<T, P>*>(this->adj_[1]);
}

template <class T, bool P>
const binary_tree<T, P>* binary_tree<T, P>::prnt() const
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning nullptr." << std::endl;
    return static_cast<const binary_tree<T, P>*>(this->adj_[0]);
  }
  return static_cast<const binary_tree<T, P>*>(this->adj_[2]);
}

template <class T, bool P>
template <bool B>
typename std::enable_if<!B, typename binary_tree<T, P>::BinaryTreeType>::type
binary_tree<T, P>::type() const
{
  if (dynamic_cast<const avl_tree<T, P>*>(this))
    return BinaryTreeType::AVL;
  else if (dynamic_cast<const binary_search_tree<T, P>*>(this))
    return BinaryTreeType::SEARCH;
  else if (dynamic_cast<const heap_tree<T, P, false>*>(this))
    return BinaryTreeType::MIN_HEAP;
  else if (dynamic_cast<const heap_tree<T, P, true>*>(this))
    return BinaryTreeType::MAX_HEAP;
  else
    return BinaryTreeType::STANDARD;
}

template <class T, bool P>
template <bool B>
typename std::enable_if<B, typename binary_tree<T, P>::BinaryTreeType>::type
binary_tree<T, P>::type() const
{
  if (dynamic_cast<const red_black_tree<T>*>(this))
    return BinaryTreeType::RED_BLACK;
  else if (dynamic_cast<const avl_tree<T, P>*>(this))
    return BinaryTreeType::AVL;
  else if (dynamic_cast<const binary_search_tree<T, P>*>(this))
    return BinaryTreeType::SEARCH;
  else if (dynamic_cast<const heap_tree<T, P, false>*>(this))
    return BinaryTreeType::MIN_HEAP;
  else if (dynamic_cast<const heap_tree<T, P, true>*>(this))
    return BinaryTreeType::MAX_HEAP;
  else
    return BinaryTreeType::STANDARD;
}

template <class T, bool P>
size_t binary_tree<T, P>::compute_heights()
{
  bt_ptrs<T, P> path;
  auto check_hcomp = [](
    binary_tree<T, P>* bt, intptr_t retl, intptr_t retr) -> int {
    if (bt == nullptr) return -1;
    intptr_t height = (retl > retr) ? (retl + 1) : (retr + 1);
    bt->height() = static_cast<size_t>(height);
    return static_cast<int>(height);
  };
  // Post-order traverse with height computation lambda and no accumulation.
  int height = this->traversal_engine(check_hcomp, path, POST_ORDER, false);
  return static_cast<size_t>(height);
}

template <class T, bool P>
template <bool B>
typename std::enable_if<B, binary_tree<T, P>*>::type
binary_tree<T, P>::link_parents()
{
  bt_ptrs<T, P> path;
  auto check_parent = [](binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return -1;
    if (bt->left()) bt->left()->prnt() = bt;
    if (bt->right()) bt->right()->prnt() = bt;
    return 1;
  };
  // Pre-order traverse with parent linker lambda and no accumulation.
  this->traversal_engine(check_parent, path, PRE_ORDER, false);
  return this;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::clear()
{
  // Delete all descendants.
  if (this->left())
    delete this->left();
  if (this->right())
    delete this->right();
  // Nullify the child pointers.
  this->left() = nullptr;
  this->right() = nullptr;
  if (P)
    this->prnt() = nullptr;
  // Reset class members.
  this->set_members(nullptr);
  return this;
}

template <class T, bool P>
bool binary_tree<T, P>::operator == (const binary_tree<T, P>& other) const
{
  cbt_ptrs<T, P> path;
  // Capture other tree's traversal stack in lambda to track it concurrently.
  stack<const binary_tree<T, P>*> other_stack;
  other_stack.push(&other);
  // Lambda returns -1 if bt's value or child count doesn't match other.
  auto check_equal = [&other_stack](
    const binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr || other_stack.is_empty()) return -1;
    const binary_tree<T, P>* other_ptr = other_stack.pop();
    if (other_ptr == nullptr) return -1;
    if (bt->val() != other_ptr->val()) return -1;
    if ((bt->left() == nullptr) != (other_ptr->left() == nullptr)) return -1;
    if ((bt->right() == nullptr) != (other_ptr->right() == nullptr)) return -1;
    if (other_ptr->right()) other_stack.push(other_ptr->right());
    if (other_ptr->left()) other_stack.push(other_ptr->left());
    return 1;
  };
  // Pre-order traverse with equality check lambda and no accumulation.
  return this->traversal_engine(check_equal, path, PRE_ORDER, false) >= 0;
}

template <class T, bool P>
bool binary_tree<T, P>::operator != (const binary_tree<T, P>& other) const
{
  return !(*this == other);
}

template <class T, bool P>
binary_tree<T, P>& binary_tree<T, P>::operator = (const binary_tree<T, P>& src)
{
  if (this == &src)
    return *this;
  this->clear();
  this->set_members(&src);
  // Heights come for free with recursive cloning.
  size_t depth_left = 0;
  size_t depth_right = 0;
  if (src.left()) {
    this->left() = src.left()->clone(this->type());
    if (P)
      this->left()->prnt() = this;
    depth_left = 1 + this->left()->height();
  }
  if (src.right()) {
    this->right() = src.right()->clone(this->type());
    if (P)
      this->right()->prnt() = this;
    depth_right = 1 + this->right()->height();
  }
  this->height() = (depth_left > depth_right) ? depth_left : depth_right;
  return *this;
}

template <class T, bool P>
binary_tree<T, P>& binary_tree<T, P>::operator = (std::initializer_list<T> list)
{
  if (list.size() == 0)
    return *this;
  this->clear();
  dyn_array<T> buf(list);
  *this = buf;
  return *this;
}

template <class T, bool P>
binary_tree<T, P>& binary_tree<T, P>::operator = (const dyn_array<T>& src)
{
  if (src.is_empty())
    return *this;
  // Clear tree, set node value and initialize queue with this node.
  this->clear();
  this->set_val(src[0]);
  queue<binary_tree<T, P>*> parents(this);
  binary_tree<T, P>* cur;
  size_t ind = 1;

  // Level-order insert dyn_array elements using a parent queue.
  // Can't use stratify engine since there is no tree for it to traverse.
  // Can't reuse insert(val) because calling it on each value is wasteful.
  while (ind < src.size() && !parents.is_empty()) {
    cur = parents.dequeue();
    cur->left() = binary_tree<T, P>::create(src[ind++], this->type());
    if (P)
      cur->left()->prnt() = cur;
    parents.enqueue(cur->left());

    if (ind >= src.size())
      break;

    cur->right() = binary_tree<T, P>::create(src[ind++], this->type());
    if (P)
      cur->right()->prnt() = cur;
    parents.enqueue(cur->right());
  }
  return *this;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::last(binary_tree<T, P>*& parent)
{
  dyn_array<bt_ptrs<T, P> > paths;
  // Lambda always returns 1 to be able to reach the last element.
  auto check_none = [](binary_tree<T, P>*, intptr_t, intptr_t) -> int {
    return 1;
  };
  // Level-order traverse without accumulating path.
  parent = this->stratify(check_none, paths, false);
  return paths[0][0];
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::next()
{
  // Find the first nullptr child in a level-order traversal.
  dyn_array<bt_ptrs<T, P> > paths;
  // Lambda returns -1 if node is missing a child.
  auto check_null = [](binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return -1;
    if (bt->left() == nullptr || bt->right() == nullptr) return -1;
    return 1;
  };
  // Level-order traverse with missing child seeker lambda and no accumulation.
  this->stratify(check_null, paths, false);
  return paths[0][0];
}

template <class T, bool P>
bool binary_tree<T, P>::is_full() const
{
  cbt_ptrs<T, P> path;
  // Lambda returns -1 if the node has only 1 child.
  auto check_fll = [](const binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return -1;
    if ((bt->left() == nullptr) != (bt->right() == nullptr)) return -1;
    return 1;
  };
  // Pre-order traverse with fullness validator lambda and no accumulation.
  return this->traversal_engine(check_fll, path, PRE_ORDER, false) >= 0;
}

template <class T, bool P>
bool binary_tree<T, P>::is_balanced() const
{
  cbt_ptrs<T, P> path;
  // Lambda returns -1 if height difference between branches is more than 1.
  auto check_bal = [](
    const binary_tree<T, P>* bt, intptr_t retl, intptr_t retr) -> int {
    if (bt == nullptr) return -1;
    intptr_t hdiff = (retl > retr) ? (retl - retr) : (retr - retl);
    int height = static_cast<int>(((retl > retr) ? (retl + 1) : (retr + 1)));
    return (hdiff > 1) ? -1 : height;
  };
  // Post-order traverse with balance checker lambda and no accumulation.
  return this->traversal_engine(check_bal, path, POST_ORDER, false) >= 0;
}

template <class T, bool P>
bool binary_tree<T, P>::is_complete() const
{
  // A root-only binary tree is considered complete.
  if (this->root_only())
    return true;
  if (this->left() == nullptr || this->right() == nullptr)
    return false;
  dyn_array<cbt_ptrs<T, P> > paths;
  bool null_found = false;
  size_t queue_size = 0;
  // Lambda returns -1 if there are nodes after a null child.
  auto check_comp = [&null_found, &queue_size](
    const binary_tree<T, P>* bt, intptr_t, intptr_t pr_que) -> int {
    if (bt == nullptr) return -1;
    queue_size = reinterpret_cast<queue<binary_tree<T, P>*>*>(pr_que)->size();
    if (null_found && (bt->left() || bt->right())) return -1;
    if (bt->left() == nullptr || bt->right() == nullptr) null_found = true;
    return 1;
  };
  // Level-order traverse with completeness checker lambda and no accumulation.
  this->stratify(check_comp, paths, false);
  return queue_size == 0;
}

template <class T, bool P>
bool binary_tree<T, P>::is_perfect() const
{
  // A root-only binary tree is considered perfect.
  if (this->root_only())
    return true;
  if (!this->left() || !this->right())
    return false;

  bt_ptrs<T, P> path;
  // Since this method is const, compute height for the tree rather than nodes.
  auto check_height = [](
    binary_tree<T, P>* bt, intptr_t retl, intptr_t retr) -> int {
    return static_cast<int>((retl > retr) ? (retl + 1) : (retr + 1));
  };
  // Post-order traverse with tree height lambda and no accumulation.
  int height = this->traversal_engine(check_height, path, POST_ORDER, false);

  // Count the number of nodes to check against the height.
  int count = 0;
  auto check_count = [&count](
    binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return 0;
    count++; return 1;
  };
  // Pre-order traverse with the node counter lambda and no accumulation.
  this->traversal_engine(check_count, path, PRE_ORDER, false);

  // Tree is perfect if it has 2^(H+1)-1 nodes.
  return count == (1ULL << (height + 1)) - 1;
}

template <class T, bool P>
bool binary_tree<T, P>::is_search() const
{
  cbt_ptrs<T, P> path;
  // Lambda returns -1 if node's value is smaller than previously visited node.
  auto check_bst = [&path](
    const binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return -1;
    if (path[0] && (bt->val() < path[0]->val())) return -1;
    return 1;
  };
  // In-order traverse with bst validator lambda and no accumulation.
  return this->traversal_engine(check_bst, path, IN_ORDER, false) >= 0;
}

template <class T, bool P>
bool binary_tree<T, P>::is_heap(bool max) const
{
  cbt_ptrs<T, P> path;
  // Lambda returns -1 if bt violates specified heap condition.
  auto check_heap = [max](
    const binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return -1;
    if (bt->left() && max && bt->val() < bt->left()->val()) return -1;
    if (bt->left() && !max && bt->val() > bt->left()->val()) return -1;
    if (bt->right() && max && bt->val() < bt->right()->val()) return -1;
    if (bt->right() && !max && bt->val() > bt->right()->val()) return -1;
    return 1;
  };
  // Pre-order traverse with heap checker lambda and no accumulation.
  return this->traversal_engine(check_heap, path, PRE_ORDER, false) >= 0;
}

template <class T, bool P>
bt_ptrs<T, P> binary_tree<T, P>::traverse(TravType type, std::ostream* osp)
{
  // Call from const version of this node to avoid code repetition.
  const binary_tree<T, P>& cth = static_cast<const binary_tree<T, P>&>(*this);
  cbt_ptrs<T, P> cpath = cth.traverse(type, osp);
  // Safely const cast results as this method itself is not const.
  bt_ptrs<T, P> path(nullptr, cpath.size());
  for (size_t i = 0; i < cpath.size(); ++i)
    path[i] = const_cast<binary_tree<T, P>*>(cpath[i]);
  return path;
}

template <class T, bool P>
cbt_ptrs<T, P> binary_tree<T, P>::traverse(TravType type, std::ostream* osp) const
{
  cbt_ptrs<T, P> path;
  dyn_array<cbt_ptrs<T, P> > paths;
  // Lambda always returns 1 to fully traverse the tree.
  auto check_none = [](const binary_tree<T, P>*, intptr_t, intptr_t) -> int {
    return 1;
  };
  if (type == LEVEL_ORDER) {
    this->stratify(check_none, paths);
    path = paths[0];
    for (size_t lvl = 1; lvl < paths.size(); ++lvl)
      path.append(paths[lvl]);
  }
  else {
    this->traversal_engine(check_none, path, type);
  }
  if (osp) {
    for (size_t i = 0; i < path.size(); ++i) {
      *osp << path[i]->val();
      if (path[i] == this)
        *osp << "[Root]";
      if (!path[i]->left() && !path[i]->right())
        *osp << "[Leaf]";
      if (i < path.size() - 1)
        *osp << " -> ";
    }
  }
  return path;
}

template <class T, bool P>
template <typename Check, typename Path>
int binary_tree<T, P>::traversal_engine(Check&& check,
                                        Path& path,
                                        TravType type,
                                        bool accumulate,
                                        intptr_t depth) const
{
  if (type == LEVEL_ORDER) {
    std::cerr << "ERROR! Traversal engine is for recursive traversals only";
    std::cerr << std::endl;
    return -1;
  }
  // Safe const cast only strips artificial const from this pointer.
  // Valid for mutable objects in non-const calls, no-mutation in const calls.
  binary_tree<T, P>* node = const_cast<binary_tree<T, P>*>(this);
  // Return variables for left branch, this node, right branch respectively.
  int rl = 0, rt = 0, rr = 0;
  // Post-order initializes return values with -1 so leaf nodes have height 0.
  if (type == POST_ORDER) {rl--; rt--; rr--;}
  intptr_t path_addr = reinterpret_cast<intptr_t>(&path);
  if (!accumulate)
    path.resize(1);
  // Check for an early-return failure for pre-order before any recursion.
  if (type == PRE_ORDER) {
    if ((rt = check(node, depth, path_addr)) == -1)
      if (check(nullptr, -1, -1) == -1)
        return -1;
    (accumulate) ? (path.add(node)) : (path[0] = node);
  }
  // Recurse into left branch, then check for an early-return failure.
  if (node->left())
    if ((rl = node->left()->traversal_engine(check, path, type,
                                             accumulate, depth+1)) == -1)
      if (check(nullptr, -1, -1) == -1)
        return -1;
  // Check for an early-return failure for in-order between branch recursions.
  if (type == IN_ORDER) {
    if ((rt = check(node, rl, path_addr)) == -1)
      if (check(nullptr, -1, -1) == -1)
        return -1;
    (accumulate) ? (path.add(node)) : (path[0] = node);
  }
  // Recurse into right branch, then check for an early-return failure.
  if (node->right())
    if ((rr = node->right()->traversal_engine(check, path, type,
                                              accumulate, depth+1)) == -1)
      if (check(nullptr, -1, -1) == -1)
        return -1;
  // For post-order: Add node to path, post-process and return branch results.
  if (type == POST_ORDER) {
    (accumulate) ? (path.add(node)) : (path[0] = node);
    return check(node, rl, rr);
  }
  // For pre-order and in-order: Aggregate and return signals.
  return (rl == -1 || rt == -1 || rr == -1) ? -1 : rt;
}

template <class T, bool P>
template <typename Check, typename BT_Ptr>
BT_Ptr binary_tree<T, P>::stratify(
    Check&& check, dyn_array<dyn_array<BT_Ptr> >& path, bool accumulate) const
{
  // Safe const cast only strips artificial const from this pointer.
  // Valid for mutable objects in non-const calls, no-mutation in const calls.
  BT_Ptr root = const_cast<binary_tree<T, P>*>(this);
  BT_Ptr child, parent;
  queue<BT_Ptr> ch_que(root), pr_que(nullptr);
  int lvl = 0;
  size_t lvl_size = 1;
  path.resize(1);
  if (!accumulate)
    path[0].resize(1);
  // Traverse until the end or a stop condition is encountered.
  while (!ch_que.is_empty()) {
    child = ch_que.dequeue();
    parent = pr_que.dequeue();
    (accumulate) ? (path[lvl].add(child)) : (path[0][0] = child);
    lvl_size--;
    intptr_t parent_queue_ptr = reinterpret_cast<intptr_t>(&pr_que);
    // Custom checkpoint.
    if (check(child, lvl, parent_queue_ptr) == -1)
      if (check(nullptr, -1, -1) == -1)
        return parent;
    // Enqueue children and current child's index as their parent.
    if (child->left()) {
      ch_que.enqueue(child->left());
      pr_que.enqueue(child);
    }
    if (child->right()) {
      ch_que.enqueue(child->right());
      pr_que.enqueue(child);
    }
    // Adjust when end of the level is reached.
    if (lvl_size == 0) {
      lvl++;
      lvl_size = ch_que.size();
      if (lvl_size > 0 && accumulate)
        path.resize(lvl+1);
    }
  }
  // Child is last element in path, so return its parent if fully traversed.
  return parent;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::search(const T& val)
{
  bt_ptrs<T, P> path;
  // Lambda returns -1 when value is found to exit immediately.
  auto check_val = [val, &path](
    binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return -1;
    if (bt->val() == val) {path[0] = bt; return -1;}
    return 1;
  };
  // Pre-order traverse with value searcher lambda and no accumulation.
  this->traversal_engine(check_val, path, PRE_ORDER, false);
  return (path[0]->val() == val) ? path[0] : nullptr;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::find_parent(const T& val)
{
  bt_ptrs<T, P> path;
  // Lambda returns -1 when value is found to exit immediately.
  auto check_chval = [val, &path](
    binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return -1;
    if (bt->left() && bt->left()->val() == val) {path[0] = bt; return -1;}
    if (bt->right() && bt->right()->val() == val) {path[0] = bt; return -1;}
    return 1;
  };
  // Pre-order traverse with child value searcher lambda and no accumulation.
  this->traversal_engine(check_chval, path, PRE_ORDER, false);
  bool found = false;
  if (path[0]->left() && path[0]->left()->val() == val)
    found = true;
  else if (path[0]->right() && path[0]->right()->val() == val)
    found = true;
  return found ? path[0] : nullptr;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::find_parent(
    const binary_tree<T, P>* child)
{
  if (child == nullptr)
    return nullptr;

  bt_ptrs<T, P> path;
  // Lambda returns -1 when value is found to exit immediately.
  auto check_chptr = [child, &path](
    binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr) return -1;
    if (bt->left() == child || bt->right() == child) {path[0] = bt; return -1;}
    return 1;
  };
  // Pre-order traverse with child node searcher lambda and no accumulation.
  this->traversal_engine(check_chptr, path, PRE_ORDER, false);
  bool found = path[0]->left() == child || path[0]->right() == child;
  return found ? path[0] : nullptr;
}

template <class T, bool P>
cbt_ptrs<T, P> binary_tree<T, P>::find(const T& val) const
{
  cbt_ptrs<T, P> path, ret;
  // Lambda always returns 1 but collects matches in internal dyn_array.
  auto check_val = [val, &ret](
    const binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt && bt->val() == val) ret.add(bt);
    return 1;
  };
  // Pre-order traverse with match collector lambda and no accumulation.
  this->traversal_engine(check_val, path, PRE_ORDER, false);
  return ret;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::find_root()
{
  binary_tree<T, P>* root = this;
  if (P)
    while (root->prnt())
      root = root->prnt();
  return root;
}

template <class T, bool P>
const binary_tree<T, P>* binary_tree<T, P>::find_root() const
{
  const binary_tree<T, P>* root = this;
  if (P)
    while (root->prnt())
      root = root->prnt();
  return root;
}

template <class T, bool P>
bt_ptrs<T, P> binary_tree<T, P>::find_leaves()
{
  bt_ptrs<T, P> path, ret;
  // Lambda always returns 1 but collects leaves in internal dyn_array.
  auto check_leaf = [&ret](
    binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt && bt->left() == nullptr && bt->right() == nullptr) ret.add(bt);
    return 1;
  };
  // Pre-order traverse with leaf collector lambda and no accumulation.
  this->traversal_engine(check_leaf, path, PRE_ORDER, false);
  return ret;
}

template <class T, bool P>
cbt_ptrs<T, P> binary_tree<T, P>::find_leaves() const
{
  cbt_ptrs<T, P> path, ret;
  // Lambda always returns 1 but collects leaves in internal dyn_array.
  auto check_leaf = [&ret](
    const binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt && bt->left() == nullptr && bt->right() == nullptr) ret.add(bt);
    return 1;
  };
  // Pre-order traverse with leaf collector lambda and no accumulation.
  this->traversal_engine(check_leaf, path, PRE_ORDER, false);
  return ret;
}

template <class T, bool P>
dyn_array<bt_ptrs<T, P> > binary_tree<T, P>::generate_leaf_paths()
{
  bt_ptrs<T, P> path;
  dyn_array<bt_ptrs<T, P> > ret;
  // Lambda always returns 1 but collects paths in 2D internal dyn_array.
  auto check_paths = [&ret](
    binary_tree<T, P>* bt, intptr_t lpath, intptr_t rpath) -> int {
    int sz = static_cast<int>(ret.size());
    if (!bt->left() && !bt->right()) {ret.add(bt_ptrs<T, P>(bt)); return sz;}
    for (int i = 0; i < ret.size(); ++i)
      if (ret[i].last() == bt->left() || ret[i].last() == bt->right())
        ret[i].add(bt);
    return 1;
  };
  // Post-order traverse with leaf path generator lambda and no accumulation.
  this->traversal_engine(check_paths, path, POST_ORDER, false);
  // Reverse each path so they all start from root.
  for (size_t i = 0; i < ret.size(); ++i)
    ret[i].reverse();
  return ret;
}

template <class T, bool P>
dyn_array<cbt_ptrs<T, P> > binary_tree<T, P>::generate_leaf_paths() const
{
  cbt_ptrs<T, P> path;
  dyn_array<cbt_ptrs<T, P> > ret;
  // Lambda always returns 1 but collects paths in 2D internal dyn_array.
  auto check_paths = [&ret](
    const binary_tree<T, P>* bt, intptr_t lpath, intptr_t rpath) -> int {
    int sz = static_cast<int>(ret.size());
    if (!bt->left() && !bt->right()) {ret.add(cbt_ptrs<T, P>(bt)); return sz;}
    for (int i = 0; i < ret.size(); ++i)
      if (ret[i].last() == bt->left() || ret[i].last() == bt->right())
        ret[i].add(bt);
    return 1;
  };
  // Post-order traverse with leaf path generator lambda and no accumulation.
  this->traversal_engine(check_paths, path, POST_ORDER, false);
  // Reverse each path so they all start from root.
  for (size_t i = 0; i < ret.size(); ++i)
    ret[i].reverse();
  return ret;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::insert(const T& val)
{
  // Find the next level-order, left-to-right insertion slot.
  binary_tree<T, P>* prn = this->next();
  if (prn == nullptr) {
    return nullptr;
  }
  else if (prn->left() == nullptr) {
    prn->left() = binary_tree<T, P>::create(val, this->type());
    if (P)
      prn->left()->prnt() = prn;
    return prn->left();
  }
  else {
    prn->right() = binary_tree<T, P>::create(val, this->type());
    if (P)
      prn->right()->prnt() = prn;
    return prn->right();
  }
  return this;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::remove(const T& val)
{
  // Check if the root node is the target.
  if (this->val() == val) {
    // If tree is root-only, delete it and return nullptr.
    if (this->root_only()) {
      delete this;
      return nullptr;
    }
    // Otherwise, remove this node and return the new root.
    return this->remove_child(this);
  }
  // Find the node to be deleted and its parent.
  binary_tree<T, P>* parent = this->find_parent(val);
  if (parent == nullptr) {
    std::cout << "Warning: Removal target not in binary_tree." << std::endl;
    return this;
  }
  // Prioritize left child for removal if values are equal.
  binary_tree<T, P>* target = nullptr;
  if (parent->left()->val() == val)
    target = parent->left();
  else if (parent->right()->val() == val)
    target = parent->right();
  assert((target != nullptr) && "parent must have child with value val");
  // Delete the node if a matching value is found.
  parent->remove_child(target);
  return this;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::remove(const binary_tree<T, P>* target)
{
  if (target == nullptr)
    return this;
  // Check if the root node is the target.
  if (this == target) {
    // If tree is root-only, delete it and return nullptr;
    if (this->root_only()) {
      delete this;
      return nullptr;
    }
    // Otherwise, remove this node and return the new root.
    return this->remove_child(this);
  }
  // Find the node to be deleted and its parent.
  binary_tree<T, P>* parent = this->find_parent(target);
  if (parent == nullptr) {
    std::cout << "Warning: Removal target not in binary_tree" << std::endl;
    return this;
  }
  binary_tree<T, P>* child = nullptr;
  if (parent->left() == target)
    child = parent->left();
  else if (parent->right() == target)
    child = parent->right();
  assert((target != nullptr) && "parent must find target as a child");
  // Delete the node if a matching value is found.
  parent->remove_child(child);
  return this;
}

template <class T, bool P>
T binary_tree<T, P>::remove()
{
  // If the tree is root-only, there's no node that can be removed.
  if (this->root_only()) {
    std::cout << "Warning: Root-only tree, returning value without removing.";
    std::cout << std::endl;
    return this->val();
  }
  // Find the deepest rightmost leaf node and remove it from its parent.
  binary_tree<T, P>* parent;
  binary_tree<T, P>* child = this->last(parent);
  T ret = child->val();
  parent->remove_child(child);
  return ret;
}

template <class T, bool P>
bool binary_tree<T, P>::is_subtree(const binary_tree<T, P>* bt_ptr) const
{
  if (bt_ptr == nullptr)
    return false;
  return bt_ptr->has_subtree(this);
}

template <class T, bool P>
bool binary_tree<T, P>::has_subtree(const binary_tree<T, P>* bt_ptr) const
{
  if (bt_ptr == nullptr)
    return true;
  dyn_array<const binary_tree<T, P>*> cands = this->find(bt_ptr->val());
  for (size_t i = 0; i < cands.size(); ++i)
    if (*(cands[i]) == *bt_ptr)
      return true;
  return false;
}

template <class T, bool P>
binary_tree<T, P>::binary_tree(const binary_tree<T, P>& src)
{
  this->resize_adj(P+2);
  this->set_members(&src);
  // Heights come for free with recursive cloning.
  size_t depth_left = 0;
  size_t depth_right = 0;
  if (src.left()) {
    this->left() = src.left()->clone(src.type());
    if (P)
      this->left()->prnt() = this;
    depth_left = 1 + this->left()->height();
  }
  if (src.right()) {
    this->right() = src.right()->clone(src.type());
    if (P)
      this->right()->prnt() = this;
    depth_right = 1 + this->right()->height();
  }
  this->height() = (depth_left > depth_right) ? depth_left : depth_right;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::clone(BinaryTreeType type) const
{
  // Use a non-const path to store cloned nodes.
  bt_ptrs<T, P> path;
  // Lambda writes its cloned node pointer to path and returns its path index.
  // Computes and stores node heights, which comes for free with post-order.
  auto check_clone = [&path, type](
    const binary_tree<T, P>* bt, intptr_t retl, intptr_t retr) -> int {
    if (bt == nullptr) return 0;
    binary_tree<T, P>* bt_cl = binary_tree<T, P>::create(bt->val(), type);
    bt_cl->set_members(bt);
    bt_cl->left() = (retl >= 0) ? path[retl] : nullptr;
    bt_cl->right() = (retr >= 0) ? path[retr] : nullptr;
    if (P && bt_cl->left()) bt_cl->left()->prnt() = bt_cl;
    if (P && bt_cl->right()) bt_cl->right()->prnt() = bt_cl;
    int hl = bt_cl->left() ? static_cast<int>(bt_cl->left()->height()) : -1;
    int hr = bt_cl->right() ? static_cast<int>(bt_cl->right()->height()) : -1;
    int height = (hl > hr) ? hl+1 : hr+1;
    bt_cl->height() = static_cast<size_t>(height);
    path[path.size()-1] = bt_cl;
    return static_cast<int>(path.size() - 1);
  };
  // Post-order traverse with cloner lambda.
  int ind = this->traversal_engine(check_clone, path, POST_ORDER);;
  return (ind >= 0 && ind < path.size()) ? path[ind] : nullptr;
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::remove_child(binary_tree<T, P>* target)
{
  if (target == nullptr)
    return this;
  // This method should only be called from target or its parent.
  if (target != this && target != this->left() && target != this->right())
    return this;

  // If node has no children, nullify parent's pointer (if any) and delete.
  if (!target->left() && !target->right()) {
    if (this == target)
      return this;
    if (this->left() == target)
      this->left() = nullptr;
    else
      this->right() = nullptr;
    if (P)
      target->prnt() = nullptr;
    delete target;
    return this;
  }
  // Otherwise, prioritize left child for promotion.
  else {
    binary_tree<T, P> *new_root, *child, *other, *buf;
    child = target->left() ? target->left() : target->right();
    other = (child == target->left()) ? target->right() : nullptr;
    // If root is the target, save the chosen child to return at the end.
    if (target == this)
      new_root = (this->left()) ? this->left() : this->right();
    // Otherwise root will be unchanged.
    else
      new_root = this;
    // If a child is the target, connect this node to its chosen child.
    if (this->left() == target)
      this->left() = child;
    else if (this->right() == target)
      this->right() = child;
    // Handle potential parent pointers.
    if (P) {
      child->prnt() = target->prnt();
      target->prnt() = nullptr;
    }
    // Nullify target's child pointers and delete it.
    target->left() = nullptr;
    target->right() = nullptr;
    delete target;
    // Move left children up the chain until reaching child with 0 or 1 child.
    while (other) {
      // If child only has a right child, attach other to left and break.
      if (!child->left() && child->right()) {
        child->left() = child->right();
        child->right() = other;
        if (P)
          other->prnt() = child;
        other = nullptr;
      }
      // Otherwise replace right child with other, then continue.
      // This will naturally break if child has no right child.
      else {
        buf = child->right();
        child->right() = other;
        if (P)
          other->prnt() = child;
        other = buf;
        child = child->left();
      }
    }
    // Return the new root.
    return new_root;
  }
}

template <class T, bool P>
binary_tree<T, P>* binary_tree<T, P>::set_members(const binary_tree<T, P>* src)
{
  // Reset members for cleanup if nullptr is passed.
  this->set_val(src ? src->val() : T());
  this->height() = src ? src->height() : 0;
  // Use RTTI to determine if additional members need to be set.
  binary_search_tree<T, P>* bst_this;
  const binary_search_tree<T, P>* bst_src;
  if (bst_src = dynamic_cast<const binary_search_tree<T, P>*>(src))
    if (bst_this = dynamic_cast<binary_search_tree<T, P>*>(this))
      bst_this->set_count(bst_src->count());
  red_black_tree<T>* rb_this;
  const red_black_tree<T>* rb_src;
  if (rb_src = dynamic_cast<const red_black_tree<T>*>(src))
    if (rb_this = dynamic_cast<red_black_tree<T>*>(this))
      rb_this->set_color(rb_src->is_red());
  return this;
}

template <class T, bool P>
std::ostream& binary_tree<T, P>::print_node(std::ostream& os) const
{
  os << "(" << this->val() << ")";
  return os;
}

template <class T, bool P>
std::ostream& operator << (std::ostream& os, const binary_tree<T, P>* bt)
{
  if (bt == nullptr)
    return os;

  cbt_ptrs<T, P> path;
  stack<std::string> pst;
  pst.push("");
  stack<int> cst;
  cst.push(0);
  bool rt = true;

  auto check_os = [&pst, &cst, &os, &rt](
    const binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr || pst.is_empty() || cst.is_empty()) return -1;
    std::string pre = pst.pop();
    int dir = cst.pop();
    while (dir < 0) {os << pre << std::endl; pre = pst.pop(); dir = cst.pop();}
    bool is_left = dir == 1;
    if (!rt) {os << pre << (is_left ? " +--" : " >--");}
    bt->print_node(os) << std::endl;
    if (bt->left() == nullptr && bt->right() == nullptr) return 0;
    std::string child_pre = rt ? "" : (pre + (is_left ? " |  " : "    "));
    if (bt->right()) {pst.push(child_pre); cst.push(2);}
    else {pst.push(child_pre + " >--(**)"); cst.push(-2);}
    if (bt->left()) {pst.push(child_pre); cst.push(1);}
    else {pst.push(child_pre + " +--(**)"); cst.push(-1);}
    rt = false; return 1;
  };
  // Pre-order traverse with output stream lambda and no accumulation.
  bt->traversal_engine(check_os, path, PRE_ORDER, false);
  while (!pst.is_empty())
    os << pst.pop() << std::endl;
  return os;
}

template <class T, bool P>
std::ostream& operator << (std::ostream& os, const binary_tree<T, P>& bt)
{
  cbt_ptrs<T, P> path;
  stack<std::string> pst;
  pst.push("");
  stack<int> cst;
  cst.push(0);
  bool rt = true;

  auto check_os = [&pst, &cst, &os, &rt](
    const binary_tree<T, P>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr || pst.is_empty() || cst.is_empty()) return -1;
    std::string pre = pst.pop();
    int dir = cst.pop();
    while (dir < 0) {os << pre << "~"; pre = pst.pop(); dir = cst.pop();;}
    bool is_left = dir == 1;
    if (!rt) {os << pre << (is_left ? "+" : ">");}
    bt->print_node(os) << "~";
    if (bt->left() == nullptr && bt->right() == nullptr) return 0;
    std::string child_pre = rt ? "" : (pre + (is_left ? "|" : " "));
    if (bt->right()) {pst.push(child_pre); cst.push(2);}
    else {pst.push(child_pre + ">*~"); cst.push(-2);}
    if (bt->left()) {pst.push(child_pre); cst.push(1);}
    else {pst.push(child_pre + "+*~"); cst.push(-1);}
    rt = false; return 1;
  };
  // Pre-order traverse with compact output stream lambda and no accumulation.
  bt.traversal_engine(check_os, path, PRE_ORDER, false);
  while (!pst.is_empty())
    os << pst.pop();
  return os;
}

//======================================================================//
//                                                                      //
//                   binary_search_tree<T, P> methods                   //
//                                                                      //
//======================================================================//

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::clone() const
{
  binary_tree<T, P>* bt = this->binary_tree<T, P>::clone(this->type());
  return dynamic_cast<binary_search_tree<T, P>*>(bt);
}

template <class T, bool P>
const binary_search_tree<T, P>* binary_search_tree<T, P>::left_ch() const
{
  return static_cast<const binary_search_tree<T, P>*>(this->adj_[0]);
}

template <class T, bool P>
const binary_search_tree<T, P>* binary_search_tree<T, P>::right_ch() const
{
  return static_cast<const binary_search_tree<T, P>*>(this->adj_[1]);
}

template <class T, bool P>
const binary_search_tree<T, P>* binary_search_tree<T, P>::parent() const
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning nullptr." << std::endl;
    return nullptr;
  }
  return static_cast<const binary_search_tree<T, P>*>(this->adj_[2]);
}

template <class T, bool P>
size_t binary_search_tree<T, P>::size() const
{
  size_t ret = 0;
  cbt_ptrs<T, P> nodes = this->traverse(IN_ORDER);
  for (size_t i = 0; i < nodes.size(); ++i)
    ret+=dynamic_cast<const binary_search_tree<T, P>*>(nodes[i])->count();
  return ret;
}

template <class T, bool P>
bool binary_search_tree<T, P>::operator == (const binary_search_tree<T, P>& other) const
{
  bool ret = this->binary_tree<T, P>::operator==(other);
  return ret && (this->size() == other.size());
}

template <class T, bool P>
bool binary_search_tree<T, P>::operator != (const binary_search_tree<T, P>& other) const
{
  return !(*this == other);
}

template <class T, bool P>
binary_search_tree<T, P>& binary_search_tree<T, P>::operator = (
    std::initializer_list<T> list)
{
  if (list.size() == 0)
    return *this;
  this->clear();
  dyn_array<T> buf(list);
  *this = buf;
  return *this;
}

template <class T, bool P>
binary_search_tree<T, P>& binary_search_tree<T, P>::operator = (
    const dyn_array<T>& src)
{
  if (src.is_empty())
    return *this;
  this->clear();
  dyn_array<T> buf(src);
  buf.merge_sort();
  dyn_array<size_t> cnt = buf.unique_cnt();
  return *this->from_dyn_arr(buf, cnt, 0, buf.size()-1);
}

template <class T, bool P>
binary_search_tree<T, P>& binary_search_tree<T, P>::operator = (
    const binary_tree<T, P>& src)
{
  if (this == &src)
    return *this;
  this->clear();
  cbt_ptrs<T, P> nodes = src.traverse(IN_ORDER);
  dyn_array<T> buf(T(), nodes.size());
  for (size_t i = 0; i < nodes.size(); ++i)
    buf[i] = nodes[i]->val();
  *this = buf;
  return *this;
}

template <class T, bool P>
binary_search_tree<T, P>& binary_search_tree<T, P>::operator = (
    const binary_search_tree<T, P>& src)
{
  if (this == &src)
    return *this;
  // Binary tree operator duplicates structure, values, heights and counts.
  this->binary_tree<T, P>::operator=(src);
  return *this;
}

template <class T, bool P>
T binary_search_tree<T, P>::min() const
{
  const binary_search_tree<T, P>* min_node = this;
  while (min_node->left())
    min_node = min_node->left();
  return min_node->val();
}

template <class T, bool P>
T binary_search_tree<T, P>::max() const
{
  const binary_search_tree<T, P>* max_node = this;
  while (max_node->right())
    max_node = max_node->right();
  return max_node->val();
}

template <class T, bool P>
dyn_array<T> binary_search_tree<T, P>::all() const
{
  cbt_ptrs<T, P> path = this->traverse(IN_ORDER);
  // Allocate enough memory for the path, let duplicate auto-resize dyn_array.
  dyn_array<T> ret(nullptr, 0, path.size());
  const binary_search_tree<T, P>* bst_node;
  for (size_t i = 0; i < path.size(); ++i) {
    bst_node = dynamic_cast<const binary_search_tree<T, P>*>(path[i]);
    for (size_t dup = 0; dup < bst_node->count(); ++dup)
      ret.add(bst_node->val());
  }
  return ret;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::search(const T& val)
{
  if (this->val() == val)
    return this;
  binary_search_tree<T, P>* parent = this->find_parent(val);
  if (parent == nullptr)
    return nullptr;
  if (parent->left() && (parent->left()->val() == val))
    return parent->left();
  else if (parent->right() && (parent->right()->val() == val))
    return parent->right();
  else
    return nullptr;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::find_parent(const T& val)
{
  // Define a lambda to check for node values.
  auto check_val = [val](const binary_search_tree<T, P>* bt) -> int {
    if (bt->val() == val) {return 0;}
    else if (val < bt->val()) {return 1;}
    else {return 2;}
  };

  return this->search_engine(check_val);
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::find_parent(
    const binary_search_tree<T, P>* child)
{
  if (child == nullptr)
    return nullptr;

  // Define a lambda to check for node addresses.
  auto check_ptr = [child](const binary_search_tree<T, P>* bt) -> int {
    if (bt == child) {return 0;}
    else if (child->val() < bt->val()) {return 1;}
    else {return 2;}
  };

  return this->search_engine(check_ptr);
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::find_succ()
{
  if (this->right() == nullptr)
    return nullptr;
  // In-order successor is the leftmost leaf node in the right subtree.
  binary_search_tree<T, P>* succ = this->right();
  while (succ->left())
    succ = succ->left();
  return succ;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::find_succ(
    binary_search_tree<T, P>*& parent)
{
  if (this->right() == nullptr) {
    parent = nullptr;
    return nullptr;
  }
  // In-order successor is the leftmost leaf node in the right subtree.
  binary_search_tree<T, P>* succ = this->right();
  // Track the parent node as well during traversal.
  parent = this;
  while (succ->left()) {
    parent = succ;
    succ = succ->left();
  }
  return succ;
}

template <class T, bool P>
T binary_search_tree<T, P>::find_closest(const T& val) const
{
  const binary_search_tree<T, P>* bst_ptr = this;
  T closest = bst_ptr->val();
  T bst_val, abs_val, abs_cls;
  // Traverse the tree and keep track of the minimum difference.
  // T needs to have a signed distance metric defined with - operator overload.
  while (bst_ptr->left() || bst_ptr->right()) {
    bst_val = bst_ptr->val();
    abs_val = (val < bst_val) ? (bst_val - val) : (val - bst_val);
    abs_cls = (val < closest) ? (closest - val) : (val - closest);
    // If an exact match is found, break.
    if (val == bst_val) {
      closest = bst_val;
      break;
    }
    // Otherwise check for a closer value.
    else if (abs_val < abs_cls) {
      closest = bst_ptr->val();
    }

    if (val < bst_val) {
      if (bst_ptr->left())
        bst_ptr = bst_ptr->left();
      else
        break;
    }
    else {
      if (bst_ptr->right())
        bst_ptr = bst_ptr->right();
      else
        break;
    }
  }
  // Check for a closer value on the last encountered leaf node.
  bst_val = bst_ptr->val();
  abs_val = (val < bst_val) ? (bst_val - val) : (val - bst_val);
  abs_cls = (val < closest) ? (closest - val) : (val - closest);
  if (abs_val < abs_cls)
    closest = bst_val;
  return closest;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::insert(const T& val)
{
  // Find the leaf node to append a new child with the given value.
  binary_search_tree<T, P>* bst_ptr = this;
  while (bst_ptr->left() || bst_ptr->right()) {
    if (val == bst_ptr->val())
      return bst_ptr->inc_count();

    if (val < bst_ptr->val()) {
      if (bst_ptr->left())
        bst_ptr = bst_ptr->left();
      else
        break;
    }
    else {
      if (bst_ptr->right())
        bst_ptr = bst_ptr->right();
      else
        break;
    }
  }
  if (val == bst_ptr->val())
    return bst_ptr->inc_count();
  // Create the new node.
  binary_tree<T, P>* new_node;
  new_node = binary_tree<T, P>::create(val, this->type());
  // At this point, bst_ptr is the leaf node where val node is to be appended.
  if (val < bst_ptr->val()) {
    bst_ptr->left() = dynamic_cast<binary_search_tree<T, P>*>(new_node);
    if (P)
      bst_ptr->left()->prnt() = bst_ptr;
    return dynamic_cast<binary_search_tree<T, P>*>(new_node);
  }
  else {
    bst_ptr->right() = dynamic_cast<binary_search_tree<T, P>*>(new_node);
    if (P)
      bst_ptr->right()->prnt() = bst_ptr;
    return dynamic_cast<binary_search_tree<T, P>*>(new_node);
  }
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::remove(const T& val)
{
  // Define a lambda to check for node values. Nullptr input should return
  // a different value than pointer-searching lambda to allow removal_engine
  // to treat value removals and node removals differently.
  auto check_val = [val](const binary_search_tree<T, P>* bt) -> int {
    if (bt == nullptr) {return -2;}
    else if (bt->val() == val) {return 0;}
    else if (val < bt->val()) {return 1;}
    else {return 2;}
  };
  // Delete the node if a matching value is found.
  binary_search_tree<T, P>* ret = this->removal_engine(check_val);
  // If nullptr is returned, this must be root-only and also the target.
  if (ret == nullptr) {
    delete this;
    return nullptr;
  }
  return ret;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::remove(
    const binary_search_tree<T, P>* target)
{
  if (target == nullptr)
    return this;
  // Define a lambda to check for node addresses. Nullptr input should return
  // a different value than value-searching lambda to allow removal_engine to
  // treat value removals and node removals differently.
  auto check_ptr = [target](const binary_search_tree<T, P>* bt) -> int {
    if (bt == nullptr) {return -1;}
    else if (bt == target) {return 0;}
    else if (target->val() < bt->val()) {return 1;}
    else {return 2;}
  };
  // Delete the node if a matching node is found.
  binary_search_tree<T, P>* ret = this->removal_engine(check_ptr);
  // If nullptr is returned, this must be root-only and also the target.
  if (ret == nullptr) {
    delete this;
    return nullptr;
  }
  return ret;
}

template <class T, bool P>
T binary_search_tree<T, P>::remove()
{
  // If the tree is root-only, there's no node that can be removed.
  if (this->root_only()) {
    if (this->count() > 1)
      this->dec_count();
    else
      std::cout << "Warning: Root-only val, skipped removal." << std::endl;
    return this->val();
  }
  // Find the deepest rightmost leaf node.
  binary_tree<T, P>* buf;
  binary_search_tree<T, P> *child, *parent;
  child = dynamic_cast<binary_search_tree<T, P>*>(this->last(buf));
  parent = dynamic_cast<binary_search_tree<T, P>*>(buf);
  T ret = child->val();
  // If count is larger than 1, decrement; otherwise remove it from its parent.
  if (child->count() > 1)
    child->dec_count();
  else
    parent->remove_child(child);
  return ret;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::balance()
{
  // In-order traverse the tree to get a sorted dyn_array of pointers.
  bt_ptrs<T, P> pointers = this->traverse(IN_ORDER);
  // Convert to dyn_array of T values
  dyn_array<T> vals(T(), pointers.size());
  dyn_array<size_t> cnts(0, pointers.size());
  for (size_t i = 0; i < pointers.size(); ++i) {
    vals[i] = pointers[i]->val();
    cnts[i] = dynamic_cast<binary_search_tree<T, P>*>(pointers[i])->count();
  }
  // Delete the pointer array and the current tree.
  pointers.clear();
  this->clear();
  return this->from_dyn_arr(vals, cnts, 0, vals.size()-1);
}

template <class T, bool P>
binary_search_tree<T, P>*& binary_search_tree<T, P>::left()
{
  return reinterpret_cast<binary_search_tree<T, P>*&>(this->adj_[0]);
}

template <class T, bool P>
binary_search_tree<T, P>*& binary_search_tree<T, P>::right()
{
  return reinterpret_cast<binary_search_tree<T, P>*&>(this->adj_[1]);
}

template <class T, bool P>
binary_search_tree<T, P>*& binary_search_tree<T, P>::prnt()
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning pointer to left child." << std::endl;
    return reinterpret_cast<binary_search_tree<T, P>*&>(this->adj_[0]);
  }
  return reinterpret_cast<binary_search_tree<T, P>*&>(this->adj_[2]);
}

template <class T, bool P>
const binary_search_tree<T, P>* binary_search_tree<T, P>::left() const
{
  return static_cast<const binary_search_tree<T, P>*>(this->adj_[0]);
}

template <class T, bool P>
const binary_search_tree<T, P>* binary_search_tree<T, P>::right() const
{
  return static_cast<const binary_search_tree<T, P>*>(this->adj_[1]);
}

template <class T, bool P>
const binary_search_tree<T, P>* binary_search_tree<T, P>::prnt() const
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning nullptr." << std::endl;
    return static_cast<const binary_search_tree<T, P>*>(this->adj_[0]);
  }
  return static_cast<const binary_search_tree<T, P>*>(this->adj_[2]);
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::search_engine(
    std::function<int(const binary_search_tree<T, P>*)> check)
{
  binary_search_tree<T, P>* bst_ptr = this;
  binary_search_tree<T, P>* parent = nullptr;
  // Traverse until target is found, break if tree is exhausted.
  while (check(bst_ptr) > 0) {
    // Check for val < bst_ptr->val()
    if (check(bst_ptr) == 1) {
      if (bst_ptr->left() == nullptr)
        break;
      parent = bst_ptr;
      bst_ptr = bst_ptr->left();
    }
    // Check for val > bst_ptr->val()
    else {
      if (bst_ptr->right() == nullptr)
        break;
      parent = bst_ptr;
      bst_ptr = bst_ptr->right();
    }
  }
  // Check if node with a matching value is found, return nullptr if not.
  if (check(bst_ptr) > 0)
    return nullptr;
  return parent;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::removal_engine(
    std::function<int(const binary_search_tree<T, P>*)> check)
{
  // Check if the root node is the target.
  if (check(this) == 0) {
    // If target is not the node and root has duplicates, decrement and return.
    if ((check(nullptr) != -1) && (this->count() > 1))
      return this->dec_count();
    // If tree is root-only, defer to the calling function to clear it.
    if (this->root_only())
      return nullptr;
    // Otherwise, remove this node and return the new root.
    return this->remove_child(this);
  }

  // Find the node to be deleted and its parent.
  binary_search_tree<T, P>* parent = this->search_engine(check);
  if (parent == nullptr) {
    std::cout << "Warning: Removal target not in search tree." << std::endl;
    return this;
  }
  binary_search_tree<T, P>* target = nullptr;
  if (parent->left() && (check(parent->left()) == 0))
    target = parent->left();
  else if (parent->right() && (check(parent->right()) == 0))
    target = parent->right();
  assert((target != nullptr) && "parent must find target as a child");

  // If target is not the node and root has duplicates, decrement and return.
  if ((check(nullptr) != -1) && (target->count() > 1))
    target->dec_count();
  // Otherwise remove the node from its parent.
  else
    parent->remove_child(target);
  return this;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::from_dyn_arr(
    const dyn_array<T>& uniques, const dyn_array<size_t>& cn,
    size_t start, size_t end, bool first)
{
  if (start > end)
  return nullptr;
  // Start with the middle value.
  size_t mid = (start + end + 1) / 2;
  // Compute the counts of left and right branches.
  size_t left_cnt = mid - start;
  size_t right_cnt = end - mid;
  // Compute the left branch's deficiency from reaching next power of 2.
  size_t left_def = 0;
  if (((left_cnt + 1) & left_cnt) != 0)
    left_def = uniques.next_pow_2(left_cnt) - left_cnt - 1;
  // Compute the right branch's excess on top of previous power of 2.
  size_t right_exc = 0;
  if (((right_cnt + 1) & right_cnt) != 0)
    right_exc = right_cnt - (uniques.next_pow_2(right_cnt) >> 1) + 1;
  // Shift until either right excess is depleted or left deficiency is closed.
  mid += ((right_exc < left_def) ? right_exc : left_def);
  binary_search_tree<T, P>* cur;
  // If this is the first node to be added, set root's value.
  if (first) {
    this->set_val(uniques[mid]);
    cur = this;
  }
  // Otherwise, create a new node.
  else {
    binary_tree<T, P>* b = binary_tree<T, P>::create(uniques[mid], this->type());
    cur = dynamic_cast<binary_search_tree<T, P>*>(b);
  }
  cur->set_count(static_cast<int>(cn[mid]));
  // Recurse into left and right sides.
  if (start < mid)
    cur->left() = cur->from_dyn_arr(uniques, cn, start, mid - 1, false);
  if (end > mid)
    cur->right() = cur->from_dyn_arr(uniques, cn, mid + 1, end, false);
  // Attach current node as parent to its children if parent links are stored.
  if (P && cur->left())
    cur->left()->prnt() = cur;
  if (P && cur->right())
    cur->right()->prnt() = cur;
  return cur;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::remove_child(
    binary_search_tree<T, P>* target)
{
  if (target == nullptr)
    return this;
  // This method should only be called from target or its parent.
  if (target != this && target != this->left() && target != this->right())
    return this;

  // If node has no children, nullify parent's pointer (if any) and delete.
  if (!target->left() && !target->right()) {
    if (this == target)
      return this;
    if (this->left() == target)
      this->left() = nullptr;
    else
      this->right() = nullptr;
    delete target;
    return this;
  }
  // If node has only 1 child, connect its parent (if any) to its child.
  else if ((target->left() == nullptr) != (target->right() == nullptr)) {
    binary_search_tree<T, P> *new_root, *child;
    child = target->left() ? target->left() : target->right();
    // If root is the target, save the child to return at the end.
    if (target == this)
      new_root = (this->left()) ? this->left() : this->right();
    // Otherwise root will be unchanged.
    else
      new_root = this;
    // If a child is the target, connect this node to its child.
    if (this->left() == target)
      this->left() = child;
    else if (this->right() == target)
      this->right() = child;
    // Handle potential parent pointers.
    if (P) {
      child->prnt() = target->prnt();
      target->prnt() = nullptr;
    }
    // Nullify target's child pointers and delete it.
    target->left() = nullptr;
    target->right() = nullptr;
    delete target;
    // Return the new root.
    return new_root;
  }
  // If node has 2 children, replace it with its in-order successor.
  else {
    // Find node's in-order successor and successor's parent.
    binary_search_tree<T, P>* parent = target;
    binary_search_tree<T, P>* succ = target->find_succ(parent);
    assert((succ != nullptr) && "2-child tree node should have a successor");
    // Copy the in-order successor value and count to the target node.
    target->set_val(succ->val());
    target->set_count(succ->count());
    // Connect in-order successor's parent to its right child, if any.
    if (parent && parent->left() == succ)
      parent->left() = succ->right();
    else if (parent && parent->right() == succ)
      parent->right() = succ->right();
    // Handle potential parent pointers.
    if (P) {
      if (succ->right())
        succ->right()->prnt() = succ->prnt();
      succ->prnt() = nullptr;
    }
    // Nullify successor's child pointers to avoid recursive deletion.
    succ->left() = nullptr;
    succ->right() = nullptr;
    delete succ;
    return this;
  }
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::set_count(int count)
{
  this->count_ = count;
  return this;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::inc_count()
{
  this->set_count(this->count() + 1);
  return this;
}

template <class T, bool P>
binary_search_tree<T, P>* binary_search_tree<T, P>::dec_count()
{
  this->set_count(this->count() - 1);
  return this;
}

template <class T, bool P>
std::ostream& binary_search_tree<T, P>::print_node(std::ostream& os) const
{
  os << "(" << this->val();
  if (this->count() > 1)
    os << "x" << this->count();
  os << ")";
  return os;
}

//======================================================================//
//                                                                      //
//                      heap_tree<T, P, M> methods                      //
//                                                                      //
//======================================================================//

template <class T, bool P, bool M>
heap_tree<T, P, M>* heap_tree<T, P, M>::clone() const
{
  binary_tree<T, P>* bt = this->binary_tree<T, P>::clone(this->type());
  return dynamic_cast<heap_tree<T, P, M>*>(bt);
}

template <class T, bool P, bool M>
const heap_tree<T, P, M>* heap_tree<T, P, M>::left_ch() const
{
  return static_cast<const heap_tree<T, P, M>*>(this->adj_[0]);
}

template <class T, bool P, bool M>
const heap_tree<T, P, M>* heap_tree<T, P, M>::right_ch() const
{
  return static_cast<const heap_tree<T, P, M>*>(this->adj_[1]);
}

template <class T, bool P, bool M>
const heap_tree<T, P, M>* heap_tree<T, P, M>::parent() const
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning nullptr." << std::endl;
    return nullptr;
  }
  return static_cast<const heap_tree<T, P, M>*>(this->adj_[2]);
}

template <class T, bool P, bool M>
heap_tree<T, P, M>* heap_tree<T, P, M>::last(heap_tree<T, P, M>*& parent)
{
  // If the tree is root-only, return root.
  if (this->root_only()) {
    parent = nullptr;
    return this;
  }
  // Iterative right-biased descent.
  heap_tree<T, P, M>* cur = this;
  parent = nullptr;
  while (cur->left() || cur->right()) {
    parent = cur;
    if (cur->right())
      cur = cur->right();
    else
      cur = cur->left();
  }
  return cur;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>& heap_tree<T, P, M>::operator = (
    std::initializer_list<T> list)
{
  if (list.size() == 0)
    return *this;
  this->clear();
  dyn_array<T> buf(list);
  *this = buf;
  return *this;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>& heap_tree<T, P, M>::operator = (const dyn_array<T>& src)
{
  if (src.is_empty())
    return *this;
  this->clear();
  dyn_array<T> buf(src);
  // Assign to self using binary_tree operator, then heapify the complete tree.
  this->binary_tree<T, P>::operator=(buf);
  this->heapify();
  return *this;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>& heap_tree<T, P, M>::operator = (
    const binary_tree<T, P>& src)
{
  if (this == &src)
    return *this;
  this->clear();
  // If source is a complete binary tree, preserve as much of its structure as
  // possible by duplicating, then heapifying it. All 3 operations are O(N).
  if (src.is_complete()) {
    this->binary_tree<T, P>::operator=(src);
    this->heapify();
  }
  // It's impossible to heapify non-complete binary trees with value-swapping
  // sift-ups, so the tree is traversed and rebuilt from dyn_array, both O(N).
  else {
    cbt_ptrs<T, P> path = src.traverse(POST_ORDER);
    dyn_array<T> buf;
    for (size_t i = 0; i < path.size(); ++i)
      buf.add(path[i]->val());
    *this = buf;
  }
  return *this;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>& heap_tree<T, P, M>::operator = (
    const binary_search_tree<T, P>& src)
{
  *this = src.all();
  return *this;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>& heap_tree<T, P, M>::operator = (const heap<T>& hp)
{
  if ((M && !hp.is_max()) || (!M && hp.is_max())) {
    std::cerr << "ERROR! Inconsistent heap and heap_tree types." << std::endl;
    return *this;
  }
  // Internal array of heap<T> is in level-insertion order, so binary_tree's
  // assignment operator can be used directly for efficiency.
  this->binary_tree<T, P>::operator=(hp.arr_);
  return *this;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>* heap_tree<T, P, M>::insert(const T& val)
{
  // Completeness-compliant insertion at the bottom using parent's method.
  binary_tree<T, P>* new_node_bt = this->binary_tree<T, P>::insert(val);
  // Generate the shortest path from this node to the inserted node.
  gn_ptrs<T> path;
  // If parent links are not stored, only option is to search from top.
  if (!P) {
    path = this->depth_first_search(new_node_bt);
  }
  // If parent links are stored, simply backtrack and build path.
  else {
    path.add(dynamic_cast<graph_node<T>*>(new_node_bt));
    while (new_node_bt->prnt()) {
      new_node_bt = new_node_bt->prnt();
      path.add(dynamic_cast<graph_node<T>*>(new_node_bt));
    }
    path.reverse();
  }
  // Index to keep track of parent nodes using the path (can be negative).
  int p_i = static_cast<int>(path.size()) - 2;
  heap_tree<T, P, M>* parent = dynamic_cast<heap_tree<T, P, M>*>(path[p_i]);
  heap_tree<T, P, M>* new_nd = dynamic_cast<heap_tree<T, P, M>*>(path[p_i+1]);
  // Sift the inserted node up the path until a correct location is found.
  while (!parent->compare(new_nd) && (p_i >= 0)) {
    // Swap the values
    T buf = parent->val();
    parent->set_val(new_nd->val());
    new_nd->set_val(buf);
    // Update the parent pointer if root is not reached.
    if (--p_i >= 0)
      parent = dynamic_cast<heap_tree<T, P, M>*>(path[p_i]);
    // Update new node pointer to point at the node with the inserted value.
    new_nd = dynamic_cast<heap_tree<T, P, M>*>(path[p_i+1]);
  }
  return new_nd;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>* heap_tree<T, P, M>::remove(const T& val)
{
  // Check if the root node is the target.
  if (this->val() == val) {
    // If tree is root-only, delete it and return nullptr;
    if (this->root_only()) {
      delete this;
      return nullptr;
    }
    // Otherwise, call remove() to remove the root.
    this->remove();
    // Root address doesn't change.
    return this;
  }
  // If root is not the target, find the target.
  heap_tree<T, P, M>* target;
  target = dynamic_cast<heap_tree<T, P, M>*>(this->search(val));
  if (target == nullptr) {
    std::cout << "Warning: Removal target not in binary_tree" << std::endl;
    return this;
  }
  // Find the deepest rightmost node to delete.
  heap_tree<T, P, M>* parent;
  heap_tree<T, P, M>* child = this->last(parent);
  T repl = child->val();
  // If target will be deleted, it shouldn't be sifted or value-adjusted later.
  bool sift = (target == child) ? false : true;
  parent->remove_child(child);
  // Replace target's value with deletion node's value if they are different.
  // No part of the tree other than the replaced node needs to be re-heapified.
  if (sift) {
    target->set_val(repl);
    target->sift_down();
  }
  return this;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>* heap_tree<T, P, M>::remove(
    const heap_tree<T, P, M>* target)
{
  if (target == nullptr)
    return this;
  // Check if the root node is the target.
  if (this == target) {
    // If tree is root-only, delete it and return nullptr.
    if (this->root_only()) {
      delete this;
      return nullptr;
    }
    // Otherwise, call remove() to remove the root.
    this->remove();
    return this;
  }
  // If root is not the target, make sure target is in the tree.
  heap_tree<T, P, M>* parent;
  parent = dynamic_cast<heap_tree<T, P, M>*>(this->find_parent(target));
  if (parent == nullptr) {
    std::cout << "Warning: Removal target not in heap_tree." << std::endl;
    return this;
  }
  heap_tree<T, P, M>* target_node;
  if (parent->left() == target)
    target_node = parent->left();
  else if (parent->right() == target)
    target_node = parent->right();
  assert((target_node != nullptr) && "parent must find target as a child");
  // Find the deepest rightmost element to delete.
  heap_tree<T, P, M>* child = this->last(parent);
  // Replace target's value with deletion node's value if they are different.
  if (target_node != child)
    target_node->set_val(child->val());
  parent->remove_child(child);
  // No part of the tree other than the replaced node needs to be re-heapified.
  target_node->sift_down();
  return this;
}

template <class T, bool P, bool M>
T heap_tree<T, P, M>::remove()
{
  T ret = this->val();
  // If the tree is root-only, there's no node that can be removed.
  if (this->root_only()) {
    std::cout << "Warning: Root-only tree, returning value without removing.";
    std::cout << std::endl;
    return ret;
  }
  heap_tree<T, P, M>* parent;
  // Replace root node value with the value of deepest rightmost node.
  heap_tree<T, P, M>* child = this->last(parent);
  this->set_val(child->val());
  // Delete the deepest rightmost node.
  parent->remove_child(child);
  // Sift the new root node down the path until a correct location is found.
  this->sift_down();
  return ret;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>* heap_tree<T, P, M>::heapify()
{
  bt_ptrs<T, P> path;
  auto check_sift = [](
    binary_tree<T, P>* bt, intptr_t retl, intptr_t retr) -> int {
    heap_tree<T, P, M>* ht = dynamic_cast<heap_tree<T, P, M>*>(bt);
    if (ht == nullptr) return -1;
    ht->sift_down(); return 1;
  };
  // Post-order traverse with heapifier lambda and no accumulation.
  this->traversal_engine(check_sift, path, POST_ORDER, false);
  return this;
}

template <class T, bool P, bool M>
heap_tree<T, P, M>*& heap_tree<T, P, M>::left()
{
  return reinterpret_cast<heap_tree<T, P, M>*&>(this->adj_[0]);
}

template <class T, bool P, bool M>
heap_tree<T, P, M>*& heap_tree<T, P, M>::right()
{
  return reinterpret_cast<heap_tree<T, P, M>*&>(this->adj_[1]);
}

template <class T, bool P, bool M>
heap_tree<T, P, M>*& heap_tree<T, P, M>::prnt()
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning pointer to left child." << std::endl;
    return reinterpret_cast<heap_tree<T, P, M>*&>(this->adj_[0]);
  }
  return reinterpret_cast<heap_tree<T, P, M>*&>(this->adj_[2]);
}

template <class T, bool P, bool M>
const heap_tree<T, P, M>* heap_tree<T, P, M>::left() const
{
  return static_cast<const heap_tree<T, P, M>*>(this->adj_[0]);
}

template <class T, bool P, bool M>
const heap_tree<T, P, M>* heap_tree<T, P, M>::right() const
{
  return static_cast<const heap_tree<T, P, M>*>(this->adj_[1]);
}

template <class T, bool P, bool M>
const heap_tree<T, P, M>* heap_tree<T, P, M>::prnt() const
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning nullptr." << std::endl;
    return nullptr;
  }
  return static_cast<const heap_tree<T, P, M>*>(this->adj_[2]);
}

template <class T, bool P, bool M>
heap_tree<T, P, M>* heap_tree<T, P, M>::sift_down()
{
  heap_tree<T, P, M>* prn = this;
  heap_tree<T, P, M>* cur;
  while (prn->left() || prn->right()) {
    if (prn->left() && prn->right())
      cur = (prn->left()->compare(prn->right())) ? prn->left() : prn->right();
    else
      cur = (prn->left()) ? prn->left() : prn->right();
    // If correct location is reached, break.
    if (prn->compare(cur))
      break;
    // Swap the values
    T buf = prn->val();
    prn->set_val(cur->val());
    cur->set_val(buf);
    prn = cur;
  }
  return this;
}

template <class T, bool P, bool M>
bool heap_tree<T, P, M>::compare(heap_tree<T, P, M>* other) const
{
  return M ? this->val() >= other->val() : this->val() <= other->val();
}

//======================================================================//
//                                                                      //
//                        avl_tree<T, P> methods                        //
//                                                                      //
//======================================================================//

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::clone() const
{
  binary_tree<T, P>* bt = this->binary_search_tree<T, P>::clone();
  return dynamic_cast<avl_tree<T, P>*>(bt);
}

template <class T, bool P>
const avl_tree<T, P>* avl_tree<T, P>::left_ch() const
{
  return static_cast<const avl_tree<T, P>*>(this->adj_[0]);
}

template <class T, bool P>
const avl_tree<T, P>* avl_tree<T, P>::right_ch() const
{
  return static_cast<const avl_tree<T, P>*>(this->adj_[1]);
}

template <class T, bool P>
const avl_tree<T, P>* avl_tree<T, P>::parent() const
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning nullptr." << std::endl;
    return nullptr;
  }
  return static_cast<const avl_tree<T, P>*>(this->adj_[2]);
}

template <class T, bool P>
int avl_tree<T, P>::balance_factor() const
{
  int lh = this->left()  ? static_cast<int>(this->left()->height())  : -1;
  int rh = this->right() ? static_cast<int>(this->right()->height()) : -1;
  return lh - rh;
}

template <class T, bool P>
avl_tree<T, P>& avl_tree<T, P>::operator = (std::initializer_list<T> list)
{
  if (list.size() == 0)
    return *this;
  this->clear();
  dyn_array<T> buf(list);
  *this = buf;
  return *this;
}

template <class T, bool P>
avl_tree<T, P>& avl_tree<T, P>::operator = (const dyn_array<T>& src)
{
  if (src.is_empty())
    return *this;
  // Binary search tree operator produces a balanced and complete binary tree.
  this->binary_search_tree<T, P>::operator=(src);
  return *this;
}

template <class T, bool P>
avl_tree<T, P>& avl_tree<T, P>::operator = (const binary_tree<T, P>& src)
{
  if (this == &src)
    return *this;
  // Binary search tree operator produces a balanced and complete binary tree.
  this->binary_search_tree<T, P>::operator=(src);
  return *this;
}

template <class T, bool P>
avl_tree<T, P>& avl_tree<T, P>::operator = (
    const binary_search_tree<T, P>& src)
{
  if (this == &src)
    return *this;
  // Binary tree operator duplicates structure, values, heights and counts.
  this->binary_tree<T, P>::operator=(src);
  // Binary search tree may not be balanced, but AVL tree needs to be.
  this->balance();
  return *this;
}

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::insert(const T& val)
{
  // If val == avl_tree.val_, increment count and return without balancing.
  if (val == this->val())
    return dynamic_cast<avl_tree<T, P>*>(this->inc_count());
  // If val < avl_tree.val_ and node has no left(), create and link to left.
  else if (val < this->val() && this->left() == nullptr) {
    binary_tree<T, P>* bt;
    bt = binary_tree<T, P>::create(val, this->type());
    this->left() = dynamic_cast<avl_tree<T, P>*>(bt);
    if (P)
      this->left()->prnt() = this;
  }
  // If val > avl_tree.val_ and node has no right(), create and link to right.
  else if (val > this->val() && this->right() == nullptr) {
    binary_tree<T, P>* bt;
    bt = binary_tree<T, P>::create(val, this->type());
    this->right() = dynamic_cast<avl_tree<T, P>*>(bt);
    if (P)
      this->right()->prnt() = this;
  }
  // Otherwise recurse down the tree according to search tree properties.
  else {
    if (val < this->val())
      this->left() = this->left()->insert(val);
    else
      this->right() = this->right()->insert(val);
  }
  // Update node height, compute balance factor, then handle needed rotations.
  return this->balance_node();
}

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::remove(const T& val)
{
  // New root to return, in case it changes.
  avl_tree<T, P>* new_root = this;
  avl_tree<T, P>* child;
  // If query is smaller than node value, search for it on the left branch.
  if (val < new_root->val()) {
    // If no left branch exists, AVL tree does not contain the query.
    if (new_root->left() == nullptr) {
      std::cout << "Warning: Removal target not in avl_tree" << std::endl;
      return new_root;
    }
    // Otherwise, recurse into left branch.
    else {
      new_root->left() = new_root->left()->remove(val);
    }
  }
  // If query is larger than node value, search for it on the right branch.
  else if (val > new_root->val()) {
    // If no left branch exists, AVL tree does not contain the query.
    if (new_root->right() == nullptr) {
      std::cout << "Warning: Removal target not in avl_tree" << std::endl;
      return new_root;
    }
    // Otherwise, recurse into left branch.
    else {
      new_root->right() = new_root->right()->remove(val);
    }
  }
  // If query is equal to node value, then this node is the node to be deleted.
  else {
    // If count is larger than 1, decrement and return without balancing.
    if (new_root->count() > 1)
      return dynamic_cast<avl_tree<T, P>*>(new_root->dec_count());
    // If node has no children, delete and return nullptr.
    if (!new_root->left() && !new_root->right()) {
      delete new_root;
      return nullptr;
    }
    // If node has 1 child, unlink and delete node and make child the new root.
    else if ((new_root->left() == nullptr) != (new_root->right() == nullptr)) {
      child = new_root->left() ? new_root->left() : new_root->right();
      new_root->left() = nullptr;
      new_root->right() = nullptr;
      if (P)
        child->prnt() = nullptr;
      delete new_root;
      new_root = child;
    }
    // If node has 2 children, recurse into in-order successor.
    else {
      // Find the in-order successor.
      child = dynamic_cast<avl_tree<T, P>*>(new_root->find_succ());
      // Copy the in-order successor's value and count to the new root.
      new_root->set_val(child->val());
      new_root->set_count(child->count());
      // Recurse into right branch to delete the in-order successor node.
      new_root->right() = new_root->right()->remove(child);
    }
  }
  return new_root->balance_node();
}

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::remove(const avl_tree<T, P>* target)
{
  // New root to return, in case it changes.
  avl_tree<T, P>* new_root = this;
  avl_tree<T, P>* child;
  // If target is not this node, determine recursion direction.
  if (target != new_root) {
    // If target value is smaller than/equal to node value, search left branch.
    if (target->val() < new_root->val()) {
      // If no left branch exists, AVL tree does not contain the query.
      if (new_root->left() == nullptr) {
        std::cout << "Warning: Removal target not in avl_tree" << std::endl;
        return new_root;
      }
      // Otherwise, recurse into left branch.
      else {
        new_root->left() = new_root->left()->remove(target);
      }
    }
    // If target value is larger than node value, search right branch.
    else {
      // If no left branch exists, AVL tree does not contain the query.
      if (new_root->right() == nullptr) {
        std::cout << "Warning: Removal target not in avl_tree" << std::endl;
        return new_root;
      }
      // Otherwise, recurse into left branch.
      else {
        new_root->right() = new_root->right()->remove(target);
      }
    }
  }
  // If target is this node, check its children to determine how to delete.
  else {
    // If node has no children, delete and return nullptr.
    if (!new_root->left() && !new_root->right()) {
      delete new_root;
      return nullptr;
    }
    // If node has 1 child, unlink and delete node and make child the new root.
    else if ((new_root->left() == nullptr) != (new_root->right() == nullptr)) {
      child = new_root->left() ? new_root->left() : new_root->right();
      new_root->left() = nullptr;
      new_root->right() = nullptr;
      if (P)
        child->prnt() = nullptr;
      delete new_root;
      new_root = child;
    }
    // If node has 2 children, recurse into in-order successor.
    else {
      // Find the in-order successor.
      child = dynamic_cast<avl_tree<T, P>*>(new_root->find_succ());
      // Copy the in-order successor's value and count to the new root.
      new_root->set_val(child->val());
      new_root->set_count(child->count());
      // Recurse into right branch to delete the in-order successor node.
      new_root->right() = new_root->right()->remove(child);
    }
  }
  return new_root->balance_node();
}

template <class T, bool P>
avl_tree<T, P>*& avl_tree<T, P>::left()
{
  return reinterpret_cast<avl_tree<T, P>*&>(this->adj_[0]);
}

template <class T, bool P>
avl_tree<T, P>*& avl_tree<T, P>::right()
{
  return reinterpret_cast<avl_tree<T, P>*&>(this->adj_[1]);
}

template <class T, bool P>
avl_tree<T, P>*& avl_tree<T, P>::prnt()
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning pointer to left child." << std::endl;
    return reinterpret_cast<avl_tree<T, P>*&>(this->adj_[0]);;
  }
  return reinterpret_cast<avl_tree<T, P>*&>(this->adj_[2]);
}

template <class T, bool P>
const avl_tree<T, P>* avl_tree<T, P>::left() const
{
  return static_cast<const avl_tree<T, P>*>(this->adj_[0]);
}

template <class T, bool P>
const avl_tree<T, P>* avl_tree<T, P>::right() const
{
  return static_cast<const avl_tree<T, P>*>(this->adj_[1]);
}

template <class T, bool P>
const avl_tree<T, P>* avl_tree<T, P>::prnt() const
{
  if (!P) {
    std::cerr << "ERROR! Child-only trees (P=false) don't store parent links.";
    std::cerr << std::endl << "Returning nullptr." << std::endl;
    return nullptr;
  }
  return static_cast<const avl_tree<T, P>*>(this->adj_[2]);
}

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::from_dyn_arr(
    const dyn_array<T>& uniques, const dyn_array<size_t>& cn,
    size_t start, size_t end, bool first)
{
  binary_search_tree<T, P>* bst;
  bst = this->binary_search_tree<T, P>::from_dyn_arr(uniques, cn,
                                                     start, end, first);
  // Valid heights need to be maintained at all times.
  if (first)
    bst->compute_heights();
  return dynamic_cast<avl_tree<T, P>*>(bst);
}

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::balance_node()
{
  // Update node height, compute balance factor, then handle needed rotations.
  this->update_node_height();
  int bal_fct = this->balance_factor();
  // Case: left left
  if (bal_fct > 1 && this->left()->balance_factor() >= 0)
    return this->rotate_right();
  // Case: left right
  if (bal_fct > 1 && this->left()->balance_factor() < 0) {
    this->left() = this->left()->rotate_left();
    return this->rotate_right();
  }
  // Case: right right
  if (bal_fct < -1 && this->right()->balance_factor() <= 0)
    return this->rotate_left();
  // Case: right left
  if (bal_fct < -1 && this->right()->balance_factor() > 0) {
    this->right() = this->right()->rotate_right();
    return this->rotate_left();
  }
  return this;
}

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::rotate_left()
{
  avl_tree<T, P>* x = this;
  avl_tree<T, P>* prn = nullptr;
  if (P)
    prn = x->prnt();
  avl_tree<T, P>* y = x->right();
  avl_tree<T, P>* t2 = y->left();
  // Perform the core rotation.
  y->left() = x;
  x->right() = t2;
  // Update heights.
  x->update_node_height();
  y->update_node_height();
  // If parent exists, link the correct child pointer to y.
  if (prn && prn->left() == x)
    prn->left() = y;
  else if ((prn && prn->right() == x))
    prn->right() = y;
  // If AVL tree is parent-linked, update parent links.
  if (P) {
    y->prnt() = prn;
    x->prnt() = y;
    if (x->right())
      x->right()->prnt() = x;
  }
  // Return new root.
  return y;
}

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::rotate_right()
{
  avl_tree<T, P>* x = this;
  avl_tree<T, P>* prn = nullptr;
  if (P)
    prn = x->prnt();
  avl_tree<T, P>* y = x->left();
  avl_tree<T, P>* t2 = y->right();
  // Perform the core rotation.
  y->right() = x;
  x->left() = t2;
  // Update heights.
  x->update_node_height();
  y->update_node_height();
  // If parent exists, link the correct child pointer to y.
  if (prn && prn->left() == x)
    prn->left() = y;
  else if ((prn && prn->right() == x))
    prn->right() = y;
  // If AVL tree is parent-linked, update parent links.
  if (P) {
    y->prnt() = prn;
    x->prnt() = y;
    if (x->left())
      x->left()->prnt() = x;
  }
  // Return new root.
  return y;
}

template <class T, bool P>
avl_tree<T, P>* avl_tree<T, P>::update_node_height()
{
  int lh = this->left()  ? static_cast<int>(this->left()->height())  : -1;
  int rh = this->right() ? static_cast<int>(this->right()->height()) : -1;
  this->height() = static_cast<size_t>(1 + ((lh > rh) ? lh : rh));
  return this;
}

//======================================================================//
//                                                                      //
//                      red_black_tree<T> methods                       //
//                                                                      //
//======================================================================//

template <class T>
red_black_tree<T>* red_black_tree<T>::clone() const
{
  binary_tree<T, true>* bt = this->binary_search_tree<T, true>::clone();
  return dynamic_cast<red_black_tree<T>*>(bt);
}

template <class T>
const red_black_tree<T>* red_black_tree<T>::left_ch() const
{
  return static_cast<const red_black_tree<T>*>(this->adj_[0]);
}

template <class T>
const red_black_tree<T>* red_black_tree<T>::right_ch() const
{
  return static_cast<const red_black_tree<T>*>(this->adj_[1]);
}

template <class T>
const red_black_tree<T>* red_black_tree<T>::parent() const
{
  return static_cast<const red_black_tree<T>*>(this->adj_[2]);
}

template <class T>
bool red_black_tree<T>::is_valid() const
{
  return !(this->is_red() || this->red_violation() || this->black_violation());
}

template <class T>
int red_black_tree<T>::count() const
{
  return (this->count_ < 0) ? -this->count_ : this->count_;
}

template <class T>
red_black_tree<T>& red_black_tree<T>::operator = (
    std::initializer_list<T> list)
{
  if (list.size() == 0)
    return *this;
  dyn_array<T> buf(list);
  *this = buf;
  return *this;
}

template <class T>
red_black_tree<T>& red_black_tree<T>::operator = (const dyn_array<T>& src)
{
  if (src.is_empty())
    return *this;
  // Binary search tree operator produces a balanced and complete binary tree.
  // Colorization is added by from_dyn_arr override of this class.
  this->binary_search_tree<T, true>::operator=(src);
  return *this;
}

template <class T>
red_black_tree<T>& red_black_tree<T>::operator = (
    const binary_tree<T, true>& src)
{
  if (this == &src)
    return *this;
  // Binary search tree operator produces a balanced and complete binary tree.
  // Colorization is added by from_dyn_arr override of this class.
  this->binary_search_tree<T, true>::operator=(src);
  return *this;
}

template <class T>
red_black_tree<T>& red_black_tree<T>::operator = (
    const binary_search_tree<T, true>& src)
{
  if (this == &src)
    return *this;
  // AVL tree operator duplicates structure, values, heights and counts.
  // It also balances using binary_search_tree's balance() method.
  this->avl_tree<T, true>::operator=(src);
  // Binary search tree will be balanced with heights saved but not colors.
  this->colorize();
  return *this;
}

template <class T>
red_black_tree<T>& red_black_tree<T>::operator = (const avl_tree<T, true>& src)
{
  if (this == &src)
    return *this;
  // Binary tree operator duplicates structure, values, heights and counts.
  this->binary_tree<T, true>::operator=(src);
  // AVL trees will be balanced at all times, but won't have color.
  this->colorize();
  return *this;
}

template <class T>
red_black_tree<T>* red_black_tree<T>::find_root()
{
  binary_tree<T, true>* root = this->binary_tree<T, true>::find_root();
  return dynamic_cast<red_black_tree<T>*>(root);
}

template <class T>
const red_black_tree<T>* red_black_tree<T>::find_root() const
{
  const binary_tree<T, true>* root = this->binary_tree<T, true>::find_root();
  return dynamic_cast<const red_black_tree<T>*>(root);
}

template <class T>
red_black_tree<T>* red_black_tree<T>::insert(const T& val)
{
  // Perform binary search tree insertion.
  binary_search_tree<T, true>* bst;
  bst = this->binary_search_tree<T, true>::insert(val);
  red_black_tree<T>* new_node = dynamic_cast<red_black_tree<T>*>(bst);
  assert((new_node != nullptr) && "bst insert should return inserted node.");
  // If no new node was added, return.
  if (new_node->count() > 1)
    return this;
  // Enforce red on new node.
  new_node->set_color(true);
  // Fix red-red violations.
  return this->fix_red(new_node);
}

template <class T>
red_black_tree<T>* red_black_tree<T>::remove(const T& val)
{
  // Define lambda for value checks using binary_search_tree removal engine.
  auto check_val = [val](const binary_search_tree<T, true>* bt) -> int {
    if (bt == nullptr) {return -2;}
    else if (bt->val() == val) {return 0;}
    else if (val < bt->val()) {return 1;}
    else {return 2;}
  };
  // Due to polymorphism, binary_search_tree's removal_engine will call
  // red_black_tree's override of the protected remove_child method.
  binary_search_tree<T, true>* bst;
  bst = this->removal_engine(check_val);
  red_black_tree<T>* ret = dynamic_cast<red_black_tree<T>*>(bst);
  // Engine returns nullptr if this is a root only node with matching value.
  // Top-level removal method needs to delete the last remaining node.
  if (ret == nullptr) {
    delete this;
    return nullptr;
  }
  // Find root, set its color to black and return.
  return ret->find_root()->set_color(false);
}

template <class T>
red_black_tree<T>* red_black_tree<T>::remove(const red_black_tree<T>* target)
{
  if (target == nullptr)
    return this;
  // Define lambda for pointer checks using binary_search_tree removal engine.
  auto check_ptr = [target](const binary_search_tree<T, true>* bt) -> int {
    if (bt == nullptr) {return -1;}
    else if (bt == target) {return 0;}
    else if (target->val() < bt->val()) {return 1;}
    else {return 2;}
  };
  // Due to polymorphism, binary_search_tree's removal_engine will call
  // red_black_tree's override of the protected remove_child method.
  binary_search_tree<T, true>* bst;
  bst = this->removal_engine(check_ptr);
  red_black_tree<T>* ret = dynamic_cast<red_black_tree<T>*>(bst);
  // Engine returns nullptr if this is a root only node with matching value.
  // Top-level removal method needs to delete the last remaining node.
  if (ret == nullptr) {
    delete this;
    return nullptr;
  }
  // Find root, set its color to black and return.
  return ret->find_root()->set_color(false);
}

template <class T>
red_black_tree<T>* red_black_tree<T>::fix_red(red_black_tree<T>* node)
{
  red_black_tree<T> *parent, *grandma, *uncle;
  while (node->is_red() && node->prnt() && node->prnt()->is_red()) {
    parent = node->prnt();
    grandma = parent->prnt();
    if (!grandma)
      break;
    uncle = (parent == grandma->left()) ? grandma->right() : grandma->left();
    bool parent_is_left = (parent == grandma->left());
    // Cases 1-4: If U is red, recolor and and set N = G for next iteration.
    if (uncle && uncle->is_red()) {
      parent->set_color(false);
      uncle->set_color(false);
      grandma->set_color(true);
      node = grandma;
    }
    // Cases 5-8: If U is black/null, perform rotation(s) and return new root.
    else {
      if (parent_is_left) {
        // Case 5: N is right, P is left: P.rotate_left, G and N swap colors.
        if (node == parent->right()) {
          node = parent->rotate_left();
          node->set_color(!node->is_red());
        }
        // Case 6: No pre-rotations, G and P swap colors.
        else {
          parent->set_color(!parent->is_red());
        }
        grandma->set_color(!grandma->is_red());
        // Cases 5-6: N is/has become left, P is left: G.rotate_right, resolve.
        return grandma->rotate_right()->find_root()->set_color(false);
      }
      else {
        // Case 7: N is left, P is right: P.rotate_right, G and N swap colors.
        if (node == parent->left()) {
          node = parent->rotate_right();
          node->set_color(!node->is_red());
        }
        // Case 8: No pre-rotations, G and P swap colors.
        else {
          parent->set_color(!parent->is_red());
        }
        grandma->set_color(!grandma->is_red());
        // Case 7-8: N is/has become right, P is right: G.rotate_left, resolve.
        return grandma->rotate_left()->find_root()->set_color(false);
      }
    }
  }
  // Find the new root in case it changed, set its color black and return.
  return node ? node->find_root()->set_color(false) : nullptr;
}

template <class T>
red_black_tree<T>* red_black_tree<T>::fix_black(red_black_tree<T>* node,
                                                red_black_tree<T>* parent)
{
  bool is_left, sib_left, sib_right;
  red_black_tree<T> *sibling, *grandma, *new_root;
  // Outer loop for propagating the double-black violation until it's resolved.
  while (parent && (!node || !node->is_red())) {
    is_left = (node == parent->left() || (!node && !parent->left()));
    sibling = is_left ? parent->right() : parent->left();
    grandma = parent->prnt();
    // Case 1: Red sibling: Set S black and P red. Rotate P.
    if (sibling->is_red()) {
      sibling->set_color(false);
      parent->set_color(true);
      // Case 1a: Red sibling, node is left child: P.rotate_left.
      // Case 1b: Red sibling, node is right child: P.rotate_right.
      (is_left) ? parent->rotate_left() : parent->rotate_right();
    }
    // Cases 2-4: Black or null sibling.
    else {
      // Case 2: Black/null sibling, both nieces black/null.
      //         Set S red. Resolve if P is red, propagate up if not.
      sib_left  = (!sibling->left()  || !sibling->left()->is_red());
      sib_right = (!sibling->right() || !sibling->right()->is_red());
      if (!sibling || (sib_left && sib_right)) {
        if (sibling)
          sibling->set_color(true);
        if (parent->is_red()) {
          parent->set_color(false);
          break;
        }
        else {
          node = parent;
          parent = node->prnt();
        }
      }
      // Cases 3-4: Black/null sibling, at least one red niece.
      else {
        // Case 3: Black/null sibling, at least one red niece, N is left.
        if (is_left) {
          // Case 3a: Black/null sibling, right/both niece(s) red, N is left.
          //          Set S P.color, P and NR black. P.rotate_left, resolve.
          if (sibling && sibling->right() && sibling->right()->is_red()) {
            sibling->set_color(parent->is_red());
            parent->set_color(false);
            sibling->right()->set_color(false);
            parent->rotate_left();
            break;
          }
          // Case 3b: Black/null sibling, only left niece red, N is left.
          //          Set NL black, set S red, S.rotate_right.
          else {
            sibling->left()->set_color(false);
            sibling->set_color(true);
            parent->right() = sibling->rotate_right();
          }
        }
        // Case 4: Black/null sibling, at least one red niece, N is right.
        else {
          // Case 4a: Black/null sibling, left/both niece(s) red, N is right.
          //          Set S P.color, P and NL black. P.rotate_right, resolve.
          if (sibling && sibling->left() && sibling->left()->is_red()) {
            sibling->set_color(parent->is_red());
            parent->set_color(false);
            sibling->left()->set_color(false);
            parent->rotate_right();
            break;
          }
          // Case 4b: Black/null sibling, only right niece red, N is right.
          //          Set NR black, set S red, S.rotate_left.
          else {
            sibling->right()->set_color(false);
            sibling->set_color(true);
            parent->left() = sibling->rotate_left();
          }
        }
      }
    }
  }
  // Find the new root in case it changed, set its color black and return.
  new_root = node ? node : parent;
  return new_root ? new_root->find_root()->set_color(false) : nullptr;
}

template <class T>
red_black_tree<T>* red_black_tree<T>::colorize()
{
  // For red-black trees, balance() also computes heights and colorizes.
  if (!this->is_balanced())
    return dynamic_cast<red_black_tree<T>*>(this->balance());
  size_t black_budget = (this->height() + 2) / 2;
  bt_ptrs<T, true> path;
  stack<size_t> bud_st;
  bud_st.push(black_budget);
  bool root = true;
  // Temporarily populate height fields (ind_) with min heights.
  this->compute_min_heights();
  // Lambda never returns 1 and executes pre-order colorization steps.
  auto check_bud = [&bud_st, &root](
    binary_tree<T, 1>* bt, intptr_t, intptr_t) -> int {
    if (bt == nullptr || bud_st.is_empty()) return -1;
    red_black_tree<T>* rb = dynamic_cast<red_black_tree<T>*>(bt);
    // Force black for root or after red parent
    bool force_black = root || (rb->prnt() && rb->prnt()->is_red());
    rb->set_color(true); root = false;
    size_t budget = bud_st.pop();
    if (force_black || budget > rb->height()) rb->set_color(false);
    budget -= (rb->is_red() ? 0 : 1);
    if (rb->right()) bud_st.push(budget);
    if (rb->left()) bud_st.push(budget);
    return 1;
  };
  // Pre-order traverse with black budget lambda and no accumulation.
  this->traversal_engine(check_bud, path, PRE_ORDER, false);
  // Compute the standard max heights.
  this->compute_heights();
  return this;
}

template <class T>
bool red_black_tree<T>::red_violation() const
{
  cbt_ptrs<T, true> path;
  // Lambda returns -1 if bt (a red-black tree) has a red-red violation.
  auto check_red = [](const binary_tree<T, 1>* bt, intptr_t, intptr_t) -> int {
    const red_black_tree<T>* rb = dynamic_cast<const red_black_tree<T>*>(bt);
    if (rb == nullptr) return -1;
    if (rb->is_red() && rb->left() && rb->left()->is_red()) return -1;
    if (rb->is_red() && rb->right() && rb->right()->is_red()) return -1;
    return 1;
  };
  // Pre-order traverse with red-red violation lambda and no accumulation.
  return this->traversal_engine(check_red, path, PRE_ORDER, false) == -1;
}

template <class T>
bool red_black_tree<T>::black_violation() const
{
  const red_black_tree<T>* root = this->find_root();
  return root->black_height() == -1;
}

template <class T>
red_black_tree<T>*& red_black_tree<T>::left()
{
  return reinterpret_cast<red_black_tree<T>*&>(this->adj_[0]);
}

template <class T>
red_black_tree<T>*& red_black_tree<T>::right()
{
  return reinterpret_cast<red_black_tree<T>*&>(this->adj_[1]);
}

template <class T>
red_black_tree<T>*& red_black_tree<T>::prnt()
{
  return reinterpret_cast<red_black_tree<T>*&>(this->adj_[2]);
}

template <class T>
const red_black_tree<T>* red_black_tree<T>::left() const
{
  return static_cast<const red_black_tree<T>*>(this->adj_[0]);
}

template <class T>
const red_black_tree<T>* red_black_tree<T>::right() const
{
  return static_cast<const red_black_tree<T>*>(this->adj_[1]);
}

template <class T>
const red_black_tree<T>* red_black_tree<T>::prnt() const
{
  return static_cast<const red_black_tree<T>*>(this->adj_[2]);
}

template <class T>
red_black_tree<T>* red_black_tree<T>::from_dyn_arr(
    const dyn_array<T>& uniques, const dyn_array<size_t>& cn,
    size_t start, size_t end, bool first)
{
  avl_tree<T, true>* avl;
  avl = this->avl_tree<T, true>::from_dyn_arr(uniques, cn, start, end, first);
  red_black_tree<T>* rb = dynamic_cast<red_black_tree<T>*>(avl);
  if (first)
    rb->colorize();
  return rb;
}

template <class T>
red_black_tree<T>* red_black_tree<T>::remove_child(
    binary_search_tree<T, true>* target)
{
  if (target == nullptr)
    return this;
  // This method should only be called for local removals.
  if (target != this && target != this->left() && target != this->right())
    return this;

  red_black_tree<T>* t = dynamic_cast<red_black_tree<T>*>(target);
  red_black_tree<T>* parent = t->prnt();
  red_black_tree<T>* child = nullptr;
  red_black_tree<T>* new_root = this;
  bool node_was_red = t->is_red();
  bool other_was_red;

  // Cases A-B: Leaf node (A: black, B: red)
  //   - Nullify node's links through binary_search_tree's remove_child.
  //   - Fix for double-black violations node was black and wasn't root.
  if (!t->left() && !t->right()) {
    other_was_red = parent ? false : true;
  }
  // Cases C-D: 1 child (C: both node and child black, D: otherwise)
  //   - Nullify and replace node through binary_search_tree's remove_child.
  //   - Fix double-black violations if both the node and its child was black.
  //     Note that Case C is not possible with only legal insertions.
  else if (!t->left() || !t->right()) {
    child = t->left() ? t->left() : t->right();
    // Save child color for later double-black check, then set it black.
    other_was_red = child->is_red();
    child->set_color(false);
  }
  // Cases E-F: 2 children (E: black successor with black child, F: otherwise).
  //   - Write successor value to node.
  //   - Nullify the successor while replacing it with its right child, if any.
  //   - Fix for black if successor was black with a black/null right child.
  else {
    // Find successor.
    red_black_tree<T>* succ = dynamic_cast<red_black_tree<T>*>(t->find_succ());
    assert((succ != nullptr) && "2-child tree node should have a successor");
    // Update parent pointer to successor's parent, if any.
    parent = succ->prnt();
    // Successor can only have a right child. If it does, set child pointer.
    child = succ->right();
    // Save child color for later double-black check, then set it black.
    other_was_red = child ? child->is_red() : false;
    (child) ? child->set_color(false) : child;
    // Update node value with successor's value.
    t->set_val(succ->val());
    t->set_count(succ->count());
    // Successor is now the node to be deleted.
    node_was_red = succ->is_red();
    t = succ;
  }

  binary_search_tree<T, true>* bst;
  // Delete the node and update new_root if necessary.
  if (parent) {
    parent->binary_search_tree<T, true>::remove_child(t);
  }
  else {
    bst = t->binary_search_tree<T, true>::remove_child(t);
    new_root = dynamic_cast<red_black_tree<T>*>(bst);
  }

  // If there is a double-black violation, fix it.
  if (!node_was_red && !other_was_red)
    new_root = new_root->find_root()->fix_black(child, parent);
  return new_root;
}

template <class T>
red_black_tree<T>* red_black_tree<T>::rotate_left()
{
  red_black_tree<T>* x = this;
  return dynamic_cast<red_black_tree<T>*>(x->avl_tree<T, 1>::rotate_left());
}

template <class T>
red_black_tree<T>* red_black_tree<T>::rotate_right()
{
  red_black_tree<T>* x = this;
  return dynamic_cast<red_black_tree<T>*>(x->avl_tree<T, 1>::rotate_right());
}

template <class T>
size_t red_black_tree<T>::compute_min_heights()
{
  bt_ptrs<T, 1> path;
  auto check_minh = [](
    binary_tree<T, 1>* bt, intptr_t retl, intptr_t retr) -> int {
    if (bt == nullptr) return -1;
    bt->height() = ((retl < retr) ? (retl + 1) : (retr + 1));
    return static_cast<int>(bt->height());
  };
  // Post-order traverse with min height computation lambda and no accumulation.
  int height = this->traversal_engine(check_minh, path, POST_ORDER, false);
  return static_cast<size_t>(height);
}

template <class T>
int red_black_tree<T>::black_height() const
{
  cbt_ptrs<T, 1> path;
  auto check_black = [](
    const binary_tree<T, 1>* bt, intptr_t retl, intptr_t retr) -> int {
    if (retl < 0) retl = 0;
    if (retr < 0) retr = 0;
    if (retl != retr) return -1;
    const red_black_tree<T>* rb = dynamic_cast<const red_black_tree<T>*>(bt);
    if (rb == nullptr) return -1;
    return (rb->is_red() ? 0 : 1) + static_cast<int>(retl);
  };
  // Post-order traverse with the black height computation lambda.
  return this->traversal_engine(check_black, path, POST_ORDER, false);
}

template <class T>
red_black_tree<T>* red_black_tree<T>::set_color(bool color)
{
  // Flip the sign of count if node color is being flipped.
  if ((color && !this->is_red()) || (!color && this->is_red()))
    this->count_ = -this->count_;
  return this;
}

template <class T>
red_black_tree<T>* red_black_tree<T>::set_count(int count)
{
  this->count_ = this->is_red() ? -count : count;
  return this;
}

template <class T>
std::ostream& red_black_tree<T>::print_node(std::ostream& os) const
{
  if (this->is_red())
    os << "<" << this->val();
  else
    os << "(" << this->val();
  if (this->count() > 1)
    os << "x" << this->count();
  if (this->is_red())
    os << ">";
  else
    os << ")";
  return os;
}