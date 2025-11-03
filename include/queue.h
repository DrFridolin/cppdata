#ifndef queue_h_
#define queue_h_

#include <iostream>
#include <cassert>
#include <initializer_list>
#include "dyn_array.h"
#include "linked_list.h"

// General-purpose queue that runs a linked_list<T, true> under the hood for
// O(1) tail access, and is mostly a thin wrapper around it that exposes a
// clean queue interface.
// Provides O(1) access to earliest inserted value, as well as last inserted
// value since it comes for free with linked lists and is often useful.
// Keeps track of its own size and represents its empty state with an empty
// linked list<T, true> member.
// Owns its memory but manages no dynamically allocated memory on its own,
// deferring instead to compiler-built defaults for intra-class assignments
// and copy construction, which observes the rule of 3.
// Assumes T is default-constructable with equality and relational operators.
template <class T>
class queue
{
  public:
    // Default constructor creates an empty queue. O(1) | O(1)
    queue(): size_(0) {}
    // Initializes queue with a single value. O(1) | O(1)
    queue(const T& val): list_(linked_list<T, true>(val)), size_(1) {}
    // Constructs queue from an initializer_list. O(N) | O(N)
    queue(std::initializer_list<T> list);
    // Destructor defers to list_ member to clean up after itself. O(N) | O(N)
    ~queue() {}

    // Creates and prepends new node with the given value. O(1) | O(1)
    queue<T>& enqueue(const T& val);
    // Deletes tail node and returns its value. O(1) | O(1)
    T dequeue();
    // Returns tail node val without deleting it, or T() if empty. O(1) | O(1)
    T peek() {return this->list_.tail()->val();}
    // Access to last item is free with doubly-linked list. O(1) | O(1)
    T last() {return this->list_.head()->val();}

    // Returns true if linked_list member is empty. O(1) | O(1)
    bool is_empty() const {return this->list_.is_empty();}
    // Returns queue size directly from member used for tracking. O(1) | O(1)
    size_t size() const {return this->size_;}
    // Clears the linked_list member. Sets size to 0. O(N) | O(N)
    queue<T>& clear();

    // Equality check operators compare the underlying linked lists.
    // O(1) average, O(N) worst | O(1)
    bool operator == (const queue<T>& other) const;
    bool operator != (const queue<T>& other) const;
    // Relational operators defer to their linked_list<T, true> counterparts.
    // O(1) average, O(N) worst | O(1)
    bool operator < (const queue<T>& other) const;
    bool operator > (const queue<T>& other) const;
    bool operator <= (const queue<T>& other) const;
    bool operator >= (const queue<T>& other) const;
    // Clears queue and assigns the init list to list_ member. O(N+M) | O(N)
    queue<T>& operator = (std::initializer_list<T> list);
    // Assigns the source linked_list to its list_ member. O(N+M) | O(N)
    queue<T>& operator = (const linked_list<T, true>& source);
    // Clears queue and assigns dyn_array to its list_ member. O(N+M) | O(N)
    queue<T>& operator = (const dyn_array<T>& source);

    // Prints queue contents. O(N) | O(1)
    template <class U>
    friend std::ostream& operator << (std::ostream& os, const queue<U>& que);

  protected:
    linked_list<T, true> list_;
    size_t size_;
};

// Templated class method implementations need to be accessible at the
// point of instantiation
#include "queue.tpp"

#endif