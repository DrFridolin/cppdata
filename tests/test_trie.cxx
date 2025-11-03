#include <iostream>
#include <string>
#include <sstream>
#include "trie.h"
#include "test_util.h"

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "TRIE TESTS" << std::endl << std::endl;

  std::string vendetta;
  vendetta = "Voila! In view, a humble vaudevillian veteran, cast vicariously "
             "as both victim and villain by the vicissitudes of fate. "
             "This visage, no mere veneer of vanity, is a vestige of the vox "
             "populi, now vacant and vanquished. However, this valorous "
             "visitation of a bygone vexation stands vivified and has vowed "
             "to vanquish these venal and virulent vermin, vanguarding vice "
             "and vouchsafing the violently vicious and voracious violation "
             "of volition. The only verdict is vengeance; a vendetta, held "
             "as a votive, not in vain, for the value and veracity of such "
             "shall one day vindicate the vigilant and the virtuous.";

  char dft_gt[] = {'~', 'V', 'o', 'i', 'l', 'a', '\0', 'I', 'n', '\0', 'v',
    'i', 'e', 'w', '\0', 'c', 'a', 'r', 'i', 'o', 'u', 's', 'l', 'y', '\0',
    't', 'i', 'm', '\0', 'i', 's', 's', 'i', 't', 'u', 'd', 'e', 's', '\0',
    'o', 'u', 's', '\0', 'e', '\0', 'l', 'l', 'a', 'i', 'n', '\0', 's', 'a',
    'g', 'e', '\0', 'i', 't', 'a', 't', 'i', 'o', 'n', '\0', 'v', 'i', 'f',
    'i', 'e', 'd', '\0', 'r', 'u', 'l', 'e', 'n', 't', '\0', 't', 'u', 'o',
    'u', 's', '\0', 'o', 'l', 'e', 'n', 't', 'l', 'y', '\0', 'a', 't', 'i',
    'o', 'n', '\0', 'n', 'd', 'i', 'c', 'a', 't', 'e', '\0', 'g', 'i', 'l',
    'a', 'n', 't', '\0', 'a', 'u', 'd', 'e', 'v', 'i', 'l', 'l', 'i', 'a',
    'n', '\0', 'n', 'i', 't', 'y', '\0', 'q', 'u', 'i', 's', 'h', 'e', 'd',
    '\0', '\0', 'g', 'u', 'a', 'r', 'd', 'i', 'n', 'g', '\0', 'c', 'a', 'n',
    't', '\0', 'l', 'o', 'r', 'o', 'u', 's', '\0', 'u', 'e', '\0', 'i', 'n',
    '\0', 'e', 't', 'e', 'r', 'a', 'n', '\0', 'n', 'e', 'e', 'r', '\0', 'a',
    'l', '\0', 'g', 'e', 'a', 'n', 'c', 'e', '\0', 'd', 'e', 't', 't', 'a',
    '\0', 's', 't', 'i', 'g', 'e', '\0', 'x', 'a', 't', 'i', 'o', 'n', '\0',
    'r', 'm', 'i', 'n', '\0', 'd', 'i', 'c', 't', '\0', 'a', 'c', 'i', 't',
    'y', '\0', 'o', 'x', '\0', 'w', 'e', 'd', '\0', 'u', 'c', 'h', 's', 'a',
    'f', 'i', 'n', 'g', '\0', 'r', 'a', 'c', 'i', 'o', 'u', 's', '\0', 'l',
    'i', 't', 'i', 'o', 'n', '\0', 't', 'i', 'v', 'e', '\0', 'a', '\0', 's',
    '\0', 'n', 'd', '\0', 'h', 'u', 'm', 'b', 'l', 'e', '\0', 'a', 's', '\0',
    'e', 'l', 'd', '\0', 'c', 'a', 's', 't', '\0', 'b', 'o', 't', 'h', '\0',
    'y', '\0', 'g', 'o', 'n', 'e', '\0', 't', 'h', 'e', '\0', 's', 'e', '\0',
    'i', 's', '\0', 'o', '\0', 'o', 'f', '\0', 'n', 'l', 'y', '\0', 'e', '\0',
    'f', 'a', 't', 'e', '\0', 'o', 'r', '\0', 'T', 'h', 'i', 's', '\0', 'e',
    '\0', 'n', 'o', '\0', 'w', '\0', 't', '\0', 'm', 'e', 'r', 'e', '\0', 'i',
    's', '\0', 'n', '\0', 'p', 'o', 'p', 'u', 'l', 'i', '\0', 'H', 'o', 'w',
    'e', 'v', 'e', 'r', '\0', 's', 't', 'a', 'n', 'd', 's', '\0', 'u', 'c',
    'h', '\0', 'h', 'a', 'l', 'l', '\0', 'd', 'a', 'y', '\0'};

  std::cout << vendetta << std::endl << std::endl;
  // Define the lambda predicate that checks for punctuation.
  // std::ispunct takes an int, so we cast the character to unsigned char for safety.
  auto is_punc = [](unsigned char c) {return std::ispunct(c);};
  // Apply the erase-remove-if idiom using the lambda.
  vendetta.erase(std::remove_if(vendetta.begin(), vendetta.end(), is_punc),
                 vendetta.end());
  std::cout << "Stripped punctuation marks." << std::endl << std::endl;
  std::cout << vendetta << std::endl << std::endl;

  // Create the trie.
  trie* tr = trie::create();
  dyn_array<std::string> str_arr;
  // Keep track of number of unique strings using a hash table during insertion.
  hash_table<std::string, unsigned> str_table;
  std::stringstream sstr(vendetta);
  std::string word;
  unsigned word_count = 0;
  while (sstr >> word) {
    tr->insert(word);
    if (str_table.contains(word)) {
      unsigned count = str_table[word];
      str_table.insert(word, count+1);
    }
    else {
      str_table.insert(word, 1);
    }
    word_count++;
  }
  std::cout << "Inserted text to a trie and a hash table:" << std::endl;
  TEST_EQUAL(word_count, 99, "There should be 99 insertions");
  size_t hash_keys = str_table.keys().size();
  TEST_EQUAL(hash_keys, 76, "There should be 76 unique words");
  tr->get_strings(str_arr);
  size_t trie_words = str_arr.size();
  TEST_EQUAL(trie_words, hash_keys, "Trie word count should match key count");
  TEST_EQUAL(tr->size(), 386, "Trie should have a total of 386 nodes");
  std::cout << std::endl;

  unsigned ht_word_count = 0;
  for (unsigned i = 0; i < str_table.vals().size(); ++i)
    ht_word_count += *str_table.vals()[i];
  std::cout << "Computed total word count from hash table values" << std::endl;
  TEST_EQUAL(ht_word_count, word_count, "Hash total should match insertions");
  std::cout << std::endl;

  std::cout << "Depth first traversal on the trie: " << std::endl;
  gn_ptrs<char> dft = tr->depth_first_traverse(&std::cout);
  std::cout << std::endl << std::endl;
  std::cout << "Checking the traversal result: " << std::endl;
  TEST_PTR_SEQ(dft, dft_gt, 386, "DFT on trie should match the ground truth");
  std::cout << std::endl;

  std::cout << "Strings in trie: " << tr << std::endl;
  std::cout << "Strings in trie in sorted order: " << *tr << std::endl;

  std::string query_str("vaudevillian");
  dyn_array<const trie*> path = tr->search(query_str);
  std::cout << "Searched trie for \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(path.size(), 12, "Path size should match character count");
  std::string found;
  for (unsigned i = 0; i < path.size(); ++i)
    found.push_back(path[i]->val());
  TEST_EQUAL(found, query_str, "Path traced should match query string");
  std::cout << std::endl;

  query_str = "a";
  path = tr->search(query_str);
  std::cout << "Searched trie for \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(path.size(), 1, "Path size should match character count");
  found.clear();
  for (unsigned i = 0; i < path.size(); ++i)
    found.push_back(path[i]->val());
  TEST_EQUAL(found, query_str, "Path traced should match query string");
  std::cout << std::endl;

  query_str = "moore";
  path = tr->search(query_str);
  std::cout << "Searched trie for \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(path.size(), 0, "Trie should not contain \"vic\" as a word");
  std::cout << std::endl;

  query_str = "vic";
  path = tr->search(query_str);
  std::cout << "Searched trie for \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(path.size(), 0, "Trie should not contain \"vic\" as a word");
  std::cout << std::endl;

  query_str = "vic";
  path = tr->find_prefix(query_str);
  std::cout << "Searched trie for prefix \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(path.size(), 3, "Path size should match prefix character count");
  found.clear();
  for (unsigned i = 0; i < path.size(); ++i)
    found.push_back(path[i]->val());
  TEST_EQUAL(found, query_str, "Path traced should match query prefix");
  std::cout << std::endl;

  query_str = "vendetta";
  path = tr->find_prefix(query_str);
  std::cout << "Searched trie for prefix \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(path.size(), 8, "Path size should match prefix character count");
  found.clear();
  for (unsigned i = 0; i < path.size(); ++i)
    found.push_back(path[i]->val());
  TEST_EQUAL(found, query_str, "Path traced should match query prefix");
  std::cout << std::endl;

  query_str = "vic";
  str_arr = tr->autocomplete(query_str);
  std::cout << "Found autocompletes for \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(str_arr.size(), 5, "There should be 5 results for \"vic\"");
  TEST_EQUAL(str_arr[0], "vicariously",  "#1 should be \"vicariously\"");
  TEST_EQUAL(str_arr[1], "victim",       "#2 should be \"victim\"");
  TEST_EQUAL(str_arr[2], "vicissitudes", "#3 should be \"vicissitudes\"");
  TEST_EQUAL(str_arr[3], "vicious",      "#4 should be \"vicious\"");
  TEST_EQUAL(str_arr[4], "vice",         "#5 should be \"vice\"");
  std::cout << "Autocompletes of \"vic\": " << std::endl;
  for (unsigned i = 0; i < str_arr.size(); ++i)
    std::cout << str_arr[i] << std::endl;
  std::cout << std::endl;

  query_str = "vermin";
  str_arr = tr->autocomplete(query_str);
  std::cout << "Found autocompletes for \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(str_arr.size(), 1, "There should be 1 result for \"vermin\"");
  TEST_EQUAL(str_arr[0], "vermin",  "It should be \"vermin\"");
  std::cout << "Autocompletes of \"vermin\": " << std::endl;
  for (unsigned i = 0; i < str_arr.size(); ++i)
    std::cout << str_arr[i] << std::endl;
  std::cout << std::endl;

  query_str = "a";
  str_arr = tr->autocomplete(query_str);
  std::cout << "Found autocompletes for \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(str_arr.size(), 3, "There should be 3 results for \"a\"");
  TEST_EQUAL(str_arr[0], "a",   "#1 should be \"a\"");
  TEST_EQUAL(str_arr[1], "as",  "#2 should be \"as\"");
  TEST_EQUAL(str_arr[2], "and", "#3 should be \"and\"");
  std::cout << "Autocompletes of \"a\": " << std::endl;
  for (unsigned i = 0; i < str_arr.size(); ++i)
    std::cout << str_arr[i] << std::endl;
  std::cout << std::endl;

  query_str = "v";
  str_arr = tr->autocomplete(query_str);
  std::cout << "Found autocompletes for \"" << query_str << "\"" << std::endl;
  TEST_EQUAL(str_arr.size(), 41, "There should be 41 results for \"v\"");
  std::cout << "Autocompletes of \"v\": " << std::endl;
  for (unsigned i = 0; i < str_arr.size(); ++i)
    std::cout << str_arr[i] << std::endl;
  std::cout << std::endl;

  bool check;
  str_arr.clear();
  trie* tr_clone = tr->clone();
  dyn_array<std::string> str_cln;
  tr->get_strings(str_arr);
  tr_clone->get_strings(str_cln);
  size_t clone_sz = tr_clone->size();
  check = (str_arr.heap_sort() == str_cln.heap_sort());
  std::cout << "Cloned the trie:" << std::endl;
  TEST_EQUAL(tr->size(), tr_clone->size(), "Clone should have the same size");
  TEST_CHECK(*tr == *tr_clone, "Clone should be equal to the original trie");
  TEST_CHECK(check, "Clone should have the same string content as original");
  std::cout << std::endl;

  check = true;
  str_arr.clear();
  tr->get_strings(str_arr);
  size_t prev = str_arr.size();
  for (unsigned i = 0; i < str_arr.size(); ++i) {
    dyn_array<std::string> content;
    tr->remove(str_arr[i]);
    tr->get_strings(content);
    size_t cur = content.size();
    if (cur != prev - 1)
      check = false;
    prev = content.size();
  }
  std::cout << "Removed all strings from first trie one by one" << std::endl;
  TEST_CHECK(check, "Each removal should have decremented word count by 1");
  TEST_EQUAL(tr->size(), 1, "Trie should only have root after removals");
  TEST_EQUAL(tr_clone->size(), clone_sz, "Clone should maintain its size");
  std::cout << std::endl;

  trie* tr_ins = trie::create();
  *tr_ins = {"The", "universe", "(which", "others", "call", "the", "Library)",
             "is", "composed", "of", "an", "indefinite", "and", "perhaps",
             "infinite", "number", "of", "hexagonal", "galleries,", "with",
             "vast", "air", "shafts", "between,", "surrounded", "by", "very",
             "low", "railings."};

  std::string borges;
  borges = "The universe (which others call the Library) is composed of an "
           "indefinite and perhaps infinite number of hexagonal galleries, "
           "with vast air shafts between, surrounded by very low railings. "
           "From any of the hexagons one can see, interminably, the upper "
           "and lower floors. The distribution of the galleries is invariable."
           "Twenty shelves, five long shelves per side, cover all the sides "
           "except two; their height, which is the distance from floor to "
           "ceiling, scarcely exceeds that of a normal bookcase. "
           "One of the free sides leads to a narrow hallway which opens onto "
           "another gallery, identical to the first and to all the rest. "
           "To the left and right of the hallway there are two very small "
           "closets. In the first, one may sleep standing up; in the other, "
           "satisfy one's fecal necessities. Also through here passes a "
           "spiral stairway, which sinks abysmally and soars upwards to "
           "remote distances. In the hallway there is a mirror which "
           "faithfully duplicates all appearances. Men usually infer from "
           "this mirror that the Library is not infinite (if it were, why "
           "this illusory duplication?); I prefer to dream that its polished "
           "surfaces represent and promise the infinite ... Light is provided "
           "by some spherical fruit which bear the name of lamps. "
           "There are two, transversally placed, in each hexagon. "
           "The light they emit is insufficient, incessant.";
  std::cout << borges << std::endl << std::endl;

  dyn_array<std::string> words;
  sstr.str(borges);
  sstr.clear();

  for (size_t i = 0; i < 29; ++i) {
    sstr >> word;
    words.add(word);
  }
  while (sstr >> word) {
    tr_ins->insert(word);
    words.add(word);
  }

  trie* tr_borges = trie::create(borges);
  trie* tr_kerouac = trie::create(words);
  std::cout << "Created a trie from a string and another from a dyn_array:";
  std::cout << std::endl;
  check = (*tr_borges == *tr_ins);
  TEST_CHECK(check, "Trie from string should match one-by-one insertion");
  check = (*tr_kerouac == *tr_ins);
  TEST_CHECK(check, "Trie from dyn_array should match one-by-one insertion");
  check = (*tr_borges != *tr_clone);
  TEST_CHECK(check, "Borges trie should be different from the vendetta trie");
  std::cout << std::endl;

  std::string kerouac;
  kerouac = "So in America when the sun goes down and I sit on the old "
            "broken-down river pier watching the long, long skies over "
            "New Jersey and sense all that raw land that rolls in one "
            "unbelievable huge bulge over to the West Coast, and all that "
            "road going, all the people dreaming in the immensity of it, "
            "and in Iowa I know by now the children must be crying in the "
            "land where they let the children cry, and tonight the stars'll "
            "be out, and don't you know that God is Pooh Bear? the evening "
            "star must be drooping and shedding her sparkler dims on the "
            "prairie, which is just before the coming of complete night that "
            "blesses the earth, darkens all rivers, cups the peaks and folds "
            "the final shore in, and nobody, nobody knows what's going to "
            "happen to anybody besides the forlorn rags of growing old, "
            "I think of Dean Moriarty, I even think of Old Dean Moriarty "
            "the father we never found, I think of Dean Moriarty.";
  std::cout << kerouac << std::endl << std::endl;

  tr_ins->clear();
  words.clear();
  sstr.str(kerouac);
  sstr.clear();
  while (sstr >> word) {
    tr_ins->insert(word);
    words.add(word);
  }

  *tr_kerouac = kerouac;
  std::cout << "Assigned to Kerouac trie from string:" << std::endl;
  check = (*tr_kerouac == *tr_ins);
  TEST_CHECK(check, "Trie from string should match one-by-one insertion");
  check = (*tr_kerouac != *tr_clone);
  TEST_CHECK(check, "Trie should not match the Borges clone from earlier");
  std::cout << std::endl;

  *tr_clone = *tr_kerouac;
  std::cout << "Assigned Kerouac trie to Borges trie:" << std::endl;
  check = (*tr_clone == *tr_kerouac);
  TEST_CHECK(check, "Borges trie should now be a Kerouac trie");
  std::cout << std::endl;

  delete tr;
  delete tr_clone;
  delete tr_ins;
  delete tr_borges;
  delete tr_kerouac;
  test_util::print_summary();
  return 0;
}