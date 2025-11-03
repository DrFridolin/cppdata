#ifndef trie_h_
#define trie_h_

#include <iostream>
#include <string>
#include <sstream>
#include "dyn_array.h"
#include "graph.h"

template <class K, class V> class hash_node;

// Tree-structured graph_node<char> implementing a general-purpose prefix tree.
// Always maintains a root node with '~' character as the "start node".
// Marks the ends of valid phrases with `\0` null termination character,
// therefore cannot handle string inputs containing this character.
// All other characters are valid trie node values.
//
// Like its parent class, trie is node-only and self-referential.
// Following the design of node and binary_tree, it restricts itself to heap
// memory with static create methods and protected constructors.
// Provides extensive support for std::string in its static create methods and
// assignment operators. Owns and manages all memory for its downstream chain.
// Use to efficiently store, access and manipulate vocabularies and lexicons,
// searh for string prefixes and get string autocomplete suggestions.
//
// Notation for big-O:
//   A: Alphabet size or node degree (constant, e.g., 256 for char).
//   L: Length of a string.
//   S: Total number of strings in trie.
//   N: Total number of nodes in the trie.
//   M: Total number of nodes created by S strings of length L.
//   K: Total number of autocompletes for a prefix.
//   Note: Since A is fixed, terms like log A or D are absorbed into the
//         constants for big-O notation, following standard trie complexity
//         conventions where operations are O(L) ignoring alphabet size.
//         Space complexities refer to auxiliary space or space for new
//         structures created, as applicable.
class trie : public graph_node<char>
{
  // Allows hash_node access to the protected copy constructor for hashing.
  template <class K, class V> friend class hash_node;

  public:
    // Heap-instantiate node without value. Creates a '~' node. O(1) | O(1)
    static trie* create() {return new trie();}
    // Heap-instantiate trie with a string broken up at spaces. O(S) | O(M)
    static trie* create(const std::string& str);
    // Heap-instantiate and insert strings from a dyn_array. O(S*L) | O(M)
    static trie* create(const dyn_array<std::string>& dyn);
    // Destructor recursively deletes all descendant nodes. O(N) | O(logN)
    virtual ~trie();
    // Clone method replaces copy constructor functionality. O(N) | O(N)
    trie* clone() const;

    // Root node should never be removed except when trie is destroyed.
    // Therefore root without any children is considered empty. O(1) | O(1)
    bool is_empty() {return this->degree() == 0;}
    // Count and return the number of nodes in the trie. O(N) | O(N)
    size_t size() {return this->breadth_first_traverse().size();}
    // Deletes all descendant nodes. O(N) | O(logN)
    trie& clear();

    // Equality check operators compare sorted arrays of trie string contents.
    // O(1) average, O(N) worst | O(logN)
    bool operator == (const trie& other) const;
    bool operator != (const trie& other) const;
    // Clones source trie's structure. O(N) | O(N)
    trie& operator = (const trie& source);
    // Breaks up the string at spaces and inserts each element. O(N+S) | O(M)
    trie& operator = (const std::string& str);
    // Builds dyn_array from init list and assigns it to trie. O(N+S*L) | O(M)
    trie& operator = (std::initializer_list<std::string> list);
    // Inserts strings in dyn_array one by one. O(N+S*L) | O(M)
    trie& operator = (const dyn_array<std::string>& source);

    // Search for a string in trie, return its full path if found. O(L) | O(L)
    dyn_array<const trie*> search(const std::string& str) const;
    // Search for a prefix in trie, return its full path if found. O(L) | O(L)
    dyn_array<const trie*> find_prefix(const std::string& str) const;
    // Collect all the autocompletes for a given prefix input. O(K*L) | O(K*L)
    dyn_array<std::string> autocomplete(const std::string& prefix) const;
    // Collect all the valid strings in the trie recursively. O(S*L) | O(S*L)
    void get_strings(dyn_array<std::string>& str_arr) const;

    // Insert a string into the trie, creating new nodes as needed. O(L) | O(L)
    trie& insert(const std::string& str);
    // Remove a string from the trie, deleting nodes as necessary. O(L) | O(1)
    trie& remove(const std::string& str);

    // Prints trie strings to output stream. O(S*L) | O(S*L)
    friend std::ostream& operator << (std::ostream& os, const trie* tr);
    // Prints sorted trie strings to output stream. O(N*L*logN) | O(S*L)
    friend std::ostream& operator << (std::ostream& os, const trie& tr);

  private:
    // Default constructor created a '~' default-value node. O(1) | O(1)
    trie(): graph_node<char>('~', 0) {}
    // Construct a single trie node holding the given character. O(1) | O(1)
    trie(char ch): graph_node<char>(ch, 0) {}
    // Construct trie node with value and number of children. O(A) | O(A)
    trie(char ch, size_t n): graph_node<char>(ch, n) {}
    // Copy constructor clones trie structure for a deep copy. O(N) | O(N)
    trie(const trie& source);
    // Recursively collect all strings in the trie. O(S*L/A) | O(S*L/A)
    bool collect(dyn_array<std::string>& s_arr, std::string& cur) const;
    // Non-const search to be used in trie-editing methods. O(L) | O(L)
    dyn_array<trie*> search_to_edit(const std::string& str);
    // Non-const find_prefix to be used in trie-editing methods. O(L) | O(L)
    dyn_array<trie*> find_prefix_to_edit(const std::string& str);
};

// Even though trie is not templated, implementations are placed in a tpp
// file to maintain the header-only nature of the library.
#include "trie.tpp"

#endif