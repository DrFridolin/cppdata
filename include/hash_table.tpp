// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "hash_table.h"

// Forward declarations needed by full class specializations of hash_fn<K>
template <class T>                 class stack;
template <class T>                 class queue;
template <class T>                 class graph_node;
template <class T>                 class graph;
template <class T, bool P>         class binary_tree;
template <class T, bool P>         class binary_search_tree;
template <class T, bool P, bool M> class heap_tree;
template <class T, bool P>         class avl_tree;
template <class T>                 class red_black_tree;
template <class T>                 class heap;
                                   class trie;
template <class T>
std::ostream& operator << (std::ostream& os, const stack<T>& st);
template <class T>
std::ostream& operator << (std::ostream& os, const queue<T>& que);
template <class T, bool P>
std::ostream& operator << (std::ostream& os, const binary_tree<T, P>& bt);
template <class T>
std::ostream& operator << (std::ostream& os, const heap<T>& h);
std::ostream& operator << (std::ostream& os, const trie& tr);


//======================================================================//
//                                                                      //
//                          hash_fn<K> methods                          //
//                                                                      //
//======================================================================//

template <class K>
size_t hash_fn<K>::mod_unsigned(unsigned key) const
{
  return key % this->size_;
}

template <class K>
size_t hash_fn<K>::mod_float(uint32_t key) const
{
  return key % this->size_;
}

template <class K>
size_t hash_fn<K>::mod_double(uint64_t key) const
{
  return key % this->size_;
}

template <class K>
size_t hash_fn<K>::mod_signed(int key) const
{
  return ((key % this->size_) + this->size_) % this->size_;
}

template <class K>
size_t hash_fn<K>::poly_roll(const std::string& key) const
{
  return hash_fn<K>::poly_roll(key, this->size_);
}

template <class K>
size_t hash_fn<K>::poly_roll(const std::string& key, size_t size)
{
  // Pick prime constants for the base and modulus operations.
  // Small prime base to avoid overflows in intermediate results.
  const unsigned long long p = 31;
  // Large prime for modulus to have a wide range of hash values while
  // keeping the values in a reasonable range.
  const unsigned long long m = static_cast<unsigned long long>(1e9) + 7;
  // Variable large enough (64-bits) to hold the hash value at each step.
  unsigned long long hash_value = 0;
  // Variable large enough (64-bits) to keep track of the polynomial power
  // to apply at each step.
  unsigned long long p_pow = 1;
  // Parse the string to compute the hash value.
  for (size_t i = 0; i < key.size(); ++i)
  {
    unsigned long long char_val = static_cast<unsigned long long>(key[i]);
    // Apply modulus to intermediate variables to avoid overflows.
    hash_value = (hash_value + ((char_val * p_pow) % m)) % m;
    // Update the polynomial power, then apply modulus to avoid overflows.
    p_pow = (p_pow * p) % m;
  }
  // Apply modulus on final result to ensure it falls within table size.
  return static_cast<size_t>(hash_value % size);
}

template <class K>
size_t hash_fn<K>::fnv(const uintptr_t& key) const
{
  // Prime and offset constants to be applied to address bytes.
  const unsigned long long fnv_prime = 1099511628211ULL;
  const unsigned long long fnv_offset = 14695981039346656037ULL;
  unsigned long long hash_value = fnv_offset;
  // Reinterpret the pointer key's address as an unsigned char address.
  // in order to access and process its value one byte at a time.
  const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);
  for (size_t i = 0; i < sizeof(uintptr_t); ++i)
  {
    hash_value ^= data[i];
    hash_value *= fnv_prime;
  }
  // Apply modulus on final result to ensure it falls within table size.
  return static_cast<size_t>(hash_value % this->size_);
}

template <class K>
size_t hash_fn<K>::jenkins(const uintptr_t& key) const
{
  unsigned long long hash_value = 0;
  // Reinterpret the pointer key's address as an unsigned char address
  // in order to access and process its value one byte at a time.
  const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);
  for (size_t i = 0; i < sizeof(uintptr_t); ++i)
  {
    hash_value += data[i];
    hash_value += (hash_value << 10);
    hash_value ^= (hash_value >> 6);
  }
  hash_value += (hash_value << 3);
  hash_value ^= (hash_value >> 11);
  hash_value += (hash_value << 15);
  // Apply modulus on final result to ensure it falls within table size.
  return static_cast<size_t>(hash_value % this->size_);
}

template <class K>
size_t hash_fn<K>::murmur(const uintptr_t& key) const
{
  // Actual MurmurHash initializes hash_value with a seed.
  unsigned long long hash_value = 0;
  // Reinterpret the pointer key's address as an unsigned char address
  // in order to access and process its value one byte at a time.
  const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);
  for (size_t i = 0; i < sizeof(uintptr_t); i += 4)
  {
    unsigned long long k = 0;
    // Build 4-byte word from input bytes.
    for (size_t j = 0; (j < 4) && ((i + j) < sizeof(uintptr_t)); ++j)
      k |= static_cast<unsigned long long>(data[i + j]) << (j * 8);
    // Mix with multiplication and shifts.
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    // Combine with hash using XOR and further mixing.
    hash_value ^= k;
    hash_value = (hash_value << 13) | (hash_value >> 19);
    hash_value = hash_value * 5 + 0xe6546b64;
  }
  // Finalize hash with length and additional mixing.
  hash_value ^= this->size_;
  hash_value ^= hash_value >> 16;
  hash_value *= 0x85ebca6b;
  hash_value ^= hash_value >> 13;
  hash_value *= 0xc2b2ae35;
  hash_value ^= hash_value >> 16;
  // Apply modulus on final result to ensure it falls within table size.
  return static_cast<size_t>(hash_value % this->size_);
}

// Full template-specialized implementations of the run method picl
// specific hash functions that are optimal for each key data type
// provided by the template argument K.

// Signed integral types: int | short | char
template<>
size_t hash_fn<int>::run(const int& key) const
{
  return this->mod_signed(key);
}
template<>
size_t hash_fn<short>::run(const short& key) const
{
  return this->mod_signed(static_cast<int>(key));
}
template<>
size_t hash_fn<char>::run(const char& key) const
{
  return this->mod_signed(static_cast<int>(key));
}
// Unsigned integral types: unsigned | unsigned short | unsigned char
template<>
size_t hash_fn<unsigned>::run(const unsigned& key) const
{
  return this->mod_unsigned(key);
}
template<>
size_t hash_fn<unsigned short>::run(const unsigned short& key) const
{
  return this->mod_unsigned(static_cast<unsigned>(key));
}
template<>
size_t hash_fn<unsigned char>::run(const unsigned char& key) const
{
  return this->mod_unsigned(static_cast<unsigned>(key));
}
// Floating-point types: float | double
template <>
size_t hash_fn<float>::run(const float& key) const
{
  uint32_t bits;
  std::memcpy(&bits, &key, sizeof(key));
  return this->mod_float(bits);
}
template <>
size_t hash_fn<double>::run(const double& key) const
{
  uint64_t bits;
  std::memcpy(&bits, &key, sizeof(key));
  return this->mod_double(bits);
}
// String container types: std::string | trie
template<>
size_t hash_fn<std::string>::run(const std::string& key) const
{
  return this->poly_roll(key);
}
template<>
size_t hash_fn<trie>::run(const trie& key) const
{
  std::stringstream ss;
  ss << key;
  return this->poly_roll(ss.str());
}
// Pointer types: uintptr_t
template<>
size_t hash_fn<uintptr_t>::run(const uintptr_t& key) const
{
  switch (this->ptr_hash_)
  {
    case FNV:     return this->fnv(key);
    case JENKINS: return this->jenkins(key);
    case MURMUR:  return this->murmur(key);
    default:      return this->fnv(key);
  }
}

// Full-class specializations for hash_fn class are needed to use templated
// data structures to be used as keys. All templated data structures in this
// library overload << operator and are capable of streaming their contents.
// These full-class specializations use the poly_roll hash function originally
// defined for std::string by streaming their contents to a stringstream, then
// using the resulting string as their key. Since all of these full class
// specializations will use poly_roll, it is implemented as a static method
// in hash_fn<K> class so that code duplications are minimized.

// Full class specialization for dyn_array<T>.
template <class T>
class hash_fn<dyn_array<T> > {
  public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const dyn_array<T>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
  private:
    size_t size_;
};
// Full class specialization for node<T, D>.
template <class T, bool D>
class hash_fn<node<T, D> > {
  public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const node<T, D>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
  private:
    size_t size_;
};
// Full class specialization for linked_list<T, D>.
template <class T, bool D>
class hash_fn<linked_list<T, D> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const linked_list<T, D>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for stack<T>.
template <class T>
class hash_fn<stack<T> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const stack<T>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for queue<T>.
template <class T>
class hash_fn<queue<T> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const queue<T>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for graph_node<T>.
template <class T>
class hash_fn<graph_node<T> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const graph_node<T>& key) const {
      std::stringstream ss;
      key.print_sorted(ss);
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for graph<T>.
template <class T>
class hash_fn<graph<T> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const graph<T>& key) const {
      std::stringstream ss;
      key.print_sorted_adj_lists(ss);
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for binary_tree<T, P>.
template <class T, bool P>
class hash_fn<binary_tree<T, P> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const binary_tree<T, P>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for binary_search_tree<T, P>.
template <class T, bool P>
class hash_fn<binary_search_tree<T, P> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const binary_search_tree<T, P>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for heap_tree<T, P>.
template <class T, bool P, bool M>
class hash_fn<heap_tree<T, P, M> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const heap_tree<T, P, M>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for avl_tree<T, P>.
template <class T, bool P>
class hash_fn<avl_tree<T, P> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const avl_tree<T, P>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for red_black_tree<T>.
template <class T>
class hash_fn<red_black_tree<T> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const red_black_tree<T>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};
// Full class specialization for heap<T>.
template <class T>
class hash_fn<heap<T> > {
public:
    hash_fn() : size_(1009) {}
    hash_fn(size_t s) : size_(s) {}
    ~hash_fn() {}
    size_t run(const heap<T>& key) const {
      std::stringstream ss;
      ss << key;
      return hash_fn<std::string>::poly_roll(ss.str(), this->size_);
    }
private:
    size_t size_;
};

//======================================================================//
//                                                                      //
//                       hash_table<K, V> methods                       //
//                                                                      //
//======================================================================//

template <class K, class V>
hash_table<K, V>::~hash_table()
{
  for (size_t i = 0; i < this->size_; ++i)
    if (this->table_[i])
      delete this->table_[i];
  delete[] this->table_;
}

template <class K, class V>
V hash_table<K, V>::operator [] (const K& key) const
{
  hash_list* l_ptr = this->find(key);
  if (l_ptr)
    return l_ptr->val().val_;
  std::cerr << "ERROR! Key not found: " << key << std::endl;
  return V();
}

template <class K, class V>
bool hash_table<K, V>::contains(const K& key) const
{
  hash_list* l_ptr = this->find(key);
  return (l_ptr != nullptr);
}

template <class K, class V>
hash_table<K, V>& hash_table<K, V>::insert(const K& key, const V& val)
{
  return this->insert(hash_node<K, V>(key, val));
}

template <class K, class V>
hash_table<K, V>& hash_table<K, V>::insert(const hash_node<K, V>& hn)
{
  // Compute the table index from the node key using hash function.
  size_t i = this->hash(hn.key_);
  // If table element is empty, create a new head node with key-value pair.
  if (!this->table_[i]) {
    this->table_[i] = hash_list::create(hn);
  // Otherwise search for the new key in the linked list and index ind.
  }
  else {
    hash_list* l_ptr = this->find(hn.key_);
    // If key exists, update its value.
    if (l_ptr)
      l_ptr->set_val(hn);
    // If key doesn't exist, append the key-value pair to list.
    else
      this->table_[i] = this->table_[i]->prepend(hn);
  }
  return *this;
}

template <class K, class V>
hash_table<K, V>& hash_table<K, V>::remove(const K& key)
{
  // Compute the table index from the key using hash function.
  size_t i = this->hash(key);
  // If a hash_list exists at this index, delete node with the matching key.
  if (this->table_[i])
    this->table_[i] = this->table_[i]->delete_node(hash_node<K, V>(key, V()));
  else
    std::cout << "Warning: Key not found, skipped removal." << std::endl;
  return *this;
}

template <class K, class V>
dyn_array<K*> hash_table<K, V>::keys() const
{
  dyn_array<K*> keys;
  for (size_t i = 0; i < this->size_; ++i) {
    hash_list* entry = table_[i];
    if (entry == nullptr)
      continue;
    keys.add(&(entry->val().key_));
    while (entry->next()) {
      entry = entry->next();
      keys.add(&(entry->val().key_));
    }
  }
  return keys;
}

template <class K, class V>
dyn_array<V*> hash_table<K, V>::vals() const
{
  dyn_array<V*> vals;
  for (size_t i = 0; i < this->size_; ++i) {
    hash_list* entry = table_[i];
    if (entry == nullptr)
      continue;
    vals.add(&(entry->val().val_));
    while (entry->next()) {
      entry = entry->next();
      vals.add(&(entry->val().val_));
    }
  }
  return vals;
}

template <class K, class V>
hash_table<K, V>& hash_table<K, V>::clear()
{
  for (size_t i = 0; i < this->size_; ++i) {
    if (this->table_[i]) {
      delete this->table_[i];
      this->table_[i] = nullptr;
    }
  }
  return *this;
}

template <class K, class V>
node<hash_node<K, V>, false>* hash_table<K, V>::find(const K& key) const
{
  // Compute the table index from the node key using hash function.
  size_t ind = this->hash(key);
  if (!this->table_[ind])
    return nullptr;
  return this->table_[ind]->search(hash_node<K, V>(key, V()));
}

template <class K, class V>
std::ostream& operator << (std::ostream& os, const hash_table<K, V>& ht)
{
  // Compute the number of decimal places needed for hash table indices.
  size_t buf = ht.size_;
  size_t dec = 1;
  while (buf >= 10) {
    dec++;
    buf = buf / 10;
  }
  for (size_t i = 0; i < ht.size_; ++i) {
    std::cout << std::setw(dec) << i << ". ";
    if (!ht.table_[i]) {
      os << "NULL";
    }
    else if (ht.table_[i] == nullptr) {
      os << "EMPTY";
    }
    else {
      hash_table<K, V>::hash_list* hl = ht.table_[i];
      while (hl) {
        os << "(\"" << hl->val().key_ << "\":" << hl->val().val_ << ")";
        if (hl->next())
          os << " ";
        hl = hl->next();
      }
    }
    if (i < (ht.size_ - 1))
      os << std::endl;
  }
  return os;
}