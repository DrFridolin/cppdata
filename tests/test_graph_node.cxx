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
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "UNDIRECTED CONNECTED GRAPH WITH UNIQUE VALUES\n" << std::endl;
  std::cout << " H-----G-----F     " << std::endl;
  std::cout << "        \\   /     " << std::endl;
  std::cout << "         \\^/      " << std::endl;
  std::cout << "    A-----E-----C  " << std::endl;
  std::cout << "     \\   / \\   / " << std::endl;
  std::cout << "      \\ /   \\ /  " << std::endl;
  std::cout << " I-----B-----D     " << std::endl;
  std::cout << " |                 " << std::endl;
  std::cout << " |                 " << std::endl;
  std::cout << " J                 " << std::endl << std::endl;

  // Instantiate all the nodes
  graph_node<char> node_A('A', 2), node_B('B', 5), node_C('C', 4);
  graph_node<char> node_D('D', 5), node_E('E', 7), node_F('F', 3);
  graph_node<char> node_G('G', 4), node_H('H', 2), node_I('I', 2);
  graph_node<char> node_J('J', 1);
  graph_node<char>* nodes[18] = {&node_A, &node_B, &node_C, &node_D, &node_E,
                                 &node_F, &node_G, &node_H, &node_I, &node_J};
  // Set up search pair inputs for the current graph
  graph_node<char>* df_pairs[10][2] = {{&node_A, &node_F}, {&node_B, &node_I},
                                       {&node_C, &node_A}, {&node_D, &node_G},
                                       {&node_E, &node_D}, {&node_F, &node_C},
                                       {&node_G, &node_H}, {&node_H, &node_E},
                                       {&node_I, &node_F}, {&node_J, &node_H}};
  graph_node<char>* bf_pairs[10][2] = {{&node_A, &node_C}, {&node_B, &node_H},
                                       {&node_C, &node_F}, {&node_D, &node_G},
                                       {&node_E, &node_E}, {&node_F, &node_H},
                                       {&node_G, &node_I}, {&node_H, &node_D},
                                       {&node_I, &node_F}, {&node_J, &node_H}};
  dyn_array<graph_node<char>*> ptrs;
  dyn_array<size_t> caps, degs;
  std::cout << "Node capacities: ";
  for (size_t i = 0; i < 10; ++i) {
    std::cout << nodes[i]->val() << ":" << nodes[i]->capacity() << " ";
    ptrs.add(nodes[i]);
    caps.add(nodes[i]->capacity());
  }
  std::cout << std::endl << "Node degrees:    ";
  for (size_t i = 0; i < 10; ++i) {
    std::cout << nodes[i]->val() << ":" << nodes[i]->degree() << " ";
    degs.add(nodes[i]->degree());
  }
  std::cout << std::endl << std::endl;

  char gtvals1[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
  size_t gtcaps1[10] = {2, 5, 4, 5, 7, 3, 4, 2, 2, 1};
  size_t gtdegs1[10] = {};
  std::cout << "Testing initial node setup:" << std::endl;
  TEST_PTR_SEQ(ptrs, gtvals1, 10, "Node vals should match node order in array");
  TEST_SEQ(caps, gtcaps1, 10, "Node capacities should match constructor args");
  TEST_SEQ(degs, gtdegs1, 10, "Node degrees should all be zero");
  std::cout << std::endl;

  // Array holding adjacent node address arrays for each node
  // and placeholder nullptr for edges that will be added later
  graph_node<char>* A_adj[2] = {&node_B, &node_E};
  graph_node<char>* B_adj[5] = {&node_A, nullptr, &node_D, &node_E, &node_I};
  graph_node<char>* C_adj[4] = {&node_D, &node_E, nullptr, nullptr};
  graph_node<char>* D_adj[5] = {&node_B, &node_C, &node_E, nullptr, nullptr};
  graph_node<char>* E_adj[7] = {&node_A, &node_B, &node_C, &node_D, &node_E,
                                &node_F, &node_G};
  graph_node<char>* F_adj[3] = {nullptr, &node_E, &node_G};
  graph_node<char>* G_adj[4] = {nullptr, &node_E, &node_F, &node_H};
  graph_node<char>* H_adj[2] = {nullptr, &node_G};
  graph_node<char>* I_adj[2] = {&node_B, &node_J};
  graph_node<char>* J_adj[1] = {&node_I};
  graph_node<char>** adj1[10] = {A_adj, B_adj, C_adj, D_adj, E_adj,
                                 F_adj, G_adj, H_adj, I_adj, J_adj};

  // Construct the graph edges between stack-allocated graph nodes
  for (size_t i = 0; i < 10; ++i)
    for (size_t n = 0; n < nodes[i]->capacity(); ++n)
      (*nodes[i])[n] = adj1[i][n];

  dyn_array<dyn_array<char> > adjgt, adjres;
  // Build the adjacency list ground truth
  adjgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 10);
  std::string str1[10] = {"BE", "ADEI", "DE", "BCE", "ABCDEFG",
                          "EG", "EFH",  "G",  "BJ",  "I"};
  for (size_t i = 0; i < adjgt.size(); ++i)
    for (size_t s = 0; s < str1[i].size(); ++s)
      adjgt[i].add(str1[i][s]);

  caps.clear();
  degs.clear();
  std::cout << "Node adjacency lists:" << std::endl;
  for (size_t i = 0; i < 10; ++i) {
    std::cout << *(nodes[i]) << std::endl;
    caps.add(nodes[i]->capacity());
    degs.add(nodes[i]->degree());
    adjres.add(dyn_array<char>());
    for (size_t n = 0; n < nodes[i]->degree(); ++n)
      adjres[i].add((*nodes[i])(n)->val());
  }
  std::cout << "Node capacities: ";
  for (size_t i = 0; i < 10; ++i)
    std::cout << nodes[i]->val() << ":" << nodes[i]->capacity() << " ";
  std::cout << std::endl << "Node degrees:    ";
  for (size_t i = 0; i < 10; ++i)
    std::cout << nodes[i]->val() << ":" << nodes[i]->degree() << " ";
  std::cout << std::endl << std::endl;

  size_t gtdegs2[10] = {2, 4, 2, 3, 7, 2, 3, 1, 2, 1};
  std::cout << "Testing connected node setup:" << std::endl;
  TEST_SEQ(caps, gtcaps1, 10, "Node capacities should be unchanged");
  TEST_SEQ(degs, gtdegs2, 10, "Node degrees should match neighbor count");
  std::cout << std::endl;
  for (size_t i = 0; i < 10; ++i) {
    std::cout << "Testing node " << nodes[i]->val() << ":" << std::endl;
    TEST_EQUAL(adjres[i].size(), adjgt[i].size(), "Adj sizes should match");
    TEST_SEQ(adjres[i], adjgt[i], adjgt[i].size(), "Adj lists should match");
  }
  std::cout << std::endl << std::endl;

  // Test traversal and search methods.
  // No need to separately test search by value since node values are unique

  // Build the ground truth for depth-first traversal paths
  dyn_array<dyn_array<char> > dfgt(dyn_array<char>(), 10);
  std::string str2[10] = {"ABDCEFGHIJ", "BAECDFGHIJ", "CDBAEFGHIJ",
                          "DBAECFGHIJ", "EABDCIJFGH", "FEABDCIJGH",
                          "GEABDCIJFH", "HGEABDCIJF", "IBAECDFGHJ",
                          "JIBAECDFGH"};
  for (size_t i = 0; i < dfgt.size(); ++i)
    for (size_t s = 0; s < str2[i].size(); ++s)
      dfgt[i].add(str2[i][s]);
  std::cout << "Depth-first traversal from each node:";
  for (size_t i = 0; i < 10; ++i) {
    std::cout << std::endl << "From " << nodes[i]->val() << " | Full: ";
    gn_ptrs<char> res = nodes[i]->depth_first_traverse(&std::cout);
    print_path(res);
    TEST_EQUAL(res.size(), dfgt[i].size(), "Size should match ground truth");
    TEST_PTR_SEQ(res, dfgt[i], res.size(), "Path should match ground truth");
  }
  std::cout << std::endl << std::endl;

  // Build the ground truth for breadth-first traversal paths
  dyn_array<dyn_array<char> > bfgt(dyn_array<char>(), 10);
  std::string str3[10] = {"ABEDICFGJH", "BADEICFGJH", "CDEBAFGIHJ",
                          "DBCEAIFGJH", "EABCDFGIHJ", "FEGABCDHIJ",
                          "GEFHABCDIJ", "HGEFABCDIJ", "IBJADECFGH",
                          "JIBADECFGH"};
  for (size_t i = 0; i < bfgt.size(); ++i)
    for (size_t s = 0; s < str3[i].size(); ++s)
      bfgt[i].add(str3[i][s]);
  std::cout << "Breadth-first traversal from each node:";
  for (size_t i = 0; i < 10; ++i) {
    std::cout << std::endl << "From " << nodes[i]->val() << " | Full: ";
    gn_ptrs<char> res = nodes[i]->breadth_first_traverse(&std::cout);
    print_path(res);
    std::cout << res.size() << " " << bfgt[i].size() << std::endl;
    TEST_EQUAL(res.size(), bfgt[i].size(), "Size should match ground truth");
    TEST_PTR_SEQ(res, bfgt[i], res.size(), "Path should match ground truth");
  }
  std::cout << std::endl << std::endl;

  dfgt.clear();
  // Build the ground truth for depth-first search paths
  dfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 10);
  std::string str4[10] = {"ABDCEF", "BI", "CDBA", "DBAEFG", "EABD",
                          "FEABDC", "GH", "HGE",  "IBAEF",  "JIBAEFGH"};
  for (size_t i = 0; i < dfgt.size(); ++i)
    for (size_t s = 0; s < str4[i].size(); ++s)
      dfgt[i].add(str4[i][s]);
  std::cout << "Depth-first search by pointer from each node:";
  for (size_t i = 0; i < 10; ++i) {
    graph_node<char>* n1 = df_pairs[i][0];
    graph_node<char>* n2 = df_pairs[i][1];
    std::cout << std::endl << n1->val() << " -> " << n2->val() << " | Full: ";
    gn_ptrs<char> res = n1->depth_first_search(n2, &std::cout);
    print_path(res);
    TEST_EQUAL(res.size(), dfgt[i].size(), "Size should match ground truth");
    TEST_PTR_SEQ(res, dfgt[i], res.size(), "Path should match ground truth");
  }
  std::cout << std::endl << std::endl;

  bfgt.clear();
  // Build the ground truth for breadth-first and bidirectional search paths
  bfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 10);
  std::string str5[10] = {"AEC", "BEGH", "CEF",  "DEG",  "E",
                          "FGH", "GEBI", "HGED", "IBEF", "JIBEGH"};
  for (size_t i = 0; i < bfgt.size(); ++i)
    for (size_t s = 0; s < str5[i].size(); ++s)
      bfgt[i].add(str5[i][s]);
  std::cout << "Breadth-first search, then bidirectional search by ptr:";
  for (size_t i = 0; i < 10; ++i) {
    graph_node<char>* n1 = bf_pairs[i][0];
    graph_node<char>* n2 = bf_pairs[i][1];
    std::cout << std::endl << n1->val() << " -> " << n2->val() << " | Full: ";
    gn_ptrs<char> bf = n1->breadth_first_search(n2, &std::cout);
    print_path(bf);
    std::cout << "BiDirec| Full: ";
    gn_ptrs<char> bd = n1->bidirectional_search(n2, &std::cout);
    print_path(bd);
    TEST_EQUAL(bf.size(), bfgt[i].size(), "BF size should match ground truth");
    TEST_PTR_SEQ(bf, bfgt[i], bf.size(), "BF path should match ground truth");
    TEST_EQUAL(bd.size(), bfgt[i].size(), "BD size should match ground truth");
    TEST_PTR_SEQ(bd, bfgt[i], bd.size(), "BD path should match ground truth");
  }
  std::cout << std::endl;

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "UNDIRECTED CONNECTED GRAPH WITH DUPLICATE VALUES" << std::endl;
  std::cout << "Added nodes shown with *" << std::endl << std::endl;
  std::cout << " E*    A*       D*-----+  " << std::endl;
  std::cout << " |     |       / \\     | " << std::endl;
  std::cout << " |     |      /   \\    | " << std::endl;
  std::cout << " H-----G-----F     I*  |  " << std::endl;
  std::cout << "        \\   /     /    | " << std::endl;
  std::cout << "         \\^/     /     | " << std::endl;
  std::cout << "    A-----E-----C------F* " << std::endl;
  std::cout << "     \\   / \\   /       |" << std::endl;
  std::cout << "      \\ /   \\ /        |" << std::endl;
  std::cout << " I-----B-----D-----H*  |  " << std::endl;
  std::cout << " |     |     |         |  " << std::endl;
  std::cout << " |     |     |         |  " << std::endl;
  std::cout << " J     C*----G*--------+  " << std::endl << std::endl;

  // Create the new nodes
  graph_node<char> node_A_('A', 1), node_C_('C', 2), node_D_('D', 3);
  graph_node<char> node_E_('E', 1), node_F_('F', 3), node_G_('G', 3);
  graph_node<char> node_H_('H', 1), node_I_('I', 2);
  nodes[10] = &node_A_;
  nodes[11] = &node_C_;
  nodes[12] = &node_D_;
  nodes[13] = &node_E_;
  nodes[14] = &node_F_;
  nodes[15] = &node_G_;
  nodes[16] = &node_H_;
  nodes[17] = &node_I_;
  // Add the new edges, preserving alphabetic order
  node_B[1] = &node_C_;
  node_C[2] = &node_F_; node_C[3] = &node_I_;
  node_D[3] = &node_G_; node_D[4] = &node_H_;
  node_F[0] = &node_D_;
  node_G[0] = &node_A_;
  node_H[0] = &node_E_;
  node_A_[0] = &node_G;
  node_C_[0] = &node_B; node_C_[1] = &node_G_;
  node_D_[0] = &node_F; node_D_[1] = &node_F_; node_D_[2] = &node_I_;
  node_E_[0] = &node_H;
  node_F_[0] = &node_C; node_F_[1] = &node_D_; node_F_[2] = &node_G_;
  node_G_[0] = &node_C_; node_G_[1] = &node_D; node_G_[2] = &node_F_;
  node_H_[0] = &node_D;
  node_I_[0] = &node_C; node_I_[1] = &node_D_;

  char gtvals2[18] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                      'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
  size_t gtcaps2[18] = {2, 5, 4, 5, 7, 3, 4, 2, 2, 1, 1, 2, 3, 1, 3, 3, 1, 2};
  std::cout << "Testing expanded node setup:" << std::endl;
  degs.clear();
  adjres.clear();
  for (size_t i = 0; i < 18; ++i) {
    if (i > 9) ptrs.add(nodes[i]);
    if (i > 9) caps.add(nodes[i]->capacity());
    degs.add(nodes[i]->degree());
    adjres.add(dyn_array<char>());
    for (size_t n = 0; n < nodes[i]->degree(); ++n)
      adjres[i].add((*nodes[i])(n)->val());
  }
  TEST_PTR_SEQ(ptrs, gtvals2, 18, "Node vals should match order in array");
  TEST_SEQ(caps, gtcaps2, 18, "Node capacities should match constructor args");
  TEST_SEQ(degs, gtcaps2, 18, "Node degrees should match number of neighbors");
  std::cout << std::endl;

  std::cout << "Node adjacency lists:" << std::endl;
  for (size_t i = 0; i < 18; ++i)
    std::cout << *(nodes[i]) << std::endl;
  std::cout << "Node capacities: ";
  for (size_t i = 0; i < 18; ++i)
    std::cout << nodes[i]->val() << ":" << nodes[i]->capacity() << " ";
  std::cout << std::endl << "Node degrees:    ";
  for (size_t i = 0; i < 18; ++i)
    std::cout << nodes[i]->val() << ":" << nodes[i]->degree() << " ";
  std::cout << std::endl << std::endl;

  adjgt.clear();
  // Build the adjacency list ground truth
  adjgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str6[18] = {"BE",   "ACDEI", "DEFI", "BCEGH", "ABCDEFG", "DEG",
                          "AEFH", "EG",    "BJ",   "I",     "G",       "BG",
                          "FFI",  "H",     "CDG",  "CDF",   "D",       "CD"};
  for (size_t i = 0; i < adjgt.size(); ++i)
    for (size_t s = 0; s < str6[i].size(); ++s)
      adjgt[i].add(str6[i][s]);

  for (size_t i = 0; i < 18; ++i) {
    std::cout << "Testing node " << nodes[i]->val();
    std::cout << ((i < 10) ? ": " : "*: ") << std::endl;
    TEST_EQUAL(adjres[i].size(), adjgt[i].size(), "Adj sizes should match");
    TEST_SEQ(adjres[i], adjgt[i], adjgt[i].size(), "Adj lists should match");
  }
  std::cout << std::endl << std::endl;

  // Test traversal and search by value methods

  dfgt.clear();
  // Build the ground truth for depth-first traversal paths
  dfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str7[18] = {"ABCGDCEFDFIGAHEHIJ", "BAECDGCFDFGAHEIHIJ",
                          "CDBAEFDFGCIGAHEIJH", "DBAECFDFGAHEIGCIJH",
                          "EABCGDCFDFGAHEIHIJ", "FDFCDBAEGAHECGIJHI",
                          "GAEABCGDCFDFIHIJHE", "HEGAEABCGDCFDFIHIJ",
                          "IBAECDGCFDFGAHEIHJ", "JIBAECDGCFDFGAHEIH",
                          "AGEABCGDCFDFIHIJHE", "CBAECDGFDFGAHEIHIJ",
                          "DFEABCGDCFIHIJGAHE", "EHGAEABCGDCFDFIHIJ",
                          "FCDBAEFDIGAHECGIJH", "GCBAECDHFDFGAHEIIJ",
                          "HDBAECFDFGAHEIGCIJ", "ICDBAEFDFGCGAHEIJH"};
  for (size_t i = 0; i < dfgt.size(); ++i)
    for (size_t s = 0; s < str7[i].size(); ++s)
      dfgt[i].add(str7[i][s]);

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

  bfgt.clear();
  // Build the ground truth for breadth-first traversal paths
  bfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 18);
  std::string str8[18] = {"ABECDICFGGHJFIDAHE", "BACDEIGCHFGJFIDAHE",
                          "CDEFIBGHAFGDCIAHJE", "DBCEGHACIFIFGJDAHE",
                          "EABCDFGCIFIGHDAHJE", "FDEGFIABCDAHGCIHEJ",
                          "GAEFHABCDDECIFIGHJ", "HEGAEFABCDDCIFIGHJ",
                          "IBJACDEGCHFGFIDAHE", "JIBACDEGCHFGFIDAHE",
                          "AGEFHABCDDECIFIGHJ", "CBGADEIFCHFGJDIAHE",
                          "DFFIEGCGABDAHCIHEJ", "EHGAEFABCDDCIFIGHJ",
                          "FCDGDEIFCBHAGIAHJE", "GCDFBCEHDAIIFGJAHE",
                          "HDBCEGACIFIFGJDAHE", "ICDDEFFBGHAGCIAHJE"};
  for (size_t i = 0; i < bfgt.size(); ++i)
    for (size_t s = 0; s < str8[i].size(); ++s)
      bfgt[i].add(str8[i][s]);
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
  dfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 10);
  std::string str9[10] = {"ABCGDCEF", "BAECDGFDI", "CDBA",     "DBAECFDFG",
                          "EABCGD",   "FDFC",      "GEABCGDH", "HE",
                          "IBAECDGF", "JIBAECDGFDFGH"};
  for (size_t i = 0; i < dfgt.size(); ++i)
    for (size_t s = 0; s < str9[i].size(); ++s)
      dfgt[i].add(str9[i][s]);
  std::cout << "Depth-first search by value from each of the old nodes:";
  for (size_t i = 0; i < 10; ++i) {
    graph_node<char>* n1 = df_pairs[i][0];
    graph_node<char>* n2 = df_pairs[i][1];
    std::cout << std::endl << n1->val() << " -> " << n2->val() << " | Full: ";
    gn_ptrs<char> res = n1->depth_first_search(n2->val(), &std::cout);
    print_path(res);
    TEST_EQUAL(res.size(), dfgt[i].size(), "Size should match ground truth");
    TEST_PTR_SEQ(res, dfgt[i], res.size(), "Path should match ground truth");
  }
  std::cout << std::endl << std::endl;

  bfgt.clear();
  // Build the ground truth for breadth-first search paths
  bfgt = dyn_array<dyn_array<char> >(dyn_array<char>(), 10);
  std::string str10[10] = {"ABC", "BDH",  "CF",   "DG",   "E",
                           "FGH", "GEBI", "HGED", "IBEF", "JIBDH"};
  for (size_t i = 0; i < bfgt.size(); ++i)
    for (size_t s = 0; s < str10[i].size(); ++s)
      bfgt[i].add(str10[i][s]);
  std::cout << "Breadth-first search by value from each of the old nodes:";
  for (size_t i = 0; i < 10; ++i) {
    graph_node<char>* n1 = bf_pairs[i][0];
    graph_node<char>* n2 = bf_pairs[i][1];
    std::cout << std::endl << n1->val() << " -> " << n2->val() << " | Full: ";
    gn_ptrs<char> res = n1->breadth_first_search(n2->val(), &std::cout);
    print_path(res);
    TEST_EQUAL(res.size(), bfgt[i].size(), "Size should match ground truth");
    TEST_PTR_SEQ(res, bfgt[i], res.size(), "Path should match ground truth");
  }
  std::cout << std::endl;

  test_util::print_summary();
  return 0;
}