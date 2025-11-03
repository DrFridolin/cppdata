#ifndef linked_list_h_
#define linked_list_h_

#include <iostream>
#include <initializer_list>
#include "dyn_array.h"

template <class T, bool D> class node;
template <class K, class V> class hash_node;

// Base class that only holds next_ pointer for singly linked lists, and
// establishes both read/write and const access to it. Trying to call prev()
// on a singly linked list (D=false) will result in undefined behavior.
template <class T, bool D>
class node_base
{
  public:
    node_base(): val_(T()), next_(nullptr) {}
    node_base(const T& val): val_(val), next_(nullptr) {}
    ~node_base() {}

    node<T, D>*& next() {return this->next_;}
    node<T, D>*& prev() {return this->next_;}
    const node<T, D>* next() const {return this->next_;}
    const node<T, D>* prev() const {return nullptr;}

    const T& val() const {return this->val_;}
    T& val() {return this->val_;}

  protected:
    T val_;
    node<T, D>* next_;
};
// Specialized base class adds prev_ pointer for doubly linked lists, and
// establishes both read/write and const access to them.
template <class T>
class node_base<T, true>
{
  public:
    node_base(): next_(nullptr), prev_(nullptr) {}
    node_base(const T& val): val_(val), next_(nullptr), prev_(nullptr) {}
    ~node_base() {}

    node<T, true>*& next() {return this->next_;}
    node<T, true>*& prev() {return this->prev_;}
    const node<T, true>* next() const {return this->next_;}
    const node<T, true>* prev() const {return this->prev_;}

    const T& val() const {return this->val_;}
    T& val() {return this->val_;}

  protected:
    T val_;
    node<T, true>* next_;
    node<T, true>* prev_;
};

// Node-only class for singly (D=false) and doubly (D=true) linked lists.
// As self-referential linked list representations, node<T, D> chains are
// handled from their head node pointers.
// Creation of node<T, D> instances are restricted to heap memory through a
// static create method that takes a value argument and creates a single
// node<T, D> on heap holding the given value.
//
// A node instance owns the memory for the entire chain in front of it.
// Methods that can change the head node return a pointer to the node occupying
// the head position at the time of return, which will be lost unless captured
// at call site. Tail-editing methods return a pointer to tail position in the
// same way if their head nodes are safe, which eliminates the need for another
// traversal for post processing the new tail in some cases. It also allows
// linked_list<T, D> wrapper class to easily update its tail pointer after
// running tail-editing node method.
//
// Implements four sorting algorithms that are native to linked lists;
// algorithms utilizing frequent random access are deemed incompatible and/or
// suboptimal. The variant of merge_sort in node<T, D> reaches better
// asymptotic worst-case time complexity than its dyn_array<T> variant, and
// uses less space by foregoing an auxiliary structure. The use of a fully
// iterative variant brings down space complexity from O(logN) down to O(1).
// Therefore, merge_sort is the optimal way to sort a node<T, D> chain.
//
// Assumes T is default-constructable with equality and relational operators.
// Loops and self-intersecting linked lists are consigned to linked_list<T, D>
// and are not supported at the node-only level. Using either append_node or
// prepend_node to link a chain to itself will create such a loop, causing
// double deletion issues and infinite loops for most traversal attempts.
// Use linked_list<T, D> to represent and handle looping linked lists.
// Observes the rule of 3.
template <class T, bool D>
class node : public node_base<T, D>
{
  // Allows hash_node access to the protected copy constructor for hashing.
  template <class K, class V> friend class hash_node;
  template <class T> friend class dyn_array;

  public:
    // Creates a new node on the heap. O(1) | O(1)
    static node<T, D>* create(const T& val) {return new node<T, D>(val);}
    // Recursively deletes all nodes in the forward direction only. O(N) | O(N)
    ~node();

    // Clones node with its linked chain, in both directions if doubly-linked.
    // Returns pointer to the head of the cloned chain. O(N) | O(N)
    node<T, D>* clone() const;
    // Clones node with its linked chain in the forward direction only.
    // Returns pointer to clone of this. O(N) | O(N)
    node<T, D>* clone_fwd() const;
    // Clones doubly-linked node chain in the backwards direction only.
    // Returns pointer to the clone, or nullptr if singly-linked. O(N) | O(N)
    node<T, D>* clone_bwd() const;
    // Clones only the current node without any of its linked nodes.
    // Returns pointer to cloned node. O(1) | O(1)
    node<T, D>* clone_node() const;

    // Sets node's value. O(1) | O(1)
    node<T, D>* set_val(const T& val);
    size_t size() const;
    // Recursively deletes all nodes in the forward direction. O(N) | O(N)
    node<T, D>* clear();

    // Equality check operators compare the entire node chain.
    // O(1) average, O(N) worst | O(1)
    bool operator == (const node<T, D>& other) const;
    bool operator != (const node<T, D>& other) const;
    // Relational operators compare node values first, chain sizes second.
    // O(1) average, O(N) worst | O(1)
    bool operator < (const node<T, D>& other) const;
    bool operator > (const node<T, D>& other) const;
    bool operator <= (const node<T, D>& other) const;
    bool operator >= (const node<T, D>& other) const;
    // Performs a deep copy of the source chain. O(N+M) | O(N)
    node<T, D>& operator = (const node<T, D>& source);
    // Creates a new node chain from an initializer list. O(N+M) | O(N)
    node<T, D>& operator = (std::initializer_list<T> list);
    // Creates a new node chain from a dyn_array. O(N+M) | O(N)
    node<T, D>& operator = (const dyn_array<T>& source);

    // Returns min/max value and pointer to the preceding node. O(N) | O(1)
    node<T, D>* min(T& min_val);
    node<T, D>* max(T& max_val);
    // Traverses back with prev() accessors to find and return chain's head.
    // If singly-linked, returns this pointer without traversing. O(N) | O(1)
    node<T, D>* find_head();
    const node<T, D>* find_head() const;
    // Traverses forward to return chain's tail. O(N) | O(1)
    node<T, D>* find_tail();
    const node<T, D>* find_tail() const;
    // Searches by value and returns node pointer or nullptr. O(N) | O(1)
    node<T, D>* search(const T& val);
    // If query is in chain, returns ptr to preceding node; nullptr otherwise.
    // Also returns nullptr if query is head, caller has to check. O(N) | O(1)
    node<T, D>* contains(const node<T, D>* query);
    node<T, D>* contains(const T& val);

    // Returns new tail since head will be unchanged. O(N) | O(1)
    node<T, D>* append(const T& val);
    // Returns new head, which can be lost unless assigned. O(1) | O(1)
    node<T, D>* prepend(const T& val);
    // Adds a node and its forward chain to tail, returns head. O(N) | O(1)
    node<T, D>* append_node(node<T, D>* n_ptr);
    // Adds a node and its backward chain to head, returns head. O(1) | O(1)
    node<T, D>* prepend_node(node<T, D>* n_ptr);
    // Detaches and deletes head, returns next as new head. O(1) | O(1)
    node<T, D>* delete_head();
    // Finds and deletes tail, returns the new tail. O(N) | O(1)
    node<T, D>* delete_tail();
    // Deletes 1st node with matching value in front. Returns head. O(N) | O(1)
    node<T, D>* delete_node(const T& val);

    // Swaps nodes if doubly-linked, just values if singly-linked. O(1) | O(1)
    node<T, D>* swap(node<T, D>* other);
    // Reverses the order, returns new head. O(N) | O(1)
    node<T, D>* reverse();

    // In-place and stable. Returns head. O(N^2) | O(1)
    node<T, D>* bubble_sort(bool inc = true);
    // In-place and stable. Returns head. O(N^2) | O(1)
    node<T, D>* insertion_sort(bool inc = true);
    // Out-of-place, stable and iterative. Returns head. O(N*logN) | O(1)
    node<T, D>* merge_sort(bool inc = true);
    // In-place and stable. Returns head. O(N^2) | O(1)
    node<T, D>* selection_sort(bool inc = true);

    // Prints node chain contents. O(N) | O(1)
    template <class T, bool D>
    friend std::ostream& operator << (std::ostream& os,
                                      const node<T, D>* n_ptr);
    // Compactly prints node chain contents by omitting arrows. O(N) | O(1)
    template <class T, bool D>
    friend std::ostream& operator << (std::ostream& os,
                                      const node<T, D>& n_ins);
    // Recursive function to print the doubly-linked nodes behind this node.
    void print_prev(std::ostream& os) const;

  protected:
    node(): node_base<T, D>() {}
    node(const T& val): node_base<T, D>(val) {}
    // Copy constructor clones the entire list linked to the front of source.
    node(const node<T, D>& source);
    // Returns true this node coming before the other complies with sort order.
    bool compare(const node<T, D>* other, bool inc = true);
    // Merges two sorted linked lists during merge_sort using auxiliary space.
    node<T, D>* merge(bool inc, node<T, D>* low_ptr, node<T, D>* high_ptr);
};

// Linked list wrapper class manages a node chain by continuously tracking its
// head and tail nodes only, and represents its empty state by nullifying both.
// Provides a cleaner way to manage and interact with linked list chains.
// Owns the memory for the full node chain and manages it via its head pointer,
// similar to how node<T, D> manages its memory. Observes the rule of 3.
// Assumes T is default-constructable and has equality/inequality operators.
//
// Many doubly-linked list operations reach optimal performance only with the
// continuous availability of O(1) tail access, so they are the de-facto use
// cases for this class. Singly-linked lists are still supported but their
// node-only variants are typically preferred for their lower overhead.
// Significant portions of core node<T, D> functionality is directly reused via
// direct calls to its methods whenever there is overlap and performance is
// optimal with or without the tail pointer.
//
// The availability of both head and tail pointers enables linked_list<T, D> to
// handle heads that loop back to a node in the chain (doubly-linked only), and
// tails that loop forward to a node in the chain (possible in both singly- and
// doubly-linked). This is because head and tail pointers mark actual endpoints
// of the linked list so that it can be traversed and processed without getting
// stuck in an infinite loop. The only way to create a looping linked list is
// to use append_node and prepend_node methods to link nodes or node chains
// directly; therefore these methods routinely scan the linked list with
// Floyd's tortoise-and-hare algorithm to account for any loops that may form
// or break as a result. All traversing methods operate head-to-tail and break
// all existing loop(s) if they need to, to be reattached post-op if possible.
// Recursive memory management is also extended to break loops before deletion.
//
// If linked_list<T, D> contains loop(s), head/tail pointers will be linked to
// the loop intersection(s). The straightforward way to look for loops in a
// linked_list<T, D> at any given time is to check if its tail has a successor
// node (tail loop) or if its head has a predecessor node ( head loop).
// Since linked_list<T, D> always keeps track of its head and tail nodes, the
// expectation is for both head()->prev() and tail()->next() to be nullptr
// except when loops are present.
template <class T, bool D>
class linked_list
{
  public:
    // Default constructor initializes head and tail as nullptr. O(1) | O(1)
    linked_list(): head_(nullptr), tail_(nullptr) {}
    // Constructs a single-node linked list. O(1) | O(1)
    linked_list(const T& val): head_(node<T, D>::create(val)), tail_(head_) {}
    // Constructs from an initializer_list. O(N) | O(N)
    linked_list(std::initializer_list<T> list);
    // Breaks head and tail loops when copying. O(N) | O(N)
    linked_list(const linked_list<T, D>& source);
    // Destructor breaks tail loops and deletes head. O(N) | O(N)
    ~linked_list();

    // Read/write and read-only accessors to head and tail nodes. O(1) | O(1)
    node<T, D>*& head() {return this->head_;}
    node<T, D>*& tail() {return this->tail_;}
    const node<T, D>* head() const {return this->head_;}
    const node<T, D>* tail() const {return this->tail_;}

    // Returns true if linked_list is empty. O(1) | O(1)
    bool is_empty() const {return this->head_ == nullptr;}
    // Returns the number of nodes in the linked list. O(N) | O(1)
    size_t size() const;
    // Breaks tail loops, deletes head, nullifies head and tail. O(N) | O(N)
    linked_list<T, D>& clear();

    // Equality check operators compare linked lists head-to-tail.
    // O(1) average, O(N) worst | O(1)
    bool operator == (const linked_list<T, D>& other) const;
    bool operator != (const linked_list<T, D>& other) const;
    // Relational operators compare head values first, list sizes second.
    // O(1) average, O(N) worst | O(1)
    bool operator < (const linked_list<T, D>& other) const;
    bool operator > (const linked_list<T, D>& other) const;
    bool operator <= (const linked_list<T, D>& other) const;
    bool operator >= (const linked_list<T, D>& other) const;
    // Clears and rebuilds as a deep copy of source linked_list. O(N+M) | O(N)
    linked_list<T, D>& operator = (const linked_list<T, D>& source);
    // Clears and rebuilds linked_list from an initializer list. O(N+M) | O(N)
    linked_list<T, D>& operator = (std::initializer_list<T> list);
    // Clears and rebuilds as a deep copy of source node chain. O(N+M) | O(N)
    linked_list<T, D>& operator = (const node<T, D>& source);
    // Clears and rebuilds linked_list from dyn_array values. O(N+M) | O(N)
    linked_list<T, D>& operator = (const dyn_array<T>& source);

    // Returns min/max value and pointer to the preceding node. O(N) | O(1)
    node<T, D>* min(T& min_val);
    node<T, D>* max(T& max_val);
    // Forward-searches by value, returns found node or nullptr. O(N) | O(1)
    node<T, D>* search(const T& val);
    // If query is in chain, returns ptr to preceding node; nullptr otherwise.
    // Also returns nullptr if query is head, caller has to check. O(N) | O(1)
    node<T, D>* contains(const node<T, D>* query);
    node<T, D>* contains(const T& val);

    // Creates and appends new tail to non-tail-looping list. O(1) | O(1)
    linked_list<T, D>& append(const T& val);
    // Appends new tail by pointer, then checks for a tail loop. O(N+M) | O(1)
    linked_list<T, D>& append_node(node<T, D>* n_ptr);
    // Creates and appends a new head to a non-head-looping list. O(1) | O(1)
    linked_list<T, D>& prepend(const T& val);
    // Appends new head by pointer, then checks for head loop if doubly-linked.
    // O(1) singly-linked, O(N+M) doubly-linked | O(1)
    linked_list<T, D>& prepend_node(node<T, D>* n_ptr);
    // Breaks head loops, deletes head and updates head pointer. O(1) | O(1)
    linked_list<T, D>& delete_head();
    // Breaks tail loops, deletes tail and updates tail pointer.
    // O(N) singly-linked, O(1) doubly-linked | O(1)
    linked_list<T, D>& delete_tail();
    // Deletes first node with matching val. Keeps unbroken loops. O(N) | O(1)
    linked_list<T, D>& delete_node(const T& val);

    // Floyd's tortoise-and-hare cycle detector for head loops. Returns the
    // intersection node, or nullptr if singly-linked/non-looping. O(N) | O(1)
    node<T, D>* detect_head_loop() const;
    // Floyd's tortoise-and-hare cycle detector for tail loops. Returns the
    // intersection node, or nullptr if non-looping. O(N) | O(1)
    node<T, D>* detect_tail_loop() const;
    // Breaks head/tail loop(s), returns intersection, or nullptr. O(1) | O(1)
    linked_list<T, D>& break_loops(node<T, D>*& hloop, node<T, D>*& tloop);
    // Reverses the order of the linked list. O(N) | O(1)
    linked_list<T, D>& reverse();

    // In-place and stable. Breaks all loops. O(N^2) | O(1)
    linked_list<T, D>& bubble_sort(bool inc = true);
    // In-place and stable. Breaks all loops. O(N^2) | O(1)
    linked_list<T, D>& insertion_sort(bool inc = true);
    // Out-of-place, stable and iterative. Breaks all loops. O(N*logN) | O(1)
    linked_list<T, D>& merge_sort(bool inc = true);
    // In-place and stable. Breaks all loops. O(N^2) | O(1)
    linked_list<T, D>& selection_sort(bool inc = true);

    // Prints linked list contents.
    template <class T, bool D>
    friend std::ostream& operator << (std::ostream& os,
                                      const linked_list<T, D>& list);
  protected:
    node<T, D>* head_;
    node<T, D>* tail_;
};

// Templated class method implementations need to be accessible at the
// point of instantiation.
#include "linked_list.tpp"

#endif