#include <iostream>
#include <string>
#include "graph.h"
#include "dyn_array.h"
#include "test_util.h"

int main()
{
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "WEIGHTED GRAPH TESTS" << std::endl << std::endl;

  bool check;
  dyn_array<int> labels, path, path_gt;
  dyn_array<size_t> inds, ind_gt;

  dyn_array<std::string> names;
  names = {"Canada", "Norway", "Denmark", "Sweden", "Finland", "UK", "Benelux",
           "France", "Spain/Portugal", "Italy", "West Germany", "Austria",
           "Greece", "Turkey", "East Germany", "Poland", "Czechoslovakia",
           "Hungary", "Yugoslavia", "Romania", "Bulgaria", "Thailand",
           "Vietnam", "Laos/Cambodia", "Indonesia", "Philippines", "Burma",
           "Malaysia", "India", "Pakistan", "Afghanistan", "Japan",
           "South Korea", "North Korea", "Taiwan", "Australia", "Israel",
           "Lebanon", "Syria", "Jordan", "Iraq", "Iran", "Saudi Arabia",
           "Gulf States", "Egypt", "Libya", "Sudan", "Ivory Coast", "Nigeria",
           "Cameroon", "Zaire", "Angola", "Zimbabwe", "Botswana",
           "South Africa", "SE African States", "Kenya", "Ethiopia", "Somalia",
           "Saharan States", "West African States", "Morocco", "Algeria",
           "Tunisia", "Mexico", "Guatemala", "El Salvador", "Honduras",
           "Nicaragua", "Costa Rica", "Panama", "Cuba", "Haiti",
           "Dominican Republic", "Venezuela", "Colombia", "Ecuador", "Peru",
           "Bolivia", "Brazil", "Argentina", "Chile", "Paraguay", "Uruguay",
           "USSR", "USA"};

  enum States {// --------------------------EUROPE----------------------------
               CANADA, NORWAY, DENMARK, SWEDEN, FINLAND, UK, BENELUX, FRANCE,
               SPAIN_PORTUGAL, ITALY, WEST_GERMANY, AUSTRIA, GREECE, TURKEY,
               EAST_GERMANY, POLAND, CZECHOSLOVAKIA, HUNGARY, YUGOSLAVIA,
               ROMANIA, BULGARIA,
               // ---------------------------ASIA-----------------------------
               THAILAND, VIETNAM, LAOS_CAMBODIA, INDONESIA, PHILIPPINES, BURMA,
               MALAYSIA, INDIA, PAKISTAN, AFGHANISTAN, JAPAN, SOUTH_KOREA,
               NORTH_KOREA, TAIWAN, AUSTRALIA,
               // ------------------------MIDDLE EAST-------------------------
               ISRAEL, LEBANON, SYRIA, JORDAN, IRAQ, IRAN, SAUDI_ARABIA,
               GULF_STATES, EGYPT, LIBYA,
               // --------------------------AFRICA----------------------------
               SUDAN, IVORY_COAST, NIGERIA, CAMEROON, ZAIRE, ANGOLA, ZIMBABWE,
               BOTSWANA, SOUTH_AFRICA, SE_AFRICAN_STATES, KENYA, ETHIOPIA,
               SOMALIA, SAHARAN_STATES, WEST_AFRICAN_STATES, MOROCCO, ALGERIA,
               TUNISIA,
               // ----------------------CENTRAL AMERICA-----------------------
               MEXICO, GUATEMALA, EL_SALVADOR, HONDURAS, NICARAGUA, COSTA_RICA,
               PANAMA, CUBA, HAITI, DOMINICAN_REPUBLIC,
               // -----------------------SOUTH AMERICA------------------------
               VENEZUELA, COLOMBIA, ECUADOR, PERU, BOLIVIA, BRAZIL, ARGENTINA,
               CHILE, PARAGUAY, URUGUAY,
               // --------------------------PLAYERS---------------------------
               USSR, USA};

  dyn_array<int> stabilities = {
    4, 4, 3, 4, 4, 5, 3, 3, 2, 2, 4, 4, 2, 2, 3, 3, 3, 3, 3, 3, 3, // Europe
    2, 1, 1, 1, 2, 2, 2, 3, 2, 2, 4, 3, 3, 3, 4,           // Asia
    4, 1, 2, 2, 3, 2, 3, 3, 2, 2,                          // Middle East
    1, 2, 1, 1, 1, 1, 1, 2, 3, 1, 2, 1, 2, 1, 2, 3, 2, 2,  // Africa
    2, 1, 1, 2, 1, 3, 2, 3, 1, 1,                          // Central America
    2, 1, 2, 2, 2, 2, 2, 3, 2, 2,                          // South America
    0, // USSR uses 0 node value as a unique identifier.
    -1 // USA uses -1 node value as a unique identifier.
  };

  dyn_array<dyn_array<int> > asym = {
    // ------------------------------EUROPE------------------------------------
    {UK, CANADA, 6},   {BENELUX, UK, 11},  {FRANCE, UK, 11},  {NORWAY, UK, 11},
    {CANADA, UK, 11},  {UK, BENELUX, 3},   {UK, FRANCE, 3},    {UK, NORWAY, 4},
    {FRANCE, SPAIN_PORTUGAL, 2}, {WEST_GERMANY, FRANCE, 3}, {ITALY, FRANCE, 3},
    {SPAIN_PORTUGAL, FRANCE, 3}, {FRANCE, WEST_GERMANY, 9}, {FRANCE, ITALY, 6},
    {WEST_GERMANY, BENELUX, 3},  {YUGOSLAVIA, GREECE, 2},  {ITALY, AUSTRIA, 5},
    {BENELUX, WEST_GERMANY, 9},  {GREECE, YUGOSLAVIA, 3},  {AUSTRIA, ITALY, 6},
    {WEST_GERMANY, DENMARK, 3},  {TURKEY, BULGARIA, 3},   {FINLAND, SWEDEN, 4},
    {DENMARK, WEST_GERMANY, 9},  {BULGARIA, TURKEY, 2},   {SWEDEN, FINLAND, 5},
    {EAST_GERMANY, AUSTRIA, 5},  {ITALY, YUGOSLAVIA, 3},  {ROMANIA, TURKEY, 2},
    {AUSTRIA, EAST_GERMANY, 9},  {YUGOSLAVIA, ITALY, 6},  {TURKEY, ROMANIA, 3},
    {WEST_GERMANY, AUSTRIA, 5},  {DENMARK, SWEDEN, 4},   {GREECE, BULGARIA, 3},
    {AUSTRIA, WEST_GERMANY, 9},  {SWEDEN, DENMARK, 3},   {BULGARIA, GREECE, 2},
    {POLAND, CZECHOSLOVAKIA, 3}, {ITALY, GREECE, 2},     {HUNGARY, AUSTRIA, 5},
    {CZECHOSLOVAKIA, POLAND, 9}, {GREECE, ITALY, 6},     {AUSTRIA, HUNGARY, 3},
    {EAST_GERMANY, CZECHOSLOVAKIA, 3},              {ITALY, SPAIN_PORTUGAL, 2},
    {CZECHOSLOVAKIA, EAST_GERMANY, 9},              {SPAIN_PORTUGAL, ITALY, 6},
    // -------------------------------ASIA-------------------------------------
    {AUSTRALIA, MALAYSIA, 2}, {JAPAN, SOUTH_KOREA, 4},  {THAILAND, VIETNAM, 1},
    {MALAYSIA, AUSTRALIA, 9}, {SOUTH_KOREA, JAPAN, 5},  {VIETNAM, THAILAND, 2},
    {SOUTH_KOREA, TAIWAN, 3}, {INDIA, PAKISTAN, 2},  {BURMA, LAOS_CAMBODIA, 1},
    {TAIWAN, SOUTH_KOREA, 4}, {PAKISTAN, INDIA, 3},  {LAOS_CAMBODIA, BURMA, 2},
    {MALAYSIA, INDONESIA, 1}, {JAPAN, TAIWAN, 3},  {PHILIPPINES, INDONESIA, 1},
    {INDONESIA, MALAYSIA, 2}, {TAIWAN, JAPAN, 5},  {INDONESIA, PHILIPPINES, 3},
    {PHILIPPINES, JAPAN, 5},  {BURMA, INDIA, 3}, {SOUTH_KOREA, NORTH_KOREA, 7},
    {JAPAN, PHILIPPINES, 3},  {INDIA, BURMA, 2}, {NORTH_KOREA, SOUTH_KOREA, 4},
    {LAOS_CAMBODIA, THAILAND, 2},                 {THAILAND, LAOS_CAMBODIA, 1},
    // ---------------------------MIDDLE EAST----------------------------------
    {JORDAN, IRAQ, 4},  {IRAQ, JORDAN, 2},   {IRAN, IRAQ, 4},  {IRAQ, IRAN, 3},
    {LEBANON, JORDAN, 2},     {ISRAEL, EGYPT, 2},    {JORDAN, SAUDI_ARABIA, 3},
    {JORDAN, LEBANON, 1},     {EGYPT, ISRAEL, 5},    {SAUDI_ARABIA, JORDAN, 2},
    {ISRAEL, LEBANON, 1},     {ISRAEL, SYRIA, 3},      {IRAQ, SAUDI_ARABIA, 3},
    {LEBANON, ISRAEL, 5},     {SYRIA, ISRAEL, 5},      {SAUDI_ARABIA, IRAQ, 4},
    {ISRAEL, JORDAN, 2},      {SYRIA, LEBANON, 1},      {IRAQ, GULF_STATES, 3},
    {JORDAN, ISRAEL, 5},      {LEBANON, SYRIA, 3},      {GULF_STATES, IRAQ, 4},
    // ------------------------------AFRICA------------------------------------
    {ZIMBABWE, BOTSWANA, 2},  {BOTSWANA, ZIMBABWE, 1},   {ALGERIA, MOROCCO, 3},
    {ANGOLA, BOTSWANA, 2},    {BOTSWANA, ANGOLA, 1},     {MOROCCO, ALGERIA, 2},
    {ETHIOPIA, SOMALIA, 2},                             {SOMALIA, ETHIOPIA, 1},
    {NIGERIA, IVORY_COAST, 2},                       {IVORY_COAST, NIGERIA, 1},
    {SOUTH_AFRICA, BOTSWANA, 2},                   {BOTSWANA, SOUTH_AFRICA, 4},
    {SOUTH_AFRICA, ANGOLA, 1},                       {ANGOLA, SOUTH_AFRICA, 4},
    {ALGERIA, SAHARAN_STATES, 1},                 {SAHARAN_STATES, ALGERIA, 2},
    {MOROCCO, WEST_AFRICAN_STATES, 2},       {WEST_AFRICAN_STATES, MOROCCO, 3},
    {SE_AFRICAN_STATES, KENYA, 2},               {KENYA, SE_AFRICAN_STATES, 1},
    // -------------------------CENTRAL AMERICA--------------------------------
    {NICARAGUA, COSTA_RICA, 3}, {HONDURAS, GUATEMALA, 1}, {NICARAGUA, CUBA, 3},
    {COSTA_RICA, NICARAGUA, 1}, {GUATEMALA, HONDURAS, 2}, {CUBA, NICARAGUA, 1},
    {EL_SALVADOR, HONDURAS, 2}, {HONDURAS, NICARAGUA, 1},     {HAITI, CUBA, 3},
    {HONDURAS, EL_SALVADOR, 1}, {NICARAGUA, HONDURAS, 2},     {CUBA, HAITI, 1},
    {COSTA_RICA, HONDURAS, 2},                          {MEXICO, GUATEMALA, 1},
    {HONDURAS, COSTA_RICA, 3},                          {GUATEMALA, MEXICO, 2},
    // --------------------------SOUTH AMERICA---------------------------------
    {COLOMBIA, VENEZUELA, 2},   {ARGENTINA, CHILE, 3},        {PERU, CHILE, 3},
    {VENEZUELA, COLOMBIA, 1},   {CHILE, ARGENTINA, 2},        {CHILE, PERU, 2},
    {ECUADOR, COLOMBIA, 1},                             {COLOMBIA, ECUADOR, 2},
    /* ------------------------------------------------------------------ */};

  dyn_array<dyn_array<int> > sym = {
    // ------------------------------EUROPE------------------------------------
    {EAST_GERMANY, POLAND, 9},  {HUNGARY, YUGOSLAVIA, 3},  {SWEDEN, NORWAY, 4},
    {ROMANIA, YUGOSLAVIA, 3},   {GREECE, TURKEY, 2},     {ROMANIA, HUNGARY, 3},
    {WEST_GERMANY, EAST_GERMANY, 9},              {HUNGARY, CZECHOSLOVAKIA, 3},
    // -------------------------------ASIA-------------------------------------
    {AFGHANISTAN, PAKISTAN, 2},                        {THAILAND, MALAYSIA, 2},
    {VIETNAM, LAOS_CAMBODIA, 1},
    // ---------------------------MIDDLE EAST----------------------------------
    {SAUDI_ARABIA, GULF_STATES, 3},                          {EGYPT, LIBYA, 2},
    // ------------------------------AFRICA------------------------------------
    {IVORY_COAST, WEST_AFRICAN_STATES, 2},    {SE_AFRICAN_STATES, ZIMBABWE, 1},
    {NIGERIA, SAHARAN_STATES, 1},                         {ZAIRE, CAMEROON, 1},
    {CAMEROON, NIGERIA, 1},       {SUDAN, ETHIOPIA, 1},   {ZAIRE, ZIMBABWE, 1},
    {ALGERIA, TUNISIA, 2},        {KENYA, SOMALIA, 2},      {ANGOLA, ZAIRE, 1},
    // -------------------------CENTRAL AMERICA--------------------------------
    {HAITI, DOMINICAN_REPUBLIC, 1},                {GUATEMALA, EL_SALVADOR, 1},
    {COSTA_RICA, PANAMA, 3},
    // --------------------------SOUTH AMERICA---------------------------------
    {VENEZUELA, BRAZIL, 2},      {PARAGUAY, ARGENTINA, 2},  {PERU, BOLIVIA, 2},
    {BOLIVIA, PARAGUAY, 2},      {PARAGUAY, URUGUAY, 2},    {ECUADOR, PERU, 2},
    {URUGUAY, ARGENTINA, 2},                              {BRAZIL, URUGUAY, 2},
    /* ------------------------------------------------------------------ */};

  graph<int> twilight_struggle = stabilities;
  for (size_t i = 0; i < sym.size(); ++i)
    twilight_struggle.add_edgew_undir(sym[i][0], sym[i][1], sym[i][2]);
  for (size_t i = 0; i < asym.size(); ++i)
    twilight_struggle.add_edgew(asym[i][0], asym[i][1], asym[i][2]);

  std::cout << "Twilight Struggle map with split continents:" << std::endl;
  TEST_EQUAL(twilight_struggle.size(), 86, "Map should have 86 countries");
  check = ((twilight_struggle.num_edges() / 2) == 104);
  TEST_CHECK(check, "Map should have 104 bidirectional links");
  check = (twilight_struggle.find_roots().size() == 2);
  TEST_CHECK(check, "USA and USSR should be the only roots");
  TEST_CHECK(twilight_struggle.has_cycle(), "Cycle should be detected");
  check = (twilight_struggle.kahn().size() == 2);
  TEST_CHECK(check, "Kahn should stop after processing USA and USSR");
  std::cout << std::endl;

  ind_gt = {74, 79, 83, 80};
  inds = twilight_struggle.bidirectional_search(VENEZUELA, ARGENTINA);
  path = twilight_struggle.dijkstra(VENEZUELA)[ARGENTINA];
  std::cout << "Shortest path from Venezuela to Argentina: ";
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "Min-cost path from Venezuela to Argentina: ";
  for (size_t i = 0; i < path.size(); ++i)
    std::cout << names[path[i]] << ((i < path.size()-1) ? " -> " : "");
  std::cout << std::endl;
  check = (inds.last() == ARGENTINA) && (path.last() == ARGENTINA);
  TEST_CHECK(check, "A path from Venezuela to Argentina should be found");
  check = (inds.size() == 4);
  TEST_CHECK(check, "Bidirectional search shortest path size should be 4");
  TEST_SEQ(inds, ind_gt, 4, "Bidirectional search path should match gt");
  TEST_SEQ(path, ind_gt, 4, "Dijkstra min-cost path should match shortest");
  std::cout << std::endl;

  ind_gt = {62, 59, 48, 49, 50, 51, 54};
  inds = twilight_struggle.bidirectional_search(ALGERIA, SOUTH_AFRICA);
  path = twilight_struggle.dijkstra(ALGERIA)[SOUTH_AFRICA];
  std::cout << "Shortest path from Algeria to South Africa: ";
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "Min-cost path from Algeria to South Africa: ";
  for (size_t i = 0; i < path.size(); ++i)
    std::cout << names[path[i]] << ((i < path.size()-1) ? " -> " : "");
  std::cout << std::endl;
  check = (inds.last() == SOUTH_AFRICA) && (path.last() == SOUTH_AFRICA);
  TEST_CHECK(check, "A path from Algeria to South Africa should be found");
  check = (inds.size() == 7);
  TEST_CHECK(check, "Bidirectional search shortest path size should be 7");
  TEST_SEQ(inds, ind_gt, 7, "Bidirectional search path should match gt");
  TEST_SEQ(path, ind_gt, 7, "Dijkstra min-cost path should match shortest");
  std::cout << std::endl;

  inds = twilight_struggle.bidirectional_search(TURKEY, PAKISTAN);
  path = twilight_struggle.dijkstra(TURKEY)[PAKISTAN];
  std::cout << "Bidirectional search path from Turkey to Pakistan: ";
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
  std::cout << std::endl;
  check = (inds.last() != PAKISTAN) && (path[0] == -1) && (path.size() == 1);
  TEST_CHECK(check, "A path from Turkey to Pakistan should not be found");
  std::cout << std::endl;

  ind_gt  = {10, 14, 16};
  path_gt = {10, 11, 17, 16};
  inds = twilight_struggle.bidirectional_search(WEST_GERMANY, CZECHOSLOVAKIA);
  path = twilight_struggle.dijkstra(WEST_GERMANY)[CZECHOSLOVAKIA];
  std::cout << "Shortest path from West Germany to Czechoslovakia: ";
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "Min-cost path from West Germany to Czechoslovakia: ";
  for (size_t i = 0; i < path.size(); ++i)
    std::cout << names[path[i]] << ((i < path.size()-1) ? " -> " : "");
  std::cout << std::endl;
  check = (inds.last() == CZECHOSLOVAKIA) && (path.last() == CZECHOSLOVAKIA);
  TEST_CHECK(check, "Both BDS and Dijkstra should find a valid path");
  check = (inds.size() != path.size());
  TEST_CHECK(check, "Min-cost and shortest paths should be different");
  TEST_SEQ(inds, ind_gt, 3, "Bidirectional search path should match gt");
  TEST_SEQ(path, path_gt, 4, "Dijkstra min-cost path should match gt");
  std::cout << std::endl;

  dyn_array<int> inf_to = twilight_struggle.shortest_dists(FRANCE);
  std::cout << "Ran Dijkstra's algorithm for distances from France. ";
  std::cout << "Shortest distance to:" << std::endl;
  for (size_t i = 0; i < inf_to.size(); ++i)
    if (inf_to[i] != -1)
      std::cout << " " << names[i] << ": " << inf_to[i] << std::endl;
  TEST_EQUAL(inf_to[UK], 11, "UK control from France should need 11 infl");
  TEST_EQUAL(inf_to[CANADA], 17, "Canada control should need 17 infl");
  TEST_EQUAL(inf_to[SPAIN_PORTUGAL], 2, "Spain/Portugal should need 2 infl");
  TEST_EQUAL(inf_to[GREECE], 8, "Greece control should need 8 infl");
  TEST_EQUAL(inf_to[NORWAY], 15, "Norway control should need 15 infl");
  std::cout << std::endl;

  inf_to = twilight_struggle.shortest_dists(NORTH_KOREA);
  std::cout << "Ran Dijkstra's algorithm for distances from North Korea. ";
  std::cout << "Shortest distance to:" << std::endl;
  for (size_t i = 0; i < inf_to.size(); ++i)
    if (inf_to[i] != -1)
      std::cout << " " << names[i] << ": " << inf_to[i] << std::endl;
  TEST_EQUAL(inf_to[SOUTH_KOREA], 4, "Control of South Korea needs 4 infl");
  TEST_EQUAL(inf_to[JAPAN], 9, "Japan control from North Korea needs 9 infl");
  TEST_EQUAL(inf_to[PHILIPPINES], 12, "Phillippines control needs 12 infl");
  TEST_EQUAL(inf_to[THAILAND], 17, "Thailand control needs 17 infl");
  TEST_EQUAL(inf_to[AUSTRALIA], 24, "Australia control needs 24 infl");
  TEST_EQUAL(inf_to[INDIA], 23, "India control needs 23 inf");
  TEST_EQUAL(inf_to[PAKISTAN], 25, "Asia control from N.Korea needs 25 infl");
  std::cout << std::endl;

  inf_to = twilight_struggle.shortest_dists(ISRAEL);
  std::cout << "Ran Dijkstra's algorithm for distances from Israel. ";
  std::cout << "Shortest distance to:" << std::endl;
  for (size_t i = 0; i < inf_to.size(); ++i)
    if (inf_to[i] != -1)
      std::cout << " " << names[i] << ": " << inf_to[i] << std::endl;
  // For USSR: Corresponds to losing Iraq access and USA having 1 inf there.
  // For USA: Corresponds to Iran influence flipping to USSR after a coup.
  TEST_EQUAL(inf_to[IRAN], 9, "Iran control from Isral should need 9 infl");
  TEST_EQUAL(inf_to[LIBYA], 4, "Libya control from Isral should need 4 infl");
  TEST_EQUAL(inf_to[SYRIA], 3, "Syria control from Isral should need 3 infl");
  std::cout << std::endl;

  inf_to = twilight_struggle.shortest_dists(VENEZUELA);
  std::cout << "Ran Dijkstra's algorithm for distances from Venezuela. ";
  std::cout << "Shortest distance to:" << std::endl;
  for (size_t i = 0; i < inf_to.size(); ++i)
    if (inf_to[i] != -1)
      std::cout << " " << names[i] << ": " << inf_to[i] << std::endl;
  TEST_EQUAL(inf_to[BRAZIL], 2, "Brazil control should need 2 infl");
  TEST_EQUAL(inf_to[PARAGUAY], 6, "Paraguay control should need 6 infl");
  TEST_EQUAL(inf_to[ARGENTINA], 6, "Argentina control should need 6 infl");
  TEST_EQUAL(inf_to[CHILE], 8, "Chile control should need 8 infl");
  check = (inf_to[ARGENTINA] < inf_to[CHILE] + 2);
  TEST_CHECK(check, "S.America domination should be easier on Brazil side");
  check = (inf_to[ARGENTINA] + 3 < inf_to[CHILE] + 6);
  TEST_CHECK(check, "S.America control should be easier on Brazil side");
  std::cout << std::endl;

  inf_to = twilight_struggle.shortest_dists(ZAIRE);
  std::cout << "Ran Dijkstra's algorithm for distances from Zaire. ";
  std::cout << "Shortest distance to:" << std::endl;
  for (size_t i = 0; i < inf_to.size(); ++i)
    if (inf_to[i] != -1)
      std::cout << " " << names[i] << ": " << inf_to[i] << std::endl;
  TEST_EQUAL(inf_to[ANGOLA], 1, "Angola control should need 2 infl");
  TEST_EQUAL(inf_to[SOUTH_AFRICA], 5, "S.Africa control should need 5 infl");
  TEST_EQUAL(inf_to[NIGERIA], 2, "Nigeria control should need 2 infl");
  TEST_EQUAL(inf_to[ALGERIA], 5, "Algeria control should need 5 infl");
  check = (inf_to[ALGERIA] < inf_to[SOUTH_AFRICA] + 1);
  TEST_CHECK(check, "USSR Africa domination should be easier on Algeria side");
  check = (inf_to[NIGERIA] + inf_to[ANGOLA] < inf_to[ALGERIA]);
  TEST_CHECK(check, "Domination should be even easier locally around Zaire");
  check = (inf_to[ALGERIA] + inf_to[SOUTH_AFRICA] == 10);
  TEST_CHECK(check, "USSR Africa control from Zaire should need 10 infl");
  std::cout << std::endl;

  inf_to = twilight_struggle.shortest_dists(CUBA);
  std::cout << "Ran Dijkstra's algorithm for distances from Cuba. ";
  std::cout << "Shortest distance to:" << std::endl;
  for (size_t i = 0; i < inf_to.size(); ++i)
    if (inf_to[i] != -1)
      std::cout << " " << names[i] << ": " << inf_to[i] << std::endl;
  check = (inf_to[MEXICO] < inf_to[PANAMA]);
  TEST_EQUAL(inf_to[COSTA_RICA], 4, "Costa Rica control should need 4 infl");
  TEST_CHECK(check, "C.America domination should be easier on Mexico side");
  check = (inf_to[MEXICO] + inf_to[PANAMA] - 1 == 12);
  TEST_CHECK(check, "USSR C.America control from Cuba should need 12 infl");
  std::cout << std::endl;

  dyn_array<size_t> europe, asia, mideast, africa, camerica, samerica;
  dyn_array<size_t> eu_gt, as_gt, me_gt, af_gt, ca_gt, sa_gt;
  eu_gt = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
           15, 16, 17, 18, 19, 20};
  as_gt = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
  me_gt = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45};
  af_gt = {46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
           61, 62, 63};
  ca_gt = {64, 65, 66, 67, 68, 69, 70, 71, 72, 73};
  sa_gt = {74, 75, 76, 77, 78, 79, 80, 81, 82, 83};
  labels = twilight_struggle.weakly_conn_comp();
  std::cout << "Weakly-connected components on split map:" << std::endl;
  TEST_EQUAL(labels.max()+1, 8, "6 continents + USSR + USA = 8 components");
  europe    = labels.search(0);
  asia      = labels.search(1);
  mideast   = labels.search(2);
  africa    = labels.search(3);
  camerica  = labels.search(4);
  samerica  = labels.search(5);
  std::cout << std::endl << "Europe according to connected components: ";
  for (size_t i = 0; i < europe.size(); ++i)
    std::cout << names[europe[i]] << ((i < europe.size()-1) ? ", " : "");
  std::cout << std::endl << "Asia according to connected components: ";
  for (size_t i = 0; i < asia.size(); ++i)
    std::cout << names[asia[i]] << ((i < asia.size()-1) ? ", " : "");
  std::cout << std::endl << "Middle East according to connected components: ";
  for (size_t i = 0; i < mideast.size(); ++i)
    std::cout << names[mideast[i]] << ((i < mideast.size()-1) ? ", " : "");
  std::cout << std::endl << "Africa according to connected components: ";
  for (size_t i = 0; i < africa.size(); ++i)
    std::cout << names[africa[i]] << ((i < africa.size()-1) ? ", " : "");
  std::cout << std::endl << "C. America according to connected components: ";
  for (size_t i = 0; i < camerica.size(); ++i)
    std::cout << names[camerica[i]] << ((i < camerica.size()-1) ? ", " : "");
  std::cout << std::endl << "S. America according to connected components: ";
  for (size_t i = 0; i < samerica.size(); ++i)
    std::cout << names[samerica[i]] << ((i < samerica.size()-1) ? ", " : "");
  std::cout << std::endl;
  TEST_SEQ(europe, eu_gt, 21, "Europe should be correctly labeled");
  TEST_SEQ(asia, as_gt, 15, "Asia should be correctly labeled");
  TEST_SEQ(mideast, me_gt, 10, "Middle East should be correctly labeled");
  TEST_SEQ(africa, af_gt, 18, "Africa should be correctly labeled");
  TEST_SEQ(camerica, ca_gt, 10, "Central America should be correctly labeled");
  TEST_SEQ(samerica, sa_gt, 10, "South America should be correctly labeled");
  std::cout << std::endl;

  dyn_array<dyn_array<size_t> > scc = twilight_struggle.strongly_conn_comp();
  std::cout << "Strongly-connected components on split map:" << std::endl;
  for (size_t c = 0; c < scc.size(); ++c) {
    scc[c].quick_sort();
    std::cout << "SCC #" << c << ": ";
    for (size_t i = 0; i < scc[c].size(); ++i)
      std::cout << names[scc[c][i]] << ((i < scc[c].size()-1) ? ", " : "");
    std::cout << std::endl;
  }
  check = (scc.size() == labels.max() + 1);
  TEST_CHECK(check, "SCC and WCC sizes should match for an undirected graph");
  check = (scc[0].quick_sort() == europe);
  TEST_CHECK(check, "Strongly-connected component #0 should match Europe");
  check = (scc[1].quick_sort() == asia);
  TEST_CHECK(check, "Strongly-connected component #1 should match Asia");
  check = (scc[2].quick_sort() == mideast);
  TEST_CHECK(check, "Strongly-connected component #2 should match M.East");
  check = (scc[3].quick_sort() == africa);
  TEST_CHECK(check, "Strongly-connected component #3 should match Europe");
  check = (scc[4].quick_sort() == camerica);
  TEST_CHECK(check, "Strongly-connected component #4 should match Asia");
  check = (scc[5].quick_sort() == samerica);
  TEST_CHECK(check, "Strongly-connected component #5 should match M.East");
  TEST_EQUAL(scc[6][0], USSR, "Str. conn. compoennt #6 should be USSR");
  TEST_EQUAL(scc[7][0], USA,  "Str. conn. compoennt #7 should be USA");
  std::cout << std::endl;

  dyn_array<dyn_array<size_t> > brd = twilight_struggle.bridges().merge_sort();
  std::cout << "Detected following bridges on the split-continent map: ";
  for (size_t b = 0; b < brd.size(); ++b) {
    std::cout << "(" << names[brd[b][0]] << "__" << names[brd[b][1]] << ")";
    std::cout << ((b < brd.size()-1) ? " " : "");
  }
  std::cout << std::endl;
  check = !brd.search({SWEDEN, FINLAND}).is_empty();
  TEST_CHECK(check, "Sweden-Finland bridge should be detected");
  check = brd.search({TURKEY, GREECE}).is_empty();
  TEST_CHECK(check, "Turkey-Greece connection is not a bridge");
  check = !brd.search({INDIA, PAKISTAN}).is_empty();
  TEST_CHECK(check, "India-Pakistan bridge should be detected");
  check = brd.search({SOUTH_KOREA, JAPAN}).is_empty();
  TEST_CHECK(check, "South Korea-Japan connection is not a bridge");
  check = !brd.search({EGYPT, LIBYA}).is_empty();
  TEST_CHECK(check, "Egypt-Libya bridge should be detected");
  check = brd.search({JORDAN, LEBANON}).is_empty();
  TEST_CHECK(check, "Jordan-Lebanon connection is not a bridge");
  check = !brd.search({ZIMBABWE, SE_AFRICAN_STATES}).is_empty();
  TEST_CHECK(check, "Zimbabwe-S.E.African States bridge should be detected");
  check = brd.search({IVORY_COAST, WEST_AFRICAN_STATES}).is_empty();
  TEST_CHECK(check, "Ivory Coast-W.African States connection is not a bridge");
  check = !brd.search({COSTA_RICA, PANAMA}).is_empty();
  TEST_CHECK(check, "Costa Rica-Panama bridge should be detected");
  check = brd.search({COSTA_RICA, NICARAGUA}).is_empty();
  TEST_CHECK(check, "Costa Rica-Nicaragua connection is not a bridge");
  check = brd.search({PERU, ECUADOR}).is_empty();
  TEST_CHECK(check, "South America has no bridges");
  check = brd.search({BOLIVIA, PERU}).is_empty();
  TEST_CHECK(check, "South America has no bridges");
  std::cout << std::endl;

  dyn_array<size_t> apt = twilight_struggle.articulation_points();
  std::cout << "Following states were identified as articulation points: ";
  for (size_t a = 0; a < apt.size(); ++a)
    std::cout << names[apt[a]] << ((a < apt.size()-1) ? ", " : "");
  std::cout << std::endl;
  check = !apt.search(UK).is_empty();
  TEST_CHECK(check, "UK should be marked as an articulation point");
  check = apt.search(WEST_GERMANY).is_empty();
  TEST_CHECK(check, "West Germany is not an articulation point");
  int non_art = 0;
  for (size_t i = 21; i < 36; ++i)
    if (apt.search(i).is_empty())
      non_art++;
  TEST_EQUAL(non_art, 5, "All but 5 Asian states are articulation points");
  check = !apt.search(THAILAND).is_empty();
  TEST_CHECK(check, "Thailand should be one of them");
  check = !apt.search(JORDAN).is_empty();
  TEST_CHECK(check, "Jordan should be marked as an articulation point");
  check = !apt.search(IRAQ).is_empty();
  TEST_CHECK(check, "Iraq should be marked as an articulation point");
  check = !apt.search(CAMEROON).is_empty();
  TEST_CHECK(check, "Cameroon should be marked as an articulation point");
  check = apt.search(ANGOLA).is_empty();
  TEST_CHECK(check, "Angola is not an articulation point");
  check = !apt.search(HONDURAS).is_empty();
  TEST_CHECK(check, "Honduras should be marked as an articulation point");
  check = !apt.search(GUATEMALA).is_empty();
  TEST_CHECK(check, "Guatemala should be marked as an articulation point");
  int art_cnt = 0;
  for (size_t i = 74; i < 84; ++i)
    if (!apt.search(i).is_empty())
      art_cnt++;
  TEST_EQUAL(art_cnt, 0, "South America should have no articulation points");
  std::cout << std::endl;

  // Connect players directly to the neighbors of states they control since
  // edge weight of 0 removes the edge rather than making it no-cost.
  dyn_array<dyn_array<int> > plyracc = {
    // ------------------------------EUROPE------------------------------------
    {USA, FRANCE, 3}, {USA, BENELUX, 3}, {USA, NORWAY, 4}, {USA, CANADA, 2},
    {USSR, FINLAND, 3},        {USSR, ROMANIA, 3},       {USSR, AUSTRIA, 3},
    {USA, EAST_GERMANY, 9},    {USA, AUSTRIA, 5},         {USA, DENMARK, 3},
    {USA, SPAIN_PORTUGAL, 2},  {USA, GREECE, 2},       {USA, YUGOSLAVIA, 3},
    {USSR, CZECHOSLOVAKIA, 3}, {USSR, WEST_GERMANY, 9},
    // -------------------------------ASIA-------------------------------------
    {USSR, AFGHANISTAN, 2},  {USSR, SOUTH_KOREA, 4},  {USA, PHILIPPINES, 1},
    {USA, SOUTH_KOREA, 2},   {USA, JAPAN, 3},            {USA, MALAYSIA, 2},
    // ---------------------------MIDDLE EAST----------------------------------
    {USSR, IRAQ, 2},  {USSR, SYRIA, 1},   {USA, ISRAEL, 3},  {USA, IRAN, 1},
    // ------------------------------AFRICA------------------------------------
    {USA, SOUTH_AFRICA, 2},
    // -------------------------CENTRAL AMERICA--------------------------------
    {USA, PANAMA, 1},        {USA, MEXICO, 2},               {USA, CUBA, 3}
    // --------------------------SOUTH AMERICA---------------------------------
    /* ------------------------------------------------------------------ */};

  for (size_t i = 0; i < plyracc.size(); ++i)
    twilight_struggle.add_edgew(plyracc[i][0], plyracc[i][1], plyracc[i][2]);

  dyn_array<int> plyr_labels = twilight_struggle.weakly_conn_comp();
  std::cout << "Connected players to their access points:" << std::endl;
  check = (plyr_labels.max() + 1 == 2);
  TEST_CHECK(check, "When players connect, there should be 2 components");
  check = (samerica == plyr_labels.search(1));
  TEST_CHECK(check, "South America should be the only disconnected component");
  std::cout << std::endl;

  dyn_array<dyn_array<int> > cross_continent = {
    // ------------------------------EUROPE------------------------------------
    {SPAIN_PORTUGAL, MOROCCO, 3}, {FRANCE, ALGERIA, 2}, {TURKEY, SYRIA, 3},
    // -------------------------------ASIA-------------------------------------
    {AFGHANISTAN, IRAN, 3},
    // ---------------------------MIDDLE EAST----------------------------------
    {IRAN, PAKISTAN, 2}, {LIBYA, TUNISIA, 2}, {EGYPT, SUDAN, 1},
    // ------------------------------AFRICA------------------------------------
    // -------------------------CENTRAL AMERICA--------------------------------
    {PANAMA, COLOMBIA, 1}
    // --------------------------SOUTH AMERICA---------------------------------
    /* ------------------------------------------------------------------ */};

  for (size_t i = 0; i < cross_continent.size(); ++i)
    twilight_struggle.add_edgew(cross_continent[i][0],
                                cross_continent[i][1],
                                cross_continent[i][2]);

  dyn_array<size_t> apt_global;
  dyn_array<dyn_array<size_t> > brd_global;
  brd_global = twilight_struggle.bridges().merge_sort();
  apt_global = twilight_struggle.articulation_points();
  scc = twilight_struggle.strongly_conn_comp();
  std::cout << "Connected continents with directed edges:" << std::endl;
  check = (brd_global.size() == brd.size());
  TEST_CHECK(check, "Connected continents should have same number of bridges");
  check = (apt_global.size() == apt.size() + 4);
  TEST_CHECK(check, "Connecting continents should result in 4 more art pts");
  check = !apt_global.search(IRAN).is_empty();
  check = check && apt.search(IRAN).is_empty();
  TEST_CHECK(check, "Iran should be a new articulation point");
  check = !apt_global.search(TURKEY).is_empty();
  check = check && apt.search(TURKEY).is_empty();
  TEST_CHECK(check, "Turkey should be a new articulation point");
  check = !brd_global.search({SPAIN_PORTUGAL, MOROCCO}).is_empty();
  check = check && brd.search({SPAIN_PORTUGAL, MOROCCO}).is_empty();
  TEST_CHECK(check, "Spain/Portugal-Morrocco should be a new bridge");
  check = brd_global.search({PAKISTAN, AFGHANISTAN}).is_empty();
  check = check && !brd.search({PAKISTAN, AFGHANISTAN}).is_empty();
  TEST_CHECK(check, "Pakistan-Afghanistan should no longer be a bridge");
  check = brd_global.search({ISRAEL, EGYPT}).is_empty();
  check = check && !brd.search({ISRAEL, EGYPT}).is_empty();
  TEST_CHECK(check, "Israel-Egypt should no longer be a bridge");
  TEST_EQUAL(scc.size(), 7, "There should now be 7 strongly conn components");
  dyn_array<size_t> silkroad = asia;
  silkroad.append(mideast);
  check = (scc[1].quick_sort() == silkroad.quick_sort());
  TEST_CHECK(check, "Asia and Middle East should merge into 1 str conn comp");
  std::cout << std::endl;

  dyn_array<dyn_array<int> > cr_back = {
    // ------------------------------EUROPE------------------------------------
    {MOROCCO, SPAIN_PORTUGAL, 2}, {ALGERIA, FRANCE, 3}, {SYRIA, TURKEY, 2},
    // -------------------------------ASIA-------------------------------------
    {IRAN, AFGHANISTAN, 2},
    // ---------------------------MIDDLE EAST----------------------------------
    {PAKISTAN, IRAN, 3}, {TUNISIA, LIBYA, 2}, {SUDAN, EGYPT, 2},
    // ------------------------------AFRICA------------------------------------
    // -------------------------CENTRAL AMERICA--------------------------------
    {COLOMBIA, PANAMA, 3}
    // --------------------------SOUTH AMERICA---------------------------------
    /* ------------------------------------------------------------------ */};

  for (size_t i = 0; i < cr_back.size(); ++i)
    twilight_struggle.add_edgew(cr_back[i][0], cr_back[i][1], cr_back[i][2]);

  scc = twilight_struggle.strongly_conn_comp();
  brd_global = twilight_struggle.bridges().merge_sort();
  apt_global = twilight_struggle.articulation_points();
  std::cout << "Fully connected all continents with bidirectional edges:";
  std::cout << std::endl;
  TEST_EQUAL(scc.size(), 4, "There should now be 4 strongly conn components");
  dyn_array<size_t> oldworld = silkroad;
  oldworld.append(europe).append(africa);
  dyn_array<size_t> americas = camerica;
  americas.append(samerica);
  check = (scc[0].quick_sort() == oldworld.quick_sort());
  TEST_CHECK(check, "Europe, Asia, M.East and Africa should merge into 1 SCC");
  check = (scc[1].quick_sort() == americas);
  TEST_CHECK(check, "Central and South America should merge into 1 SCC");
  TEST_EQUAL(scc[2][0], USSR, "Str. conn. compoennt #3 should be USSR");
  TEST_EQUAL(scc[3][0], USA,  "Str. conn. compoennt #4 should be USA");
  TEST_EQUAL(brd_global.size(), 18, "There should only be 18 bridges left");
  TEST_EQUAL(apt_global.size(), 23, "There should only be 23 art points left");
  std::cout << std::endl;

  dyn_array<int> continents_traveled;
  inds = twilight_struggle.bidirectional_search(TURKEY, PAKISTAN);
  std::cout << "Tried bidirectional search again from Turkey to Pakistan: ";
  for (size_t i = 0; i < inds.size(); ++i) {
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
    continents_traveled.add(labels[inds[i]]);
  }
  std::cout << std::endl;
  check = (inds.last() == PAKISTAN);
  TEST_CHECK(check, "There should now be a path from Turkey to Pakistan");
  check = (continents_traveled.unique().size() == 3);
  TEST_CHECK(check, "Turkey-Pakistan path should pass through 3 continents");
  std::cout << std::endl;

  dyn_array<int> ussr_to = twilight_struggle.shortest_dists(USSR);
  dyn_array<int> usa_to  = twilight_struggle.shortest_dists(USA);
  std::cout << "Got Dijkstra distances from USSR and USA." << std::endl;
  size_t ussr_acc = twilight_struggle.size() - ussr_to.search(-1).size() - 1;
  size_t usa_acc  = twilight_struggle.size() - usa_to.search(-1).size()  - 1;
  TEST_EQUAL(ussr_acc, 64, "64 out of 84 states should be accessible to USSR");
  TEST_EQUAL(usa_acc,  84, "All 84 states should be accessible to USA");
  int cent_amr = 0;
  for (size_t i = 64; i < 74; ++i)
    if (ussr_to[i] != -1)
      cent_amr++;
  TEST_EQUAL(cent_amr, 0, "USSR should have no access to Central America");
  cent_amr = 0;
  for (size_t i = 64; i < 74; ++i)
    if (usa_to[i] != -1)
      cent_amr++;
  TEST_EQUAL(cent_amr, 10, "USA should have access to all states in C.America");
  std::cout << std::endl;

  ind_gt  = {7, 9, 12, 13, 38, 37};
  path_gt = {7, 62, 63, 45, 44, 36, 37};
  inds = twilight_struggle.bidirectional_search(FRANCE, LEBANON);
  path = twilight_struggle.dijkstra(FRANCE)[LEBANON];
  std::cout << "Shortest path from France to Lebanon: ";
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "Min-cost path from France to Lebanon: ";
  for (size_t i = 0; i < path.size(); ++i)
    std::cout << names[path[i]] << ((i < path.size()-1) ? " -> " : "");
  std::cout << std::endl;
  check = (inds.last() == LEBANON) && (path.last() == LEBANON);
  TEST_CHECK(check, "Both BDS and Dijkstra should find a valid path");
  check = (inds.size() != path.size());
  TEST_CHECK(check, "Min-cost and shortest paths should be different");
  TEST_SEQ(inds, ind_gt, 6, "Bidirectional search path should match gt");
  TEST_SEQ(path, path_gt, 7, "Dijkstra min-cost path should match gt");
  std::cout << std::endl;

  ind_gt  = {84, 32, 31, 25, 24, 27};
  path_gt = {84, 30, 29, 28, 26, 23, 21, 27};
  inds = twilight_struggle.bidirectional_search(USSR, MALAYSIA);
  path = twilight_struggle.dijkstra(USSR)[MALAYSIA];
  std::cout << "Shortest path from USSR to Malaysia: ";
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "Min-cost path from USSR to Malaysia: ";
  for (size_t i = 0; i < path.size(); ++i)
    std::cout << names[path[i]] << ((i < path.size()-1) ? " -> " : "");
  std::cout << std::endl;
  check = (inds.last() == MALAYSIA) && (path.last() == MALAYSIA);
  TEST_CHECK(check, "Both BDS and Dijkstra should find a valid path");
  check = (path.size() == inds.size() + 2);
  TEST_CHECK(check, "Min-cost path should take 2 more steps than BDS");
  TEST_SEQ(inds, ind_gt, 6, "Bidirectional search path should match gt");
  TEST_SEQ(path, path_gt, 8, "Dijkstra min-cost path should match gt");
  std::cout << std::endl;

  ind_gt  = {85, 7, 62, 59, 48};
  path_gt = {85, 54, 51, 50, 49, 48};
  inds = twilight_struggle.bidirectional_search(USA, NIGERIA);
  path = twilight_struggle.dijkstra(USA)[NIGERIA];
  std::cout << "Shortest path from USA to Malaysia: ";
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
  std::cout << std::endl;
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << inds[i] << ((i < inds.size()-1) ? ", " : "");
  std::cout << std::endl;
  std::cout << "Min-cost path from USA to Malaysia: ";
  for (size_t i = 0; i < path.size(); ++i)
    std::cout << names[path[i]] << ((i < path.size()-1) ? " -> " : "");
  std::cout << std::endl;
  for (size_t i = 0; i < path.size(); ++i)
    std::cout << path[i] << ((i < path.size()-1) ? ", " : "");
  std::cout << std::endl;
  check = (inds.last() == NIGERIA) && (path.last() == NIGERIA);
  TEST_CHECK(check, "Both BDS and Dijkstra should find a valid path");
  check = (inds.size() != path.size());
  TEST_CHECK(check, "Min-cost and shortest paths should be different");
  TEST_CHECK(path[1] != inds[1], "Paths should start from different states");
  TEST_SEQ(inds, ind_gt, 5, "Bidirectional search path should match gt");
  TEST_SEQ(path, path_gt, 6, "Dijkstra min-cost path should match gt");
  std::cout << std::endl;

  int ussr_inf, usa_inf;
  size_t ussr_rnd, usa_rnd;
  dyn_array<int> dists;
  dyn_array<size_t> ussr_p1, ussr_p2, usa_p1, usa_p2;
  dyn_array<dyn_array<int> > adjm = twilight_struggle.adj_mat();

  std::cout << "BEHIND THE IRON CURTAIN: " << std::endl;
  std::cout << "Can USSR take France more easily than USA takes Poland?";
  std::cout << std::endl;
  check = (ussr_to[FRANCE] < usa_to[POLAND]);
  TEST_CHECK(check, "Taking Poland should cost USA more influence");
  ussr_p1 = twilight_struggle.bidirectional_search(USSR, FRANCE);
  usa_p1 = twilight_struggle.bidirectional_search(USSR, FRANCE);
  check = (ussr_p1.size() == usa_p1.size());
  TEST_CHECK(check, "They both should need the same number of rounds");

  std::cout << "THE GREAT CRAWL:" << std::endl;
  std::cout << "Can USA reach India from Australia before USSR gets Thailand?";
  std::cout << std::endl;
  ussr_inf = ussr_to[THAILAND];
  usa_inf = twilight_struggle.shortest_dists(AUSTRALIA)[INDIA];
  TEST_CHECK(ussr_inf > usa_inf, "USSR should need more influence than USA");
  ussr_rnd = twilight_struggle.bidirectional_search(USSR, THAILAND).size();
  usa_rnd = twilight_struggle.bidirectional_search(AUSTRALIA, INDIA).size();
  TEST_CHECK(ussr_rnd > usa_rnd, "USSR should need more rounds than USA");
  std::cout << std::endl;

  std::cout << "SUEZ CRISIS:" << std::endl;
  std::cout << "Can USSR deny access to USA with no influence left in M.East?";
  std::cout << std::endl;
  // Temporarily adjust for lost US access and influence.
  twilight_struggle.remove_edge(USA, ISRAEL);
  twilight_struggle.remove_edge(USA, IRAN);
  twilight_struggle.remove_edge(SYRIA, ISRAEL);
  twilight_struggle.add_edgew(SYRIA, ISRAEL, 4);
  // Update shortest distances after the adjustment.
  ussr_to = twilight_struggle.shortest_dists(USSR);
  usa_to = twilight_struggle.shortest_dists(USA);
  // Path to enter from Iran goes through Syria, so only 2 options.
  usa_p1 = twilight_struggle.bidirectional_search(USA, SYRIA);
  usa_p2 = twilight_struggle.bidirectional_search(USA, LIBYA);
  std::cout << "USA path for reentry through Syria: ";
  for (size_t i = 0; i < usa_p1.size(); ++i)
    std::cout << names[usa_p1[i]] << ((i < usa_p1.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "USA has to spend " << usa_to[SYRIA] << " influence and ";
  std::cout << usa_p1.size() - 1 << " rounds on Syria path." << std::endl;
  std::cout << "USA path for reentry through Libya: ";
  for (size_t i = 0; i < usa_p2.size(); ++i)
    std::cout << names[usa_p2[i]] << ((i < usa_p2.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "USA has to spend " << usa_to[LIBYA] << " influence and ";
  std::cout << usa_p2.size() - 1 << " rounds on Libya path." << std::endl;
  ussr_p1 = twilight_struggle.bidirectional_search(USSR, SYRIA);
  ussr_p2 = twilight_struggle.bidirectional_search(USSR, LIBYA);
  std::cout << "USSR path for blocking Syria: ";
  for (size_t i = 0; i < ussr_p1.size(); ++i)
    std::cout << names[ussr_p1[i]] << ((i < ussr_p1.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "USSR needs " << ussr_to[SYRIA] << " influence and ";
  std::cout << ussr_p1.size() - 1 << " rounds to block Syria." << std::endl;
  std::cout << "USSR path for blocking Libya: ";
  for (size_t i = 0; i < ussr_p2.size(); ++i)
    std::cout << names[ussr_p2[i]] << ((i < ussr_p2.size()-1) ? " -> " : "");
  std::cout << std::endl;
  std::cout << "USSR needs " << ussr_to[LIBYA] << " influence and ";
  std::cout << ussr_p2.size() - 1 << " rounds to block Libya." << std::endl;

  check = (ussr_to[SYRIA] <= usa_to[SYRIA]);
  TEST_CHECK(check, "USSR should have enough influence to block Syria");
  check = (ussr_p1.size() <= usa_p1.size());
  TEST_CHECK(check, "USSR should have enough time to block Syria");
  check = (ussr_to[LIBYA] <= usa_to[LIBYA]);
  TEST_CHECK(check, "USSR should have enough influence to block Libya");
  check = (ussr_p2.size() <= usa_p2.size());
  TEST_CHECK(check, "USSR should have enough time to block Libya");
  // Reset the board.
  twilight_struggle.add_edgew(USA, ISRAEL, 3);
  twilight_struggle.add_edgew(USA, IRAN, 1);
  twilight_struggle.remove_edge(SYRIA, ISRAEL);
  twilight_struggle.add_edgew(SYRIA, ISRAEL, 5);
  std::cout << std::endl;

  std::cout << "THE HEART OF DARKNESS:" << std::endl;
  std::cout << "Can USSR reach Zaire before USA fully controls S.E. Asia?";
  std::cout << std::endl;
  ussr_p1 = twilight_struggle.bidirectional_search(USSR, KENYA);
  ussr_p1.append(twilight_struggle.bidirectional_search(KENYA, ZAIRE));
  std::cout << "USSR traversal path to Zaire: ";
  for (size_t i = 0; i < ussr_p1.size(); ++i)
    std::cout << names[ussr_p1[i]] << ((i < ussr_p1.size()-1) ? " -> " : "");
  std::cout << std::endl;
  ussr_inf = ussr_to[ZAIRE];
  ussr_rnd = ussr_p1.size() - 2;
  std::cout << "USSR spent " << ussr_inf << " influence and " << ussr_rnd;
  std::cout << " rounds to reach Zaire." << std::endl;
  usa_p1 = twilight_struggle.bidirectional_search(USA, BURMA);
  usa_p1.append(twilight_struggle.bidirectional_search(USA, INDONESIA));
  std::cout << "USA traversal path through Southeast Asia: ";
  for (size_t i = 0; i < usa_p1.size(); ++i)
    std::cout << names[usa_p1[i]] << ((i < usa_p1.size()-1) ? " -> " : "");
  std::cout << std::endl;
  usa_inf = usa_to[BURMA] + usa_to[INDONESIA] + adjm[THAILAND][VIETNAM];
  usa_rnd = usa_p1.size() - 1;
  std::cout << "USA needs " << usa_inf << " influence and " << usa_rnd;
  std::cout << " rounds to control Southeast Asia." << std::endl;
  TEST_CHECK(usa_inf < ussr_inf, "USA should need less inf to control SE Asia");
  TEST_CHECK(usa_rnd < ussr_rnd, "USSR should take longer to reach Zaire");
  std::cout << std::endl;

  std::cout << "HEMISPHERIC DEFENSE:" << std::endl;
  std::cout << "Can USSR spread faster from Cuba than USA in South America?";
  std::cout << std::endl;
  ussr_p1 = twilight_struggle.bidirectional_search(CUBA, MEXICO);
  ussr_p1.append(twilight_struggle.bidirectional_search(NICARAGUA, PANAMA));
  std::cout << "USSR traversal path through Central America: ";
  for (size_t i = 0; i < ussr_p1.size(); ++i)
    std::cout << names[ussr_p1[i]] << ((i < ussr_p1.size()-1) ? " -> " : "");
  std::cout << std::endl;
  dists = twilight_struggle.shortest_dists(CUBA);
  ussr_inf = dists[MEXICO] + dists[PANAMA] - adjm[CUBA][NICARAGUA];
  ussr_rnd = ussr_p1.size() - 2;
  std::cout << "USSR needs " << ussr_inf << " influence and " << ussr_rnd;
  std::cout << " rounds to control Central America." << std::endl;
  usa_p1 = twilight_struggle.bidirectional_search(PANAMA, BRAZIL);
  usa_p1.append(twilight_struggle.bidirectional_search(BRAZIL, CHILE));
  std::cout << "USA traversal path through South America: ";
  for (size_t i = 0; i < usa_p1.size(); ++i)
    std::cout << names[usa_p1[i]] << ((i < usa_p1.size()-1) ? " -> " : "");
  std::cout << std::endl;
  usa_to = twilight_struggle.shortest_dists(PANAMA);
  dists = twilight_struggle.shortest_dists(BRAZIL);
  usa_inf = usa_to[BRAZIL] + dists[CHILE];
  usa_rnd = usa_p1.size() - 2;
  std::cout << "USA needs " << usa_inf << " influence and " << usa_rnd;
  std::cout << " rounds to control South America." << std::endl;
  TEST_CHECK(usa_inf == ussr_inf, "They should need the same amount of inf");
  TEST_CHECK(usa_inf == ussr_inf, "They should need same amount of rounds");
  std::cout << std::endl;

  dyn_array<dyn_array<int> > unrealistic = {
    // ------------------------------EUROPE------------------------------------
    {BENELUX, UK},       {FRANCE, UK},        {NORWAY, UK},       {CANADA, UK},
    {DENMARK, WEST_GERMANY},  {BENELUX, WEST_GERMANY},  {FRANCE, WEST_GERMANY},
    {AUSTRIA, EAST_GERMANY},  {AUSTRIA, WEST_GERMANY},  {POLAND, EAST_GERMANY},
    {WEST_GERMANY, EAST_GERMANY},                 {EAST_GERMANY, WEST_GERMANY},
    {CZECHOSLOVAKIA, EAST_GERMANY},
    // -------------------------------ASIA-------------------------------------
    {PHILIPPINES, JAPAN},  {SOUTH_KOREA, NORTH_KOREA},   {MALAYSIA, AUSTRALIA},
    // ---------------------------MIDDLE EAST----------------------------------
    {JORDAN, LEBANON},          {JORDAN, ISRAEL},          {GULF_STATES, IRAQ},
    // ------------------------------AFRICA------------------------------------
    // -------------------------CENTRAL AMERICA--------------------------------
    {NICARAGUA, CUBA},                                           {HAITI, CUBA},
    // --------------------------SOUTH AMERICA---------------------------------
    /* ------------------------------------------------------------------ */};

  for (size_t i = 0; i < unrealistic.size(); ++i)
    twilight_struggle.remove_edge(unrealistic[i][0], unrealistic[i][1]);

  scc = twilight_struggle.strongly_conn_comp();
  std::cout << "Removed irrelevant/suboptimal sides of bidirectional links:";
  std::cout << std::endl;
  TEST_EQUAL(scc.size(), 16, "There should now be 16 strongly-conn. comps");
  ind_gt = {NORWAY, DENMARK, SWEDEN, FINLAND};
  scc[1].quick_sort();
  scc[8].quick_sort();
  TEST_SEQ(scc[1], ind_gt, 4, "Scandinavian states should form their own SCC");
  ind_gt = {JAPAN, SOUTH_KOREA, TAIWAN};
  TEST_SEQ(scc[8], ind_gt, 3, "Japan, S.Korea and Taiwan should be a new SCC");
  std::cout << std::endl;

  dyn_array<dyn_array<int> > acycl_removal = {
    // ------------------------------EUROPE------------------------------------
    {TURKEY, GREECE}, {GREECE, ITALY}, {YUGOSLAVIA, ITALY},  {TURKEY, ROMANIA},
    {SWEDEN, FINLAND}, {DENMARK, SWEDEN}, {SWEDEN, NORWAY}, {BULGARIA, TURKEY},
    {SPAIN_PORTUGAL, FRANCE},  {BULGARIA, GREECE},     {EAST_GERMANY, AUSTRIA},
    {SPAIN_PORTUGAL, ITALY},   {YUGOSLAVIA, ROMANIA},    {YUGOSLAVIA, HUNGARY},
    {POLAND, CZECHOSLOVAKIA},  {GREECE, YUGOSLAVIA}, {CZECHOSLOVAKIA, HUNGARY},
    {HUNGARY, AUSTRIA}, {HUNGARY, ROMANIA},  {FRANCE, ITALY}, {ITALY, AUSTRIA},
    // -------------------------------ASIA-------------------------------------
    {BURMA, INDIA},  {TAIWAN, JAPAN},  {VIETNAM, THAILAND},  {INDIA, PAKISTAN},
    {TAIWAN, SOUTH_KOREA},   {LAOS_CAMBODIA, BURMA},  {INDONESIA, PHILIPPINES},
    {PAKISTAN, AFGHANISTAN}, {VIETNAM, LAOS_CAMBODIA},   {MALAYSIA, INDONESIA},
    {JAPAN, SOUTH_KOREA},    {THAILAND, LAOS_CAMBODIA},   {THAILAND, MALAYSIA},

    // ---------------------------MIDDLE EAST----------------------------------
    {ISRAEL, EGYPT},  {SAUDI_ARABIA, JORDAN},    {IRAN, IRAQ},  {IRAQ, JORDAN},
    {LEBANON, ISRAEL},  {SYRIA, ISRAEL}, {SAUDI_ARABIA, IRAQ},  {LIBYA, EGYPT},
    {SAUDI_ARABIA, GULF_STATES},                              {LEBANON, SYRIA},
    // ------------------------------AFRICA------------------------------------
    {NIGERIA, SAHARAN_STATES}, {SOMALIA, ETHIOPIA}, {SE_AFRICAN_STATES, KENYA},
    {SAHARAN_STATES, ALGERIA}, {NIGERIA, CAMEROON},        {NIGERIA, CAMEROON},
    {SOUTH_AFRICA, BOTSWANA},  {MOROCCO, ALGERIA},          {ANGOLA, BOTSWANA},
    {ANGOLA, SOUTH_AFRICA},   {CAMEROON, ZAIRE},            {ALGERIA, TUNISIA},
    {IVORY_COAST, NIGERIA},  {ZAIRE, ANGOLA},    {ZIMBABWE, SE_AFRICAN_STATES},
    {BOTSWANA, ZIMBABWE},  {ETHIOPIA, SUDAN},   {MOROCCO, WEST_AFRICAN_STATES},
    {ZAIRE, ZIMBABWE},   {KENYA, SOMALIA},  {WEST_AFRICAN_STATES, IVORY_COAST},
    // -------------------------CENTRAL AMERICA--------------------------------
    {COSTA_RICA, NICARAGUA},  {GUATEMALA, HONDURAS},      {COSTA_RICA, PANAMA},
    {EL_SALVADOR, HONDURAS},  {HONDURAS, NICARAGUA},    {HONDURAS, COSTA_RICA},
    {GUATEMALA, EL_SALVADOR}, {MEXICO, GUATEMALA}, {DOMINICAN_REPUBLIC, HAITI},
    // --------------------------SOUTH AMERICA---------------------------------
    {BOLIVIA, PARAGUAY},         {ARGENTINA, URUGUAY},         {PERU, BOLIVIA},
    {ECUADOR, COLOMBIA},         {COLOMBIA, VENEZUELA},        {PERU, ECUADOR},
    {BRAZIL, VENEZUELA},         {ARGENTINA, PARAGUAY},          {CHILE, PERU},
    {PARAGUAY, URUGUAY},         {CHILE, ARGENTINA},         {URUGUAY, BRAZIL},
    /* ------------------------------------------------------------------ */};

  for (size_t i = 0; i < acycl_removal.size(); ++i)
    twilight_struggle.remove_edge(acycl_removal[i][0], acycl_removal[i][1]);
  // Turn cross-continental links back into directed edges.
  for (size_t i = 0; i < cr_back.size(); ++i)
    twilight_struggle.remove_edge(cr_back[i][0], cr_back[i][1]);

  scc = twilight_struggle.strongly_conn_comp();
  inds = twilight_struggle.kahn();
  std::cout << "Made all edges directed to create a acyclic map" << std::endl;
  std::cout << "Kahn's sorted order: ";
  for (size_t i = 0; i < inds.size(); ++i)
    std::cout << names[inds[i]] << ((i < inds.size()-1) ? " -> " : "");
  std::cout << std::endl;
  check = (inds.size() == twilight_struggle.size());
  TEST_CHECK(check, "Kahn's algorithm should be able to access all states");
  check = (scc.size() == twilight_struggle.size());
  TEST_CHECK(check, "Each state should now be a standalone str. conn. comp.");
  check = (inds.search(GREECE)[0] < inds.search(TURKEY)[0]);
  TEST_CHECK(check, "Greece should come before Turkey in topological sort");
  check = (inds.search(SAHARAN_STATES)[0] > inds.search(ALGERIA)[0]);
  TEST_CHECK(check, "Saharan States should come after Algeria");
  check = (inds.search(KENYA)[0] > inds.search(SOMALIA)[0]);
  TEST_CHECK(check, "Indonesia should come after Malaysia");
  check = (inds.search(HUNGARY)[0] < inds.search(CZECHOSLOVAKIA)[0]);
  TEST_CHECK(check, "Hungary should come before Czechoslovakia");
  check = (inds.search(CHILE)[0] > inds.search(ARGENTINA)[0]);
  TEST_CHECK(check, "Chile should come after Argentina");

  test_util::print_summary();
  return 0;
}