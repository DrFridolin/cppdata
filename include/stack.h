#ifndef stack_h_
#define stack_h_

#include <iostream>
#include <cassert>
#include <initializer_list>
#include "dyn_array.h"
#include "linked_list.h"

// General-purpose stack that runs a node<T, false> chain under the hood and is
// mostly a thin wrapper around it that exposes a clean stack interface.
// Provides O(1) access to last inserted value. Observes the rule of 3.
// Keeps track of its own size and represents its empty state by nullifying
// its node pointer member. Owns the memory for its linked list member.
// Assumes T is default-constructable with equality and relational operators.
template <class T>
class stack
{
  public:
    // Default constructor creates an empty stack. O(1) | O(1)
    stack(): list_(nullptr), size_(0) {}
    // Initializes stack with a single value. O(1) | O(1)
    stack(const T& val): list_(node<T, false>::create(val)), size_(1) {}
    // Constructs stack from an initializer_list. O(N) | O(N)
    stack(std::initializer_list<T> list);
    // Performs a deep copy of the source's node<T, false> chain. O(N) | O(N)
    stack(const stack<T>& source);
    // Destructor deletes node pointer member. O(N) | O(N)
    ~stack();

    // Creates and prepends new node with the given value. O(1) | O(1)
    stack<T>& push(const T& val);
    // Deletes head node and returns its value. O(1) | O(1)
    T pop();
    // Returns head node val without deleting it, or T() if empty. O(1) | O(1)
    T peek() {return this->list_->val();}

    // Returns true if node pointer member is set to nullptr. O(1) | O(1)
    bool is_empty() const {return this->list_ == nullptr;}
    // Returns stack size directly from member used for tracking. O(1) | O(1)
    size_t size() const {return this->size_;}
    // Deletes and nullifies list_ member. Sets size to 0. O(N) | O(N)
    stack<T>& clear();

    // Equality check operators compare the underlying linked lists.
    // O(1) average, O(N) worst | O(1)
    bool operator == (const stack<T>& other) const;
    bool operator != (const stack<T>& other) const;
    // Relational operators defer to their node<T, false> counterparts.
    // O(1) average, O(N) worst | O(1)
    bool operator < (const stack<T>& other) const;
    bool operator > (const stack<T>& other) const;
    bool operator <= (const stack<T>& other) const;
    bool operator >= (const stack<T>& other) const;
    // Performs a deep copy of source with its node chain. O(N+M) | O(N)
    stack<T>& operator = (const stack<T>& source);
    // Clears stack and assigns the init list to list_ member. O(N+M) | O(N)
    stack<T>& operator = (std::initializer_list<T> list);
    // Clones the source node chain into its member. O(N+M) | O(N)
    stack<T>& operator = (const node<T, false>* source);
    // Clears stack and assigns dyn_array to its list_ member. O(N+M) | O(N)
    stack<T>& operator = (const dyn_array<T>& source);

    // Prints stack contents. O(N) | O(1)
    template <class U>
    friend std::ostream& operator << (std::ostream& os, const stack<U>& st);

  protected:
    node<T, false>* list_;
    size_t size_;
};

// Templated class method implementations need to be accessible at the
// point of instantiation
#include "stack.tpp"

#endif