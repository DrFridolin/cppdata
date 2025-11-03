#ifndef graph_h_
#define graph_h_

#include <iostream>
#include <iomanip>
#include <cassert>
#include <initializer_list>
#include <functional>
#include <limits>
#include "dyn_array.h"
#include "stack.h"

template <class T> class graph_node;
template <class T> class graph;
template <class T> class stack;
template <class K, class V> class hash_table;

using sq_init_list = std::initializer_list<std::initializer_list<int> >;
template <class T> using gn_ptrs = dyn_array<graph_node<T>*>;
template <class T> using cgn_ptrs = dyn_array<const graph_node<T>*>;

// Node-only connected graph class where each node holds a value, an index and
// a dyn_array to store pointers to its neighboring nodes.
// This self-referential design is sufficient on its own to structurally and
// algorithmically represent acyclic unweighted connected graphs and trees.
// Per-node adjacency list representation is inherently capable of supporting
// both directed and undirected graphs, and the use of dyn_array<T> unlocks
// support for dynamic graphs.
//
// The design intention is for graph_node<T> to also serve as a maximally-
// configurable base class for all current and future self-referential tree
// and graph classes. To this end, it is minimally restrictive and makes almost
// no assumptions about its structure, while still providing engines for core
// functionality to be shared through inheritance.
// Currently, the entire binary_tree<T, P>> inheritance hierarchy, as well as
// the non-templated trie inherit graph_node<T> functionality.
// It owns and manages no memory, and delegates this responsibility to wrappers
// and descendants, or to the user if being used directly.
//
// Standard traversals are handled by protected engines that can be customized
// with simple lambdas that specify target types and early exit conditions.
// These engines make extensive use of hash_table<uintptr_t, V> and/or a
// queue<const graph_node<T>*> to track visited nodes.
// For hashing, pointers are cast to uintptr_t first.
//
// Two sets of accessors and mutators are provided, one for direct access to
// adjacency list pointers, and another for indexing non-nullptr elements,
// which represent existing neighbors.
// Following this distinction, capacity() returns the actual allocated size of
// the adjacency list, while degree() returns the number of neighboring nodes
// that actually exist. These can be thought of as graph_node<T> equivalents of
// capacity() vs size() in dyn_array<T>.
//
// Neighbors are attached via non-const access to the adjacency list using one
// of the two operators mentioned above, or by calling add_edge on the target
// node; either way the node being attached has to be created beforehand.
// The index member ind_ is only exposed through a const accessor, as its
// primary purpose is to assume context-dependent roles in descendant and
// wrapper classes. For example, graph<T> uses it to uniquely index its nodes,
// while binary_tree<T, P> hierarchy uses it to store node heights.
//
// Due to limitations both inherent and deliberate in its design, graph_node<T>
// does not support disconnected subgraphs, cycles, loops or bidirectional
// searches on directed graphs. Graphs with disconnected components can only be
// represented as a set of separate graph_node<T> chains.
// Chains with directed edges can have nodes unreachable from any other node.
// Cyclic graphs and trees can be created, but traversal attempts will result
// in infinite loops, as support for loops and cycles are delegated to
// classes like graph<T> that globally manage their connectivity structure.
template <class T>
class graph_node
{
  friend class graph<T>;
  public:
    // Default constructor initializes adjacency lists with size 2. O(1) | O(1)
    graph_node();
    // Instantiate with all-nullptr adjacency list of size n. O(A) | O(A)
    graph_node(size_t n);
    // Initialize with node value and adjacency list size.  O(A) | O(A)
    graph_node(const T& v, size_t n);
    // Virtual destructor for polymorphism via reinterpret_cast. O(1) | O(1)
    virtual ~graph_node() {}

    // Number of non-nullptr neighbors the node currently has. O(A) | O(1)
    size_t degree() const;
    // Maximum number of neighbors the node can support. O(1) | O(1)
    size_t capacity() const {return this->adj_.size();}
    // Get/set node value. O(1) | O(1)
    const T& val() const {return this->val_;}
    graph_node<T>& set_val(const T& v) {this->val_ = v; return *this;}
    // Read-only access to the node's index. O(1) | O(1)
    size_t ind() const {return this->ind_;}

    // Equality check operators compare the node values and sets of immediate
    // neighbor values, permutations being considered equal. Indices ignored.
    // O(1) average, O(A^2) worst | O(1)
    bool operator == (const graph_node<T>& other) const;
    bool operator != (const graph_node<T>& other) const;
    // Relational operators compare node values first, node degrees second.
    // O(1) average, O(A) worst | O(1)
    bool operator < (const graph_node<T>& other) const;
    bool operator > (const graph_node<T>& other) const;
    bool operator <= (const graph_node<T>& other) const;
    bool operator >= (const graph_node<T>& other) const;
    // Sets the neighborhood structure from initializer list. Caller needs to
    // still manage lifetimes and avoid cycles/loops. O(A) | O(A)
    graph_node<T>& operator = (std::initializer_list<graph_node<T>*> list);

    // Resizes the dyn_array holding node's neighbor pointers. O(A) | O(A)
    graph_node<T>& resize_adj(size_t n) {this->adj_.resize(n); return *this;}
    // Adds a new neighbor using a pointer to the new neighbor. O(A) | O(1)
    graph_node<T>& add_edge(graph_node<T>* neighbor_ptr);
    // Clears the adjacency list for the node. O(A) | O(1)
    graph_node<T>& clear_edges();

    // Read/write access to the i-th existing neighbor of the node.
    // Allows neighbor loops with degree() method. O(A) | O(1)
    graph_node<T>*& operator () (size_t i);
    // Read-only access to the i-th existing neighbor of the node.
    // Allows neighbor loops with degree() method. O(A) | O(1)
    const graph_node<T>* operator () (size_t i) const;
    // Direct read/write access to neighboring graph_node<T> pointers.
    // Allows edits to both pointers and nodes they point to. O(1) | O(1)
    graph_node<T>*& operator [] (size_t n);
    // Direct read-only access to a const graph_node's neighbor pointers.
    // Disallows all edits on pointers and nodes they point to. O(1) | O(1)
    const graph_node<T>* operator [] (size_t n) const;

    // Fully traverses the graph reachable from this node in a depth-first
    // fashion, returns full path as graph_node pointer array.
    // If is_post=true, each node will be visited after the entire network of
    // nodes accessible from it are visited. O(N+E) | O(N)
    gn_ptrs<T>  depth_first_traverse(std::ostream* os_ptr = nullptr,
                                     bool is_post = false);
    cgn_ptrs<T> depth_first_traverse(std::ostream* os_ptr = nullptr,
                                     bool is_post = false) const;
    // Depth-first search for a node object, immediate stop if/when found.
    // Returns path to target, or full traversal path if it's not found.
    // O(N_v+E_v) average, O(N+E) worst | O(N)
    gn_ptrs<T>  depth_first_search(const graph_node<T>* target,
                                   std::ostream* os_ptr = nullptr);
    cgn_ptrs<T> depth_first_search(const graph_node<T>* target,
                                   std::ostream* os_ptr = nullptr) const;
    // Depth-first search for a node value, immediate stop if/when found.
    // Returns path to target, or full traversal path if it's not found.
    // O(N_v+E_v) average, O(N+E) worst | O(N)
    gn_ptrs<T>  depth_first_search(const T& target,
                                   std::ostream* os_ptr = nullptr);
    cgn_ptrs<T> depth_first_search(const T& target,
                                   std::ostream* os_ptr = nullptr) const;

    // Fully traverses the graph reachable from this node in a breadth-first
    // fashion, returns full path as graph_node pointer array. O(N+E) | O(N)
    gn_ptrs<T>  breadth_first_traverse(std::ostream* os_ptr = nullptr);
    cgn_ptrs<T> breadth_first_traverse(std::ostream* os_ptr = nullptr) const;
    // Breadth-first search for a node object, immediate stop if/when found.
    // Guaranteed to return shortest path to target node if it is found.
    // Returns path to target, or full traversal path if it's not found.
    // O(N_D + E_D) average, O(N+E) worst | O(N)
    gn_ptrs<T>  breadth_first_search(const graph_node<T>* target,
                                     std::ostream* os_ptr = nullptr);
    cgn_ptrs<T> breadth_first_search(const graph_node<T>* target,
                                     std::ostream* os_ptr = nullptr) const;
    // Breadth-first search for a node value, immediate stop if/when found.
    // Guaranteed to return shortest path to closest node with matching value.
    // Returns path to node, or full traversal path if no such node is found.
    // O(N_D + E_D) average, O(N+E) worst | O(N)
    gn_ptrs<T>  breadth_first_search(const T& target,
                                     std::ostream* os_ptr = nullptr);
    cgn_ptrs<T> breadth_first_search(const T& target,
                                     std::ostream* os_ptr = nullptr) const;

    // Two simultaneous BFS to find a path from input node to this node.
    // Returns the path found in graph_node<T> pointer array format.
    // Only supports undirected graphs as backwards edges are not stored
    // to keep this class compact, and since running a bfs to reverse the
    // edges removes the need to run a bidirectional search afterwards.
    // O(N_D/2 + E_D/2) average, O(N+E) worst | O(N)
    gn_ptrs<T>  bidirectional_search(const graph_node<T>* target,
                                     std::ostream* os_ptr);
    cgn_ptrs<T> bidirectional_search(const graph_node<T>* target,
                                     std::ostream* os_ptr) const;
    // Detects whether there is a cycle reachable from this node. O(N+E) | O(N)
    bool has_cycle() const;

    // Prints node's value, followed by a sorted sequence of neighbor values.
    // O(AlogA) average, O(A^2) worst | O(1)
    std::ostream& print_sorted(std::ostream& os) const;

    // Print set of neighboring edge values per node. O(A) | O(1)
    template <class U>
    friend std::ostream& operator << (std::ostream& os,
                                      const graph_node<U>& gn);

  protected:
    // Initialize with node value, adjacency list size and index. Primarily
    // used by descendant and friend classes to store additional attributes.
    graph_node(const T& v, size_t n, size_t i);
    // Recursive depth-first traversal engine to support the public methods.
    // Deals with graph_node pointers in uintptr_t format to support both.
    // const and non-const public traversal and search methods. O(N+E) | O(N)
    bool dft(std::function<bool(const graph_node<T>*)> check,
             std::ostream* os_ptr,
             hash_table<uintptr_t, uintptr_t>& ht,
             dyn_array<uintptr_t>& path,
             bool is_post = false) const;
    // Iterative breadth-first traversal engine to support the public methods.
    // Deals with graph_node pointers in uintptr_t format to support both.
    // const and non-const public traversal and search methods. O(N+E) | O(N)
    dyn_array<uintptr_t> bft(std::function<int(const graph_node<T>*)> check,
                             std::ostream* os_ptr) const;
    // Recursive cycle detection engine.
    bool has_cyc(hash_table<uintptr_t, bool>& ht) const;
    // Reconstruct path back from this node using hash table. O(D) | O(D)
    gn_ptrs<T>  reconstruct_path(hash_table<uintptr_t, uintptr_t>& ht);
    cgn_ptrs<T> reconstruct_path(hash_table<uintptr_t, uintptr_t>& ht) const;

    // Node's value.
    T val_;
    // Resizable array holds pointers to other nodes.
    gn_ptrs<T> adj_;
    // Node's private index. Utilized for different purposes by other classes.
    size_t ind_;
};

// General-purpose lightweight union-find class for set merge operations.
class union_find
{
  public:
    union_find(int n) : parent(0, n), rank(0, n)
    {
      for (int i = 0; i < n; ++i)
        parent[i] = i;
    }

    int find(int x)
    {
      if (parent[x] != x)
        parent[x] = find(parent[x]);
      return parent[x];
    }

    void union_sets(int x, int y)
    {
      int root_x = find(x);
      int root_y = find(y);
      if (root_x != root_y) {
        if (rank[root_x] > rank[root_y]) {
          parent[root_y] = root_x;
        }
        else if (rank[root_x] < rank[root_y]) {
          parent[root_x] = root_y;
        }
        else {
          parent[root_y] = root_x;
          ++rank[root_x];
        }
      }
    }

    dyn_array<int> parent;
    dyn_array<int> rank;
};

// Global unscoped enum of Tarjan's algorithm modes.
enum TarjanMode {SCC, BRIDGE, ARTICULATION};

// Storage class that bundles all auxiliary variables passed between recursions
// in Tarjan's algorithm. Allocates only the data structures required by the
// TarjanMode specified in its construction.
class tarjan_bundle
{
  public:
    tarjan_bundle(TarjanMode mode, size_t n): mode_(mode), timer_(0)
    {
      this->disc = dyn_array<int>(-1, n);
      this->low  = dyn_array<int>(-1, n);
      if (mode == SCC)
        this->on_stack = dyn_array<bool>(false, n);
      else if (mode == BRIDGE || mode == ARTICULATION)
        this->parent   = dyn_array<int>(-1, n);
      if (mode == ARTICULATION)
        this->art_pts  = dyn_array<bool>(false, n);
    }

    TarjanMode mode() const {return this->mode_;}
    int timestamp() {return this->timer_++;}
    void reset_timer()  {this->timer_ = 0;}
    void push(size_t u) {this->dft_stack.push(u);}
    size_t pop() {return this->dft_stack.pop();}
    bool is_empty() const {return this->dft_stack.is_empty();}

    // Discovery time of each node, -1 for undiscovered.
    dyn_array<int> disc;
    // Lowest discovery time reachable from the subtree rooted at each node.
    dyn_array<int> low;
    // Parent indices of each node, -1 for undiscovered.
    dyn_array<int> parent;
    // Flags indicating for each node whether it's currently on DFT stack.
    dyn_array<bool> on_stack;
    // Stack to track recursive node order to build connected components later.
    stack<size_t> dft_stack;
    // Members to store outputs of different modes.
    dyn_array<dyn_array<size_t> > scc_arr;
    dyn_array<dyn_array<size_t> > bridges;
    dyn_array<bool> art_pts;

  private:
    TarjanMode mode_;
    // Counter for assigning discovery times.
    int timer_;
};

// Compositional graph class for weighted and unweighted graphs, compatible
// with both directed or undirected edges. Observes the rule of 3.
// Supports loops, cycles and graphs with multiple disconnected components.
// Dynamically allocates and owns the memory for its graph_node<T> instances to
// ensure stable adjacency list resizing, and stores their pointers in a
// dyn_array member for easy direct access.
//
// Nodes are uniquely indexed by assigning their index in this dyn_array member
// to their protected ind_ members upon creation. Node order never changes, so
// indices remain unique and unchanged through each node's lifetime.
// All class methods handle nodes using these unique indices rather than their
// pointers, and traversals return index sequences rather than pointer paths.
//
// For its edge representation, graph<T> employs a hybrid design by maintaining
// per-node local adjacency lists of its graph_node<T> members, and in parallel
// tracking the same connectivity structure using a fully-owned dual-purpose
// adjacency/weight matrix member in order to offer support for the widest
// possible range of graph algorithms.
// The dual-purpose matrix is stored as a 2D dyn_array of ints that carries
// edge weights when representing weighted graphs, and is treated as a boolean
// adjacency matrix by unweighted graphs, with 0 elements indicating a lack of
// an edge in either case.
// This hybrid design also unlocks the complete set of graph_node<T> methods
// for reuse, and indeed all graph<T> traversals are thin wrappers around their
// graph_node<T> counterparts that merely convert pointers to indices.
//
// None of the class methods expose mutable node pointers, and manual edits to
// the node or edge structure of the graph are disallowed.
// Instead, a set of dedicated class methods and operators monopolize creation
// and destruction of all class components to keep the hybrid design tractable.
// Algorithms that need to modify the graph on-the-fly are still allowed to do
// so if they're implemented as non-const methods.
// By jointly maintaining both list and matrix representations for edges,
// graph<T> admits an increase in the operational overhead of these deducated
// methods, and in return provides all class methods with both options to parse
// graph edges, allowing some graph algorithms to reach optimal performance.
// Methods can also alternate between the two representations, or use both of
// them at the same time.
// All non-algorithmic handling of this edge matrix is agnostic to whether the
// graph is weighted or not; and the class currently runs no compatibility
// checks based on the contents of its matrix member.
//
// Assumes T is default-constructable and has equality/inequality operators.
// Supports 2 types of list/dyn_array inputs for construction and assignment:
// - 1D list/array to assign values to existing nodes and create the missing
//     ones without adding or removing any edges.
// - 2D list/array to rebuild the edge structure from scratch, without changing
//     any node values but adding and removing them as needed to comply with
//     the size dictated by the edge matrix.
template <class T>
class graph
{
  public:
    graph() {}
    // Construct from a list of node values. O(1) | O(1)
    graph(std::initializer_list<T> node_vals);
    // Construct from a dyn_array of node values. O(N) | O(N)
    graph(const dyn_array<T>& node_vals);
    // Construct from edge matrix as a 2D initializer list. O(N^2) | O(N^2)
    graph(sq_init_list adj_mat);
    // Construct from edge matrix as a 2D dyn_array. O(N^2) | O(N^2)
    graph(const dyn_array<dyn_array<int> >& adj_mat);
    // Copy constructor for deep copy of nodes. O(N^2) | O(N^2)
    graph(const graph<T>& source);
    // Destructor deletes each graph node. O(N) | O(1)
    ~graph();

    // Read-only access to a graph_node using its index. O(1) | O(1)
    const graph_node<T>& operator [] (size_t ind) const;
    // Converts indices to node values, skips if out-of-bounds. O(1) | O(1)
    dyn_array<T> operator [] (const dyn_array<size_t>& inds) const;
    // Read-only access to graph_node value using its index. O(1) | O(1)
    const T& operator () (size_t ind) const;
    // Read/write access to graph_node value using its index. O(1) | O(1)
    T& operator () (size_t ind);

    // Checks if the graph is empty using the node array. O(1) | O(1)
    bool is_empty() const {return this->nodes_.is_empty();}
    // Returns the number of nodes using the node array. O(1) | O(1)
    size_t size() const {return this->nodes_.size();}
    // Counts the edges using the adjacency/weight matrix. O(N^2) | O(1)
    size_t num_edges() const;
    // Deletes nodes, then clears node array and edge matrix. O(N^2) | O(1)
    graph<T>& clear();
    // Clears node adjacency lists and the edge matrix. O(N^2) | O(1)
    graph<T>& clear_edges();

    // Equality check operators compare node values and edge structure.
    // Permutations of identical structures are considered not equal.
    // O(1) average, O(N^2*A^2) worst | O(1)
    bool operator == (const graph<T>& other) const;
    bool operator != (const graph<T>& other) const;
    // Relational operators compare node counts first, edge counts second.
    // O(1) average, O(N^2) worst | O(1)
    bool operator < (const graph<T>& other) const;
    bool operator > (const graph<T>& other) const;
    bool operator <= (const graph<T>& other) const;
    bool operator >= (const graph<T>& other) const;
    // Performs deep copies the node and edge structure. O(N^2+M^2) | O(N^2)
    graph<T>& operator = (const graph<T>& source);
    // Rebuilds graph edges from the given 2D square adjacency/weight matrix.
    // Preserves existing node values/indices while removing excess nodes.
    // If list is larger, pads default nodes with no edges. O(N^2+M^2) | O(N^2)
    graph<T>& operator = (sq_init_list adj_mat);
    graph<T>& operator = (const dyn_array<dyn_array<int> >& adj_mat);
    // Replaces node values in index order with the given sequence of values.
    // If list/array size is larger, creates new isolated default-valued nodes.
    // Otherwise, only changes nodes within input bounds. O(N^2+M^2) | O(N^2)
    graph<T>& operator = (std::initializer_list<T> node_vals);
    graph<T>& operator = (const dyn_array<T>& node_vals);
    // Rebuilds graph to duplicate the subgraph input. O(N^2+M^2) | O(N^2)
    graph<T>& operator = (graph_node<T>* subgraph);

    // Get a copy of the min/max node value. O(N) | O(1)
    T min() const;
    T max() const;
    // Returns dyn_array of all node values. O(N) | O(N)
    dyn_array<T> node_vals() const;
    // Returns dyn_array of all node indices with the given value. O(N) | O(N)
    dyn_array<size_t> node_inds(const T& val) const;
    // Returns dyn_array of neighbors of the node at index ind. O(A) | O(A)
    dyn_array<size_t> neighbors(size_t ind) const;
    // Returns a copy of the edge matrix as 2D dyn_array O(N^2) | O(N^2).
    const dyn_array<dyn_array<int> >& adj_mat() const {return this->edges_;}

    // Creates a graph_node on heap memory holding the given value and no
    // neighbors, then adds it to the graph and returns its index.
    // O(N) average, O(N^2) worst | O(N)
    size_t add_node(const T& val);
    // Adds a graph_node and all nodes connected to and reachable from it.
    // Matches indices of subgraph nodes to cloned node indices in the graph.
    // Returns the index assigned to the input node.
    // O((N*S)+(S^2)+E_S) average, O((N^2*S)+(S^2)+E_S) worst | O((N*S)+(S^2))
    size_t add_node(graph_node<T>* subgraph);
    // Removes the graph_node with the given index.
    // Returns false if node with that index doesn't exist. O(N^2) | O(1)
    bool remove_node(size_t ind);
    // Adds a directed edge going from node with index i to node with index j.
    // Returns false if a node with either index doesn't exist. O(1) | O(1)
    bool add_edge(size_t i, size_t j);
    // Adds a directed weighted edge from node w. index i to node w. index j.
    // Returns false if a node with either index doesn't exist. O(1) | O(1)
    bool add_edgew(size_t i, size_t j, int weight);
    // Adds an undirected edge between node with index i and node with index j.
    // Returns false if a node with either index doesn't exist. O(1) | O(1)
    bool add_edge_undir(size_t i, size_t j);
    // Adds an undirected weighted edge between nodes with indices i and j.
    // Returns false if a node with either index doesn't exist. O(1) | O(1)
    bool add_edgew_undir(size_t i, size_t j, int weight);
    // Removes the edge going from node with index i to the node with index j.
    // Returns false if the edge doesn't exist or if an index is out of bounds.
    // O(A) average, O(N) worst | O(1)
    bool remove_edge(size_t i, size_t j);
    // Removes all edges between the nodes with indices i and j.
    // Returns false if the edge doesn't exist or if indices are out of bounds.
    // O(A) average, O(N) worst | O(1)
    bool remove_edge_undir(size_t i, size_t j);

    // Depth-first traverse with graph_node<T> method. O(N+E) | O(N)
    dyn_array<size_t> depth_first_traverse(size_t from_ind,
                                        std::ostream* os_ptr = nullptr) const;
    // Breadth-first traverse with graph_node<T> method. O(N+E) | O(N)
    dyn_array<size_t> breadth_first_traverse(size_t from_ind,
                                        std::ostream* os_ptr = nullptr) const;
    // Depth-first search. O(N_D+E_D) average, O(N+E) worst | O(N)
    dyn_array<size_t> depth_first_search(size_t from_ind, const T& val,
                                        std::ostream* os_ptr = nullptr) const;
    // Breadth-first search. O(N_D+E_D) average, O(N+E) worst | O(N)
    dyn_array<size_t> breadth_first_search(size_t from_ind, const T& val,
                                        std::ostream* os_ptr = nullptr) const;
    // Bidirectional search. O(N_D/2 + E_D/2) average, O(N+E) worst | O(N)
    dyn_array<size_t> bidirectional_search(size_t i, size_t j,
                                        std::ostream* os_ptr = nullptr) const;

    // Returns pointers to all the nodes with no incoming edges. O(N^2) | O(N)
    dyn_array<size_t> find_roots() const;
    // Runs 3-color depth-first traversal with hash tables. O(N+E) | O(N)
    bool has_cycle() const;
    // Computes node in-degrees, then sorts with depth-first traversal. Returns
    // topologically sorted indices, empty if cycles exist. O(N+E) | O(N)
    dyn_array<T> topological_sort() const;
    // Finds strongly-connected components of a directed graph. O(N+E) | O(N)
    dyn_array<dyn_array<size_t> > strongly_conn_comp() const;
    // Finds weakly-connected components using hashed depth-first traversal and
    // union-find to return contiguous labels in index order. O(N+E) | O(N)
    dyn_array<int> weakly_conn_comp() const;
    // Finds all cut-edges, which are edges whose deletion would increase the
    // graph's number of connected components. O(N+E) | O(N)
    dyn_array<dyn_array<size_t> > bridges() const;
    // Finds all cut-vertices, whose deletion together with their edges would
    // increase the graph's number of connected components. O(N+E) | O(N)
    dyn_array<size_t> articulation_points() const;
    // Finds shortest distances by edge weight from src. O((N+E)*logN) | O(N)
    dyn_array<int> shortest_dists(size_t src) const;
    // Dijkstra's algorithm either finds shortest paths or minimum distances
    // from src to all other nodes, depending on the accumulate arg provided.
    // When it's false, returns only distances at index 0 of the 2D dyn_array.
    // For unreachable nodes, returns -1 distance. O((N+E)*logN) | O(N*D)
    dyn_array<dyn_array<int> > dijkstra(size_t src, bool accumulate = 1) const;
    // Kahn's algorithm for topological sorting and cycle detection. Returns
    // topologically sorted indices, incomplete if cycles exist. O(N+E) | O(N)
    dyn_array<size_t> kahn() const;
    // Tarjan's algorithm with 3 different modes for finding strongly connected
    // components, bridges and articulation points. O(N+E) | O(N)
    void tarjan(size_t ind, tarjan_bundle& tbun) const;

    // Prints node indices and values. O(N) | O(1)
    std::ostream& print_nodes(std::ostream& os) const;
    // Prints node values and values of their neighbors. O(N+E) | O(1)
    std::ostream& print_adj_lists(std::ostream& os) const;
    // Prints node values and sorted neighbor values. O(N+E*logN) | O(1)
    std::ostream& print_sorted_adj_lists(std::ostream& os) const;
    // 	Prints the adjacency/weight matrix. O(N^2) | O(1)
    std::ostream& print_adj_matrix(std::ostream& os) const;

    // Prints all components, aligning corresponding fields. O(N^2) | O(1)
    template <class U>
    friend std::ostream& operator << (std::ostream& os, const graph<U>& graph);

  private:
    // Shared construction engine for all adjacency/weight matrix sources.
    graph<T>& build_adj_mat(const dyn_array<dyn_array<int> >& adj_mat);
    // Recursive backend for cycle detection.
    bool cycle_rec(size_t ind,
                   hash_table<uintptr_t, bool>& visited,
                   hash_table<uintptr_t, bool>& in_stack) const;
    // Recursive backend for connected component labeling.
    // Returns an array of labels encountered during traversal.
    void comp_rec(size_t ind,
                  hash_table<uintptr_t, int>& ht,
                  dyn_array<int>& conn_lbl,
                  union_find& uf) const;

    // Method with template specializtions that determines printout widths.
    void get_dec(int& dec, int& off, int& num, int& max) const;

    dyn_array<graph_node<T>*> nodes_;
    dyn_array<dyn_array<int> > edges_;
};

// Templated class method implementations need to be accessible at the
// point of instantiation
#include "graph.tpp"

#endif