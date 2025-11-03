// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "heap.h"

//======================================================================//
//                                                                      //
//                           heap<T> methods                            //
//                                                                      //
//======================================================================//

template <class T>
heap<T>::heap(std::initializer_list<T> list, bool is_max): is_max_(is_max)
{
  // Assign the input array to the internal array.
  this->arr_ = list;
  // Last non-leaf node is the parent of node at index arr.size()-1.
  size_t i_non = parent(this->arr_.size() - 1);
  // Starting from last non-leaf node going backwards, sift down each element.
  for (int i = static_cast<int>(i_non); i >= 0; --i)
    this->sift_down(i);
}

template <class T>
heap<T>::heap(const dyn_array<T>& source, bool is_max): is_max_(is_max)
{
  // Assign the input array to the internal array.
  this->arr_ = source;
  // Last non-leaf node is the parent of node at index arr.size()-1.
  size_t i_non = parent(this->arr_.size() - 1);
  // Starting from last non-leaf node going backwards, sift down each element.
  for (int i = static_cast<int>(i_non); i >= 0; --i)
    this->sift_down(i);
}

template <class T>
heap<T>& heap<T>::clear()
{
  this->arr_.clear();
  return *this;
}

template <class T>
bool heap<T>::operator == (const heap<T>& other) const
{
  return this->arr_ == other.arr_;
}

template <class T>
bool heap<T>::operator != (const heap<T>& other) const
{
  return !(*this == other);
}

template <class T>
bool heap<T>::operator < (const heap<T>& other) const
{
  return this->arr_ < other.arr_;
}

template <class T>
bool heap<T>::operator > (const heap<T>& other) const
{
  return this->arr_ > other.arr_;
}

template <class T>
bool heap<T>::operator <= (const heap<T>& other) const
{
  return this->arr_ <= other.arr_;
}

template <class T>
bool heap<T>::operator >= (const heap<T>& other) const
{
  return this->arr_ >= other.arr_;
}

template <class T>
heap<T>& heap<T>::operator = (std::initializer_list<T> list)
{
  if (list.size() == 0)
    return *this;
  this->clear();
  dyn_array<T> buf(list);
  *this = buf;
  return *this;
}

template <class T>
heap<T>& heap<T>::operator = (const dyn_array<T>& arr)
{
  this->clear();
  // Assign the input array to the internal array.
  this->arr_ = arr;
  // Last non-leaf node is the parent of node at index arr.size()-1.
  size_t i_non = parent(this->arr_.size() - 1);
  // Starting from last non-leaf node going backwards, sift down each element.
  for (int i = static_cast<int>(i_non); i >= 0; --i)
    this->sift_down(i);
  return *this;
}

template <class T>
template <bool P, bool M>
heap<T>& heap<T>::operator = (heap_tree<T, P, M>* hpt)
{
  this->clear();
  this->is_max_ = M ? true: false;
  // Array-based heap stores node values in level order.
  bt_ptrs<T, P> path = hpt->traverse(LEVEL_ORDER);
  this->arr_ = dyn_array<T>(T(), path.size());
  for (size_t i = 0; i < path.size(); ++i)
    this->arr_[i] = path[i]->val();
  return *this;
}

template <class T>
heap<T>& heap<T>::insert(const T& val)
{
  this->arr_.add(val);
  // Get the index of the inserted element.
  size_t ind = this->arr_.size() - 1;
  // Sift up the inserted element until root node is reached.
  return this->sift_up(ind);
}

template <class T>
T heap<T>::remove()
{
  if (this->is_empty()) {
    std::cerr << "ERROR! Cannot remove from an empty heap." << std::endl;
    return T();
  }
  T ret = this->arr_[0];
  // Remove the last element from the dyn_array
  T last = this->arr_.remove();
  // If heap is empty after removal, last==ret and heap should now be empty.
  if (this->is_empty())
    return ret;
  // If heap isn't empty, assign removed element to root and sift down.
  this->arr_[0] = last;
  // Sift down until index is out of bounds or correct location is found.
  this->sift_down(0);
  return ret;
}

template <class T>
T heap<T>::peek() const
{
  if (this->is_empty()) {
    std::cerr << "ERROR! Cannot peek at an empty heap." << std::endl;
    return T();
  }
  return this->arr_[0];
}

template <class T>
bool heap<T>::compare(size_t i1, size_t i2) const
{
  if (this->is_max_)
    return this->arr_[i1] > this->arr_[i2];
  else
    return this->arr_[i1] < this->arr_[i2];
}

template <class T>
bool heap<T>::validate_index(size_t i, bool verbose) const
{
  if (i < 0) {
    if (verbose){
      std::cerr << "ERROR! Root node has no parents" << std::endl;
      std::cerr << "Returning root index" << std::endl;
    }
    return false;
  }
  if (i >= this->size()) {
    if (verbose) {
      std::cerr << "ERROR! Node index i is not in the heap" << std::endl;
      std::cerr << "Returning root index" << std::endl;
    }
    return false;
  }
  return true;
}

template <class T>
size_t heap<T>::parent(size_t i) const
{
  if (!this->validate_index(i, true))
    return 0;
  return (i - 1) / 2;
}

template <class T>
size_t heap<T>::left(size_t i) const
{
  if (!this->validate_index(i, true))
    return 0;
  return 2*i + 1;
}

template <class T>
size_t heap<T>::right(size_t i) const
{
  if(!this->validate_index(i, true))
    return 0;
  return 2*i + 2;
}

template <class T>
heap<T>& heap<T>::sift_up(size_t i)
{
  // Sift up i-th node until root node or correct location is reached.
  while (this->validate_index(this->parent(i))) {
    // If correct location is found, break.
    if (this->compare(this->parent(i), i))
      break;
    // Otherwise swap the element with its parent.
    this->arr_.swap(i, this->parent(i));
    i = this->parent(i);
  }
  return *this;
}

template <class T>
heap<T>& heap<T>::sift_down(size_t i)
{
  // Sift down i-th node until a leaf node or correct location is reached.
  while (this->validate_index(i)) {
    bool left_valid = this->validate_index(this->left(i));
    bool right_valid = this->validate_index(this->right(i));
    size_t swap_child;
    if (left_valid && right_valid) {
      if (this->compare(this->left(i), this->right(i)))
        swap_child = this->left(i);
      else
        swap_child = this->right(i);
    }
    else {
      // If node has no children anymore, break.
      if (!left_valid && !right_valid)
        break;
      swap_child = (left_valid) ? this->left(i) : this->right(i);
    }
    // If correct location is found, break
    if (this->compare(i, swap_child))
      break;
    // Otherwise swap the inserted element with its child.
    this->arr_.swap(i, swap_child);
    i = swap_child;
  }
  return *this;
}

template <class T>
std::ostream& operator << (std::ostream& os, const heap<T>& hp)
{
  os << hp.arr_;
  return os;
}