// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "graph.h"
#include "queue.h"
#include "hash_table.h"
#include "heap.h"

//======================================================================//
//                                                                      //
//                        graph_node<T> methods                         //
//                                                                      //
//======================================================================//

template <class T>
graph_node<T>::graph_node()
    : val_(T()), adj_(gn_ptrs<T>(nullptr, 2)), ind_(0)
{
}

template <class T>
graph_node<T>::graph_node(size_t n)
    : val_(T()), adj_(n > 0 ? gn_ptrs<T>(nullptr, n) : gn_ptrs<T>()), ind_(0)
{
}

template <class T>
graph_node<T>::graph_node(const T& v, size_t n)
    : val_(v), adj_(n > 0 ? gn_ptrs<T>(nullptr, n) : gn_ptrs<T>()), ind_(0)
{
}

// Protected constructor for friends and descendants
template <class T>
graph_node<T>::graph_node(const T& v, size_t n, size_t i)
    : val_(v), adj_(n > 0 ? gn_ptrs<T>(nullptr, n) : gn_ptrs<T>()), ind_(i)
{
}

template <class T>
size_t graph_node<T>::degree() const
{
  size_t deg = 0;
  for (size_t n = 0; n < this->adj_.size(); ++n)
    if (this->adj_[n])
      deg++;
  return deg;
}

template <class T>
bool graph_node<T>::operator == (const graph_node<T>& other) const
{
  if (this->val_ != other.val_)
    return false;
  if (this->degree() != other.degree())
    return false;
  // Create a neighbor value set for each node.
  dyn_array<T> this_set, other_set;
  for (size_t n = 0; n < this->degree(); ++n) {
    this_set.add((*this)(n)->val_);
    other_set.add(other(n)->val_);
  }
  // Compare sorted value sets using dyn_array<T> operator.
  return this_set.quick_sort() == other_set.quick_sort();
}

template <class T>
bool graph_node<T>::operator != (const graph_node<T>& other) const
{
  return !(*this == other);
}

template <class T>
bool graph_node<T>::operator < (const graph_node<T>& other) const
{
  if (this->val_ == other.val_)
    return this->degree() < other.degree();
  return this->val_ < other.val_;
}

template <class T>
bool graph_node<T>::operator > (const graph_node<T>& other) const
{
  if (this->val_ == other.val_)
    return this->degree() > other.degree();
  return this->val_ > other.val_;
}

template <class T>
bool graph_node<T>::operator <= (const graph_node<T>& other) const
{
  if (this->val_ == other.val_)
    return this->degree() <= other.degree();
  return this->val_ <= other.val_;
}

template <class T>
bool graph_node<T>::operator >= (const graph_node<T>& other) const
{
  if (this->val_ == other.val_)
    return this->degree() >= other.degree();
  return this->val_ >= other.val_;
}

template <class T>
graph_node<T>& graph_node<T>::operator = (
    std::initializer_list<graph_node<T>*> list)
{
  this->adj_ = list;
  return *this;
}

template <class T>
graph_node<T>& graph_node<T>::add_edge(graph_node<T>* neighbor_ptr)
{
  this->adj_.add(neighbor_ptr);
  return *this;
}

template <class T>
graph_node<T>& graph_node<T>::clear_edges()
{
  this->adj_.clear();
  return *this;
}

template <class T>
graph_node<T>*& graph_node<T>::operator () (size_t i)
{
  for (size_t n = 0; n < this->adj_.size(); ++n) {
    if ((*this)[n] == nullptr)
      i++;
    else if (n == i)
      return (*this)[n];
  }
  // If this point is reached, i-th neighbor doesn't exist.
  return (*this)[this->adj_.size()];
}

template <class T>
const graph_node<T>* graph_node<T>::operator () (size_t i) const
{
  for (size_t n = 0; n < this->adj_.size(); ++n) {
    if ((*this)[n] == nullptr)
      i++;
    else if (n == i)
      return (*this)[n];
  }
  // If this point is reached, i-th neighbor doesn't exist.
  return (*this)[this->adj_.size()];
}

template <class T>
graph_node<T>*& graph_node<T>::operator [] (size_t n)
{
  assert(n < this->adj_.size() && "graph_node adj_ index out of bounds");
  return this->adj_[n];
}

template <class T>
const graph_node<T>* graph_node<T>::operator [] (size_t n) const
{
  assert(n < this->adj_.size() && "graph_node adj_ index out of bounds");
  return this->adj_[n];
}

template <class T>
gn_ptrs<T> graph_node<T>::depth_first_traverse(std::ostream* os_ptr,
                                               bool is_post)
{
  // Initialize the hash_table to keep track of visited nodes.
  hash_table<uintptr_t, uintptr_t> ht;
  ht.insert(reinterpret_cast<uintptr_t>(this),
            reinterpret_cast<uintptr_t>(nullptr));
  // Store the full traversal path unless a target node is found.
  dyn_array<uintptr_t> addr_path;
  // Lambda always returns false to fully traverse the graph.
  auto check_none = [](const graph_node<T>*) -> bool {return false;};
  this->dft(check_none, os_ptr, ht, addr_path, is_post);
  gn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
cgn_ptrs<T> graph_node<T>::depth_first_traverse(std::ostream* os_ptr,
                                                bool is_post) const
{
  // Initialize the hash_table to keep track of visited nodes.
  hash_table<uintptr_t, uintptr_t> ht;
  ht.insert(reinterpret_cast<uintptr_t>(this),
            reinterpret_cast<uintptr_t>(nullptr));
  // Store the full traversal path unless a target node is found.
  dyn_array<uintptr_t> addr_path;
  // Lambda always returns false to fully traverse the graph.
  auto check_none = [](const graph_node<T>*) -> bool {return false;};
  this->dft(check_none, os_ptr, ht, addr_path, is_post);
  cgn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<const graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
gn_ptrs<T> graph_node<T>::depth_first_search(const graph_node<T>* target,
                                                std::ostream* os_ptr)
{
  // Initialize the hash_table to keep track of visited nodes.
  hash_table<uintptr_t, uintptr_t> ht;
  ht.insert(reinterpret_cast<uintptr_t>(this),
            reinterpret_cast<uintptr_t>(nullptr));
  // Store the full traversal path unless a target node is found.
  dyn_array<uintptr_t> addr_path;
  // Lambda returns true if a pointer matching the target is found.
  auto check_ptr = [target](const graph_node<T>* gnptr) -> bool {
    return (gnptr == target) ? true : false;
  };
  // If target is found, reconstruct and return the path.
  if (this->dft(check_ptr, os_ptr, ht, addr_path)) {
    graph_node<T>* last = reinterpret_cast<graph_node<T>*>(addr_path.last());
    return last->reconstruct_path(ht).reverse();
  }
  // Otherwise return the full traversal path.
  gn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
cgn_ptrs<T> graph_node<T>::depth_first_search(const graph_node<T>* target,
                                              std::ostream* os_ptr) const
{
  // Initialize the hash_table to keep track of visited nodes.
  hash_table<uintptr_t, uintptr_t> ht;
  ht.insert(reinterpret_cast<uintptr_t>(this),
            reinterpret_cast<uintptr_t>(nullptr));
  // Store the full traversal path unless a target node is found.
  dyn_array<uintptr_t> addr_path;
  // Lambda returns true if a pointer matching the target is found.
  auto check_ptr = [target](const graph_node<T>* gnptr) -> bool {
    return (gnptr == target) ? true : false;
  };
  // If target is found, reconstruct and return the path.
  if (this->dft(check_ptr, os_ptr, ht, addr_path)) {
    uintptr_t l = addr_path.last();
    const graph_node<T>* last = reinterpret_cast<const graph_node<T>*>(l);
    return last->reconstruct_path(ht).reverse();
  }
  // Otherwise return the full traversal path.
  cgn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<const graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
gn_ptrs<T> graph_node<T>::depth_first_search(const T& target,
                                             std::ostream* os_ptr)
{
  // Initialize the hash_table to keep track of visited nodes.
  hash_table<uintptr_t, uintptr_t> ht;
  ht.insert(reinterpret_cast<uintptr_t>(this),
            reinterpret_cast<uintptr_t>(nullptr));
  // Store the full traversal path unless a target node is found.
  dyn_array<uintptr_t> addr_path;
  // Lambda returns true if a node with a value matching the target is found.
  auto check_val = [target](const graph_node<T>* gnptr) -> bool {
    return (gnptr->val() == target) ? true : false;
  };
  if (this->dft(check_val, os_ptr, ht, addr_path)) {
    graph_node<T>* last = reinterpret_cast<graph_node<T>*>(addr_path.last());
    return last->reconstruct_path(ht).reverse();
  }
  // Otherwise return the full traversal path.
  gn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
cgn_ptrs<T> graph_node<T>::depth_first_search(const T& target,
                                              std::ostream* os_ptr) const
{
  // Initialize the hash_table to keep track of visited nodes.
  hash_table<uintptr_t, uintptr_t> ht;
  ht.insert(reinterpret_cast<uintptr_t>(this),
            reinterpret_cast<uintptr_t>(nullptr));
  // Store the full traversal path unless a target node is found.
  dyn_array<uintptr_t> addr_path;
  // Lambda returns true if a node with a value matching the target is found.
  auto check_val = [target](const graph_node<T>* gnptr) -> bool {
    return (gnptr->val() == target) ? true : false;
  };
  if (this->dft(check_val, os_ptr, ht, addr_path)) {
    uintptr_t l = addr_path.last();
    const graph_node<T>* last = reinterpret_cast<const graph_node<T>*>(l);
    return last->reconstruct_path(ht).reverse();
  }
  // Otherwise return the full traversal path.
  cgn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<const graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
gn_ptrs<T> graph_node<T>::breadth_first_traverse(std::ostream* os_ptr)
{
  // Lambda returns 2 to request full traversal path from private engine.
  auto check_none = [](const graph_node<T>* gnptr) -> int {return 2;};
  dyn_array<uintptr_t> addr_path = this->bft(check_none, os_ptr);
  gn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
cgn_ptrs<T> graph_node<T>::breadth_first_traverse(std::ostream* os_ptr) const
{
  // Lambda returns 2 to request full traversal path from private engine.
  auto check_none = [](const graph_node<T>* gnptr) -> int {return 2;};
  dyn_array<uintptr_t> addr_path = this->bft(check_none, os_ptr);
  cgn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<const graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
gn_ptrs<T> graph_node<T>::breadth_first_search(const graph_node<T>* target,
                                               std::ostream* os_ptr)
{
  // Lambda returns 1 if a pointer matching the target is found.
  auto check_ptr = [target](const graph_node<T>* gnptr) -> int {
    return (gnptr == target) ? 1 : 0;
  };
  dyn_array<uintptr_t> addr_path = this->bft(check_ptr, os_ptr);
  gn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
cgn_ptrs<T> graph_node<T>::breadth_first_search(const graph_node<T>* target,
                                                std::ostream* os_ptr) const
{
  // Lambda returns 1 if a pointer matching the target is found.
  auto check_ptr = [target](const graph_node<T>* gnptr) -> int {
    return (gnptr == target) ? 1 : 0;
  };
  dyn_array<uintptr_t> addr_path = this->bft(check_ptr, os_ptr);
  cgn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<const graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
gn_ptrs<T> graph_node<T>::breadth_first_search(const T& target,
                                               std::ostream* os_ptr)
{
  // Lambda returns 1 if a node with a value matching the target is found.
  auto check_val = [target](const graph_node<T>* gnptr) -> int {
    return (gnptr->val() == target) ? 1 : 0;
  };
  dyn_array<uintptr_t> addr_path = this->bft(check_val, os_ptr);
  gn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
cgn_ptrs<T> graph_node<T>::breadth_first_search(const T& target,
                                               std::ostream* os_ptr) const
{
  // Lambda returns 1 if a node with a value matching the target is found.
  auto check_val = [target](const graph_node<T>* gnptr) -> int {
    return (gnptr->val() == target) ? 1 : 0;
  };
  dyn_array<uintptr_t> addr_path = this->bft(check_val, os_ptr);
  cgn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    path.add(reinterpret_cast<const graph_node<T>*>(addr_path[i]));
  return path;
}

template <class T>
gn_ptrs<T> graph_node<T>::bidirectional_search(const graph_node<T>* target,
                                               std::ostream* os_ptr)
{
  // Call the const version of the method.
  cgn_ptrs<T> cp;
  cp = static_cast<const graph_node<T>&>(*this).bidirectional_search(target,
                                                                     os_ptr);
  // Safely const cast results as this method itself is not const.
  gn_ptrs<T> path(nullptr, cp.size());
  for (size_t i = 0; i < cp.size(); ++i)
    path[i] = const_cast<graph_node<T>*>(cp[i]);
  return path;
}

template <class T>
cgn_ptrs<T> graph_node<T>::bidirectional_search(const graph_node<T>* target,
                                                std::ostream* os_ptr) const
{
  // Quick return if the query is the same as this node.
  if (this == target)
    return cgn_ptrs<T>().add(this);
  // Store separate hash tables for the two simultaneous searches.
  hash_table<uintptr_t, uintptr_t> ht1, ht2;
  // Store separate queues to allow nodes to fully expand in each iteration.
  queue<const graph_node<T>*> que1, que2;
  // Store full traversal path to return if target is not found.
  cgn_ptrs<T> path1, path2;
  // Enqueue both of the input nodes to start.
  que1.enqueue(this);
  ht1.insert(reinterpret_cast<uintptr_t>(this),
             reinterpret_cast<uintptr_t>(nullptr));
  que2.enqueue(target);
  ht2.insert(reinterpret_cast<uintptr_t>(target),
             reinterpret_cast<uintptr_t>(nullptr));
  const graph_node<T>* n1_ptr;
  const graph_node<T>* n2_ptr;
  while (!que1.is_empty() && !que2.is_empty()) {
    // Iterate the first node.
    n1_ptr = que1.dequeue();
    if (os_ptr)
      *os_ptr << n1_ptr->val();
    path1.add(n1_ptr);
    // Expand the first node.
    for (size_t n = 0; n < n1_ptr->degree(); ++n) {
      if (!ht1.contains(reinterpret_cast<uintptr_t>((*n1_ptr)(n)))) {
        // If the other search added this node to the queue, path is found
        // and this node is the intersection.
        if (ht2.contains(reinterpret_cast<uintptr_t>((*n1_ptr)(n)))) {
          if (os_ptr)
            *os_ptr << " [Target Found]";
          path1.clear();
          path2.clear();
          // Reconstruct the first path from the visited node.
          path1 = n1_ptr->reconstruct_path(ht1).reverse();
          // Reconstruct the second path from the intersection node.
          path2 = (*n1_ptr)(n)->reconstruct_path(ht2);
          // Merge and return the two paths.
          return path1.append(path2);
        }
        ht1.insert(reinterpret_cast<uintptr_t>((*n1_ptr)(n)),
                   reinterpret_cast<uintptr_t>(n1_ptr));
        que1.enqueue((*n1_ptr)(n));
      }
    }
    // Iterate the second node.
    n2_ptr = que2.dequeue();
    if (os_ptr)
      *os_ptr << " (" << n2_ptr->val() << ")";
    path2.add(n2_ptr);
    // Expand the second node.
    for (size_t n = 0; n < n2_ptr->degree(); ++n) {
      if (!ht2.contains(reinterpret_cast<uintptr_t>((*n2_ptr)(n)))) {
        // If the other search added this node to the queue, path is found
        // and this node is the intersection.
        if (ht1.contains(reinterpret_cast<uintptr_t>((*n2_ptr)(n)))) {
          path1.clear();
          path2.clear();
          // Reconstruct the first path from the intersection node.
          path1 = (*n2_ptr)(n)->reconstruct_path(ht1).reverse();
          // Reconstruct the second path from the visited node.
          path2 = n2_ptr->reconstruct_path(ht2);
          // Merge and return the two paths.
          return path1.append(path2);
        }
        ht2.insert(reinterpret_cast<uintptr_t>((*n2_ptr)(n)),
                   reinterpret_cast<uintptr_t>(n2_ptr));
        que2.enqueue((*n2_ptr)(n));
      }
    }
    if (!que1.is_empty() && !que2.is_empty() && os_ptr)
      *os_ptr << " -> ";
  }
  // If path isn't found, return merged traversal paths.
  return path1.append(path2);
}

template <class T>
bool graph_node<T>::has_cycle() const
{
  hash_table<uintptr_t, bool> ht;
  return this->has_cyc(ht);
}

template <class T>
bool graph_node<T>::dft(std::function<bool(const graph_node<T>*)> check,
                        std::ostream* os_ptr,
                        hash_table<uintptr_t, uintptr_t>& ht,
                        dyn_array<uintptr_t>& path,
                        bool is_post) const
{
  // If not post-order, visit this node first
  if (!is_post) {
    if (os_ptr)
      *os_ptr << this->val();
    path.add(reinterpret_cast<uintptr_t>(this));
    // If target is found on this node, return true.
    if (check(this)) {
      if (os_ptr)
        *os_ptr << " [Target Found]";
      return true;
    }
  }
  // Pick a branch and fully explore it before moving onto the next.
  for (size_t n = 0; n < this->degree(); ++n)
  {
    // If the neighbor is not yet visited.
    if (!ht.contains(reinterpret_cast<uintptr_t>((*this)(n)))) {
      if (os_ptr && !is_post)
        *os_ptr << " -> ";
      // Add this node as parent to the subsequent node to be visited.
      ht.insert(reinterpret_cast<uintptr_t>((*this)(n)),
                reinterpret_cast<uintptr_t>(this));
      // If target is found in a branch, recursion will return true.
      if ((*this)(n)->dft(check, os_ptr, ht, path, is_post))
        return true;
      if (os_ptr && is_post)
        *os_ptr << " -> ";
    }
  }
  // If post-order, visit this node after all its neighbors
  if (is_post)
  {
    if (os_ptr)
      *os_ptr << this->val();
    path.add(reinterpret_cast<uintptr_t>(this));
    // If target is found on this node, return true.
    if (check(this)) {
      if (os_ptr)
        *os_ptr << " [Target Found]";
      return true;
    }
  }
  // If target wasn't found in any of the branches, return false.
  return false;
}

template <class T>
dyn_array<uintptr_t> graph_node<T>::bft(
    std::function<int(const graph_node<T>*)> check, std::ostream* os_ptr) const
{
  // Initialize the hash_table to keep track of visited nodes.
  hash_table<uintptr_t, uintptr_t> ht;
  // Initialize the pointer queue to establish traversal order.
  queue<const graph_node<T>*> que;
  // Store full traversal path to return if target is not found.
  dyn_array<uintptr_t> path;
  // Enqueue the start node.
  que.enqueue(this);
  ht.insert(reinterpret_cast<uintptr_t>(this),
            reinterpret_cast<uintptr_t>(nullptr));
  // Traverse until the queue is empty or the target node is found.
  const graph_node<T>* cur_ptr;
  while (!que.is_empty()) {
    cur_ptr = que.dequeue();
    if (os_ptr)
      *os_ptr << cur_ptr->val();
    path.add(reinterpret_cast<uintptr_t>(cur_ptr));
    // If target is found, reconstruct the path from target and return it.
    if (check(cur_ptr) == 1) {
      if (os_ptr)
        *os_ptr << " [Target Found]";
      cgn_ptrs<T> ptr_path = cur_ptr->reconstruct_path(ht).reverse();
      path.clear();
      for (size_t i = 0; i < ptr_path.size(); ++i)
        path.add(reinterpret_cast<uintptr_t>(ptr_path[i]));
      return path;
    }
    // Enqueue all the unvisited neighbors of the current node.
    for (size_t n = 0; n < cur_ptr->degree(); ++n) {
      if (!ht.contains(reinterpret_cast<uintptr_t>((*cur_ptr)(n)))) {
        // Hash the node with its parent to avoid queueing unvisited nodes
        // more than once and to reconstruct the path later.
        ht.insert(reinterpret_cast<uintptr_t>((*cur_ptr)(n)),
                  reinterpret_cast<uintptr_t>(cur_ptr));
        que.enqueue((*cur_ptr)(n));
      }
    }
    if (!que.is_empty() && os_ptr)
      *os_ptr << " -> ";
  }
  // If target is not found, return the traversal path.
  return path;
}

template <class T>
bool graph_node<T>::has_cyc(hash_table<uintptr_t, bool>& ht) const
{
  if (ht.contains(reinterpret_cast<uintptr_t>(this)))
    return true;
  ht.insert(reinterpret_cast<uintptr_t>(this), true);
  for (size_t n = 0; n < this->degree(); ++n)
    if ((*this)(n)->has_cyc(ht))
      return true;
  ht.remove(reinterpret_cast<uintptr_t>(this));
  return false;
}

template <class T>
gn_ptrs<T> graph_node<T>::reconstruct_path(
    hash_table<uintptr_t, uintptr_t>& ht)
{
  gn_ptrs<T> path;
  graph_node<T>* cur_ptr = this;
  path.add(cur_ptr);
  uintptr_t key = reinterpret_cast<uintptr_t>(cur_ptr);
  uintptr_t uint_nullptr = reinterpret_cast<uintptr_t>(nullptr);
  while (ht.contains(key) && (ht[key] != uint_nullptr)) {
    // Add the parent
    cur_ptr = reinterpret_cast<graph_node<T>*>(ht[key]);
    path.add(cur_ptr);
    // Make the parent the new child
    key = ht[key];
  }
  return path;
}

template <class T>
cgn_ptrs<T> graph_node<T>::reconstruct_path(
    hash_table<uintptr_t, uintptr_t>& ht) const
{
  cgn_ptrs<T> path;
  const graph_node<T>* cur_ptr = this;
  path.add(cur_ptr);
  uintptr_t key = reinterpret_cast<uintptr_t>(cur_ptr);
  uintptr_t uint_nullptr = reinterpret_cast<uintptr_t>(nullptr);
  while (ht.contains(key) && (ht[key] != uint_nullptr)) {
    // Add the parent
    cur_ptr = reinterpret_cast<const graph_node<T>*>(ht[key]);
    path.add(cur_ptr);
    // Make the parent the new child
    key = ht[key];
  }
  return path;
}

template <class T>
std::ostream& graph_node<T>::print_sorted(std::ostream& os) const
{
  dyn_array<T> set;
  for (size_t n = 0; n < this->degree(); ++n)
    set.add((*this)(n)->val_);
  os << this->val_ << " " << set.quick_sort();
  return os;
}

template <class T>
std::ostream& operator << (std::ostream& os, const graph_node<T>& gn)
{
  os << gn.val_ << ": {";
  for (size_t n = 0; n < gn.degree(); ++n) {
    os << gn(n)->val();
    if (n < gn.degree() - 1)
      os << ", ";
  }
  os << "}";
  return os;
}

//======================================================================//
//                                                                      //
//                           graph<T> methods                           //
//                                                                      //
//======================================================================//

template <class T>
graph<T>::graph(std::initializer_list<T> node_vals)
{
  this->nodes_.realloc(node_vals.size());
  size_t ind = 0;
  for (const T& val : node_vals) {
    graph_node<T>* cur_node = new graph_node<T>(val, 0);
    cur_node->ind_ = ind++;
    this->nodes_.add(cur_node);
  }
  size_t sz = this->size();
  this->edges_ = dyn_array<dyn_array<int> >(dyn_array<int>(0, sz), sz);
}

template <class T>
graph<T>::graph(const dyn_array<T>& node_vals)
{
  this->nodes_.realloc(node_vals.size());
  for (size_t ind = 0; ind < node_vals.size(); ++ind) {
    graph_node<T>* cur_node = new graph_node<T>(node_vals[ind], 0);
    cur_node->ind_ = ind;
    this->nodes_.add(cur_node);
  }
  size_t sz = this->size();
  this->edges_ = dyn_array<dyn_array<int> >(dyn_array<int>(0, sz), sz);
}

template <class T>
graph<T>::graph(sq_init_list adj_mat)
{
  if (adj_mat.size() > 0) {
    size_t nrows = adj_mat.size();
    // Assert that each row size be equal to number of rows while parsing.
    dyn_array<dyn_array<int> > buf(dyn_array<int>(false, nrows), nrows);
    size_t r = 0;
    for (const std::initializer_list<int>& row : adj_mat) {
      assert(row.size() == nrows && "Edge matrix must be square");
      buf[r++] = row;
    }
    this->build_adj_mat(buf);
  }
}

template <class T>
graph<T>::graph(const dyn_array<dyn_array<int> >& adj_mat)
{
  if (adj_mat.size() > 0) {
    size_t nrows = adj_mat.size();
    // Assert that each row size be equal to number of rows while parsing.
    dyn_array<dyn_array<int> > buf(dyn_array<int>(false, nrows), nrows);
    for (size_t r = 0; r < nrows; ++r) {
      assert(adj_mat[r].size() == nrows && "Edge matrix must be square");
      buf[r] = adj_mat[r];
    }
    this->build_adj_mat(buf);
  }
}

template <class T>
graph<T>::graph(const graph<T>& source)
{
  this->nodes_.realloc(source.size());
  // Copy nodes using graph_node copy constructor.
  for (size_t i = 0; i < source.size(); ++i) {
    graph_node<T>* cur_node = new graph_node<T>(source[i]);
    cur_node->adj_.clear();
    this->nodes_.add(cur_node);
  }
  size_t sz = this->size();
  this->edges_ = dyn_array<dyn_array<int> >(dyn_array<int>(0, sz), sz);
  // Link each node with its neighbors.
  for (size_t i = 0; i < source.size(); ++i)
    for (size_t n = 0; n < source[i].degree(); ++n)
      this->add_edge(source[i].ind_, source[i](n)->ind_);
}

template <class T>
graph<T>::~graph()
{
  for (size_t n = 0; n < this->size(); ++n)
    delete this->nodes_[n];
}

template <class T>
const graph_node<T>& graph<T>::operator [] (size_t ind) const
{
  assert(ind < this->size() && "node index out of graph<T> bounds");
  return *(this->nodes_[ind]);
}

template <class T>
dyn_array<T> graph<T>::operator [] (const dyn_array<size_t>& inds) const
{
  dyn_array<T> ret;
  for (size_t i = 0; i < inds.size(); ++i)
    ret.add(this->nodes_[inds[i]]->val());
  return ret;
}

template <class T>
const T& graph<T>::operator () (size_t ind) const
{
  assert(ind < this->size() && "node index out of graph<T> bounds");
  return this->nodes_[ind]->val_;
}

template <class T>
T& graph<T>::operator () (size_t ind)
{
  assert(ind < this->size() && "node index out of graph<T> bounds");
  return this->nodes_[ind]->val_;
}

template <class T>
size_t graph<T>::num_edges() const
{
  size_t edg_count = 0;
  for (size_t row = 0; row < this->size(); ++row)
    for (size_t col = 0; col < this->size(); ++col)
      if (this->edges_[row][col])
        edg_count++;
  return edg_count;
}

template <class T>
graph<T>& graph<T>::clear()
{
  for (size_t n = 0; n < this->size(); ++n) {
    delete this->nodes_[n];
    this->nodes_[n] = nullptr;
  }
  this->nodes_.clear();
  this->edges_.clear();
  return *this;
}

template <class T>
graph<T>& graph<T>::clear_edges()
{
  size_t sz = this->size();
  for (size_t n = 0; n < sz; ++n)
    this->nodes_[n]->adj_.clear();
  this->edges_ = dyn_array<dyn_array<int> >(dyn_array<int>(0, sz), sz);
  return *this;
}

template <class T>
bool graph<T>::operator == (const graph<T>& other) const
{
  if (this->size() != other.size())
    return false;
  if (this->num_edges() != other.num_edges())
    return false;
  gn_ptrs<T> t_nodes = this->nodes_;
  gn_ptrs<T> o_nodes = other.nodes_;
  return t_nodes.quick_sort() == o_nodes.quick_sort();
}

template <class T>
bool graph<T>::operator != (const graph<T>& other) const
{
  return !(*this == other);
}

template <class T>
bool graph<T>::operator < (const graph<T>& other) const
{
  if (this->size() == other.size())
    return this->num_edges() < other.num_edges();
  return this->size() < other.size();
}

template <class T>
bool graph<T>::operator > (const graph<T>& other) const
{
  if (this->size() == other.size())
    return this->num_edges() > other.num_edges();
  return this->size() > other.size();
}

template <class T>
bool graph<T>::operator <= (const graph<T>& other) const
{
  if (this->size() == other.size())
    return this->num_edges() <= other.num_edges();
  return this->size() <= other.size();
}

template <class T>
bool graph<T>::operator >= (const graph<T>& other) const
{
  if (this->size() == other.size())
    return this->num_edges() >= other.num_edges();
  return this->size() >= other.size();
}

template <class T>
graph<T>& graph<T>::operator = (const graph<T>& source)
{
  if (this == &source)
    return *this;
  this->clear();
  if (source.is_empty())
    return *this;
  this->nodes_.realloc(source.size());
  // Copy nodes using graph_node copy constructor.
  for (size_t i = 0; i < source.size(); ++i) {
    graph_node<T>* cur_node = new graph_node<T>(source[i]);
    cur_node->adj_.clear();
    this->nodes_.add(cur_node);
  }
  size_t sz = this->size();
  this->edges_ = dyn_array<dyn_array<int> >(dyn_array<int>(0, sz), sz);
  // Link each node with its neighbors.
  for (size_t i = 0; i < source.size(); ++i)
    for (size_t n = 0; n < source[i].degree(); ++n)
      this->add_edge(source[i].ind_, source[i](n)->ind_);
  return *this;
}

template <class T>
graph<T>& graph<T>::operator = (sq_init_list adj_mat)
{
  if (adj_mat.size() == 0)
    return *this;
  size_t nrows = adj_mat.size();
  // Assert that each row size be equal to number of rows while parsing.
  dyn_array<dyn_array<int> > buf(dyn_array<int>(false, nrows), nrows);
  size_t r = 0;
  for (const std::initializer_list<int>& row : adj_mat) {
    assert(row.size() == nrows && "Edge matrix must be square");
    buf[r++] = row;
  }
  return this->build_adj_mat(buf);
}

template <class T>
graph<T>& graph<T>::operator = (const dyn_array<dyn_array<int> >& adj_mat)
{
  if (adj_mat.size() == 0)
    return *this;
  size_t nrows = adj_mat.size();
  // Assert that each row size be equal to number of rows while parsing.
  dyn_array<dyn_array<int> > buf(dyn_array<int>(false, nrows), nrows);
  for (size_t r = 0; r < nrows; ++r) {
    assert(adj_mat[r].size() == nrows && "Edge matrix must be square");
    buf[r] = adj_mat[r];
  }
  return this->build_adj_mat(buf);
}

template <class T>
graph<T>& graph<T>::operator = (std::initializer_list<T> node_vals)
{
  size_t n = 0;
  for (const T& val : node_vals) {
    if (n >= this->size())
      this->add_node(val);
    else
      this->nodes_[n]->set_val(val);
    n++;
  }
  return *this;
}

template <class T>
graph<T>& graph<T>::operator = (const dyn_array<T>& node_vals)
{
  for (size_t n = 0; n < node_vals.size(); ++n) {
    if (n >= this->size())
      this->add_node(node_vals[n]);
    else
      this->nodes_[n]->set_val(node_vals[n]);
  }
  return *this;
}

template <class T>
graph<T>& graph<T>::operator = (graph_node<T>* subgraph)
{
  this->clear();
  if (subgraph)
    this->add_node(subgraph);
  return *this;
}

template <class T>
T graph<T>::min() const
{
  if (this->is_empty())
    return T();
  T ret;
  ret = this->nodes_[0]->val();
  for (size_t n = 1; n < this->size(); ++n)
    if (ret > this->nodes_[n]->val())
      ret = this->nodes_[n]->val();
  return ret;
}

template <class T>
T graph<T>::max() const
{
  if (this->is_empty())
    return T();
  T ret;
  ret = this->nodes_[0]->val();
  for (size_t n = 1; n < this->size(); ++n)
    if (ret < this->nodes_[n]->val())
      ret = this->nodes_[n]->val();
  return ret;
}

template <class T>
dyn_array<T> graph<T>::node_vals() const
{
  dyn_array<T> ret;
  ret.realloc(this->size());
  for (size_t n = 0; n < this->size(); ++n)
    ret.add(this->nodes_[n]->val());
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::node_inds(const T& val) const
{
  dyn_array<size_t> ret;
  if (this->is_empty())
    return ret;
  for (size_t n = 0; n < this->size(); ++n)
    if (this->nodes_[n]->val() == val)
      ret.add(n);
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::neighbors(size_t ind) const
{
  assert(ind < this->size() && "node index out of graph<T> bounds");
  dyn_array<size_t> ret;
  for (size_t col = 0; col < this->size(); ++col)
    if (this->edges_[ind][col])
      ret.add(col);
  return ret;
}

template <class T>
size_t graph<T>::add_node(const T& val)
{
  // Index of the added node should be the node count prior to addition.
  size_t ret = this->size();
  // Create the new node and set its index.
  graph_node<T>* new_node = new graph_node<T>(val, 0);
  new_node->ind_ = ret;
  // Add the node's pointer to the nodes_ list.
  this->nodes_.add(new_node);
  // Add a new column to the adjacency/weight matrix.
  for (size_t row = 0; row < ret; ++row)
    this->edges_[row].add(0);
  // Add a new row to the adjacency/weight matrix.
  this->edges_.add(dyn_array<int>(0, ret+1));
  return ret;
}

template <class T>
size_t graph<T>::add_node(graph_node<T>* subgraph)
{
  gn_ptrs<T> p = subgraph->breadth_first_traverse();
  // Index of the added node dhould be the node count prior to addition.
  size_t ret = this->size();
  // Go through the path, attach indices to each node on it, then add nodes.
  for (size_t n = 0; n < p.size(); ++n) {
    p[n]->ind_ = ret + n;
    this->add_node(p[n]->val());
  }
  // Go through the path nodes again and add edges from their adjacency lists.
  for (size_t n = 0; n < p.size(); ++n)
    for (size_t e = 0; e < p[n]->degree(); ++e)
      this->add_edge(p[n]->ind_, (*p[n])(e)->ind_);
  return ret;
}

template <class T>
bool graph<T>::remove_node(size_t ind)
{
  if (ind >= this->size())
    return false;
  graph_node<T>* del_node = this->nodes_[ind];
  // Clear outgoing edges. Each deletion moves del_node(0) to the next.
  size_t degree = del_node->degree();
  for (size_t n = 0; n < degree; ++n)
    (*del_node)(0) = nullptr;
  // Clear incoming edges using the adjacency/weight matrix
  for (size_t row = 0; row < this->size(); ++row) {
    if (this->edges_[row][ind]) {
      graph_node<T>* node_row = this->nodes_[row];
      for (size_t n = 0; n < node_row->degree(); ++n) {
        if ((*node_row)(n) == del_node) {
          (*node_row)(n) = nullptr;
          break;
        }
      }
    }
  }
  // Remove its row and column from the adjacency/weight matrix.
  size_t newsz = this->size() - 1;
  dyn_array<dyn_array<int> > new_matrix(dyn_array<int>(0, newsz), newsz);
  for (size_t row = 0; row < this->size(); ++row) {
    for (size_t col = 0; col < this->size(); ++col) {
      if ((row == ind) || (col == ind))
        continue;
      if (!this->edges_[row][col])
        continue;
      if ((row < ind) && (col < ind))
        new_matrix[row][col] = this->edges_[row][col];
      else if ((row < ind) && (col > ind))
        new_matrix[row][col-1] = this->edges_[row][col];
      else if ((row > ind) && (col < ind))
        new_matrix[row-1][col] = this->edges_[row][col];
      else if ((row > ind) && (col > ind))
        new_matrix[row-1][col-1] = this->edges_[row][col];
    }
  }
  this->edges_ = new_matrix;
  // Update node indices that are greater than the deleted index.
  for (size_t i = ind + 1; i < this->size(); ++i) {
    this->nodes_[i-1] = this->nodes_[i];
    this->nodes_[i-1]->ind_--;
  }
  delete del_node;
  this->nodes_.remove();
  return true;
}

template <class T>
bool graph<T>::add_edge(size_t i, size_t j)
{
  if ((i >= this->size()) || (j >= this->size()))
    return false;
  // Check/set the corresponding element in the adjacency/weight matrix.
  if (this->edges_[i][j])
    return true;
  else
    this->edges_[i][j] = 1;
  // Add pointer to node j to the adjacency list of node i.
  this->nodes_[i]->adj_.add(this->nodes_[j]);
  return true;
}

template <class T>
bool graph<T>::add_edgew(size_t i, size_t j, int weight)
{
  if ((i >= this->size()) || (j >= this->size()))
    return false;
  // Check/set the corresponding element in the adjacency/weight matrix.
  if (this->edges_[i][j] == weight)
    return true;
  else
    this->edges_[i][j] = weight;
  // Add pointer to node j to the adjacency list of node i.
  this->nodes_[i]->adj_.add(this->nodes_[j]);
  return true;
}

template <class T>
bool graph<T>::add_edge_undir(size_t i, size_t j)
{
  if ((i >= this->size()) || (j >= this->size()))
    return false;
  // Check/set the corresponding elements in the adjacency/weight matrix.
  if (this->edges_[i][j] && this->edges_[j][i])
    return true;
  if (!this->edges_[i][j])
    this->nodes_[i]->adj_.add(this->nodes_[j]);
  this->edges_[i][j] = 1;
  if (!this->edges_[j][i])
    this->nodes_[j]->adj_.add(this->nodes_[i]);
  this->edges_[j][i] = 1;
  return true;
}

template <class T>
bool graph<T>::add_edgew_undir(size_t i, size_t j, int weight)
{
  if ((i >= this->size()) || (j >= this->size()))
    return false;
  // Check/set the corresponding elements in the adjacency/weight matrix.
  if (this->edges_[i][j] == weight && this->edges_[j][i] == weight)
    return true;
  if (!this->edges_[i][j])
    this->nodes_[i]->adj_.add(this->nodes_[j]);
  this->edges_[i][j] = weight;
  if (!this->edges_[j][i])
    this->nodes_[j]->adj_.add(this->nodes_[i]);
  this->edges_[j][i] = weight;
  return true;
}

template <class T>
bool graph<T>::remove_edge(size_t i, size_t j)
{
  if ((i >= this->size()) || (j >= this->size()))
    return false;
  if (!this->edges_[i][j])
    return false;
  // Find and nullify the adjacency pointers.
  graph_node<T>* node_i = this->nodes_[i];
  for (size_t n = 0; n < node_i->degree(); ++n) {
    if ((*node_i)(n) == this->nodes_[j]) {
      (*node_i)(n) = nullptr;
      break;
    }
  }
  // Update the adjacency/weight matrix
  this->edges_[i][j] = 0;
  return true;
}

template <class T>
bool graph<T>::remove_edge_undir(size_t i, size_t j)
{
  if ((i >= this->size()) || (j >= this->size()))
    return false;
  if (!this->edges_[i][j] && !this->edges_[j][i])
    return false;
  // Find and nullify the adjacency pointers.
  bool ret = true;
  if (edges_[i][j]) {
    graph_node<T>* node_i = this->nodes_[i];
    for (size_t n = 0; n < node_i->degree(); ++n) {
      if ((*node_i)(n) == this->nodes_[j]) {
        (*node_i)(n) = nullptr;
        break;
      }
    }
  }
  else {
    ret = false;
  }
  if (edges_[j][i]) {
    graph_node<T>* node_j = this->nodes_[j];
    for (size_t n = 0; n < node_j->degree(); ++n) {
      if ((*node_j)(n) == this->nodes_[i]) {
        (*node_j)(n) = nullptr;
        break;
      }
    }
  }
  else {
    ret = false;
  }
  // Update the adjacency/weight matrix
  this->edges_[i][j] = 0;
  this->edges_[j][i] = 0;
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::depth_first_traverse(size_t from_ind,
                                                 std::ostream* os_ptr) const
{
  assert(from_ind < this->size() && "node index out of graph<T> bounds");
  gn_ptrs<T> p = this->nodes_[from_ind]->depth_first_traverse(os_ptr);
  dyn_array<size_t> ret(0, p.size());
  for (size_t i = 0; i < p.size(); ++i)
    ret[i] = p[i]->ind_;
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::breadth_first_traverse(size_t from_ind,
                                                   std::ostream* os_ptr) const
{
  assert(from_ind < this->size() && "node index out of graph<T> bounds");
  gn_ptrs<T> p = this->nodes_[from_ind]->breadth_first_traverse(os_ptr);
  dyn_array<size_t> ret(0, p.size());
  for (size_t i = 0; i < p.size(); ++i)
    ret[i] = p[i]->ind_;
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::depth_first_search(size_t from_ind, const T& val,
                                               std::ostream* os_ptr) const
{
  assert(from_ind < this->size() && "node index out of graph<T> bounds");
  gn_ptrs<T> p = this->nodes_[from_ind]->depth_first_search(val, os_ptr);
  dyn_array<size_t> ret(0, p.size());
  for (size_t i = 0; i < p.size(); ++i)
    ret[i] = p[i]->ind_;
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::breadth_first_search(size_t from_ind, const T& val,
                                                 std::ostream* os_ptr) const
{
  assert(from_ind < this->size() && "node index out of graph<T> bounds");
  gn_ptrs<T> p = this->nodes_[from_ind]->breadth_first_search(val, os_ptr);
  dyn_array<size_t> ret(0, p.size());
  for (size_t i = 0; i < p.size(); ++i)
    ret[i] = p[i]->ind_;
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::bidirectional_search(size_t i, size_t j,
                                                 std::ostream* os_ptr) const
{
  assert(i < this->size() && "node index i out of graph<T> bounds");
  assert(j < this->size() && "node index j out of graph<T> bounds");
  gn_ptrs<T> p;
  p = this->nodes_[i]->bidirectional_search(this->nodes_[j], os_ptr);
  dyn_array<size_t> ret(0, p.size());
  for (size_t i = 0; i < p.size(); ++i)
    ret[i] = p[i]->ind_;
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::find_roots() const
{
  // Use edge matrix columns to find nodes with no incoming edges.
  dyn_array<size_t> root_nodes;
  for (size_t col = 0; col < this->size(); ++col) {
    bool no_incoming = true;
    for (size_t row = 0; row < this->size(); ++row) {
      if (this->edges_[row][col]) {
        no_incoming = false;
        break;
      }
    }
    if (no_incoming)
      root_nodes.add(col);
  }
  return root_nodes;
}

template <class T>
bool graph<T>::has_cycle() const
{
  if (this->is_empty())
    return false;
  hash_table<uintptr_t, bool> visited;
  hash_table<uintptr_t, bool> in_stack;
  for (size_t n = 0; n < this->size(); ++n) {
    uintptr_t key = reinterpret_cast<uintptr_t>(this->nodes_[n]);
    if (!visited.contains(key)) {
      if (this->cycle_rec(n, visited, in_stack))
        return true;
    }
  }
  return false;
}

template <class T>
bool graph<T>::cycle_rec(size_t ind,
                         hash_table<uintptr_t, bool>& visited,
                         hash_table<uintptr_t, bool>& in_stack) const
{
  // DFS cycle detect with global visited + local stack.
  uintptr_t key = reinterpret_cast<uintptr_t>(this->nodes_[ind]);
  // Back-edge to current path: cycle.
  if (in_stack.contains(key))
    return true;
  // Finished or cross: safe.
  if (visited.contains(key))
    return false;
  // Mark visited.
  visited.insert(key, true);
  // Mark in-stack (gray).
  in_stack.insert(key, true);
  const graph_node<T>* cur = this->nodes_[ind];
  for (size_t n = 0; n < cur->degree(); ++n) {
    uintptr_t neigh_key = reinterpret_cast<uintptr_t>((*cur)(n));
    if (this->cycle_rec((*cur)(n)->ind_, visited, in_stack))
      return true;
  }
  // Pop from stack (black now).
  in_stack.remove(key);
  return false;
}

template <class T>
dyn_array<T> graph<T>::topological_sort() const
{
  dyn_array<T> ret;
  if (this->is_empty() || this->has_cycle())
    return ret;
  // Compute in-degrees from adj_ list
  size_t num_nodes = this->size();
  dyn_array<int> in_degree(0, num_nodes);
  // Sum in-degrees: O(V+E)
  for (size_t u = 0; u < num_nodes; ++u) {
    const graph_node<T>* node_u = this->nodes_[u];
    for (size_t n = 0; n < node_u->degree(); ++n) {
      const graph_node<T>* node_v = (*node_u)(n);
      // Skip nulls.
      if (node_v && node_v->ind_ < num_nodes)
          in_degree[node_v->ind_]++;
    }
  }
  // Collect sources (in_degree == 0)
  dyn_array<size_t> start_nodes;
  for (size_t u = 0; u < num_nodes; ++u)
    if (in_degree[u] == 0)
      start_nodes.add(u);
  // Use hash table to keep track of visited nodes and pass to dft method.
  hash_table<uintptr_t, uintptr_t> ht;
  // Lambda always returns false to fully traverse the graph.
  auto check_none = [](const graph_node<T>*) -> bool {return false;};
  dyn_array<uintptr_t> addr_path;
  for (size_t i = 0; i < start_nodes.size(); ++i) {
    const graph_node<T>* cur = this->nodes_[start_nodes[i]];
    ht.insert(reinterpret_cast<uintptr_t>(cur),
              reinterpret_cast<uintptr_t>(nullptr));
    cur->dft(check_none, nullptr, ht, addr_path, true);
  }
  gn_ptrs<T> path;
  for (size_t i = 0; i < addr_path.size(); ++i)
    ret.add(reinterpret_cast<graph_node<T>*>(addr_path[i])->val());
  return ret;
}

template <class T>
dyn_array<dyn_array<size_t> > graph<T>::strongly_conn_comp() const
{
  if (this->is_empty())
    return dyn_array<dyn_array<size_t> >();
  size_t num_nodes = this->size();
  // Initialize tbun with strongly-connected component mode.
  tarjan_bundle tbun(SCC, num_nodes);
  // Depth-first traverse from each unvisited node with Tarjan's algorithm.
  for (size_t u = 0; u < num_nodes; ++u)
    if (tbun.disc[u] == -1)
      this->tarjan(u, tbun);
  return tbun.scc_arr;
}

template <class T>
dyn_array<int> graph<T>::weakly_conn_comp() const
{
  if (this->is_empty())
    return dyn_array<int>();
  // -1 is reserved for not visited, indicates an issue with graph object.
  dyn_array<int> ret(-1, this->size());
  // Use hash table to keep track of visited nodes and their labels.
  hash_table<uintptr_t, int> ht;
  // Create union-find for merging during traversal.
  union_find uf(static_cast<int>(this->size()));
  // Recursively traverse from each node and label until all nodes are visited.
  for (size_t n = 0; n < this->size(); ++n)
    if (!ht.contains(reinterpret_cast<uintptr_t>(this->nodes_[n])))
      this->comp_rec(n, ht, ret, uf);
  // Map roots to contiguous labels + update hash_table selectively.
  dyn_array<int> root_map(-1, this->size());
  int label = 0;
  for (size_t i = 0; i < this->size(); ++i) {
    int r = uf.find(static_cast<int>(i));
    if (root_map[static_cast<size_t>(r)] == -1)
      root_map[static_cast<size_t>(r)] = label++;
    ret[i] = root_map[static_cast<size_t>(r)];
    // Selective ht update (like original): only for visited keys.
    uintptr_t key = reinterpret_cast<uintptr_t>(this->nodes_[i]);
    // Replace labels in the hash table with updated ones.
    if (ht.contains(key))
      ht.insert(key, ret[i]);
  }
  return ret;
}

template <class T>
void graph<T>::comp_rec(size_t ind,
                        hash_table<uintptr_t, int>& ht,
                        dyn_array<int>& conn_lbl,
                        union_find& uf) const
{
  const graph_node<T>* cur = this->nodes_[ind];
  uintptr_t key = reinterpret_cast<uintptr_t>(cur);
  // Mark recursed node as visited with -1.
  ht.insert(key, -1);
  // Also mark label array, eventual label will be assigned later.
  conn_lbl[cur->ind_] = -1;
  for (size_t i = 0; i < cur->degree(); ++i) {
    uintptr_t neigh_key = reinterpret_cast<uintptr_t>((*cur)(i));
    size_t neigh_ind = (*cur)(i)->ind_;
    // Union on every neighbor (weak connectivity).
    uf.union_sets(static_cast<int>(ind), static_cast<int>(neigh_ind));
    // Recurse into each unvisited neighbor.
    if (!ht.contains(key))
      this->comp_rec(neigh_ind, ht, conn_lbl, uf);
  }
  return;
}

template <class T>
dyn_array<dyn_array<size_t> > graph<T>::bridges() const
{
  if (this->is_empty())
    return dyn_array<dyn_array<size_t> >();
  size_t num_nodes = this->size();
  // Initialize tbun with bridge detection mode.
  tarjan_bundle tbun(BRIDGE, num_nodes);
  // Depth-first traverse from each unvisited node with Tarjan's algorithm.
  for (size_t u = 0; u < num_nodes; ++u)
    if (tbun.disc[u] == -1)
      this->tarjan(u, tbun);
  return tbun.bridges;
}

template <class T>
dyn_array<size_t> graph<T>::articulation_points() const
{
  if (this->is_empty())
    return dyn_array<size_t>();
  size_t num_nodes = this->size();
  // Initialize tbun with articulation point finder mode.
  tarjan_bundle tbun(ARTICULATION, num_nodes);
  // Depth-first traverse from each unvisited node with Tarjan's algorithm.
  for (size_t u = 0; u < num_nodes; ++u)
    if (tbun.disc[u] == -1)
      this->tarjan(u, tbun);
  return tbun.art_pts.search(true);
}

template <class T>
dyn_array<int> graph<T>::shortest_dists(size_t src) const
{
  return this->dijkstra(src, false)[0];
}

template <class T>
dyn_array<dyn_array<int>> graph<T>::dijkstra(size_t src, bool accumulate) const
{
  assert(src < this->size() && "dijkstra's src out of graph<T> bounds");
  // Initialize distance vector with -1 values, set src distance to 0.
  dyn_array<int> dist(-1, this->size());
  dist[src] = 0;
  // Initialize predecessor vector with -1 values.
  dyn_array<int> pred(-1, this->size());
  // Insert (distance, index) pairs as 2-element dyn_arrays into min heap.
  heap<dyn_array<int> > min_heap(false);
  dyn_array<int> pair(0, 2), buf(0, 2);
  pair[1] = static_cast<int>(src);
  min_heap.insert(pair);
  int v, dist_v;
  graph_node<T> node_u;
  // Process nodes until heap is empty.
  while (!min_heap.is_empty()) {
    pair = min_heap.remove();
    // Access neighbors from adjacency lists for efficiency.
    node_u = *(this->nodes_[pair[1]]);
    for (int n = 0; n < node_u.degree(); ++n) {
      v = static_cast<int>(node_u(n)->ind_);
      // Update distance to v if a shorter distance is found through u.
      dist_v = this->edges_[pair[1]][v] + dist[pair[1]];
      if (dist[v] == -1 || dist_v < dist[v]) {
        dist[v] = dist_v;
        pred[v] = pair[1];
        buf = {dist[v], static_cast<int>(v)};
        // Add v with its updated distance to min heap.
        min_heap.insert(buf);
      }
    }
  }
  // Return only distances at index 0 if accumulate is set to false.
  if (!accumulate) {
    dyn_array<dyn_array<int> > ret(dist);
    return ret;
  }
  // Reconstruct paths using predecessors if accumulate is set to true.
  dyn_array<dyn_array<int> > ret(dyn_array<int>(-1), this->size());
  for (int i = 0; i < this->size(); ++i) {
    if (dist[i] == -1)
      continue;
    dyn_array<int> path;
    int cur = i;
    while (cur != src) {
      path.add(cur);
      cur = pred[cur];
    }
    path.add(cur);
    ret[i] = path.reverse();
  }
  return ret;
}

template <class T>
dyn_array<size_t> graph<T>::kahn() const
{
  dyn_array<size_t> ret;
  if (this->is_empty())
    return ret;
  // Compute in-degrees from adj_ list.
  size_t num_nodes = this->size();
  dyn_array<int> in_degree(0, num_nodes);
  // Sum in-degrees: O(V+E)
  for (size_t u = 0; u < num_nodes; ++u) {
    const graph_node<T>* node_u = this->nodes_[u];
    for (size_t n = 0; n < node_u->degree(); ++n) {
      const graph_node<T>* node_v = (*node_u)(n);
      if (node_v) {
        assert(node_v->ind_ < num_nodes && "graph's node index out of bounds");
        in_degree[node_v->ind_]++;
      }
    }
  }
  // Collect sources (in_degree == 0) in queue.
  queue<size_t> nodes;
  for (size_t u = 0; u < num_nodes; ++u)
    if (in_degree[u] == 0)
      nodes.enqueue(u);
  // Dequeue order will be topologically sorted.
  size_t cur_ind;
  while (!nodes.is_empty()) {
    cur_ind = nodes.dequeue();
    ret.add(cur_ind);
    for (size_t edge = 0; edge < this->edges_[cur_ind].size(); ++edge) {
      if (this->edges_[cur_ind][edge])
        if (--in_degree[edge] == 0)
          nodes.enqueue(edge);
    }
  }
  return ret;
}

template <class T>
void graph<T>::tarjan(size_t ind, tarjan_bundle& tbun) const
{
  tbun.disc[ind] = tbun.low[ind] = tbun.timestamp();
  int children = 0;
  if (tbun.mode() == SCC) {
    tbun.push(ind);
    tbun.on_stack[ind] = true;
  }
  size_t v, scc_node;
  const graph_node<T> cur = *(this->nodes_[ind]);
  for (size_t n = 0; n < cur.degree(); ++n) {
    v = cur(n)->ind_;
    // Recurse into each unvisited neighbor of current node with index ind.
    if (tbun.disc[v] == -1) {
      children++;
      if (tbun.mode() == BRIDGE || tbun.mode() == ARTICULATION)
        tbun.parent[v] = static_cast<int>(ind);
      this->tarjan(v, tbun);
      // If recursion found a lower discovery time reachable from v, it is by
      // definition reachable from u as well, so propagate the new min up.
      if (tbun.low[v] < tbun.low[ind])
        tbun.low[ind] = tbun.low[v];
      // Bridge check.
      if (tbun.mode() == BRIDGE && tbun.low[v] > tbun.disc[ind]) {
        dyn_array<size_t> br_edge;
        (ind < v) ? (br_edge = {ind, v}) : (br_edge = {v, ind});
        tbun.bridges.add(br_edge);
      }
      // Articultion point check for non-root.
      if (tbun.mode() == ARTICULATION)
        if (tbun.parent[ind] != -1 && tbun.low[v] >= tbun.disc[ind])
          tbun.art_pts[ind] = true;
    }
    // If neighbor is already visited, handle back edges.
    else {
      // If neighbor is visited and is on the stack, this is a back edge.
      // Neighbor is an ancestor and current node can reach its discovery time.
      if (tbun.mode() == SCC)
        if (tbun.on_stack[v])
          if (tbun.disc[v] < tbun.low[ind])
            tbun.low[ind] = tbun.disc[v];
      // In bridge or articulation mode, update only if neighbor is not parent.
      if (tbun.mode() == BRIDGE || tbun.mode() == ARTICULATION)
        if (v != tbun.parent[ind])
          if (tbun.disc[v] < tbun.low[ind])
            tbun.low[ind] = tbun.disc[v];
    }
  }
  // Strongly-connected component in scc_arr is ready if cur is its root.
  // Pop the stack until current node is popped and added to the component.
  if (tbun.mode() == SCC) {
    if (tbun.low[ind] == tbun.disc[ind]) {
      dyn_array<size_t> scc;
      while (!tbun.is_empty()) {
        scc_node = tbun.pop();
        tbun.on_stack[scc_node] = false;
        scc.add(scc_node);
        if (scc_node == ind)
          break;
      }
      tbun.scc_arr.add(scc);
    }
  }
  // Root with children is an articulation point.
  else if (tbun.mode() == ARTICULATION) {
    if (tbun.parent[ind] == -1)
      if (children > 1)
        tbun.art_pts[ind] = true;
  }
  return;
}

template <class T>
graph<T>& graph<T>::build_adj_mat(const dyn_array<dyn_array<int> >& adj_mat)
{
  size_t num_nodes = adj_mat.size();
  size_t prev_sz = this->size();
  // Constructing from empty graph.
  if (prev_sz == 0) {
    this->nodes_.resize(num_nodes);
    // Create nodes and assign indices.
    for (size_t n = 0; n < this->size(); ++n) {
      this->nodes_[n] = new graph_node<T>(0);
      this->nodes_[n]->ind_ = n;
    }
    // Initialize edge matrix.
    dyn_array<int> zero_row(0, num_nodes);
    this->edges_ = dyn_array<dyn_array<int> >(zero_row, num_nodes);
  }
  // Assigning to an existing non-empty graph.
  else {
    // If size will shrink, delete excess nodes prior to resizing nodes_ array.
    for (size_t n = prev_sz; n > num_nodes; --n)
      this->remove_node(n-1);
    // If size grew, create new nodes without neighbors and assign their indices.
    for (size_t n = prev_sz; n < num_nodes; ++n) {
      this->nodes_.add(new graph_node<T>(0));
      this->nodes_.last()->ind_ = n;
    }
    // Clear all edges from edge matrix and adjacency lists.
    this->clear_edges();
  }
  // Add edges.
  for (size_t row = 0; row < adj_mat.size(); ++row)
    for (size_t col = 0; col < adj_mat[row].size(); ++col)
      if (adj_mat[row][col])
        this->add_edge(row, col);
  return *this;
}

template <class T>
std::ostream& graph<T>::print_nodes(std::ostream& os) const
{
  if (this->is_empty())
    return os;
  int dec, off, num, max;
  this->get_dec(dec, off, num, max);
  os << "IND";
  for (int i = 0; i < off + dec - 2; ++i)
    os << " ";
  os << ": ";
  for (size_t n = 0; n < this->size(); ++n)
    os << std::setw(dec) << this->nodes_[n]->ind_ << ((n < num) ? " " : "");
  os << std::endl;
  os << "VAL";
  for (int i = 0; i < off + dec - 2; ++i)
    os << " ";
  os << ": ";
  for (size_t n = 0; n < this->size(); ++n)
    os << std::setw(dec) << this->nodes_[n]->val() << ((n < num) ? " " : "");
  return os << std::endl;
}

template <class T>
std::ostream& graph<T>::print_adj_lists(std::ostream& os) const
{
  if (this->is_empty())
    return os;
  int dec, off, num, max;
  this->get_dec(dec, off, num, max);
  for (size_t n = 0; n < this->size(); ++n)
    os << std::setw(off) << *this->nodes_[n] << std::endl;
  return os;
}

template <class T>
std::ostream& graph<T>::print_sorted_adj_lists(std::ostream& os) const
{
  if (this->is_empty())
    return os;
  gn_ptrs<T> nodes = this->nodes_;
  nodes.quick_sort();
  for (size_t i = 0; i < this->size(); ++i) {
    gn_ptrs<T> neighbors = nodes[i]->adj_;
    neighbors.quick_sort();
    os << nodes[i]->val() << "{";
    for (size_t n = 0; n < neighbors.size(); ++n)
      os << neighbors[n]->val() << ((n < neighbors.size()-1) ? "," : "");
    os << "}";
  }
  return os;
}

template <class T>
std::ostream& graph<T>::print_adj_matrix(std::ostream& os) const
{
  if (this->is_empty())
    return os;
  int dec, off, num, max;
  this->get_dec(dec, off, num, max);
  for (size_t row = 0; row < this->size(); ++row) {
    os << std::setw(off) << row << " ";
    os << std::setw(dec) << this->nodes_[row]->val() << ": ";
    for (size_t col = 0; col < this->size(); ++col) {
      os << std::setw(dec) << static_cast<int>(this->edges_[row][col]);
      ((col < num) ? (os << " ") : (os << std::endl));
    }
  }
  return os;
}

template <class T>
std::ostream& operator << (std::ostream& os, const graph<T>& graph)
{
  graph.print_adj_lists(os);
  graph.print_nodes(os);
  graph.print_adj_matrix(os);
  return os;
}

template <class T>
void graph<T>::get_dec(int& dec, int& off, int& num, int& max) const
{
  num = static_cast<int>(this->size()) - 1;
  max = static_cast<int>(this->max());
  max = (num > max) ? num : max;
  dec = 1;
  // Compute all relevant decimal places for various printouts to align.
  while (max >= 10) {
    dec++;
    max = max / 10;
  }
  off = 1;
  int buf = num;
  while (buf >= 10) {
    off++;
    buf = buf / 10;
  }
  return;
}

template <>
void graph<std::string>::get_dec(int& dec, int& off, int& num, int& max) const
{
  num = static_cast<int>(this->size()) - 1;
  max = 0;
  for (size_t n = 0; n < this->size(); ++n) {
    int str_sz = static_cast<int>(this->nodes_[n]->val().size());
    max = (str_sz > max) ? str_sz : max;
  }
  max = (num > max) ? num : max;
  dec = max;
  off = 1;
  int buf = num;
  while (buf >= 10) {
    off++;
    buf = buf / 10;
  }
  return;
}