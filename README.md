# cppdata

<div style="font-weight: bold;">
  A header-only, templated C++11 library of fundamental data structures and their associated algorithms, implemented optimally without any external dependencies.
</div>

&nbsp;

<img src="uml/cppdata_simple.svg" alt="Simplified UML Class Diagram" style="width: 100%; height: auto; max-width: 100%; display: block;">

&nbsp;

The overarching design principle is to distill core data structures into a minimal set of compact and modular classes that deliver optimal performance with near-zero overhead.

Each class prioritizes thin, inspectable abstractions over comprehensive, production-hardened encapsulation to foster structural clarity and modular extensibility.

Class templates and methods are extensively reused for overlapping functionality, as long as such reuse does not degrade the performance bottom-line.

This results in a compositional and highly-polymorphic library architecture, illustrated by the simplified UML class diagram above.

Currently, cppdata implements the following data structures:
- [dyn_array&lt;T&gt;](#dyn_arrayt) : Resizable dynamic array.
- [node&lt;T, D&gt;](#nodet-d) : Self-referential singly-linked (D=false) and doubly-linked (D=true) list nodes.
- [linked_list&lt;T, D&gt;](#linked_listt-d) : Singly-linked (D=false) and doubly-linked (D=true) list with head/tail tracking and loop support.
- [stack&lt;T&gt;](#stackt) : General-purpose stack implemented with a singly-linked list (node&lt;T, false&gt;).
- [queue&lt;T&gt;](#queuet) : General-purpose queue implemented with a doubly-linked list (linked_list&lt;T, true&gt;).
- [graph_node&lt;T&gt;](#graph_nodet) : Self-referential graph nodes for unweighted connected graphs and general trees.
- [graph&lt;T&gt;](#grapht) : Weighted or unweighted graph with directed or undirected edges that tracks its graph_node pointers and adjacency/edge weight matrix in a 1D and 2D dyn_array member respectively.
- [binary_tree&lt;T, P&gt;](#binary_treet-p) : Self-referential binary tree nodes, with (P=true) or without (P=false) parent links.
- [binary_search_tree&lt;T, P&gt;](#binary_search_treet-p) : Self-referential binary search tree nodes, with (P=true) or without (P=false) parent links.
- [avl_tree&lt;T, P&gt;](#avl_treet-p) : Self-referential AVL tree nodes, with (P=true) or without (P=false) parent links.
- [red_black_tree&lt;T&gt;](#red_black_treet) : Self-referential red-black tree nodes.
- [heap_tree&lt;T, P, M&gt;](#heap_treet-p-m) : Self-referential tree-based min (M=false) and max (M=true) heap, with (P=true) or without (P=false) parent links.
- [heap&lt;T&gt;](#heapt) : Array-based min/max heap.
- [trie](#trie) : Self-referential prefix tree nodes.
- [hash_table&lt;K, V&gt;](#hash_tablek-v) : Type-aware hash table implemented with a fixed-size raw array of singly-linked lists (node&lt;T, false&gt;) that automatically picks the best hash function for its key type.

## Table of Contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Installation](#installation)
- [Building and Running Tests](#usage)
- [API Reference](#api-reference)
- [License](#license)

## Overview

All data structure classes (colorized in UML) comprise one or more value-holding elements and impose a well-defined relational pattern among them. These patterns dictate native algorithms and distinct memory management strategies to each class.

<u>Compositional classes</u> (yellow, green and blue in UML) are built out of arrays or other cppdata classes and have a global view of their components. They:
- Own their memory.
- Maintain O(1) direct access to mission-critical components.
- Support stack and heap instantiation with a variety of public constructors selected based on class needs, most common ones being the default constructor (yellow, green, blue), copy constructor (yellow, green), single-value constructor (yellow, green), structure-size constructor (green, blue), list-initialization constructor (yellow, green), and dyn_array conversion constructor (yellow).
- Allow array allocation of their instances since they are default-constructable with a well-defined empty state.
- Provide high degree of operational safety by insulating their internals, exposing only a limited set of accessors and mutators to protected data.

<u>Node-only classes</u> (red and purple in UML) represent self-referential node structures that hold a single value and one or more pointers to other nodes of the same type, collectively forming a distributed chain structure:
- Node chains with an inherent directionality to them (red in UML) can establish a clear acyclic ordering within their structure and assign each node the ownership of its successor elements along this order.
- The nodes are then able to trigger recursive creations and deletions of their downstream chains without risking memory leaks or double deletions, enabling call site management of an entire chain via its "initial node".
- These classes expose no public constructors and are restricted to heap instantiation through their static create and clone methods. This design prevents stack-allocated nodes from being appended to chains, allowing unrestricted recursive cleanups from a single preceding node.
- As a result, heap-only node chains are designed primarily for pointer-based manipulation, with methods that accept and return node pointers accordingly.
- Within this framework, mutators that could alter a calling node's position in the chain always return a pointer to the node now occupying that position, with capture at call site required to maintain downstream access. If position is guaranteed to remain unchanged, the mutator may return a different node if doing so provides operational benefits.
- Since nodes always hold a value by definition, nonexistent nodes are simply null pointers, and a chain becomes empty when its last remaining node returns nullptr upon self-deletion.
- The only self-referential class not built this way is graph_node (purple in UML) which cannot establish a well-defined acyclic ordering and therefore owns no memory and is not restricted to heap instantiation or pointer-based handling.

<u>Utility classes</u> (gray in UML) provide functional support to certain data structure classes, either by isolating necessary template specializations to limit their scope, or by bundling various auxiliary variables and methods for clarity and convenience.

All data structure classes in cppdata (including heap-only ones):
- Are const-correct.
- Observe the "rule of three" if they manage dynamic memory, with matching destructors, copy constructors and assignment operators. Protected copy constructors for heap-only classes prevent stack backdoors, granted access via selective friend declarations.
- Define valid assignments from std::initializer_list and dyn_array (green in UML) to rebuild class instances from value sequences ingested into the class structure.
- Are supported key and value types for hash_table (blue in UML).
- Require their value type T to be default-constructable, strictly-ordered by relational operators and equality-comparable with sufficiently deep checks to make them hashable.
- Implement size() and clear() methods. Classes that have valid empty states also implement an is_empty() method.
- Overload a friend ostream << operator to print/stream their structure, the output of which is also used to hash them as keys.

## Requirements

- A C++11-compliant compiler (GCC 4.8+, Clang 3.3+, MSVC 2013+).
- CMake 3.10+ (for building and running tests, library itself is zero-install for core usage).

## Installation

As a header-only library, cppdata requires no separate build or installation steps beyond integrating its headers into your C++ project. Simply clone the repository and include the desired header files in your source code.

1. Clone the repository:
    ```
    git clone https://github.com/DrFridolin/cppdata.git
    ```

2. Make sure the cloned repo is in your include path. Some standard methods:
    - Compiler flags
    ```
    g++ -std=c++11 -I~/cppdata main.cpp -o main
    ```
    - CMake
    ```
    include_directories(../libs/cppdata/include)
    ```
    - Makefiles
    ```
    CXXFLAGS += -I~/cppdata
    main: main.cpp
      $(CXX) $(CXXFLAGS) -std=c++11 main.cpp -o main
    ```
    - Environment variables
    ```
    export CPATH=~/cppdata:$CPATH
    ```

3. In your C++ source file (e.g., main.cpp), include the necessary headers:
    ```
    #include <iostream>
    #include "dyn_array.h"
    #include "hash_table.h"
    #include "binary_tree.h"

    int main() {
      // Example usage
      dyn_array<int> arr(5, 2);
      std::cout << arr << std::endl;

      hash_table<dyn_array<int>, bool> ht;
      ht.insert(arr);

      arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      std::cout << arr << std::endl;
      ht.insert(arr);

      if (ht.contains(arr))
        std::cout << "dyn_array hashed correctly." << std::endl;

      avl_tree<int, 1>* avl = binary_tree<int, 1>::AVL(0);
      std::cout << avl << std::endl;

      *avl = arr;
      std::cout << avl << std::endl;
      return 0;
    }
    ```

4. Compile your project with C++11 or later.
    ```
    g++ -std=c++11 main.cpp -o main
    clang++ -std=c++11 main.cpp -o main
    cl /std:c++11 main.cpp
    ```

## Building and Running Tests

For detailed usage patterns, method signatures, and exhaustive code snippets covering all available features, please refer to the executables in tests/ directory. These serve both as unit tests and comprehensive usage examples for all classes. Build with CMake:
```
cd cppdata
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

## API Reference

<img src="uml/cppdata.svg" alt="UML Class Diagram" style="width: 100%; height: auto; max-width: 100%; display: block;">

&nbsp;

&nbsp;

### dyn_array&lt;T&gt;

<img src="uml/cppdata_dyn_array.svg" alt="UML Class Diagram for dyn_array" style="width: 100%; height: auto; max-width: 100%; display: block;">

Resizable dynamic array. Stores elements in a raw array member and dynamically manages its size and capacity much like std::vector does: When size reaches capacity, __dyn_array&lt;T&gt;__ will increase its size to the next power of 2 by allocating a new array of that size, copying over its elements, then deleting its old array, for an amortized cost of O(1). Offers a wide range of sorting methods, some enabled only for compatible T types through "Substitution Failure Is Not An Error" (SFINAE). Tracks whether its elements are sorted with a class member that is set to 1 if array is sorted by one of its methods in increasing order, to 2 if sorted in decreasing order, and to 0 if one of its order-disrupting non-const methods are called. As long as __dyn_array&lt;T&gt;__ is sorted, its search method will run binary search on its elements; at all other times it will run linear search.

The most distinctive feature of __dyn_array&lt;T&gt;__ is in its handling of pointer T types: All sorting algorithms, relational and equality operators, friend ostream << operator and search, min and max methods of __dyn_array&lt;T&gt;__ will dereference array elements during execution if T is a pointer type, operating on pointed-to values rather than pointer addresses. This is achieved by centralizing all comparisons and equality checks under five protected tag-dispatch methods, each with its own distinct way of dereferencing queries prior to the comparison. Any class method is then able to execute runtime type checks on its template argument T using type_traits, and dispatch a call to one of these methods, using the type check result as a tag argument. By giving up its ability to sort/search its pointer elements by their addresses, __dyn_array&lt;T&gt;__ gains the capability to sort/search any set of class instances through their pointers as long as the class is strictly ordered. This feature becomes very useful for all classes that keep track of some or more of their constitutent parts by storing their pointers in a __dyn_array&lt;T&gt;__.

All data structure classes in cppdata accept assignments from __dyn_array&lt;T&gt;__ (represented with green "uses" links in UML). Most stack-constructable ones also offer a __dyn_array&lt;T&gt;__ conversion constructor. As they both represent value sequences, all classes consider initializer lists and __dyn_array&lt;T&gt;__ objects as value sequences and handle constructions and assignments from them in exactly the same way; though the way the sequence data is interpreted differs for each data structure. Many classes also utilize __dyn_array&lt;T&gt;__ objects as data-organizing buffers and as intermediating links to other classes in cppdata.

&nbsp;

Notation for __dyn_array&lt;T&gt;__ big-O:
- N: Number of elements stored in the array.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-----:|:-----:|
|`dyn_array()` | Default constructor. Does no allocation. | O(1) | O(1) | O(1) |
|`dyn_array(const T& val)` | Constructor to initialize dyn_array with a single value. | O(1) | O(1) | O(1) |
|`dyn_array(const T& val, size_t n)` | Constructor to initialize dyn_array with n copies of val. | O(N) | O(N) | O(N) |
|`dyn_array(const T* in, size_t start, size_t end)` | Constructor to initialize dyn_array with the sequence of values between the start and end indices of a raw array. If in is nullptr, memory for end-start elements will be allocated but not initialized; so dyn_array capacity will increase, but size will not. Elements can be added to this allocated memory without triggering a resize, but attempting to use accessors/mutators on uninitialized indices will lead to undefined behavior. Useful to pre-allocate dyn_arrays prior to sequential insertions with add/append methods. | O(N) | O(N) | O(N) |
|`dyn_array(std::initializer_list<T> list)` | Construct dyn_array from an initializer list of values. | O(N) | O(N) | O(N) |
|`dyn_array(const dyn_array<T>& source)` | Copy constructor allocates exactly the amount of memory needed to match the size and capacity of the source, then copies over source elements. | O(N) | O(N) | O(N) |
|`~dyn_array()` | Destructor deletes the protected raw array list_. Note that it does not call delete on individual elements of the array. | O(N) | O(N) | O(1) |
|`operator[](size_t i) : T&` | Operator for read/write access to dyn_array elements. | O(1) | O(1) | O(1) |
|`operator[](size_t i) const : const T&` | Operator for read-only access to dyn_array elements. | O(1) | O(1) | O(1) |
|`realloc(size_t n) : dyn_array<T>&` | For n>size, expands the array to capacity n without increasing its size; for n<size, shrinks the array and copies old data without its tail portion. | O(N) | O(N) | O(1)
|`resize(size_t n) : dyn_array<T>&` | For n>size, pads the array to size n with default T() values. For n<size, shrinks the array and copies old data without its tail portion.  | O(N) | O(N) | O(1) |
|`is_empty() const : bool` | Returns true if array is holding no elements. | O(1) | O(1) | O(1) |
|`size() const : size_t` | Returns the number of elements stored in the array. | O(1) | O(1) | O(1) |
|`clear() : dyn_array<T>&` | Deletes the raw array and sets size and capacity to 0. | O(N) | O(N) | O(1) |
|`operator == (const dyn_array<T>& other) const : bool` | Returns true if value sequences are identical. | O(1) | O(N) | O(1) |
|`operator != (const dyn_array<T>& other) const : bool` | Returns true if value sequences are not identical. | O(1) | O(N) | O(1) |
|`operator < (const dyn_array<T>& other) const : bool` | Returns true if size or lowest index value is smaller.  | O(1) | O(N) | O(1) |
|`operator > (const dyn_array<T>& other) const : bool` | Returns true if size or lowest index value is larger.  | O(1) | O(N) | O(1) |
|`operator <= (const dyn_array<T>& other) const : bool` | Returns true if equal or less. | O(1) | O(N) | O(1) |
|`operator >= (const dyn_array<T>& other) const : bool` | Returns true if equal or greater. | O(1) | O(N) | O(1) |
|`operator = (const dyn_array<T>&) : dyn_array<T>&` | Assignment operator performs deep copy from source. | O(N) | O(N) | O(1) |
|`min() const : T` | Returns a copy of the minimum element in the array. | O(N) | O(N) | O(1) |
|`max() const: T` | Returns a copy of the maximum element in the array. | O(N) | O(N) | O(1) |
|`last() const : T` | Returns a copy of the last element in the array. | O(1) | O(1) | O(1) |
|`add(const T& val) : dyn_array<T>&` | Adds val to dyn_array, reallocs if needed. | O(1) | O(N) | O(1) |
|`add(const T* const arr, size_t start, size_t end) : dyn_array<T>&` | Appends raw array segment to dyn_array. | O(K) | O(N+K) | O(K) |
|`append(const dyn_array<T>& source) : dyn_array<T>&` | Appends another dyn_array to this. | O(K) | O(N+K) | O(K) |
|`swap(size_t i, size_t j) : dyn_array<T>&` | Swap dyn_array elements at indices i and j. | O(1) | O(1) | O(1) |
|`remove() : T` | Pops the last element, reducing dyn_array size by 1. | O(1) | O(1) | O(1) |
|`search(const typename strip<T>::type& qr) const : dyn_array<size_t>` | Runs linear or binary search, depending on whether array is sorted or not. Chooses between dereferenced and direct comparison based on T type. Returns indices for derefereced/direct values matching query qr. | O(K+logN) | O(N) | O(K) |
|`bubble_sort(bool inc = true) : dyn_array<T>&` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. In-place and stable. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`insertion_sort(bool inc = true) : dyn_array<T>&` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. In-place and stable. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`merge_sort(bool inc = true) : dyn_array<T>&` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. Out-of-place and stable. Uses both auxiliary and input space to sort. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`selection_sort(bool inc = true) : dyn_array<T>&` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. In-place and unstable. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`quick_sort(bool inc = true) : dyn_array<T>&` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. In-place and unstable. | O(NlogN) | O(N<sup>2</sup>) | O(1) |
|`heap_sort(bool inc = true) : dyn_array<T>&` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. In-place and unstable. | O(NlogN) | O(NlogN) | O(1) |
|`counting_sort(bool inc = true) : dyn_array<T>` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. Out-of-place, stable and non-comparison-based. Restricted to T=integral. | O(N+K) | O(N+K) | O(N+K) |
|`counting_sort(int, bool inc = true) : dyn_array<T>` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. Out-of-place, stable and non-comparison-based. Sorts integers by their decimal place dec. Restricted to T=integral. | O(N) | O(N) | O(N) |
|`counting_sort(int, bool inc = true) : dyn_array<T>` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. Out-of-place, stable and non-comparison-based. Restricted to T=dyn_array<integral>. Sorts arrays by the specified digit. | O(N+K) | O(N+K) | O(N+K) |
|`radix_sort(bool inc = true) : dyn_array<T>` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. Out-of-place, stable and non-comparison-based. Restricted to T=integral. Calls counting_sort on each decimal place. | O(DN) | O(DN) | O(N) |
|`radix_sort(bool inc = true) : dyn_array<T>` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. Out-of-place, stable and non-comparison-based. Restricted to T=dyn_array<integral>. Calls counting_sort on all digits of sequences stored in dyn_arrays. | O(D(N+B)) | O(D(N+B)) | O(N+B) |
|`bucket_sort(bool inc = true, size_t num_buckets = 0) : dyn_array<T>` | Sorts dyn_array in increasing (inc=true) or decreasing (inc=false) order. Out-of-place and stable. Uses node<T, false> chains in each bucket. Restricted to T=arithmetic. Achieves better average and worst-case time than the standard version by reversing and merge sorting the linked lists directly. | O(N+ Blog(N/B)) | O(NlogN) | O(N+B) |
|`shuffle() : dyn_array<T>&` | Fisher-Yates in-place shuffle. |  |  |  |
|`reverse() : dyn_array<T>&` | Reverses the order of elements in-place. | O(N) | O(N) | O(1) |
|`permutations() const : dyn_array<dyn_array<T>>` | Computes all the permutations of array elements. | O(NN!) | O(NN!) | O(NN!) |
|`unique() const : dyn_array<T>` | Returns unique values in the array. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(N) |
|`unique_cnt() : dyn_array<size_t>` | In-place unique that replaces array with its unique values and returns their counts. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(N) |
|`next_pow_2(size_t n) const : size_t` | Utility function to compute next power of 2 for memory allocations. | O(1) | O(1) | O(1) |
|`friend operator << (std::ostream& os, const dyn_array<T>& al)` | Prints dyn_array contents. | O(N) | O(N) | O(1) |

&nbsp;

### node&lt;T, D&gt;

<img src="uml/cppdata_node.svg" alt="UML Class Diagram for node" style="width: 100%; height: auto; max-width: 100%; display: block;">

Node-only class for singly (D=false) and doubly (D=true) linked lists. Base class __node_base&lt;T, D&gt;__ only holds node value and next_ pointer for singly linked lists, and adds an additional prev_ pointer for doubly-linked lists via full specialization of its class template. It establishes both read/write and const access to these pointers via next() and prev() methods, with the latter intended to be called only on doubly-linked nodes.

All other operations are implemented in its child class __node&lt;T, D&gt;__, which is the primary self-referential linked list representation where chains are handled from their head node pointers. These nodes are restricted to heap memory instantiation and are created via the class static create method with an initial value. A node instance owns the memory for the entire chain in front of it. Methods that can change the head node return a pointer to the node occupying the head position at the time of return, which will be lost unless captured at call site. Tail-editing methods return a pointer to tail position in the same way if their head nodes are safe, which eliminates the need for an additional traversal for post processing in some cases. It also allows [linked_list&lt;T, D&gt;](#linked_listt-d) wrapper class to easily update its tail pointer after running a tail-editing node method.

Implements four sorting algorithms that are native to linked lists; algorithms utilizing frequent random access are deemed incompatible and/or suboptimal. The variant of merge_sort in __node&lt;T, D&gt;__ reaches better asymptotic worst-case time complexity than its [dyn_array&lt;T&gt;](#dyn_arrayt) variant, and uses less space by foregoing an auxiliary structure. The use of a fully iterative implementation brings down the space complexity from O(logN) to O(1). Therefore, merge_sort is the optimal way to sort a __node&lt;T, D&gt;__ chain or a [linked_list&lt;T, D&gt;](#linked_listt-d) object.

Loops and self-intersecting linked lists are consigned to [linked_list&lt;T, D&gt;](#linked_listt-d) and are not supported at the node-only level. Using either append_node or prepend_node to link a chain to itself will create such a loop, causing double deletion issues and infinite loops for most traversal attempts. Use [linked_list&lt;T, D&gt;](#linked_listt-d) if you need to represent and handle looping linked lists.

&nbsp;

Notation for __node&lt;T, D&gt;__ big-O:
- N: Total number of nodes in the chain.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-----:|:-----:|
|`static create(const T& val) : node<T, D>*` | Creates a node on heap memory with value val and returns a pointer to it. | O(1) | O(1) | O(1) |
|`~node()` | Destructor triggers recursive destruction of node's downstream chain. | O(N) | O(N) | O(N) |
|`clone() const : node<T, D>*` | Clones node with its linked chain, in both directions if doubly-linked. Returns pointer to the head of the cloned chain. | O(N) | O(N) | O(N) |
|`clone_fwd() const : node<T, D>*` | Clones node with its linked chain in the forward direction only. Returns pointer to this node's clone. | O(N) | O(N) | O(N) |
|`clone_bwd() const : node<T, D>*` | Clones doubly-linked node chain in the backwards direction only. Returns pointer to this node's clone, or nullptr if list is singly-linked. | O(N) | O(N) | O(N) |
|`clone_node() const : node<T, D>*` | Clones only the calling node without any of its linked nodes. | O(1) | O(1) | O(1) |
|`next() : node<T, D>*&` | Returns pointer to next node in the chain. | O(1) | O(1) | O(1) |
|`prev() : node<T, D>*&` | Returns pointer to previous node in the chain. Restricted to D=true. | O(1) | O(1) | O(1) |
|`next() const : const node<T, D>*` | Returns const pointer to next node in the chain. | O(1) | O(1) | O(1) |
|`prev() const : const node<T, D>*` | Returns const pointer to previous node in the chain. Restricted to D=true. | O(1) | O(1) | O(1) |
|`val() const : const T&` | Read-only access to node value. | O(1) | O(1) | O(1) |
|`val() : T&` | Read/write access to node value. | O(1) | O(1) | O(1) |
|`set_val(const T& val) : node<T, D>*` | Sets node's value. | O(1) | O(1) | O(1) |
|`size() : const : size_t` | Returns the number of nodes in the chain downstream from this node. | O(N) | O(N) | O(1) |
|`clear() : node<T, D>*` | Deletes all nodes accessible from head node, then returns pointer to it. | O(N) | O(N) | O(N) |
|`operator == (const node<T, D>& other) const : bool` | Simultaneously scans both node chains and returns true if they represent identical value sequences. | O(1) | O(N) | O(1) |
|`operator != (const node<T, D>& other) const : bool` | Returns true if node chain value sequences are different. | O(1) | O(N) | O(1) |
|`operator < (const node<T, D>& other) const : bool` | Nodes with smaller values are less. If values are equal, node with the shorter chain is less. | O(1) | O(N) | O(1) |
|`operator > (const node<T, D>& other) const : bool` | Nodes with larger values are greater. If values are equal, node with the longer chain is greater. | O(1) | O(N) | O(1) |
|`operator <= (const node<T, D>& other) const : bool` | Returns true if equal or less. | O(1) | O(N) | O(1) |
|`operator >= (const node<T, D>& other) const : bool` | Returns true if equal or greater. | O(1) | O(N) | O(1) |
|`operator = (const node<T, D>& source) : node<T, D>&` | Performs deep copy of the source node chain. | O(N) | O(N) | O(N) |
|`operator = (std::initializer_list<T> list) : node<T, D>&` | Creates a node chain from values in the initializer list. | O(N) | O(N) | O(N) |
|`operator = (const dyn_array<T>& source) : node<T, D>&` | Creates a node chain from values in the dyn_array. | O(N) | O(N) | O(1) |
|`min(T& min_val) : node<T, D>*` | Finds minimum valued node and returns its value and the node preceding it in the chain. | O(N) | O(N) | O(1) |
|`max(T& max_val) : node<T, D>*` | Finds maximum valued node and returns its value and the node preceding it in the chain. | O(N) | O(N) | O(1) |
|`find_head() : node<T, D>*` | Backward traverses and returns pointer to head node for doubly linked lists. Directly returns this pointer for singly-linked lists. | O(N) | O(N) | O(1) |
|`find_head() const : const node<T, D>*` | Backward traverses and returns pointer to head node for doubly linked lists. Directly returns this pointer for singly-linked lists. | O(N) | O(N) | O(1) |
|`find_tail() : node<T, D>*` | Forward traverses and returns pointer to tail node. | O(N) | O(N) | O(1) |
|`find_tail() const : const node<T, D>*` | Forward traverses and returns pointer to tail node. | O(N) | O(N) | O(1) |
|`search(const T& val) : node<T, D>*` | Searches node chain for value and returns found node's pointer or nullptr if none found. | O(N) | O(N) | O(1) |
|`contains(const node<T, D>* query) : node<T, D>*` | If query is in chain, returns pointer to preceding node; returns nullptr otherwise. Also returns nullptr if query is head; caller has to check. | O(N) | O(N) | O(1) |
|`contains(const T& val) : node<T, D>*` | If query is in chain, returns pointer to preceding node; returns nullptr otherwise. Also returns nullptr if query is head; caller has to check. | O(N) | O(N) | O(1) |
|`append(const T& val) : node<T, D>*` | Appends a new node with the given value to chain's tail. Returns new tail since head will be unchanged. | O(N) | O(N) | O(1) |
|`prepend(const T& val) : node<T, D>*` | Prepends a new node with the given value to chain's head. Returns new head, which can be lost unless assigned. | O(1) | O(1) | O(1) |
|`append_node(node<T, D>* n_ptr) : node<T, D>*` | Adds a node and its forward chain to tail, returns head. | O(N) | O(N) | O(1) |
|`prepend_node(node<T, D>* n_ptr) : node<T, D>*` | Adds a node and its backward chain to head, returns pointer to new head. | O(1) | O(1) | O(1) |
|`delete_head() : node<T, D>*` | Detaches and deletes head, returns next as new head. | O(1) | O(1) | O(1) |
|`delete_tail() : node<T, D>*` | Finds and deletes tail, returns the new tail. | O(N) | O(N) | O(1) |
|`delete_node(const T& val) : node<T, D>*` | Deletes first encountered node in forward direction with matching value. Returns head since it can change. | O(N) | O(N) | O(1) |
|`swap(node<T, D>* other) : node<T, D>*` | Swaps nodes if they are doubly-linked, just their values if they are singly-linked. | O(1) | O(1) | O(1) |
|`reverse() : node<T, D>*` | Reverses the order of the node chain, returns new head. | O(N) | O(N) | O(1) |
|`bubble_sort(bool inc = true) : node<T, D>*` | Sorts node chain in increasing (inc=true) or decreasing (inc=false) order. In-place and stable. Returns head. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`insertion_sort(bool inc = true) : node<T, D>*` | Sorts node chain in increasing (inc=true) or decreasing (inc=false) order. In-place and stable. Returns head. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`merge_sort(bool inc = true) : node<T, D>*` | Sorts node chain in increasing (inc=true) or decreasing (inc=false) order. Out-of-place, stable and iterative. | O(NlogN) | O(NlogN) | O(1) |
|`selection_sort(bool inc = true) : node<T, D>*` | Sorts node chain in increasing (inc=true) or decreasing (inc=false) order. In-place and stable. Returns head. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`friend operator << (std::ostream& os, const node<T, D>* n_ptr) : std::ostream&` | Prints node chain contents. | O(N) | O(N) | O(1) |
|`friend operator << (std::ostream& os, const node<T, D>& n_ins) : std::ostream&` | Compactly prints node chain contents. | O(N) | O(N) | O(1) |

&nbsp;

### linked_list&lt;T, D&gt;

<img src="uml/cppdata_linked_list.svg" alt="UML Class Diagram for linked_list" style="width: 100%; height: auto; max-width: 100%; display: block;">

Linked list wrapper class manages a [node&lt;T, D&gt;](#nodet-d) chain by continuously tracking its head and tail nodes, and represents its empty state by nullifying both. Provides a cleaner way to manage and interact with linked lists. Owns the memory for all its nodes and manages it via a single head pointer in the same way that a [node&lt;T, D&gt;](#nodet-d) instance manages the memory of its downstream chain.

Many doubly-linked list operations reach optimal performance only with the continuous availability of O(1) tail access, so they are the de-facto use cases for __linked_list&lt;T, D&gt;__. Singly-linked lists are still supported, with some methods seeing performance benefits from tail access, but their node-only versions are typically preferred if overhead is a concern. Significant number of [node&lt;T, D&gt;](#nodet-d) methods are directly called by __linked_list&lt;T, D&gt;__ when handling overlapping functionality, as long as performance remains optimal without direct tail access.

By tracking both pointers, __linked_list&lt;T, D&gt;__ is able to represent heads that loop back to a node in the chain (doubly-linked only), and tails that loop forward to a node in the chain (possible in both singly- and doubly-linked). This is because head and tail pointers mark actual endpoints of the linked list so that it can be traversed and processed without getting stuck in an infinite loop. The only way to create a looping __linked_list&lt;T, D&gt;__ is to use append_node and prepend_node methods to link nodes or node chains directly; therefore these methods routinely scan the linked list with Floyd's tortoise-and-hare algorithm to account for any loops that may form or break as a result. All traversing methods operate head-to-tail and break all existing loop(s) if necessary, to be reattached post-op if possible. Recursive memory management is also extended to break loops before deletion.

If __linked_list&lt;T, D&gt;__ contains loop(s), head/tail pointers will be linked to the loop intersection(s). The straightforward way to check for loops at either end of a __linked_list&lt;T, D&gt;__ at any given time is to see if its tail has a successor node (indicating tail loop) or if its head has a predecessor node (indicating head loop). Since __linked_list&lt;T, D&gt;__ always keeps track of its head and tail nodes, the expectation is for both head()->prev() and tail()->next() to be nullptr except when loops are present.

&nbsp;

Notation for __linked_list&lt;T, D&gt;__ big-O:
- N: Number of linked list nodes.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-----:|:-----:|
|`linked_list()` | Default constructor initializes head and tail pointers with nullptr to create an empty linked list. | O(1) | O(1) | O(1) |
|`linked_list(const T& val)` | Constructs a linked list with a single node holding the given value. | O(1) | O(1) | O(1) |
|`linked_list(std::initializer_list<T> list)` | Construct a linked list from an initializer list of values. | O(N) | O(N) | O(N) |
|`linked_list(const linked_list<T, D>& source)` | Copy constructor breaks head and tail loops when performing a deep copy of the source. | O(N) | O(N) | O(N) |
|`~linked_list()` | Destructor breaks tail loops and deletes head. | O(N) | O(N) | O(N) |
|`head() : node<T, D>*&` | Read/write access to head node. | O(1) | O(1) | O(1) |
|`head() const : const node<T, D>*&` | Read-only access to head node. | O(1) | O(1) | O(1) |
|`tail() : node<T, D>*&` | Read/write access to tail node. | O(1) | O(1) | O(1) |
|`tail() const : const node<T, D>*&` | Read-only access to tail node. | O(1) | O(1) | O(1) |
|`is_empty() const : bool` | Returns true if both head and tail pointers are nullptr. | O(1) | O(1) | O(1) |
|`size() const : size_t` | Scans head-to-tail and returns node count. | O(N) | O(N) | O(1) |
|`clear() : linked_list<T, D>&` | Breaks tail loops, deletes head and nullifies head and tail pointers. | O(N) | O(N) | O(N) |
|`operator == (const linked_list<T, D>& other) const : bool` | Simultaneously scans both linked lists head-to-tail and returns true if they represent identical value sequences and duplicate the same loop structures.  | O(1) | O(N) | O(1) |
|`operator != (const linked_list<T, D>& other) const : bool` | Returns true if linked list value sequences and/or loop structures are different. | O(1) | O(N) | O(1) |
|`bool operator < (const linked_list<T, D>& other) const : bool` | Uses node<T, D> criteria and operator to run an equivalent comparison from head nodes. | O(1) | O(N) | O(1) |
|`operator > (const linked_list<T, D>& other) const : bool` | Uses node<T, D> criteria and operator to run an equivalent comparison from head nodes. | O(1) | O(N) | O(1) |
|`operator <= (const linked_list<T, D>& other) const : bool` | Uses node<T, D> criteria and operator to run an equivalent comparison from head nodes. | O(1) | O(N) | O(1) |
|`operator >= (const linked_list<T, D>& other) const : bool` | Uses node<T, D> criteria and operator to run an equivalent comparison from head nodes. | O(1) | O(N) | O(1) |
|`operator = (const linked_list<T, D>& source) : linked_list<T, D>&` | Performs deep copy from another linked_list. | O(N) | O(N) | O(N) |
|`operator = (std::initializer_list<T> list) : linked_list<T, D>&` | Creates a linked list from values in the initializer list. | O(N) | O(N) | O(N) |
|`operator = (const node<T, D>& source) : linked_list<T, D>&` | Creates a linked from a node chain. | O(N) | O(N) | O(N) |
|`operator = (const dyn_array<T>& source) : linked_list<T, D>&` | Creates a node chain from values in the dyn_array. | O(N) | O(N) | O(1) |
|`min(T& min_val) : node<T, D>*` | Temporarily breaks loops, runs node<T, D> min() from head node, reattaches the loops and returns the found node's value and pointer to the node preceding it. | O(N) | O(N) | O(1) |
|`max(T& max_val) : node<T, D>*` | Temporarily breaks loops, runs node<T, D> max() from head node, reattaches the loops and returns the found node's value and pointer to the node preceding it. | O(N) | O(N) | O(1) |
|`search(const T& val) : node<T, D>*` | Temporarily breaks loops, forward searches for the query value from head node, then reattaches loops. | O(N) | O(N) | O(1) |
|`contains(const node<T, D>* query) : node<T, D>*` | Temporarily breaks loops, runs the matching node method from head node, then reattaches loops. | O(N) | O(N) | O(1) |
|`contains(const T& val) : node<T, D>*` | Temporarily breaks loops, runs the matching node method from head node, then reattaches loops. | O(N) | O(N) | O(1) |
|`append(const T& val) : linked_list<T, D>&` | Creates and appends new tail to a non-tail-looping linked list. | O(1) | O(1) | O(1) |
|`prepend(const T& val) : linked_list<T, D>&` | Appends new tail by pointer, then checks for a tail loop. | O(1) | O(1) | O(1) |
|`append_node(node<T, D>* n_ptr) : linked_list<T, D>&` | Creates and appends a new head to a non-head-looping linked list. | O(N+M) | O(N+M) | O(1) |
|`prepend_node(node<T, D>* n_ptr) : linked_list<T, D>&` | Appends new head by pointer, then checks for head loop if doubly-linked. | O(1) O(N+M) | O(1) O(N+M) | O(1) |
|`delete_head() : linked_list<T, D>&` | Breaks head loops, deletes head and updates head pointer. | O(1) | O(1) | O(1) |
|`delete_tail() : linked_list<T, D>&` | Breaks tail loops, deletes tail and updates tail pointer. | O(N) O(1) | O(N) O(1) | O(1) |
|`delete_node(const T& val) : linked_list<T, D>&` | Deletes first node found with value matching query. Preserves unbroken loops. | O(N) | O(N) | O(1) |
|`detect_head_loop() const : node<T, D>*` | Runs Floyd's tortoise-and-hare cycle detection algorithm backwards. Return intersection node. Directly returns nullptr for singly-linked lists. | O(N) | O(N) | O(1) |
|`detect_tail_loop() const : node<T, D>*` | Runs Floyd's tortoise-and-hare cycle detection algorithm forward. Return intersection node. | O(N) | O(N) | O(1) |
|`break_loops(node<T, D>*& hloop, node<T, D>* tloop) : linked_list<T, D>&` | Breaks head/tail loop(s), and returns their intersections with hloop and tloop (nullptr for nonexistent loops). | O(1) | O(1) | O(1) |
|`reverse() : linked_list<T, D>&` | Temporarily breaks loops, runs the matching node method head-to-tail, then reattaches doubly-linked list loops.Singly-linked lists cannot have a head loop and their tail loops can't survive the reversal. | O(N) | O(N) | O(1) |
|`bubble_sort(bool inc = true) : linked_list<T, D>&` | Permanently breaks loops, runs the matching node method from head node to sort the linked list in increasing (inc=true) or decreasing (inc=false) order. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`insertion_sort(bool inc = true) : linked_list<T, D>&` | Permanently breaks loops, runs the matching node method from head node to sort the linked list in increasing (inc=true) or decreasing (inc=false) order. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`merge_sort(bool inc = true) : linked_list<T, D>&` | Permanently breaks loops, runs the matching node method from head node to sort the linked list in increasing (inc=true) or decreasing (inc=false) order. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`selection_sort(bool inc = true) : linked_list<T, D>&` | Permanently breaks loops, runs the matching node method from head node to sort the linked list in increasing (inc=true) or decreasing (inc=false) order. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`friend operator << (std::ostream& os, const linked_list<T, D>& list) : std::ostream&` | Prints linked list contents. | O(N) | O(N) | O(1) |

&nbsp;

### stack&lt;T&gt;

<img src="uml/cppdata_stack.svg" alt="UML Class Diagram for stack" style="width: 100%; height: auto; max-width: 100%; display: block;">

General-purpose stack that runs a singly-linked [node&lt;T, false&gt;](#nodet-d) chain under the hood and is mostly a thin wrapper around it that exposes a clean stack interface. Provides O(1) access to last inserted value. Owns its memory and manages it from the node chain head. Keeps track of its own size and represents its empty state with a nullified node member.

&nbsp;

Notation for __stack&lt;T&gt;__ big-O:
- N: Number of queue elements.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-----:|:-----:|
|`stack()` | Default constructor creates an empty stack. | O(1) | O(1) | O(1) |
|`stack(const T& val)` | Initializes stack with a single value. | O(1) | O(1) | O(1) |
|`stack(std::initializer_list<T> list)` | Constructs a stack by directly assigning the initializer list to its node member. | O(N) | O(N) | O(N) |
|`stack(const stack<T>& source)` | Copy constructor performs a deep copy of the underlying node chain. | O(N) | O(N) | O(N) |
|`~stack()` | Destructor deletes the node pointer member, triggering a recursive deletion of stack's node chain. | O(N) | O(N) | O(N) |
|`push(const T& val) : stack<T>&` | Prepends a node to its node chain with the given value. Increases size by 1. | O(1) | O(1) | O(1) |
|`pop() : T` | Calls delete_head() on its node pointer member, then returns its value. Decreases size by 1. | O(1) | O(1) | O(1) |
|`peek() : T` | Returns head node's value without deleting it. Returns default T value if stack is empty. | O(1) | O(1) | O(1) |
|`is_empty() const : bool` | Returns true if node pointer member is set to nullptr. | O(1) | O(1) | O(1) |
|`size() const : size_t` | Returns stack size directly from member used for tracking. | O(1) | O(1) | O(1) |
|`clear() : stack<T>&` | Deletes and nullifies its node pointer member. Sets size to 0. | O(N) | O(N) | O(N) |
|`operator == (const stack<T>& other) const : bool` | Compares node chains directly using node<T, false> operator. | O(1) | O(N) | O(1) |
|`operator != (const stack<T>& other) const : bool` | Compares node chains directly using node<T, false> operator. | O(1) | O(N) | O(1) |
|`operator < (const stack<T>& other) const : bool` | Compares node chains directly using node<T, false> operator. | O(1) | O(N) | O(1) |
|`operator > (const stack<T>& other) const : bool` | Compares node chains directly using node<T, false> operator. | O(1) | O(N) | O(1) |
|`operator <= (const stack<T>& other) const : bool` | Compares node chains directly using node<T, false> operator. | O(1) | O(N) | O(1) |
|`operator >= (const stack<T>& other) const : bool` | Compares node chains directly using node<T, false> operator. | O(1) | O(N) | O(1) |
|`operator = (const stack<T>& source) : stack<T>&` | Performs a deep copy of source with its node chain. | O(N) | O(N) | O(N) |
|`operator = (std::initializer_list<T> list) : stack<T>&` | Clears stack and directly assigns the initializer list to its node member. | O(N) | O(N) | O(N) |
|`operator = (const node<T, false>* source) : stack<T>&` | Clones the source node chain into its member. | O(N) | O(N) | O(N) |
|`operator = (const dyn_array<T>& source) : stack<T>&` | Clears stack and directly assigns dyn_array to its node member. | O(N) | O(N) | O(1) |
|`friend operator << (std::ostream& os, const stack<T>& st) : std::ostream&` | Prints stack contents. | O(N) | O(N) | O(1) |

&nbsp;

### queue&lt;T&gt;

<img src="uml/cppdata_queue.svg" alt="UML Class Diagram for queue" style="width: 100%; height: auto; max-width: 100%; display: block;">

General-purpose queue that runs a doubly-linked [linked_list&lt;T, true&gt;](#linked_listt-d) under the hood for O(1) tail access, and is mostly a thin wrapper around it that exposes a clean queue interface. Provides O(1) access to earliest inserted value, as well as the last inserted value since it comes for free with linked lists and is often useful. Keeps track of its own size and represents its empty state with an empty list_ member. Owns its memory but manages no dynamically allocated memory of its own, deferring instead to compiler-built defaults for intra-class assignments and copy construction, and to its [linked_list&lt;T, true&gt;](#linked_listt-d) member for recursive cleanups.

&nbsp;

Notation for __queue&lt;T&gt;__ big-O:
- N: Number of queue elements.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-----:|:-----:|
|`queue()` | Default constructor creates an empty queue. | O(1) | O(1) | O(1) |
|`queue(const T& val)` | Initializes queue with a single value. | O(1) | O(1) | O(1) |
|`queue(std::initializer_list<T> list)` | Constructs a queue by directly assigning the initializer list to its linked list member. | O(N) | O(N) | O(N) |
|`~queue()` | Destructor executes no additional steps, defers to the linked list to clean up after itself. | O(N) | O(N) | O(N) |
|`enqueue(const T&) : queue<T>&` | Prepends a node to its linked list with the given value. Increases size by 1. | O(1) | O(1) | O(1) |
|`dequeue() : T` | Calls delete_tail() on its linked list member, then returns its value. Decreases size by 1. | O(1) | O(1) | O(1) |
|`peek() : T` | Returns tail node's value without deleting it. Returns default T value if queue is empty. | O(1) | O(1) | O(1) |
|`last() : T` | Returns head node's value without deleting it. Returns default T value if queue is empty. | O(1) | O(1) | O(1) |
|`is_empty() const : bool` | Returns true if linked list member is empty. | O(1) | O(1) | O(1) |
|`size() const : size_t` | Returns queue size directly from member used for tracking. | O(1) | O(1) | O(1) |
|`clear() : queue<T>&` | Calls clear() on its linked list member. Sets size to 0. | O(N) | O(N) | O(N) |
|`operator == (const queue<T>& other) const : bool` | Compares linked lists directly using linked_list<T, true> operator. | O(1) | O(N) | O(1) |
|`operator != (const queue<T>& other) const : bool` | Compares linked lists directly using linked_list<T, true> operator. | O(1) | O(N) | O(1) |
|`operator < (const queue<T>& other) const : bool` | Compares linked lists directly using linked_list<T, true> operator. | O(1) | O(N) | O(1) |
|`operator <= (const queue<T>& other) const : bool` | Compares linked lists directly using linked_list<T, true> operator. | O(1) | O(N) | O(1) |
|`operator >= (const queue<T>& other) const : bool` | Compares linked lists directly using linked_list<T, true> operator. | O(1) | O(N) | O(1) |
|`operator = (std::initializer_list<T> list) : queue<T>&` | Clears queue and directly assigns the initializer list to its linked list member. | O(N) | O(N) | O(N) |
|`operator = (const linked_list<T, true>& source) : queue<T>&` | Assigns the source linked list directly to its member. | O(N) | O(N) | O(N) |
|`operator = (const dyn_array<T>& source) : queue<T>&` | Clears queue and directly assigns dyn_array to its linked list member. | O(N) | O(N) | O(1) |
|`friend operator << (std::ostream& os, const queue<T>& que) : std::ostream&` | Prints queue contents. | O(N) | O(N) | O(1) |

&nbsp;

### graph_node&lt;T&gt;

<img src="uml/cppdata_graph_node.svg" alt="UML Class Diagram for graph_node" style="width: 100%; height: auto; max-width: 100%; display: block;">

Node-only connected graph class where each node holds a value, an index and a [dyn_array&lt;graph_node&lt;T&gt;*&gt;](#dyn_arrayt) to store pointers to its neighboring nodes. This self-referential design is sufficient on its own to structurally and algorithmically represent unweighted connected graphs and general trees. Per-node adjacency list representation is inherently capable of supporting both directed and undirected graphs, and the use of [dyn_array&lt;graph_node&lt;T&gt;*&gt;](#dyn_arrayt) unlocks support for dynamic graphs.

The design intention is for __graph_node&lt;T&gt;__ to also serve as a maximally configurable base class for all current and future self-referential tree and graph classes. To this end, it is minimally restrictive and makes almost no assumptions about its structure, while still providing engines for core functionality to be shared through inheritance. Currently, the entire [binary_tree&lt;T, P&gt;](#binary_treet-p) inheritance hierarchy, as well as the non-templated [trie](#trie) inherit this functionality. Neither owns nor manages any memory, and delegates this responsibility to wrappers and descendants, or to the user if being used directly.

Standard traversals are handled by protected engines that can be customized with simple lambdas that specify target types and early exit conditions. These engines make extensive use of [hash_table&lt;uintptr_t, V&gt;](#hash_tablek-v) and/or a [queue&lt;const graph_node&lt;T&gt;*&gt;](#queuet) to track visited nodes, thereby unlocking full support for cyclic graphs and trees (for hashing, pointers are cast to uintptr_t first).

Two sets of accessors and mutators are provided, one for direct access to adjacency list pointers, and another for indexing non-nullptr elements, which represent existing neighbors. Following this distinction, capacity() returns the actual allocated size of the adjacency list, while degree() returns the number of existing neighbor nodes. These can be thought of as __graph_node&lt;T&gt;__ equivalents of capacity() vs size() in [dyn_array&lt;T&gt;](#dyn_arrayt).

Neighbors are attached via non-const access to the adjacency list using one of the two operators mentioned above, or by calling add_edge on the target node; either way the node being attached has to be created beforehand. The index member ind_ is only exposed through a const accessor, as its primary purpose is to assume context-dependent roles in descendant and wrapper classes. For example, [graph&lt;T&gt;](#grapht) uses it to uniquely index its nodes, while [binary_tree&lt;T, P&gt;](#binary_treet-p) hierarchy uses it to store node heights.

Due to limitations both inherent and deliberate in its design, __graph_node&lt;T&gt;__ does not support disconnected subgraphs, or bidirectional searches on directed graphs since global edge-flipping requires a full traversal in a node-only design. Graphs with disconnected components can only be represented as a set of separate __graph_node&lt;T&gt;__ chains. If edges are directed, some nodes may end up being unreachable from any other node.

&nbsp;

Notation for __graph_node&lt;T&gt;__ big-O:
- N: Total number of nodes in the graph node chain.
- E: Total number of edges in the graph.
- A: Expected adjacency list size for graph nodes.
- N<sub>D</sub>: Number of nodes traversed to find target at depth D.
- E<sub>D</sub>: Number of edges traversed to find target at depth D.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-----:|:-----:|
|`graph_node()` | Default constructor initializes adjacency lists with size 2 and node value with default T. | O(1) | O(1) | O(1) |
|`graph_node(size_t n)` | Instantiates graph_node with adjacency list of size n without value initialization. Sets node value to default T. | O(A) | O(A) | O(1) |
|`graph_node(const T& v, size_t n)` | Initialize graph_node with its value and adjacency list size. | O(A) | O(A) | O(1) |
|`~graph_node()` | Destructor does nothing. | O(1) | O(1) | O(1) |
|`degree() const : size_t` | Null-counts the adjacency list and returns the number of neighbors graph_node currently has. | O(A) | O(A) | O(1) |
|`capacity() const : size_t` | Returns the actual size of the dynamic adjacency list, including nullptr neighbors. | O(1) | O(1) | O(1) |
|`val() const : const T&` | Returns graph_node's value. | O(1) | O(1) | O(1) |
|`set_val(const T& v) : graph_node<T>&` | Sets graph_node's value. | O(1) | O(1) | O(1) |
|`ind() const : size_t` | Returns a copy of graph_node's index. | O(1) | O(1) | O(1) |
|`operator == (const graph_node<T>& other) const : bool` | Returns true if node values and neighbor value sets are identical. All permutations of the same set of neighbor values are considered equal. | O(1) | O(A<sup>2</sup>) | O(1) |
|`operator != (const graph_node<T>& other) const : bool` | Returns true if node values or neighbor value sets are different. | O(1) | O(A<sup>2</sup>) | O(1) |
|`operator < (const graph_node<T>& other) const : bool` | Nodes with smaller values are less. If values are equal, node with the smaller degree is less. | O(1) | O(A) | O(1) |
|`operator > (const graph_node<T>& other) const : bool` | Nodes with larger values are greater. If values are equal, node with the larger degree is greater. | O(1) | O(A) | O(1) |
|`operator <= (const graph_node<T>& other) const : bool` | Returns true if equal or less. | O(1) | O(A) | O(1) |
|`operator >= (const graph_node<T>& other) const : bool` | Returns true if equal or greater. | O(1) | O(A) | O(1) |
|`operator = (std::initializer_list<graph_node<T>*> list) : graph_node<T>&` | Sets initializer list elements as node's neighbors. Caller is responsible for managing lifetimes and avoiding cycles/loops. | O(A) | O(A) | O(1) |
|`resize_adj(size_t n) : graph_node<T>&` | Resizes the dyn_array holding node's neighbor pointers. | O(A) | O(A) | O(1) |
|`add_edge(graph_node<T>* neighbor_ptr) : graph_node<T>&` | Adds a new neighbor to the node using its pointer. | O(1) | O(A) | O(1) |
|`clear_edges() : graph_node<T>&` | Clears the dyn_array holding node's neighbor pointers. | O(A) | O(A) | O(1) |
|`operator () (size_t i) : graph_node<T>*&` | Read/write access to the i-th neighbor of the node. Supports loops over neighbors that use the degree() method for size as long as the loop doesn't alter neighborhood structure. | O(A) | O(A) | O(1) |
|`operator () (size_t i) const : const graph_node<T>*&` | Read-only access to the i-th neighbor of the node. Supports loops over neighbors that use the degree() method. | O(A) | O(A) | O(1) |
|`operator [] (size_t i) : graph_node<T>*&` | Direct read/write access to neighboring graph_node<T> pointers. Allows edits to both pointers and the objects they point to. Supports loops over neighbors that use the capacity() method for size as long as the loop doesn't resize the dynamic adjacency list. | O(1) | O(1) | O(1) |
|`operator [] (size_t i) const : const graph_node<T>*&` | Direct read-only access to a const graph_node's neighbor pointers. Supports loops over neighbors that use the capacity() method for size. | O(1) | O(1) | O(1) |
|`depth_first_traverse(std::ostream* os_ptr = nullptr, bool is_post = false) : dyn_array<graph_node<T>*>` | Recursively traverses the graph reachable from this graph_node in a depth-first fashion and returns full path as graph_node pointer array. If is_post=true, each node will be visited post-order, i.e., after the entire network of nodes accessible from it are visited. | O(N+E) | O(N+E) | O(N) |
|`depth_first_traverse(std::ostream* os_ptr = nullptr, bool is_post = false) const : dyn_array<const graph_node<T>*>` | Const version of depth-first traversal returns path as a dyn_array of const pointers. | O(N+E) | O(N+E) | O(N) |
|`depth_first_search(const graph_node<T>* target, std::ostream* os_ptr = nullptr) : dyn_array<graph_node<T>*>` | Recursive depth-first search for a node object that terminates immediately if it is found. Returns path to target, or full traversal path if it's not found. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`depth_first_search(const graph_node<T>* target, std::ostream* os_ptr = nullptr) const : dyn_array<const graph_node<T>*>` | Const version of depth-first search for a node object returns path as a dyn_array of const pointers. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`depth_first_search(const T& target, std::ostream* os_ptr = nullptr) : dyn_array<graph_node<T>*>` | Recursive depth-first search for a node value that terminates immediately if it is found. Returns path to target, or full traversal path if it's not found. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`depth_first_search(const T& target, std::ostream* os_ptr = nullptr) const : dyn_array<const graph_node<T>*>` | Const version of depth-first search for a node value returns path as a dyn_array of const pointers. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`breadth_first_traverse(std::ostream* os_ptr = nullptr) : dyn_array<graph_node<T>*>` | Iteratively traverses the graph reachable from this graph_node in a breadth-first fashion using queue&lt;T&gt;, returns full path as graph_node pointer array. | O(N+E) | O(N+E) | O(N) |
|`breadth_first_traverse(std::ostream* os_ptr = nullptr) const : dyn_array<const graph_node<T>*>` | Const version of breadth-first traversal returns path as a dyn_array of const pointers. | O(N+E) | O(N+E) | O(N) |
|`breadth_first_search(const graph_node<T>* target, std::ostream* os_ptr = nullptr) : dyn_array<graph_node<T>*>` | Iterative breadth-first search for a node object that terminates immediately if it is found. Returns path to target, or full traversal path if it's not found. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`breadth_first_search(const graph_node<T>* target, std::ostream* os_ptr = nullptr) const : dyn_array<const graph_node<T>*>` | Const version of breadth-first search for a node object returns path as a dyn_array of const pointers. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`breadth_first_search(const T& target, std::ostream* os_ptr = nullptr) : dyn_array<graph_node<T>*>` | Iterative breadth-first search for a node value that terminates immediately if it is found. Returns path to target, or full traversal path if it's not found. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`breadth_first_search(const T& target, std::ostream* os_ptr = nullptr) const : dyn_array<const graph_node<T>*>` | Const version of breadth-first search for a node value returns path as a dyn_array of const pointers. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`bidirectional_search(const graph_node<T>* target, std::ostream* os_ptr = nullptr) : dyn_array<graph_node<T>*>` | Runs two simultaneous breadth-first searches to find a path from this node to target node. Returns the path found as a dyn_array of graph_node pointers. Only supports undirected graphs as directed graphs won't have backwards edges and running a traversal to reverse the edges removes the need to run a bidirectional search in the first place. | O(N<sub>D/2</sub>+E<sub>D/2</sub>) | O(N+E) | O(N) |
|`bidirectional_search(const graph_node<T>* target, std::ostream* os_ptr = nullptr) const : dyn_array<const graph_node<T>*>` | Const version of bidirectional search returns path as a dyn_array of const pointers. | O(N<sub>D/2</sub>+E<sub>D/2</sub>) | O(N+E) | O(N) |
|`has_cycle() const : bool` | Returns true if there is at least 1 cycle reachable from this graph_node. | O(N+E) | O(N+E) | O(N) |
|`print_sorted(std::ostream& os) const : std::ostream&` | Prints node's value, followed by a sorted sequence of neighbor values. | O(AlogA) | O(A<sup>2</sup>) | O(1) |
|`friend operator << (std::ostream& os, const graph_node<T>& gn) : std::ostream&` | Prints node's value and neighbor values in the order they appear in the adjacency list. | O(A) | O(A) | O(1) |

&nbsp;

### graph&lt;T&gt;

<img src="uml/cppdata_graph.svg" alt="UML Class Diagram for graph" style="width: 100%; height: auto; max-width: 100%; display: block;">

Compositional graph class for weighted and unweighted graphs with either directed or undirected edges. Supports cyclic graphs as well as graphs with multiple disconnected components. Dynamically allocates and owns the memory for its [graph_node&lt;T&gt;](#graph_nodet) instances to ensure stable adjacency list resizing, and stores their pointers in a [dyn_array&lt;graph_node&lt;T&gt;*&gt;](#dyn_arrayt) member for easy direct access. Nodes are uniquely indexed by assigning their positions in this member to their protected ind_ members upon creation. Node order never changes, so indices remain unique and unchanged through each node's lifetime. All class methods handle nodes using their unique indices rather than their pointers, and traversals return index sequences rather than pointer paths.

For its edge representation, __graph&lt;T&gt;__ employs a hybrid design by maintaining the per-node local adjacency lists of its [graph_node&lt;T&gt;](#graph_nodet) members, while in parallel tracking the same connectivity structure using a fully-owned dual-purpose adjacency/weight matrix member in order to offer support for the widest possible range of graph algorithms. The dual-purpose matrix is stored as a [dyn_array&lt;dyn_array&lt;int&gt;&gt;](#dyn_arrayt) member that carries edge weights when representing weighted graphs, and is treated as a boolean adjacency matrix by unweighted graphs, with 0 elements indicating a lack of an edge in either case. This hybrid design also unlocks the complete set of [graph_node&lt;T&gt;](#graph_nodet) methods for reuse, and indeed all __graph&lt;T&gt;__ traversals are thin wrappers around their [graph_node&lt;T&gt;](#graph_nodet) counterparts that merely convert pointer paths to index sequences post-traversal.

None of the __graph&lt;T&gt;__ methods expose mutable [graph_node&lt;T&gt;](#graph_nodet) pointers, and manual edits to the node or edge structure of the graph are disallowed. Instead, a set of dedicated class methods and operators monopolize all creation and destruction to keep the hybrid design tractable. Algorithms that need to modify the graph on-the-fly are still allowed to do so if they're implemented as non-const methods. By jointly maintaining both list and matrix representations for edges, __graph&lt;T&gt;__ admits an increase in the operational overhead of these dedicated methods, and in return provides all class methods with both options to parse graph edges, allowing some graph algorithms to reach optimal performance. Methods can also alternate between the two representations, or use both of them at the same time. All non-algorithmic handling of this edge matrix is agnostic to whether the graph is weighted or not; and the class currently runs no compatibility checks based on the contents of its matrix member.

Two types of initializer list and [dyn_array&lt;T&gt;](#dyn_arrayt) sources are supported by __graph&lt;T&gt;__ for both construction and assignment:
- 1D list/array to assign values to existing nodes, and create the missing ones without adding or removing any edges.
- 2D list/array to rebuild the edge structure from scratch without changing any node values, but adding or removing them as needed to comply with the size dictated by the edge matrix.

Methods implementing well-known graph algorithms utilize a wide range of data structure and utility classes from cppdata, including [hash_table&lt;uintptr_t, V&gt;](#hash_tablek-v) for visited node tracking, __union_find__ for label-merging, __tarjan_bundle__ and its [stack&lt;size_t&gt;](#stackt) member to have a clean and unified interface for finding strongly-connected components, bridges and articulation points, [heap&lt;dyn_array&lt;int&gt;&gt;](#heapt) to track minimum distances and [queue&lt;size_t&gt;](#queuet) or [queue&lt;const graph_node&lt;T&gt;*&gt;](#queuet) to implement breadth-first traversing algorithms.

&nbsp;

Notation for __graph&lt;T&gt;__ big-O:
- N: Number of nodes in the graph.
- E: Number of edges in the graph.
- K: Number of queries or matches for the query.
- A: Expected adjacency list size for graph nodes.
- S: Number of nodes in the subgraph.
- E<sub>S</sub>: Number of edges in the subgraph.
- N<sub>D</sub>: Number of nodes traversed to find target at depth D.
- E<sub>D</sub>: Number of edges traversed to find target at depth D.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`graph()` | Default constructor instantiates an empty graph. | O(1) | O(1) | O(1) |
|`graph(std::initializer_list<T> node_vals)` | Constructs graph from a list of node values. | O(N) | O(N) | O(1) |
|`graph(const dyn_array<T>& node_vals)` | Constructs graph from a dyn_array of node values. | O(N) | O(N) | O(1) |
|`graph(sq_init_list adj_mat)` | Constructs graph from a 2D square initializer list matrix of ints. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(N<sup>2</sup>) |
|`graph(const dyn_array<dyn_array<int> >& adj_mat)` | Constructs graph from a 2D square dyn_array matrix of ints. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(N<sup>2</sup>) |
|`graph(const graph<T>& source)` | Copy constructor performs deep copy of nodes and edges. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`~graph()` | Destructor deletes each node in the node array. | O(N) | O(N) | O(1) |
|`operator [] (size_t ind) const : const graph_node<T>&` | Grants read-only access to a graph_node using its index. | O(1) | O(1) | O(1) |
|`operator [] (const dyn_array<size_t>& inds) const : dyn_array<T>` | Converts dyn_array of indices to node values, skips if out-of-bounds. | O(K) | O(K) | O(K) |
|`operator () (size_t ind) const : const T&` | Grants read-only access to graph_node value using its index. | O(1) | O(1) | O(1) |
|`operator () (size_t ind) : T&` | Grants read/write access to graph_node's value using its index. | O(1) | O(1) | O(1) |
|`is_empty() const : bool` | Returns true if the node array is empty. | O(1) | O(1) | O(1) |
|`size() const : size_t` | Returns the number of nodes in the graph. | O(1) | O(1) | O(1) |
|`num_edges() const : size_t` | Returns the number of edges in the graph. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`clear() : graph<T>&` | Deletes nodes, clears node array, then clears adjacency matrix. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`clear_edges() : graph<T>&` | Clears all local adjacency lists on nodes and resets adjacency matrix. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`operator == (const graph<T>& other) const : bool` | Returns true if sorted node pointer arrays are equal. Uses dyn_array to sort and equality-check pointer arrays, which in turn uses graph_node's equality operator to compare individual elements, factoring in their edge structure. Pre-comparison sorting ensures that all permutations of the same set of nodes with the same local edge structure will be considered equal. There is no need to separately check the adjacency/weight matrix. | O(1) | O(N<sup>2</sup>A<sup>2</sup>) | O(1) |
|`operator != (const graph<T>& other) const : bool` | Returns true if node pointer arrays are not identical. | O(1) | O(N<sup>2</sup>A<sup>2</sup>) | O(1) |
|`operator < (const graph<T>& other) const : bool` | Graphs with less nodes are less. If holding same number of nodes, graph with less edges is less. | O(1) | O(N<sup>2</sup>) | O(1) |
|`operator > (const graph<T>& other) const : bool` | Graphs with more nodes are greater. If holding same number of nodes, graph with more edges is greater. | O(1) | O(N<sup>2</sup>) | O(1) |
|`operator <= (const graph<T>& other) const : bool` | Returns true if equal or less. | O(1) | O(N<sup>2</sup>) | O(1) |
|`operator >= (const graph<T>& other) const : bool` | Returns true if equal or greater. | O(1) | O(N<sup>2</sup>) | O(1) |
|`operator = (const graph<T>& source) : graph<T>&` | Performs deep copy of source nodes and edge structure. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`operator = (sq_init_list adj_mat) : graph<T>&` | Rebuilds graph edges from 2D initializer list. Preserves existing node values and indices while removing excess nodes. If matrix is larger, creates new default-valued nodes with no neighbors. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(N<sup>2</sup>) |
|`operator = (const dyn_array<dyn_array<int> >& adj_mat) : graph<T>&` | Rebuilds graph edges from 2D dyn_array. Preserves existing node values and indices while removing excess nodes. If matrix is larger, creates new default-valued nodes without any neighbors. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(N<sup>2</sup>) |
|`operator = (std::initializer_list<T> node_vals) : graph<T>&` | Reassigns node values from 1D initializer list in node index order. If list is larger than the node array, creates new default-valued nodes with no neighbors. If list is smaller, only the indices within list bounds will have their values reassigned. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`operator = (const dyn_array<T>& node_vals) : graph<T>&` | Reassigns node values from 1D dyn_array in node index order. If dyn_array is larger than the node array, creates new default-valued nodes with no neighbors. If dyn_array is smaller, only the indices within dyn_array bounds will have their values reassigned. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`operator = (graph_node<T>* subgraphs) : graph<T>&` | Clears and rebuilds graph as a copy of the subgraph reachable from the assigned graph_node. | O(NS+ S<sup>2</sup>+E<sub>S</sub>) | O(N<sup>2</sup>S+ S<sup>2</sup>+E<sub>S</sub>) | O(S) |
|`min() const: T` | Returns a copy of the minimum node value in the graph. | O(N) | O(N) | O(1) |
|`max() const: T` | Returns a copy of the maximum node value in the graph. | O(N) | O(N) | O(1) |
|`node_vals() const : dyn_array<T>` | Returns a dyn_array of all node values in the graph. | O(N) | O(N) | O(N) |
|`node_inds(const T& val) const : dyn_array<size_t>` | Returns a dyn_array of indices of all nodes holding the query value. | O(N) | O(N) | O(N) |
|`neighbors(size_t ind) const : dyn_array<size_t>` | Returns a dyn_array holding neighbor indices of the node at index ind. | O(A) | O(A) | O(A) |
|`adj_mat() const : dyn_array<dyn_array<int> >` | Returns the adjacency/weight matrix as a 2D dyn_array of ints. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(N<sup>2</sup>) |
|`add_node(const T& val) : size_t` | Creates a graph_node on heap memory holding the given value and no neighbors, then adds it to the graph and returns its index. | O(N) | O(N<sup>2</sup>) | O(1) |
|`add_node(graph_node<T>* subgraph) : size_t` | Copies the given graph_node and all nodes connected to and reachable from it into the graph. Changes the indices of the subgraph nodes to match them to the indices of their copies in the graph. Returns the index assigned to the input node. | O(NS+ S<sup>2</sup>+E<sub>S</sub>) | O(N<sup>2</sup>S+ S<sup>2</sup>+E<sub>S</sub>) | O(S) |
|`remove_node(size_t ind) : bool` | Deletes the graph_node with the given index from the graph. Readjusts node array, node indices and adjacency/weight matrix. Returns false if node with that index doesn't exist. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`add_edge(size_t i, size_t j) : bool` | Adds a directed edge going from node with index i to node with index j. Returns false if a node with either index doesn't exist. | O(1) | O(1) | O(1) |
|`add_edgew(size_t i, size_t j, int weight) : bool` | Adds a directed weighted edge from node with index i to node with index j. Returns false if a node with either index doesn't exist. | O(1) | O(1) | O(1) |
|`add_edge_undir(size_t i, size_t j) : bool` | Adds an undirected edge between node with index i and node with index j. Returns false if a node with either index doesn't exist. | O(1) | O(1) | O(1) |
|`add_edgew_undir(size_t i, size_t j, int weight) : bool` | Adds an undirected weighted edge between nodes with indices i and j. Returns false if a node with either index doesn't exist. | O(1) | O(1) | O(1) |
|`remove_edge(size_t i, size_t j) : bool` | Removes the edge going from node with index i to the node with index j. Returns false if the edge doesn't exist or if an index is out of bounds. | O(A) | O(N) | O(1) |
|`remove_edge_undir(size_t i, size_t j) : bool` | Removes all edges between the nodes with indices i and j. Returns false if the edge doesn't exist or if indices are out of bounds. | O(A) | O(N) | O(1) |
|`depth_first_traverse(size_t from_ind, std::ostream* os_ptr = nullptr) const : dyn_array<size_t>` | Calls the corresponding graph_node method, then converts path to index array. | O(N+E) | O(N+E) | O(N) |
|`breadth_first_traverse(size_t from_ind, std::ostream* os_ptr = nullptr) const : dyn_array<size_t>` | Calls the corresponding graph_node method, then converts path to index array. | O(N+E) | O(N+E) | O(N) |
|`depth_first_search(size_t from_ind, const T&, std::ostream* os_ptr = nullptr) const : dyn_array<size_t>` | Calls the corresponding graph_node method, then converts path to index array. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`breadth_first_search(size_t from_ind, const T&, std::ostream* os_ptr = nullptr) const : dyn_array<size_t>` | Calls the corresponding graph_node method, then converts path to index array. | O(N<sub>D</sub>+E<sub>D</sub>) | O(N+E) | O(N) |
|`bidirectional_search(size_t i, size_t j, std::ostream* os_ptr = nullptr) const : dyn_array<size_t>` | Calls the corresponding graph_node method, then converts path to index array. | O(N<sub>D/2</sub> +E<sub>D/2</sub>) | O(N+E) | O(N) |
|`find_roots() const : dyn_array<size_t>` | Returns pointers to all the nodes in the graph with no incoming edges. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(N) |
|`has_cycle() const : bool` | Runs 3-color depth-first traversal with hash tables to determine whether graph has any cycles. | O(N+E) | O(N+E) | O(N) |
|`topological_sort() const : dyn_array<T>` | Computes node in-degrees, then topologically sorts them with depth-first traversal. Returns dyn_array of ordered indices, empty if cycle(s) exist. | O(N+E) | O(N+E) | O(N) |
|`strongly_conn_comp() const : dyn_array<dyn_array<size_t> >` | Finds strongly-connected components of a directed graph using Tarjan's algorithm and returns each component as a dyn_array of node indices. On undirected graphs, returns regular connected components. | O(N+E) | O(N+E) | O(N) |
|`weakly_conn_comp() const : dyn_array<int>` | Finds weakly-connected components using hashed depth-first traversal and union-find. Returns contiguous component labels in index order. | O(N+E) | O(N+E) | O(N) |
|`bridges() const : dyn_array<dyn_array<size_t> >` | Finds all cut-edges using Tarjan's algorithm. These are edges whose deletion would increase the number of connected components in the graph. | O(N+E) | O(N+E) | O(N) |
|`articulation_points() const : dyn_array<size_t>` | Finds all cut-vertices using Tarjan's algorithm. These are nodes whose deletion together with their edges would increase the number of connected components in the graph. | O(N+E) | O(N+E) | O(N) |
|`shortest_dists(size_t src) const : dyn_array<int>` | Computes shortest distances by edge weight from src to all other accessible nodes using Dijkstra's algorithm. Returns -1 for nodes inaccessible from src. | O(logN (N+E)) | O(logN (N+E)) | O(N) |
|`dijkstra(size_t src, bool accumulate = true) const : dyn_array<dyn_array<int> >` | Dijkstra's algorithm either finds shortest paths or minimum distances from src to all other nodes, depending on the accumulate argument provided. When it is false, dijkstra returns only distances by placing the distance array at index 0 of the 2D dyn_array, substituting -1 distance for unreachable nodes. Otherwise, 2D dyn_array will contain shortest paths to each other node. If a node is not accessible from src, its path will have a single -1 value to indicate this. | O(logN (N+E)) | O(logN (N+E)) | O(ND) |
|`kahn() const : dyn_array<size_t>` | Kahn's algorithm for topological sorting and cycle detection. Returns a dyn_array of topologically sorted node indices, which will be incomplete if graph has a cycle. | O(N+E) | O(N+E) | O(N) |
|`tarjan(size_t ind, tarjan_bundle& tbun) const : void` | Tarjan's algorithm with 3 different modes for finding strongly connected components, bridges and articulation points. | O(N+E) | O(N+E) | O(N) |
|`print_nodes(std::ostream& os) const : std::ostream&` | Prints node indices and values. | O(N) | O(N) | O(1) |
|`print_adj_lists(std::ostream& os) const : std::ostream&` | Prints values of nodes and their neighbors. | O(N+E) | O(N+E) | O(1) |
|`print_sorted_adj_lists(std::ostream& os) const : std::ostream&` | Prints values of nodes and sorted values of their neighbors. | O(N+ ElogN) | O(N+ ElogN) | O(1) |
|`print_adj_matrix(std::ostream& os) const : std::ostream&` | Prints the adjacency/weight matrix | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |
|`friend operator << (std::ostream& os, const graph<T>& graph) : std::ostream&` | Prints all components of the graph, aligning corresponding fields. | O(N<sup>2</sup>) | O(N<sup>2</sup>) | O(1) |

&nbsp;

### binary_tree&lt;T, P&gt;

<img src="uml/cppdata_binary_tree.svg" alt="UML Class Diagram for binary_tree" style="width: 100%; height: auto; max-width: 100%; display: block;">

The extensive inheritance hierarchy rooted at [graph_node&lt;T&gt;](#graph_nodet) takes on its most baroque form with __binary_tree&lt;T, P&gt;__ polymorphism, which aims not only to extract archetypal abstractions, but also to capture the intricate and subtle forms of interplay between different types of binary trees. The fundamental design principle is for classes higher up the chain to expose modular implementations of their core machinery for granular reuse, and know very little about the particulars of the inheritance hierarchy beneath them. The descendants then interject and tweak this machinery with carefully insulated virtual methods to adjust it to their own needs. This precept establishes a clear division of labor pattern between subclasses where each one can be delegated a core functionality that heavily informs their design.

As the top-level class in this hierarchy, the responsibilities of __binary_tree&lt;T, P&gt;__ are foundational and wide-ranging. Its first job is to elucidate the semantic transition from graphs to binary trees by establishing a set of conventions and restrictions on [graph_node&lt;T&gt;](#graph_nodet) inheritance that all of its descendants will be mandated to follow. Specifically, nodes of __binary_tree&lt;T, P&gt;__ and all its descendants are required to:
- Use the adjacency list member adj_ to store pointers to nodes they are linked to.
- Store at least a pair of such pointers, one for each child node.
- Store pointer to the left child at adj_&#91;0&#93; and implement const/non-const left() accessors for it. These can be public or protected.
- Store pointer to the right child at adj_&#91;1&#93; and implement const/non-const right() accessors for it. These can be public or protected.
- Use the boolean template argument P to control whether the nodes will also store a pointer to their parent nodes (P=true) or not (P=false).
- Store pointer to the parent at adj_&#91;2&#93; if parent-linked and implement const/non-const prnt() accessors for it. These can be public or protected.
- Call [graph_node&lt;T&gt;](#graph_nodet) constructor with consistent adjacency list size arguments in their own constructors, which should at least be P+2.
- Never resize adj_ post-construction and ensure that its size remains unchanged throughout their lifetime.
- Use nullptr to represent link slots missing a linked node.
- Use ind_ member to store node heights.

Storing of tree links at adjacency lists inherited from [graph_node&lt;T&gt;](#graph_nodet) in a manner that is congruent with their use in graph contexts provides all types of binary trees the ability to run all [graph_node&lt;T&gt;](#graph_nodet) methods on their tree structure directly and without any restrictions. Furthermore, it establishes a consistent representation for tree links that is stable under inter-class conversions and polymorphic overrides, allowing __binary_tree&lt;T, P&gt;__ to assume full ownership of this shared base structure without having to offer customized accomodations for its descendants. Specialized subclasses are still able to place stronger constraints on the structure they inherit, or store additional links in the remaining indices of adj_ without breaking any inherited functionality.

Once put in charge of the general tree structure, it makes sense for __binary_tree&lt;T, P&gt;__ to also be delegated its creation, replication and destruction. This is done by giving it a pair of static factory create methods that centralize the creation of all binary trees using an enum option set to pick among supported dynamic types. All created nodes are returned as __binary_tree&lt;T, P&gt;__ pointers, enforcing polymorphic uniformity on the creation process. To permit __binary_tree&lt;T, P&gt;__ subtypes that are stable and functional only for child-only (P=false) or parent-linked (P=true) connectivity, the enums for the two sets are kept separate as ChildrenOnlyType and ParentLinkedType enums, with types that support both appearing under identical enumerator names and values in both enum scopes, functioning as unique type identifiers across both modalities. Using "Substitution Failure Is Not An Error" (SFINAE), __binary_tree&lt;T, P&gt;__ sets an internal alias BinaryTreeType to one of these two enum sets based on its template argument P and enables only one of the static create methods, specifically the one with the switch list of types supporting binary_tree's P argument. Both factory create methods expect BinaryTreeType as their enum argument (e.g., `binary_tree<T, P>::create(val, BinaryTreeType::AVL)`), therefore become seamlessly constrained to the dynamic types listed in the enum that the alias is referring to. This serves as a strong compile-time guard against attempts to create trees with unsupported P arguments, without complicating the enum-based type specification at call site. Two separate sets of create methods and RTTI type() accessors are needed to maintain two separate switch lists of supported types, with only one being SFINAE-enabled on any given class instance based on its P.

Despite the uniformity, extensibility and loose insulation provided by this single factory method, having to cast each created node to its dynamic type before being able run its class methods can easily become too cumbersome. For this reason, __binary_tree&lt;T, P&gt;__ implements for each descendant an additional static create method, with an identical name as the descendant's enumerator identity (e.g. `binary_tree<T, P>::AVL(val)`), that calls the base create method, then casts the pointer to its dynamic type before returning. These additional static methods are disabled via SFINAE for P arguments they don't support, providing another compiler-time check for type validity. This is also one of the rare instances in this inheritance hierarchy where a class implements descendant-specific functionality. All static factory methods are public, with the base method being the only option for runtime type specification, while the named methods providing more convenient syntax when type is known at compile time.

Cloning is similarly centralized in a protected __binary_tree&lt;T, P&gt;__ method that passes a BinaryTreeType argument to its factory create calls, allowing the same cloning recursion to be deployed regardless of the dynamic types being cloned. Members of cloned nodes are set using the virtual set_members method, which descendants override to add their own class-specific members to the cloning process. Public `binary_tree<T, P>::clone()` method simply passes `this->type()` as the enum argument to its protected method, and all other classes in the hierarchy clone themselves by calling `binary_tree<T, P>::clone()` directly and their dynamic types are seamlessly passed on via the RTTI mechanism in `this->type()`, resulting in a clone with identical type and class-specific member values.

Having an inherent acyclic directionality in their link structure and as descendants of a self-referential node-only class, all types of binary trees are inherently capable of deploying the same fully-recursive memory ownership model as [node&lt;T, D&gt;](#nodet-d). To facilitate this, all classes in the hierarchy declare their constructors protected and grant friend access to __binary_tree&lt;T, P&gt;__ to restrict their node creation to heap memory and to the centralized factory methods.

Since standard traversals on a binary tree require nothing more than access to the tree's link structure, __binary_tree&lt;T, P&gt;__ provides a lambda-customizable public traversal engine that optimally executes one of the three recursive traversals enumerated by TravType. It allows injection of customized operations and termination conditions through a standardized lambda signature that takes a __binary_tree&lt;T, P&gt;__ pointer and two intptr_t integer arguments with context-dependent meanings:
  - During PRE_ORDER traversal, they are passed traversal depth and path array address
  - During IN_ORDER traversal, they are passed left branch return and path array address.
  - During POST_ORDER traversal, they are passed left branch return and right branch return.

Traversal engine will run the lambda (check) on current node before, in-between or after recursions depending on the traversal type and it will always recurse left first. Return value -1 is reserved for error signaling; if lambda returns -1 the engine will perform another check on-the-spot by running the same lambda on a nullptr input. If this check also returns -1, traversal terminates immediately. The error signal will be propagated to the return of the traversal iteration even if no early termination condition is hit. These dual-use lambdas provide a convenient unified interface for all descendants and their methods to run conditional or modified traversals by dispatching their own custom lambdas.

Level-order traversals, on the other hand, are iterative in nature and require a [queue&lt;T&gt;](#queue-t) to track nodes; therefore they are powered by a separate engine called stratify. Following a similar idea and using the exact same lambda signature for customization, stratify deploys a parent and a child [queue&lt;binary_tree&lt;T, P&gt;*&gt;](#queue-t) to traverse and separate the tree levels, running the lambda on each dequeued node with current level and parent queue pointer passed as the two intptr_t arguments. Lambda returns of -1 will trigger an early termination check with nullptr, consistent with traversal engine behavior.

All classes in the __binary_tree&lt;T, P&gt;__ inheritance hierarchy can be assigned to and from each other, regardless of their positions in the chain. The assignment operators will often leverage __binary_tree&lt;T, P&gt;__ as a complete structural representation shared by all of its descendants to efficiently duplicate source tree's structure and provide target side compliance with minimal amount of deviation. Since most of its descendants, and indeed __binary_tree&lt;T, P&gt;__ itself inherit all their members and introduce no additional ones, most such assignments amount to little more than this structure duplication.

For its core virtual methods like search, insert and remove, the goal of __binary_tree&lt;T, P&gt;__ is to provide maximally-useful default behaviors that are well-defined on all binary trees. For example, even though there is no inherent insertion order defined for a general __binary_tree&lt;T, P&gt;__, `binary_tree<T, P>::insert`performs level-order left-to-right insertion and guarantees to preserve or approach both balance and completeness with each insertion. This method ends up forming the backbone of insertions into a [heap_tree&lt;T, P, M&gt;](#heap_treet-p-m).

No supporting functionality is currently offered for cyclic binary trees.

&nbsp;

Notation for __binary_tree&lt;T, P&gt;__ big-O:
- N: Number of nodes in the binary tree.
- H: Height of the binary tree.
- W: Width of the binary tree.
- K: Number of matches for the query.
- L: Number of leaf nodes in the binary tree.
- S: Number of nodes in the subtree candidate.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`static create(const T& v, BinaryTreeType type = BinaryTreeType::STANDARD) : binary_tree<T, P>*` | Polymorphic SFINAE factory method enabled for P=false, with a matching switch list of subtypes supporting P=false. Creates on heap memory a node for any of the binary_tree subtypes in the inheritance hierarchy, returns it as binary_tree pointers. | O(1) | O(1) | O(1) |
|`static create(const T& v, BinaryTreeType type = BinaryTreeType::STANDARD) : binary_tree<T, P>*` | Polymorphic SFINAE factory method enabled for P=true, with a matching switch list of subtypes supporting P=ttrue. Creates on heap memory a node for any of the binary_tree subtypes in the inheritance hierarchy, returns it as binary_tree pointers. | O(1) | O(1) | O(1) |
|`static STANDARD(const T& v) : binary_tree<T, P>*` | Static factory helper to create a binary_tree node and return it as a binary_tree pointer. | O(1) | O(1) | O(1) |
|`static SEARCH(const T& v) : binary_search_tree<T, P>*` | Static factory helper to create a binary_search_tree node and return it as a binary_search_tree pointer.  | O(1) | O(1) | O(1) |
|`static MIN_HEAP(const T& v) : heap_tree<T, P, false>*` | Static factory helper to create a min heap_tree node and return it as a heap_tree pointer.  | O(1) | O(1) | O(1) |
|`static MAX_HEAP(const T& v) : heap_tree<T, P, true>*` | Static factory helper to create a max heap_tree node and return it as a heap_tree pointer.  | O(1) | O(1) | O(1) |
|`static AVL(const T& v) : avl_tree<T, P>*` | Static factory helper to create an avl_tree node and return it as an avl_tree pointer.  | O(1) | O(1) | O(1) |
|`static RED_BLACK(const T& v) : red_black_tree<T>*` | Static factory helper to create a red_black_tree node and return it as a binary_tree pointer.  | O(1) | O(1) | O(1) |
|`~binary_tree()` | Destructor recursively deletes all descendant nodes. | O(N) | O(N) | O(H) |
|`clone() const : binary_tree<T, P>*` | Clones the subtree rooted at this node. Duplicates tree structure and node members. Computes and stores clone's node heights on-the-fly. Called by descent clone methods since cloned tree's type is set with dynamic, RTTI-determined type of the calling node. | O(N) | O(N) | O(H) |
|`left() : binary_tree<T, P>*&` | Mutator for left child; a reference to adj_[0]. | O(1) | O(1) | O(1) |
|`left() const : const binary_tree<T, P>*` | Accessor for left child; a reference adj_[0]. | O(1) | O(1) | O(1) |
|`right() : binary_tree<T, P>*&` | Mutator for right child; a reference to adj_[1]. | O(1) | O(1) | O(1) |
|`right() const : const binary_tree<T, P>*` | Accessor for right child; a reference to adj_[1]. | O(1) | O(1) | O(1) |
|`prnt() : binary_tree<T, P>*&` | Mutator for parent; a reference to adj_[2]. Returns left child instead if P=false, which is an invalid call. | O(1) | O(1) | O(1) |
|`prnt() const : const binary_tree<T, P>*` | Accessor for parent; a reference to adj_[2]. Returns left child instead if P=false, which is an invalid call. | O(1) | O(1) | O(1) |
|`type() : BinaryTreeType` | Determines the type of binary tree using RTTI dynamic_cast mechanism. SFINAE-enabled for P=false, with a matching switch list of subtypes supporting P=false. | O(1) | O(1) | O(1) |
|`type() : BinaryTreeType` | Determines the type of binary tree using RTTI dynamic_cast mechanism. SFINAE-enabled for P=true, with a matching switch list of subtypes supporting P=true. | O(1) | O(1) | O(1) |
|`height() const : const size_t&` | Grants read-only access to the last computed node height. Note that valid heights are not maintained through structural changes. | O(1) | O(1) | O(1) |
|`height() : size_t&` | Grants read/write access to the last computed max distance to a leaf node from this node. Note that valid heights are not maintained through structural changes. | O(1) | O(1) | O(1) |
|`compute_heights() : size_t` | Computes and stores node heights with bottom-up dynamic programming within a post-order traversal. Returns height of this node. | O(N) | O(N) | O(H) |
|`link_parents() : binary_tree<T, P>*` | Links parent nodes. SFINAE-disabled for P=false. Useful when constructing trees manually with mutators rather than through insertions. | O(N) | O(N) | O(H) |
|`root_only() const : bool` | Returns true if the node is childless. Does not traverse backwards. | O(1) | O(1) | O(1) |
|`size() const : size_t` | Returns the number of nodes, including itself. Does not count backwards. | O(N) | O(N) | O(N) |
|`clear() : binary_tree<T, P>*` | Deletes all descendant nodes and resets all class members. | O(N) | O(N) | O(H) |
|`operator == (const binary_tree<T, P>& other) const : bool` | Simultaneously traverses both trees and returns true if all node values are identical. Does not consider any other member or node state. | O(1) | O(N) | O(H) |
|`operator != (const binary_tree<T, P>& other) const : bool` | Returns true if trees are not equal. | O(1) | O(N) | O(H) |
|`operator = (const binary_tree<T, P>& src) : binary_tree<T, P>&` | Clones source tree starting from the given node. | O(N) | O(N) | O(H) |
|`operator = (std::initializer_list<T> list) : binary_tree<T, P>&` | Turns init list to dyn_array, then rebuilds tree from it. | O(N) | O(N) | O(N) |
|`operator = (const dyn_array<T>& src) : binary_tree<T, P>&` | Level-order rebuilds complete binary tree from dyn_array. | O(N) | O(N) | O(W) |
|`min() const : T` | Returns minimum node value in the tree. | O(N) | O(N) | O(N) |
|`max() const : T` | Returns maximum node value in the tree. | O(N) | O(N) | O(N) |
|`last(binary_tree<T, P>*& parent) : binary_tree<T, P>*` | Returns the deepest rightmost leaf node and its parent. | O(N) | O(N) | O(W) |
|`next() : binary_tree<T, P>*` | Returns the parent of the next free level-order left-to-right insertion slot. | O(N) | O(N) | O(W) |
|`is_full() const : bool` | Returns true if all nodes have 0 or 2 children. | O(N) | O(N) | O(H) |
|`is_balanced() const : bool` | Returns true if branch heights differ by at most 1 for all nodes. | O(N) | O(N) | O(H) |
|`is_complete() const : bool` | Returns true if tree is filled in left-to-right order. | O(N) | O(N) | O(W) |
|`is_perfect() const : bool` | Returns true if all tree levels are completely filled. | O(N) | O(N) | O(H) |
|`is_search() const : bool` | Returns true if all nodes are in-order with respect to their children. | O(N) | O(N) | O(H) |
|`is_heap(bool max = false) const : bool` | Returns true if all nodes are smaller(M=false) / larger(M=true) than their children. | O(N) | O(N) | O(H) |
|`traverse(TravType type, std::ostream* osp = nullptr) : dyn_array<binary_tree<T, P>*>` | Traversal engine wrapper for a simpler interface. Executes the traversal specified by TravType. Builds returns full traversal path. Optional osp argument is for streaming path, root and leaves. | O(N) | O(N) | O(N) |
|`traverse(TravType type, std::ostream* osp = nullptr) const : dyn_array<const binary_tree<T, P>*>` | Const version of traverse returns a path array of const binary_tree pointers. | O(N) | O(N) | O(N) |
|`traversal_engine(Check&& check, Path& path, TravType type, bool accumulate = true, intptr_t depth = 0) const : int` | Executes one of the 3 recursive traversals specified by TravType. Allows injection of customized operations and termination conditions, see class descriptioon above for details. | O(N) | O(N) | O(N) O(H) |
|`stratify(Check&& check, dyn_array<dyn_array<BT_Ptr> >& path, bool accumulate = true) const : BT_Ptr` | Executes level-order traversal. Allows injection of customized operations and termination conditions, see class descriptioon above for details. | O(N) | O(N) | O(N) O(W) |
|`search(const T& val) : binary_tree<T, P>*` | Searches for query value with pre-order traversal. Returns pointer to the found node if found, nullptr otherwise. | O(N) | O(N) | O(H) |
|`find_parent(const T& val) : binary_tree<T, P>*` | Searches all non-root nodes for query value with pre-order traversal. Returns pointer to the found node's parent, or nullptr if none are found. | O(N) | O(N) | O(H) |
|`find_parent(const binary_tree<T, P>* child) : binary_tree<T, P>*` | Searches all non-root nodes for target with pre-order traversal. Returns pointer to target's parent if found, nullptr otherwise. | O(N) | O(N) | O(H) |
|`find(const T& val) const : dyn_array<const binary_tree<T, P>*>` | Recursively finds all nodes in the tree with values matching query. | O(N) | O(N) | O(H+K) |
|`find_root() : binary_tree<T, P>*` | Finds and returns root. Returns the node itself if P=false. | O(H) | O(H) | O(1) |
|`find_root() const : const binary_tree<T, P>*` | Const version of find_root returns root as a const binary_tree pointer. | O(H) | O(H) | O(1) |
|`find_leaves() : dyn_array<binary_tree<T, P>*>` | Finds all leaves of the node and returns them in a dyn_array of pointers. | O(N) | O(N) | O(H+L) |
|`find_leaves() const : dyn_array<const binary_tree<T, P>*>` | Const version of find_leaves returns leaves as const binary tree pointers. | O(N) | O(N) | O(H+L) |
|`generate_leaf_paths() : dyn_array<dyn_array<binary_tree<T, P>*> >` | Generates all paths from this node to its leaves. Returns paths in a 2D dyn_array. | O(NlogN) | O(NlogN) | O(NlogN) |
|`generate_leaf_paths() const : dyn_array<dyn_array<const binary_tree<T, P>*> >` | Const version of generate_leaf_paths returns path arrays of const binary_tree pointers. | O(NlogN) | O(NlogN) | O(NlogN) |
|`insert(const T& val) : binary_tree<T, P>*` | Level-order left-to-right insertion. Returns a pointer to inserted node. Preserves or approaches completeness and balance. | O(N) | O(N) | O(W) |
|`remove(const T& val) : binary_tree<T, P>*` | Removes first encountered node with matching value. Returns pointer to new root. If calling node's value matches the query and it has no children, deletes it and returns nullptr. | O(N) | O(N) | O(H) |
|`remove(const binary_tree<T, P>* target) : binary_tree<T, P>*` | Finds and removes the target. Returns pointer to new root. If target is the calling node and it has no children, deletes it and returns nullptr. | O(N) | O(N) | O(H) |
|`remove() : T` | Level-order right-to-left deletion. Returns deleted node's value. Preserves or approaches completeness and balance. | O(N) | O(N) | O(W) |
|`is_subtree(const binary_tree<T, P>* bt_ptr) const : bool` | Returns true if calling node's tree is a subtree of bt_ptr. | O(N+KS) | O(NS) | O(H+K) |
|`has_subtree(const binary_tree<T, P>* bt_ptr) const : bool` | Returns true if bt_ptr is a subtree of calling node's tree. | O(N+MS) | O(NS) | O(H+M) |
|`friend operator << (std::ostream& os, const binary_tree<T, P>* bt) : std::ostream&` | Prints tree contents. | O(N) | O(N) | O(H) |
|`friend operator << (std::ostream& os, const binary_tree<T, P>& bt) : std::ostream&` | Compactly prints tree contents. | O(N) | O(N) | O(H) |

&nbsp;

### binary_search_tree&lt;T, P&gt;

<img src="uml/cppdata_binary_search_tree.svg" alt="UML Class Diagram for binary_search_tree" style="width: 100%; height: auto; max-width: 100%; display: block;">

A type of [binary_tree&lt;T, P&gt;](#binary_treet-p) specialized for efficient search. It plays the same role for all search trees that [binary_tree&lt;T, P&gt;](#binary_treet-p) does for the entire hierarchy by providing the following set of foundational capabilities to its descendants:
- A new count_ member for storing duplicate value counts at each node, and a robust duplicate value handling mechanism that maintains a base structure of unique values.
- Inter-class assignment operators that account for the structural discrepancies between binary tree types with and without the newly-introduced count_ member.
- An on-demand balance() method that levereges the fact that duplicate values are factored out of the tree structure by the new mechanism, and guarantees to produce trees that are both balanced and complete on all of its descendants.
- Optimal and lambda-customizable search and removal engines that are directly applicable to all search trees.
- Iterative node insertion method that is shared among most search trees.
- Conditional node deletion logic based on node's child count, also shared widely among most search trees.

Core search tree traversals require T to define a signed distance metric via the - operator. Insertions and removals swap node values rather than node objects, so node addresses are not guaranteed to preserve their values during state-changing operations.

To make its count_ member fully customizable via polymorphic interjections and overrides by descendants, __binary_search_tree&lt;T, P&gt;__ encapsulates all const and non-const access to this member in simple virtual methods.

The balance method traverses the tree in-order to get sorted values without actually sorting them, then recursively rebuilds a balanced and complete __binary_search_tree&lt;T, P&gt;__ from the array of sorted values. It provides completeness guarantee by adjusting the mid-value selection heuristic and skew the recursion in favor of the left branch just enough to ensure nodes at the deepest level are accumulated contiguously on the left side. Same recursion is deployed to build search trees from any [dyn_array&lt;T&gt;](#dyn_arrayt) simply by sorting them prior to recursive buildup; or indeed from any [binary_tree&lt;T, P&gt;](#binary_treet-p) since they can be a serialized into node value arrays with a full traversal.

Despite delivering such a strict balance method with tight guarantees on all search trees, __binary_search_tree&lt;T, P&gt;__ is a general, high-level class and is not self-balancing. Therefore its instances are not guaranteed to be balanced at all times; in fact, sequential insertions of sorted values is guaranteed to produce skewed trees without any balancing. Since heights are not an integral part of their core machinery, they also do not compute, store or maintain node heights unless `binary_tree<T, P>::compute_heights()` method is explicitly called to request them.

To protect the integrity of its internal state, __binary_search_tree&lt;T, P&gt;__ exposes the alternative, const-only accessor set publicly (left_ch(), right_ch() and parent()). Manual modifications to its structure are highly discouraged but possible with dynamic casts to [binary_tree&lt;T, P&gt;](#binary_treet-p) pointers to expose the mutators. Use to avoid self-balancing overhead when balance scheduling is handled externally.

&nbsp;

Notation for __binary_search_tree&lt;T, P&gt;__ big-O:
- N: Number of nodes in the binary search tree.
- H: Height of the binary search tree.
- W: Width of the binary search tree.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`~binary_search_tree()` | Destructor executes no additional steps. | O(N) | O(N) | O(H) |
|`clone() const : binary_search_tree<T, P>*` | Calls binary_tree's clone method, then dynamic casts the return. | O(N) | O(N) | O(H) |
|`left_ch() const : const binary_search_tree<T, P>*` | Grants read-only access to node's left child. | O(1) | O(1) | O(1) |
|`right_ch() const : const binary_search_tree<T, P>*` | Grants read-only access to node's right child. | O(1) | O(1) | O(1) |
|`parent() const : const binary_search_tree<T, P>*` | Grants read-only access to node's parent. Returns left child instead if P=false, which is an invalid call. | O(1) | O(1) | O(1) |
|`count() const : int` | Polymorphic accessor to the duplicate value count of the node. | O(1) | O(1) | O(1) |
|`size() const : size_t` | Returns the total number of values in the tree. | O(N) | O(N) | O(N) |
|`num_nodes() const : size_t` | Returns the total number of nodes in the tree. | O(N) | O(N) | O(N) |
|`operator == (const binary_search_tree<T, P>& other) const : bool` | Trees are equal if binary_tree's operator thinks they're equal, and they have equal size (number of values). | O(1) | O(N) | O(H) |
|`operator != (const binary_search_tree<T, P>& other) const : bool` | Returns true if trees are not equal. | O(1) | O(N) | O(H) |
|`operator = (std::initializer_list<T> list) : binary_search_tree<T, P>&` | Clears tree, turns initializer list into a dyn_array, then directly assigns it to itself with dyn_array assignment operator. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const dyn_array<T>& src) : binary_search_tree<T, P>&` | Clears tree, sorts dyn_array and rebuilds itself from it. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const binary_tree<T, P>& src) : binary_search_tree<T, P>&` | Clears tree, traverses and sorts unique values in the binary_tree, rebuilds itself from the sorted dyn_array of values. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const binary_search_tree<T, P>& src) : binary_search_tree<T, P>&` | Runs the binary_tree operator, which clones tree structure and copies all members with RTTI. | O(N) | O(N) | O(H) |
|`min() const : T` | Override to find minimum value faster using search tree properties. | O(H) | O(N) | O(1) |
|`max() const : T` | Override to find maximum value faster using search tree properties. | O(H) | O(N) | O(1) |
|`all() const : dyn_array<T>` | Returns a sorted dyn_array of all node values, including duplicates. | O(N) | O(N) | O(N) |
|`search(const T& val) : binary_search_tree<T, P>*` | Returns first node with matching value, or nullptr if none found. | O(H) | O(N) | O(1) |
|`find_parent(const T& val) : binary_search_tree<T, P>*` | Searches for a node with specified value, return a pointer to its parent. | O(H) | O(N) | O(1) |
|`find_parent(const binary_search_tree<T, P>* child) : binary_search_tree<T, P>*` | Finds and returns the query child's parent. | O(H) | O(N) | O(1) |
|`find_succ() : binary_search_tree<T, P>*` | Finds and returns node's in-order successor, searching only downwards. | O(H) | O(N) | O(1) |
|`find_succ(binary_search_tree<T, P>*& parent) : binary_search_tree<T, P>*` | Finds and returns the node's in-order successor and its parent. | O(H) | O(N) | O(1) |
|`find_closest(const T& val) const : T` | Finds and returns the node value that is closest to the query value. | O(H) | O(N) | O(1) |
|`insert(const T& val) : binary_search_tree<T, P>*` | Standard iterative search tree insertion. Returns pointer to the inserted node. | O(H) | O(N) | O(1) |
|`remove(const T& val) : binary_search_tree<T, P>*` | Standard iterative search tree removal by value. Returns new root. If calling node's value matches query and it has no children, deletes it and returns nullptr. | O(H) | O(N) | O(1) |
|`remove(const binary_search_tree<T, P>* target) : binary_search_tree<T, P>*` | Standard iterative search tree removal by pointer. Returns new root. If calling node is the target and it has no children, deletes it and returns nullptr. | O(H) | O(N) | O(1) |
|`remove() : T` | Removes or decrements count of deepest rightmost node, returns its value. | O(N) | O(N) | O(W) |
|`balance() : binary_search_tree<T, P>*` | Balances the tree using in-order traversal, followed by recursive rebuilding from the sorted array of values. Guaranteed to produce balanced and complete search trees. Result is also guaranteed to be AVL-compliant, although no specific accomodations were made. | O(N) | O(N) | O(N) |

&nbsp;

### avl_tree&lt;T, P&gt;

<img src="uml/cppdata_avl_tree.svg" alt="UML Class Diagram for avl_tree" style="width: 100%; height: auto; max-width: 100%; display: block;">

A self-balancing [binary_search_tree&lt;T, P&gt;](#binary_search_treet-p) that is primarily responsible for:
- Valid height maintenance.
- Search tree rotations
- A strict self-balancing routine that uses recursive rotations to comply with the formal definition of a balanced [binary_tree&lt;T, P&gt;](#binary_treet-p) at all times.

Overrides insert and remove methods with their recursive versions to allow direct integration of the self-balancing routine into these operations. Both insertion and removal recursions in __avl_tree&lt;T, P&gt;__ seek their targets in their winding phase, and rebalance locally with tree rotations in their unwinding phase.

Inherited methods do a lot of the heavy lifting in __avl_tree&lt;T, P&gt;__'s handling of its core search functions, local node removal logic, assignments from other types of binary trees, recursive rebuilding from value arrays, duplicate value handling, deep equality checks and on-demand balancing for its completeness guarantee. Also inherits the signed distance metric requirement for T.

Like its parent, __avl_tree&lt;T, P&gt;__ hides its standard accessors and mutators to discourage manual edits, and exposes the const-only left_ch(), right_ch() and parent() set instead for inspections and traversals. Use when rigid self-balancing is needed and the higher overhead is not a concern.

&nbsp;

Notation for __avl_tree&lt;T, P&gt;__ big-O:
- N: Number of nodes in the AVL tree.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`~avl_tree()` | Destructor executes no additional steps. | O(N) | O(N) | O(logN) |
|`clone() const : avl_tree<T, P>*` | Calls binary_tree's clone method, then dynamic casts the return. | O(N) | O(N) | O(logN) |
|`left_ch() const : const avl_tree<T, P>*` | Grants read-only access to node's left child. | O(1) | O(1) | O(1) |
|`right_ch() const : const avl_tree<T, P>*` | Grants read-only access to node's right child. | O(1) | O(1) | O(1) |
|`parent() const : const avl_tree<T, P>*` | Grants read-only access to node's parent. Returns left child instead if P=false, which is an invalid call. | O(1) | O(1) | O(1) |
|`balance_factor() const : int` | Computes and returns the balance factor of the node. | O(1) | O(1) | O(1) |
|`operator = (std::initializer_list<T> list) : avl_tree<T, P>&` | Builds a dyn_array from init list, then assigns it to itself with dyn_array assignment operator. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const dyn_array<T>& src) : avl_tree<T, P>&` | Runs binary_search_tree operator, which uses avl_tree's polymorphic from_dyn_arr override. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const binary_tree<T, P>& src) : avl_tree<T, P>&` | Runs binary_search_tree operator, which uses avl_tree's polymorphic from_dyn_arr override. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const binary_search_tree<T, P>& src) : avl_tree<T, P>&` | Runs binary_tree operator, then balances with binary_search_tree method. | O(N) | O(N) | O(N) |
|`insert(const T& val) : avl_tree<T, P>*` | Recursively searches for insert location to preserve AVL properties. Unlike parent's method, returns root as it can change. | O(logN) | O(logN) | O(logN) |
|`remove(const T& val) : avl_tree<T, P>*` | Recursively searches for the node to be deleted, balancing nodes during its unwinding phase after the node is deleted. Node values may change due to value swaps. Returns new root. | O(logN) | O(logN) | O(logN) |
|`remove(const avl_tree<T, P>* target) : avl_tree<T, P>*` | Recursively searches for the node to be deleted, balancing nodes during its unwinding phase after the node is deleted. Node values may change due to value swaps. Returns new root. | O(logN) | O(logN) | O(logN) |


&nbsp;

### red_black_tree&lt;T&gt;

<img src="uml/cppdata_red_black_tree.svg" alt="UML Class Diagram for red_black_tree" style="width: 100%; height: auto; max-width: 100%; display: block;">

Can be thought of as a specialized [avl_tree&lt;T, true&gt;](#avl_treet-p) with looser standards for balance, and lower operational overhead. Self-balances using a binary red-black node colorization scheme combined with [avl_tree&lt;T, true&gt;](#avl_treet-p) rotations to cap the longest root-to-leaf path at twice the length of the shortest, which also requires __red_black_tree&lt;T&gt;__ to maintain valid heights, like its parent class. Inherits the signed distance metric requirement for T.

Needs parent links on its nodes for its core color-balancing logic, therefore __red_black_tree&lt;T&gt;__ is defined exclusively as a ParentLinkedType. Attempts to create __red_black_tree&lt;T&gt;__ nodes without parent links (i.e., from a P=false namespace or specialization) will trigger a compiler error if the offender is a constant expression; otherwise it will be caught at runtime by the switch list in factory create method, which won't recognize it.

Makes extensive use of inheritance and polymorphism, relying on [avl_tree&lt;T, true&gt;](#avl_treet-p) for its rotations and height maintenance, and on [binary_search_tree&lt;T, true&gt;](#binary_search_treet-p) for the entirety of its search functionality, the backbone of its insert and remove methods, on-demand balancing with completeness guarantee, duplicate value handling mechanism and equality operators. Most __red_black_tree&lt;T&gt;__ virtual overrides call an equivalent ancestor method to duplicate their behavior, then post-process to fix color violations. In the same way, its assignment operators typically invoke an ancestor's operator to duplicate the source tree structure, followed by full colorization from scratch.

Instead of storing node colors as an additional member at each node, __red_black_tree&lt;T&gt;__ manipulates the sign of the count_ member inherited from [binary_search_tree&lt;T, true&gt;](#binary_search_treet-p) so that red nodes store their duplicate counts as negative integers; and overrides all inherited virtual accessors and mutators of count_ to account for this change. This insulates the internal color storage mechanism from the outside world, and ensures that all inherited methods remain unaffected.

Unless manually tampered with, __red_black_tree&lt;T&gt;__ instances should never contain any violations. Nevertheless, a set of methods are provided for granular inspections and color-based validity checks. All other color-related functionality is considered internal, with const accessors provided for convenience.

Two __red_black_tree&lt;T&gt;__ instances are considered equal if their tree structures, node values and duplicate counts are identical, regardless of node coloring. Like its parent, it exposes the const-only accessor set, in order to discourage manual edits to its structure. Most operations on a __red_black_tree&lt;T&gt;__ will return the tree root since entire tree needs to be checked and fixed for any violations after edits. Use when self-balancing is needed under frequent insertions and deletions.

&nbsp;

Notation for __red_black_tree&lt;T&gt;__ big-O:
- N: Number of nodes in the red-black tree.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`~red_black_tree()` | Destructor executes no additional steps. | O(N) | O(N) | O(logN) |
|`clone() const : red_black_tree<T>*` | Calls binary_tree's clone method, then dynamic casts the return. | O(N) | O(N) | O(logN) |
|`left_ch() const : const red_black_tree<T>*` | Grants read-only access to node's left child. | O(1) | O(1) | O(1) |
|`right_ch() const : const red_black_tree<T>*` | Grants read-only access to node's right child. | O(1) | O(1) | O(1) |
|`parent() const : const red_black_tree<T>*` | Grants read-only access to node's parent. | O(1) | O(1) | O(1) |
|`is_red() const : bool` | Returns true if the node is red. | O(1) | O(1) | O(1) |
|`is_valid() const : bool` | Returns true if the node's tree is a valid red-black tree. | O(N) | O(N) | O(logN) |
|`count() const : int` | Polymorphic override flips the sign of negative counts of red nodes to return correct count values. | O(1) | O(1) | O(1) |
|`operator = (std::initializer_list<T> list) : red_black_tree<T>&` | Builds a dyn_array from initializer list, then assigns it to itself with dyn_array assignment operator. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const dyn_array<T>& src) : red_black_tree<T>&` | Runs binary_search_tree operator, which uses red_black_tree's polymorphic from_dyn_arr override. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const binary_tree<T, true>& src) : red_black_tree<T>&` | Runs binary_search_tree operator, which uses red_black_tree's polymorphic from_dyn_arr override. | O(NlogN) | O(N<sup>2</sup>) | O(N) |
|`operator = (const binary_search_tree<T, true>& src) : red_black_tree<T>&` | Runs avl_tree operator, then colorizes the entire tree. | O(N) | O(N) | O(N) |
|`operator = (const avl_tree<T, true>& src) : red_black_tree<T>&` | Runs binary_tree operator, then colorizes the entire tree. | O(N) | O(N) | O(logN) |
|`find_root() : red_black_tree<T>*` | Runs binary_tree method, then dynamic casts to red_black_tree pointer. | O(logN) | O(logN) | O(1) |
|`find_root() const : const red_black_tree<T>*` | Const version of find_root returns a const red_black_tree pointer. | O(logN) | O(logN) | O(1) |
|`insert(const T& val) : red_black_tree<T>*` | Calls binary_search_tree insert method, then fixes red-red violations. Returns new root as it can change during fixes. | O(logN) | O(logN) | O(1) |
|`remove(const T& val) : red_black_tree<T>*` | Calls binary_search_tree removal_engine to remove node by value, then fixes black violations. Returns new root as it can change. |  O(logN) |  O(logN) |  O(logN) |
|`remove(const red_black_tree<T>* target) : red_black_tree<T>*` | Calls binary_search_tree removal_engine to remove node by pointer, then fixes black violations. Returns new root as it can change. |  O(logN) |  O(logN) |  O(logN) |
|`fix_red(red_black_tree<T>* node) : red_black_tree<T>*` | Fixes red-red violations post-insertion. | O(logN) | O(logN) | O(1) |
|`fix_black(red_black_tree<T>* node, red_black_tree<T>* parent) : red_black_tree<T>*` | Fixes black violations post-removal. | O(logN) | O(logN) | O(1) |
|`colorize() : red_black_tree<T>*` | Colorizes the tree from scratch using min heights and a black node budget. | O(N) | O(N) | O(logN) |
|`red_violation() const : bool` | Recursively checks downwards for 2 consecutive red nodes. | O(N) | O(N) | O(logN) |
|`black_violation() const : bool` | Returns true if a root-to-leaf path has different number of black nodes. | O(N) | O(N) | O(logN) |

&nbsp;

### heap_tree&lt;T, P, M&gt;

<img src="uml/cppdata_heap_tree.svg" alt="UML Class Diagram for heap_tree" style="width: 100%; height: auto; max-width: 100%; display: block;">

A complete [binary_tree&lt;T, P&gt;](#binary_treet-p) specialized as a tree-based heap implementation. Uses a templated type separation, with M=false for min heap and M=true for max heap, because node-only classes cannot store global states and per-node storage of type would be wasteful. Requires T to define a signed distance metric via the - operator.

Can be assigned to and from its more efficient array-based sibling class [heap&lt;T&gt;](#heapt) in a way that preserves the exact structure of the source. When assigned another type of complete binary tree, __heap_tree&lt;T, P, M&gt;__ duplicates its structure using [binary_tree&lt;T, P&gt;](#binary_treet-p) assignment operator, then heapifies the structure by sifting down each non-leaf node.

Insertions and removals swap node values rather than node objects, so node addresses aren't guaranteed to preserve their values during state-changing operations. To protect the integrity of its internal state, only exposes the const accessor set left_ch(), right_ch() and parent() publicly. Manual modifications are highly discouraged but possible with dynamic casts to [binary_tree&lt;T, P&gt;](#binary_treet-p) pointers to expose the mutators.

Use primarily to:
- Handle highly-variable heap sizes.
- Inspect or visualize [heap&lt;T&gt;](#heapt) array as a [binary_tree&lt;T, P&gt;](#binary_treet-p) structure.
- Implement custom heap types that are inherently tree-structured or recursive.
- Leverage tree and graph algorithms on heap structures.

&nbsp;

Notation for __heap_tree&lt;T, P, M&gt;__ big-O:
- N: Number of nodes in the heap tree.
- W: Width of the heap tree.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`~heap_tree()` | Destructor executes no additional steps. | O(N) | O(N) | O(logN) |
|`clone() const : heap_tree<T, P, M>*` | Calls binary_tree's clone method, then dynamic casts the return. | O(N) | O(N) | O(logN) |
|`left_ch() const : const heap_tree<T, P, M>*` | Grants read-only access to node's left child. | O(1) | O(1) | O(1) |
|`right_ch() const : const heap_tree<T, P, M>*` | Grants read-only access to node's right child. | O(1) | O(1) | O(1) |
|`parent() const : const heap_tree<T, P, M>*` | Grants read-only access to node's parent. Returns left child instead if P=false, which is an invalid call. | O(1) | O(1) | O(1) |
|`is_max() const : bool` | Returns true if heap is a max heap. | O(1) | O(1) | O(1) |
|`last(heap_tree<T, P, M>*& parent) : heap_tree<T, P, M>*` | Leverages completeness to find last node faster than binary_tree's method. | O(logN) | O(logN) | O(1) |
|`operator = (std::initializer_list<T> list) : heap_tree<T, P, M>&` | Builds a dyn_array from initializer list, then assigns it to itself with dyn_array assignment operator. | O(N) | O(N) | O(N) |
|`operator = (const dyn_array<T>& src) : heap_tree<T, P, M>&` | Assigns to self with binary_tree operator to rebuild as a complete tree, then heapifies itself. | O(N) | O(N) | O(W) |
|`operator = (const binary_tree<T, P>& src) : heap_tree<T, P, M>&` | Assigns complete trees with binary_tree operator, then heapifies itself. All others are traversed and rebuilt from dyn_array. | O(N) | O(N) | O(logN) O(N) |
|`operator = (const binary_search_tree<T, P>& src) : heap_tree<T, P, M>&` | Builds a dyn_array from binary_search_tree's node values including duplicates, then assigns it to itself with dyn_array assignment operator. | O(N) | O(N) | O(N) |
|`operator = (const heap<T>& hp) : heap_tree<T, P, M>&` | Rebuilds heap_tree by duplicating the structure of an array-based heap. | O(N) | O(N) | O(W) |
|`insert(const T& val) : heap_tree<T, P, M>*` | Calls binary_tree insert, then sifts up the inserted node. | O(N) | O(N) | O(logN) |
|`remove(const T& val) : heap_tree<T, P, M>*` | Runs binary_search_tree removal by value, then sifts down replaced value. | O(N) | O(N) | O(logN) |
|`remove(const heap_tree<T, P, M>* target) : heap_tree<T, P, M>*` | Runs binary_search_tree removal by pointer, then sifts down replaced value. | O(N) | O(N) | O(logN) |
|`remove() : T` | Replaces root value with deepest rightmost node value, then deletes that node, sifts down root's value and returns root. | O(logN) | O(logN) | O(1) |
|`heapify() : heap_tree<T, P, M>*` | Recursively heapify the subtree rooted at this node. | O(N) | O(N) | O(logN) |

&nbsp;

### heap&lt;T&gt;

<img src="uml/cppdata_heap.svg" alt="UML Class Diagram for heap" style="width: 100%; height: auto; max-width: 100%; display: block;">

Array-based min/max heap that is more efficient than its tree-based sibling [heap_tree&lt;T,P,M&gt;](#heap_treet-p-m) almost across the board, but especially in terms of cache locality, direct access speed, execution time and memory overhead. Uses a resizable [dyn_array&lt;T&gt;](#dyn_arrayt) for its internal array structure and defers to this member for all memory management, is_empty() and size() methods, ostream << operator and all comparison operators. Min/max type is stored as a member, but __heap&lt;T&gt;__ provides no methods to change this member after construction.

Implements array counterparts of all standard heap operations in a manner that is consistent with [heap_tree&lt;T,P,M&gt;] conventions.(#heap_treet-p-m).

When constructed from or assigned a [dyn_array&lt;T&gt;](#dyn_arrayt) or an initializer list, __heap&lt;T&gt;__ copies values to its internal array, then heapifies it by sifting down each array index corresponding to a non-leaf node. Efficiently handles assignments to and from [heap_tree&lt;T,P,M&gt;](#heap_treet-p-m) by leveraging their matching structures.

Notation for __heap&lt;T&gt;__ big-O:
- N: Number of heap elements.

&nbsp;

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`heap()` | Default constructor instantiates a min heap with no values. | O(1) | O(1) | O(1) |
|`heap(bool is_max)` | Construct an empty heap of given type. | O(1) | O(1) | O(1) |
|`heap(const T& v)` | Construct a min heap holding a single value v. | O(1) | O(1) | O(1) |
|`heap(const T& v, bool is_max)` | Construct a heap with given type and a single value v. | O(1) | O(1) | O(1) |
|`heap(std::initializer_list<T> list, bool is_max)` | Assigns the initializer list of values to its internal dyn_array directly, then sifts down each element starting from last non-leaf node going backwards. | O(N) | O(N) | O(1) |
|`heap(const dyn_array<T>& source, bool is_max)` | Assigns the dyn_array to its heap's dyn_array member directly, then sifts down each element starting from last non-leaf node going backwards. | O(N) | O(N) | O(1) |
|`~heap()` | Destructor executes no additional steps and relies on dyn_array to clean up after itself. | O(N) | O(N) | O(1) |
|`is_max() const : bool` | Returns true if heap is a max heap. | O(1) | O(1) | O(1) |
|`is_empty() const : bool` | Returns true if heap's internal dyn_array is empty. | O(1) | O(1) | O(1) |
|`size() const : size_t` | Returns the size of heap's internal dyn_array. | O(1) | O(1) | O(1) |
|`clear() : heap<T>&` | Clears heap's internal dyn_array. | O(N) | O(N) | O(1) |
|`operator == (const heap<T>& other) const : bool` | Returns true if internal dyn_array members are equal. | O(1) | O(N) | O(1) |
|`operator != (const heap<T>& other) const : bool` | Returns true if internal dyn_array members are not equal. | O(1) | O(N) | O(1) |
|`operator < (const heap<T>& other) const : bool` | Returns true if internal dyn_array member is less than other's. | O(1) | O(N) | O(1) |
|`operator > (const heap<T>& other) const : bool` | Returns true if internal dyn_array member is greater than other's. | O(1) | O(N) | O(1) |
|`operator <= (const heap<T>& other) const : bool` | Returns true if internal dyn_array member is less than or equal to other's. | O(1) | O(N) | O(1) |
|`operator >= (const heap<T>& other) const : bool` | Returns true if internal dyn_array member is greater than or equal to other's. | O(1) | O(N) | O(1) |
|`operator = (std::initializer_list<T> list) : heap<T>&` | Builds a dyn_array from initializer list, then assigns it to itself using dyn_array assignment operator. | O(N) | O(N) | O(N) |
|`operator = (const dyn_array<T>& arr) : heap<T>&` | Assigns dyn_array directly to its internal member, then heapifies it. | O(N) | O(N) | O(1) |
|`operator = (heap_tree<T, P, M>* hpt) : heap<T>&` | Clears and rebuilds heap from a heap_tree of any type. | O(N) | O(N) | O(N) |
|`insert(const T& val) : heap<T>&` | Inserts a new element, preserving heap properties. | O(logN) | O(logN) | O(1) |
|`remove() : T` | Removes root and returns its value, preserving heap properties. | O(logN) | O(logN) | O(1) |
|`peek() const : T` | Peek at the min/max value at the root without removing it. | O(1) | O(1) | O(1) |
|`friend operator << (std::ostream& os, const heap<T>& hp) : std::ostream&` | Print heap contents. | O(N) | O(N) | O(1) |

&nbsp;

### trie

<img src="uml/cppdata_trie.svg" alt="UML Class Diagram for trie" style="width: 100%; height: auto; max-width: 100%; display: block;">

A specialized, tree-structured [graph_node&lt;char&gt;](#graph_nodet) that implements a general-purpose prefix tree. Always maintains a root node with '~' character as the "start node", even when empty. Marks the ends of valid phrases with `\0` null termination character nodes, therefore cannot handle string inputs containing this character. All other characters are valid __trie__ node values.

Like its parent class, __trie__ is node-only and self-referential. Following in the footsteps of [node&lt;T, D&gt;](#nodet-d) and [binary_tree&lt;T, P&gt;](#binary_treet-p), __trie__ recursively manages all its node memory, and restricts itself to heap memory with static create methods and protected constructors. Provides extensive support for std::string sources for creations and assignments.

Use to efficiently store, access and manipulate vocabularies and lexicons, search for string prefixes and get string autocomplete suggestions.

&nbsp;

Notation for __trie__ big-O:
- L: Length of a string.
- S: Number of strings.
- N: Total number of nodes in the trie.
- K: Number of autocompletes for a prefix.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`static create() : trie*` | Heap-instantiate node without value. Creates a '~' node. | O(1) | O(1) | O(1) |
|`static create(const std::string& str) : trie*` | Heap-instantiate trie with a single string by breaking it up at spaces. | O(S) | O(S) | O(1) |
|`static create(const dyn_array<std::string>& dyn) : trie*` | Heap-instantiate trie with a dyn_array of strings with no breaking. | O(SL) | O(SL) | O(1) |
|`~trie()` | Destructor recursively deletes all descendant nodes. | O(N) | O(N) | O(logN) |
|`clone() const : trie*` | Heap-instantiates an identical trie, returns pointer to its root node. | O(N) | O(N) | O(logN) |
|`is_empty() : bool` | Returns true if node has no children. | O(1) | O(1) | O(1) |
|`size() : size_t` | Returns the number of nodes in the trie. | O(N) | O(N) | O(N) |
|`clear() : trie&` | Deletes all descendant nodes. | O(N) | O(N) | O(logN) |
|`operator == (const trie& other) const : bool` | Recursively compares each trie node value and sorted values of its child nodes, returns true if they all match. | O(1) | O(N) | O(logN) |
|`operator != (const trie& other) const : bool` | Returns true if tries are not equal. | O(1) | O(N) | O(logN) |
|`operator = (const trie& source) : trie&` | Clears trie, then clones the source trie's structure. | O(N) | O(N) | O(logN) |
|`operator = (const std::string& str) : trie&` | Clears trie, breaks up the string into space-separated words, then inserts each word one by one. | O(N+S) | O(N+S) | O(L) |
|`operator = (std::initializer_list<std::string> list) : trie&` | Clears trie, builds a dyn_array from initializer list, then assigns it to itself using dyn_array assignment operator. | O(N+SL) | O(N+SL) | O(SL) |
|`operator = (const dyn_array<std::string>& source) : trie&` | Clears trie, then inserts dyn_array strings one by one. | O(N+SL) | O(N+SL) | O(1) |
|`search(const std::string& str) const : dyn_array<const trie*>` | Searches for a string in the trie, returns its full path if found. | O(L) | O(L) | O(L) |
|`find_prefix(const std::string& str) const : dyn_array<const trie*>` | Searches for a prefix in the trie, returns its full path if found. | O(L) | O(L) | O(L) |
|`autocomplete(const std::string& prefix) const : dyn_array<std::string>` | Collects all the autocompletes for a given prefix input. | O(KL) | O(KL) | O(KL) |
|`get_strings(dyn_array<std::string>& str_arr) const : void` | Collects all the valid strings in the trie recursively. | O(SL) | O(SL) | O(SL) |
|`insert(const std::string& str) : trie&` | Inserts a string into trie, creating new nodes as needed. | O(L) | O(L) | O(1) |
|`remove(const std::string& str) : trie&` | Removes a string from trie, deleting nodes as needed. | O(L) | O(L) | O(1) |
|`friend operator << (std::ostream& os, const trie* tr) : std::ostream&` | Prints trie's strings. | O(SL) | O(SL) | O(SL) |
|`friend operator << (std::ostream& os, const trie& tr) : std::ostream&` | Prints trie's strings in sorted order. | O(NLlogN) | O(NLlogN) | O(SL) |

&nbsp;

### hash_table&lt;K, V&gt;

<img src="uml/cppdata_hash_table.svg" alt="UML Class Diagram for hash_table" style="width: 100%; height: auto; max-width: 100%; display: block;">

Type-aware hash table implemented with a fixed-size raw array of [node&lt;hash_node&lt;K, V&gt;, false&gt;](#nodet-d) chains. Uses __hash_node&lt;K, V&gt;__ utility class to store key-value pairs. All hash functions are encapsulated in another utility class __hash_fn&lt;K&gt;__, which picks the optimal choice for each supported K key type via its full template specializations. Hash functions generate an index for the internal raw array from key objects, then actual searches, insertions and deletions at that index are executed using [node&lt;hash_node&lt;K, V&gt;, false&gt;](#nodet-d) methods and equality operators defined by K type. If an existing key is inserted again, its new value will replace the old one. Owns all its memory, managing raw array memory directly, and the memory for linked lists from their head nodes.

Supports all fundamental C++ data types, std::string, and all cppdata data structure classes as keys (represented with blue "uses" links in UML). The consistent availability of deep equality check and friend ostream << operators on all data structure classes permits __hash_table&lt;K, V&gt;__ to use these operators to generate strings that fully describe the internal structure of each class instance, hash these strings with polynomial rolling hash function just like regular std::string hashing, and execute deep structural comparisons for keys that are part of the same linked list. Heap-only classes need to grant __hash_node&lt;K, V&gt;__ utility class friend access in order to be hashable, otherwise they can't be copied to its key_ member.

For non-templated K key types, __hash_table&lt;K, V&gt;__'s hash_ member uses the matching template specialization of `hash_fn<K>::run` method, whereas for templated ones, full class template specializations are needed since methods can't be partially specialized. Below is a complete list of supported K types, together with the optimal hash function picked by __hash_fn&lt;K&gt;__ for each one:

| Category | Types | Hash Function |
|----------|-------|:-------------:|
| signed integral | int, short, char | signed modulus |
| unsigned integral | unsigned, unsigned short, unsigned char | unsigned modulus |
| floating-point | float, double | unsigned modulus on floating-point bit pattern |
| string container | std::string, trie | rolling polynomial |
| pointer | uintptr_t | fowler-noll-vo, jenkins or murmur depending on PtrHashType selected |
| templated cppdata | dyn_array, node, linked_list, stack, queue, graph_node, graph, binary_tree, binary_search_tree, avl_tree, red_black_tree, heap_tree, heap | rolling polynomial |

Three different hash functions are offered for uintptr_t pointer keys and __hash_table&lt;K, V&gt;__ instances are bound to using one of them at construction time with PtrHashType enum set. For all other types, __hash_fn&lt;K&gt;__ picks what's best without providing options.

Any class that wants to use a __hash_table&lt;K, V&gt;__ but also wants to be hashable itself, creates what appears to be a co-dependency relationship that risks creating compiler parsing issues due to circular includes. This risk is particularly acute with [graph_node&lt;T&gt;](#graph_nodet) which makes extensive use of __hash_table&lt;K, V&gt;__ and therefore has to include its header, and __hash_table&lt;K, V&gt;__ needs access to all classes it supports to be able to hash them. Complicating matters further, [graph_node&lt;T&gt;](#graph_nodet) has a lot of descendant classes that also use __hash_table&lt;K, V&gt;__ in their methods, which requires them to have access to both of these classes. Unless properly handled, this chain of dependencies can be a hotbed of dormant circular includes waiting to be triggered by a specific include order.

One observation is key to understanding how this is resolved in cppdata: What appears to be a co-dependency at first glance are actually two different use cases that are operationally quite separate, because the call site either wants to hash a cppdata class as keys, in which case it needs to include the class header, or it has no need for the class and doesn't include its header, in which case there is no risk of circular includes. This means that __hash_table&lt;K, V&gt;__ can rely entirely on forward declarations to implement all __hash_fn&lt;K&gt;__ template specializations, anticipating that when a class needs to be hashed its header will be included, and definitions of all forward-declared classes, methods and operators will become available. If class header is not included, the specialization for that class cannot be invoked, allowing code to compile.

&nbsp;

Notation for __hash_table&lt;K, V&gt;__ big-O:
- N: Number of key-value pairs in the hash table.
- T: Fixed table size.

| Method Signature | Description | Average | Worst | Space |
|------------------|-------------|:-------:|:-------:|:-----:|
|`hash_table()` | Default constructor uses prime number 1009 for raw array size and FNV for pointer hash function. | O(1) | O(1) | O(1) |
|`hash_table(size_t table_size)` | Constructs hash_table with given array size. Uses FNV for pointer hash function. | O(T) | O(T) | O(1) |
|`hash_table(size_t table_size, PtrHashType ptr_h)` | Constructs hash_table given array size and pointer hash function. | O(T) | O(T) | O(1) |
|`~hash_table()` | Destructors calls delete on each linked list head, then deallocates the raw array member. | O(N+T) | O(N+T) | O(1) |
|`operator [] (const K& key) const : V` | Returns a copy of key's value in hash_table. | O(1) | O(N) | O(1) |
|`contains(const K& key) const : bool` | Returns true if key exists in hash_table. | O(1) | O(N) | O(1) |
|`size() const : size_t` | Returns the size of hash_table's raw array member. | O(1) | O(1) | O(1) |
|`clear() : hash_table<K, V>&` | Deletes all linked lists and nullifies raw array elements. | O(N+T) | O(N+T) | O(1) |
|`insert(const K& key, const V& val) : hash_table<K, V>&` | Inserts a key-value pair to the hash_table. | O(1) | O(N) | O(1) |
|`insert(const hash_node<K, V>& hn) : hash_table<K, V>&` | Inserts a hash_node to the hash_table. | O(1) | O(N) | O(1) |
|`remove(const K& key) : hash_table<K, V>&` | Deletes the first matching entry in given key's hash bucket. | O(1) | O(N) | O(1) |
|`keys() const : dyn_array<K*>` | Return a dyn_array of pointers to all the keys in the hash table. | O(N+T) | O(N+T) | O(N) |
|`vals() const : dyn_array<V*>` | Return a dyn_array of pointers to all the values in the hash table. | O(N+T) | O(N+T) | O(N) |
|`friend operator << (std::ostream& os, const hash_table<K, V>& ht) : std::ostream&` | Prints hash_table contents. | O(N+T) | O(N+T) | O(1) |

&nbsp;

## License

MIT License. See LICENSE file for details.

Copyright (c) 2025 Anil Usumezbas. All rights reserved.