#ifndef binary_tree_h_
#define binary_tree_h_

#include <iostream>
#include <cassert>
#include <initializer_list>
#include <functional>
#include <type_traits>
#include "dyn_array.h"
#include "graph.h"
#include "heap.h"

// template <class T> class graph_node;
template <class T, bool P> class binary_tree;
template <class T, bool P> class binary_search_tree;
template <class T, bool P, bool M> class heap_tree;
template <class T, bool P> class avl_tree;
template <class T> class red_black_tree;
template <class K, class V> class hash_node;

template <class T, bool P>
using bt_ptrs = dyn_array<binary_tree<T, P>*>;
template <class T, bool P>
using cbt_ptrs = dyn_array<const binary_tree<T, P>*>;
template <class T, bool P>
using rb_ok = typename std::enable_if<P, red_black_tree<T>*>::type;
template <class T, bool P>
using TreeType = typename binary_tree<T, P>::BinaryTreeType;

// Binary tree subtypes that can be built with child links only (P=false).
enum class ChildrenOnlyType {STANDARD = 0, SEARCH = 1, MIN_HEAP = 2,
                             MAX_HEAP = 3, AVL    = 4};
// Binary tree subtypes that can be built with parent links (P=true).
enum class ParentLinkedType {STANDARD = 0, SEARCH = 1, MIN_HEAP  = 2,
                             MAX_HEAP = 3, AVL    = 4, RED_BLACK = 5};
// Unscoped global enum to specify traversal types internally.
enum TravType {IN_ORDER, PRE_ORDER, POST_ORDER, LEVEL_ORDER};

// The extensive inheritance hierarchy rooted at graph_node<T> takes on its
// most baroque form with binary_tree<T, P> polymorphism, which aims not only
// to extract archetypal abstractions, but also to capture the intricate and
// subtle forms of interplay between different types of binary trees.
// The fundamental design principle is for classes higher up the chain to
// expose modular implementations of their core machinery for granular reuse,
// and know very little about the particulars of the inheritance hierarchy
// beneath them. The descendants then interject and tweak this machinery with
// carefully insulated virtual methods to adjust it to their own needs.
// This precept establishes a clear division of labor pattern between
// subclasses where each one can be delegated a core functionality that
// heavily informs their design.
//
// As the top-level class in this hierarchy, the responsibilities of
// binary_tree<T,P> are foundational and wide-ranging. Its first job is to
// elucidate the semantic transition from graphs to binary trees by
// establishing a set of conventions and restrictions on graph_node<T>
// inheritance that all of its descendants will be mandated to follow.
// Specifically, binary_tree<T, P> and all its descendants are required to:
// - Use adjacency list member adj_ to store pointers to their linked nodes.
// - Store at least a pair of such pointers, one for each child node.
// - Store pointer to the left child at adj_[0] and implement const/non-const
//   left() accessors for it. These can be public or protected.
// - Store pointer to the right child at adj_[1] and implement const/non-const
//   right() accessors for it. These can be public or protected.
// - Use the boolean template argument P to control whether the nodes will also
//   store a pointer to their parent nodes (P=true) or not (P=false).
// - Store pointer to the parent at adj_[2] if parent-linked and implement
//   const/non-const prnt() accessors for it. These can be public or protected.
// - Call graph_node<T> constructor with consistent adjacency list size
//   arguments in their own constructors, which should at least be P+2.
// - Never resize adj_ post-construction and ensure that its size remains
//   unchanged throughout their lifetimes.
// - Use nullptr to represent link slots missing a linked node.
// - Use ind_ member to store node heights.
//
// Storing of tree links at adjacency lists inherited from graph_node<T> in a
// manner that is congruent with their use in graph contexts provides all types
// of binary trees the ability to run all graph_node<T> methods on their tree
// structure directly without any restrictions. Furthermore, it establishes a
// consistent representation for tree links that is stable under inter-class
// conversions and polymorphic overrides, allowing binary_tree<T, P> to assume
// full ownership of this shared base structure without having to offer
// customized accomodations for its descendants.
// Specialized subclasses are still able to place stronger constraints on the
// structure they inherit, or store additional links in the remaining indices
// of adj_ without breaking any inherited functionality.
//
// Once put in charge of the general tree structure, it makes sense for
// binary_tree<T, P> to also be delegated its creation, replication and
// destruction. This is done by giving it a pair of static factory create
// methods that centralize the creation of all binary trees using an enum
// option set to pick among supported dynamic types.
// All created nodes are returned as binary_tree<T, P> pointers, enforcing
// polymorphic uniformity on the creation process.
// To permit binary_tree<T, P> subtypes that are stable and functional only
// for child-only (P=false) or parent-linked (P=true) connectivity, the enums
// for the two sets are kept separate as ChildrenOnlyType and ParentLinkedType
// enums, with types that support both appearing with identical enumerator
// names and values in both enum scopes.
// Using "Substitution Failure Is Not An Error" (SFINAE), binary_tree<T, P>
// sets an internal alias BinaryTreeType to one of these two enum sets based
// on its template argument P, and enables only one create method, specifically
// the one with the switch list of types supporting binary tree's P argument.
// Both factory create methods expect BinaryTreeType as their enum argument
// (e.g., binary_tree<T, P>::create(val, BinaryTreeType::AVL)), therefore
// become seamlessly constrained to the dynamic types listed in the enum that
// the alias is referring to. This serves as a strong compile-time guard
// against attempts to create trees with unsupported P arguments, without
// complicating the enum-based type specification at call site.
// Two separate create methods and two separate RTTI type() accessors are
// needed to maintain two separate switch lists of supported types, with only
// one being SFINAE-enabled on any given class instance based on its P.
//
// Despite the uniformity, extensibility and loose insulation provided by this
// single factory method, having to cast each created node to its dynamic type
// before being able run its class methods can easily become too cumbersome.
// For this reason, binary_tree<T, P> implements for each descendant an extra
// static create method, with an identical name as the descendant's enumerator
// (e.g. binary_tree<T, P>::AVL(val)), that calls the base create method,
// then casts the pointer to its dynamic type before returning.
// These additional static methods are disabled via SFINAE for P arguments they
// don't support, providing another compiler-time check for type validity.
// This is also one of the rare instances in this inheritance hierarchy where a
// class provides descendant-specific implementation for some functionality.
// All static factory methods are public, with the base method being the only
// option for runtime type specification, while the named methods providing
// more convenient syntax when type is known at compile time.
//
// Cloning is similarly centralized in a protected method that passes a
// BinaryTreeType argument to its factory create calls, allowing the same
// cloning recursion to be deployed regardless of the dynamic types being
// cloned. Members of cloned nodes are set using the virtual set_members
// method, which descendants override to add their own class-specific members
// to the cloning process. Public clone() in binary_tree<T, P> simply passes
// this->type() as the enum argument to its protected method, and all other
// classes in the hierarchy clone themselves by calling this base clone method
// directly and their dynamic types are seamlessly passed on via the RTTI
// mechanism in binary_tree<T, P>'s type() method, resulting in a clone with
// identical type and class-specific members values.
//
// Having an inherent acyclic directionality in their link structure and as
// descendants of a self-referential node-only class, all types of binary trees
// are inherently capable of deploying the same fully-recursive memory
// ownership model as node<T, D>.
// To facilitate this, all classes in the hierarchy declare their constructors
// protected and grant friend access to binary_tree<T, P> to restrict their
// node creation to heap memory and to the centralized factory methods.
//
// Since standard traversals on a binary tree require nothing more than access
// to tree's link structure, binary_tree<T, P> provides a lambda-customizable
// public traversal engine that optimally executes one of the three recursive
// traversals enumerated by TravType. It allows injection of customized
// operations and termination conditions through a standardized lambda
// signature that takes a binary_tree<T, P> pointer and two intptr_t integer
// arguments with context-dependent meanings, see method comments for details.
// These dual-use lambdas provide a convenient unified interface for all
// descendants and their methods to run conditional or modified traversals by
// dispatching their own custom lambdas.
//
// Level-order traversals, on the other hand, are iterative in nature and
// require a queue<T> to track nodes; therefore they are powered by a separate
// engine called stratify. Following a similar idea and using the exact same
// lambda signature for customization, stratify deploys a queue<T> to traverse
// and separate the tree levels, running the lambda on each dequeued node.
//
// All classes in the binary_tree<T, P> inheritance hierarchy can be assigned
// to and from each other, regardless of their positions in the chain.
// The assignment operators will often leverage binary_tree<T, P> as a complete
// structural representation shared by all of its descendants to efficiently
// duplicate source tree's structure and provide target side compliance with
// minimal amount of deviation.
// Since most of its descendants, and indeed binary_tree<T, P> itself inherit
// all their members and introduce no additional ones, most such assignments
// amount to little more than this structure duplication.
//
// For its core virtual methods like search, insert and remove, the goal of
// this class is to provide maximally-useful default behaviors that are well-
// defined on all binary trees. For example, even though there is no inherent
// insertion order defined for a general binary_tree<T, P>, its insert method
//`performs level-order left-to-right insertion and guarantees to preserve or
// approach both balance and completeness with each insertion. This method ends
// up forming the backbone of heap_tree<T, P, M> insertions.
//
// Assumes T is default-constructable with equality and relational operators.
// Observes the rule of 3. Cyclic binary trees are currently not supported.
template <class T, bool P>
class binary_tree : public graph_node<T>
{
  // Allows hash_node to access protected copy constructor for hashing.
  template <class K, class V> friend class hash_node;

  public:
    // Member alias is a context-dependent enum type based on P.
    using BinaryTreeType = typename std::conditional<
        P, ParentLinkedType, ChildrenOnlyType>::type;

    // Polymorphic SFINAE factory method for creating on heap memory all the
    // binary_tree subtypes in the inheritance hierarchy, while returning a
    // binary_tree pointer for polymorphic uniformity, seamless extensibility,
    // and loose insulation of internals from invokers. O(1) | O(1)
    template <bool B = P>
    static typename std::enable_if<!B, binary_tree<T, P>*>::type
    create(const T& v, BinaryTreeType type = BinaryTreeType::STANDARD);

    template <bool B = P>
    static typename std::enable_if<B, binary_tree<T, P>*>::type
    create(const T& v, BinaryTreeType type = BinaryTreeType::STANDARD);

    // Static factory helpers cast to and return requested type's pointer.
    // They are SFINAE-disabled when requested type is not supported for P.
    // They form the primary interface for binary tree creation. O(1) | O(1)
    static binary_tree<T, P>* STANDARD(const T& v);
    static binary_search_tree<T, P>* SEARCH(const T& v);
    static heap_tree<T, P, false>* MIN_HEAP(const T& v);
    static heap_tree<T, P, true>* MAX_HEAP(const T& v);
    static avl_tree<T, P>* AVL(const T& v);
    template <bool B = P> static typename rb_ok<T, B> RED_BLACK(const T& v);

    // Destructor recursively deletes all descendant nodes. O(N) | O(H)
    virtual ~binary_tree();
    // Clones the subtree rooted at this node. Duplicates tree structure and
    // members of any class using RTTI. O(N) | O(H)
    virtual binary_tree<T, P>* clone() const;

    // Accessors and mutators for node's children and parent. O(1) | O(1)
    binary_tree<T, P>*& left();
    binary_tree<T, P>*& right();
    binary_tree<T, P>*& prnt();
    const binary_tree<T, P>* left() const;
    const binary_tree<T, P>* right() const;
    const binary_tree<T, P>* prnt() const;

    // SFINAE method returns contextual BinaryTreeType based on P. Determines
    // the type of binary tree using RTTI dynamic_cast mechanism. O(1) | O(1)
    template <bool B = P>
    typename std::enable_if<!B, BinaryTreeType>::type type() const;
    template <bool B = P>
    typename std::enable_if<B, BinaryTreeType>::type type() const;
    // Returns last computed max distance to a leaf node from this node.
    // Note that heights are not updated when tree is modified. O(1) | O(1)
    const size_t& height() const {return this->ind_;}
    size_t& height() {return this->ind_;}
    // Computes and stores node heights with bottom-up dynamic programming
    // within a post-order traversal. Returns height of this node. O(N) | O(H)
    size_t compute_heights();

    // Links parent nodes if P=true. Useful during manual edits. O(N) | O(H)
    template <bool B = P>
    typename std::enable_if<B, binary_tree<T, P>*>::type link_parents();
    // Returns true if node is childless. Does not seek real root. O(1) | O(1)
    bool root_only() const {return (!this->left()) && (!this->right());}
    // Returns tree's total node count. Does not count backwards. O(N) | O(N)
    virtual size_t size() const {return this->traverse(PRE_ORDER).size();}
    // Deletes all descendant nodes and resets all class members. O(N) | O(H)
    binary_tree<T, P>* clear();

    // Checks this tree's structure and node values against the other.
    // O(1) average, O(N) worst | O(H)
    bool operator == (const binary_tree<T, P>& other) const;
    bool operator != (const binary_tree<T, P>& other) const;
    // Clones source tree starting from the given node. O(N+M) | O(N)
    binary_tree<T, P>& operator = (const binary_tree<T, P>& src);
    // Turns init list to dyn_array, then rebuilds tree from it. O(N+M) | O(N)
    binary_tree<T, P>& operator = (std::initializer_list<T> list);
    // Level-order rebuilds complete binary tree from dyn_array. O(N+M) | O(N)
    binary_tree<T, P>& operator = (const dyn_array<T>& src);

    // Find and return minimum/maximum value stored in the tree. O(N) | O(N)
    virtual T min() const {return this->traverse(PRE_ORDER).min()->val();}
    virtual T max() const {return this->traverse(PRE_ORDER).max()->val();}
    // Returns the deepest rightmost leaf and its parent. O(N) | O(W)
    virtual binary_tree<T, P>* last(binary_tree<T, P>*& parent);
    // Returns the parent of the next level-order left-to-right insertion slot.
    // Does not check or validate any tree properties. O(N) | O(W)
    binary_tree<T, P>* next();

    // Checks if all nodes have 0 or 2 children. O(N) | O(H)
    bool is_full() const;
    // Checks if branch heights differ by at most 1 for all nodes. O(N) | O(H)
    bool is_balanced() const;
    // Checks if tree is filled in left-to-right order. O(N) | O(W)
    bool is_complete() const;
    // Checks if all tree levels are completely filled. O(N) | O(H)
    bool is_perfect() const;
    // Checks if all nodes are in-order with its children. O(N) | O(H)
    bool is_search() const;
    // Checks if all nodes are smaller/larger than children. O(N) | O(H)
    bool is_heap(bool max = false) const;

    // Traversal engine wrapper for a simpler interface, returns full path.
    // Optional osp argument is for printing path, root and leaves. O(N) | O(N)
    bt_ptrs<T, P>  traverse(TravType type, std::ostream* osp = nullptr);
    cbt_ptrs<T, P> traverse(TravType type, std::ostream* osp = nullptr) const;

    // Executes one of the 3 recursive traversals specified by TravType.
    // Allows injection of customized operations and termination conditions
    // through a standardized lambda signature that takes a binary_tree pointer
    // and two intptr_t integer arguments with context-dependent meanings:
    //   PRE_ORDER: traversal depth and path array address
    //   IN_ORDER: left branch return and path array address.
    //   POST_ORDER: left branch return and right branch return.
    // Engine will run the lambda (check) on current node before, in-between or
    // after recursions depending on the traversal type.
    // Return value -1 is reserved for error signaling; if lambda returns -1
    // the engine will perform another check on-the-spot by running the same
    // lambda on a nullptr input, if this also returns -1 traversal terminates.
    // The error signal will be propagated to the return of the traversal
    // iteration even if no early termination condition is hit.
    // O(N) | O(N) if accumulating path, O(H) if not.
    template <typename Check, typename Path>
    int traversal_engine(Check&& check, Path& path, TravType type,
                         bool accumulate = true, intptr_t depth = 0) const;

    // Runs level-order traversal with the same lambda signature as traversal
    // engine but passes current level and parent queue pointer as arguments.
    // Builds a level-separated path if accumulate is on, returns last parent.
    // Runs lambda on each dequeued node, then if -1 is returned runs it again
    // with nullptr to check if an early termination is requested.
    // O(N) | O(N) if accumulating path, O(W) if not.
    template <typename Check, typename BT_Ptr>
    BT_Ptr stratify(Check&& check, dyn_array<dyn_array<BT_Ptr> >& path,
                    bool accumulate = true) const;

    // Searches for node with matching value to return its pointer. O(N) | O(H)
    virtual binary_tree<T, P>* search(const T& val);
    // Searches all non-root nodes for query to return its parent. O(N) | O(H)
    virtual binary_tree<T, P>* find_parent(const T& val);
    // Searches all non-root nodes for target to return its parent. O(N) | O(H)
    virtual binary_tree<T, P>* find_parent(const binary_tree<T, P>* child);
    // Recursively finds all M nodes with values matching query. O(N) | O(H+M)
    cbt_ptrs<T, P> find(const T& val) const;
    // Finds and returns root. Returns this node for P=false. O(H) | O(1)
    binary_tree<T, P>* find_root();
    const binary_tree<T, P>* find_root() const;
    // Finds and returns dyn_array of pointers to all L leaves. O(N) | O(H+L)
    bt_ptrs<T, P> find_leaves();
    cbt_ptrs<T, P> find_leaves() const;
    // Generates all paths from this node to its leaves. O(N*logN) | O(N*logN)
    dyn_array<bt_ptrs<T, P> > generate_leaf_paths();
    dyn_array<cbt_ptrs<T, P> > generate_leaf_paths() const;

    // Level-order left-to-right insertion returns a pointer to inserted node.
    // Preserves or approaches completeness and balance. O(N) | O(W)
    virtual binary_tree<T, P>* insert(const T& val);
    // Removes first encountered node with matching value/address.
    // Returns pointer to new root. If target is the calling node and it has
    // no children, deletes it and returns nullptr. O(N) | O(H)
    virtual binary_tree<T, P>* remove(const T& val);
    virtual binary_tree<T, P>* remove(const binary_tree<T, P>* target);
    // Level-order right-to-left deletion returns deleted node value.
    // Preserves or approaches completeness and balance. O(N) | O(W)
    virtual T remove();

    // Checks if this tree is a subtree of bt_ptr or vice versa.
    // O(N+(M*S)) average, O(N*S) worst | O(H+M)
    bool is_subtree(const binary_tree<T, P>* bt_ptr) const;
    bool has_subtree(const binary_tree<T, P>* bt_ptr) const;

    // Prints tree structure. O(N) | O(H)
    template <class U, bool P>
    friend std::ostream& operator << (std::ostream& os,
                                      const binary_tree<U, P>* bt);
    // Compactly prints tree structure by replacing endlines with '~' and
    // getting rid of unnecessary spaces. O(N) | O(H)
    template <class U, bool P>
    friend std::ostream& operator << (std::ostream& os,
                                      const binary_tree<U, P>& bt);

  protected:
    // Only constructor is reserved for factory create method.
    // Allocates P+2 links using graph_node constructor and sets node value.
    binary_tree(const T& v): graph_node<T>(v, P+2, 0) {}
    // Copies value and height, then clones left and right branches of source.
    binary_tree(const binary_tree<T, P>& src);
    // Clone engine that can clone into any type in the inheritance hierarchy.
    binary_tree<T, P>* clone (BinaryTreeType type) const;
    // Deletes a child of this node, prioritizing its left child to move up.
    // Could be used on a root node with itself as target if it has a child.
    // If deletion target was root with at least a child, returns new root.
    // Otherwise returns pointer to deleted node's parent. O(H) | O(1)
    virtual binary_tree<T, P>* remove_child(binary_tree<T, P>* target);
    // Protected RTTI method copies all members from source.
    binary_tree<T, P>* set_members(const binary_tree<T, P>* src);
    // Polymorphic method to print nodes in different formats.
    virtual std::ostream& print_node(std::ostream& os) const;
};

// A type of binary_tree<T, P> specialized for efficient search.
// It plays the same role for all search trees that binary_tree<T, P> does for
// all binary trees by providing the following set of foundational capabilities
// to its descendants:
// - A new count_ member for storing duplicate value counts at each node, and a
//   robust mechanism to maintain a base tree structure of unique values.
// - Inter-class assignment operators that account for structural discrepancies
//   between binary tree types with and without the new count_ member.
// - An on-demand balance() method that levereges the fact that duplicates are
//   factored out of the tree structure, and guarantees to produce trees that
//   are both balanced and complete on all of its descendants.
// - Optimal and lambda-customizable search and removal engines that are
//   directly applicable to all search trees.
// - Iterative node insertion method that is shared among most search trees.
// - Node deletion logic based on child count, widely shared among search trees.
//
// To make new count_ member fully customizable via polymorphic interjections
// and overrides by descendants, binary_search_tree<T, P> encapsulates all
// const and non-const access to this member in simple virtual methods.
//
// The balance method traverses the tree in-order to get sorted values, then
// recursively rebuilds a balanced and complete tree from the sorted array of
// values. It provides completeness guarantee by adjusting the mid-value
// selection heuristic and skew the recursion in favor of the left branch just
// enough to ensure nodes at the deepest level are accumulated contiguously on
// the left side. Same recursion is deployed to build search trees from any
// dyn_array<T> simply by sorting it prior to recursive buildup; or indeed from
// any binary_tree<T, P> since they can be a serialized into node value arrays
// with a full traversal.
//
// Despite delivering such a strict balance method with tight guarantees on all
// search trees, binary_search_tree<T, P> is a general, high-level class and is
// not self-balancing. Therefore its instances aren't guaranteed to be balanced
// at all times; in fact, sequential insertions of sorted values is guaranteed
// to produce skewed trees without any balancing.
// Since heights are not an integral part of their core machinery, they also do
// not compute, store or maintain node heights unless binary_tree<T, P>'s
// compute_heights() method is explicitly called to request them.
//
// Insertions and removals swap node values rather than node objects, so node
// addresses are not guaranteed to preserve their values during state-changing
// operations.
// To protect the integrity of its internal state, binary_search_tree<T, P>
// only exposes the alternative, const-only accessor set publicly.
// Manual modifications to its structure are highly discouraged but possible
// with dynamic casts to binary_tree<T, P> pointers to expose the mutators.
//
// Owns all the memory of its descendant nodes. Observes the rule of 3.
// Restricted to heap-only instantiation through factory method in binary_tree.
// Assumes T is default-constructable with equality and relational operators
// as well as a signed distance metric defined via the - operator. Use to avoid
// self-balancing overhead when balance scheduling is handled externally.
template <class T, bool P>
class binary_search_tree : public binary_tree<T, P>
{
  // Allows hash_node access to the protected copy constructor for hashing.
  template <class K, class V> friend class hash_node;
  // Allows binary_tree access to the protected constructors for creation.
  friend class binary_tree<T, P>;

  public:
    // Destructor executes no additional steps. O(N) | O(H)
    virtual ~binary_search_tree() {}
    // Casts the return of binary_tree's clone method. O(N) | O(H)
    virtual binary_search_tree<T, P>* clone() const;
    // Accessors to children and parent return class pointers. O(1) | O(1)
    const binary_search_tree<T, P>* left_ch() const;
    const binary_search_tree<T, P>* right_ch() const;
    const binary_search_tree<T, P>* parent() const;

    // Polymorphic accessor to the duplicate count of the node. O(1) | O(1)
    virtual int count() const {return this->count_;}
    // Override returns the total number of values in the tree. O(N) | O(N)
    virtual size_t size() const;
    // Returns the total number of nodes in the tree. O(N) | O(N)
    size_t num_nodes() const {return this->binary_tree<T, P>::size();}

    using binary_tree::operator==;
    using binary_tree::operator!=;
    // Override adds duplicate counts to the comparison.
    // O(1) average, O(N) worst | O(H)
    bool operator == (const binary_search_tree<T, P>& other) const;
    bool operator != (const binary_search_tree<T, P>& other) const;
    // Clears tree, turns init list into dyn_array, sorts and rebuilds from it.
    // O(N*logN) average, O(N^2) worst | O(N)
    binary_search_tree<T, P>& operator = (std::initializer_list<T> list);
    // Clears tree, sorts dyn_array and rebuilds from it.
    // O(N*logN) average, O(N^2) worst | O(N)
    binary_search_tree<T, P>& operator = (const dyn_array<T>& src);
    // Clears tree, traverses and sorts unique values, rebuilds from dyn_array.
    // O(N*logN) average, O(N^2) worst | O(N)
    binary_search_tree<T, P>& operator = (const binary_tree<T, P>& src);
    // Runs binary_tree operator to clone all members with RTTI. O(N+M) | O(N)
    binary_search_tree<T, P>& operator = (const binary_search_tree<T, P>& src);

    // Overrides find min/max faster by leveraging search tree properties.
    // O(H) average, O(N) worst | O(1)
    virtual T min() const;
    virtual T max() const;
    // Returns sorted node values, including duplicates. O(N)| O(N)
    dyn_array<T> all() const;
    // Returns first node with matching value, or nullptr if none found.
    // Override for search tree traversal. O(H) average, O(N) worst | O(1)
    virtual binary_search_tree<T, P>* search(const T& val);
    // Search for a node with specified value, return a pointer to its parent.
    // Useful when the tree structure will be modified using the searched node.
    // Override for search tree traversal. O(H) average, O(N) worst | O(1)
    virtual binary_search_tree<T, P>* find_parent(const T& val);
    // Finds and returns the query node's parent.
    // Override for search tree traversal. O(H) average, O(N) worst | O(1)
    virtual binary_search_tree<T, P>* find_parent(
        const binary_search_tree<T, P>* child);
    // Finds and returns node's in-order successor, searching only downwards.
    // O(H) average, O(N) worst | O(1)
    binary_search_tree<T, P>* find_succ();
    // Finds and returns the node's in-order successor and its parent.
    // O(H) average, O(N) worst | O(1)
    binary_search_tree<T, P>* find_succ(binary_search_tree<T, P>*& parent);
    // Finds and returns the node value that is closest to the query value.
    // O(H) average, O(N) worst | O(1)
    T find_closest(const T& val) const;

    // Standard iterative search tree insertion.
    // Returns pointer to the inserted node. O(H) average, O(N) worst | O(1)
    virtual binary_search_tree<T, P>* insert(const T& val);
    // Standard iterative search tree removal by value and by pointer.
    // Returns new root. If calling node is the target and it has no children,
    // deletes it and returns nullptr. O(H) average, O(N) worst | O(1)
    virtual binary_search_tree<T, P>* remove(const T& val);
    virtual binary_search_tree<T, P>* remove(
        const binary_search_tree<T, P>* target);
    // Removes/decrements count of deepest rightmost node, returns its value.
    // Guaranteed to comply with binary search tree properties. O(N) | O(W)
    virtual T remove();
    // Balances using in-order traversal. Biased towards filling left branch.
    // Guaranteed to produce balanced and complete search trees.
    // Result is also guaranteed to be AVL-compliant. O(N) | O(N)
    virtual binary_search_tree<T, P>* balance();

  protected:
    binary_search_tree(const T& v): binary_tree<T, P>(v), count_(1) {}
    binary_search_tree(const binary_search_tree<T, P>& src):
        binary_tree<T, P>(src), count_(src.count()) {}
    // Hiding methods protected for internal use, to discourage manual editing.
    binary_search_tree<T, P>*& left();
    binary_search_tree<T, P>*& right();
    binary_search_tree<T, P>*& prnt();
    const binary_search_tree<T, P>* left() const;
    const binary_search_tree<T, P>* right() const;
    const binary_search_tree<T, P>* prnt() const;
    // Core search method for generalized handling of queries, returns parent.
    // O(H) average, O(N) worst | O(1)
    binary_search_tree<T, P>* search_engine(
        std::function<int(const binary_search_tree<T, P>*)> check);
    // Core removal method for generalized handling of queries, returns root.
    // O(H) average, O(N) worst | O(1)
    binary_search_tree<T, P>* removal_engine(
        std::function<int(const binary_search_tree<T, P>*)> check);
    // Builds from a dyn_array by recursively dividing the array in a way to
    // give sufficient nodes to left subtree for balance and completeness.
    // Always uses its own insert, even when called from a descendant class.
    // Expects dyn_arrays of unique vals and their counts. O(N) | O(logN)
    virtual binary_search_tree<T, P>* from_dyn_arr(const dyn_array<T>& uniques,
                                                   const dyn_array<size_t>& cn,
                                                   size_t start,
                                                   size_t end,
                                                   bool first = true);
    // Deletes a child of this node, handling 0, 1 and 2 child cases.
    // Could be used on a root node with itself as target if it has a child.
    // Returns deleted node's parent or new root if target is root with child.
    // O(H) average, O(N) worst | O(1)
    virtual binary_search_tree<T, P>* remove_child(
        binary_search_tree<T, P>* target);
    // Polymorphic setter for the duplicate count of the node.
    virtual binary_search_tree<T, P>* set_count(int count);
    // Polymorphic methods for incrementing and decrementing counts.
    virtual binary_search_tree<T, P>* inc_count();
    virtual binary_search_tree<T, P>* dec_count();
    // Polymorphic override prints counts larger than 1 along with node values.
    virtual std::ostream& print_node(std::ostream& os) const;

    // Node's duplicate value count. Can have additional uses in descendants.
    int count_;
};

// A complete binary_tree<T, P> specialized as tree-based heap implementation.
// Uses a templated type separation (M=false is min heap, M=true is max heap).
// Can be assigned to and from its more efficient array-based sibling class
// heap<T> in a way that preserves the exact structure of the source.
// When assigned another type of complete binary tree, duplicates its structure
// using binary_tree<T, P> assignment operator, then heapifies the structure by
// sifting down each non-leaf node.
//
// Insertions and removals swap node values rather than node objects, so node
// addresses aren't guaranteed to preserve their values during state-changes.
// To protect the integrity of its internal state, only exposes const accessors
// publicly. Manual modifications are highly discouraged but possible with
// dynamic casts to binary_tree<T, P> pointers to expose the mutators.
//
// Owns all the memory of its descendant nodes. Observes the rule of 3.
// Restricted to heap-only instantiation through factory method in binary_tree.
// Assumes T is default-constructable with equality and relational operators,
// as well as a signed distance metric defined via the - operator.
//
// Use primarily to:
// - Handle highly-variable heap sizes.
// - Inspect or visualize heap<T> array as a binary_tree<T, P> structure.
// - Implement custom heaps that are inherently tree-structured or recursive.
// - Leverage tree and graph algorithms on heap structures.
template <class T, bool P, bool M>
class heap_tree : public binary_tree<T, P>
{
  // Allows hash_node access to the protected copy constructor for hashing.
  template <class K, class V> friend class hash_node;
  // Allows binary_tree access to the protected constructors for creation.
  friend class binary_tree<T, P>;
  // Establishes two-way friendship between tree-based and array-based heaps.
  friend class heap<T>;

  public:
    // Destructor executes no additional steps. O(N) | O(logN)
    virtual ~heap_tree() {}
    // Casts the return of binary_tree's clone method. O(N) | O(logN)
    virtual heap_tree<T, P, M>* clone() const;
    // Accessors to children and parent return class pointers. O(1) | O(1)
    const heap_tree<T, P, M>* left_ch() const;
    const heap_tree<T, P, M>* right_ch() const;
    const heap_tree<T, P, M>* parent() const;

    // Accessor to the type of the heap_tree. O(1) | O(1)
    bool is_max() const {return M ? true : false;}
    // Leverages completeness guarantee to find last node faster. O(logN) | O(1)
    virtual heap_tree<T, P, M>* last(heap_tree<T, P, M>*& parent);

    // Builds dyn_array from init list, then assigns to itself. O(N+M) | O(N)
    heap_tree<T, P, M>& operator = (std::initializer_list<T> list);
    // Assigns to self with binary_tree operator, then heapifies. O(N+M) | O(N)
    heap_tree<T, P, M>& operator = (const dyn_array<T>& src);
    // Assigns complete trees with binary_tree operator, then heapifies itself.
    // All others are traversed and rebuilt from dyn_array. O(N+M) | O(N)
    heap_tree<T, P, M>& operator = (const binary_tree<T, P>& src);
    // Assigns dyn_array of all bst values, including duplicates. O(N+M) | O(N)
    heap_tree<T, P, M>& operator = (const binary_search_tree<T, P>& src);
    // Duplicates the structure of the array-based heap<T>. O(N+M) | O(N)
    heap_tree<T, P, M>& operator = (const heap<T>& hp);

    // Calls binary_tree insert, then sifts up inserted node. O(N) | O(logN)
    virtual heap_tree<T, P, M>* insert(const T& val);
    // Runs targeted bst removal, then sifts down replaced val. O(N) | O(logN)
    virtual heap_tree<T, P, M>* remove(const T& val);
    virtual heap_tree<T, P, M>* remove(const heap_tree<T, P, M>* target);
    // Replaces root value with deepest rightmost node value, then deletes that
    // node, sifts down root's value then returns root. O(logN) | O(1)
    virtual T remove();
    // Recursively heapify the subtree rooted at this node. O(N) | O(logN)
    heap_tree<T, P, M>* heapify();

  protected:
    heap_tree(const T& v): binary_tree<T, P>(v) {}
    heap_tree(const heap_tree<T, P, M>& src): binary_tree<T, P>(src) {}
    // Hiding methods protected for internal use, to discourage manual editing.
    heap_tree<T, P, M>*& left();
    heap_tree<T, P, M>*& right();
    heap_tree<T, P, M>*& prnt();
    const heap_tree<T, P, M>* left() const;
    const heap_tree<T, P, M>* right() const;
    const heap_tree<T, P, M>* prnt() const;
    // Sifts down the node it is called from. O(logN) | O(1)
    heap_tree<T, P, M>* sift_down();
    // Internal method to compare the values of this node and another.
    // For a min/max heap, returns true if this node's value is smaller/larger.
    // If this method returns true, this node should be above the other.
    bool compare(heap_tree<T, P, M>* other) const;
};

// A self-balancing binary_search_tree<T, P> that is primarily responsible for:
// - Valid height maintenance.
// - Search tree rotations
// - A strict self-balancing routine that uses recursive rotations to comply
//   with the formal definition of a balanced tree at all times.
//
// Overrides insert and remove methods with their recursive versions to allow
// direct integration of the self-balancing routine into these operations.
// Both insertion and removal recursions seek their targets in their winding
// phase, and rebalance locally with tree rotations in their unwinding phase.
//
// Inherited methods do a lot of the heavy lifting in avl_tree<T, P>'s handling
// of its core search functions, local node removal logic, assignments from
// other types of binary trees, recursive rebuilding from value arrays,
// duplicate val handling and on-demand balancing with completeness guarantee.
//
// Like its parent, avl_tree<T, P> hides its standard accessors and mutators to
// discourage manual edits, and exposes the const-only set instead for
// inspections and traversals.
//
// Owns all the memory of its descendant nodes. Observes the rule of 3.
// Restricted to heap-only instantiation through factory method in binary_tree.
// Assumes T is default-constructable with equality and relational operators
// as well as a signed distance metric defined via the - operator.
// Use when rigid self-balancing is needed and higher overhead isn't a concern.
template <class T, bool P>
class avl_tree : public binary_search_tree<T, P>
{
  // Allows hash_node access to the protected copy constructor for hashing.
  template <class K, class V> friend class hash_node;
  // Allows binary_tree access to the protected constructors for creation.
  friend class binary_tree<T, P>;

  public:
    // Destructor executes no additional steps. O(N) | O(logN)
    virtual ~avl_tree() {}
    // Casts the return of binary_tree's clone method. O(N) | O(logN)
    virtual avl_tree<T, P>* clone() const;
    // Accessors to children and parent return class pointers. O(1) | O(1)
    const avl_tree<T, P>* left_ch() const;
    const avl_tree<T, P>* right_ch() const;
    const avl_tree<T, P>* parent() const;

    // Computes and returns the balance factor of the node. O(1) | O(1)
    int balance_factor() const;
    // Builds dyn_array from init list, then assigns it with its own operator.
    // O(N*logN) average, O(N^2) worst | O(N)
    avl_tree<T, P>& operator = (std::initializer_list<T> list);
    // Runs binary_search_tree operator with its own from_dyn_arr override.
    // O(N*logN) average, O(N^2) worst | O(N)
    avl_tree<T, P>& operator = (const dyn_array<T>& src);
    // Runs binary_search_tree operator with its own from_dyn_arr override.
    // O(N*logN) average, O(N^2) worst | O(N)
    avl_tree<T, P>& operator = (const binary_tree<T, P>& src);
    // Runs binary_tree operator, then balances itself. O(N+M) | O(N)
    avl_tree<T, P>& operator = (const binary_search_tree<T, P>& src);

    // Recursively searches for insert location to preserve AVL properties.
    // Unlike parent's method, returns root as it can change. O(logN) | O(logN)
    virtual avl_tree<T, P>* insert(const T& val);
    // Recursively searches for removal target, balancing nodes during its
    // unwinding phase after the node is deleted. Returns root.
    // Node values may change due to val swaps. O(logN) | O(logN)
    virtual avl_tree<T, P>* remove(const T& val);
    virtual avl_tree<T, P>* remove(const avl_tree<T, P>* target);

  protected:
    avl_tree(const T& v): binary_search_tree<T, P>(v) {}
    avl_tree(const avl_tree<T, P>& src): binary_search_tree<T, P>(src) {}
    // Hiding methods protected for internal use, to discourage manual editing.
    avl_tree<T, P>*& left();
    avl_tree<T, P>*& right();
    avl_tree<T, P>*& prnt();
    const avl_tree<T, P>* left() const;
    const avl_tree<T, P>* right() const;
    const avl_tree<T, P>* prnt() const;
    // Calls binary_search_tree's method, then computes heights. O(N) | O(logN)
    virtual avl_tree<T, P>* from_dyn_arr(const dyn_array<T>& uniques,
                                         const dyn_array<size_t>& cn,
                                         size_t start,
                                         size_t end,
                                         bool first = true);
    // Updates height from children then rotates for balance factor. O(1)| O(1)
    avl_tree<T, P>* balance_node();
    // Standard left and right binary tree rotations. O(1)| O(1)
    virtual avl_tree<T, P>* rotate_left();
    virtual avl_tree<T, P>* rotate_right();
    // Updates node's height from heights of its children, if any. O(1)| O(1)
    avl_tree<T, P>* update_node_height();

  private:
    // Remove method that doesn't specify a target is hidden.
    // Removing leftmost bottommost element can break AVL tree balance.
    virtual T remove() {return this->val();}
};

// Can be thought of as a specialized avl_tree<T, true> with looser standards
// for balance, and lower operational overhead. Self-balances using a node
// colorization scheme combined with avl_tree<T, true> rotations to cap the
// longest root-to-leaf path at twice the length of the shortest, which also
// requires red_black_tree<T> to maintain valid heights, like its parent class.
//
// Needs parent links for its core color-balancing logic, therefore it is
// defined exclusively as a ParentLinkedType.
// Attempts to create red_black_tree<T> nodes without parent links (i.e., from
// a P=false namespace or specialization) will trigger a compiler error if the
// offender is a constant expression; otherwise it will be caught at runtime by
// the switch list in the enabled factory method, which won't recognize it.
//
// Makes extensive use of inheritance and polymorphism, and relyies on
// avl_tree<T, true> for its rotations and height maintenance, and on
// binary_search_tree<T, true> for the entirety of its search functionality,
// the backbone of its insert and remove methods, on-demand balancing with
// completeness guarantee, duplicate value handling mechanism and == operators.
// Most red_black_tree<T> virtual overrides call an equivalent ancestor method
// to duplicate their behavior, then post-process to fix color violations.
// In the same way, its assignment operators typically invoke an ancestor's
// operator to duplicate the source structure, then colorize tree from scratch.
//
// Instead of storing node colors as an additional member at each node,
// red_black_tree<T> manipulates the sign of the count_ member inherited from
// binary_search_tree<T, true> so that red nodes store their duplicate counts
// as negative integers; and overrides all inherited virtual accessors and
// mutators of count_ to account for this change. This insulates the internal
// color storage mechanism from the outside world, and ensures that all
// inherited methods remain unaffected.
//
// Unless manually tampered with, red_black_tree<T> instances should never
// contain any violations. Nevertheless, a set of methods are provided for
// granular inspections and color-based validity checks.
// All other color-related functionality is considered internal, with const
// accessors provided for convenience.
//
// Two red_black_tree<T> instances are considered equal if their structures,
// node values and duplicate counts are identical, regardless of node coloring.
// Like its parent, it exposes the const-only accessor set only, in order to
// discourage manual edits to its structure.
// Most operations will return the tree root since entire tree needs to be
// checked and fixed for any violations after edits.
// Owns all the memory of its descendant nodes. Observes the rule of 3.
// Use when self-balancing is needed under frequent insertions and deletions.
// Restricted to heap-only instantiation through factory method in binary_tree.
// Assumes T is default-constructable with equality and relational operators
// as well as a signed distance metric defined via the - operator.
template <class T>
class red_black_tree : public avl_tree<T, true>
{
  // Allows hash_node access to the protected copy constructor for hashing.
  template <class K, class V> friend class hash_node;
  // Allows binary_tree access to the protected constructors for creation.
  // Compiler wants access to P=false version even though it won't be invoked.
  friend class binary_tree<T, true>;
  friend class binary_tree<T, false>;

  public:
    // Destructor executes no additional steps. O(N) | O(logN)
    virtual ~red_black_tree() {}
    // Casts the return of binary_tree's clone method. O(N) | O(logN)
    virtual red_black_tree<T>* clone() const;
    // Accessors to children and parent return class pointers. O(1) | O(1)
    const red_black_tree<T>* left_ch() const;
    const red_black_tree<T>* right_ch() const;
    const red_black_tree<T>* parent() const;

    // Checks node color. O(1) | O(1)
    bool is_red() const {return this->count_ < 0;}
    // Checks if this node's subtree is a valid red-black tree. O(N) | O(logN)
    bool is_valid() const;
    // Polymorphic override flips sign of red node counts. O(1) | O(1)
    virtual int count() const;

    // Builds dyn_array from init list, then assigns it with its own operator.
    // O(N*logN) average, O(N^2) worst | O(N)
    red_black_tree<T>& operator = (std::initializer_list<T> list);
    // Runs binary_search_tree operator with its own from_dyn_arr override.
    // O(N*logN) average, O(N^2) worst | O(N)
    red_black_tree<T>& operator = (const dyn_array<T>& src);
    // Runs binary_search_tree operator with its own from_dyn_arr override.
    // O(N*logN) average, O(N^2) worst | O(N)
    red_black_tree<T>& operator = (const binary_tree<T, true>& src);
    // Runs avl_tree operator, then colorizes the tree. O(N+M) | O(N)
    red_black_tree<T>& operator = (const binary_search_tree<T, true>& src);
    // Runs binary_tree operator, then colorizes the tree. O(N+M) | O(N)
    red_black_tree<T>& operator = (const avl_tree<T, true>& src);

    // Runs binary_tree method, then casts to class pointer. O(logN) | O(1)
    red_black_tree<T>* find_root();
    const red_black_tree<T>* find_root() const;

    // Calls binary_search_tree insert, then fixes red-red violations.
    // Returns new root as it can change during fixes. O(logN) | O(1)
    virtual red_black_tree<T>* insert(const T& val);
    // Calls binary_search_tree removal_engine, then fixes double-blacks.
    // Returns new root as it can change. O(logN) | O(logN)
    virtual red_black_tree<T>* remove(const T& val);
    // Pointer version directly accesses target's parent. O(logN) | O(logN)
    virtual red_black_tree<T>* remove(const red_black_tree<T>* target);

    // Fixes red-red violations post-insertion. O(logN) | O(1)
    red_black_tree<T>* fix_red(red_black_tree<T>* node);
    // Fixes black violations post-removal. O(logN) | O(1)
    red_black_tree<T>* fix_black(red_black_tree<T>* node,
                                 red_black_tree<T>* parent);
    // Colorize from scratch using min heights and black budget. O(N) | O(logN)
    red_black_tree<T>* colorize();
    // Recursively checks downwards for 2 consecutive red nodes. O(N) | O(logN)
    bool red_violation() const;
    // Checks if a root-to-leaf path has different black count. O(N) | O(logN)
    bool black_violation() const;

  protected:
    red_black_tree(const T& v): avl_tree<T, true>(v) {}
    red_black_tree(const red_black_tree<T>& src): avl_tree<T, true>(src) {}
    // Hiding methods protected for internal use, to discourage manual editing.
    red_black_tree<T>*& left();
    red_black_tree<T>*& right();
    red_black_tree<T>*& prnt();
    const red_black_tree<T>* left() const;
    const red_black_tree<T>* right() const;
    const red_black_tree<T>* prnt() const;
    // Calls avl_tree's method, then colorizes the tree. O(N) | O(logN)
    virtual red_black_tree<T>* from_dyn_arr(const dyn_array<T>& uniques,
                                            const dyn_array<size_t>& cn,
                                            size_t start,
                                            size_t end,
                                            bool first = true);
    // Calls binary_search_tree's remove_child, then links parents.
    // Returns parent of the node actually deleted. O(logN) | O(1)
    virtual red_black_tree<T>* remove_child(
        binary_search_tree<T, true>* target);
    // Rotator overrides mainly for convenient chaining; they just call
    // avl_tree's rotation methods and dynamic_cast the returned pointer.
    virtual red_black_tree<T>* rotate_left();
    virtual red_black_tree<T>* rotate_right();
    // Computes minimum of two branch heights for each node. O(N) | O(logN)
    size_t compute_min_heights();
    // Computes and validates black heights simultaneously. O(N) | O(logN)
    int black_height() const;
    red_black_tree<T>* set_color(bool color);
    // Polymorphic setter for assigning negative counts to red nodes.
    virtual red_black_tree<T>* set_count(int count);
    // Polymorphic override surrounds red nodes with <> instead of ().
    virtual std::ostream& print_node(std::ostream& os) const;
};

// Templated class method implementations need to be accessible at the
// point of instantiation.
#include "binary_tree.tpp"

#endif