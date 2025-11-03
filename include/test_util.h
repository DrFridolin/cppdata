#ifndef data_test_util_h_
#define data_test_util_h_

#include <iostream>
#include <iomanip>
#include <string>

namespace test_util {
  namespace {
    int test_count = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    std::string failures;
  }

  void print_summary() {

    std::string title;
    if (failed_tests == 0)
      title = "|       ALL TESTS EXECUTED SUCCESSFULLY      |";
    else
      title = "|     TESTS EXECUTED WITH SOME FAILURES      |";

    std::cout << "----------------------------------------------" << std::endl;
    std::cout <<                      title                       << std::endl;
    std::cout << "|                  SUMMARY:                  |" << std::endl;
    std::cout << "|             " << std::setw(3) << passed_tests;
    std::cout <<                  " tests passed               |" << std::endl;
    std::cout << "|             " << std::setw(3) << failed_tests;
    std::cout <<                  " tests failed               |" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    if (!failures.empty())
      std::cout << "The following tests failed:" << std::endl << failures;
  }
}

#define TEST_CHECK(condition, message) \
  do { \
    test_util::test_count++; \
    if (!(condition)) { \
      std::cerr << "  TEST FAILED: " << message << " at " << __FILE__ << ":" \
                << __LINE__ << std::endl; \
      test_util::failures += "  TEST FAILED: " + std::string(message) + \
                             " at " + __FILE__ + ":" + \
                             std::to_string(__LINE__) + "\n"; \
      test_util::failed_tests++; \
    } else { \
      std::cout <<   "  TEST PASSED: " << message << std::endl; \
      test_util::passed_tests++; \
    } \
  } while (false)

#define TEST_EQUAL(actual, expected, message) \
  TEST_CHECK((actual) == (expected), message)

#define TEST_NOT_EQUAL(actual, expected, message) \
  TEST_CHECK((actual) != (expected), message)

#define TEST_SEQ(container, expected, count, message) \
  do { \
    test_util::test_count++; \
    bool passed = true; \
    size_t size = (count); \
    const auto& cont = (container); \
    const auto& exp = (expected); \
    if (cont.size() != size) passed = false; \
    else { \
      for (size_t i = 0; i < size; ++i) { \
        if (cont[i] != exp[i]) { \
          passed = false; \
          break; \
        } \
      } \
    } \
    if (!passed) { \
      std::cerr << "  TEST FAILED: " << message << '\n'; \
      std::cerr << "    at " << __FILE__ << ":" << __LINE__ << '\n'; \
      test_util::failures += "  TEST FAILED: " + std::string(message) + \
                             "\n    at " + __FILE__ + ":" + \
                             std::to_string(__LINE__) + "\n"; \
      test_util::failed_tests++; \
    } else { \
      std::cout << "  TEST PASSED: " << message << '\n'; \
      test_util::passed_tests++; \
    } \
  } while (false)

#define TEST_PTR(container, expected, count, message) \
  do { \
    test_util::test_count++; \
    bool passed = true; \
    size_t size = (count); \
    const auto& cont = (container); \
    const auto& exp = (expected); \
    if (cont.size() != size) passed = false; \
    else { \
      for (size_t i = 0; i < size; ++i) { \
        if (cont[i] && *cont[i] != exp[i]) { \
          passed = false; \
          break; \
        } \
      } \
    } \
    if (!passed) { \
      std::cerr << "  TEST FAILED: " << message << '\n'; \
      std::cerr << "    at " << __FILE__ << ":" << __LINE__ << '\n'; \
      test_util::failures += "  TEST FAILED: " + std::string(message) + \
                             "\n    at " + __FILE__ + ":" + \
                             std::to_string(__LINE__) + "\n"; \
      test_util::failed_tests++; \
    } else { \
      std::cout << "  TEST PASSED: " << message << '\n'; \
      test_util::passed_tests++; \
    } \
  } while (false)

#define TEST_PTR_SEQ(container, expected, count, message) \
  do { \
    test_util::test_count++; \
    bool passed = true; \
    size_t size = (count); \
    const auto& cont = (container); \
    const auto& exp = (expected); \
    if (cont.size() != size) passed = false; \
    else { \
      for (size_t i = 0; i < size; ++i) { \
        const auto& val = cont[i]->val(); \
        if (val != exp[i]) { \
          passed = false; \
          break; \
        } \
      } \
    } \
    if (!passed) { \
      std::cerr << "  TEST FAILED: " << message << '\n'; \
      std::cerr << "    at " << __FILE__ << ":" << __LINE__ << '\n'; \
      test_util::failures += "  TEST FAILED: " + std::string(message) + \
                             "\n    at " + __FILE__ + ":" + \
                             std::to_string(__LINE__) + "\n"; \
      test_util::failed_tests++; \
    } else { \
      std::cout << "  TEST PASSED: " << message << '\n'; \
      test_util::passed_tests++; \
    } \
  } while (false)

#define TEST_LIST_SEQ(list, expected, count, message) \
  do { \
    test_util::test_count++; \
    bool passed = true; \
    unsigned size = (count); \
    const auto& l = (list); \
    decltype(l.head()) p = l.head(); \
    unsigned i = 0; \
    while (i < size && p) { \
      if (p->val() != (expected)[i]) { \
        passed = false; \
        break; \
      } \
      if (i < size - 1) p = p->next(); \
      ++i; \
    } \
    if (i != size) passed = false; \
    if (!passed) { \
      std::cerr << "  TEST FAILED: " << message << " at " << __FILE__ << ":" \
                << __LINE__ << '\n'; \
      test_util::failures += "  TEST FAILED: " + std::string(message) + \
                             " at " + __FILE__ + ":" + \
                             std::to_string(__LINE__) + "\n"; \
      test_util::failed_tests++; \
    } else { \
      std::cout << "  TEST PASSED: " << message << '\n'; \
      test_util::passed_tests++; \
    } \
  } while (0)

#define TEST_NODE_SEQ(node_ref, expected, count, message) \
  do { \
    test_util::test_count++; \
    bool passed = true; \
    unsigned size = (count); \
    auto expected_copy = expected; \
    decltype(&(node_ref)) p = &(node_ref); \
    unsigned i = 0; \
    while (p && i < size) { \
      auto expected_val = expected_copy[i]; \
      if (p->val() != expected_val) { \
        passed = false; \
      } \
      p = p->next(); \
      ++i; \
    } \
    if (p || i != size) { \
      passed = false; \
    } \
    if (!passed) { \
      std::cerr << "  TEST FAILED: " << message << " at " << __FILE__ << ":" \
                << __LINE__ << '\n'; \
      test_util::failures += "  TEST FAILED: " + std::string(message) + \
                             " at " + __FILE__ + ":" + \
                             std::to_string(__LINE__) + "\n"; \
      test_util::failed_tests++; \
    } else { \
      std::cout << "  TEST PASSED: " << message << '\n'; \
      test_util::passed_tests++; \
    } \
  } while (0)

#endif