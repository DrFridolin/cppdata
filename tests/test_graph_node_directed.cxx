#include <iostream>
#include "graph.h"
#include "dyn_array.h"
#include "test_util.h"

void print_path(const dyn_array<graph_node<char>*>& path)
{
  std::cout << std::endl << "       | Path: ";
  for (size_t i = 0; i < path.size(); ++i) {
    if (path[i] == nullptr)
      std::cout << "NULL";
    else
      std::cout << path[i]->val();
    if (i < path.size() - 1)
      std::cout << " -> ";
  }
  std::cout << std::endl;
}

int main()
{
  // Re-create the larger undirected graph from test_graph.cxx.
  // First initialize all the nodes with their values.
  graph_node<char> node_A('A', 2),  node_B('B', 5),  node_C('C', 4);
  graph_node<char> node_D('D', 5),  node_E('E', 7),  node_F('F', 3);
  graph_node<char> node_G('G', 4),  node_H('H', 2),  node_I('I', 2);
  graph_node<char> node_J('J', 1),  node_A_('A', 1), node_C_('C', 2);
  graph_node<char> node_D_('D', 3), node_E_('E', 1), node_F_('F', 3);
  graph_node<char> node_G_('G', 3), node_H_('H', 1), node_I_('I', 2);
  // This time use initializer list instantiation/assignment.
  node_A = {&node_B,  &node_E};
  node_B = {&node_A,  &node_C_, &node_D,  &node_E,  &node_I};
  node_C = {&node_D,  &node_E,  &node_F_, &node_I_};
  node_D = {&node_B,  &node_C,  &node_E,  &node_G_, &node_H_};
  node_E = {&node_A,  &node_B,  &node_C,  &node_D,  &node_E, &node_F, &node_G};
  node_F = {&node_D_, &node_E,  &node_G};
  node_G = {&node_A_, &node_E,  &node_F,  &node_H};
  node_H = {&node_E_, &node_G};
  node_I = {&node_B,  &node_J};
  node_J = {&node_I};
  node_A_= {&node_G};
  node_C_= {&node_B,  &node_G_};
  node_D_= {&node_F,  &node_F_, &node_I_};
  node_E_= {&node_H};
  node_F_= {&node_C,  &node_D_, &node_G_};
  node_G_= {&node_C_, &node_D,  &node_F_};
  node_H_= {&node_D};
  node_I_= {&node_C,  &node_D_};
  // Place nodes in an array for loops.
  graph_node<char>* nodes[18] = {&node_A, &node_B, &node_C, &node_D, &node_E,
                                 &node_F, &node_G, &node_H, &node_I, &node_J,
                                 &node_A_, &node_C_, &node_D_, &node_E_,
                                 &node_F_, &node_G_, &node_H_, &node_I_};

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "TESTS ON DIRECTED CONNECTED GRAPH" << std::endl << std::endl;

  std::cout << "       E*    A*           " << std::endl;
  std::cout << "       |     |            " << std::endl;
  std::cout << "       |     |            " << std::endl;
  std::cout << "       v     v            " << std::endl;
  std::cout << "       H---->+-------->+  " << std::endl;
  std::cout << "       |     G<---E<---F  " << std::endl;
  std::cout << "       |               ^  " << std::endl;
  std::cout << "       |               |  " << std::endl;
  std::cout << "       |               |  " << std::endl;
  std::cout << "       |               v  " << std::endl;
  std::cout << "       |     I*<-------D* " << std::endl;
  std::cout << "       |     |         ^  " << std::endl;
  std::cout << "       |     |         |  " << std::endl;
  std::cout << "       v     v         |  " << std::endl;
  std::cout << "       A     C-------->F* " << std::endl;
  std::cout << "       |     |         ^  " << std::endl;
  std::cout << "       |     |         |  " << std::endl;
  std::cout << "       v     v         |  " << std::endl;
  std::cout << " I---->B<----D--->H*   |  " << std::endl;
  std::cout << " ^     |               |  " << std::endl;
  std::cout << " |     |               |  " << std::endl;
  std::cout << " |     v               |  " << std::endl;
  std::cout << " J     C*------------->G* " << std::endl << std::endl;

  // Remove edges to break adjacency symmetry and createa the directed graph
  // Use a mixture of both access operators to cover a wide variety of cases
  node_B[0] = nullptr; node_B[3] = nullptr;
  node_C[1] = nullptr;
  node_D[2] = nullptr;
  // The i-th neighbor operator can "pop" edges this way
  for (unsigned i = 0; i < 6; ++i)
    node_E(0) = nullptr;
  // Remove more edges using a mixture of accessor operators to test them
  node_F[2] = nullptr;
  node_G[1] = nullptr;
  node_C_(0) = nullptr;
  node_D_[1] = nullptr;
  node_F_[0] = nullptr; node_F_[2] = nullptr;
  node_G_(0) = nullptr; node_G_(0) = nullptr;
  node_I_[1] = nullptr;
  node_A(1) = nullptr;
  // Change an edge to make the graph a little more interesting
  node_H[0] = &node_A;
  // Remove more to make parts of the graph inaccessible from certain nodes
  node_D(1) = nullptr; node_D(1) = nullptr;
  node_G(0) = nullptr; node_G(1) = nullptr;
  // The i-th neighbor operator can access the last neighbor this way
  node_C(node_C.degree()-1) = nullptr;
  node_B(node_B.degree()-1) = nullptr;
  node_B(node_B.degree()-1) = nullptr;
  node_I(node_I.degree()-1) = nullptr;
  node_H_(0) = nullptr;
  // Set up new search pairs for the directed graph
  graph_node<char>* pairs[18][2] = {{&node_A, &node_F},   {&node_B, &node_I},
                                    {&node_C, &node_A},   {&node_D, &node_G},
                                    {&node_E, &node_D},   {&node_F, &node_C},
                                    {&node_G, &node_H},   {&node_H, &node_E},
                                    {&node_I, &node_F},   {&node_J, &node_H},
                                    {&node_A_, &node_E},  {&node_C_, &node_B},
                                    {&node_D_, &node_F_}, {&node_E_, &node_A},
                                    {&node_F_, &node_C},  {&node_G_, &node_C_},
                                    {&node_H_, &node_D},  {&node_I_, &node_G}};

  size_t gtcaps3[18] = {2, 5, 4, 5, 7, 3, 4, 2, 2, 1, 1, 2, 3, 1, 3, 3, 1, 2};
  size_t gtdegs3[18] = {1, 1, 2, 2, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 0, 1};

  dyn_array<dyn_array<char> > adjgt;
  // Build the directed adjacency list ground truth
  adjgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str1[18] = {"B",  "C",  "DF", "BH", "G", "DE",
                           "F",  "AG", "B",  "I",  "G", "G",
                           "FI", "H",  "D",  "F",  "",  "C"};
  for (size_t i = 0; i < adjgt.size(); ++i)
    for (size_t s = 0; s < str1[i].size(); ++s)
      adjgt[i].add(str1[i][s]);

  dyn_array<size_t> caps, degs;
  dyn_array<dyn_array<char> > adjres;
  std::cout << "Node adjacency lists:" << std::endl;
  for (size_t i = 0; i < 18; ++i) {
    std::cout << *(nodes[i]) << std::endl;
    caps.add(nodes[i]->capacity());
    degs.add(nodes[i]->degree());
    adjres.add(dyn_array<char>());
    for (size_t n = 0; n < nodes[i]->degree(); ++n)
      adjres[i].add((*nodes[i])(n)->val());
  }
  std::cout << "Node capacities: ";
  for (size_t i = 0; i < 18; ++i)
    std::cout << nodes[i]->val() << ":" << nodes[i]->capacity() << " ";
  std::cout << std::endl << "Node degrees:    ";
  for (size_t i = 0; i < 18; ++i)
    std::cout << nodes[i]->val() << ":" << nodes[i]->degree() << " ";
  std::cout << std::endl << std::endl;

  std::cout << "Testing directed edge setup:" << std::endl;
  TEST_SEQ(caps, gtcaps3, 18, "Node capacities should be unchanged");
  TEST_SEQ(degs, gtdegs3, 18, "Node degrees should match new neighbor count");

  std::cout << std::endl;
  for (size_t i = 0; i < 18; ++i) {
    std::cout << "Testing node " << nodes[i]->val();
    std::cout << ((i < 10) ? ": " : "*: ") << std::endl;
    TEST_EQUAL(adjres[i].size(), adjgt[i].size(), "Adj sizes should match");
    TEST_SEQ(adjres[i], adjgt[i], adjgt[i].size(), "Adj lists should match");
  }
  std::cout << std::endl << std::endl;

  // Test traversal and search methods

  dyn_array<dyn_array<char> > dfgt;
  // Build the ground truth for depth-first traversal paths
  dfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str2[18] = {"ABCGFDFEGICDH", "BCGFDFEGICDH",
                          "CDBCGFDFEGIH",  "DBCGFDFEGICH",
                          "EGFDICDBCGFH",  "FDICDBCGFHEG",
                          "GFDICDBCGFHE",  "HABCGFDFEGICDH",
                          "IBCGFDFEGICDH", "JIBCGFDFEGICDH",
                          "AGFDICDBCGFHE", "CGFDFEGICDBH",
                          "DFEGICDBCGFH",  "EHABCGFDFEGICDH",
                          "FDFEGICDBCGH",  "GFDFEGICDBCH",
                          "H",             "ICDBCGFDFEGH"};
  for (size_t i = 0; i < dfgt.size(); ++i)
    for (size_t s = 0; s < str2[i].size(); ++s)
      dfgt[i].add(str2[i][s]);

  std::cout << "Depth-first traversal from each node:";
  for (size_t i = 0; i < 18; ++i) {
    const char* out = (i < 10) ? " | Full: " : "*| Full: ";
    std::cout << std::endl << "From " << nodes[i]->val() << out;
    gn_ptrs<char> res = nodes[i]->depth_first_traverse(&std::cout);
    print_path(res);
    TEST_EQUAL(res.size(), dfgt[i].size(), "Size should match ground truth");
    TEST_PTR_SEQ(res, dfgt[i], res.size(), "Path should match ground truth");
  }
  std::cout << std::endl << std::endl;

  dyn_array<dyn_array<char> > bfgt;
  // Build the ground truth for breadth-first traversal paths
  bfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str3[18] = {"ABCGFDFIECGDH", "BCGFDFIECGDH",
                          "CDFBHDCFIGEG",  "DBHCGFDFIECG",
                          "EGFDICDFBHCG",  "FDEIGCDFBHCG",
                          "GFDEICDFBHCG",  "HAGBFCDEGIFCDH",
                          "IBCGFDFIECGDH", "JIBCGFDFIECGDH",
                          "AGFDEICDFBHCG", "CGFDFIECGDBH",
                          "DFIECGDFBHCG",  "EHAGBFCDEGIFCDH",
                          "FDFIECGDBHCG",  "GFDFIECGDBHC",
                          "H",             "ICDFBHDCFGEG"};
  for (size_t i = 0; i < bfgt.size(); ++i)
    for (size_t s = 0; s < str3[i].size(); ++s)
      bfgt[i].add(str3[i][s]);
  std::cout << "Breadth-first traversal from each node:";
  for (size_t i = 0; i < 18; ++i) {
    const char* out = (i < 10) ? " | Full: " : "*| Full: ";
    std::cout << std::endl << "From " << nodes[i]->val() << out;
    gn_ptrs<char> res = nodes[i]->breadth_first_traverse(&std::cout);
    print_path(res);
    TEST_EQUAL(res.size(), bfgt[i].size(), "Size should match ground truth");
    TEST_PTR_SEQ(res, bfgt[i], res.size(), "Path should match ground truth");
  }
  std::cout << std::endl << std::endl;

  dfgt.clear();
  // Build the ground truth for depth-first search paths
  dfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str4[18] = {"ABCGFDF",        "BCGFDFEGICDH", "CDBCGFDFEGIH",
                          "DBCGFDFEG",      "EGFDICD",      "FDIC",
                          "GFDICDBCGFHE",   "HABCGFDFE",    "IBCGFDF",
                          "JIBCGFDFEGICDH", "AGFE",         "CGFDICDB",
                          "DICDBCGF",       "EHA",          "FDIC",
                          "GFDICDBC",       "H",            "ICDBCGFDFEG"};
  for (size_t i = 0; i < dfgt.size(); ++i)
    for (size_t s = 0; s < str4[i].size(); ++s)
      dfgt[i].add(str4[i][s]);
  dyn_array<dyn_array<char> > valgt(dyn_array<char>(), 18);
  std::string str5[18] = {"ABCGF",       "BCGFDI",    "CDBCGFDFEGIH",
                          "DBCG",        "EGFD",      "FDIC",
                          "GFDICDH",     "HABCGFDFE", "IBCGF",
                          "JIBCGFDICDH", "AGFE",      "CGFDICDB",
                          "DF",          "EHA",       "FDIC",
                          "GFDIC",       "H",         "ICDBCG"};
  for (size_t i = 0; i < valgt.size(); ++i)
    for (size_t s = 0; s < str5[i].size(); ++s)
      valgt[i].add(str5[i][s]);
  std::cout << "Depth-first search by ptr, then by value from each node:";
  for (size_t i = 0; i < 18; ++i) {
    const char* out = (i < 10) ? " | Full: " : "*| Full: ";
    const char* arr = (i < 10) ? " -> " : "*-> ";
    graph_node<char>* n1 = pairs[i][0];
    graph_node<char>* n2 = pairs[i][1];
    std::cout << std::endl << n1->val() << arr << n2->val() << out;
    gn_ptrs<char> ptr = n1->depth_first_search(n2, &std::cout);
    print_path(ptr);
    std::cout << "ByValue| Full: ";
    gn_ptrs<char> val = n1->depth_first_search(n2->val(), &std::cout);
    print_path(val);
    TEST_EQUAL(ptr.size(), dfgt[i].size(), "Ptr search size should match");
    TEST_PTR_SEQ(ptr, dfgt[i], ptr.size(), "Ptr search path should match");
    TEST_EQUAL(val.size(), valgt[i].size(), "Val search size should match");
    TEST_PTR_SEQ(val, valgt[i], val.size(), "Val search path should match");
  }
  std::cout << std::endl << std::endl;

  bfgt.clear();
  valgt.clear();
  // Build the ground truth for breadth-first search paths
  bfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str6[18] = {"ABCGFDF",        "BCGFDFIECGDH", "CDFBHDCFIGEG",
                          "DBCGFDFEG",      "EGFDICD",      "FDIC",
                          "GFDEICDFBHCG",   "HGFE",         "IBCGFDF",
                          "JIBCGFDFIECGDH", "AGFE",         "CGFDICDB",
                          "DICF",           "EHA",          "FDIC",
                          "GFDICDBC",       "H",            "ICFDFEG"};
  for (size_t i = 0; i < bfgt.size(); ++i)
    for (size_t s = 0; s < str6[i].size(); ++s)
      bfgt[i].add(str6[i][s]);
  valgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str7[18] = {"ABCGF",       "BCGFDI", "CDFBHDCFIGEG",
                          "DBCG",        "EGFD",   "FDIC",
                          "GFDICDH",     "HGFE",   "IBCGF",
                          "JIBCGFDICDH", "AGFE",   "CGFDICDB",
                          "DF",          "EHA",    "FDIC",
                          "GFDIC",       "H",      "ICDBCG"};
  for (size_t i = 0; i < valgt.size(); ++i)
    for (size_t s = 0; s < str7[i].size(); ++s)
      valgt[i].add(str7[i][s]);
  std::cout << "Breadth-first search by ptr, then by value from each node:";
  for (size_t i = 0; i < 18; ++i) {
    const char* out = (i < 10) ? " | Full: " : "*| Full: ";
    const char* arr = (i < 10) ? " -> " : "*-> ";
    graph_node<char>* n1 = pairs[i][0];
    graph_node<char>* n2 = pairs[i][1];
    std::cout << std::endl << n1->val() << arr << n2->val() << out;
    gn_ptrs<char> ptr = n1->breadth_first_search(n2, &std::cout);
    print_path(ptr);
    std::cout << "ByValue| Full: ";
    gn_ptrs<char> val = n1->breadth_first_search(n2->val(), &std::cout);
    print_path(val);
    TEST_EQUAL(ptr.size(), bfgt[i].size(), "Ptr search size should match");
    TEST_PTR_SEQ(ptr, bfgt[i], ptr.size(), "Ptr search path should match");
    TEST_EQUAL(val.size(), valgt[i].size(), "Val search size should match");
    TEST_PTR_SEQ(val, valgt[i], val.size(), "Val search path should match");
  }
  std::cout << std::endl;

  std::cout << "Looking for a cycle starting from node E*: ";
  std::cout << ((node_E_.has_cycle()) ? "Cycle detected" : "Cycle not found");
  std::cout << std::endl;
  TEST_CHECK(node_E_.has_cycle(), "Cycle should be detected, there are many");
  std::cout << std::endl;

  test_util::print_summary();
  return 0;
}