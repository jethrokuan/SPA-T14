#include "program_knowledge_base/pkb_manager.h"
#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>

using Simple::SimpleInterface;

TEST_CASE ("Test PKB for next bip extension") {
  auto ast = SimpleInterface::getAstFromFile(
      "tests/simple_source/call/complex_call_structure.txt");
  PKB::PKBManager pkb = PKB::PKBManager(ast);

  SECTION ("Next Bip") {
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

  SECTION ("NextT Bip") {
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

  SECTION ("Previous Bip") {
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

  SECTION ("PreviousT Bip") {
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

TEST_CASE ("Test PKB for affects bip extension") {
  auto ast = SimpleInterface::getAstFromFile("tests/Test1-Source.txt");
  PKB::PKBManager pkb = PKB::PKBManager(ast);

  SECTION ("affects relations") {
    auto affects_test_1 = pkb.isLineAffectsLineBip("5", "6");
    REQUIRE(affects_test_1 == true);
    auto affects_test_2 = pkb.isLineAffectsLineBip("5", "7");
    REQUIRE(affects_test_2 == true);
    auto affects_test_3 = pkb.isLineAffectsLineBip("5", "8");
    REQUIRE(affects_test_3 == false);
    auto affects_test_4 = pkb.isLineAffectsLineBip("5", "9");
    REQUIRE(affects_test_4 == false);
    auto affects_test_5 = pkb.isLineAffectsLineBip("5", "10");
    REQUIRE(affects_test_5 == false);
    auto affects_test_6 = pkb.isLineAffectsLineBip("6", "7");
    REQUIRE(affects_test_6 == true);
    auto affects_test_7 = pkb.isLineAffectsLineBip("6", "8");
    REQUIRE(affects_test_7 == true);
    auto affects_test_8 = pkb.isLineAffectsLineBip("6", "9");
    REQUIRE(affects_test_8 == false);
    auto affects_test_9 = pkb.isLineAffectsLineBip("6", "10");
    REQUIRE(affects_test_9 == false);
    auto affects_test_10 = pkb.isLineAffectsLineBip("7", "8");
    REQUIRE(affects_test_10 == true);
    auto affects_test_11 = pkb.isLineAffectsLineBip("7", "9");
    REQUIRE(affects_test_11 == false);
    auto affects_test_12 = pkb.isLineAffectsLineBip("7", "10");
    REQUIRE(affects_test_12 == true);
    auto affects_test_13 = pkb.isLineAffectsLineBip("11", "12");
    REQUIRE(affects_test_13 == false);
    auto affects_test_14 = pkb.isLineAffectsLineBip("11", "13");
    REQUIRE(affects_test_14 == false);
    auto affects_test_15 = pkb.isLineAffectsLineBip("14", "14");
    REQUIRE(affects_test_15 == true);
    auto affects_test_16 = pkb.isLineAffectsLineBip("15", "13");
    REQUIRE(affects_test_16 == false);
    auto affects_test_17 = pkb.isLineAffectsLineBip("18", "17");
    REQUIRE(affects_test_17 == true);
    auto affects_test_18 = pkb.isLineAffectsLineBip("15", "19");
    REQUIRE(affects_test_18 == true);
    auto affects_test_19 = pkb.isLineAffectsLineBip("17", "19");
    REQUIRE(affects_test_19 == true);
    auto affects_test_20 = pkb.isLineAffectsLineBip("15", "20");
    REQUIRE(affects_test_20 == false);
    auto affects_test_21 = pkb.isLineAffectsLineBip("17", "20");
    REQUIRE(affects_test_21 == false);
    auto affects_test_22 = pkb.isLineAffectsLineBip("18", "20");
    REQUIRE(affects_test_22 == true);
    auto affects_test_23 = pkb.isLineAffectsLineBip("19", "20");
    REQUIRE(affects_test_23 == true);
    auto affects_test_24 = pkb.isLineAffectsLineBip("20", "21");
    REQUIRE(affects_test_24 == false);

    auto affects_test_25 = pkb.getAffectModifiesLineBip("1");
    REQUIRE(affects_test_25 == std::nullopt);

    auto affects_test_26 = pkb.getAffectModifiesLineBip("2");
    REQUIRE(affects_test_26 == std::nullopt);

    auto affects_test_27 = pkb.getAffectModifiesLineBip("3");
    REQUIRE(affects_test_27 == std::nullopt);

    auto affects_test_28 = pkb.getAffectModifiesLineBip("4");
    REQUIRE(affects_test_28 == std::nullopt);

    auto affects_test_29 = pkb.getAffectModifiesLineBip("5");
    REQUIRE(affects_test_29 == std::nullopt);

    std::unordered_set<std::string> affects_test_30_check;
    affects_test_30_check.insert("5");
    auto affects_test_30 = pkb.getAffectModifiesLineBip("6");
    REQUIRE(*affects_test_30 == affects_test_30_check);

    std::unordered_set<std::string> affects_test_31_check;
    affects_test_31_check.insert("5");
    affects_test_31_check.insert("6");
    auto affects_test_31 = pkb.getAffectModifiesLineBip("7");
    REQUIRE(*affects_test_31 == affects_test_31_check);

    std::unordered_set<std::string> affects_test_32_check;
    affects_test_32_check.insert("6");
    affects_test_32_check.insert("7");
    auto affects_test_32 = pkb.getAffectModifiesLineBip("8");
    REQUIRE(*affects_test_32 == affects_test_32_check);

    auto affects_test_33 = pkb.getAffectModifiesLineBip("9");
    REQUIRE(affects_test_33 == std::nullopt);

    std::unordered_set<std::string> affects_test_34_check;
    affects_test_34_check.insert("7");
    affects_test_34_check.insert("18");
    auto affects_test_34 = pkb.getAffectModifiesLineBip("10");
    REQUIRE(*affects_test_34 == affects_test_34_check);

    auto affects_test_35 = pkb.isLineAffectsLineTBip("5", "6");
    REQUIRE(affects_test_35 == true);
    auto affects_test_36 = pkb.isLineAffectsLineTBip("5", "7");
    REQUIRE(affects_test_36 == true);
    auto affects_test_37 = pkb.isLineAffectsLineTBip("5", "8");
    REQUIRE(affects_test_37 == true);
    auto affects_test_38 = pkb.isLineAffectsLineTBip("5", "9");
    REQUIRE(affects_test_38 == false);
    auto affects_test_39 = pkb.isLineAffectsLineTBip("5", "10");
    REQUIRE(affects_test_39 == true);
    auto affects_test_40 = pkb.isLineAffectsLineTBip("6", "7");
    REQUIRE(affects_test_40 == true);
    auto affects_test_41 = pkb.isLineAffectsLineTBip("6", "8");
    REQUIRE(affects_test_41 == true);
    auto affects_test_42 = pkb.isLineAffectsLineTBip("6", "9");
    REQUIRE(affects_test_42 == false);
    auto affects_test_43 = pkb.isLineAffectsLineTBip("6", "10");
    REQUIRE(affects_test_43 == true);
    auto affects_test_44 = pkb.isLineAffectsLineTBip("7", "8");
    REQUIRE(affects_test_44 == true);
    auto affects_test_45 = pkb.isLineAffectsLineTBip("7", "9");
    REQUIRE(affects_test_45 == false);
    auto affects_test_46 = pkb.isLineAffectsLineTBip("7", "10");
    REQUIRE(affects_test_46 == true);
    auto affects_test_47 = pkb.isLineAffectsLineTBip("11", "12");
    REQUIRE(affects_test_47 == false);
    auto affects_test_48 = pkb.isLineAffectsLineTBip("11", "13");
    REQUIRE(affects_test_48 == false);
    auto affects_test_49 = pkb.isLineAffectsLineTBip("14", "14");
    REQUIRE(affects_test_49 == true);
    auto affects_test_50 = pkb.isLineAffectsLineTBip("14", "20");
    REQUIRE(affects_test_50 == false);
    auto affects_test_51 = pkb.isLineAffectsLineTBip("18", "17");
    REQUIRE(affects_test_51 == true);
    auto affects_test_52 = pkb.isLineAffectsLineTBip("15", "19");
    REQUIRE(affects_test_52 == true);
    auto affects_test_53 = pkb.isLineAffectsLineTBip("17", "19");
    REQUIRE(affects_test_53 == true);
    auto affects_test_54 = pkb.isLineAffectsLineTBip("15", "20");
    REQUIRE(affects_test_54 == true);
    auto affects_test_55 = pkb.isLineAffectsLineTBip("17", "20");
    REQUIRE(affects_test_55 == true);
    auto affects_test_56 = pkb.isLineAffectsLineTBip("18", "20");
    REQUIRE(affects_test_56 == true);
    auto affects_test_57 = pkb.isLineAffectsLineTBip("19", "20");
    REQUIRE(affects_test_57 == true);
    auto affects_test_58 = pkb.isLineAffectsLineTBip("18", "18");
    REQUIRE(affects_test_58 == true);

    auto affects_test_59 = pkb.getAffectUsesLineTBip("1");
    REQUIRE(affects_test_59 == std::nullopt);

    auto affects_test_60 = pkb.getAffectUsesLineTBip("2");
    REQUIRE(affects_test_60 == std::nullopt);

    auto affects_test_61 = pkb.getAffectUsesLineTBip("3");
    REQUIRE(affects_test_61 == std::nullopt);

    auto affects_test_62 = pkb.getAffectUsesLineTBip("4");
    REQUIRE(affects_test_62 == std::nullopt);

    std::unordered_set<std::string> affects_test_63_check;
    affects_test_63_check.insert("6");
    affects_test_63_check.insert("7");
    affects_test_63_check.insert("8");
    affects_test_63_check.insert("10");
    affects_test_63_check.insert("14");
    affects_test_63_check.insert("17");
    affects_test_63_check.insert("18");
    affects_test_63_check.insert("19");
    affects_test_63_check.insert("20");
    auto affects_test_63 = pkb.getAffectUsesLineTBip("5");
    REQUIRE(*affects_test_63 == affects_test_63_check);

    std::unordered_set<std::string> affects_test_64_check;
    affects_test_64_check.insert("7");
    affects_test_64_check.insert("8");
    affects_test_64_check.insert("10");
    affects_test_64_check.insert("14");
    affects_test_64_check.insert("17");
    affects_test_64_check.insert("18");
    affects_test_64_check.insert("19");
    affects_test_64_check.insert("20");
    auto affects_test_64 = pkb.getAffectUsesLineTBip("6");
    REQUIRE(*affects_test_64 == affects_test_64_check);

    std::unordered_set<std::string> affects_test_65_check;
    affects_test_65_check.insert("8");
    affects_test_65_check.insert("10");
    affects_test_65_check.insert("14");
    affects_test_65_check.insert("17");
    affects_test_65_check.insert("18");
    affects_test_65_check.insert("19");
    affects_test_65_check.insert("20");
    auto affects_test_65 = pkb.getAffectUsesLineTBip("7");
    REQUIRE(*affects_test_65 == affects_test_65_check);

    std::unordered_set<std::string> affects_test_66_check;
    affects_test_66_check.insert("19");
    affects_test_66_check.insert("20");
    auto affects_test_66 = pkb.getAffectUsesLineTBip("8");
    REQUIRE(*affects_test_66 == affects_test_66_check);

    auto affects_test_67 = pkb.getAffectUsesLineTBip("9");
    REQUIRE(affects_test_67 == std::nullopt);

    auto affects_test_68 = pkb.getAffectUsesLineTBip("10");
    REQUIRE(affects_test_68 == std::nullopt);

    auto affects_test_69 = pkb.getAffectUsesLineTBip("11");
    REQUIRE(affects_test_69 == std::nullopt);

    auto affects_test_70 = pkb.getAffectUsesLineTBip("12");
    REQUIRE(affects_test_70 == std::nullopt);

    auto affects_test_71 = pkb.getAffectUsesLineTBip("13");
    REQUIRE(affects_test_71 == std::nullopt);

    std::unordered_set<std::string> affects_test_72_check;
    affects_test_72_check.insert("14");
    auto affects_test_72 = pkb.getAffectUsesLineTBip("14");
    REQUIRE(*affects_test_72 == affects_test_72_check);

    std::unordered_set<std::string> affects_test_73_check;
    affects_test_73_check.insert("19");
    affects_test_73_check.insert("20");
    auto affects_test_73 = pkb.getAffectUsesLineTBip("15");
    REQUIRE(*affects_test_73 == affects_test_73_check);

    auto affects_test_74 = pkb.getAffectUsesLineTBip("16");
    REQUIRE(affects_test_74 == std::nullopt);

    std::unordered_set<std::string> affects_test_75_check;
    affects_test_75_check.insert("10");
    affects_test_75_check.insert("17");
    affects_test_75_check.insert("18");
    affects_test_75_check.insert("19");
    affects_test_75_check.insert("20");
    auto affects_test_75 = pkb.getAffectUsesLineTBip("17");
    REQUIRE(*affects_test_75 == affects_test_75_check);

    std::unordered_set<std::string> affects_test_76_check;
    affects_test_76_check.insert("10");
    affects_test_76_check.insert("17");
    affects_test_76_check.insert("18");
    affects_test_76_check.insert("19");
    affects_test_76_check.insert("20");
    auto affects_test_76 = pkb.getAffectUsesLineTBip("18");
    REQUIRE(*affects_test_76 == affects_test_76_check);

    std::unordered_set<std::string> affects_test_77_check;
    affects_test_77_check.insert("20");
    auto affects_test_77 = pkb.getAffectUsesLineTBip("19");
    REQUIRE(*affects_test_77 == affects_test_77_check);

    auto affects_test_78 = pkb.getAffectUsesLineTBip("20");
    REQUIRE(affects_test_78 == std::nullopt);

    auto affects_test_79 = pkb.getAffectUsesLineTBip("21");
    REQUIRE(affects_test_79 == std::nullopt);

    auto affects_test_80 = pkb.getAffectModifiesLineTBip("1");
    REQUIRE(affects_test_80 == std::nullopt);

    auto affects_test_81 = pkb.getAffectModifiesLineTBip("2");
    REQUIRE(affects_test_81 == std::nullopt);

    auto affects_test_82 = pkb.getAffectModifiesLineTBip("3");
    REQUIRE(affects_test_82 == std::nullopt);

    auto affects_test_83 = pkb.getAffectModifiesLineTBip("4");
    REQUIRE(affects_test_83 == std::nullopt);

    auto affects_test_84 = pkb.getAffectModifiesLineTBip("5");
    REQUIRE(affects_test_84 == std::nullopt);

    std::unordered_set<std::string> affects_test_85_check;
    affects_test_85_check.insert("5");
    auto affects_test_85 = pkb.getAffectModifiesLineTBip("6");
    REQUIRE(*affects_test_85 == affects_test_85_check);

    std::unordered_set<std::string> affects_test_86_check;
    affects_test_86_check.insert("5");
    affects_test_86_check.insert("6");
    auto affects_test_86 = pkb.getAffectModifiesLineTBip("7");
    REQUIRE(*affects_test_86 == affects_test_86_check);

    std::unordered_set<std::string> affects_test_87_check;
    affects_test_87_check.insert("5");
    affects_test_87_check.insert("6");
    affects_test_87_check.insert("7");
    auto affects_test_87 = pkb.getAffectModifiesLineTBip("8");
    REQUIRE(*affects_test_87 == affects_test_87_check);

    auto affects_test_88 = pkb.getAffectModifiesLineTBip("9");
    REQUIRE(affects_test_88 == std::nullopt);

    std::unordered_set<std::string> affects_test_89_check;
    affects_test_89_check.insert("5");
    affects_test_89_check.insert("6");
    affects_test_89_check.insert("7");
    affects_test_89_check.insert("17");
    affects_test_89_check.insert("18");
    auto affects_test_89 = pkb.getAffectModifiesLineTBip("10");
    REQUIRE(*affects_test_89 == affects_test_89_check);

    auto affects_test_90 = pkb.getAffectModifiesLineTBip("11");
    REQUIRE(affects_test_90 == std::nullopt);

    auto affects_test_91 = pkb.getAffectModifiesLineTBip("12");
    REQUIRE(affects_test_91 == std::nullopt);

    auto affects_test_92 = pkb.getAffectModifiesLineTBip("13");
    REQUIRE(affects_test_92 == std::nullopt);

    std::unordered_set<std::string> affects_test_93_check;
    affects_test_93_check.insert("5");
    affects_test_93_check.insert("6");
    affects_test_93_check.insert("7");
    affects_test_93_check.insert("14");
    auto affects_test_93 = pkb.getAffectModifiesLineTBip("14");
    REQUIRE(*affects_test_93 == affects_test_93_check);

    auto affects_test_94 = pkb.getAffectModifiesLineTBip("15");
    REQUIRE(affects_test_94 == std::nullopt);

    auto affects_test_95 = pkb.getAffectModifiesLineTBip("16");
    REQUIRE(affects_test_95 == std::nullopt);

    std::unordered_set<std::string> affects_test_96_check;
    affects_test_96_check.insert("5");
    affects_test_96_check.insert("6");
    affects_test_96_check.insert("7");
    affects_test_96_check.insert("17");
    affects_test_96_check.insert("18");
    auto affects_test_96 = pkb.getAffectModifiesLineTBip("17");
    REQUIRE(*affects_test_96 == affects_test_96_check);

    std::unordered_set<std::string> affects_test_97_check;
    affects_test_97_check.insert("5");
    affects_test_97_check.insert("6");
    affects_test_97_check.insert("7");
    affects_test_97_check.insert("17");
    affects_test_97_check.insert("18");
    auto affects_test_97 = pkb.getAffectModifiesLineTBip("18");
    REQUIRE(*affects_test_97 == affects_test_97_check);

    std::unordered_set<std::string> affects_test_98_check;
    affects_test_98_check.insert("5");
    affects_test_98_check.insert("6");
    affects_test_98_check.insert("7");
    affects_test_98_check.insert("8");
    affects_test_98_check.insert("15");
    affects_test_98_check.insert("17");
    affects_test_98_check.insert("18");
    auto affects_test_98 = pkb.getAffectModifiesLineTBip("19");
    REQUIRE(*affects_test_98 == affects_test_98_check);

    std::unordered_set<std::string> affects_test_99_check;
    affects_test_99_check.insert("5");
    affects_test_99_check.insert("6");
    affects_test_99_check.insert("7");
    affects_test_99_check.insert("8");
    affects_test_99_check.insert("15");
    affects_test_99_check.insert("17");
    affects_test_99_check.insert("18");
    affects_test_99_check.insert("19");
    auto affects_test_99 = pkb.getAffectModifiesLineTBip("20");
    REQUIRE(*affects_test_99 == affects_test_99_check);
  }
}