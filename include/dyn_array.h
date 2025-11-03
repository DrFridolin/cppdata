#ifndef dyn_array_h_
#define dyn_array_h_

#include <iostream>
#include <cassert>
#include <initializer_list>
#include <type_traits>
#include <random>

template <class T>
class dyn_array;

using ptr = std::true_type;
using val = std::false_type;

template <class U>
using is_ptr = std::is_pointer<U>;
template <class U>
using strip = std::remove_pointer<U>;
template <class U>
using is_int = std::enable_if<std::is_integral<U>::value, int>;
template <class U>
using is_art = std::enable_if<std::is_arithmetic<U>::value, int>;
template <class U>
using is_arr = std::is_same<U, dyn_array<typename U::value_type> >;
template <class U>
using has_int = std::is_integral<typename U::value_type>;
template <class U>
using is_dyn = std::enable_if<is_arr<U>::value && has_int<U>::value, int>;

// Resizable dynamic array. Stores elements in a raw array and dynamically
// manages its size and capacity much like std::vector does: When size reaches
// capacity, dyn_array will increase its size to the next power of 2 by
// allocating a new array of that size, copying over its elements, then
// deleting its old array, for an amortized cost of O(1).
// Offers a wide range of sorting methods, some enabled only for compatible T
// types through "Substitution Failure Is Not An Error" (SFINAE).
// Tracks whether its elements are sorted with a class member that is set to 1
// if array is sorted by one of its methods in increasing order, to 2 if
// sorted in decreasing order, and to 0 if one of its order-disrupting
// non-const methods are called.
// As long as dyn_array is sorted, its search method will run binary search on
// its elements; at all other times it will run linear search.
//
// The most distinctive feature of dyn_array<T> is in its handling of pointer T
// types: All sorting algorithms, relational and equality operators, friend
// ostream << operator and search, min and max methods of dyn_array<T> will
// dereference array elements during execution if T is a pointer type,
// operating on pointed-to values rather than pointer addresses.
// This is achieved by centralizing all comparisons and equality checks under
// five protected tag-dispatch methods, each with its own distinct way of
// dereferencing queries prior to the comparison. Any class method is then able
// to execute runtime type checks on the template argument T using type_traits,
// and dispatch a call to one of these methods, using the type check result as
// a tag argument. By giving up its ability to sort/search its pointer elements
// by their addresses, dyn_array<T> gains the capability to sort/search any set
// of class instances through their pointers if the class is strictly ordered.
// This feature becomes very useful for classes that track their constitutent
// parts by storing their pointers in a dyn_array<T>.
//
// All data structure classes in cppdata accept assignments from dyn_array<T>.
// Most stack-constructable ones also offer a dyn_array<T> conversion
// constructor. As they both represent value sequences, all classes consider
// initializer lists and dyn_array<T> objects as value sequences and handle
// constructions and assignments from them in exactly the same way; though the
// way the sequence data is interpreted differs for each data structure.
// Many classes also utilize dyn_array<T> objects as data-organizing buffers
// and as intermediating links to other classes in cppdata.
//
// Observes the rule of 3. Owns the memory for its underlying raw array.
// Does not individually call delete on each raw array element when destroyed.
// Assumes T is default-constructable with equality and relational operators.
template <class T>
class dyn_array
{
  public:
    typedef T value_type;
    // Default constructor does no allocation. O(1) | O(1)
    dyn_array();
    // Constructor to initialize dyn_array with a single value. O(1) | O(1)
    dyn_array(const T& val);
    // Constructor to initialize dyn_array with n copies of val. O(N) | O(N)
    dyn_array(const T& val, size_t n);
    // Constructor to initialize with an array of n values.
    // If in==nullptr, allocates the memory without initializing. O(N) | O(N)
    dyn_array(const T* in, size_t start, size_t end);
    // Constructs dyn_array from an initializer_list. O(N) | O(N)
    dyn_array(std::initializer_list<T> list);
    // Duplicates source elements, size and capacity. O(N) | O(N)
    dyn_array(const dyn_array<T>& source);
    // Destructor deletes the protected raw array list_. O(N) | O(1)
    ~dyn_array();

    // Read/write access to array elements. O(1) | O(1)
    T& operator [] (size_t i);
    // Read-only access to array elements. O(1) | O(1)
    const T& operator [] (size_t i) const;
    // For n>size: Expands the array to capacity n without increasing its size.
    // For n<size: Shrinks and copies old data without tail. O(N) | O(1)
    dyn_array<T>& realloc(size_t n);
    // For n>size: Pads the array to size n with default T() values.
    // For n<size: Shrinks and copies without tail elements. O(N) | O(N)
    dyn_array<T>& resize(size_t n);

    // Returns true if the raw array member is holding no elements. O(1) | O(1)
    bool is_empty() const {return this->size_ == 0;}
    // Returns the size of the underlying raw array. O(1) | O(1)
    size_t size() const {return this->size_;}
    // Deletes the raw array and sets size and capacity to 0. O(N) | O(1)
    dyn_array<T>& clear();

    // Equality check operators compare all elements.
    // O(1) average, O(N) worst | O(1)
    bool operator == (const dyn_array<T>& other) const;
    bool operator != (const dyn_array<T>& other) const;
    // Relational operators compare sizes first, first element values second.
    // O(1) average, O(N) worst | O(1)
    bool operator < (const dyn_array<T>& other) const;
    bool operator > (const dyn_array<T>& other) const;
    bool operator <= (const dyn_array<T>& other) const;
    bool operator >= (const dyn_array<T>& other) const;
    // Assignment operator for deep copy from source dyn_array. O(N) | O(N)
    dyn_array<T>& operator = (const dyn_array<T>& source);

    // Get a copy of dyn_array's min/max element. O(N) | O(1)
    T min() const;
    T max() const;
    // Get a copy of dyn_array's last element. O(1) | O(1)
    T last() const {return this->list_[this->size_-1];}

    // Adds val to list_, reallocs if needed. O(1) average, O(N) worst | O(1)
    dyn_array<T>& add(const T& val);
    // Appends an array to list_. O(K) average, O(N+K) worst | O(K)
    dyn_array<T>& add(const T* const arr, size_t start, size_t end);
    // Appends another dyn_array. O(K) average, O(N+K) worst | O(K)
    dyn_array<T>& append(const dyn_array<T>& source);
    // Swap the elements at indices i and j. O(1) | O(1)
    dyn_array<T>& swap(size_t i, size_t j);
    // Pops the last element, reducing the size by 1. O(1) | O(1)
    T remove();

    // Linear or binary search, depending on whether array is sorted or not.
    // Chooses between dereferenced and direct comparison based on T type.
    // Returns indices for derefereced/direct values matching query qr.
    // O(K+logN) sorted, O(N) unsorted with O(1) amortized adds | O(K)
    dyn_array<size_t> search(const typename strip<T>::type& qr) const;

    // In-place and stable. O(N^2) | O(1)
    dyn_array<T>& bubble_sort(bool inc = true);
    // In-place and stable. O(N^2) | O(1)
    dyn_array<T>& insertion_sort(bool inc = true);
    // Out-of-place and stable. Uses both auxiliary and input space to sort.
    // O(N*logN) average, O(N^2) worst | O(N)
    dyn_array<T>& merge_sort(bool inc = true);

    // In-place and unstable. O(N^2) | O(1)
    dyn_array<T>& selection_sort(bool inc = true);
    // In-place and unstable. O(N*logN) average, O(N^2) worst | O(logN)
    dyn_array<T>& quick_sort(bool inc = true);
    // In-place and unstable. O(N*logN) | O(1)
    dyn_array<T>& heap_sort(bool inc = true);

    // Out-of-place, stable and non-comparison-based.
    // Restricted to T=integral. O(N+K) | O(N+K)
    template <class U = T, typename is_int<U>::type = 0>
    dyn_array<T> counting_sort(bool inc = true);
    // Out-of-place, stable and non-comparison-based. Sorts by decimal dec.
    // Restricted to T=integral. O(N) | O(N)
    template <class U = T, typename is_int<U>::type = 0>
    dyn_array<T> counting_sort(int dec, bool inc);
    // Out-of-place, stable and non-comparison-based. Sorts by given digit.
    // Restricted to T=dyn_array<integral>. O(N+K) | O(N+K)
    template <class U = T, typename is_dyn<U>::type = 0>
    dyn_array<T> counting_sort(int digit, bool inc = true);
    // Out-of-place, stable and non-comparison-based.
    // Restricted to T=integral. O(D*N) | O(N)
    // Calls counting_sort on each decimal.
    template <class U = T, typename is_int<U>::type = 0>
    dyn_array<T> radix_sort(bool inc = true);
    // Out-of-place, stable and non-comparison-based.
    // Restricted to T=dyn_array<integral>. O(D*(N+B)) | O(N+B)
    // Calls counting_sort on all digits of sequences stored in dyn_arrays.
    template <class U = T, typename is_dyn<U>::type = 0>
    dyn_array<T> radix_sort(bool inc = true);
    // Out-of-place and stable. Uses node<T, false> chains in each bucket.
    // Restricted to T=arithmetic. Achieves better average and worst-case time
    // than the standard version by reversing and merge sorting the linked
    // lists directly. O(N+B*log(N/B)) average, O(N*logN) worst | O(N+B)
    template <class U = T, typename is_art<U>::type = 0>
    dyn_array<T> bucket_sort(bool inc = true, size_t num_buckets = 0);

    // Fisher-Yates in-place shuffle.
    dyn_array<T>& shuffle();
    // Reverses the order of elements in-place. O(N) | O(1)
    dyn_array<T>& reverse();
    // Computes all the permutations of array elements. O(N*N!) | O(N*N!)
    dyn_array<dyn_array<T> > permutations() const;
    // Returns unique values in the array. O(N^2) | O(N)
    dyn_array<T> unique() const;
    // Replaces array with its unique vals, returns their counts. O(N^2) | O(N)
    dyn_array<size_t> unique_cnt();

    // Utility to compute next power of 2 for memory allocations. O(1) | O(1)
    size_t next_pow_2(size_t n) const;

    // Prints dyn_array contents. O(N) | O(1)
    template <class U>
    friend std::ostream& operator << (std::ostream& os,
                                      const dyn_array<U>& al);
  protected:
    // Tag-dispatch methods for dereferenced/direct element comparisons.
    // Choice of the type of comparison is decided based on type of T.
    // They return:
    //   0: If *v1/v1/list_[i] preceding *v2/v2/list_[j] violates sort order.
    //   2: If *v1/v1/list_[i] is equal to *v2/v2/list_[j].
    //   1: Otherwise (no violation or equality).
    int compare(size_t i, size_t j, bool inc) const;
    int compare(const T& v1, const T& v2, bool inc, ptr) const;
    int compare(const T& v1, const T& v2, bool inc, val) const;
    int check(size_t i, const typename strip<T>::type& v, bool inc, ptr) const;
    int check(size_t i, const typename strip<T>::type& v, bool inc, val) const;

    // Sift down i-th element on the subarray [0, j) during heap sort.
    dyn_array<T>& sift_down(size_t i, size_t j, bool inc = true);
    // Recursive function for internal use during quick sort.
    dyn_array<T>& quick_rec(bool inc, int left, int right);
    // Recursive function for internal use during merge sort.
    void merge_rec(bool inc, size_t low, size_t high, T* aux);
    // Recursive private helper to compute array element permutations.
    void perms(dyn_array<dyn_array<T> >& pr, dyn_array<T>& pre) const;

    // Tag-dispatch methods to support the public << ostream overload.
    std::ostream& print_arr(std::ostream& os, ptr) const;
    std::ostream& print_arr(std::ostream& os, val) const;

    // Number of elements dyn_array is holding.
    size_t size_;
    // Total capacity allocated for dyn_array on memory.
    size_t capacity_;
    // Array that holds the data.
    T* list_;
    // 0: unsorted, 1: increasing-order sorted, 2: decreasing-order sorted.
    int sorted_;
};

// Templated class method implementations need to be accessible at the
// point of instantiation.
#include "dyn_array.tpp"

#endif