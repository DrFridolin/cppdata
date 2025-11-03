cmake_minimum_required(VERSION 3.10)
project(cppdata)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add include directory for header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Explicitly list test executables
add_executable(test_dyn_array tests/test_dyn_array.cxx)
add_executable(test_linked_list tests/test_linked_list.cxx)

add_executable(test_hash_table tests/test_hash_table.cxx)
add_executable(test_hash_table_containers tests/test_hash_table_containers.cxx)

add_executable(test_stack tests/test_stack.cxx)
add_executable(test_queue tests/test_queue.cxx)

add_executable(test_graph_node tests/test_graph_node.cxx)
add_executable(test_graph_node_directed tests/test_graph_node_directed.cxx)
add_executable(test_graph_unweighted tests/test_graph_unweighted.cxx)
add_executable(test_graph_weighted tests/test_graph_weighted.cxx)

add_executable(test_binary_tree tests/test_binary_tree.cxx)
add_executable(test_binary_search_tree tests/test_binary_search_tree.cxx)
add_executable(test_binary_avl_tree tests/test_binary_avl_tree.cxx)
add_executable(test_binary_red_black_tree tests/test_binary_red_black_tree.cxx)
add_executable(test_heap tests/test_heap.cxx)
add_executable(test_trie tests/test_trie.cxx)