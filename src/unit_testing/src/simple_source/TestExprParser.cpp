#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::shared_ptr;

TEST_CASE ("Test Expr parse works") {
  SECTION ("i = 5") {
    std::string filename = "tests/simple_source/arithmetic/1.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    auto proc = parser.parse();
    std::vector<StmtNode> stmtList;

    auto assign = std::make_shared<AssignNode>(make_shared<VariableNode>("i"),
                                               make_shared<NumberNode>("5"));

    stmtList.push_back(std::move(assign));

    auto StmtList = std::make_shared<StmtListNode>(std::move(stmtList));

    auto expected =
        std::make_shared<ProcedureNode>("main", std::move(StmtList));

    REQUIRE(*proc == *expected);
  }

  SECTION ("i = 2 + 5") {
    std::string filename = "tests/simple_source/arithmetic/2.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    auto proc = parser.parse();
    std::vector<StmtNode> stmtList;

    auto assign = std::make_shared<AssignNode>(
        make_shared<VariableNode>("i"),
        make_shared<BinOpNode>(make_shared<NumberNode>("2"),
                               make_shared<NumberNode>("5"), "+"));

    stmtList.push_back(std::move(assign));

    auto StmtList = std::make_shared<StmtListNode>(std::move(stmtList));

    auto expected =
        std::make_shared<ProcedureNode>("main", std::move(StmtList));

    REQUIRE(*proc == *expected);
  }

  SECTION ("i = 2 + 5 * j") {
    std::string filename = "tests/simple_source/arithmetic/3.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    auto proc = parser.parse();
    std::vector<StmtNode> stmtList;

    auto assign = std::make_shared<AssignNode>(
        make_shared<VariableNode>("i"),
        make_shared<BinOpNode>(
            make_shared<NumberNode>("2"),
            make_shared<BinOpNode>(make_shared<NumberNode>("5"),
                                   make_shared<VariableNode>("j"), "*"),
            "+"));

    stmtList.push_back(std::move(assign));

    auto StmtList = std::make_shared<StmtListNode>(std::move(stmtList));

    auto expected =
        std::make_shared<ProcedureNode>("main", std::move(StmtList));

    REQUIRE(*proc == *expected);
  }

  SECTION ("i = 2 + (5 * j)") {
    std::string filename = "tests/simple_source/arithmetic/4.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    auto proc = parser.parse();
    std::vector<StmtNode> stmtList;

    auto assign = std::make_shared<AssignNode>(
        make_shared<VariableNode>("i"),
        make_shared<BinOpNode>(
            make_shared<NumberNode>("2"),
            make_shared<BinOpNode>(make_shared<NumberNode>("5"),
                                   make_shared<VariableNode>("j"), "*"),
            "+"));

    stmtList.push_back(std::move(assign));

    auto StmtList = std::make_shared<StmtListNode>(std::move(stmtList));

    auto expected =
        std::make_shared<ProcedureNode>("main", std::move(StmtList));

    REQUIRE(*proc == *expected);
  }

  SECTION ("i = (2 + 5) * j") {
    std::string filename = "tests/simple_source/arithmetic/5.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    auto proc = parser.parse();
    std::vector<StmtNode> stmtList;

    auto assign = std::make_shared<AssignNode>(
        make_shared<VariableNode>("i"),
        make_shared<BinOpNode>(
            make_shared<BinOpNode>(make_shared<NumberNode>("2"),
                                   make_shared<NumberNode>("5"), "+"),
            make_shared<VariableNode>("j"), "*"));

    stmtList.push_back(std::move(assign));

    auto StmtList = std::make_shared<StmtListNode>(std::move(stmtList));

    auto expected =
        std::make_shared<ProcedureNode>("main", std::move(StmtList));

    REQUIRE(*proc == *expected);
  }

  SECTION ("while (i > (2 + 5) * j)") {
    std::string filename = "tests/simple_source/arithmetic/6.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    auto proc = parser.parse();

    std::vector<StmtNode> stmtList;

    std::vector<StmtNode> ws;
    auto assign = make_shared<AssignNode>(
        make_shared<VariableNode>("i"),
        make_shared<BinOpNode>(make_shared<VariableNode>("i"),
                               make_shared<NumberNode>("1"), "+"));

    ws.push_back(std::move(assign));

    auto w = std::make_shared<WhileNode>(
        make_shared<CondExprNode>(make_shared<RelExprNode>(
            make_shared<VariableNode>("i"), ">",
            make_shared<BinOpNode>(
                make_shared<BinOpNode>(make_shared<NumberNode>("2"),
                                       make_shared<NumberNode>("5"), "+"),
                make_shared<VariableNode>("j"), "*"))),
        make_shared<StmtListNode>(std::move(ws)));

    stmtList.push_back(std::move(w));

    auto StmtList = std::make_shared<StmtListNode>(std::move(stmtList));

    auto expected =
        std::make_shared<ProcedureNode>("main", std::move(StmtList));

    REQUIRE(*proc == *expected);
  }

  SECTION ("valid assign statements") {
    std::string filename = "tests/simple_source/arithmetic/valid.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    REQUIRE_NOTHROW(parser.parse());
  }
}

TEST_CASE ("Test Expr parse throws when invalid") {
  SECTION ("consecutive operands") {
    std::string filename =
        "tests/simple_source/arithmetic/consecutive_operands.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    REQUIRE_THROWS_WITH(parser.parse(),
                        "Expected operator or semicolon, got '9'.");
  }

  SECTION ("double operators") {
    std::string filename =
        "tests/simple_source/arithmetic/double_operators.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    REQUIRE_THROWS_WITH(parser.parse(),
                        "Expected a number, variable or expression, got '-'.");
  }

  SECTION ("empty parentheses") {
    std::string filename =
        "tests/simple_source/arithmetic/empty_parentheses.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    REQUIRE_THROWS_WITH(parser.parse(),
                        "Expected a number, variable or expression, got ')'.");
  }

  SECTION ("incomplete parentheses") {
    std::string filename =
        "tests/simple_source/arithmetic/incomplete_parentheses.txt";
    std::ifstream input(filename);

    Lexer lexer = Lexer(input);
    lexer.parse();

    Parser parser = Parser(lexer.tokens);
    REQUIRE_THROWS_WITH(parser.parse(), "Expected ')', got ';'.");
  }

  SECTION ("other invalid expressions") {
    int NUM_TESTCASES = 21;
    for (int i = 0; i < NUM_TESTCASES; i++) {
      std::string filename = "tests/simple_source/arithmetic/other_invalids/" +
                             std::to_string(i) + ".txt";
      std::ifstream input(filename);

      Lexer lexer = Lexer(input);
      lexer.parse();

      Parser parser = Parser(lexer.tokens);
      REQUIRE_THROWS(parser.parse());
    }
  }
}
