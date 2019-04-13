#include "program_knowledge_base/pkb_manager.h"
#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>

using Simple::SimpleInterface;

TEST_CASE ("Test PKB for extension") {
  auto ast = SimpleInterface::getAstFromFile(
      "tests/simple_source/call/complex_call_structure.txt");
  PKB::PKBManager pkb = PKB::PKBManager(ast);

  SECTION("Next Bip") {
    std::unordered_set<std::string> next_bip_test_1_check;
    next_bip_test_1_check.insert("3");
    auto next_bip_test_1 = pkb.getNextLineBip("1");
    REQUIRE(*next_bip_test_1 == next_bip_test_1_check);

    auto next_bip_test_2 = pkb.getNextLineBip("2");
    REQUIRE(next_bip_test_2 == std::nullopt);

    std::unordered_set<std::string> next_bip_test_3_check;
    next_bip_test_3_check.insert("6");
    auto next_bip_test_3 = pkb.getNextLineBip("3");
    REQUIRE(*next_bip_test_3 == next_bip_test_3_check);

    std::unordered_set<std::string> next_bip_test_4_check;
    next_bip_test_4_check.insert("5");
    auto next_bip_test_4 = pkb.getNextLineBip("4");
    REQUIRE(*next_bip_test_4 == next_bip_test_4_check);

    std::unordered_set<std::string> next_bip_test_5_check;
    next_bip_test_5_check.insert("2");
    auto next_bip_test_5 = pkb.getNextLineBip("5");
    REQUIRE(*next_bip_test_5 == next_bip_test_5_check);

    std::unordered_set<std::string> next_bip_test_6_check;
    next_bip_test_6_check.insert("9");
    auto next_bip_test_6 = pkb.getNextLineBip("6");
    REQUIRE(*next_bip_test_6 == next_bip_test_6_check);

    std::unordered_set<std::string> next_bip_test_7_check;
    next_bip_test_7_check.insert("11");
    auto next_bip_test_7 = pkb.getNextLineBip("7");
    REQUIRE(*next_bip_test_7 == next_bip_test_7_check);

    std::unordered_set<std::string> next_bip_test_8_check;
    next_bip_test_8_check.insert("4");
    auto next_bip_test_8 = pkb.getNextLineBip("8");
    REQUIRE(*next_bip_test_8 == next_bip_test_8_check);

    std::unordered_set<std::string> next_bip_test_9_check;
    next_bip_test_9_check.insert("10");
    auto next_bip_test_9 = pkb.getNextLineBip("9");
    REQUIRE(*next_bip_test_9 == next_bip_test_9_check);

    std::unordered_set<std::string> next_bip_test_10_check;
    next_bip_test_10_check.insert("7");
    auto next_bip_test_10 = pkb.getNextLineBip("10");
    REQUIRE(*next_bip_test_10 == next_bip_test_10_check);

    std::unordered_set<std::string> next_bip_test_11_check;
    next_bip_test_11_check.insert("12");
    auto next_bip_test_11 = pkb.getNextLineBip("11");
    REQUIRE(*next_bip_test_11 == next_bip_test_11_check);

    std::unordered_set<std::string> next_bip_test_12_check;
    next_bip_test_12_check.insert("8");
    auto next_bip_test_12 = pkb.getNextLineBip("12");
    REQUIRE(*next_bip_test_12 == next_bip_test_12_check);

    std::unordered_set<std::string> next_bip_test_13_check;
    next_bip_test_13_check.insert("15");
    auto next_bip_test_13 = pkb.getNextLineBip("13");
    REQUIRE(*next_bip_test_13 == next_bip_test_13_check);

    auto next_bip_test_14 = pkb.getNextLineBip("14");
    REQUIRE(next_bip_test_14 == std::nullopt);

    std::unordered_set<std::string> next_bip_test_15_check;
    next_bip_test_15_check.insert("16");
    auto next_bip_test_15 = pkb.getNextLineBip("15");
    REQUIRE(*next_bip_test_15 == next_bip_test_15_check);

    std::unordered_set<std::string> next_bip_test_16_check;
    next_bip_test_16_check.insert("14");
    auto next_bip_test_16 = pkb.getNextLineBip("16");
    REQUIRE(*next_bip_test_16 == next_bip_test_16_check);
  }
}