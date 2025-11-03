// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "trie.h"

//======================================================================//
//                                                                      //
//                             trie methods                             //
//                                                                      //
//======================================================================//

trie* trie::create(const std::string& str)
{
  trie* new_trie = new trie();
  std::stringstream sstr(str);
  std::string word;
  while (sstr >> word)
    new_trie->insert(word);
  return new_trie;
}

trie* trie::create(const dyn_array<std::string>& dyn)
{
  trie* new_trie = new trie();
  for (size_t i = 0; i < dyn.size(); ++i)
    new_trie->insert(dyn[i]);
  return new_trie;
}

trie::~trie()
{
  for (size_t n = 0; n < this->capacity();  ++n) {
    if ((*this)[n])
      delete (*this)[n];
    (*this)[n] = nullptr;
  }
}

trie* trie::clone() const
{
  trie* new_trie = new trie(this->val_, this->capacity());
  new_trie->ind_ = this->ind_;
  for (size_t n = 0; n < this->capacity();  ++n) {
    const trie* child = dynamic_cast<const trie*>((*this)[n]);
    if (child)
      (*new_trie)[n] = child->clone();
  }
  return new_trie;
}

trie& trie::clear()
{
  for (size_t n = 0; n < this->capacity();  ++n) {
    if ((*this)[n])
      delete (*this)[n];
    (*this)[n] = nullptr;
  }
  return *this;
}

bool trie::operator == (const trie& other) const
{
  // Use graph_node operator to locally compare trie node and its children.
  if (!this->graph_node<char>::operator==(other))
    return false;
  // Create a neighbor value set for each trie node.
  dyn_array<const trie*> this_set, other_set;
  for (size_t n = 0; n < this->degree(); ++n) {
    this_set.add(dynamic_cast<const trie*>((*this)(n)));
    other_set.add(dynamic_cast<const trie*>(other(n)));
  }
  // Sort neighbor pointers, then recursively compare corresponding children.
  this_set.merge_sort();
  other_set.merge_sort();
  for (size_t n = 0; n < this_set.size(); ++n)
    if (*this_set[n] != *other_set[n])
      return false;
  return true;
}

bool trie::operator != (const trie& other) const
{
  return !(*this == other);
}

trie& trie::operator = (const trie& source)
{
  if (this == &source)
    return *this;
  this->clear();
  this->set_val(source.val());
  this->ind_ = source.ind_;
  this->resize_adj(source.capacity());
  for (size_t n = 0; n < source.capacity();  ++n) {
    const trie* child = dynamic_cast<const trie*>(source[n]);
    if (child)
      (*this)[n] = child->clone();
  }
  return *this;
}

trie& trie::operator = (const std::string& str)
{
  if (str.empty())
    return *this;
  this->clear();
  std::stringstream sstr(str);
  std::string word;
  while (sstr >> word)
    this->insert(word);
  return *this;
}

trie& trie::operator = (std::initializer_list<std::string> list)
{
  if (list.size() == 0)
    return *this;
  this->clear();
  dyn_array<std::string> buf(list);
  *this = buf;
  return *this;
}

trie& trie::operator = (const dyn_array<std::string>& source)
{
  if (source.is_empty())
    return *this;
  this->clear();
  for (size_t i = 0; i < source.size(); ++i)
    this->insert(source[i]);
  return *this;
}

dyn_array<const trie*> trie::search(const std::string& str) const
{
  dyn_array<const trie*> trie_list = this->find_prefix(str);
  if (trie_list.is_empty())
    return trie_list;
  const trie* last = trie_list[trie_list.size() - 1];
  // Check if a termination node exists if the prefix is found.
  // If termination node is missing, prefix is not a string.
  for (size_t n = 0; n < last->degree(); ++n)
    if ((*last)(n)->val() == '\0')
      return trie_list;
  return trie_list.clear();
}

dyn_array<const trie*> trie::find_prefix(const std::string& str) const
{
  dyn_array<const trie*> trie_list, empty_list;
  if (str.find('\0') != std::string::npos) {
    std::cerr << "Strings with null character \\0 are forbidden" << std::endl;
    return empty_list;
  }
  const trie* cur_ptr = this;
  const trie* next_ptr = nullptr;
  // Traverse the trie and parse the string simultaneously.
  for (size_t i = 0; i < str.size(); ++i) {
    next_ptr = nullptr;
    // Check all the child nodes for the current character.
    for (size_t n = 0; n < cur_ptr->degree(); ++n) {
      if ((*cur_ptr)(n)->val() == str[i]) {
        next_ptr = dynamic_cast<const trie*>((*cur_ptr)(n));
        break;
      }
    }
    // If it isn't found, trie does not contain the string.
    if (next_ptr == nullptr)
      return empty_list;
    trie_list.add(next_ptr);
    cur_ptr = next_ptr;
  }
  return trie_list;
}

dyn_array<std::string> trie::autocomplete(const std::string& prefix) const
{
  dyn_array<std::string> str_arr;
  dyn_array<const trie*> trie_list = this->find_prefix(prefix);
  // If prefix is not found, return an empty dyn_array.
  if (trie_list.is_empty())
    return str_arr;
  // Otherwise collect all strings starting at the end of the prefix.
  trie_list[trie_list.size() - 1]->get_strings(str_arr);
  // Add the prefix to the beginning of each string found.
  for (size_t i = 0; i < str_arr.size(); ++i)
    str_arr[i] = prefix + str_arr[i];
  return str_arr;
}

void trie::get_strings(dyn_array<std::string>& str_arr) const
{
  std::string cur_str;
  // Root is not a part of any string, so call collect on all its children.
  for (size_t n = 0; n < this->degree(); ++n) {
    const trie* child = dynamic_cast<const trie*>((*this)(n));
    child->collect(str_arr, cur_str);
  }
  return;
}

trie& trie::insert(const std::string& str)
{
  if (str.find('\0') != std::string::npos) {
    std::cerr << "Strings with null character \\0 are forbidden" << std::endl;
    return *this;
  }
  trie* cur_ptr = this;
  trie* next_ptr = nullptr;
  // Traverse the trie and parse the string simultaneously.
  for (size_t i = 0; i < str.size(); ++i) {
    next_ptr = nullptr;
    // Check all the child nodes for the current character.
    for (size_t n = 0; n < cur_ptr->degree(); ++n) {
      if ((*cur_ptr)(n)->val() == str[i]) {
        next_ptr = dynamic_cast<trie*>((*cur_ptr)(n));
        break;
      }
    }
    // If it isn't found, create a new child with the current character.
    if (next_ptr == nullptr) {
      next_ptr = new trie(str[i]);
      cur_ptr->adj_.add(next_ptr);
    }
    cur_ptr = next_ptr;
  }
  // When string is parsed, add the termination node if it's not already there.
  for (size_t n = 0; n < cur_ptr->degree(); ++n)
    if ((*cur_ptr)(n)->val() == '\0')
      return *this;
  cur_ptr->adj_.add(new trie('\0'));
  return *this;
}

trie& trie::remove(const std::string& str)
{
  // Search for the string in the trie. Nothing to do if it's not found.
  dyn_array<trie*> trie_list = this->search_to_edit(str);
  if (trie_list.is_empty())
    return *this;
  // If found, parse the string backwards and delete nodes with no children.
  int ind = static_cast<int>(trie_list.size()) - 1;
  trie* parent = trie_list[ind];
  trie* child;
  // Start by deleting the termination node.
  for (size_t n = 0; n < parent->degree(); ++n) {
    if ((*parent)(n)->val() == '\0') {
      trie* del_node = dynamic_cast<trie*>((*parent)(n));
      (*parent)(n) = nullptr;
      delete del_node;
      break;
    }
  }
  ind--;
  // Iterate backwards on path, nullify incoming link from parent, then delete.
  for (; ind >= -1; --ind)
  {
    gn_ptrs<char> trav;
    parent = (ind < 0) ? this : trie_list[ind];
    child = trie_list[ind+1];
    // If the child has children, no other node on the path can be deleted.
    if (child->degree() > 0)
      break;
    // If child's degree is 0, first set the parent's pointer to it to nullptr.
    if (child->degree() == 0) {
      for (size_t n = 0; n < parent->degree(); ++n) {
        if ((*parent)(n) == child) {
          (*parent)(n) = nullptr;
          break;
        }
      }
      // Then safely delete the child.
      delete child;
    }
  }
  return *this;
}

trie::trie(const trie& source)
{
  this->set_val(source.val());
  this->ind_ = source.ind_;
  this->resize_adj(source.capacity());
  for (size_t n = 0; n < source.capacity();  ++n) {
    const trie* child = dynamic_cast<const trie*>(source[n]);
    if (child)
      (*this)[n] = child->clone();
  }
}

bool trie::collect(dyn_array<std::string>& s_arr, std::string& cur) const
{
  // If this is a termination node, cur_str is a valid string.
  if (this->val() == '\0') {
    s_arr.add(cur);
    // Termination nodes should have no children.
    return false;
  }
  // Otherwise, all the character stored in this node to cur_str.
  cur += this->val();
  // Then recurse into all the children of this node.
  for (size_t n = 0; n < this->degree(); ++n) {
    const trie* child = dynamic_cast<const trie*>((*this)(n));
    // Remove child node's character before moving onto the next child.
    child->collect(s_arr, cur);
  }
  cur.pop_back();
  return true;
}

dyn_array<trie*> trie::search_to_edit(const std::string& str)
{
  dyn_array<trie*> trie_list = this->find_prefix_to_edit(str);
  if (trie_list.is_empty())
    return trie_list;
  trie* last = trie_list[trie_list.size() - 1];
  // Check if a termination node exists if the prefix is found.
  // If termination node is missing, prefix is not a string.
  for (size_t n = 0; n < last->degree(); ++n)
    if ((*last)(n)->val() == '\0')
      return trie_list;
  return trie_list.clear();
}

dyn_array<trie*> trie::find_prefix_to_edit(const std::string& str)
{
  dyn_array<trie*> trie_list, empty_list;
  if (str.find('\0') != std::string::npos) {
    std::cerr << "Strings with null character \\0 are forbidden" << std::endl;
    return empty_list;
  }
  trie* cur_ptr = this;
  trie* next_ptr = nullptr;
  // Traverse the trie and parse the string simultaneously.
  for (size_t i = 0; i < str.size(); ++i) {
    next_ptr = nullptr;
    // Check all the child nodes for the current character.
    for (size_t n = 0; n < cur_ptr->degree(); ++n) {
      if ((*cur_ptr)(n)->val() == str[i]) {
        next_ptr = dynamic_cast<trie*>((*cur_ptr)(n));
        break;
      }
    }
    // If it isn't found, trie does not contain the string.
    if (next_ptr == nullptr)
      return empty_list;
    trie_list.add(next_ptr);
    cur_ptr = next_ptr;
  }
  return trie_list;
}

std::ostream& operator << (std::ostream& os, const trie* tr)
{
  dyn_array<std::string> str_arr;
  tr->get_strings(str_arr);
  os << str_arr << std::endl;
  return os;
}

std::ostream& operator << (std::ostream& os, const trie& tr)
{
  dyn_array<std::string> str_arr;
  tr.get_strings(str_arr);
  // Since tries tend to be large, sort using O(1) space.
  os << str_arr.heap_sort();
  return os;
}