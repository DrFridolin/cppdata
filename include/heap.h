#ifndef heap_h_
#define heap_h_

#include <iostream>
#include <initializer_list>
#include "dyn_array.h"
#include "binary_tree.h"

template <class T, bool P, bool M> class heap_tree;

// Array-based min/max heap that is more efficient than its tree-based sibling.
// almost across the board, but especially in terms of cache locality, direct
// access speed, execution time and memory overhead.
// Uses a resizable dyn_array<T> for its internal array structure and defers to
// this member for all memory management, its is_empty() and size() methods,
// its ostream << operator and all comparison operators.
// Min/max type is stored as a member, but it provides no methods to change it.
//
// Implements array counterparts of all standard heap operations in a manner
// that is consistent with heap_tree<T, P, M>.
// When constructed from or assigned a dyn_array<T> or initializer list, copies
// values to its internal array, then heapifies it by sifting down each array
// index corresponding to a non-leaf node.
// Efficiently handles assignments to and from heap_tree<T, P, M> by leveraging
// their matching structures.
// Assumes T is default-constructable with equality and relational operators.
template <class T>
class heap
{
  // Long-standing unaddressed MSVC bug in some cases cannot parse partial
  // template specializations in friend declarations. This is one of them.
  friend class heap_tree<T, 0, true>;
  friend class heap_tree<T, 0, false>;
  friend class heap_tree<T, 1, true>;
  friend class heap_tree<T, 1, false>;

  public:
    // Creates an empty heap. O(1) | O(1)
    heap(): is_max_(false) {}
    // Creates an empty heap of given type. O(1) | O(1)
    heap(bool is_max): is_max_(is_max) {}
    // Creates a min heap holding a single value v. O(1) | O(1)
    heap(const T& v): arr_(dyn_array<T>(v)), is_max_(false) {}
    // Creates a heap of given type, holding a single value v. O(1) | O(1)
    heap(const T& v, bool is_max): arr_(dyn_array<T>(v)), is_max_(is_max) {}
    // Assigns list to internal array, then heapifies it. O(N) | O(N)
    heap(std::initializer_list<T> list, bool is_max);
    // Assigns dyn_array to internal array, then heapifies it. O(N) | O(N)
    heap(const dyn_array<T>& source, bool is_max);
    // Destructor executes no additional steps. O(N) | O(1)
    ~heap() {}

    // Returns true if max heap. O(1) | O(1)
    bool is_max() const {return is_max_;}
    // Returns true if internal dyn_array is empty. O(1) | O(1)
    bool is_empty() const {return this->arr_.is_empty();}
    // Returns internal dyn_array size. O(1) | O(1)
    size_t size() const {return this->arr_.size();}
    // Clears the internal dyn_array. O(N) | O(1)
    heap<T>& clear();

    // Equality check operators compare all elements.
    // O(1) average, O(N) worst | O(1)
    bool operator == (const heap<T>& other) const;
    bool operator != (const heap<T>& other) const;
    // Relational operators defer to their dyn_array<T> counterparts.
    // O(1) average, O(N) worst | O(1)
    bool operator < (const heap<T>& other) const;
    bool operator > (const heap<T>& other) const;
    bool operator <= (const heap<T>& other) const;
    bool operator >= (const heap<T>& other) const;
    // Builds dyn_array from init list and assigns it to itself. O(N+M) | O(N)
    heap<T>& operator = (std::initializer_list<T> list);
    // Assigns dyn_array to internal member, then heapifies it. O(N+M) | O(N)
    heap<T>& operator = (const dyn_array<T>& arr);
    // Clears and rebuilds heap from a heap_tree of any type. O(N+M) | O(N)
    template <bool P, bool M> heap<T>& operator = (heap_tree<T, P, M>* hpt);

    // Inserts a new element while preserving heap properties. O(logN) | O(1)
    heap<T>& insert(const T& val);
    // Removes root and returns its value, preserves heapness. O(logN) | O(1)
    T remove();
    // Peek at the min/max value at the root without removing it. O(1) | O(1)
    T peek() const;

    // Prints heap contents. O(N) | O(1)
    template <class U>
    friend std::ostream& operator << (std::ostream& os, const heap<U>& hp);

  private:
    // Use heap type to compare two node values by underlying dyn_array index.
    bool compare(size_t i1, size_t i2) const;
    // Check whether an index is valid or it's out of range of the dyn_array.
    bool validate_index(size_t i, bool verbose = false) const;
    // Compute parent, left child and right child indices on the dyn_array.
    size_t parent(size_t i) const;
    size_t left(size_t i) const;
    size_t right(size_t i) const;
    // Methods to sift up and sift down nodes by index. O(logN) | O(1)
    heap<T>& sift_up(size_t i);
    heap<T>& sift_down(size_t i);

    dyn_array<T> arr_;
    bool is_max_;
};

// Templated class method implementations need to be accessible at the
// point of instantiation.
#include "heap.tpp"

#endif