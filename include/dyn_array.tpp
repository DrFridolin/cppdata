// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "dyn_array.h"
#include "linked_list.h"

//======================================================================//
//                                                                      //
//                         dyn_array<T> methods                         //
//                                                                      //
//======================================================================//

template <class T>
dyn_array<T>::dyn_array()
    : size_(0), capacity_(0), list_(nullptr), sorted_(0)
{
}

template <class T>
dyn_array<T>::dyn_array(const T& val)
    : size_(1), capacity_(1), list_(new T[1]), sorted_(0)
{
  this->list_[0] = val;
}

template <class T>
dyn_array<T>::dyn_array(const T& val, size_t n)
    : size_(n), capacity_(n), list_(n > 0 ? new T[n] : nullptr), sorted_(0)
{
  for (size_t i = 0; i < n; ++i)
    this->list_[i] = val;
}

template <class T>
dyn_array<T>::dyn_array(const T* in, size_t start, size_t end)
    : size_(in && end > start ? end-start : 0),
      capacity_(end > start ? end-start : 0),
      list_(end > start ? new T[end-start] : nullptr),
      sorted_(0)
{
  if (in)
    for (size_t i = start; i < end; ++i)
      this->list_[i-start] = *(in + i);
}

template <class T>
dyn_array<T>::dyn_array(std::initializer_list<T> list)
    : size_(list.size()),
      capacity_(list.size()),
      list_(new T[list.size()]),
      sorted_(0)
{
  // Copy elements from the initializer list
  size_t i = 0;
  for (const T& val : list) {
    this->list_[i++] = val;
  }
}

template <class T>
dyn_array<T>::dyn_array(const dyn_array<T>& source)
{
  // Copies allocate exactly the amount of memory needed to match source size
  this->capacity_ = source.size_;
  this->list_ = new T[this->capacity_];
  for (size_t i = 0; i < source.size_; ++i)
    this->list_[i] = source.list_[i];
  this->size_ = source.size_;
  this->sorted_ = source.sorted_;
}

template <class T>
dyn_array<T>::~dyn_array()
{
  if (this->list_)
    delete[] this->list_;
}

template <class T>
T& dyn_array<T>::operator [] (size_t i)
{
  assert(i >= 0 && i < this->size_ && "dyn_array index out of bounds");
  this->sorted_ = 0;
  return this->list_[i];
}

template <class T>
const T& dyn_array<T>::operator [] (size_t i) const
{
  assert(i >= 0 && i < this->size_ && "dyn_array index out of bounds");
  return this->list_[i];
}

template <class T>
dyn_array<T>& dyn_array<T>::realloc(size_t n)
{
  this->capacity_ = n;
  T* new_list = new T[n];
  // Copy the existing elements up to previous size or n, whichever is smaller
  for (size_t i = 0; (i < this->size_) && (i < n); ++i)
    new_list[i] = this->list_[i];
  // Delete the old array
  delete[] this->list_;
  // Set list_ to point to the new array
  this->list_ = new_list;
  // Adjust size if needed
  if (n < this->size_)
    this->size_ = n;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::resize(size_t n)
{
  this->capacity_ = n;
  T* new_list = new T[n];
  // Copy the existing elements up to previous size or n, whichever is smaller
  for (size_t i = 0; (i < this->size_) && (i < n); ++i)
    new_list[i] = this->list_[i];
  // Delete the old array
  delete[] this->list_;
  // Set list_ to point to the new array
  this->list_ = new_list;
  // Pad if needed and adjust size
  if (this->size_ > n)
    this->size_ = n;
  else
    while (this->size_ < n)
      this->add(T());
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::clear()
{
  if (this->list_)
    delete[] this->list_;
  this->list_ = nullptr;
  this->size_ = 0;
  this->capacity_ = 0;
  this->sorted_ = 0;
  return *this;
}

template <class T>
bool dyn_array<T>::operator == (const dyn_array<T>& other) const
{
  if (this->size_ != other.size_)
    return false;
  for (size_t i = 0; i < this->size_; ++i) {
    if (this->compare(this->list_[i], other.list_[i], true, is_ptr<T>{}) != 2)
      return false;
  }
  return true;
}

template <class T>
bool dyn_array<T>::operator != (const dyn_array<T>& other) const
{
  return !(*this == other);
}

template <class T>
bool dyn_array<T>::operator < (const dyn_array<T>& other) const
{
  // If sizes are equal, compare corresponding digits.
  if (this->size_ == other.size_) {
    if (this->is_empty())
      return false;
    size_t i = 0;
    // Traverse for as long as corresponding digits are equal.
    while(this->compare(this->list_[i], other[i], true, is_ptr<T>{}) == 2)
      if (++i >= this->size_)
        break;
    // If digits are equal until the end, the dyn_arrays are equal.
    if (i == this->size_)
      return false;
    // If a non-equal digit is found, return comparison with inc=true.
    else
      return this->compare(this->list_[i], other[i], true, is_ptr<T>{}) == 1;
  }
  // Otherwise, compare sizes.
  return this->size_ < other.size_;
}

template <class T>
bool dyn_array<T>::operator > (const dyn_array<T>& other) const
{
  // If sizes are equal, compare corresponding digits.
  if (this->size_ == other.size_) {
    if (this->is_empty())
      return false;
    size_t i = 0;
    // Traverse for as long as corresponding digits are equal.
    while(this->compare(this->list_[i], other[i], false, is_ptr<T>{}) == 2)
      if (++i >= this->size_)
        break;
    // If digits are equal until the end, the dyn_arrays are equal.
    if (i == this->size_)
      return false;
    // If a non-equal digit is found, return comparison with inc=false.
    else
      return this->compare(this->list_[i], other[i], false, is_ptr<T>{}) == 1;
  }
  // Otherwise, compare sizes.
  return this->size_ > other.size_;
}

template <class T>
bool dyn_array<T>::operator <= (const dyn_array<T>& other) const
{
  // If sizes are equal, compare corresponding digits.
  if (this->size_ == other.size_) {
    if (this->is_empty())
      return true;
    size_t i = 0;
    // Traverse for as long as corresponding digits are equal.
    while(this->compare(this->list_[i], other[i], true, is_ptr<T>{}) == 2)
      if (++i >= this->size_)
        break;
    // If digits are equal until the end, the dyn_arrays are equal.
    if (i == this->size_)
      return true;
    // If a non-equal digit is found, return comparison with inc=true.
    else
      return this->compare(this->list_[i], other[i], true, is_ptr<T>{});
  }
  // Otherwise, compare sizes.
  return this->size_ <= other.size_;
}

template <class T>
bool dyn_array<T>::operator >= (const dyn_array<T>& other) const
{
  // If sizes are equal, compare corresponding digits.
  if (this->size_ == other.size_) {
    if (this->is_empty())
      return true;
    size_t i = 0;
    // Traverse for as long as corresponding digits are equal.
    while(this->compare(this->list_[i], other[i], false, is_ptr<T>{}) == 2)
      if (++i >= this->size_)
        break;
    // If digits are equal until the end, the dyn_arrays are equal.
    if (i == this->size_)
      return true;
    // If a non-equal digit is found, return comparison with inc=false.
    else
      return this->compare(this->list_[i], other[i], false, is_ptr<T>{});
  }
  // Otherwise, compare sizes.
  return this->size_ >= other.size_;
}

template <class T>
dyn_array<T>& dyn_array<T>::operator = (const dyn_array<T>& source)
{
  if (this != &source) {
    // Only re-allocate if current capacity is insufficient
    if (this->capacity_ < source.size_) {
      if (this->list_)
        delete[] this->list_;
      this->capacity_ = source.size_;
      this->list_ = new T[this->capacity_];
    }
    // Capacity is not updated if it was sufficient to hold all elements
    for (size_t i = 0; i < source.size_; ++i)
      this->list_[i] = source.list_[i];
    this->size_ = source.size_;
    this->sorted_ = source.sorted_;
  }
  return *this;
}

template <class T>
T dyn_array<T>::min() const
{
  if (this->size_ == 0)
    return T();
  size_t i_min = 0;
  for (size_t i = 1; i < this->size_; ++i)
    if (!this->compare(i_min, i, true))
      i_min = i;
  return this->list_[i_min];
}

template <class T>
T dyn_array<T>::max() const
{
  if (this->size_ == 0)
    return T();
  size_t i_max = 0;
  for (size_t i = 1; i < this->size_; ++i)
    if (!this->compare(i_max, i, false))
      i_max = i;
  return this->list_[i_max];
}

template <class T>
dyn_array<T>& dyn_array<T>::add(const T& val)
{
  // Check if list capacity is reached. If full, create a new array with size
  // set to the smallest power of 2 larger than current capacity. This doubles
  // the size of dyn_array if capacity_ is already a power of 2.
  if (this->size_ == this->capacity_)
    this->realloc(this->next_pow_2(this->capacity_));
  // Set the value at the next index to val and increment size
  this->list_[this->size_++] = val;
  this->sorted_ = 0;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::add(const T* const arr, size_t start, size_t end)
{
  if (end <= start)
    return *this;
  size_t n = end - start;
  // Check if new memory allocation is needed
  if (this->size_ + n >= this->capacity_) {
    // If allocation is needed, allocate in powers of 2
    this->capacity_ = this->next_pow_2(this->size_ + n);
    this->realloc(this->capacity_);
  }
  // Append the array values to list_
  for (size_t i = start; i < end; ++i)
    this->list_[this->size_++] = *(arr + i);
  this->sorted_ = 0;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::append(const dyn_array<T>& source)
{
  // Check if new memory allocation is needed
  if (this->size_ + source.size_ > this->capacity_) {
    // If allocation is needed, allocate in powers of 2
    this->capacity_ = this->next_pow_2(this->size_ + source.size_);
    this->realloc(this->capacity_);
  }
  // Append the source dyn_array<T> values to this->list_
  for (size_t i = 0; i < source.size_; ++i)
    this->list_[this->size_++] = source[i];
  this->sorted_ = 0;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::swap(size_t i, size_t j)
{
  T buf = this->list_[i];
  this->list_[i] = this->list_[j];
  this->list_[j] = buf;
  this->sorted_ = 0;
  return *this;
}

template <class T>
T dyn_array<T>::remove()
{
  T ret = this->list_[this->size_ - 1];
  this->list_[this->size_ - 1] = T();
  this->size_--;
  this->sorted_ = 0;
  return ret;
}

template <class T>
dyn_array<size_t> dyn_array<T>::search(const typename strip<T>::type& qr) const
{
  dyn_array<size_t> ret;
  // Run binary search if dyn_array is marked as sorted.
  if (this->sorted_) {
    bool inc = this->sorted_ == 1 ? true : false;
    size_t i_low = 0;
    size_t i_high = this->size_ - 1;
    size_t i;
    while (i_low <= i_high) {
      i = (i_high + i_low) / 2;
      if (this->check(i, qr, inc, is_ptr<T>{}) == 2) {
        while (i > 0 && this->check(i-1, qr, inc, is_ptr<T>{}) == 2)
          i--;
        while (i < this->size_ && this->check(i, qr, inc, is_ptr<T>{}) == 2)
          ret.add(i++);
        return ret;
      }
      if (this->check(i, qr, inc, is_ptr<T>{}))
        i_low = i + 1;
      else
        i_high = i - 1;
    }
  }
  // Run linear search otherwise.
  else {
    for (size_t i = 0; i < this->size_; ++i)
      if (this->check(i, qr, true, is_ptr<T>{}) == 2)
        ret.add(i);
  }
  return ret;
}

template <class T>
dyn_array<T>& dyn_array<T>::bubble_sort(bool inc)
{
  if (this->size_ < 2)
    return *this;
  // Flag for early termination when no swaps are made during traversal.
  bool swapped;
  // Traverse and build the sorted portion on the left.
  for (size_t i = 0; i < this->size_ - 1; ++i) {
    swapped = false;
    // Last element is in correct place so range shrinks after each traversal.
    for (size_t j = 0; j < this->size_ - 1 - i; ++j) {
      if (!this->compare(j, j+1, inc)) {
        this->swap(j, j+1);
        swapped = true;
      }
    }
    if (!swapped)
      break;
  }
  this->sorted_ = inc ? 1 : 2;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::insertion_sort(bool inc)
{
  if (this->size_ < 2)
    return *this;
  // Traverse and build the sorted portion on the left.
  for (size_t i = 1; i < this->size_; ++i)
    // Working backwards, compare to all elements before and swap if needed.
    for (size_t j = i; j > 0 && !this->compare(j-1, j, inc); --j)
      this->swap(j, j-1);
  this->sorted_ = inc ? 1 : 2;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::merge_sort(bool inc)
{
  if (this->size_ < 2)
    return *this;
  // Initialize auxiliary as a raw pointer, using dyn_array's list_ member.
  T* aux = new T[this->size_]; //= *this;
  // Start recursion with the full array contents.
  this->merge_rec(inc, 0, this->size_ - 1, aux);
  this->sorted_ = inc ? 1 : 2;
  delete[] aux;
  return *this;
}

template <class T>
void dyn_array<T>::merge_rec(bool inc, size_t low, size_t high, T* aux)
{
  if (low >= high)
    return;
  size_t mid = (high + low) / 2;
  // Recurse and sort the low half.
  this->merge_rec(inc, low, mid, aux);
  // Recurse and sort the high half.
  this->merge_rec(inc, mid+1, high, aux);
  // Merge the two halves.
  for (size_t i = low; i <= high; ++i)
    aux[i] = this->list_[i];
  size_t i_low = low;
  size_t i_high = mid + 1;
  size_t i_cur = low;
  // Pick from two halves in correct order until one passes its upper bound.
  while (i_low <= mid && i_high <= high) {
    if (this->compare(aux[i_low], aux[i_high], inc, is_ptr<T>{}))
      this->list_[i_cur++] = aux[i_low++];
    else
      this->list_[i_cur++] = aux[i_high++];
  }
  // Copy the remaining elements on the low half, if any.
  for (size_t i = i_low; i <= mid; ++i)
    this->list_[i_cur++] = aux[i_low++];
  return;
}

template <class T>
dyn_array<T>& dyn_array<T>::selection_sort(bool inc)
{
  if (this->size_ < 2)
    return *this;
  size_t next_ind;
  // Traverse and build the sorted portion on the left.
  for (size_t i = 0; i < this->size_ - 1; ++i) {
    // Start with the current element as minimum/maximum.
    next_ind = i;
    // Search the rest of the array for the actual minimum/maximum.
    for (size_t j = i + 1; j < this->size_; ++j)
      if (this->compare(j, next_ind, inc))
        next_ind = j;
    // If a different minimum/maximum was found, swap elements.
    if (next_ind != i)
      this->swap(i, next_ind);
  }
  this->sorted_ = inc ? 1 : 2;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::quick_sort(bool inc)
{
  if (this->size_ < 2)
    return *this;
  // Start recursion with the full array contents.
  this->quick_rec(inc, 0, static_cast<int>(this->size_ - 1));
  this->sorted_ = inc ? 1 : 2;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::quick_rec(bool inc, int left, int right)
{
  if (left > right)
    return *this;
  // Pick pivot and partition the array around pivot.
  T pivot = this->list_[(right + left) / 2];
  int i_left = left;
  int i_right = right;
  // Partitioning ends when left and right indices cross over.
  while (i_left <= i_right) {
    // Find the next out-of-order or equal-to-pivot element on the left.
    while (this->compare(this->list_[i_left], pivot, inc, is_ptr<T>{}) == 1)
      i_left++;
    // Find the next out-of-order or equal-to-pivot element on the right.
    while (this->compare(pivot, this->list_[i_right], inc, is_ptr<T>{}) == 1)
      i_right--;
    // If there's no crossover, swap left and right elements, then update them.
    if (i_left <= i_right)
      this->swap(i_left++, i_right--);
  }
  // Recurse into the left sub-array if it has at least 2 elements to sort.
  if (left < i_left - 1)
    this->quick_rec(inc, left, i_left - 1);
  // Recurse into the right sub-array if it has at least 2 elements to sort.
  if (i_left < right)
    this->quick_rec(inc, i_left, right);
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::heap_sort(bool inc)
{
  if (this->size_ < 2)
    return *this;
  // Last non-leaf node is the parent of the last node.
  size_t i_non = (this->size_ / 2) - 1;
  //From last non-leaf element going backwards, sift down each element.
  for (int i = static_cast<int>(i_non); i >= 0; --i)
    this->sift_down(i, this->size_, !inc);
  // From last element going backwards, swap first and last elements, then
  // sift down the new first element up until the new last element.
  for (size_t j = this->size_-1; j > 0; --j) {
    this->swap(0, j);
    this->sift_down(0, j, !inc);
  }
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::sift_down(size_t i, size_t j, bool inc)
{
  // Sift down element until finding its correct place or going out of bounds.
  while ((i >= 0) && (i < j)) {
    bool left_valid = ((2*i + 1 >= 0) && (2*i + 1 < j)) ? 1 : 0;
    bool right_valid = ((2*i + 2 >= 0) && (2*i + 2 < j)) ? 1 : 0;
    size_t swap_child;
    // If both children exist, pick the one that should precede the other.
    if (left_valid && right_valid)
      swap_child = (this->compare(2*i+1, 2*i+2, inc)) ? (2*i + 1) : (2*i + 2);
    // If node has no children anymore, break.
    else if (!left_valid && !right_valid)
      break;
    // If node has only one child, pick the one that exists.
    else
      swap_child = (left_valid) ? (2*i + 1) : (2*i + 2);
    // If correct location for the element is found, break
    if (this->compare(i, swap_child, inc))
      break;
    // Otherwise swap the inserted element with its child.
    this->swap(i, swap_child);
    // Update the index to the new location of the element being sifted down.
    i = swap_child;
  }
  return *this;
}

template <class T>
template <class U, typename is_int<U>::type>
dyn_array<T> dyn_array<T>::counting_sort(bool inc)
{
  dyn_array<T> ret = *this;
  if (this->size_ < 2)
    return ret;
  // Check array for negative values, return it unmodified if found.
  // Also save the maximum element while traversing.
  T el_max = 0;
  for (size_t i = 0; i < this->size_; ++i) {
    assert(this->list_[i] >= 0 && "dyn_array counting_sort needs pos. values");
    if (el_max < this->list_[i])
      el_max = this->list_[i];
  }
  // No sorting needed on an input array of all zeroes.
  if (el_max == 0)
    return ret;
  // Create counting array with size equal to the maximum element.
  dyn_array<unsigned> cnt(0, static_cast<size_t>(el_max + 1));
  // Traverse again to populate the counting array.
  for (size_t i = 0; i < this->size_; ++i)
    cnt[this->list_[i]]++;
  // Traverse counting array and replace counts with cumulative sums of counts.
  // This is the only place sorting order is taken into account.
  if (inc)
    for (int i = 1; i < static_cast<int>(cnt.size()); ++i)
      cnt[i]+=cnt[i-1];
  else
    for (int i = static_cast<int>(cnt.size())-2; i >= 0; --i)
      cnt[i]+=cnt[i+1];
  // Use cumulative sums to place the elements to their correct locations.
  // Starting placement from the end ensures stability.
  for (int i = static_cast<int>(this->size_-1); i >= 0; --i)
    ret[--cnt[this->list_[i]]] = this->list_[i];
  return ret;
}

template <class T>
template <class U, typename is_int<U>::type>
dyn_array<T> dyn_array<T>::counting_sort(int dec, bool inc)
{
  dyn_array<T> ret = *this;
  if (this->size_ < 2)
    return ret;
  // Counting array always has a size of 10 for decimal sorting.
  dyn_array<unsigned> cnt(0, 10);
  int exp = 1;
  for (int d = 0; d < dec; ++d)
    exp*=10;
  // Traverse to populate the counting array and check for negatives.
  for (size_t i = 0; i < this->size_; ++i) {
    assert(this->list_[i] >= 0 && "dyn_array counting_sort needs pos. values");
    cnt[(this->list_[i] / exp) % 10]++;
  }
  // Traverse counting array and replace counts with cumulative sums of counts.
  // This is the only place sorting order is taken into account.
  if (inc)
    for (int i = 1; i < 10; ++i)
      cnt[i]+=cnt[i-1];
  else
    for (int i = 8; i >= 0; --i)
      cnt[i]+=cnt[i+1];
  // Use cumulative sums to place the elements to their correct locations.
  // Starting placement from the end ensures stability.
  for (int i = static_cast<int>(this->size_-1); i >= 0; --i)
    ret[--cnt[(this->list_[i] / exp) % 10]] = this->list_[i];
  return ret;
}

template <class T>
template <class U, typename is_dyn<U>::type>
dyn_array<T> dyn_array<T>::counting_sort(int digit, bool inc)
{
  dyn_array<T> ret = *this;
  if (this->size_ < 2)
    return ret;
  // Check arrays for negative values at given digit, return them unmodified
  // if found. Also save the maximum element while traversing.
  size_t el_max = 0;
  int j;
  for (size_t i = 0; i < this->size_; ++i) {
    j = static_cast<int>(this->list_[i].size()) - 1 - digit;
    if (j < 0)
      continue;
    assert(this->list_[i][j] >= 0 && "dyn_array counting_sort needs pos vals");
    if (el_max < this->list_[i][j])
      el_max = this->list_[i][j];
  }
  // No sorting needed on an input arrays all with zeroes at the given digit.
  if (el_max == 0)
    return ret;
  // Create counting array with size equal to the maximum element.
  dyn_array<unsigned> cnt(0, static_cast<size_t>(el_max + 1));
  // Traverse again to populate the counting array.
  for (size_t i = 0; i < this->size_; ++i) {
    j = static_cast<int>(this->list_[i].size()) - 1 - digit;
    // Non-existent digits are handled as zeroes.
    if (j < 0)
      cnt[0]++;
    else
      cnt[this->list_[i][j]]++;
  }
  // Traverse counting array and replace counts with cumulative sums of counts.
  // This is the only place sorting order is taken into account.
  if (inc)
    for (int i = 1; i < static_cast<int>(cnt.size()); ++i)
      cnt[i]+=cnt[i-1];
  else
    for (int i = static_cast<int>(cnt.size())-2; i >= 0; --i)
      cnt[i]+=cnt[i+1];
  // Use cumulative sums to place the elements to their correct locations.
  // Starting placement from the end ensures stability.
  for (int i = static_cast<int>(this->size_-1); i >= 0; --i) {
    j = static_cast<int>(this->list_[i].size()) - 1 - digit;
    if (j >= 0)
      ret[--cnt[this->list_[i][j]]] = this->list_[i];
    else
      ret[--cnt[0]] = this->list_[i];
  }
  return ret;
}

template <class T>
template <class U, typename is_int<U>::type>
dyn_array<T> dyn_array<T>::radix_sort(bool inc)
{
  dyn_array<T> ret = *this;
  if (this->size_ < 2)
    return ret;
  // Find the maximum element while traversing.
  T el_max = 0;
  for (size_t i = 0; i < this->size_; ++i)
    if (el_max < this->list_[i])
      el_max = this->list_[i];
  // Compute the number of decimal places in maximum element.
  T buf = el_max;
  size_t dec = 1;
  while (buf >= 10) {
    dec++;
    buf = buf / 10;
  }
  // Call counting sort on each decimal place, from least to most significant.
  for (int d = 0; d < dec; ++d)
    ret = ret.counting_sort(d, inc);
  return ret;
}

template <class T>
template <class U, typename is_dyn<U>::type>
dyn_array<T> dyn_array<T>::radix_sort(bool inc)
{
  dyn_array<T> ret = *this;
  if (this->size_ < 2)
    return ret;
  // Find the maximum number of digits.
  size_t sz_max = 0;
  for (size_t i = 0; i < this->size_; ++i)
    if (sz_max < this->list_[i].size())
      sz_max = this->list_[i].size();
  // Call counting sort on each digit, from least to most significant.
  for (int dgt = 0; dgt < static_cast<int>(sz_max); ++dgt)
    ret = ret.counting_sort(dgt, inc);
  return ret;
}

template <class T>
template <class U, typename is_art<U>::type>
dyn_array<T> dyn_array<T>::bucket_sort(bool inc, size_t num_buckets)
{
  if (this->size_ < 2)
    return *this;
  // Find min and max values of the array for bucket mapping.
  T min_val = this->min();
  T max_val = this->max();
  // Check if min and max are the same. Array is already sorted if they are.
  if (min_val == max_val)
    return *this;
  // Set default number of buckets to array size if not specified
  if (num_buckets == 0)
    num_buckets = this->size_;
  // Create buckets as an array of pointers to singly-linked node chains.
  node<T, false>** buckets = new node<T, false>*[num_buckets]();
  size_t ind;
  // Distribute elements into buckets by handling them as doubles.
  double range = static_cast<double>(max_val - min_val);
  for (size_t i = 0; i < this->size_; ++i) {
    double norm_val = (this->list_[i] - min_val) / range;
    ind = static_cast<size_t>(norm_val * num_buckets);
    // Handle edge cases where the element is equal to max_val
    if (ind == num_buckets)
      ind--;
    // If linked list doesn't exist, create it.
    if (buckets[ind] == nullptr)
      buckets[ind] = node<T, false>::create(this->list_[i]);
    // Otherwise append the value to the head of the linked list.
    else
      buckets[ind] = buckets[ind]->prepend(this->list_[i]);
  }
  dyn_array<T> ret;
  node<T, false>* node_ptr;
  // Traverse the buckets to sort each one before adding elements to ret.
  for (size_t i = 0; i < num_buckets; ++i) {
    ind = inc ? i : (num_buckets - 1 - i);
    if (buckets[ind] == nullptr)
      continue;
    // Run node-based merge_sort on each non-empty bucket in increasing order.
    // Reverse the linked list prior to sorting to produce stable results.
    buckets[ind] = buckets[ind]->reverse()->merge_sort(inc);
    node_ptr = buckets[ind];
    while (node_ptr) {
      ret.add(node_ptr->val());
      node_ptr = node_ptr->delete_head();
    }
    buckets[ind] = nullptr;
  }
  delete[] buckets;
  return ret;
}

template <class T>
dyn_array<T>& dyn_array<T>::shuffle()
{
  // If size is 0 or 1, no shuffling is needed.
  if (this->size_ < 2)
      return *this;
  // Provides a non-deterministic seed.
  std::random_device rd;
  // Mersenne Twister engine for high-quality randomness.
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dis(0, size_ - 1);
  // Fisher-Yates shuffle
  for (size_t i = this->size_-1; i > 0; --i) {
    // Generate a random index from 0 to i.
    size_t j = dis(gen) % (i + 1);
    // Swap elements at indices i and j.
    T buf = list_[i];
    list_[i] = list_[j];
    list_[j] = buf;
  }
  this->sorted_ = 0;
  return *this;
}

template <class T>
dyn_array<T>& dyn_array<T>::reverse()
{
  T swap;
  // Swap elements until the midpoint of the dyn_array.
  for (size_t i = 0; i < this->size_ / 2; ++i) {
    swap = this->list_[i];
    this->list_[i] = this->list_[this->size_-1-i];
    this->list_[this->size_-1-i] = swap;
  }
  return *this;
}

template <class T>
dyn_array<dyn_array<T> > dyn_array<T>::permutations() const
{
  dyn_array<dyn_array<T> > perms;
  if (this->size_ == 0)
    return perms;
  dyn_array<T> prefix;
  this->perms(perms, prefix);
  return perms;
}

template <class T>
void dyn_array<T>::perms(dyn_array<dyn_array<T> >& pr, dyn_array<T>& pre) const
{
  if (this->size_ == 1) {
    pr.add(pre.add((*this)[0]));
    pre.remove();
    return;
  }
  // Add each element to pre before recursing.
  for (size_t i = 0; i < this->size_; ++i) {
    pre.add((*this)[i]);
    // Create a new array from remaining elements.
    dyn_array<T> rem(nullptr, 0, this->size_-1);
    for (size_t j = 0; j < this->size_; ++j) {
      if (j == i)
        continue;
      rem.add((*this)[j]);
    }
    rem.perms(pr, pre);
    pre.remove();
  }
  return;
}

template <class T>
dyn_array<T> dyn_array<T>::unique() const
{
  dyn_array<T> ret;
  for (size_t i = 0; i < this->size_; ++i) {
    T cur = this->list_[i];
    bool already_seen = false;
    for (size_t j = 0; j < ret.size(); ++j) {
      if (cur == ret[j]) {
        already_seen = true;
        break;
      }
    }
    if (!already_seen)
      ret.add(cur);
  }
  return ret;
}

template <class T>
dyn_array<size_t> dyn_array<T>::unique_cnt()
{
  dyn_array<size_t> cnt;
  dyn_array<T> rep;
  for (size_t i = 0; i < this->size_; ++i) {
    T cur = this->list_[i];
    bool already_seen = false;
    for (size_t j = 0; j < rep.size(); ++j) {
      if (cur == rep[j]) {
        already_seen = true;
        cnt[j]++;
      }
    }
    if (!already_seen) {
      rep.add(cur);
      cnt.add(1);
    }
  }
  this->resize(rep.size());
  for (size_t i = 0; i < rep.size(); ++i)
    this->list_[i] = rep[i];
  return cnt;
}

template <class T>
size_t dyn_array<T>::next_pow_2(size_t n) const
{
  if (n <= 1)
    return n == 0 ? 1 : 2;
  size_t ret = n;
  ret |= ret >> 1;
  ret |= ret >> 2;
  ret |= ret >> 4;
  ret |= ret >> 8;
  ret |= ret >> 16;
  ret |= ret >> (sizeof(size_t) * 8 / 2);
  return ret + 1;
}

template <class T>
int dyn_array<T>::compare(size_t i, size_t j, bool inc) const
{
  if (i >= this->size_ || j >= this->size_)
    return false;
  return this->compare(this->list_[i], this->list_[j], inc, is_ptr<T>{});
}

template <class T>
int dyn_array<T>::compare(const T& v1, const T& v2, bool inc, ptr) const
{
  if (!v1 || !v2)
    return v1 == v2;
  if (*v1 == *v2)
    return 2;
  else
    return inc ? *v1 < *v2 : *v1 > *v2;
}

template <class T>
int dyn_array<T>::compare(const T& v1, const T& v2, bool inc, val) const
{
  if (v1 == v2)
    return 2;
  else
    return inc ? v1 < v2 : v1 > v2;
}

template <class T>
int dyn_array<T>::check(size_t i, const typename strip<T>::type& v,
                        bool inc, ptr) const
{
  if (i >= this->size_)
    return false;
  if (*this->list_[i] == v)
    return 2;
  else
    return inc ? *this->list_[i] < v : *this->list_[i] > v;
}

template <class T>
int dyn_array<T>::check(size_t i, const typename strip<T>::type& v,
                        bool inc, val) const
{
  if (i >= this->size_)
    return false;
  if (this->list_[i] == v)
    return 2;
  else
    return inc ? this->list_[i] < v : this->list_[i] > v;
}

template <class T>
std::ostream& dyn_array<T>::print_arr(std::ostream& os, ptr) const
{
  for (size_t i = 0; i < this->size_; ++i)
    os << *(this->list_[i]) << ((i < (this->size_ - 1)) ? " " : "");
  return os;
}

template <class T>
std::ostream& dyn_array<T>::print_arr(std::ostream& os, val) const
{
  for (size_t i = 0; i < this->size_; ++i)
    os << this->list_[i] << ((i < (this->size_ - 1)) ? " " : "");
  return os;
}

template <class T>
std::ostream& operator << (std::ostream& os, const dyn_array<T>& al)
{
  al.print_arr(os, is_ptr<T>{});
  return os;
}
