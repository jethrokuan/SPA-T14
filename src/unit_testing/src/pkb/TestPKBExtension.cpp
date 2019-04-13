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

  SECTION("NextT Bip") {
    std::unordered_set<std::string> next_bip_test_1_check;
    next_bip_test_1_check.insert("3");
    next_bip_test_1_check.insert("6");
    next_bip_test_1_check.insert("9");
    next_bip_test_1_check.insert("10");
    next_bip_test_1_check.insert("7");
    next_bip_test_1_check.insert("11");
    next_bip_test_1_check.insert("12");
    next_bip_test_1_check.insert("8");
    next_bip_test_1_check.insert("4");
    next_bip_test_1_check.insert("5");
    next_bip_test_1_check.insert("2");
    auto next_bip_test_1 = pkb.getNextLineTBip("1");
    REQUIRE(*next_bip_test_1 == next_bip_test_1_check);

    auto next_bip_test_2 = pkb.getNextLineTBip("2");
    REQUIRE(next_bip_test_2 == std::nullopt);

    std::unordered_set<std::string> next_bip_test_3_check;
    next_bip_test_3_check.insert("6");
    next_bip_test_3_check.insert("9");
    next_bip_test_3_check.insert("10");
    next_bip_test_3_check.insert("7");
    next_bip_test_3_check.insert("11");
    next_bip_test_3_check.insert("12");
    next_bip_test_3_check.insert("8");
    next_bip_test_3_check.insert("4");
    next_bip_test_3_check.insert("5");
    next_bip_test_3_check.insert("2");
    auto next_bip_test_3 = pkb.getNextLineTBip("3");
    REQUIRE(*next_bip_test_3 == next_bip_test_3_check);

    std::unordered_set<std::string> next_bip_test_4_check;
    next_bip_test_4_check.insert("5");
    next_bip_test_4_check.insert("2");
    auto next_bip_test_4 = pkb.getNextLineTBip("4");
    REQUIRE(*next_bip_test_4 == next_bip_test_4_check);

    std::unordered_set<std::string> next_bip_test_5_check;
    next_bip_test_5_check.insert("2");
    auto next_bip_test_5 = pkb.getNextLineTBip("5");
    REQUIRE(*next_bip_test_5 == next_bip_test_5_check);

    std::unordered_set<std::string> next_bip_test_6_check;
    next_bip_test_6_check.insert("9");
    next_bip_test_6_check.insert("10");
    next_bip_test_6_check.insert("7");
    next_bip_test_6_check.insert("11");
    next_bip_test_6_check.insert("12");
    next_bip_test_6_check.insert("8");
    next_bip_test_6_check.insert("4");
    next_bip_test_6_check.insert("5");
    next_bip_test_6_check.insert("2");
    auto next_bip_test_6 = pkb.getNextLineTBip("6");
    REQUIRE(*next_bip_test_6 == next_bip_test_6_check);

    std::unordered_set<std::string> next_bip_test_7_check;
    next_bip_test_7_check.insert("11");
    next_bip_test_7_check.insert("12");
    next_bip_test_7_check.insert("8");
    next_bip_test_7_check.insert("4");
    next_bip_test_7_check.insert("5");
    next_bip_test_7_check.insert("2");
    auto next_bip_test_7 = pkb.getNextLineTBip("7");
    REQUIRE(*next_bip_test_7 == next_bip_test_7_check);

    std::unordered_set<std::string> next_bip_test_8_check;
    next_bip_test_8_check.insert("4");
    next_bip_test_8_check.insert("5");
    next_bip_test_8_check.insert("2");
    auto next_bip_test_8 = pkb.getNextLineTBip("8");
    REQUIRE(*next_bip_test_8 == next_bip_test_8_check);

    std::unordered_set<std::string> next_bip_test_9_check;
    next_bip_test_9_check.insert("10");
    next_bip_test_9_check.insert("7");
    next_bip_test_9_check.insert("11");
    next_bip_test_9_check.insert("12");
    next_bip_test_9_check.insert("8");
    next_bip_test_9_check.insert("4");
    next_bip_test_9_check.insert("5");
    next_bip_test_9_check.insert("2");
    auto next_bip_test_9 = pkb.getNextLineTBip("9");
    REQUIRE(*next_bip_test_9 == next_bip_test_9_check);

    std::unordered_set<std::string> next_bip_test_10_check;
    next_bip_test_10_check.insert("7");
    next_bip_test_10_check.insert("11");
    next_bip_test_10_check.insert("12");
    next_bip_test_10_check.insert("8");
    next_bip_test_10_check.insert("4");
    next_bip_test_10_check.insert("5");
    next_bip_test_10_check.insert("2");
    auto next_bip_test_10 = pkb.getNextLineTBip("10");
    REQUIRE(*next_bip_test_10 == next_bip_test_10_check);

    std::unordered_set<std::string> next_bip_test_11_check;
    next_bip_test_11_check.insert("12");
    next_bip_test_11_check.insert("8");
    next_bip_test_11_check.insert("4");
    next_bip_test_11_check.insert("5");
    next_bip_test_11_check.insert("2");
    auto next_bip_test_11 = pkb.getNextLineTBip("11");
    REQUIRE(*next_bip_test_11 == next_bip_test_11_check);

    std::unordered_set<std::string> next_bip_test_12_check;
    next_bip_test_12_check.insert("8");
    next_bip_test_12_check.insert("4");
    next_bip_test_12_check.insert("5");
    next_bip_test_12_check.insert("2");
    auto next_bip_test_12 = pkb.getNextLineTBip("12");
    REQUIRE(*next_bip_test_12 == next_bip_test_12_check);

    std::unordered_set<std::string> next_bip_test_13_check;
    next_bip_test_13_check.insert("15");
    next_bip_test_13_check.insert("16");
    next_bip_test_13_check.insert("14");
    auto next_bip_test_13 = pkb.getNextLineTBip("13");
    REQUIRE(*next_bip_test_13 == next_bip_test_13_check);

    auto next_bip_test_14 = pkb.getNextLineTBip("14");
    REQUIRE(next_bip_test_14 == std::nullopt);

    std::unordered_set<std::string> next_bip_test_15_check;
    next_bip_test_15_check.insert("16");
    next_bip_test_15_check.insert("14");
    auto next_bip_test_15 = pkb.getNextLineTBip("15");
    REQUIRE(*next_bip_test_15 == next_bip_test_15_check);

    std::unordered_set<std::string> next_bip_test_16_check;
    next_bip_test_16_check.insert("14");
    auto next_bip_test_16 = pkb.getNextLineTBip("16");
    REQUIRE(*next_bip_test_16 == next_bip_test_16_check);
  }

  SECTION("Previous Bip") {
    auto previous_bip_test_1 = pkb.getPreviousLineBip("1");
    REQUIRE(previous_bip_test_1 == std::nullopt);

    std::unordered_set<std::string> previous_bip_test_2_check;
    previous_bip_test_2_check.insert("5");
    auto previous_bip_test_2 = pkb.getPreviousLineBip("2");
    REQUIRE(*previous_bip_test_2 == previous_bip_test_2_check);

    std::unordered_set<std::string> previous_bip_test_3_check;
    previous_bip_test_3_check.insert("1");
    auto previous_bip_test_3 = pkb.getPreviousLineBip("3");
    REQUIRE(*previous_bip_test_3 == previous_bip_test_3_check);

    std::unordered_set<std::string> previous_bip_test_4_check;
    previous_bip_test_4_check.insert("8");
    auto previous_bip_test_4 = pkb.getPreviousLineBip("4");
    REQUIRE(*previous_bip_test_4 == previous_bip_test_4_check);

    std::unordered_set<std::string> previous_bip_test_5_check;
    previous_bip_test_5_check.insert("4");
    auto previous_bip_test_5 = pkb.getPreviousLineBip("5");
    REQUIRE(*previous_bip_test_5 == previous_bip_test_5_check);

    std::unordered_set<std::string> previous_bip_test_6_check;
    previous_bip_test_6_check.insert("3");
    auto previous_bip_test_6 = pkb.getPreviousLineBip("6");
    REQUIRE(*previous_bip_test_6 == previous_bip_test_6_check);

    std::unordered_set<std::string> previous_bip_test_7_check;
    previous_bip_test_7_check.insert("10");
    auto previous_bip_test_7 = pkb.getPreviousLineBip("7");
    REQUIRE(*previous_bip_test_7 == previous_bip_test_7_check);

    std::unordered_set<std::string> previous_bip_test_8_check;
    previous_bip_test_8_check.insert("12");
    auto previous_bip_test_8 = pkb.getPreviousLineBip("8");
    REQUIRE(*previous_bip_test_8 == previous_bip_test_8_check);

    std::unordered_set<std::string> previous_bip_test_9_check;
    previous_bip_test_9_check.insert("6");
    auto previous_bip_test_9 = pkb.getPreviousLineBip("9");
    REQUIRE(*previous_bip_test_9 == previous_bip_test_9_check);

    std::unordered_set<std::string> previous_bip_test_10_check;
    previous_bip_test_10_check.insert("9");
    auto previous_bip_test_10 = pkb.getPreviousLineBip("10");
    REQUIRE(*previous_bip_test_10 == previous_bip_test_10_check);

    std::unordered_set<std::string> previous_bip_test_11_check;
    previous_bip_test_11_check.insert("7");
    auto previous_bip_test_11 = pkb.getPreviousLineBip("11");
    REQUIRE(*previous_bip_test_11 == previous_bip_test_11_check);

    std::unordered_set<std::string> previous_bip_test_12_check;
    previous_bip_test_12_check.insert("11");
    auto previous_bip_test_12 = pkb.getPreviousLineBip("12");
    REQUIRE(*previous_bip_test_12 == previous_bip_test_12_check);

    auto previous_bip_test_13 = pkb.getPreviousLineBip("13");
    REQUIRE(previous_bip_test_13 == std::nullopt);

    std::unordered_set<std::string> previous_bip_test_14_check;
    previous_bip_test_14_check.insert("16");
    auto previous_bip_test_14 = pkb.getPreviousLineBip("14");
    REQUIRE(*previous_bip_test_14 == previous_bip_test_14_check);

    std::unordered_set<std::string> previous_bip_test_15_check;
    previous_bip_test_15_check.insert("13");
    auto previous_bip_test_15 = pkb.getPreviousLineBip("15");
    REQUIRE(*previous_bip_test_15 == previous_bip_test_15_check);

    std::unordered_set<std::string> previous_bip_test_16_check;
    previous_bip_test_16_check.insert("15");
    auto previous_bip_test_16 = pkb.getPreviousLineBip("16");
    REQUIRE(*previous_bip_test_16 == previous_bip_test_16_check);
  }

  SECTION("PreviousT Bip") {
    auto previous_bip_test_1 = pkb.getPreviousLineTBip("1");
    REQUIRE(previous_bip_test_1 == std::nullopt);

    std::unordered_set<std::string> previous_bip_test_2_check;
    previous_bip_test_2_check.insert("5");
    previous_bip_test_2_check.insert("4");
    previous_bip_test_2_check.insert("8");
    previous_bip_test_2_check.insert("12");
    previous_bip_test_2_check.insert("11");
    previous_bip_test_2_check.insert("7");
    previous_bip_test_2_check.insert("10");
    previous_bip_test_2_check.insert("9");
    previous_bip_test_2_check.insert("6");
    previous_bip_test_2_check.insert("3");
    previous_bip_test_2_check.insert("1");
    auto previous_bip_test_2 = pkb.getPreviousLineTBip("2");
    REQUIRE(*previous_bip_test_2 == previous_bip_test_2_check);

    std::unordered_set<std::string> previous_bip_test_3_check;
    previous_bip_test_3_check.insert("1");
    auto previous_bip_test_3 = pkb.getPreviousLineTBip("3");
    REQUIRE(*previous_bip_test_3 == previous_bip_test_3_check);

    std::unordered_set<std::string> previous_bip_test_4_check;
    previous_bip_test_4_check.insert("8");
    previous_bip_test_4_check.insert("12");
    previous_bip_test_4_check.insert("11");
    previous_bip_test_4_check.insert("7");
    previous_bip_test_4_check.insert("10");
    previous_bip_test_4_check.insert("9");
    previous_bip_test_4_check.insert("6");
    previous_bip_test_4_check.insert("3");
    previous_bip_test_4_check.insert("1");
    auto previous_bip_test_4 = pkb.getPreviousLineTBip("4");
    REQUIRE(*previous_bip_test_4 == previous_bip_test_4_check);

    std::unordered_set<std::string> previous_bip_test_5_check;
    previous_bip_test_5_check.insert("4");
    previous_bip_test_5_check.insert("8");
    previous_bip_test_5_check.insert("12");
    previous_bip_test_5_check.insert("11");
    previous_bip_test_5_check.insert("7");
    previous_bip_test_5_check.insert("10");
    previous_bip_test_5_check.insert("9");
    previous_bip_test_5_check.insert("6");
    previous_bip_test_5_check.insert("3");
    previous_bip_test_5_check.insert("1");
    auto previous_bip_test_5 = pkb.getPreviousLineTBip("5");
    REQUIRE(*previous_bip_test_5 == previous_bip_test_5_check);

    std::unordered_set<std::string> previous_bip_test_6_check;
    previous_bip_test_6_check.insert("3");
    previous_bip_test_6_check.insert("1");
    auto previous_bip_test_6 = pkb.getPreviousLineTBip("6");
    REQUIRE(*previous_bip_test_6 == previous_bip_test_6_check);

    std::unordered_set<std::string> previous_bip_test_7_check;
    previous_bip_test_7_check.insert("10");
    previous_bip_test_7_check.insert("9");
    previous_bip_test_7_check.insert("6");
    previous_bip_test_7_check.insert("3");
    previous_bip_test_7_check.insert("1");
    auto previous_bip_test_7 = pkb.getPreviousLineTBip("7");
    REQUIRE(*previous_bip_test_7 == previous_bip_test_7_check);

    std::unordered_set<std::string> previous_bip_test_8_check;
    previous_bip_test_8_check.insert("12");
    previous_bip_test_8_check.insert("11");
    previous_bip_test_8_check.insert("7");
    previous_bip_test_8_check.insert("10");
    previous_bip_test_8_check.insert("9");
    previous_bip_test_8_check.insert("6");
    previous_bip_test_8_check.insert("3");
    previous_bip_test_8_check.insert("1");
    auto previous_bip_test_8 = pkb.getPreviousLineTBip("8");
    REQUIRE(*previous_bip_test_8 == previous_bip_test_8_check);

    std::unordered_set<std::string> previous_bip_test_9_check;
    previous_bip_test_9_check.insert("6");
    previous_bip_test_9_check.insert("3");
    previous_bip_test_9_check.insert("1");
    auto previous_bip_test_9 = pkb.getPreviousLineTBip("9");
    REQUIRE(*previous_bip_test_9 == previous_bip_test_9_check);

    std::unordered_set<std::string> previous_bip_test_10_check;
    previous_bip_test_10_check.insert("9");
    previous_bip_test_10_check.insert("6");
    previous_bip_test_10_check.insert("3");
    previous_bip_test_10_check.insert("1");
    auto previous_bip_test_10 = pkb.getPreviousLineTBip("10");
    REQUIRE(*previous_bip_test_10 == previous_bip_test_10_check);

    std::unordered_set<std::string> previous_bip_test_11_check;
    previous_bip_test_11_check.insert("7");
    previous_bip_test_11_check.insert("10");
    previous_bip_test_11_check.insert("9");
    previous_bip_test_11_check.insert("6");
    previous_bip_test_11_check.insert("3");
    previous_bip_test_11_check.insert("1");
    auto previous_bip_test_11 = pkb.getPreviousLineTBip("11");
    REQUIRE(*previous_bip_test_11 == previous_bip_test_11_check);

    std::unordered_set<std::string> previous_bip_test_12_check;
    previous_bip_test_12_check.insert("11");
    previous_bip_test_12_check.insert("7");
    previous_bip_test_12_check.insert("10");
    previous_bip_test_12_check.insert("9");
    previous_bip_test_12_check.insert("6");
    previous_bip_test_12_check.insert("3");
    previous_bip_test_12_check.insert("1");
    auto previous_bip_test_12 = pkb.getPreviousLineTBip("12");
    REQUIRE(*previous_bip_test_12 == previous_bip_test_12_check);

    auto previous_bip_test_13 = pkb.getPreviousLineTBip("13");
    REQUIRE(previous_bip_test_13 == std::nullopt);

    std::unordered_set<std::string> previous_bip_test_14_check;
    previous_bip_test_14_check.insert("16");
    previous_bip_test_14_check.insert("15");
    previous_bip_test_14_check.insert("13");
    auto previous_bip_test_14 = pkb.getPreviousLineTBip("14");
    REQUIRE(*previous_bip_test_14 == previous_bip_test_14_check);

    std::unordered_set<std::string> previous_bip_test_15_check;
    previous_bip_test_15_check.insert("13");
    auto previous_bip_test_15 = pkb.getPreviousLineTBip("15");
    REQUIRE(*previous_bip_test_15 == previous_bip_test_15_check);

    std::unordered_set<std::string> previous_bip_test_16_check;
    previous_bip_test_16_check.insert("15");
    previous_bip_test_16_check.insert("13");
    auto previous_bip_test_16 = pkb.getPreviousLineTBip("16");
    REQUIRE(*previous_bip_test_16 == previous_bip_test_16_check);
  }
}