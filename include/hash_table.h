#ifndef hash_table_h_
#define hash_table_h_

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <bitset>
#include "dyn_array.h"
#include "linked_list.h"

// Simple hash node class.
// Holds key-value pairs for hash_table.
// Uses matching template parameters to hash_table.
template <class K, class V>
class hash_node
{
  public:
    hash_node() {}
    hash_node(const K& key, const V& val): key_(key), val_(val) {}
    ~hash_node() {}
    bool operator == (const hash_node<K, V>& n) {return this->key_ == n.key_;}
    K key_;
    V val_;
};

// Unscoped global enum to pick hash function for pointer types.
enum PtrHashType {FNV, JENKINS, MURMUR};

// Hash function class templated on key type that automatically selects and
// applies the hash function most suitable for keys of type K.
// Uses its full template specializations to pick optimal hash functions.
// For non-templated K types, the matching template specialization of its run
// method is used; whereas for templated ones, full hash_fn<K> class template
// specializations are needed since methods can't be partially specialized.
// Complete list of supported K types and their optimal hash functions are:
// - signed integral: signed modulus (mod_signed)
//     int, short, char
// - unsigned integral: unsigned modulus (mod_unsigned)
//     unsigned, unsigned short, unsigned char
// - floating-point: unsigned modulus on floating-point bit pattern (mod_float)
//     float, double
// - string container: rolling polynomial (poly_roll)
//     std::string, trie
// - pointer: fowler-noll-vo (fnv), jenkins or murmur depending on PtrHashType.
//     uintptr_t
// - templated cppdata: rolling polynomial on ostream string (static poly_roll)
//     dyn_array, node, linked_list, stack, queue, graph_node, graph, heap,
//     binary_tree, binary_search_tree, heap_tree, avl_tree, red_black_tree
template <class K>
class hash_fn
{
  public:
    hash_fn(): size_(1009), ptr_hash_(FNV) {}
    hash_fn(size_t s): size_(s), ptr_hash_(FNV) {}
    hash_fn(size_t s, PtrHashType ptr_h): size_(s), ptr_hash_(ptr_h) {}
    ~hash_fn() {}

    size_t run(const K& key) const {
      std::cerr << "ERROR! Unrecognized key type." << std::endl;
      return 0;
    }

    // Simple, modulus-based hash function for unsigned keys.
    size_t mod_unsigned(unsigned key) const;
    // Modulus-based hash function for float bit pattern keys.
    size_t mod_float(uint32_t key) const;
    // Modulus-based hash function for double bit pattern keys.
    size_t mod_double(uint64_t key) const;
    // Simple, modulus-based hash function for integer keys.
    size_t mod_signed(int key) const;
    // Polynomial rolling hash function for string keys of arbitrary length.
    size_t poly_roll(const std::string& key) const;
    // Static poly_roll function holds the actual implementation to allow.
    // full class specializations to be able to call it for different classes.
    static size_t poly_roll(const std::string& key, size_t size);
    // Fowler-Noll-Vo hash function for uintptr_t pointer keys.
    size_t fnv(const uintptr_t& key) const;
    // Jenkins hash function for uintptr_t pointer keys.
    size_t jenkins(const uintptr_t& key) const;
    // Simplified version of MurmurHash function for uintptr_t pointer keys.
    size_t murmur(const uintptr_t& key) const;

  private:
    size_t size_;
    PtrHashType ptr_hash_;
};

// Type-aware hash table that supports all fundamental data types, std::string,
// and all cppdata data structure classes as keys.
// Represents its table with raw array of node<hash_node<K, V>, false> chains.
// All hash functions are encapsulated in hash_fn<K> member, which picks the
// optimal hash function for each supported K key type.
// Hash functions generate an index for the fixed-size raw array from keys,
// then actual searches, insertions and deletions at that index are executed
// using node<T, D> methods and equality operators defined by K type.
// If an existing key is inserted again, its new value will replace the old.
// Owns all its memory. Manages raw array memory directly, and the memory for
// the linked lists from their head nodes.
//
// The consistent availability of deep equality check == and friend ostream <<
// operators on all non-utility cppdata classes permits hash_table<K, V> to use
// them to generate strings that fully describe the internal structure of each
// class instance, and then hash these strings with polynomial rolling hash
// just like regular std::string keys; and execute deep structural comparisons
// for keys that are part of the same linked list.
// Heap-only classes need to grant hash_node<K, V> utility class friend access
// in order to be hashable, otherwise they can't be copied to its key_ member.
//
// Three different hash functions are on offer for uintptr_t keys and instances
// of hash_table<K, V> are bound to using one of them at construction time with
// PtrHashType enum set. For all other types, hash_ member picks what's best
// without providing options.
//
// Any class that wants to use a hash_table<K, V> but also wants to be hashable
// itself, creates what appears to be a co-dependency relationship that risks
// creating compiler parsing issues due to circular includes.
// This risk is particularly acute with graph_node<T> which makes extensive use
// of hash_table<K, V> so has to include its header, and hash_table<K, V> needs
// access to all classes it supports to be able to hash them.
// Complicating matters further, graph<T> has a lot of descendant classes that
// also want to use hash_table<K, V>, which requires them to have access to
// both of these classes. Unless properly handled, this chain of dependencies
// can be a hotbed of dormant circular includes waiting to be triggered by a
// specific include order.
//
// One observation is key to understanding how this is resolved in cppdata:
// What appears to be a co-dependency at first glance are actually two
// different use cases that are operationally quite separate, because the call
// site either wants to hash a cppdata class as keys, in which case it needs to
// include the class header, or it has no need for the class and doesn't
// include its header, in which case there is no risk of circular includes.
// This means that hash_table<K, V> can rely entirely on forward declarations
// in its .tpp file to implement all hash_fn<K> template specializations,
// anticipating that when a class needs to be hashed, its header will be
// included, and definitions of all forward-declared classes, methods or
// operators will become available. If class header is not included, the
// specialization for that class cannot be invoked, allowing code to compile.
template <class K, class V>
class hash_table
{
  public:
    // Default constructor uses a prime array size of 1009. O(1) | O(1)
    hash_table()
        : size_(1009),
          hash_(hash_fn<K>(this->size_)),
          table_(new hash_list*[this->size_]()) {}
    // Constructs with specified size for array of linked lists. O(T) | O(T)
    hash_table(size_t table_size)
        : size_(table_size),
          hash_(hash_fn<K>(this->size_)),
          table_(new hash_list*[this->size_]()) {}
    // Constructs with size and hash function for pointer keys. O(T) | O(T)
    hash_table(size_t table_size, PtrHashType ptr_h)
        : size_(table_size),
          hash_(hash_fn<K>(this->size_, ptr_h)),
          table_(new hash_list*[this->size_]()) {}
    // Destructor deallocates the array member. O(N+T) | O(N/T)
    ~hash_table();

    // Returns a copy of key's value. O(1) average, O(N) worst | O(1)
    V operator [] (const K& key) const;
    // Returns true if key is in hash_table. O(1) average, O(N) worst | O(1)
    bool contains(const K& key) const;
    // Returns the size of raw array member, which is tracked. O(1) | O(1)
    size_t size() const {return this->size_;}
    // Deletes all linked lists and nullifies array elements. O(N+T) | O(N/T)
    hash_table<K, V>& clear();

    // Inserts a key-value pair to the table. O(1) average, O(N) worst | O(1)
    hash_table<K, V>& insert(const K& key, const V& val);
    // Inserts a hash_node to the table. O(1) average, O(N) worst | O(1)
    hash_table<K, V>& insert(const hash_node<K, V>& hn);
    // Deletes the first matching entry in given key's hash bucket.
    // O(1) average, O(N) worst | O(1)
    hash_table<K, V>& remove(const K& key);
    // Return pointers to all the keys in the hash table. O(N+T) | O(N)
    dyn_array<K*> keys() const;
    // Return pointers to all the values in the hash table. O(N+T) | O(N)
    dyn_array<V*> vals() const;

    // Prints hash table contents to output stream. O(N+T) | O(1)
    template <class Key, class Val>
    friend std::ostream& operator << (std::ostream& os,
                                      const hash_table<Key, Val>& ht);

  private:
    typedef node<hash_node<K, V>, false> hash_list;
    // Hash function is run through the hash_fn class.
    // O(1) for fundamental types, O(L) for containers | O(1)
    size_t hash(const K& key) const {return this->hash_.run(key);}
    // Method for internal search. O(1) average, O(N) worst | O(1)
    hash_list* find(const K& key) const;

    // Size of the member array.
    size_t size_;
    // Hashing function used to build indices.
    hash_fn<K> hash_;
    // Array of pointers to singly-linked lists of key-value pairs.
    hash_list** table_;
};

// Templated class method implementations need to be accessible at the
// point of instantiation.
#include "hash_table.tpp"

#endif