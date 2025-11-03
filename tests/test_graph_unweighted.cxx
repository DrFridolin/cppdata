#include <iostream>
#include <string>
#include "graph.h"
#include "dyn_array.h"
#include "test_util.h"

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "SMALL GRAPH TESTS" << std::endl << std::endl;

  bool check;
  dyn_array<char> vals;
  dyn_array<size_t> indices;
  dyn_array<int> labels;

  graph_node<char> node_a('a', 0);
  graph_node<char> node_b('b', 0);
  graph_node<char> node_c('c', 0);
  graph_node<char> node_d('d', 0);
  graph_node<char> node_e('e', 0);
  graph_node<char> node_f('f', 0);
  node_d.add_edge(&node_a);
  node_b.add_edge(&node_f);
  node_d.add_edge(&node_b);
  node_a.add_edge(&node_f);
  node_c.add_edge(&node_d);
  std::cout << "Small connected graph node network + single unconnected node ";
  std::cout << "created with graph_node<char>." << std::endl;
  std::cout << "Adjacency lists:" << std::endl;
  std::cout << node_a << std::endl << node_b << std::endl << node_c;
  std::cout << std::endl << node_d << std::endl << node_e << std::endl;
  std::cout << node_f << std::endl;
  std::cout << "Testing edge counts:" << std::endl;
  TEST_EQUAL(node_a.degree(), 1, "There should be 1 edge from a to f");
  TEST_EQUAL(node_b.degree(), 1, "There should be 1 edge from b to f");
  TEST_EQUAL(node_c.degree(), 1, "There should be 1 edge from c to d");
  TEST_EQUAL(node_d.degree(), 2, "There should be 2 edges from d to a, b");
  TEST_EQUAL(node_e.degree(), 0, "There should be no outgoing edges from e");
  TEST_EQUAL(node_f.degree(), 0, "There should be no outgoing edges from f");

  // Ground truth for DFT
  std::string str8("cdafb");
  dyn_array<char> gt;
  for (size_t s = 0; s < str8.size(); ++s)
    gt.add(str8[s]);
  std::cout << "Depth first traversal from c: ";
  gn_ptrs<char> res1 = node_c.depth_first_traverse(&std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(res1, gt, res1.size(), "DFT path should match ground truth");

  // Ground truth for BFT
  std::string str9("cdabf");
  gt.clear();
  for (size_t s = 0; s < str9.size(); ++s)
    gt.add(str9[s]);
  std::cout << "Breadth first traversal from c: ";
  gn_ptrs<char> res2 = node_c.breadth_first_traverse(&std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(res2, gt, res2.size(), "BFT path should match ground truth");

  // Ground truth for post-order depth-first traversal
  std::string str10("fabdc");
  gt.clear();
  for (size_t s = 0; s < str10.size(); ++s)
    gt.add(str10[s]);
  std::cout << "Post-order depth first traversal from c: ";
  gn_ptrs<char> res3 = node_c.depth_first_traverse(&std::cout, true);
  std::cout << std::endl;
  TEST_PTR_SEQ(res3, gt, res3.size(), "PODFT path should match ground truth");

  // Ground truth for post-order depth-first traversal
  std::string str11("e");
  gt.clear().add(str11[0]);
  std::cout << "Post-order depth first traversal from e: ";
  gn_ptrs<char> res4 = node_e.depth_first_traverse(&std::cout, true);
  std::cout << std::endl;
  TEST_PTR_SEQ(res4, gt, res4.size(), "PODFT path should match ground truth");
  std::cout << std::endl;

  dyn_array<char> node_vals = {'a', 'b', 'c', 'd', 'e', 'f'};
  graph<char> g_small = node_vals;
  g_small.add_edge(0, 5); // a -> f
  g_small.add_edge(1, 5); // b -> f
  g_small.add_edge(2, 3); // c -> d
  g_small.add_edge(3, 0); // d -> a
  g_small.add_edge(3, 1); // d -> b
  std::cout << "Built same structure with graph<T>:" << std::endl << g_small;
  std::cout << "Testing edge counts against graph_node edges:" << std::endl;
  TEST_EQUAL(node_a.degree(), g_small[0].degree(), "Should be same as before");
  TEST_EQUAL(node_b.degree(), g_small[1].degree(), "Should be same as before");
  TEST_EQUAL(node_c.degree(), g_small[2].degree(), "Should be same as before");
  TEST_EQUAL(node_d.degree(), g_small[3].degree(), "Should be same as before");
  TEST_EQUAL(node_e.degree(), g_small[4].degree(), "Should be same as before");
  TEST_EQUAL(node_f.degree(), g_small[5].degree(), "Should be same as before");

  std::cout << "Depth first traversal from c: ";
  indices = g_small.depth_first_traverse(2, &std::cout);
  vals = g_small[indices];
  std::cout << std::endl;
  TEST_PTR_SEQ(res1, vals, vals.size(), "DFT path should be same as before");

  std::cout << "Breadth first traversal from c: ";
  indices = g_small.breadth_first_traverse(2, &std::cout);
  vals = g_small[indices];
  std::cout << std::endl;
  TEST_PTR_SEQ(res2, vals, vals.size(), "BFT path should match ground truth");

  std::string str12("fabdce");
  gt.clear();
  for (size_t s = 0; s < str12.size(); ++s)
    gt.add(str12[s]);
  std::cout << "Topologically sort the graph: ";
  vals = g_small.topological_sort();
  std::cout << vals << std::endl;
  TEST_SEQ(vals, gt, 6, "Topological sort result should be: f a b d c e");

  str12 = "cedabf";
  gt.clear();
  for (size_t s = 0; s < str12.size(); ++s)
    gt.add(str12[s]);
  std::cout << "Kahn's algorithm before creating cycle: ";
  vals = g_small[g_small.kahn()];
  std::cout << vals << std::endl;
  TEST_EQUAL(vals.size(), g_small.size(), "Kahn should detect no cycles");
  TEST_SEQ(vals, gt, 6, "Kahn result should be: c e d a b f");

  int ccgt1[6] = {0, 0, 0, 0, 1, 0};
  std::cout << "Connected component labels before creating cycle: ";
  labels = g_small.weakly_conn_comp();
  std::cout << labels << std::endl;
  TEST_SEQ(labels, ccgt1, 6, "Component labels should be: 0 0 0 0 1 0");
  std::cout << std::endl;

  g_small.add_edge(1, 4); // b -> e
  g_small.add_edge(4, 2); // e -> c
  std::cout << "Created a cycle:" << std::endl << g_small;
  TEST_CHECK(g_small.has_cycle(), "Cycle should be detected");
  check = g_small.kahn().size() != g_small.size();
  TEST_CHECK(check, "Kahn should detect cycle(s)");

  int ccgt2[6] = {0, 0, 0, 0, 0, 0};
  std::cout << "Connected component labels after creating cycle: ";
  labels = g_small.weakly_conn_comp();
  std::cout << labels << std::endl;
  TEST_SEQ(labels, ccgt2, 6, "Component labels should be: 0 0 0 0 0 0");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "MEDIUM-SIZED GRAPH TESTS" << std::endl << std::endl;

  node_a.clear_edges();
  node_b.clear_edges();
  node_c.clear_edges();
  node_d.clear_edges();
  node_e.clear_edges();
  node_f.clear_edges();
  graph_node<char> node_g('g', 0);
  graph_node<char> node_h('h', 0);
  // Add directed edges from dependendant to the dependee
  node_e.add_edge(&node_a);
  node_e.add_edge(&node_b);
  node_a.add_edge(&node_b);
  node_a.add_edge(&node_c);
  node_a.add_edge(&node_f);
  node_b.add_edge(&node_f);
  node_c.add_edge(&node_f);
  node_g.add_edge(&node_d);
  node_h.add_edge(&node_b);
  std::cout << "Medium connected graph node network + 2 connected nodes ";
  std::cout << "created with graph<char>." << std::endl;
  std::cout << "Adjacency lists:" << std::endl;
  std::cout << node_a << std::endl << node_b << std::endl << node_c;
  std::cout << std::endl << node_d << std::endl << node_e << std::endl;
  std::cout << node_f << std::endl << node_g << std::endl << node_h;
  std::cout << std::endl;
  std::cout << "Testing edge counts:" << std::endl;
  TEST_EQUAL(node_a.degree(), 3, "There should be 3 edges from a to b, c, f");
  TEST_EQUAL(node_b.degree(), 1, "There should be 1 edge from b to f");
  TEST_EQUAL(node_c.degree(), 1, "There should be 1 edge from c to d");
  TEST_EQUAL(node_d.degree(), 0, "There should be no outgoing edges from d");
  TEST_EQUAL(node_e.degree(), 2, "There should be 2 edges from e to a, b");
  TEST_EQUAL(node_f.degree(), 0, "There should be no outgoing edges from f");
  TEST_EQUAL(node_g.degree(), 1, "There should be 1 edge from g to d");
  TEST_EQUAL(node_h.degree(), 1, "There should be 1 edge from h to b");

  // Ground truth for DFT
  std::string str13("eabfc");
  gt.clear();
  for (size_t s = 0; s < str13.size(); ++s)
    gt.add(str13[s]);
  std::cout << "Depth first traversal from e: ";
  res1 = node_e.depth_first_traverse(&std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(res1, gt, res1.size(), "DFT path should match ground truth");

  // Ground truth for BFT
  std::string str14("eabcf");
  gt.clear();
  for (size_t s = 0; s < str14.size(); ++s)
    gt.add(str14[s]);
  std::cout << "Breadth first traversal from e: ";
  res2 = node_e.breadth_first_traverse(&std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(res2, gt, res2.size(), "BFT path should match ground truth");

  // Ground truth for post-order depth-first traversal
  std::string str15("fbcae");
  gt.clear();
  for (size_t s = 0; s < str15.size(); ++s)
    gt.add(str15[s]);
  std::cout << "Post-order depth first traversal from e: ";
  res3 = node_e.depth_first_traverse(&std::cout, true);
  std::cout << std::endl;
  TEST_PTR_SEQ(res3, gt, res3.size(), "PODFT path should match ground truth");

  // Ground truth for post-order depth-first traversal
  std::string str16("dg");
  gt.clear();
  for (size_t s = 0; s < str16.size(); ++s)
    gt.add(str16[s]);
  std::cout << "Post-order depth first traversal from g: ";
  res4 = node_g.depth_first_traverse(&std::cout, true);
  std::cout << std::endl;
  TEST_PTR_SEQ(res4, gt, res4.size(), "PODFT path should match ground truth");

  // Ground truth for post-order depth-first traversal
  std::string str17("fbh");
  gt.clear();
  for (size_t s = 0; s < str17.size(); ++s)
    gt.add(str17[s]);
  std::cout << "Post-order depth first traversal from h: ";
  gn_ptrs<char> res5 = node_h.depth_first_traverse(&std::cout, true);
  std::cout << std::endl;
  TEST_PTR_SEQ(res5, gt, res5.size(), "PODFT path should match ground truth");
  std::cout << std::endl;

  graph<char> g_medium = {'a', 'b', 'c', 'e', 'f'};
  g_medium.add_edge(0, 1);    // a -> b
  g_medium.add_edge(0, 2);    // a -> c
  g_medium.add_edge(0, 4);    // a -> f
  g_medium.add_edge(1, 4);    // b -> f
  g_medium.add_edge(2, 4);    // c -> f
  g_medium.add_edge(3, 0);    // e -> a
  g_medium.add_edge(3, 1);    // e -> b
  g_medium.add_node(&node_g); // g -> d
  g_medium.add_node('h');
  g_medium.add_edge(7, 1);    // h -> b
  std::cout << "Built same structure with graph<T>:" << std::endl << g_medium;
  std::cout << "Testing edge counts against graph_node edges:" << std::endl;
  TEST_EQUAL(node_a.degree(), g_medium[0].degree(), "Should be same as before");
  TEST_EQUAL(node_b.degree(), g_medium[1].degree(), "Should be same as before");
  TEST_EQUAL(node_c.degree(), g_medium[2].degree(), "Should be same as before");
  TEST_EQUAL(node_e.degree(), g_medium[3].degree(), "Should be same as before");
  TEST_EQUAL(node_f.degree(), g_medium[4].degree(), "Should be same as before");
  TEST_EQUAL(node_g.degree(), g_medium[5].degree(), "Should be same as before");
  TEST_EQUAL(node_d.degree(), g_medium[6].degree(), "Should be same as before");
  TEST_EQUAL(node_h.degree(), g_medium[7].degree(), "Should be same as before");
  TEST_EQUAL(g_medium.num_edges(), 9, "There should be a total of 9 edges");

  std::cout << "Depth first traversal from e: ";
  indices = g_medium.depth_first_traverse(3, &std::cout);
  vals = g_medium[indices];
  std::cout << std::endl;
  TEST_PTR_SEQ(res1, vals, vals.size(), "DFT path should be same as before");

  std::cout << "Breadth first traversal from e: ";
  indices = g_medium.breadth_first_traverse(3, &std::cout);
  vals = g_medium[indices];
  std::cout << std::endl;
  TEST_PTR_SEQ(res2, vals, vals.size(), "BFT path should match ground truth");

  std::string str18("fbcaedgh");
  gt.clear();
  for (size_t s = 0; s < str18.size(); ++s)
    gt.add(str18[s]);
  std::cout << "Topologically sort the graph: ";
  vals = g_medium.topological_sort();
  std::cout << vals << std::endl;
  TEST_SEQ(vals, gt, 8, "Topological sort result should be: f b c a e d g h");

  str18 = "eghadbcf";
  gt.clear();
  for (size_t s = 0; s < str18.size(); ++s)
    gt.add(str18[s]);
  std::cout << "Kahn's algorithm before creating cycle: ";
  vals = g_medium[g_medium.kahn()];
  std::cout << vals << std::endl;
  TEST_EQUAL(vals.size(), g_medium.size(), "Kahn should detect no cycles");
  TEST_SEQ(vals, gt, 8, "Kahn result should be: e g h a d b c f");

  int ccgt3[8] = {0, 0, 0, 0, 0, 1, 1, 0};
  std::cout << "Connected component labels before creating cycle: ";
  labels = g_medium.weakly_conn_comp();
  std::cout << labels << std::endl;
  TEST_SEQ(labels, ccgt3, 8, "Component labels should be: 0 0 0 0 0 1 1 0");
  std::cout << std::endl;

  g_medium.add_edge(6, 1); // d -> b
  g_medium.add_edge(4, 5); // f -> g
  std::cout << "Created a cycle:" << std::endl << g_medium;
  TEST_CHECK(g_medium.has_cycle(), "Cycle should be detected");
  check = g_medium.kahn().size() != g_medium.size();
  TEST_CHECK(check, "Kahn should detect cycle(s)");

  int ccgt4[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  std::cout << "Connected component labels after creating cycle: ";
  labels = g_medium.weakly_conn_comp();
  std::cout << labels << std::endl;
  TEST_SEQ(labels, ccgt4, 8, "Component labels should be: 0 0 0 0 0 0 0 0");

  std::cout << "Testing the accessors" << std::endl;
  TEST_EQUAL(g_medium[0].val(), 'a', "graph[0].val() should be 'a'");
  TEST_EQUAL(g_medium(0), 'a', "graph(0) should be 'a'");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "LARGE GRAPH TESTS" << std::endl << std::endl;

  // Clear all the edges and rebuild the larger graph from the solution
  node_a.clear_edges();
  node_b.clear_edges();
  node_c.clear_edges();
  node_d.clear_edges();
  node_e.clear_edges();
  node_f.clear_edges();
  node_g.clear_edges();
  // Add the extra nodes
  graph_node<char> node_x('x', 0); graph_node<char> node_y('y', 0);
  graph_node<char> node_z('z', 0); graph_node<char> node_t('t', 0);
  graph_node<char> node_m('m', 0); graph_node<char> node_n('n', 0);
  graph_node<char> node_p('p', 0);
  // Add directed edges from dependendant to the dependee
  node_c.add_edge(&node_d); node_d.add_edge(&node_a); node_d.add_edge(&node_b);
  node_a.add_edge(&node_e); node_a.add_edge(&node_f); node_t.add_edge(&node_e);
  node_e.add_edge(&node_z); node_z.add_edge(&node_b); node_z.add_edge(&node_n);
  node_b.add_edge(&node_f); node_b.add_edge(&node_y); node_n.add_edge(&node_m);
  node_y.add_edge(&node_x); node_m.add_edge(&node_p); node_m.add_edge(&node_y);
  std::cout << "Large connected graph, not fully traversable from one node ";
  std::cout << "created with graph<char>." << std::endl << "Adjacency lists:";
  std::cout << std::endl << node_a << std::endl << node_b << std::endl;
  std::cout << node_c << std::endl << node_d << std::endl << node_e;
  std::cout << std::endl << node_f << std::endl << node_x << std::endl;
  std::cout << node_y << std::endl << node_z << std::endl << node_t;
  std::cout << std::endl << node_m << std::endl << node_n << std::endl;
  std::cout << node_p << std::endl;
  std::cout << "Testing edge counts against graph_node edges:" << std::endl;
  TEST_EQUAL(node_a.degree(), 2, "There should be 2 edges from a to e, f");
  TEST_EQUAL(node_b.degree(), 2, "There should be 2 edges from b to f, y");
  TEST_EQUAL(node_c.degree(), 1, "There should be 1 edge from c to d");
  TEST_EQUAL(node_d.degree(), 2, "There should be 2 edges from d to a, b");
  TEST_EQUAL(node_e.degree(), 1, "There should be 1 edge from e to z");
  TEST_EQUAL(node_f.degree(), 0, "There should be no outgoing edges from f");
  TEST_EQUAL(node_x.degree(), 0, "There should be no outgoing edges from x");
  TEST_EQUAL(node_y.degree(), 1, "There should be 1 edge from y to x");
  TEST_EQUAL(node_z.degree(), 2, "There should be 2 edges from z to b, n");
  TEST_EQUAL(node_t.degree(), 1, "There should be 1 edge from t to e");
  TEST_EQUAL(node_m.degree(), 2, "There should be 2 edges from m to p, y");
  TEST_EQUAL(node_n.degree(), 1, "There should be 1 edge from n to m");
  TEST_EQUAL(node_p.degree(), 0, "There should be no outgoing edges from f");

  // Ground truth for DFT
  std::string str19("cdaezbfyxnmp");
  gt.clear();
  for (size_t s = 0; s < str19.size(); ++s)
    gt.add(str19[s]);
  std::cout << "Depth first traversal from c: ";
  res1 = node_c.depth_first_traverse(&std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(res1, gt, res1.size(), "DFT path should match ground truth");

  // Ground truth for BFT
  std::string str20("cdabefyzxnmp");
  gt.clear();
  for (size_t s = 0; s < str20.size(); ++s)
    gt.add(str20[s]);
  std::cout << "Breadth first traversal from c: ";
  res2 = node_c.breadth_first_traverse(&std::cout);
  std::cout << std::endl;
  TEST_PTR_SEQ(res2, gt, res2.size(), "BFT path should match ground truth");

  // Ground truth for post-order depth-first traversal
  std::string str21("fxybpmnzeadc");
  gt.clear();
  for (size_t s = 0; s < str21.size(); ++s)
    gt.add(str21[s]);
  std::cout << "Post-order depth first traversal from c: ";
  res3 = node_c.depth_first_traverse(&std::cout, true);
  std::cout << std::endl;
  TEST_PTR_SEQ(res3, gt, res3.size(), "PODFT path should match ground truth");

  // Ground truth for post-order depth-first traversal
  std::string str22("fxybpmnzet");
  gt.clear();
  for (size_t s = 0; s < str22.size(); ++s)
    gt.add(str22[s]);
  std::cout << "Post-order depth first traversal from t: ";
  res4 = node_t.depth_first_traverse(&std::cout, true);
  std::cout << std::endl;
  TEST_PTR_SEQ(res4, gt, res4.size(), "PODFT path should match ground truth");
  std::cout << std::endl;

  // Assign the entire graph except node_t with the assignment operator.
  graph<char> g_large;
  g_large = (&node_c);
  g_large.add_edge(g_large.add_node('t'), 4);
  std::cout << "Built same structure with graph<T>:" << std::endl << g_large;
  std::cout << "Testing edge counts against graph_node edges:" << std::endl;
  TEST_EQUAL(node_c.degree(), g_large[0].degree(), "Must be same as before");
  TEST_EQUAL(node_d.degree(), g_large[1].degree(), "Must be same as before");
  TEST_EQUAL(node_a.degree(), g_large[2].degree(), "Must be same as before");
  TEST_EQUAL(node_b.degree(), g_large[3].degree(), "Must be same as before");
  TEST_EQUAL(node_e.degree(), g_large[4].degree(), "Must be same as before");
  TEST_EQUAL(node_f.degree(), g_large[5].degree(), "Must be same as before");
  TEST_EQUAL(node_y.degree(), g_large[6].degree(), "Must be same as before");
  TEST_EQUAL(node_z.degree(), g_large[7].degree(), "Must be same as before");
  TEST_EQUAL(node_x.degree(), g_large[8].degree(), "Must be same as before");
  TEST_EQUAL(node_n.degree(), g_large[9].degree(), "Must be same as before");
  TEST_EQUAL(node_m.degree(), g_large[10].degree(), "Must be same as before");
  TEST_EQUAL(node_p.degree(), g_large[11].degree(), "Must be same as before");
  TEST_EQUAL(node_t.degree(), g_large[12].degree(), "Must be same as before");
  TEST_EQUAL(g_large.num_edges(), 15, "There should be a total of 15 edges");

  std::cout << "Depth first traversal from c: ";
  indices = g_large.depth_first_traverse(0, &std::cout);
  vals = g_large[indices];
  std::cout << std::endl;
  TEST_PTR_SEQ(res1, vals, vals.size(), "DFT path should be same as before");

  std::cout << "Breadth first traversal from c: ";
  indices = g_large.breadth_first_traverse(0, &std::cout);
  vals = g_large[indices];
  std::cout << std::endl;
  TEST_PTR_SEQ(res2, vals, vals.size(), "BFT path should match ground truth");

  std::string str23("fxybpmnzeadct");
  gt.clear();
  for (size_t s = 0; s < str23.size(); ++s)
    gt.add(str23[s]);
  std::cout << "Topologically sort the graph: ";
  vals = g_large.topological_sort();
  std::cout << vals << std::endl;
  TEST_SEQ(vals, gt, 13, "Sorted result should be: f x y b p m n z e a d c t");
  TEST_EQUAL(g_large.has_cycle(), 0, "Graph does not contain any cycles");

  str23 = "ctdaezbnfmypx";
  gt.clear();
  for (size_t s = 0; s < str23.size(); ++s)
    gt.add(str23[s]);
  std::cout << "Kahn's algorithm sequence: ";
  vals = g_large[g_large.kahn()];
  std::cout << vals << std::endl;
  TEST_EQUAL(vals.size(), g_large.size(), "Kahn should detect no cycles");
  TEST_SEQ(vals, gt, 13, "Kahn result should be: c t d a e z b n f m y p x");

  int ccgt5[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::cout << "Connected component labels: ";
  labels = g_large.weakly_conn_comp();
  std::cout << labels << std::endl;
  TEST_SEQ(labels, ccgt5, 13, "Labels should be: 0 0 0 0 0 0 0 0 0 0 0 0 0");

  std::cout << "Testing node access by value:" << std::endl;
  TEST_EQUAL(g_large.node_inds('c')[0], 0, "Index of c should be 0");
  TEST_EQUAL(g_large.node_inds('e')[0], 4, "Index of e should be 4");
  TEST_EQUAL(g_large.node_inds('t')[0], 12, "Index of t should be 12");
  std::cout << std::endl;

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "GRAPH EDGE REMOVAL TESTS" << std::endl << std::endl;

  graph<char> g_tiny;
  g_tiny.add_node('a');
  g_tiny.add_node('b');
  g_tiny.add_node('c');
  g_tiny.add_node('d');
  g_tiny.add_node('e');
  g_tiny.add_edge(0, 1);
  g_tiny.add_edge(0, 2);
  g_tiny.add_edge(1, 2);
  g_tiny.add_edge_undir(2, 4);
  g_tiny.add_edge(3, 0);
  g_tiny.add_edge(4, 0);
  std::cout << g_tiny << std::endl << "Counting edges:" << std::endl;
  TEST_EQUAL(g_tiny.num_edges(), 7, "Initial edge count should be 7");

  g_tiny.remove_edge(1, 2);
  std::cout << "Removed edge (1, 2):" << std::endl;
  TEST_EQUAL(g_tiny.num_edges(), 6, "Dir edge removal should give 6 edges");

  g_tiny.remove_edge_undir(4, 2);
  std::cout << "Removed undirected edge (4, 2):" << std::endl;
  TEST_EQUAL(g_tiny.num_edges(), 4, "Undir edge removal should give 4 edges");

  g_tiny.remove_node(0);
  std::cout << "Removed node 0:" << std::endl;
  TEST_EQUAL(g_tiny.num_edges(), 0, "Node a removal should give 0 edges");

  g_tiny.remove_node(1);
  g_tiny.remove_node(2);
  g_tiny.remove_node(0);
  g_tiny.remove_node(0);
  std::cout << "Removed nodes 1, 2, 0 and 0:" << std::endl;
  TEST_CHECK(g_tiny.is_empty(), "Graph should be empty with all nodes gone");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "GRAPH INITIALIZATION AND ASSIGNMENT TESTS" << std::endl;
  std::cout << std::endl;

  dyn_array<int> val_gt;
  dyn_array<size_t> ind_gt;
  dyn_array<dyn_array<int> > adj_gt;

  graph<int> g = {{1, 0, 1, 1}, {0, 0, 0, 0}, {0, 1, 0, 1}, {1, 1, 0, 0}};
  std::cout << g << "Constructed graph from 2D initializer list:" << std::endl;
  TEST_CHECK(!g.is_empty(), "Graph should not be empty");
  TEST_EQUAL(g.size(), 4, "Graph size should be 4");
  ind_gt = {0, 1, 2, 3};
  TEST_SEQ(g.node_inds(0), ind_gt, 4, "Indices with val 0 should match gt");
  val_gt = {0, 0, 0, 0};
  TEST_SEQ(g[ind_gt], val_gt, 4, "All nodes should have value 0");
  TEST_EQUAL(g.num_edges(), 7, "Graph should have 7 edges");
  adj_gt = {{1, 0, 1, 1}, {0, 0, 0, 0}, {0, 1, 0, 1}, {1, 1, 0, 0}};
  TEST_CHECK(g.adj_mat() == adj_gt, "Edge structure should match init list");
  ind_gt = {0, 2, 3};
  TEST_EQUAL(g.neighbors(0).size(), 3, "Expected 3 neighbors for ind0");
  TEST_SEQ(g.neighbors(0), ind_gt, 3, "ind0 neighbor inds should be correct");
  ind_gt = {0, 1};
  TEST_EQUAL(g.neighbors(3).size(), 2, "Expected 2 neighbors for ind3");
  TEST_SEQ(g.neighbors(3), ind_gt, 2, "ind3 neighbor inds should be correct");
  TEST_CHECK(g.find_roots().is_empty(), "Graph should have no roots");
  TEST_CHECK(g.has_cycle(), "Graph should detect a cycle");
  std::cout << std::endl;

  g = {{0, 1, 0, 1}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
  std::cout << g << "Assigned edges to graph from 2D init list:" << std::endl;
  TEST_EQUAL(g.size(), 4, "Graph size should still be 4");
  ind_gt = {0, 1, 2, 3};
  TEST_SEQ(g.node_inds(0), ind_gt, 4, "Indices with val 0 should match gt");
  TEST_EQUAL(g.num_edges(), 5, "Graph should now have 5 edges");
  adj_gt = {{0, 1, 0, 1}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
  TEST_CHECK(g.adj_mat() == adj_gt, "Edge structure should match init list");
  ind_gt = {1, 3};
  TEST_EQUAL(g.neighbors(0).size(), 2, "Expected 2 neighbors for ind0");
  TEST_SEQ(g.neighbors(0), ind_gt, 2, "ind0 neighbor inds should be correct");
  ind_gt = {0, 1};
  TEST_EQUAL(g.neighbors(3).size(), 0, "Expected 0 neighbors for ind3");
  TEST_CHECK(g.neighbors(3).is_empty(), "ind3 neighbor inds should be empty");
  TEST_CHECK(g.find_roots().size(), "Graph should now have 1 root");
  TEST_EQUAL(g.find_roots()[0], 0, "Graph's only root should have ind 0");
  TEST_CHECK(!g.has_cycle(), "Graph should not have a cycle anymore");
  TEST_EQUAL(g.weakly_conn_comp().unique().size(), 1, "Only 1 component");
  std::cout << std::endl;

  g = {10, 20, 30, 40, 50};
  val_gt = {10, 20, 30, 40, 50};
  std::cout << g << "Assigned node vals from larger init list:" << std::endl;
  TEST_EQUAL(g.size(), 5, "Graph size should now be 5");
  TEST_SEQ(g.node_vals(), val_gt, 5, "Node values should match input");
  TEST_EQUAL(g.num_edges(), 5, "Graph should still have 5 edges");
  adj_gt = {{0, 1, 0, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
  TEST_CHECK(g.adj_mat() == adj_gt, "Edge structure should be unchanged");
  val_gt = {30, 40};
  TEST_EQUAL(g.neighbors(1).size(), 2, "Expected 2 neighbors for ind1");
  TEST_SEQ(g[g.neighbors(1)], val_gt, 2, "ind0 neighbor vals should match gt");
  TEST_EQUAL(g.neighbors(2).size(), 1, "Expected 1 neighbor for ind2");
  TEST_EQUAL(g[g.neighbors(2)][0], 40, "ind2 neighbor val should be correct");
  std::cout << std::endl;

  g = {100, 200, 300};
  std::cout << g << "Assigned node vals from smaller init list:" << std::endl;
  val_gt = {100, 200, 300, 40, 50};
  TEST_EQUAL(g.size(), 5, "Graph size should still be 5");
  TEST_SEQ(g.node_vals(), val_gt, 5, "Node values should match input");
  TEST_EQUAL(g.min(), 40, "Minimum node value in graph should be 40");
  TEST_EQUAL(g.max(), 300, "Maximum node value in graph should be 300");
  std::cout << std::endl;

  val_gt = {100, 200, 300, 400, 500, 600};
  g = val_gt;
  std::cout << "Assigned node vals from larger dyn_array:" << std::endl;
  TEST_EQUAL(g.size(), 6, "Graph size should now be 6");
  TEST_SEQ(g.node_vals(), val_gt, 6, "Node values should match input");

  val_gt = {1000, 2000};
  g = val_gt;
  val_gt = {1000, 2000, 300, 400, 500, 600};
  std::cout << "Assigned node vals from smaller dyn_array:" << std::endl;
  TEST_EQUAL(g.size(), 6, "Graph size should still be 6");
  TEST_SEQ(g.node_vals(), val_gt, 6, "Node values should match input");
  std::cout << std::endl;

  g = {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 0, 0}, {0, 1, 0, 0}};
  val_gt = {1000, 2000, 300, 400};
  std::cout << g << "Assigned edges from smaller 2D init list:" << std::endl;
  TEST_EQUAL(g.size(), 4, "Graph size should now be 4");
  TEST_SEQ(g.node_vals(), val_gt, 4, "Remaining node values should be same");
  TEST_EQUAL(g.num_edges(), 2, "Graph should now have 2 edges");
  TEST_CHECK(g.find_roots().size(), "Graph should have 1 root");
  TEST_EQUAL(g.find_roots()[0], 0, "Graph's only root should have ind 0");
  TEST_CHECK(g.has_cycle(), "Graph should have a cycle");
  val_gt = {0, 1, 2, 1};
  TEST_EQUAL(g.weakly_conn_comp().unique().size(), 3, "Should be 3 comp");
  TEST_SEQ(g.weakly_conn_comp(), val_gt, 4, "Comps should be correct");
  std::cout << std::endl;

  adj_gt = {{1, 0, 1}, {0, 1, 1}, {0, 0, 1}};
  g = adj_gt;
  val_gt = {1000, 2000, 300};
  std::cout << g << "Assigned edges from smaller 2D dyn_array:" << std::endl;
  TEST_EQUAL(g.size(), 3, "Graph size should now be 3");
  TEST_SEQ(g.node_vals(), val_gt, 3, "Remaining node values should be same");
  TEST_EQUAL(g.num_edges(), 5, "Graph should now have 5 edges");
  TEST_CHECK(g.adj_mat() == adj_gt, "Edge structure should match dyn_array");
  std::cout << std::endl;

  adj_gt = {{1, 1, 1, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 1}};
  g = adj_gt;
  val_gt = {1000, 2000, 300, 0};
  std::cout << g << "Assigned edges from larger 2D dyn_array:" << std::endl;
  TEST_EQUAL(g.size(), 4, "Graph size should now be 4");
  TEST_SEQ(g.node_vals(), val_gt, 4, "New node should have value 0");
  TEST_EQUAL(g.num_edges(), 12, "Graph should now have 12 edges");
  TEST_CHECK(g.adj_mat() == adj_gt, "Edge structure should match dyn_array");
  std::cout << std::endl;

  g = {{0, 1, 0, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 1, 0},
       {1, 0, 0, 0, 0}, {0, 1, 0, 0, 1}};
  val_gt = {1000, 2000, 300, 0, 0};
  std::cout << g << "Assigned edges from larger 2D init list:" << std::endl;
  TEST_EQUAL(g.size(), 5, "Graph size should now be 5");
  TEST_SEQ(g.node_vals(), val_gt, 5, "New node should have value 0");
  adj_gt = {{0, 1, 0, 1, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 1, 0},
            {1, 0, 0, 0, 0}, {0, 1, 0, 0, 1}};
  TEST_EQUAL(g.num_edges(), 8, "Graph should now have 8 edges");
  TEST_CHECK(g.adj_mat() == adj_gt, "Edge structure should match dyn_array");
  std::cout << std::endl;

  graph<int> h = adj_gt;
  h = {1000, 2000, 300, 0, 0};
  std::cout << h << "Built new graph from the same 2D dyn_array:" << std::endl;
  TEST_EQUAL(h.size(), 5, "Graph size should be 5");
  TEST_CHECK(h.adj_mat() == adj_gt, "Edge structure should match dyn_array");
  std::cout << std::endl;

  graph<int> k(h);
  std::cout << k << "Copied graph with copy constructor:" << std::endl;
  TEST_EQUAL(k.size(), 5, "Graph size should be 5");
  TEST_SEQ(k.node_vals(), val_gt, 5, "Node values should match input");
  TEST_CHECK(k.adj_mat() == adj_gt, "Edge structure should match dyn_array");
  TEST_CHECK(k == h, "Copy should be equal to the source");
  std::cout << std::endl;

  k.remove_edge(1, 2);
  k.add_edge(3, 4);
  std::cout << k << "Changed one edge on the copy:" << std::endl;
  TEST_CHECK(k.num_edges() == h.num_edges(), "Copy edge count should be same");
  TEST_CHECK(k.node_vals() == h.node_vals(), "Copy node vals should be same");
  TEST_CHECK(k != h, "Copy should not be equal to the source after edits");
  std::cout << std::endl;

  g = {2000, 0, 1000, 0, 300};
  g.clear_edges();
  g.add_edge_undir(0, 1);
  g.add_edge(1, 3);
  g.add_edge_undir(2, 3);
  g.add_edge(2, 0);
  g.add_edge(4, 3);
  g.add_edge(0, 4);
  std::cout << g << "Created permutation of same graph structure" << std::endl;
  TEST_CHECK(g.adj_mat() != h.adj_mat(), "Adj matrices should be different");
  TEST_CHECK(g == h, "Graphs with different adj matrices should be equal");
  std::cout << std::endl;

  h = k;
  std::cout << "Assigned the copy with changed edge to its source" << std::endl;
  TEST_CHECK(g.size() == h.size(), "Node counts should be equal");
  TEST_CHECK(g.num_edges() == h.num_edges(), "Edge counts should be equal");
  dyn_array<int> gval_sorted = g.node_vals().quick_sort();
  dyn_array<int> hval_sorted = h.node_vals().quick_sort();
  TEST_CHECK(gval_sorted == hval_sorted, "Sorted node vals should be equal");
  TEST_CHECK(g != h, "Graphs should not be equal");

  test_util::print_summary();
  return 0;
}