#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_unique;
using std::unique_ptr;

TEST_CASE ("Test Parser works") {
  std::string filename = "tests/simple_source/simple_1.txt";
  std::ifstream input(filename);

  Simple::Lexer lexer = Simple::Lexer(input);

  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();
  std::vector<unique_ptr<Node>> stmtList;

  // i = 5;
  auto assign = std::make_unique<AssignNode>(
      make_unique<VariableNode>("i"),
      make_unique<ExprNode>(make_unique<TermNode>(
          make_unique<FactorNode>(make_unique<NumberNode>("5")))));

  // print i;
  auto print = make_unique<PrintNode>(make_unique<VariableNode>("i"));

  // read i;
  auto read = make_unique<ReadNode>(make_unique<VariableNode>("i"));

  stmtList.push_back(std::move(assign));
  stmtList.push_back(std::move(print));
  stmtList.push_back(std::move(read));

  auto expected = std::make_unique<ProcedureNode>(
      std::make_unique<VariableNode>("main"), std::move(stmtList));

  REQUIRE(*proc == *expected);
}

TEST_CASE ("Node equality comparisons") {
  SECTION ("NumberNode") {
    auto n1 = make_unique<NumberNode>("1");
    auto n2 = make_unique<NumberNode>("1");
    auto n3 = make_unique<NumberNode>("2");

    REQUIRE(*n1 == *n2);
    REQUIRE(*n1 != *n3);
  }

  SECTION ("VariableNode") {
    auto v1 = make_unique<VariableNode>("i");
    auto v2 = make_unique<VariableNode>("i");
    auto v3 = make_unique<VariableNode>("j");

    REQUIRE(*v1 == *v2);
    REQUIRE(*v1 != *v3);
  }

  SECTION ("ReadNode") {
    auto r1 = make_unique<ReadNode>(make_unique<VariableNode>("i"));
    auto r2 = make_unique<ReadNode>(make_unique<VariableNode>("i"));
    auto r3 = make_unique<ReadNode>(make_unique<VariableNode>("j"));

    REQUIRE(*r1 == *r2);
    REQUIRE(*r1 != *r3);
  }

  SECTION ("PrintNode") {
    auto p1 = make_unique<ReadNode>(make_unique<VariableNode>("i"));
    auto p2 = make_unique<ReadNode>(make_unique<VariableNode>("i"));
    auto p3 = make_unique<ReadNode>(make_unique<VariableNode>("j"));

    REQUIRE(*p1 == *p2);
    REQUIRE(*p1 != *p3);
  }

  SECTION ("FactorNode") {
    auto f_var1 = make_unique<FactorNode>(make_unique<VariableNode>("i"));
    auto f_var2 = make_unique<FactorNode>(make_unique<VariableNode>("i"));
    auto f_var3 = make_unique<FactorNode>(make_unique<VariableNode>("j"));

    REQUIRE(*f_var1 == *f_var2);
    REQUIRE(*f_var1 != *f_var3);

    auto f_val1 = make_unique<FactorNode>(make_unique<NumberNode>("1"));
    auto f_val2 = make_unique<FactorNode>(make_unique<NumberNode>("1"));
    auto f_val3 = make_unique<FactorNode>(make_unique<NumberNode>("2"));

    REQUIRE(*f_val1 == *f_val2);
    REQUIRE(*f_val1 != *f_val3);

    // TODO: Test f_expr

    REQUIRE(*f_val1 != *f_var1);
  }

  SECTION ("ProcedureNode") {
    auto r1 = make_unique<ReadNode>(make_unique<VariableNode>("i"));
    auto r2 = make_unique<ReadNode>(make_unique<VariableNode>("i"));
    auto r3 = make_unique<ReadNode>(make_unique<VariableNode>("j"));

    std::vector<unique_ptr<Node>> s1;
    std::vector<unique_ptr<Node>> s2;
    std::vector<unique_ptr<Node>> s3;

    s1.push_back(std::move(r1));
    s2.push_back(std::move(r2));
    s3.push_back(std::move(r3));

    auto p1 = make_unique<ProcedureNode>(make_unique<VariableNode>("main"),
                                         std::move(s1));
    auto p2 = make_unique<ProcedureNode>(make_unique<VariableNode>("main"),
                                         std::move(s2));
    auto p3 = make_unique<ProcedureNode>(make_unique<VariableNode>("main2"),
                                         std::move(s2));
    auto p4 = make_unique<ProcedureNode>(make_unique<VariableNode>("main"),
                                         std::move(s3));

    REQUIRE(*p1 == *p2);
    REQUIRE(*p1 != *p3);
    REQUIRE(*p1 != *p4);
  }
};
