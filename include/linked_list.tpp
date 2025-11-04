// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "linked_list.h"
#include "dyn_array.h"

//======================================================================//
//                                                                      //
//                          node<T, D> methods                          //
//                                                                      //
//======================================================================//

template <class T, bool D>
node<T, D>::~node()
{
  if (this->next())
    delete this->next();
}

template <class T, bool D>
node<T, D>* node<T, D>::clone() const
{
  const node<T, D>* head = this->find_head();
  node<T, D>* new_node = node<T, D>::create(head->val_);
  if (head->next()) {
    new_node->next() = head->next()->clone_fwd();
    if (D)
      new_node->next()->prev() = new_node;
  }
  return new_node;
}

template <class T, bool D>
node<T, D>* node<T, D>::clone_fwd() const
{
  node<T, D>* new_node = node<T, D>::create(this->val_);
  if (this->next()) {
    new_node->next() = this->next()->clone_fwd();
    if (D)
      new_node->next()->prev() = new_node;
  }
  return new_node;
}

template <class T, bool D>
node<T, D>* node<T, D>::clone_bwd() const
{
  if (!D)
    return nullptr;
  node<T, D>* new_node = node<T, D>::create(this->val_);
  if (this->prev()) {
    new_node->prev() = this->prev()->clone_bwd();
    new_node->prev()->next() = new_node;
  }
  return new_node;
}

template <class T, bool D>
node <T, D>* node<T, D>::clone_node() const
{
  node<T, D>* new_node = node<T, D>::create(this->val_);
  return new_node;
}

template <class T, bool D>
node<T, D>* node<T, D>::set_val(const T& val)
{
  this->val_ = val;
  return this;
}

template <class T, bool D>
size_t node<T, D>::size() const
{
  size_t ret = 1;
  const node<T, D>* n_ptr = this->find_head();
  while (n_ptr->next()) {
    n_ptr = n_ptr->next();
    ret++;
  }
  return ret;
}

template <class T, bool D>
node<T, D>* node<T, D>::clear()
{
  node<T, D>* head = this->find_head();
  if (head->next())
    delete head->next();
  head->next() = nullptr;
  return head;
}

template <class T, bool D>
bool node<T, D>::operator == (const node<T, D>& other) const
{
  const node<T, D>* t_ptr = this->find_head();
  const node<T, D>* o_ptr = other.find_head();
  while (t_ptr) {
    if (o_ptr == nullptr)
      return false;
    if (o_ptr->val() != t_ptr->val())
      return false;
    t_ptr = t_ptr->next();
    o_ptr = o_ptr->next();
  }
  if (o_ptr)
    return false;
  return true;
}

template <class T, bool D>
bool node<T, D>::operator != (const node<T, D>& other) const
{
  return !(*this== other);
}

template <class T, bool D>
bool node<T, D>::operator < (const node<T, D>& other) const
{
  if (this->val_ == other.val_)
    return this->size() < other.size();
  return this->val_ < other.val_;
}

template <class T, bool D>
bool node<T, D>::operator > (const node<T, D>& other) const
{
  if (this->val_ == other.val_)
    return this->size() > other.size();
  return this->val_ > other.val_;
}

template <class T, bool D>
bool node<T, D>::operator <= (const node<T, D>& other) const
{
  if (this->val_ == other.val_)
    return this->size() <= other.size();
  return this->val_ <= other.val_;
}

template <class T, bool D>
bool node<T, D>::operator >= (const node<T, D>& other) const
{
  if (this->val_ == other.val_)
    return this->size() >= other.size();
  return this->val_ >= other.val_;
}

template <class T, bool D>
node<T, D>& node<T, D>::operator = (const node<T, D>& source)
{
  if (this == &source)
    return *this;
  node<T, D>* head = this->clear();
  head->val_ = source.val_;
  if (source.next()) {
    head->next() = source.next()->clone();
    if (D)
      head->next()->prev() = head;
  }
  return *head;
}

template <class T, bool D>
node<T, D>& node<T, D>::operator = (std::initializer_list<T> list)
{
  dyn_array<T> buf(list);
  *this = buf;
  return *this;
}

template <class T, bool D>
node<T, D>& node<T, D>::operator = (const dyn_array<T>& source)
{
  if (source.is_empty())
    return *this;
  node<T, D>* head = this->clear();
  head->val_ = source[0];
  if (source.size() > 1) {
    node<T, D>* tail = node<T, D>::create(source.last());
    for (int i = static_cast<int>(source.size()-2); i > 0; --i)
      tail = tail->prepend(source[i]);
    head->append_node(tail);
  }
  return *head;
}

template <class T, bool D>
node<T, D>* node<T, D>::min(T& min_val)
{
  node<T, D>* node_cur = this->find_head();
  node<T, D>* node_prev = nullptr;
  node<T, D>* min_prec = nullptr;
  min_val = node_cur->val();
  while (node_cur->next()) {
    node_prev = node_cur;
    node_cur = node_cur->next();
    if (min_val > node_cur->val()) {
      min_val = node_cur->val();
      min_prec = node_prev;
    }
  }
  return min_prec;
}

template <class T, bool D>
node<T, D>* node<T, D>::max(T& max_val)
{
  node<T, D>* node_cur = this->find_head();
  node<T, D>* node_prev = nullptr;
  node<T, D>* max_prec = nullptr;
  max_val = node_cur->val();
  while (node_cur->next()) {
    node_prev = node_cur;
    node_cur = node_cur->next();
    if (max_val < node_cur->val()) {
      max_val = node_cur->val();
      max_prec = node_prev;
    }
  }
  return max_prec;
}

template <class T, bool D>
node<T, D>* node<T, D>::find_head()
{
  if (!D)
    return this;
  node<T, D>* head = this;
  while (head->prev())
    head = head->prev();
  return head;
}

template <class T, bool D>
const node<T, D>* node<T, D>::find_head() const
{
  if (!D)
    return this;
  const node<T, D>* head = this;
  while (head->prev())
    head = head->prev();
  return head;
}

template <class T, bool D>
node<T, D>* node<T, D>::find_tail()
{
  node<T, D>* tail = this;
  while (tail->next())
    tail = tail->next();
  return tail;
}

template <class T, bool D>
const node<T, D>* node<T, D>::find_tail() const
{
  const node<T, D>* tail = this;
  while (tail->next())
    tail = tail->next();
  return tail;
}

template <class T, bool D>
node<T, D>* node<T, D>::search(const T& val)
{
  node<T, D>* node_cur = this->find_head();
  while (node_cur) {
    if (node_cur->val() == val)
      return node_cur;
    node_cur = node_cur->next();
  }
  return nullptr;
}

template <class T, bool D>
node<T, D>* node<T, D>::contains(const node<T, D>* query)
{
  node<T, D>* node_cur = this->find_head();
  if (node_cur == query)
    return nullptr;
  node<T, D>* node_prev = nullptr;
  while (node_cur->next()) {
    node_prev = node_cur;
    node_cur = node_cur->next();
    if (node_cur == query)
      return node_prev;
  }
  return nullptr;
}

template <class T, bool D>
node<T, D>* node<T, D>::contains(const T& val)
{
  node<T, D>* node_cur = this->find_head();
  if (node_cur->val() == val)
    return nullptr;
  node<T, D>* node_prev = nullptr;
  while (node_cur->next()) {
    node_prev = node_cur;
    node_cur = node_cur->next();
    if (node_cur->val() == val)
      return node_prev;
  }
  return nullptr;
}

template <class T, bool D>
node<T, D>* node<T, D>::append(const T& val)
{
  // Find the tail and append
  node<T, D>* node_new = node<T, D>::create(val);
  node<T, D>* node_cur = this;
  while (node_cur->next())
    node_cur = node_cur->next();
  node_cur->next() = node_new;
  if (D)
    node_new->prev() = node_cur;
  return node_new;
}

template <class T, bool D>
node<T, D>* node<T, D>::prepend(const T& val)
{
  // Find the head.
  node<T, D>* head = this->find_head();
  // Create and link the new head.
  node<T, D>* new_head = node<T, D>::create(val);
  new_head->next() = head;
  if (D)
    head->prev() = new_head;
  // Return pointer to new head.
  return new_head;
}

template <class T, bool D>
node<T, D>* node<T, D>::append_node(node<T, D>* n_ptr)
{
  if (n_ptr == nullptr)
    return this;
  // Find the tail and append
  node<T, D>* node_cur = this;
  while (node_cur->next())
    node_cur = node_cur->next();
  node_cur->next() = n_ptr;
  if (D)
    n_ptr->prev() = node_cur;
  return this;
}

template <class T, bool D>
node<T, D>* node<T, D>::prepend_node(node<T, D>* n_ptr)
{
  if (n_ptr == nullptr)
    return this;
  // Find the head.
  node<T, D>* head = this->find_head();
  // Link the new head.
  n_ptr->next() = head;
  if (D)
    head->prev() = n_ptr;
  // Return pointer to new head, primarily for chaining since it's just n_ptr.
  return n_ptr;
}

template <class T, bool D>
node<T, D>* node<T, D>::delete_head()
{
  node<T, D>* head = this->find_head();
  node<T, D>* new_head = head->next();
  if (D)
    new_head->prev() = nullptr;
  head->next() = nullptr;
  delete head;
  return new_head;
}

template <class T, bool D>
node<T, D>* node<T, D>::delete_tail()
{
  node<T, D>* tail = this;
  node<T, D>* prev = nullptr;
  while (tail->next()) {
    prev = tail;
    tail = tail->next();
  }
  prev->next() = nullptr;
  if (D)
    tail->prev() = nullptr;
  delete tail;
  return prev;
}

template <class T, bool D>
node<T, D>* node<T, D>::delete_node(const T& val)
{
  node<T, D>* node_del = nullptr;
  // If head node is to be deleted...
  if (this->val_ == val) {
    // ...and it's the only node, delete it and return nullptr
    if (this->next() == nullptr) {
      delete this;
      return nullptr;
    }
    // Otherwise unlink this node, delete it and return a pointer to next node.
    node<T, D>* ret = this->next();
    if (D)
      ret->prev() = nullptr;
    this->next() = nullptr;
    delete this;
    return ret;
  }
  // If deletion target is not the head node, traverse to find the target
  node<T, D>* node_cur = this;
  while (node_cur->next()) {
    // When it's found, unlink and delete, then update remaining pointers
    if (node_cur->next()->val() == val) {
      node_del = node_cur->next();
      node_cur->next() = node_cur->next()->next();
      node_del->next() = nullptr;
      if (D) {
        if (node_cur->next())
          node_cur->next()->prev() = node_cur;
        node_del->prev() = nullptr;
      }
      break;
    }
    node_cur = node_cur->next();
  }
  if (node_del)
    delete node_del;
  else
    std::cout << "Warning: Deletion target not found, skipping." << std::endl;
  return this;
}

template <class T, bool D>
node<T, D>* node<T, D>::swap(node<T, D>* other)
{
  if (other == nullptr || other == this)
    return this;
  node<T, D>* buf;
  // Singly-linked nodes can't swap as they can't update pointers to them.
  if (!D) {
    // They swap only their values instead.
    T buf = this->val_;
    this->val_ = other->val_;
    other->val_ = buf;
    return this;
  }
  // Doubly linked lists can swap nodes and update all related links.
  if (this->next() == other) {
    // Case 1: this -> other (adjacent)
    buf = this->prev();
    if (buf)
      buf->next() = other;
    other->prev() = buf;
    buf = other->next();
    this->next() = buf;
    if (buf)
      buf->prev() = this;
    other->next() = this;
    this->prev() = other;
  }
  else if (other->next() == this) {
    // Case 2: other -> this (adjacent)
    buf = other->prev();
    if (buf)
      buf->next() = this;
    this->prev() = buf;
    buf = this->next();
    other->next() = buf;
    if (buf)
      buf->prev() = other;
    this->next() = other;
    other->prev() = this;
  }
  else {
    // Case 3: Non-adjacent
    buf = this->next();
    this->next() = other->next();
    other->next() = buf;
    buf = this->prev();
    this->prev() = other->prev();
    other->prev() = buf;
    if (this->next())
      this->next()->prev() = this;
    if (this->prev())
      this->prev()->next() = this;
    if (other->next())
      other->next()->prev() = other;
    if (other->prev())
      other->prev()->next() = other;
  }
  return this;
}

template <class T, bool D>
node<T, D>* node<T, D>::reverse()
{
  node<T, D>* node_cur = this->find_head();
  node<T, D>* node_prev = nullptr;
  node<T, D>* node_next = nullptr;
  while (node_cur) {
    node_next = node_cur->next();
    node_cur->next() = node_prev;
    if (D)
      node_cur->prev() = node_next;
    node_prev = node_cur;
    node_cur = node_next;
  }
  return node_prev;
}

template <class T, bool D>
node<T, D>* node<T, D>::bubble_sort(bool inc)
{
  node<T, D>* head = this->find_head();
  if (!head->next())
    return head;
  // Flag for early termination when no swaps are made during traversal.
  bool swapped = true;
  node<T, D> *node_cur, *node_prev, *node_next;
  node<T, D> *node_last = nullptr;
  // Traverse and build a sorted linked list until no swaps are needed.
  while (swapped) {
    swapped = false;
    // Reset node_cur to new head and node_prev to nullptr for a new traversal.
    node_cur = head;
    node_prev = nullptr;
    // Traverse until node_cur->next() reaches node_last.
    while (node_cur && node_cur->next() != node_last) {
      if (!node_cur->compare(node_cur->next(), inc)) {
        node_next = node_cur->next();
        // If there was a previous node, update its pointer.
        if (node_prev)
          node_prev->next() = node_next;
        // If head node is to be swapped, update head to be able to re-traverse.
        else
          head = node_next;
        // Update pointers from current node.
        node_cur->next() = node_cur->next()->next();
        if (D) {
          node_cur->prev() = node_next;
          if (node_cur->next())
            node_cur->next()->prev() = node_cur;
        }
        // Update pointers from next node.
        node_next->next() = node_cur;
        if (D)
          node_next->prev() = node_prev;
        // For next iteration, node_next becomes node_prev.
        node_prev = node_next;
        swapped = true;
      }
      else {
        // If no swap is needed, move onto the next node.
        node_prev = node_cur;
        node_cur = node_cur->next();
      }
    }
    // Once inner loop is done, node_cur is the new node_last.
    node_last = node_cur;
  }
  return head;
}

template <class T, bool D>
node<T, D>* node<T, D>::insertion_sort(bool inc)
{
  node<T, D>* head = this->find_head();
  if (!head->next())
    return head;
  // Initialize sorted linked list with the head node
  node<T, D>* node_next = head->next();
  // Detach head node from the chain.
  head->next() = nullptr;
  if (D)
    node_next->prev() = nullptr;
  node<T, D>* node_cur = node_next;
  node<T, D> *node_prev, *node_ins;
  // Traverse until the end of the linked list.
  while (node_cur) {
    // Save next node.
    node_next = node_cur->next();
    // Detach current node.
    node_cur->next() = nullptr;
    if (D && node_next)
      node_next->prev() = nullptr;
    // Find insertion location for the detached node.
    node_ins = head;
    node_prev = nullptr;
    while (node_ins && node_ins->compare(node_cur, inc)) {
      node_prev = node_ins;
      node_ins = node_ins->next();
    }
    // Update head pointer if insertion location is the beginning.
    if (node_ins == head)
      head = node_cur;
    // Update relevant links to perform insertion into the sorted linked list.
    if (node_ins)
      node_cur->next() = node_ins;
    if (node_prev)
      node_prev->next() = node_cur;
    if (D) {
      node_cur->prev() = node_prev;
      if (node_cur->next())
        node_cur->next()->prev() = node_cur;
    }
    node_cur = node_next;
  }
  return head;
}

template <class T, bool D>
node<T, D>* node<T, D>::merge_sort(bool inc) {
  node<T, D>* head = this->find_head();
  if (!head || !head->next())
    return head;
  // O(N) traverse once to get the size.
  size_t len = head->size();
  if (len <= 1)
    return head;
  node<T, D>* cur_head = head;
  size_t sublist_size = 1;
  while (sublist_size < len) {
    node<T, D>* pass_head = nullptr;
    node<T, D>* pass_tail = nullptr;
    node<T, D>* curr = cur_head;
    while (curr) {
      // Left sublist start and end.
      node<T, D>* left_start = curr;
      node<T, D>* left_end = left_start;
      size_t left_steps = 0;
      while (left_steps < sublist_size-1 && left_end->next()) {
        left_end = left_end->next();
        ++left_steps;
      }
      node<T, D>* right_start = left_end->next();
      if (!right_start) {
        // Partial left at end: attach as-is.
        if (!pass_head) {
          pass_head = left_start;
          pass_tail = left_end;
        }
        else {
          pass_tail->next() = left_start;
          if (D)
            left_start->prev() = pass_tail;
          pass_tail = left_end;
        }
        // End of list.
        break;
      }
      // Split left.
      node<T, D>* old_next = left_end->next();
      left_end->next() = nullptr;
      if (D && old_next) old_next->prev() = nullptr;
      right_start = old_next;
      // Right sublist end.
      node<T, D>* right_end = right_start;
      size_t right_steps = 0;
      while (right_steps < sublist_size-1 && right_end->next()) {
        right_end = right_end->next();
        ++right_steps;
      }
      // Split right.
      old_next = right_end ? right_end->next() : nullptr;
      if (right_end) {
        right_end->next() = nullptr;
        if (D && old_next)
          old_next->prev() = nullptr;
      }
      // Merge.
      node<T, D>* merged = left_start->merge(inc, left_start, right_start);
      node<T, D>* merged_tail = merged->find_tail();
      // Attach to pass chain.
      if (!pass_head) {
        pass_head = merged;
        pass_tail = merged_tail;
      }
      else {
        pass_tail->next() = merged;
        if (D)
          merged->prev() = pass_tail;
        pass_tail = merged_tail;
      }
      // Next pair.
      curr = old_next;
    }
    cur_head = pass_head;
    sublist_size *= 2;
  }
  return cur_head;
}

template <class T, bool D>
node<T, D>* node<T, D>::merge(bool inc, node<T, D>* low_ptr, node<T, D>* high_ptr)
{
  if (!low_ptr && !high_ptr) {
    return nullptr;
  }
  else if (!low_ptr) {
    if (D)
      high_ptr->prev() = nullptr;
    return high_ptr;
  }
  else if (!high_ptr) {
    if (D)
      low_ptr->prev() = nullptr;
    return low_ptr;
  }
  // Initialize the node that will become the new head and returned.
  node<T, D>* head_ptr;
  if (low_ptr->compare(high_ptr, inc)) {
    head_ptr = low_ptr;
    low_ptr = low_ptr->next();
  }
  else {
    head_ptr = high_ptr;
    high_ptr = high_ptr->next();
  }
  if (D)
    head_ptr->prev() = nullptr;
  // Initialize another pointer to track the merging.
  node<T, D>* aux_ptr = head_ptr;
  // Merge the remaining nodes.
  while (low_ptr && high_ptr) {
    if (low_ptr->compare(high_ptr, inc)) {
      aux_ptr->next() = low_ptr;
      if (D)
        low_ptr->prev() = aux_ptr;
      low_ptr = low_ptr->next();
    }
    else {
      aux_ptr->next() = high_ptr;
      if (D)
        high_ptr->prev() = aux_ptr;
      high_ptr = high_ptr->next();
    }
    aux_ptr = aux_ptr->next();
  }
  // Attach the remaining nodes. Leftover high nodes also need to be attached,
  // unlike dyn_array version of merge_sort.
  node<T, D>* rem_ptr = low_ptr ? low_ptr : high_ptr;
  aux_ptr->next() = rem_ptr;
  if (D && rem_ptr)
    rem_ptr->prev() = aux_ptr;
  return head_ptr;
}

template <class T, bool D>
node<T, D>* node<T, D>::selection_sort(bool inc)
{
  node<T, D>* head = this->find_head();
  if (!head->next())
    return head;
  node<T, D> *node_prev, *node_cur;
  node<T, D>* head_sorted = nullptr;
  T buf;
  while (head) {
    node_prev = inc ? head->max(buf) : head->min(buf);
    // If head will be detached, update head pointer.
    if (node_prev == nullptr) {
      node_cur = head;
      head = node_cur->next();
    // Otherwise link previous and next nodes.
    }
    else {
      node_cur = node_prev->next();
      node_prev->next() = node_cur->next();
    }
    // Detach the min/max node from the unsorted chain.
    if (D) {
      node_cur->prev() = nullptr;
      if (node_cur->next())
        node_cur->next()->prev() = node_prev;
    }
    node_cur->next() = nullptr;
    // Find insertion location.
    T val = node_cur->val();
    if (head_sorted) {
      if (head_sorted->val() == val) {
        // Insert at the end of equal valued nodes to ensure stability.
        node_prev = head_sorted;
        while (node_prev->next() && (node_prev->next()->val() == val))
          node_prev = node_prev->next();

        node_cur->next() = node_prev->next();
        node_prev->next() = node_cur;
        if (D) {
          node_cur->prev() = node_prev;
          if (node_prev->next())
            node_prev->next()->prev() = node_cur;
        }
        continue;
      }
      node_cur->next() = head_sorted;
      if (D)
        head_sorted->prev() = node_cur;
    }
    // Update sorted chain head.
    head_sorted = node_cur;
  }
  return head_sorted;
}

template <class T, bool D>
node<T, D>::node(const node<T, D>& source)
{
  this->val_ = source.val_;
  if (source.next()) {
    this->next() = source.next()->clone();
    if (D)
      this->next()->prev() = this;
  }
}

template <class T, bool D>
bool node<T, D>::compare(const node<T, D>* other, bool inc)
{
  return inc ? this->val() <= other->val() : this->val() >= other->val();
}

template <class T, bool D>
void node<T, D>::print_prev(std::ostream& os) const
{
  if (!D)
    return;
  if (this->prev())
    this->prev()->print_prev(os);
  os << this->val() << " <- ";
  return;
}

template <class T, bool D>
std::ostream& operator << (std::ostream& os, const node<T, D>* n_ptr)
{
  if (n_ptr) {
    const node<T, D>* n_cur = n_ptr;
    n_cur = n_cur->find_head();
    while (n_cur) {
      os << n_cur->val();
      if (n_cur->next()) {
        if (D)
          os << " <-> ";
        else
          os << " -> ";
      }
      n_cur = n_cur->next();
    }
  }
  return os;
}

template <class T, bool D>
std::ostream& operator << (std::ostream& os, const node<T, D>& n_ins)
{
  const node<T, D>* n_cur = &n_ins;
  while (n_cur) {
    os << n_cur->val();
    if (n_cur->next())
      os << " ";
    n_cur = n_cur->next();
  }
  return os;
}

//======================================================================//
//                                                                      //
//                      linked_list<T, D> methods                       //
//                                                                      //
//======================================================================//

template <class T, bool D>
linked_list<T, D>::linked_list(std::initializer_list<T> list)
{
  this->head_ = node<T, D>::create(T());
  *(this->head_) = list;
  this->tail_ = this->head_->find_tail();
}

template <class T, bool D>
linked_list<T, D>::linked_list(const linked_list<T, D>& source)
{
  if (source.is_empty()) {
    this->head_ = nullptr;
    this->tail_ = nullptr;
  }
  else {
    this->head_ = source.head_->clone_node();
    if (source.head_ == source.tail_) {
      this->tail_ = this->head_;
    }
    else {
      node<T, D>* s_ptr = source.head_;
      node<T, D>* t_ptr = this->head_;
      while (s_ptr->next()) {
        t_ptr->next() = s_ptr->next()->clone_node();
        if (D)
          t_ptr->next()->prev() = t_ptr;
        t_ptr = t_ptr->next();
        s_ptr = s_ptr->next();
        if (s_ptr == source.tail_)
          break;
      }
      this->tail_ = t_ptr;
    }
  }
  // If source has head/tail loops, replicate them.
  if (D && source.head_->prev()) {
    node<T, D>* hloop = source.head_->prev();
    node<T, D>* s_ptr = source.head_;
    node<T, D>* t_ptr = this->head_;
    while (s_ptr != hloop) {
      s_ptr = s_ptr->next();
      t_ptr = t_ptr->next();
    }
    // When source ptr reaches intersection, so does this ptr.
    this->head_->prev() = t_ptr;
  }
  if (source.tail_->next()) {
    node<T, D>* tloop = source.tail_->next();
    node<T, D>* s_ptr = source.head_;
    node<T, D>* t_ptr = this->head_;
    while (s_ptr != tloop) {
      s_ptr = s_ptr->next();
      t_ptr = t_ptr->next();
    }
    // When source ptr reaches intersection, so does this ptr.
    this->tail_->next() = t_ptr;
  }
}

template <class T, bool D>
linked_list<T, D>::~linked_list()
{
  // If there is a tail loop, break it prior to deletion
  if (this->tail_)
    this->tail_->next() = nullptr;
  // This will recursively delete the tail node as well
  if(this->head_)
    delete this->head_;
}

template <class T, bool D>
size_t linked_list<T, D>::size() const
{
  size_t ret = 0;
  if (this->is_empty())
    return ret;
  else
    ret++;
  const node<T, D>* n_ptr = this->head_;
  while (n_ptr->next() && n_ptr != this->tail_) {
    n_ptr = n_ptr->next();
    ret++;
  }
  return ret;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::clear()
{
  // If there is a tail loop, break it prior to clearing
  if (this->tail_)
    this->tail_->next() = nullptr;
  // This will recursively delete the tail node as well
  if(this->head_)
    delete this->head_;
  // Nullify and return
  this->head_ = nullptr;
  this->tail_ = nullptr;
  return *this;
}

template <class T, bool D>
bool linked_list<T, D>::operator == (const linked_list<T, D>& other) const
{
  if (this->is_empty() != other.is_empty())
    return false;
  if (this->is_empty() && other.is_empty())
    return true;
  const node<T, D>* t_ptr = this->head_;
  const node<T, D>* o_ptr = other.head_;
  while (t_ptr) {
    if (o_ptr == nullptr)
      return false;
    if (o_ptr->val() != t_ptr->val())
      return false;
    if (t_ptr == this->tail_ || o_ptr == other.tail_)
      break;
    t_ptr = t_ptr->next();
    o_ptr = o_ptr->next();
  }
  if (t_ptr != this->tail_ || o_ptr != other.tail_)
    return false;
  // Check that head/tail loops also match.
  if (D && other.head_->prev()) {
    if (!this->head_->prev())
      return false;
    if (this->head_->prev()->val() != other.head_->prev()->val())
      return false;
  }
  if (other.tail_->next()) {
    if (!this->tail_->next())
      return false;
    if (this->tail_->next()->val() != other.tail_->next()->val())
      return false;
  }
  return true;
}

template <class T, bool D>
bool linked_list<T, D>::operator != (const linked_list<T, D>& other) const
{
  return !(*this == other);
}

template <class T, bool D>
bool linked_list<T, D>::operator < (const linked_list<T, D>& other) const
{
  if (this->head_->val() == other.head_->val())
    return this->size() < other.size();
  return this->head_->val() < other.head_->val();
}

template <class T, bool D>
bool linked_list<T, D>::operator > (const linked_list<T, D>& other) const
{
  if (this->head_->val() == other.head_->val())
    return this->size() > other.size();
  return this->head_->val() > other.head_->val();
}

template <class T, bool D>
bool linked_list<T, D>::operator <= (const linked_list<T, D>& other) const
{
  if (this->head_->val() == other.head_->val())
    return this->size() <= other.size();
  return this->head_->val() <= other.head_->val();
}

template <class T, bool D>
bool linked_list<T, D>::operator >= (const linked_list<T, D>& other) const
{
  if (this->head_->val() == other.head_->val())
    return this->size() >= other.size();
  return this->head_->val() >= other.head_->val();
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::operator = (
    const linked_list<T, D>& source)
{
  if (this == &source)
    return *this;
  this->clear();
  if (source.is_empty())
    return *this;
  this->head_ = source.head_->clone_node();
  if (source.head_ == source.tail_) {
    this->tail_ = this->head_;
  }
  else {
    node<T, D>* s_ptr = source.head_;
    node<T, D>* t_ptr = this->head_;
    while (s_ptr->next()) {
      t_ptr->next() = s_ptr->next()->clone_node();
      if (D)
        t_ptr->next()->prev() = t_ptr;
      t_ptr = t_ptr->next();
      s_ptr = s_ptr->next();
      if (s_ptr == source.tail_)
        break;
    }
    this->tail_ = t_ptr;
  }
  // If source has head/tail loops, replicate them.
  if (D && source.head_->prev()) {
    node<T, D>* hloop = source.head_->prev();
    node<T, D>* s_ptr = source.head_;
    node<T, D>* t_ptr = this->head_;
    while (s_ptr != hloop) {
      s_ptr = s_ptr->next();
      t_ptr = t_ptr->next();
    }
    // When source ptr reaches intersection, so does this ptr.
    this->head_->prev() = t_ptr;
  }
  if (source.tail_->next()) {
    node<T, D>* tloop = source.tail_->next();
    node<T, D>* s_ptr = source.head_;
    node<T, D>* t_ptr = this->head_;
    while (s_ptr != tloop) {
      s_ptr = s_ptr->next();
      t_ptr = t_ptr->next();
    }
    // When source ptr reaches intersection, so does this ptr.
    this->tail_->next() = t_ptr;
  }
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::operator = (
    std::initializer_list<T> list)
{
  this->clear();
  this->head_ = node<T, D>::create(T());
  *(this->head_) = list;
  this->tail_ = this->head_->find_tail();
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::operator = (const node<T, D>& source)
{
  this->clear();
  this->head_ = source.find_head()->clone();
  this->tail_ = this->head_->find_tail();
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::operator = (const dyn_array<T>& source)
{
  this->clear();
  if (source.is_empty())
    return *this;
  this->head_ = node<T, D>::create(T());
  *(this->head_) = source;
  this->tail_ = this->head_->find_tail();
  return *this;
}

template <class T, bool D>
node<T, D>* linked_list<T, D>::min(T& min_val)
{
  // Temporarily break any head/tail loop(s) prior to searching.
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  // Find minimum using node method.
  node<T, D>* ret = this->head_->min(min_val);
  // Re-attach broken loop(s).
  if (D)
    this->head_->prev() = hloop;
  this->tail_->next() = tloop;
  return ret;
}

template <class T, bool D>
node<T, D>* linked_list<T, D>::max(T& max_val)
{
  // Temporarily break any head/tail loop(s) prior to searching.
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  // Find maximum using node method.
  node<T, D>* ret = this->head_->max(max_val);
  // Re-attach broken loop(s).
  if (D)
    this->head_->prev() = hloop;
  this->tail_->next() = tloop;
  return ret;
}

template <class T, bool D>
node<T, D>* linked_list<T, D>::search(const T& val)
{
  if (this->is_empty())
    return nullptr;
  // Temporarily break any head/tail loop(s) prior to searching.
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  // Search using node method.
  node<T, D>* ret = this->head_->search(val);
  // Re-attach broken loop(s).
  if (D)
    this->head_->prev() = hloop;
  this->tail_->next() = tloop;
  return ret;
}

template <class T, bool D>
node<T, D>* linked_list<T, D>::contains(const node<T, D>* query)
{
  if (this->is_empty())
    return nullptr;
  // Temporarily break any head/tail loop(s) prior to searching.
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  // Check using node method.
  node<T, D>* ret = this->head_->contains(query);
  // Re-attach broken loop(s).
  if (D)
    this->head_->prev() = hloop;
  this->tail_->next() = tloop;
  return ret;
}

template <class T, bool D>
node<T, D>* linked_list<T, D>::contains(const T& val)
{
  if (this->is_empty())
    return nullptr;
  // Temporarily break any head/tail loop(s) prior to searching.
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  // Check using node method.
  node<T, D>* ret = this->head_->contains(val);
  // Re-attach broken loop(s).
  if (D)
    this->head_->prev() = hloop;
  this->tail_->next() = tloop;
  return ret;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::append(const T& val)
{
  // Appending to empty linked list sets both head and tail pointers
  // to a newly allocated node containing the input value.
  if (this->is_empty()) {
    this->head_ = node<T, D>::create(val);
    this->tail_ = this->head_;
  }
  // Appending to the tail of a looping linked list is not possible
  else if (this->tail_->next()) {
    std::cerr << "ERROR! Appending to the tail of a linked list with a ";
    std::cerr << "tail loop isn't allowed. Call is ignored." << std::endl;
  }
  // Otherwise start traversal from the tail to append faster.
  // Appending by value creates a new node, so always update tail pointer.
  else {
    this->tail_ = this->tail_->append(val);
  }
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::append_node(node<T, D>* n_ptr)
{
  if (this->is_empty()) {
    this->head_ = n_ptr;
    this->tail_ = n_ptr;
  }
  else if (this->tail_->next()) {
    std::cerr << "ERROR! Appending to the tail of a linked list with a ";
    std::cerr << "tail loop isn't allowed. Call is ignored." << std::endl;
  }
  else {
    this->tail_->append_node(n_ptr);
    if (node<T, D>* loop_tail = this->detect_tail_loop()) {
      this->tail_ = loop_tail;
    }
    else {
      node<T, D>* new_tail = n_ptr;
      while (new_tail->next())
        new_tail = new_tail->next();
      this->tail_ = new_tail;
    }
  }
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::prepend(const T& val)
{
  if (this->is_empty()) {
    this->head_ = node<T, D>::create(val);
    this->tail_ = this->head_;
  }
  else if (D && this->head_->prev()) {
    std::cerr << "ERROR! Appending to the head of a linked list with a ";
    std::cerr << "head loop isn't allowed. Call is ignored." << std::endl;
  }
  else {
    this->head_ = this->head_->prepend(val);
  }
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::prepend_node(node<T, D>* n_ptr)
{
  if (this->is_empty()) {
    this->head_ = n_ptr;
    this->tail_ = n_ptr;
  }
  else if (!D) {
    n_ptr->next() = this->head_;
    this->head_ = n_ptr;
  }
  else if (this->head_->prev()) {
    std::cerr << "ERROR! Appending to the head of a linked list with a ";
    std::cerr << "head loop isn't allowed. Call is ignored." << std::endl;
  }
  else {
    this->head_->prev() = n_ptr;
    if (n_ptr->next() == nullptr)
      n_ptr->next() = this->head_;
    if (node<T, D>* loop_head = this->detect_head_loop()) {
      this->head_ = loop_head;
    }
    else {
      node<T, D>* new_head = this->head_;
      while (new_head->prev() && (new_head->prev()->next() == new_head))
        new_head = new_head->prev();
      this->head_ = new_head;
    }
  }
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::delete_head()
{
  if (this->is_empty()) {
    std::cerr << "ERROR! Cannot delete node from an empty linked list. ";
    std::cerr << "Delete call is ignored." << std::endl;
    return *this;
  }
  // If there's a single node, clear and return.
  if (this->head_ == this->tail_)
    return this->clear();
  // Use node method to delete head and update head_ pointer.
  if (D)
    this->head_->prev() = nullptr;
  this->head_ = this->head_->delete_head();
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::delete_tail()
{
  if (this->is_empty()) {
    std::cerr << "ERROR! Cannot delete node from an empty linked list. ";
    std::cerr << "Delete call is ignored." << std::endl;
    return *this;
  }
  // If there's a single node, clear and return.
  if (this->head_ == this->tail_)
    return this->clear();

  // For singly-linked lists, use node method to delete and update tail.
  if (!D) {
    this->tail_->next() = nullptr;
    this->tail_ = this->head_->delete_tail();
  }
  // For doubly-linked lists, delete tail directly using its prev() pointer.
  else {
    this->tail_->next() = nullptr;
    node<T, D>* new_tail = this->tail_->prev();
    new_tail->next() = nullptr;
    this->tail_->prev() = nullptr;
    delete this->tail_;
    this->tail_ = new_tail;
  }
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::delete_node(const T& val)
{
  if (this->is_empty()) {
    std::cerr << "ERROR! Cannot delete node from an empty linked list. ";
    std::cerr << "Delete call is ignored." << std::endl;
    return *this;
  }
  // If there's a single node, clear and return.
  if (this->head_ == this->tail_)
    return this->clear();
  // Temporarily break any tail loop prior to deletion.
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  // First find the node to be deleted to see if it's intersection/head/tail.
  node<T, D>* node_prev = this->head_->contains(val);
  // If deletion target is head or head loop intersection, nullify head_loop.
  // If head value doesn't match, nullptr indicates val not found.
  bool no_del = false, tail_del = false;
  if (!node_prev) {
    if (this->head_->val() == val) {
      hloop = nullptr;
      // head_ might be the looping intersection of tail_.
      if (this->tail_->prev() == this->head_)
        tloop = nullptr;
      this->head_ = this->head_->delete_head();
    }
    else {
      std::cout << "Warning: Delete target not found, skipping." << std::endl;
    }
    no_del = true;
  }
  else {
    // Deletion target is head loop intersection.
    if (node_prev->next() == hloop)
      hloop = nullptr;
    // Deletion target is tail loop intersection (both can be true).
    if (node_prev->next() == tloop)
      tloop = nullptr;
    // Mark tail_ pointer to be recomputed later if it's the deletion target.
    if (node_prev->next() == this->tail_) {
      tloop = nullptr;
      tail_del = true;
    }
  }
  if (!no_del) {
    node_prev->delete_node(val);
    // If tail is changed, update
    if (tail_del)
      this->tail_ = this->head_->find_tail();
  }
  // Reattach any unbroken head and/or tail loop.
  if (D)
    this->head_->prev() = hloop;
  this->tail_->next() = tloop;
  return *this;
}

template <class T, bool D>
node<T, D>* linked_list<T, D>::detect_head_loop() const
{
  if ((!D) || this->is_empty())
    return nullptr;
  if (!this->tail_->prev())
    return nullptr;
  if (!this->tail_->prev()->prev()) {
    if (this->tail_ == this->tail_->prev())
      return this->tail_;
    else
      return nullptr;
  }
  node<T, D>* slow_ptr = this->tail_;
  node<T, D>* fast_ptr = this->tail_;
  while (slow_ptr->prev() != fast_ptr->prev()->prev()) {
    slow_ptr = slow_ptr->prev();
    fast_ptr = fast_ptr->prev()->prev();
    if ((!slow_ptr->prev()) || (!fast_ptr->prev()))
      return nullptr;
    if (!fast_ptr->prev()->prev())
      return nullptr;
  }
  // A loop will cause two pointers to meet k nodes ahead of intersection
  // node, where k is the length of the non-looping segment at the tail.
  // In this case, reset the slow pointer to the tail node and move each
  // pointer one step at a time to have them meet at the loop intersection.
  // If k=0 first meeting point for the two pointers will be the tail node.
  if (slow_ptr->prev() == this->tail_)
    return slow_ptr;
  slow_ptr = this->tail_;
  fast_ptr = fast_ptr->prev()->prev();
  while (slow_ptr->prev() != fast_ptr->prev()) {
    slow_ptr = slow_ptr->prev();
    fast_ptr = fast_ptr->prev();
  }
  return fast_ptr;
}

template <class T, bool D>
node<T, D>* linked_list<T, D>::detect_tail_loop() const
{
  if (this->is_empty())
    return nullptr;
  if (!this->head_->next())
    return nullptr;
  if (!this->head_->next()->next()) {
    if (this->head_ == this->head_->next())
      return this->head_;
    else
      return nullptr;
  }
  node<T, D>* slow_ptr = this->head_;
  node<T, D>* fast_ptr = this->head_;
  while (slow_ptr->next() != fast_ptr->next()->next()) {
    slow_ptr = slow_ptr->next();
    fast_ptr = fast_ptr->next()->next();
    if ((!slow_ptr->next()) || (!fast_ptr->next()))
      return nullptr;
    if (!fast_ptr->next()->next())
      return nullptr;
  }
  // A loop will cause two pointers to meet k nodes behind the intersection
  // node, where k is the length of the non-looping segment at the head.
  // In this case, reset the slow pointer to the head node and move each
  // pointer one step at a time to have them meet at the loop intersection.
  // If k=0 first meeting point for the two pointers will be the head node.
  if (slow_ptr->next() == this->head_)
    return slow_ptr;
  slow_ptr = this->head_;
  fast_ptr = fast_ptr->next()->next();
  while (slow_ptr->next() != fast_ptr->next()) {
    slow_ptr = slow_ptr->next();
    fast_ptr = fast_ptr->next();
  }
  return fast_ptr;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::break_loops(node<T, D>*& hloop,
                                                  node<T, D>*& tloop)
{
  hloop = nullptr;
  tloop = nullptr;
  // Store head/tail intersection node pointers, if any.
  if (D) {
    hloop = this->head_->prev();
    this->head_->prev() = nullptr;
  }
  tloop = this->tail_->next();
  this->tail_->next() = nullptr;
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::reverse()
{
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  // Reverse from head_ to tail_ using node method.
  this->tail_ = this->head_;
  this->head_ = this->head_->reverse();
  // Swap and re-attach head and tail loops.
  if (D)
    this->head_->prev() = tloop;
  this->tail_->next() = hloop;
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::bubble_sort(bool inc)
{
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  this->head_ = this->head_->bubble_sort(inc);
  this->tail_ = this->head_->find_tail();
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::insertion_sort(bool inc)
{
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  this->head_ = this->head_->insertion_sort(inc);
  this->tail_ = this->head_->find_tail();
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::merge_sort(bool inc)
{
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  this->head_ = this->head_->merge_sort(inc);
  this->tail_ = this->head_->find_tail();
  return *this;
}

template <class T, bool D>
linked_list<T, D>& linked_list<T, D>::selection_sort(bool inc)
{
  node<T, D> *hloop, *tloop;
  this->break_loops(hloop, tloop);
  this->head_ = this->head_->selection_sort(inc);
  this->tail_ = this->head_->find_tail();
  return *this;
}

template <class T, bool D>
std::ostream& operator << (std::ostream& os, const linked_list<T, D>& list)
{
  if (!list.is_empty()) {
    const node<T, D>* n_cur = list.head();
    // If there's a head loop, print the loop once with asterisk indicators
    if (D && (n_cur->prev())) {
      if (list.detect_head_loop()) {
        const node<T, D>* stop = n_cur->prev()->next();
        // If stop is already pointing to head node, linked list is a full loop
        if (n_cur == stop) {
          os << n_cur->val() << "* <-> ";
          n_cur = n_cur->next();
        }
        while (n_cur != stop) {
          os << n_cur->val() << "* <-> ";
          n_cur = n_cur->next();
        }
      }
      else {
        n_cur->prev()->print_prev(os);
      }
    }
    n_cur = list.head();
    // Print until the tail node
    while (n_cur != list.tail()) {
      os << n_cur->val();
      if (D)
        os << " <-> ";
      else
        os << " -> ";
      n_cur = n_cur->next();
    }
    // Print the tail node
    os << n_cur->val();
    // If there's a tail loop, print the loop once with asterisk indicators
    if (n_cur->next()) {
      n_cur = n_cur->next();
      while (n_cur != list.tail()) {
        if (D)
          os << " <-> " << n_cur->val() << "*";
        else
          os << " -> " << n_cur->val() << "*";
        n_cur = n_cur->next();
      }
      // Print the tail node once more
      if (D)
        os << " <-> " << n_cur->val() << "*";
      else
        os << " -> " << n_cur->val() << "*";
    }
  }
  return os;
}