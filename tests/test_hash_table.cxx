#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include "test_util.h"
#include "hash_table.h"

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH std::string KEYS" << std::endl << std::endl;

  std::string pynchon[23];
  pynchon[0]  = "With a clattering of chairs, upended shell cases, benches, ";
  pynchon[1]  = "and ottomans, Pirate's mob gather at the shores of the ";
  pynchon[2]  = "great refectory table, a southern island well across a ";
  pynchon[3]  = "tropic or two from chill Corydon Throsp's mediaeval ";
  pynchon[4]  = "fantasies, crowded now over the swirling dark grain of its ";
  pynchon[5]  = "walnut uplands with banana omelets, banana sandwiches, ";
  pynchon[6]  = "banana casseroles, mashed bananas molded into the shape of ";
  pynchon[7]  = "a British lion rampant, blended with eggs into batter for ";
  pynchon[8]  = "French toast, squeezed out a pastry nozzle across the ";
  pynchon[9]  = "quivering creamy reaches of a banana blancmange to spell ";
  pynchon[10] = "out the words C'est magnifique, mais ce n'est pas la guerre ";
  pynchon[11] = "(attributed to a French observer during the Charge of the ";
  pynchon[12] = "Light Brigade) which Pirate has appropriated as his ";
  pynchon[13] = "motto... tall cruets of pale banana syrup to pour oozing ";
  pynchon[14] = "over banana waffles, a giant glazed crock where diced ";
  pynchon[15] = "bananas have been fermenting since the summer with wild ";
  pynchon[16] = "honey and muscat raisins, up out of which, this winter ";
  pynchon[17] = "morning, one now dips foam mugsfull of banana mead... ";
  pynchon[18] = "banana croissants and banana kreplach, and banana oatmeal ";
  pynchon[19] = "and banana jam and banana bread, and bananas flamed in ";
  pynchon[20] = "ancient brandy Pirate brought back last year from a cellar ";
  pynchon[21] = "in the Pyrenees also containing a clandestine radio ";
  pynchon[22] = "transmitter...";

  hash_table<std::string, int> ht1(107);
  std::string str;
  for (unsigned i = 0; i < 23; ++i) {
    std::cout << pynchon[i] << std::endl;
    std::stringstream ss(pynchon[i].c_str());
    int v = 0;
    while (ss >> str)
      ht1.insert(str, (1000*i) + (v++));
  }
  std::cout << std::endl << "Hash contents: " << std::endl << ht1 << std::endl;
  TEST_CHECK(ht1.contains("banana"), "Hash should contain \"banana\"");
  TEST_EQUAL(ht1["banana"], 19004, "Key \"banana\" should have value 19004");
  TEST_EQUAL(ht1.contains("musaceous"), 0, "Shouldn't have key \"musaceous\"");
  TEST_EQUAL(ht1.keys().size(), 144, "Hash should have 144 std::string keys");
  TEST_EQUAL(ht1.vals().size(), 144, "Hash should have 144 int values");

  ht1.remove("banana").remove("the").remove("into").remove("mediaeval");
  std::cout << "Removed \"banana\", \"the\", \"into\" and \"mediaeval\"";
  TEST_EQUAL(ht1.contains("banana"), 0, "Hash shouldn't have key \"banana\"");
  TEST_EQUAL(ht1.contains("the"), 0, "Hash shouldn't have key \"the\"");
  TEST_EQUAL(ht1.keys().size(), 140, "There should be 140 keys left");
  TEST_EQUAL(ht1.vals().size(), 140, "There should be 140 values left");

  std::cout << "Trying to remove \"into\" and \"candle\"" << std::endl;
  ht1.remove("into").remove("candle");
  TEST_EQUAL(ht1.keys().size(), 140, "There should still be 140 keys");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH unsigned KEYS" << std::endl;

  hash_table<unsigned, int> ht2(107);
  for (unsigned i = 1; i <= 200; ++i) {
    ht2.insert(i*10, static_cast<int>(i));
  }
  std::cout << "Inserted 200 multiples of 10 with their indices" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht2 << std::endl;
  TEST_CHECK(ht2.contains(100), "Hash should contain 100");
  TEST_EQUAL(ht2[100], 10, "Key 100 should have value 10");
  TEST_EQUAL(ht2.contains(103), 0, "Shouldn't have key 103");
  TEST_EQUAL(ht2.keys().size(), 200, "Hash should have 200 unsigned keys");
  TEST_EQUAL(ht2.vals().size(), 200, "Hash should have 200 int values");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH uintptr_t POINTER KEYS" << std::endl;

  node<std::string, false>* str_list = nullptr;
  node<std::string, false>* other = nullptr;
  std::string buf;
  for (unsigned i = 0; i < 23; ++i) {
    std::stringstream ss(pynchon[i].c_str());
    while (ss >> buf) {
      if (str_list)
        str_list->append(buf);
      else
        str_list = node<std::string, false>::create(buf);
    }
  }
  std::cout << "Created node-only linked list" << std::endl;
  TEST_EQUAL(str_list->size(), 203, "Linked list should have 204 nodes");

  hash_table<uintptr_t, bool> ht3(107, JENKINS);
  node<std::string, false>* node_ptr = str_list;
  while (node_ptr) {
    ht3.insert(reinterpret_cast<uintptr_t>(node_ptr), true);
    node_ptr = node_ptr->next();
  }
  std::cout << "Hashed each node by pointer" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht3 << std::endl;
  TEST_EQUAL(ht3.keys().size(), 203, "Hash should have 204 node pointers");
  bool contains_all = true;
  node_ptr = str_list;
  while (node_ptr) {
    if (!ht3.contains(reinterpret_cast<uintptr_t>(node_ptr))) {
      contains_all = false;
      break;
    }
    node_ptr = node_ptr->next();
  }
  TEST_CHECK(contains_all, "Hash should have pointers of all inserted nodes");
  uintptr_t ot = reinterpret_cast<uintptr_t>(other);
  TEST_EQUAL(ht3.contains(ot), 0, "Hash shouldn't have an unrelated node ptr");

  delete str_list;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH float KEYS" << std::endl;

  // Hashing floats with their uint32_t versions for visual inspection.
  hash_table<float, uint32_t> ht4(107);
  uint32_t fval;
  for (float i = 0; i < 200; ++i) {
    // Smallest increment that float can represent without change is 1e-6.
    float key = i*1e-6f;
    std::memcpy(&fval, &key, sizeof(key));
    ht4.insert(key, fval);
  }
  std::cout << "Hashed 200 multiples of 1e-6 with bit patterns" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht4 << std::endl;
  TEST_EQUAL(ht4.keys().size(), 200, "Hash should have 200 float keys");
  contains_all = true;
  for (float i = 0; i < 200; ++i) {
    if (!ht4.contains(i*1e-6f)) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain each distinct float inserted");
  TEST_EQUAL(ht4.contains(0.0002f), 0, "Hash shouldn't contain 0.0002");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH double KEYS" << std::endl;

  // Hashing doubles with their uint64_t versions for visual inspection.
  hash_table<double, uint64_t> ht5(107);
  uint64_t dval;
  for (double i = 0; i < 200; ++i) {
    // Smallest increment that double can represent without change is 1e-15.
    double key = i*1e-15;
    std::memcpy(&dval, &key, sizeof(key));
    ht5.insert(key, dval);
  }
  std::cout << "Hashed 200 multiples of 1e-15 with bit patterns" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht5 << std::endl;
  TEST_EQUAL(ht5.keys().size(), 200, "Hash should have 200 double keys");
  contains_all = true;
  for (double i = 0; i < 200; ++i) {
    if (!ht5.contains(i*1e-15)) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain each distinct double inserted");
  TEST_EQUAL(ht5.contains(2e-13), 0, "Hash shouldn't contain 2e-13");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HASH TABLE WITH char KEYS" << std::endl;

  hash_table<char, size_t> ht6(107);
  for (size_t c = 1; c < 7; ++c)
    ht6.insert(static_cast<char>(c), c);
  for (size_t c = 11; c < 13; ++c)
    ht6.insert(static_cast<char>(c), c);
  for (size_t c = 15; c < 27; ++c)
    ht6.insert(static_cast<char>(c), c);
  for (size_t c = 49; c < 255; ++c)
    ht6.insert(static_cast<char>(c), c);
  std::cout << "Hashed all printable and formattable characters" << std::endl;
  std::cout << "Hash contents: " << std::endl << ht6 << std::endl;
  TEST_EQUAL(ht6.keys().size(), 226, "Hash should have 226 char keys");
  contains_all = true;
  for (size_t c = 1; c < 7; ++c) {
    if (!ht6.contains(static_cast<char>(c))) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain characters [1-7)");
  contains_all = true;
  for (size_t c = 11; c < 13; ++c) {
    if (!ht6.contains(static_cast<char>(c))) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain characters [11-13)");
  contains_all = true;
  for (size_t c = 15; c < 27; ++c) {
    if (!ht6.contains(static_cast<char>(c))) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain characters [15-27)");
  contains_all = true;
  for (size_t c = 49; c < 255; ++c) {
    if (!ht6.contains(static_cast<char>(c))) {
      contains_all = false;
      break;
    }
  }
  TEST_CHECK(contains_all, "Hash should contain characters [49-255)");

  test_util::print_summary();
  return 0;
}