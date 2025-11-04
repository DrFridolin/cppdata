#include <iostream>
#include <iomanip>
#include "dyn_array.h"
#include "test_util.h"

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "BUILDING FROM AN EMPTY ARRAY_LIST OF DOUBLES" << std::endl;

  dyn_array<double> al1;
  TEST_EQUAL(al1.size(), 0, "Initial size of al1 should be 0");

  al1.add(3.14);
  TEST_EQUAL(al1.size(), 1, "Size of al1 should be 1 after adding 3.14");
  TEST_EQUAL(al1[0], 3.14, "First element of al1 should be 3.14");

  al1.add(2.71).add(0.0).add(-1024.025).add(-0.00001);
  TEST_EQUAL(al1.size(), 5, "Chain-adding 4 more elements should give size 5");
  std::cout << "Elements of al1: {";
  for (size_t i = 0; i < al1.size(); ++i)
  {
    std::cout << al1[i];
    if (i < al1.size() - 1)
      std::cout << ", ";
    else
      std::cout << "}" << std::endl;
  }
  double al1_exp[] = {3.14, 2.71, 0.0, -1024.025, -0.00001};
  TEST_SEQ(al1, al1_exp, 5, "Values of al1 should match to what is set");

  unsigned pell[10] = {0, 1, 2, 5, 12, 29, 70, 169, 408, 985};
  dyn_array<unsigned> err1(pell, 0, 0);
  dyn_array<unsigned> err2(pell, 23, 15);
  dyn_array<unsigned> err3(pell, 32, 32);
  dyn_array<unsigned> err4(nullptr, 0, 5);
  std::cout << "Testing erroneous constructions from pointer:" << std::endl;
  TEST_CHECK(err1.is_empty(), "Array should be empty if end is not larger");
  TEST_CHECK(err2.is_empty(), "Array should be empty if end is not larger");
  TEST_CHECK(err3.is_empty(), "Array should be empty if end is not larger");
  TEST_CHECK(err4.is_empty(), "Array should be empty if end is not larger");

  unsigned pgt1[3] = {12, 29, 70};
  dyn_array<unsigned> pll(pell, 4, 7);
  std::cout << "Constructed dyn_array from 5th, 6th and 7th Pell numbers: ";
  std::cout << pll << std::endl;
  TEST_SEQ(pll, pgt1, 3, "Pell numbers in dyn_array should be correct");
  pll.add(pell, 7, 3);
  TEST_SEQ(pll, pgt1, 3, "Adding with wrong indices should not change array");
  pll.add(pell, 7, 7);
  TEST_SEQ(pll, pgt1, 3, "Adding with wrong indices should not change array");

  unsigned pgt2[5] = {12, 29, 70, 169, 408};
  pll.add(pell, 7, 9);
  std::cout << "Added 8th and 9th Pell numbers: " << pll << std::endl;
  TEST_SEQ(pll, pgt2, 5, "Pell numbers in dyn_array should be correct");

  pll.resize(6);
  std::cout << "Resized dyn_array to 6:" << std::endl;
  TEST_EQUAL(pll.size(), 6, "New size for dyn_array should be 6");
  TEST_EQUAL(pll[5], 0, "Expanding resize should pad dyn_array with default");

  unsigned pgt3[6] = {12, 29, 70, 169, 408, 985};
  pll[5] = pell[9];
  std::cout << "Set the 6th element to 10th Pell number: " << pll << std::endl;
  TEST_SEQ(pll, pgt3, 6, "Pell numbers in dyn_array should be correct");

  pll.resize(3);
  std::cout << "Resized dyn_array to 3: " << pll << std::endl;
  TEST_EQUAL(pll.size(), 3, "New size for dyn_array should be 3");
  TEST_SEQ(pll, pgt1, 3, "Pell numbers in dyn_array should be correct");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "INITIALIZING FROM A LONG ARRAY OF UNSIGNED INTS" << std::endl;

  unsigned in[200], add[400];
  for (unsigned i = 0; i < 200; ++i)
    in[i] = i;
  for (unsigned i = 0; i < 400; ++i)
    add[i] = i * 2;

  std::cout << "Building from a raw array using its pointer:" << std::endl;
  dyn_array<unsigned> al2(in, 0, 200);
  TEST_EQUAL(al2.size(), 200, "Building al2 from array should give size 200");
  TEST_SEQ(al2, in, 200, "Initial values for al2 should be integer seq 0-199");

  for (unsigned i = 0; i < 400; ++i)
    al2.add(add[i]);
  std::cout << "Added 400 more elements to al2 one by one" << std::endl;
  TEST_EQUAL(al2.size(), 600, "Size should be 600 after one-by-one addition");

  unsigned arr[100];
  for (unsigned i = 0; i < 100; ++i)
    arr[i] = i / 10;
  al2.add(arr, 0, 100);
  std::cout << "Bulk-added 10 copies of each integer in seq 0-9" << std::endl;
  TEST_EQUAL(al2.size(), 700, "Size should be 700 after bulk addition");

  al2[699] = 99;
  std::cout << "Changed the last element of al2 to 99" << std::endl;
  std::cout << "New last element: " << al2[699] << std::endl;
  TEST_EQUAL(al2[699], 99, "Last element should be 99");
  std::cout << "Contents of al2 after all the additions:" << std::endl;
  std::cout << al2 << std::endl;

  dyn_array<unsigned> al3(nullptr, 0, 100);
  dyn_array<unsigned> al4(nullptr, 0, 100);
  dyn_array<unsigned> al5(nullptr, 0, 100);
  std::cout << "Instanted empty array lists of size 100" << std::endl;
  TEST_EQUAL(al3.size(), 0, "Initial size of al3 should be 0");
  TEST_EQUAL(al4.size(), 0, "Initial size of al4 should be 0");
  TEST_EQUAL(al5.size(), 0, "Initial size of al5 should be 0");

  for (unsigned i = 0; i < 200; ++i)
    al3.add(in[i]);
  std::cout << "Added 200 integers to al3 one by one" << std::endl;
  TEST_EQUAL(al3.size(), 200, "Size of al3 should be 200");
  TEST_SEQ(al3, in, 200, "Values of al3 should match integer sequence 0-199");

  al4.add(in, 0, 200);
  std::cout << "Added 200 integers to al4 in one shot" << std::endl;
  TEST_EQUAL(al4.size(), 200, "Size of al4 should be 200");
  TEST_SEQ(al4, in, 200, "Values of al4 should match integer sequence 0-199");

  al5.add(add, 0, 80);
  std::cout << "Added 80 even integers to al5 in one shot" << std::endl;
  TEST_EQUAL(al5.size(), 80, "Size of al5 should be 80");
  TEST_SEQ(al5, add, 80, "Values of al5 should match even integer seq 0-158");

  al5.realloc(150);
  std::cout << "Expanded al5 to capacity 150" << std::endl;
  TEST_EQUAL(al5.size(), 80, "Size of al5 should still be 80");
  TEST_SEQ(al5, add, 80, "Values of al5 should match even integer seq 0-158");

  al5.add(&add[80], 0, 70);
  std::cout << "Added 70 more even integers to al5 in one shot" << std::endl;
  TEST_EQUAL(al5.size(), 150, "Size of al5 should be 150");
  TEST_SEQ(al5, add, 150, "Values of al5 should match even integer seq 0-298");

  al5.add(&add[150], 0, 50);
  std::cout << "Added 50 more even integers to al5 in one shot" << std::endl;
  TEST_EQUAL(al5.size(), 200, "Size of al5 should be 200");
  TEST_SEQ(al5, add, 200, "Values of al5 should match even integer seq 0-398");

  al5.realloc(100);
  std::cout << "Resized al5 to 100 elements" << std::endl;
  TEST_EQUAL(al5.size(), 100, "Size of al5 should be reduced to 100");
  TEST_SEQ(al5, add, 100, "Values of al5 should match even integer seq 0-98");
  std::cout << "Contents of al5:" << std::endl << al5 << std::endl;
  std::cout << std::endl;

  dyn_array<unsigned> al6(al5);
  std::cout << "Created al6 as a copy of al5" << std::endl;
  TEST_EQUAL(al5.size(), al6.size(), "Size of al5 and al6 should match");
  TEST_SEQ(al5, al6, 100, "Values of al5 and al6 should match");

  dyn_array<unsigned> al7;
  al7.add(19);
  al7 = al5;
  std::cout << "Created one-element al7, then assigned al5 to it" << std::endl;
  TEST_EQUAL(al5.size(), al7.size(), "Size of al5 and al7 should match");
  TEST_EQUAL(al5.size(), al6.size(), "Size of al5 and al6 should match");
  TEST_EQUAL(al6.size(), al7.size(), "Size of al6 and al7 should match");
  TEST_SEQ(al5, al7, 100, "Values of al5 and al7 should match");
  TEST_SEQ(al5, al6, 100, "Values of al5 and al6 should match");
  TEST_SEQ(al6, al7, 100, "Values of al6 and al7 should match");

  al7.reverse();
  unsigned gt1[100];
  for (unsigned i = 0; i < 100; ++i)
    gt1[i] = 198 - i*2;
  std::cout << "Reversed al7" << std::endl;
  TEST_SEQ(al7, gt1, 100, "Reversed al7 should have the correct order");
  TEST_EQUAL(al7.last(), al5[0], "Tail of al7 should match the head of al5");
  TEST_EQUAL(al7[0], al6.last(), "Head of al7 should match the tail of al6");

  al5.append(al7);
  unsigned gt2[200];
  for (unsigned i = 0; i < 100; ++i)
    gt2[i] = i*2;
  for (unsigned i = 0; i < 100; ++i)
    gt2[i+100] = 198 - i*2;
  std::cout << "Appended al7 to al5" << std::endl;
  TEST_EQUAL(al5.size(), 200, "Size of al5 should be 200 after appending");
  TEST_SEQ(al5, gt2, 200, "Values of al5 should be 0-198 followed by 198-0");

  al5.clear();
  std::cout << "Cleared al5" << std::endl;
  TEST_EQUAL(al5.size(), 0, "Size of al5 should be 0 after clearing");
  TEST_NOT_EQUAL(al6.size(), 0, "Size of al6 should not be affected");
  TEST_NOT_EQUAL(al7.size(), 0, "Size of al7 should not be affected");

  al6.clear();
  std::cout << "Cleared al6" << std::endl;
  TEST_EQUAL(al6.size(), 0, "Size of al6 should be 0 after clearing");
  TEST_NOT_EQUAL(al7.size(), 0, "Size of al7 should not be affected");

  al7.clear();
  std::cout << "Cleared al7" << std::endl;
  TEST_EQUAL(al6.size(), 0, "Size of al7 should be 0 after clearing");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "INITIALIZER LIST CONSTRUCTOR TESTS" << std::endl;

  bool beethoven[] = {false, false, false, true};
  char welles[] = {'r', 'o', 's', 'e', 'b', 'u', 'd'};
  unsigned char deficient[] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11};
  unsigned short fortunate[] = {3, 5, 7, 13, 23, 17, 19, 23, 37, 61};
  unsigned weird[] = {70, 836, 4030, 5830, 7192, 7912, 9272, 10430, 10570,
                      10792, 10990, 11410, 11690, 12110, 12530, 12670, 13370};
  unsigned long long perfect[] = {6, 28, 496, 8128, 33550336, 8589869056,
                                  137438691328, 2305843008139952128};
  short euler[] = {1, 0, -1, 0, 5, 0, -61, 0, 1385, 0};
  int bernoulli[] = {1, -1, 1, 0, -1, 0, 1, 0, -1, 0, 5, 0, -691, 0, 7, 0,
                     -3617, 0, 43867, 0, -174611, 0, 854513, 0, -236364091};
  long long ramanujan[] = {1, -24, 252, -1472, 4830, -6048, -16744, 84480,
                           -113643, -115920, 534612, -370944, -577738};
  double pi = {3.141592653589793238462643383279502884197169399375};

  dyn_array<bool> bth = {false, false, false, true};
  dyn_array<char> wls = {'r', 'o', 's', 'e', 'b', 'u', 'd'};
  dyn_array<unsigned char> dfc = {1, 2, 3, 4, 5, 7, 8, 9, 10, 11};
  dyn_array<unsigned short> frt = {3, 5, 7, 13, 23, 17, 19, 23, 37, 61};
  dyn_array<unsigned> wrd = {70, 836, 4030, 5830, 7192, 7912, 9272, 10430,
    10570, 10792, 10990, 11410, 11690, 12110, 12530, 12670, 13370};
  dyn_array<unsigned long long> prf = {6, 28, 496, 8128, 33550336, 8589869056,
    137438691328, 2305843008139952128};
  dyn_array<short> eul = {1, 0, -1, 0, 5, 0, -61, 0, 1385, 0};
  dyn_array<int> brn = {1, -1, 1, 0, -1, 0, 1, 0, -1, 0, 5, 0, -691, 0, 7, 0,
    -3617, 0, 43867, 0, -174611, 0, 854513, 0, -236364091};
  dyn_array<long long> rmj = {1, -24, 252, -1472, 4830, -6048, -16744, 84480,
    -113643, -115920, 534612, -370944, -577738};
  dyn_array<float> emp = {};
  dyn_array<double> pi_ = {3.141592653589793238462643383279502884197169399375};

  std::cout << "Testing initializer list for each data type:" << std::endl;
  TEST_SEQ(bth, beethoven, 4, "Sequences of bool should match");
  TEST_SEQ(wls, welles, 7, "Sequences of char should match");
  TEST_SEQ(dfc, deficient, 10, "Sequences of unsigned char should match");
  TEST_SEQ(frt, fortunate, 10, "Sequences of unsigned short should match");
  TEST_SEQ(wrd, weird, 17, "Sequences of unsigned should match");
  TEST_SEQ(prf, perfect, 8, "Sequences of unsigned long long should match");
  TEST_SEQ(eul, euler, 10, "Sequences of short should match");
  TEST_SEQ(brn, bernoulli, 25, "Sequences of int should match");
  TEST_SEQ(rmj, ramanujan, 13, "Sequences of long long should match");
  TEST_CHECK(emp.is_empty(), "Empty list should create empty dyn_array");
  TEST_EQUAL(pi_.size(), 1, "Single-element list should create with size 1");
  TEST_EQUAL(pi_[0], pi, "Single element values should match");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "LINEAR AND BINARY SEARCH TESTS" << std::endl << std::endl;

  // Version to test direct value search.
  dyn_array<unsigned short> euler_totient;
  euler_totient = { 1,  1,  2,  2,  4,  2,  6,  4,  6,  4, 10,  4, 12,  6,  8,
                    8, 16,  6, 18,  8, 12, 10, 22,  8, 20, 12, 18, 12, 28,  8,
                   30, 16, 20, 16, 24, 12, 36, 18, 24, 16, 40, 12, 42, 20, 24,
                   22, 46, 16, 42, 20, 32, 24, 52, 18, 40, 24, 36, 28, 58, 16,
                   60, 30, 36, 32, 48, 20, 66, 32, 44, 24, 70, 24, 72, 36, 40,
                   36, 60, 24, 78, 32, 54, 40, 82, 24, 64, 42, 56, 40, 88, 24,
                   72, 44, 60, 46, 72, 32, 96, 42, 60, 40};
  // Version to test pointed-to value search.
  dyn_array<unsigned short*> euler_ptr(nullptr, 100);
  for (size_t i = 0; i < euler_totient.size(); ++i)
    euler_ptr[i] = &euler_totient[i];
  // Ground truth sorted order.
  dyn_array<unsigned short> eul_gt;
  eul_gt = { 1,  1,  2,  2,  2,  4,  4,  4,  4,  6,  6,  6,  6,  8,  8,  8,  8,
             8, 10, 10, 12, 12, 12, 12, 12, 12, 16, 16, 16, 16, 16, 16, 18, 18,
            18, 18, 20, 20, 20, 20, 20, 22, 22, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 28, 28, 30, 30, 32, 32, 32, 32, 32, 36, 36, 36, 36, 36, 40,
            40, 40, 40, 40, 40, 42, 42, 42, 42, 44, 44, 46, 46, 48, 52, 54, 56,
            58, 60, 60, 60, 60, 64, 66, 70, 72, 72, 72, 78, 82, 88, 96};

  // Version to test direct value search.
  dyn_array<double> leibniz_pi(0, 100);
  for (int i = 0; i < leibniz_pi.size(); ++i)
    leibniz_pi[i] = (i % 2) ? ((-1.0) / (2*i + 1.0)) : (1.0 / (2*i + 1.0));
  // Version to test pointed-to value search.
  dyn_array<double*> leibniz_ptr(nullptr, 100);
  for (size_t i = 0; i < leibniz_pi.size(); ++i)
    leibniz_ptr[i] = &leibniz_pi[i];
  // Ground truth for sorted indices.
  dyn_array<size_t> leib_gtind;
  leib_gtind = { 1,  3,  5,  7,  9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31,
                33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63,
                65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 87, 89, 91, 93, 95,
                97, 99, 98, 96, 94, 92, 90, 88, 86, 84, 82, 80, 78, 76, 74, 72,
                70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40,
                38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10,  8,
                 6,  4,  2,  0};
  // Ground truth sorted order.
  dyn_array<double> leib_gt(0, 100);
  for (int i = 0; i < leibniz_pi.size(); ++i)
    leib_gt[i] = leibniz_pi[leib_gtind[i]];

  // Version to test direct value search.
  dyn_array<int> catalan_mobius;
  catalan_mobius = {       1,       -2,       -5,        0,      -42,       14,
                        -132,        0,        0,     -429,   -16796,        0,
                      -58786,      143,      132,        0,  -184756,        0,
                     -319770,        0,   134596,  -520676, -9694845,        0,
                           0,      273,        0,   756756, -1763580, -1307504,
                    -3932705,        0};
  // Version to test pointed-to value search.
  dyn_array<int*> catalan_ptr(nullptr, 32);
  for (size_t i = 0; i < catalan_mobius.size(); ++i)
    catalan_ptr[i] = &catalan_mobius[i];
  // Ground truth sorted order.
  dyn_array<int> cat_gt;
  cat_gt = {-9694845, -3932705, -1763580, -1307504,  -520676,  -319770,
             -184756,   -58786,   -16796,     -429,     -132,      -42,
                  -5,       -2,        0,        0,        0,        0,
                   0,        0,        0,        0,        0,        0,
                   0,        1,       14,      132,      143,      273,
              134596,   756756};

  // Initialize test variables to reuse during testing.
  unsigned short *eulp1, *eulp2;
  double *leibp1, *leibp2;
  int *catp1, *catp2;
  dyn_array<size_t> inds;
  dyn_array<unsigned short> eul_tot;
  dyn_array<unsigned short*> eul_ptr;
  dyn_array<double> leib_pi;
  dyn_array<double*> leib_ptr;
  dyn_array<int> cat_mob;
  dyn_array<int*> cat_ptr;
  bool all_match;

  std::cout << "Linear search on Euler's totient function" << std::endl;
  (eul_tot = euler_totient).shuffle();
  inds = eul_tot.search(16);
  TEST_EQUAL(inds.size(), 6, "Linear search should find 6 vals of 16");
  all_match = true;
  for (size_t i = 0; i < inds.size(); ++i) {
    if (eul_tot[inds[i]] != 16) {
      all_match = false;
      break;
    }
  }
  TEST_CHECK(all_match, "All vals at found indices should be 16");
  std::cout << std::endl;

  std::cout << "Linear search on ptrs to Euler's totient values" << std::endl;
  (eul_ptr = euler_ptr).shuffle();
  inds = eul_ptr.search(72);
  TEST_EQUAL(inds.size(), 3, "Linear search should find 3 ptrs with vals 72");
  all_match = true;
  for (size_t i = 0; i < inds.size(); ++i) {
    if (*eul_ptr[inds[i]] != 72) {
      all_match = false;
      break;
    }
  }
  TEST_CHECK(all_match, "All ptrs at found indices should point to 72");
  std::cout << std::endl;

  std::cout << "Binary search on Mobius-signed Catalan numbers" << std::endl;
  (cat_mob = catalan_mobius).shuffle().bubble_sort(false);
  inds = cat_mob.search(756756);
  TEST_EQUAL(inds.size(), 1, "Binary search should find 1 val of 756756");
  TEST_EQUAL(cat_mob[inds[0]], 756756, "Found val should be 756756");
  std::cout << std::endl;

  std::cout << "Binary search on ptrs to Catalan-Mobius numbers" << std::endl;
  (cat_ptr = catalan_ptr).shuffle().bubble_sort();
  inds = cat_ptr.search(0);
  std::cout << "FIRST FOUND: " << inds[0] << " " << *cat_ptr[inds[0]] << std::endl;
  TEST_EQUAL(inds.size(), 11, "Binary search should find 11 ptrs with vals 0");
  all_match = true;
  for (size_t i = 0; i < inds.size(); ++i) {
    if (*cat_ptr[inds[i]] != 0) {
      all_match = false;
      break;
    }
  }
  TEST_CHECK(all_match, "All ptrs at found indices should point to 0");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "BUBBLE SORT TESTS" << std::endl << std::endl;

  std::cout << "Bubble sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  std::cout << "Output: " << eul_tot.bubble_sort() << std::endl;
  TEST_SEQ(eul_tot, eul_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  eul_gt.reverse();
  std::cout << "Bubble sort on ptrs to Euler's totient values" << std::endl;
  std::cout << "Input: " << (eul_ptr = euler_ptr).shuffle() << std::endl;
  eulp1 = eul_ptr[eul_ptr.search(1)[0]];
  std::cout << "Output: " << eul_ptr.bubble_sort(false) << std::endl;
  eulp2 = eul_ptr[eul_ptr.search(1)[0]];
  TEST_PTR(eul_ptr, eul_gt, 100, "Ptrs should be in decreasing val order");
  TEST_CHECK(eulp1 == eulp2, "Bubble sort should be stable");
  std::cout << std::endl;
  eul_gt.reverse();

  std::cout << "Bubble sort on terms of Leibniz series for pi" << std::endl;
  std::cout << "Input: " << (leib_pi = leibniz_pi).shuffle() << std::endl;
  std::cout << "Output: " << leib_pi.bubble_sort() << std::endl;
  TEST_SEQ(leib_pi, leib_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  std::cout << "Bubble sort on ptrs to Leibniz series terms" << std::endl;
  std::cout << "Input: " << (leib_ptr = leibniz_ptr).shuffle() << std::endl;
  leibp1 = leib_ptr[leib_ptr.search(0.2)[0]];
  std::cout << "Output: " << leib_ptr.bubble_sort() << std::endl;
  leibp2 = leib_ptr[leib_ptr.search(0.2)[0]];
  TEST_PTR(leib_ptr, leib_gt, 100, "Ptrs should be in increasing val order");
  TEST_CHECK(leibp1 == leibp2, "Bubble sort should be stable");
  std::cout << std::endl;

  std::cout << "Bubble sort on Mobius-signed Catalan numbers" << std::endl;
  std::cout << "Input: " << (cat_mob = catalan_mobius).shuffle() << std::endl;
  std::cout << "Output: " << cat_mob.bubble_sort() << std::endl;
  TEST_SEQ(cat_mob, cat_gt, 32, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  cat_gt.reverse();
  std::cout << "Bubble sort on ptrs to Catalan-Mobius numbers" << std::endl;
  std::cout << "Input: " << (cat_ptr = catalan_ptr).shuffle() << std::endl;
  catp1 = cat_ptr[cat_ptr.search(0)[0]];
  std::cout << "Output: " << cat_ptr.bubble_sort(false) << std::endl;
  catp2 = cat_ptr[cat_ptr.search(0)[0]];
  TEST_PTR(cat_ptr, cat_gt, 32, "Ptrs should be in decreasing val order");
  TEST_CHECK(catp1 == catp2, "Bubble sort should be stable");
  cat_gt.reverse();

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "INSERTION SORT TESTS" << std::endl << std::endl;

  std::cout << "Insertion sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  std::cout << "Output: " << eul_tot.insertion_sort() << std::endl;
  TEST_SEQ(eul_tot, eul_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  eul_gt.reverse();
  std::cout << "Insertion sort on ptrs to Euler's totient values" << std::endl;
  std::cout << "Input: " << (eul_ptr = euler_ptr).shuffle() << std::endl;
  eulp1 = eul_ptr[eul_ptr.search(42)[0]];
  std::cout << "Output: " << eul_ptr.insertion_sort(false) << std::endl;
  eulp2 = eul_ptr[eul_ptr.search(42)[0]];
  TEST_PTR(eul_ptr, eul_gt, 100, "Ptrs should be in decreasing val order");
  TEST_CHECK(eulp1 == eulp2, "Insertion sort should be stable");
  std::cout << std::endl;
  eul_gt.reverse();

  leib_gt.reverse();
  std::cout << "Insertion sort on terms of Leibniz series for pi" << std::endl;
  std::cout << "Input: " << (leib_pi = leibniz_pi).shuffle() << std::endl;
  std::cout << "Output: " << leib_pi.insertion_sort(false) << std::endl;
  TEST_SEQ(leib_pi, leib_gt, 100, "Vals should be sorted in decreasing order");
  std::cout << std::endl;

  std::cout << "Insertion sort on ptrs to Leibniz series terms" << std::endl;
  std::cout << "Input: " << (leib_ptr = leibniz_ptr).shuffle() << std::endl;
  leibp1 = leib_ptr[leib_ptr.search(1)[0]];
  std::cout << "Output: " << leib_ptr.insertion_sort(false) << std::endl;
  leibp2 = leib_ptr[leib_ptr.search(1)[0]];
  TEST_PTR(leib_ptr, leib_gt, 100, "Ptrs should be in decreasing val order");
  TEST_CHECK(leibp1 == leibp2, "Insertion sort should be stable");
  std::cout << std::endl;
  leib_gt.reverse();

  std::cout << "Insertion sort on Mobius-signed Catalan numbers" << std::endl;
  std::cout << "Input: " << (cat_mob = catalan_mobius).shuffle() << std::endl;
  std::cout << "Output: " << cat_mob.insertion_sort() << std::endl;
  TEST_SEQ(cat_mob, cat_gt, 32, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  std::cout << "Insertion sort on ptrs to Catalan-Mobius numbers" << std::endl;
  std::cout << "Input: " << (cat_ptr = catalan_ptr).shuffle() << std::endl;
  catp1 = cat_ptr[cat_ptr.search(0)[0]];
  std::cout << "Output: " << cat_ptr.insertion_sort() << std::endl;
  catp2 = cat_ptr[cat_ptr.search(0)[0]];
  TEST_PTR(cat_ptr, cat_gt, 32, "Ptrs should be in increasing val order");
  TEST_CHECK(catp1 == catp2, "Insertion sort should be stable");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "MERGE SORT TESTS" << std::endl << std::endl;

  eul_gt.reverse();
  std::cout << "Merge sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient) << std::endl;
  std::cout << "Output: " << eul_tot.merge_sort(false) << std::endl;
  TEST_SEQ(eul_tot, eul_gt, 100, "Vals should be sorted in decreasing order");
  std::cout << std::endl;
  eul_gt.reverse();

  std::cout << "Merge sort on ptrs to Euler's totient values" << std::endl;
  std::cout << "Input: " << (eul_ptr = euler_ptr).shuffle() << std::endl;
  eulp1 = eul_ptr[eul_ptr.search(72)[0]];
  std::cout << "Output: " << eul_ptr.merge_sort() << std::endl;
  eulp2 = eul_ptr[eul_ptr.search(72)[0]];
  TEST_PTR(eul_ptr, eul_gt, 100, "Ptrs should be in increasing val order");
  TEST_CHECK(eulp1 == eulp2, "Merge sort should be stable");
  std::cout << std::endl;

  std::cout << "Merge sort on terms of Leibniz series for pi" << std::endl;
  std::cout << "Input: " << (leib_pi = leibniz_pi).shuffle() << std::endl;
  std::cout << "Output: " << leib_pi.merge_sort() << std::endl;
  TEST_SEQ(leib_pi, leib_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  leib_gt.reverse();
  std::cout << "Merge sort on ptrs to Leibniz series terms" << std::endl;
  std::cout << "Input: " << (leib_ptr = leibniz_ptr).shuffle() << std::endl;
  leibp1 = leib_ptr[leib_ptr.search(-1.0 / 3.0)[0]];
  std::cout << "Output: " << leib_ptr.merge_sort(false) << std::endl;
  leibp2 = leib_ptr[leib_ptr.search(-1.0 / 3.0)[0]];
  TEST_PTR(leib_ptr, leib_gt, 100, "Ptrs should be in decreasing val order");
  TEST_CHECK(leibp1 == leibp2, "Merge sort should be stable");
  std::cout << std::endl;
  leib_gt.reverse();

  cat_gt.reverse();
  std::cout << "Merge sort on Mobius-signed Catalan numbers" << std::endl;
  std::cout << "Input: " << (cat_mob = catalan_mobius).shuffle() << std::endl;
  std::cout << "Output: " << cat_mob.merge_sort(false) << std::endl;
  TEST_SEQ(cat_mob, cat_gt, 32, "Vals should be sorted in decreasing order");
  std::cout << std::endl;
  cat_gt.reverse();

  std::cout << "Merge sort on ptrs to Catalan-Mobius numbers" << std::endl;
  std::cout << "Input: " << (cat_ptr = catalan_ptr).shuffle() << std::endl;
  catp1 = cat_ptr[cat_ptr.search(0)[0]];
  std::cout << "Output: " << cat_ptr.merge_sort() << std::endl;
  catp2 = cat_ptr[cat_ptr.search(0)[0]];
  TEST_PTR(cat_ptr, cat_gt, 32, "Ptrs should be in increasing val order");
  TEST_CHECK(catp1 == catp2, "Insertion sort should be stable");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "SELECTION SORT TESTS" << std::endl << std::endl;

  eul_gt.reverse();
  std::cout << "Selection sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  std::cout << "Output: " << eul_tot.selection_sort(false) << std::endl;
  TEST_SEQ(eul_tot, eul_gt, 100, "Vals should be sorted in decreasing order");
  std::cout << std::endl;
  eul_gt.reverse();

  std::cout << "Selection sort on ptrs to Euler's totient values" << std::endl;
  std::cout << "Input: " << (eul_ptr = euler_ptr).shuffle() << std::endl;
  std::cout << "Output: " << eul_ptr.selection_sort() << std::endl;
  TEST_PTR(eul_ptr, eul_gt, 100, "Ptrs should be in increasing val order");
  std::cout << std::endl;

  std::cout << "Selection sort on terms of Leibniz series for pi" << std::endl;
  std::cout << "Input: " << (leib_pi = leibniz_pi).shuffle() << std::endl;
  std::cout << "Output: " << leib_pi.selection_sort() << std::endl;
  TEST_SEQ(leib_pi, leib_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  leib_gt.reverse();
  std::cout << "Selection sort on ptrs to Leibniz series terms" << std::endl;
  std::cout << "Input: " << (leib_ptr = leibniz_ptr).shuffle() << std::endl;
  std::cout << "Output: " << leib_ptr.selection_sort(false) << std::endl;
  TEST_PTR(leib_ptr, leib_gt, 100, "Ptrs should be in decreasing val order");
  std::cout << std::endl;
  leib_gt.reverse();

  cat_gt.reverse();
  std::cout << "Selection sort on Mobius-signed Catalan numbers" << std::endl;
  std::cout << "Input: " << (cat_mob = catalan_mobius).shuffle() << std::endl;
  std::cout << "Output: " << cat_mob.selection_sort(false) << std::endl;
  TEST_SEQ(cat_mob, cat_gt, 32, "Vals should be sorted in decreasing order");
  std::cout << std::endl;
  cat_gt.reverse();

  std::cout << "Selection sort on ptrs to Catalan-Mobius numbers" << std::endl;
  std::cout << "Input: " << (cat_ptr = catalan_ptr).shuffle() << std::endl;
  std::cout << "Output: " << cat_ptr.selection_sort() << std::endl;
  TEST_PTR(cat_ptr, cat_gt, 32, "Ptrs should be in increasing val order");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "QUICK SORT TESTS" << std::endl << std::endl;

  std::cout << "Quick sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  std::cout << "Output: " << eul_tot.quick_sort() << std::endl;
  TEST_SEQ(eul_tot, eul_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  std::cout << "Quick sort on ptrs to Euler's totient values" << std::endl;
  std::cout << "Input: " << (eul_ptr = euler_ptr).shuffle() << std::endl;
  std::cout << "Output: " << eul_ptr.quick_sort() << std::endl;
  TEST_PTR(eul_ptr, eul_gt, 100, "Ptrs should be in increasing val order");
  std::cout << std::endl;

  leib_gt.reverse();
  std::cout << "Quick sort on terms of Leibniz series for pi" << std::endl;
  std::cout << "Input: " << (leib_pi = leibniz_pi).shuffle() << std::endl;
  std::cout << "Output: " << leib_pi.quick_sort(false) << std::endl;
  TEST_SEQ(leib_pi, leib_gt, 100, "Vals should be sorted in decreasing order");
  std::cout << std::endl;

  std::cout << "Quick sort on ptrs to Leibniz series terms" << std::endl;
  std::cout << "Input: " << (leib_ptr = leibniz_ptr).shuffle() << std::endl;
  std::cout << "Output: " << leib_ptr.quick_sort(false) << std::endl;
  TEST_PTR(leib_ptr, leib_gt, 100, "Ptrs should be in decreasing val order");
  std::cout << std::endl;
  leib_gt.reverse();

  std::cout << "Quick sort on Mobius-signed Catalan numbers" << std::endl;
  std::cout << "Input: " << (cat_mob = catalan_mobius).shuffle() << std::endl;
  std::cout << "Output: " << cat_mob.quick_sort() << std::endl;
  TEST_SEQ(cat_mob, cat_gt, 32, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  cat_gt.reverse();
  std::cout << "Quick sort on ptrs to Catalan-Mobius numbers" << std::endl;
  std::cout << "Input: " << (cat_ptr = catalan_ptr).shuffle() << std::endl;
  std::cout << "Output: " << cat_ptr.quick_sort(false) << std::endl;
  TEST_PTR(cat_ptr, cat_gt, 32, "Ptrs should be in decreasing val order");
  cat_gt.reverse();

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "HEAP SORT TESTS" << std::endl << std::endl;

  eul_gt.reverse();
  std::cout << "Heap sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  std::cout << "Output: " << eul_tot.heap_sort(false) << std::endl;
  TEST_SEQ(eul_tot, eul_gt, 100, "Vals should be sorted in decreasing order");
  std::cout << std::endl;
  eul_gt.reverse();

  std::cout << "Heap sort on ptrs to Euler's totient values" << std::endl;
  std::cout << "Input: " << (eul_ptr = euler_ptr).shuffle() << std::endl;
  std::cout << "Output: " << eul_ptr.heap_sort() << std::endl;
  TEST_PTR(eul_ptr, eul_gt, 100, "Ptrs should be in increasing val order");
  std::cout << std::endl;

  std::cout << "Heap sort on terms of Leibniz series for pi" << std::endl;
  std::cout << "Input: " << (leib_pi = leibniz_pi).shuffle() << std::endl;
  std::cout << "Output: " << leib_pi.heap_sort() << std::endl;
  TEST_SEQ(leib_pi, leib_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  leib_gt.reverse();
  std::cout << "Heap sort on ptrs to Leibniz series terms" << std::endl;
  std::cout << "Input: " << (leib_ptr = leibniz_ptr).shuffle() << std::endl;
  std::cout << "Output: " << leib_ptr.heap_sort(false) << std::endl;
  TEST_PTR(leib_ptr, leib_gt, 100, "Ptrs should be in decreasing val order");
  std::cout << std::endl;
  leib_gt.reverse();

  cat_gt.reverse();
  std::cout << "Heap sort on Mobius-signed Catalan numbers" << std::endl;
  std::cout << "Input: " << (cat_mob = catalan_mobius).shuffle() << std::endl;
  std::cout << "Output: " << cat_mob.heap_sort(false) << std::endl;
  TEST_SEQ(cat_mob, cat_gt, 32, "Vals should be sorted in decreasing order");
  std::cout << std::endl;
  cat_gt.reverse();

  std::cout << "Heap sort on ptrs to Catalan-Mobius numbers" << std::endl;
  std::cout << "Input: " << (cat_ptr = catalan_ptr).shuffle() << std::endl;
  std::cout << "Output: " << cat_ptr.heap_sort() << std::endl;
  TEST_PTR(cat_ptr, cat_gt, 32, "Ptrs should be in increasing val order");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "COUNTING SORT TESTS" << std::endl << std::endl;

  dyn_array<unsigned short> eul_sort;
  std::cout << "Counting sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  eul_sort = eul_tot.counting_sort();
  std::cout << "Output: " << eul_sort << std::endl;
  TEST_SEQ(eul_sort, eul_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  eul_gt.reverse();
  std::cout << "Counting sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  eul_sort = eul_tot.counting_sort(false);
  std::cout << "Output: " << eul_sort << std::endl;
  TEST_SEQ(eul_sort, eul_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;
  eul_gt.reverse();

  // Magic constant sequence is the ground truth for increasing sort order.
  dyn_array<dyn_array<size_t> > magic_const, magic_cns, magic_sorted;
  magic_const = {      {0},       {1},       {5},     {1,5},     {3,4},
                     {6,5},   {1,1,1},   {1,7,5},   {2,6,0},   {3,6,9},
                   {5,0,5},   {6,7,1},   {8,7,0}, {1,1,0,5}, {1,3,7,9},
                 {1,6,9,5}, {2,0,5,6}, {2,4,6,5}, {2,9,2,5}, {3,4,3,9},
                 {4,0,1,0}, {4,6,4,1}, {5,3,3,5}, {6,0,9,5}, {6,9,2,4},
                 {7,8,2,5}, {8,8,0,1}, {9,8,5,5}};
  // Ground truth for magic constants sorted by their least significant digit.
  dyn_array<size_t> magic0_gt, magic0;
  magic0_gt = {0, 0, 0, 0, 1, 1, 1, 1, 1, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5,
               5, 5, 5, 5, 6, 9, 9, 9};

  // Landau's function sequence is the ground truth for increasing sort order.
  dyn_array<unsigned> landau, land_fn, land_sorted;
  landau = {    1,      1,      2,      3,      4,      6,      6,     12,
               15,     20,     30,     30,     60,     60,     84,    105,
              140,    210,    210,    420,    420,    420,    420,    840,
              840,   1260,   1260,   1540,   2310,   2520,   4620,   4620,
             5460,   5460,   9240,   9240,  13860,  13860,  16380,  16380,
            27720,  30030,  32760,  60060,  60060,  60060,  60060, 120120};

  std::cout << "Counting sort on digit 0 of magic constants" << std::endl;
  (magic_cns = magic_const).shuffle();
  magic_cns = magic_cns.counting_sort(0);
  for (size_t i = 0; i < magic_cns.size(); ++i)
    magic0.add(magic_cns[i][0]);
  TEST_SEQ(magic0, magic0, 28, "0 digits should be in increasing order");
  std::cout << std::endl;

  landau.reverse();
  std::cout << "Counting sort on values of Landau's function" << std::endl;
  std::cout << "Input: " << (land_fn = landau).shuffle() << std::endl;
  land_fn = land_fn.counting_sort(false);
  std::cout << "Output: " << land_fn << std::endl;
  TEST_SEQ(land_fn, landau, 48, "Vals should be sorted in decreasing order");
  landau.reverse();

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "RADIX SORT TESTS" << std::endl << std::endl;

  eul_gt.reverse();
  std::cout << "Radix sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  std::cout << "Output: " << (eul_tot = eul_tot.radix_sort(0)) << std::endl;
  TEST_SEQ(eul_tot, eul_gt, 100, "Vals should be sorted in decreasing order");
  eul_gt.reverse();

  // Look and say sequence is the ground truth for increasing sort order.
  dyn_array<dyn_array<short> > ls, look_n_say, lns_sorted;
  ls = {{1},
        {1,1},
        {2,1},
        {1,2,1,1},
        {1,1,1,2,2,1},
        {3,1,2,2,1,1},
        {1,3,1,1,2,2,2,1},
        {1,1,1,3,2,1,3,2,1,1},
        {3,1,1,3,1,2,1,1,1,3,1,2,2,1},
        {1,3,2,1,1,3,1,1,1,2,3,1,1,3,1,1,2,2,1,1},
        {1,1,1,3,1,2,2,1,1,3,3,1,1,2,1,3,2,1,1,3,2,1,2,2,2,1},
        {3,1,1,3,1,1,2,2,2,1,2,3,2,1,1,2,1,1,1,3,1,2,2,1,1,3,1,2,1,1,3,2,1,1}};

  std::cout << "Radix sort on magic constants" << std::endl;
  (magic_cns = magic_const).shuffle();
  magic_cns = magic_cns.radix_sort();
  TEST_CHECK(magic_cns == magic_const, "Should sort in increasing order.");
  std::cout << std::endl;

  ls.reverse();
  std::cout << "Radix sort on look and say array sequences" << std::endl;
  (look_n_say = ls).shuffle();
  std::cout << "Input:" << std::endl;
  for (size_t i = 0; i < look_n_say.size(); ++i)
    std::cout << look_n_say[i] << std::endl;
  look_n_say = look_n_say.radix_sort(false);
  std::cout << "Output: " << std::endl;
  for (size_t i = 0; i < look_n_say.size(); ++i)
    std::cout << look_n_say[i] << std::endl;
  TEST_CHECK(look_n_say == ls, "Arrays should be sorted in decreasing order.");
  std::cout << std::endl;
  ls.reverse();

  std::cout << "Radix sort on values of Landau's function" << std::endl;
  std::cout << "Input: " << (land_fn = landau).shuffle() << std::endl;
  std::cout << "Output: " << (land_fn = land_fn.radix_sort()) << std::endl;
  TEST_SEQ(land_fn, landau, 48, "Vals should be sorted in increasing order");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "BUCKET SORT TESTS" << std::endl << std::endl;

  eul_gt.reverse();
  std::cout << "Default bucket sort on Euler's totient function" << std::endl;
  std::cout << "Input: " << (eul_tot = euler_totient).shuffle() << std::endl;
  eul_tot = eul_tot.bucket_sort(false);
  std::cout << "Output: " << eul_tot << std::endl;
  TEST_SEQ(eul_tot, eul_gt, 100, "Vals should be sorted in decreasing order");
  std::cout << std::endl;
  eul_gt.reverse();

  std::cout << "10-bucket sort on terms of Leibniz series for pi" << std::endl;
  std::cout << "Input: " << (leib_pi = leibniz_pi).shuffle() << std::endl;
  leib_pi = leib_pi.bucket_sort(1, 10);
  std::cout << "Output: " << leib_pi << std::endl;
  TEST_SEQ(leib_pi, leib_gt, 100, "Vals should be sorted in increasing order");
  std::cout << std::endl;

  cat_gt.reverse();
  std::cout << "1-bucket sort on Mobius-signed Catalan numbers" << std::endl;
  std::cout << "Input: " << (cat_mob = catalan_mobius).shuffle() << std::endl;
  cat_mob = cat_mob.bucket_sort(0, 1);
  std::cout << "Output: " << cat_mob << std::endl;
  TEST_SEQ(cat_mob, cat_gt, 32, "Vals should be sorted in decreasing order");
  std::cout << std::endl;
  cat_gt.reverse();

  std::cout << "1000-bucket sort on values of Landau's function" << std::endl;
  std::cout << "Input: " << (land_fn = landau).shuffle() << std::endl;
  land_fn = land_fn.bucket_sort(true, 1000);
  std::cout << "Output: " << land_fn << std::endl;
  TEST_SEQ(land_fn, landau, 48, "Vals should be sorted in increasing order");

  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "INEQUALITY OPERATOR TESTS" << std::endl << std::endl;

  // Initialize test arrays.
  dyn_array<int> pan1 = {1, 0, 2, 3, 4, 5, 6, 7, 8, 9};
  dyn_array<int> pan2 = {1, 0, 2, 3, 4, 5, 6, 7, 9, 8};
  dyn_array<int> dig1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  dyn_array<int> dig2 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  dyn_array<int> dig3 = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  dyn_array<int> empt;

  dyn_array<int*> pan1_ptr(nullptr, pan1.size());
  dyn_array<int*> pan2_ptr(nullptr, pan2.size());
  dyn_array<int*> dig1_ptr(nullptr, dig1.size());
  dyn_array<int*> dig2_ptr(nullptr, dig2.size());
  dyn_array<int*> dig3_ptr(nullptr, dig3.size());
  dyn_array<int*> empt_ptr;

  // Populate pointer arrays
  for (size_t i = 0; i < pan1.size(); ++i)
    pan1_ptr[i] = &pan1[i];
  for (size_t i = 0; i < pan2.size(); ++i)
    pan2_ptr[i] = &pan2[i];
  for (size_t i = 0; i < dig1.size(); ++i)
    dig1_ptr[i] = &dig1[i];
  for (size_t i = 0; i < dig2.size(); ++i)
    dig2_ptr[i] = &dig2[i];
  for (size_t i = 0; i < dig3.size(); ++i)
    dig3_ptr[i] = &dig3[i];

  std::cout << "Testing direct-comparison inequalities" << std::endl;
  TEST_CHECK(  pan1 <  pan2,  "pan1 <  pan2 should be true");
  TEST_CHECK(  pan2 >= pan1,  "pan2 >= pan1 should be true");
  TEST_CHECK(!(dig3 >  pan1), "dig3 >  pan1 should be false");
  TEST_CHECK(  dig1 <= pan2,  "dig1 <= pan2 should be true");
  TEST_CHECK(!(dig1 >= pan1), "dig1 >= pan1 should be false");
  TEST_CHECK(!(dig1 <= dig2), "dig1 <= dig2 should be false");
  TEST_CHECK(  dig3 >  dig2,  "dig3 >  dig2 should be true");
  TEST_CHECK(!(dig3 >= dig1), "dig3 >= dig1 should be false");
  TEST_CHECK(  empt <  dig3,  "empt <  dig3 should be true");
  TEST_CHECK(  empt <= empt,  "empt <= empt should be true");
  TEST_CHECK(!(empt >  empt), "empt >  empt should be false");
  std::cout << std::endl;

  std::cout << "Testing dereferenced-comparison inequalities" << std::endl;
  TEST_CHECK(  pan1_ptr <  pan2_ptr,  "pan1_ptr <  pan2_ptr should be true");
  TEST_CHECK(  pan2_ptr >= pan1_ptr,  "pan2_ptr >= pan1_ptr should be true");
  TEST_CHECK(!(dig3_ptr >  pan1_ptr), "dig3_ptr >  pan1_ptr should be false");
  TEST_CHECK(  dig1_ptr <= pan2_ptr,  "dig1_ptr <= pan2_ptr should be true");
  TEST_CHECK(!(dig1_ptr >= pan1_ptr), "dig1_ptr >= pan1_ptr should be false");
  TEST_CHECK(!(dig1_ptr <= dig2_ptr), "dig1_ptr <= dig2_ptr should be false");
  TEST_CHECK(  dig3_ptr >  dig2_ptr,  "dig3_ptr >  dig2_ptr should be true");
  TEST_CHECK(!(dig3_ptr >= dig1_ptr), "dig3_ptr >= dig1_ptr should be false");
  TEST_CHECK(  empt_ptr <  dig3_ptr,  "empt_ptr <  dig3_ptr should be true");
  TEST_CHECK(  empt_ptr <= empt_ptr,  "empt_ptr <= empt_ptr should be true");
  TEST_CHECK(!(empt_ptr >  empt_ptr), "empt_ptr >  empt_ptr should be false");


  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "OTHER ALGORITHM TESTS" << std::endl << std::endl;

  dyn_array<int> perm_input;
  perm_input.add(0).add(2).add(4).add(6);
  std::cout << "Input array for permutation: " << perm_input << std::endl;
  dyn_array<dyn_array<int> > perms = perm_input.permutations();
  std::cout << "Computed array permutations:" << std::endl;
  TEST_EQUAL(perms.size(), 24, "There should be 24 permutations in total");
  std::cout << std::endl;

  int pgt[24][4] = {{0, 2, 4, 6}, {0, 2, 6, 4}, {0, 4, 2, 6}, {0, 4, 6, 2},
                    {0, 6, 2, 4}, {0, 6, 4, 2}, {2, 0, 4, 6}, {2, 0, 6, 4},
                    {2, 4, 0, 6}, {2, 4, 6, 0}, {2, 6, 0, 4}, {2, 6, 4, 0},
                    {4, 0, 2, 6}, {4, 0, 6, 2}, {4, 2, 0, 6}, {4, 2, 6, 0},
                    {4, 6, 0, 2}, {4, 6, 2, 0}, {6, 0, 2, 4}, {6, 0, 4, 2},
                    {6, 2, 0, 4}, {6, 2, 4, 0}, {6, 4, 0, 2}, {6, 4, 2, 0}};
  std::cout << "Checking permutations for correctness:" << std::endl;
  for (size_t p = 0; p < perms.size(); ++p)
    TEST_SEQ(perms[p], pgt[p], 4, "Permutations should match recursion order");
  std::cout << "Permutations found: " << std::endl;
  for (size_t i = 0; i < perms.size(); ++i)
    std::cout << perms[i] << std::endl;
  std::cout << std::endl;

  dyn_array<int> unique_in;
  unique_in.add(0).add(2).add(0).add(1).add(0).add(4).add(1).add(2);
  unique_in.add(9).add(8).add(7).add(8).add(6).add(0).add(5).add(5);
  dyn_array<int> unique_out = unique_in.unique();
  std::cout << "Unique method test input: " << unique_in << std::endl;
  std::cout << "Unique output: " << unique_in.unique() << std::endl;
  int gt3[9] = {0, 2, 1, 4, 9, 8, 7, 6, 5};
  TEST_EQUAL(unique_out.size(), 9, "There should be 9 unique values in total");
  TEST_SEQ(unique_out, gt3, 9, "Unique values should match expected result");

  size_t total_cnt = 0;
  size_t old_sz = unique_in.size();
  int gt4[9] = {4, 2, 2, 1, 1, 2, 1, 1, 2};
  dyn_array<size_t> counts = unique_in.unique_cnt();
  std::cout << "Ran in-place unique method returning counts:" << std::endl;
  TEST_EQUAL(unique_in.size(), counts.size(), "Sizes of results should match");
  for (size_t i = 0; i < counts.size(); ++i)
    total_cnt+=counts[i];
  TEST_EQUAL(total_cnt, old_sz, "Total count should match input size");
  TEST_CHECK(unique_in == unique_out, "Earlier output should match dyn_array");
  TEST_SEQ(counts, gt4, 9, "Unique counts should match expected results");

  test_util::print_summary();
  return 0;
}