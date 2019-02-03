#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::shared_ptr;

TEST_CASE ("Test Parser works") {
  std::string filename = "tests/simple_source/simple_1.txt";
  std::ifstream input(filename);

  Simple::Lexer lexer = Simple::Lexer(input);

  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();
  std::vector<shared_ptr<Node>> stmtList;

  // i = 5;
  auto assign = std::make_shared<AssignNode>(
      make_shared<VariableNode>("i"),
      make_shared<ExprNode>(make_shared<TermNode>(
          make_shared<FactorNode>(make_shared<NumberNode>("5")))));

  // while (i==5) {
  //   print i;
  // }

  std::vector<shared_ptr<Node>> whileStmtList;
  auto print_j = make_shared<PrintNode>(make_shared<VariableNode>("j"));
  whileStmtList.push_back(std::move(print_j));

  std::string op = "==";
  auto whileStmt = make_shared<WhileNode>(
      make_shared<CondExprNode>(make_shared<RelExprNode>(
          make_shared<RelFactorNode>(make_shared<VariableNode>("i")), op,
          make_shared<RelFactorNode>(make_shared<NumberNode>("5")))),
      make_shared<StmtListNode>(std::move(whileStmtList)));

  // print i;
  auto print_i = make_shared<PrintNode>(make_shared<VariableNode>("i"));

  // read i;
  auto read = make_shared<ReadNode>(make_shared<VariableNode>("i"));

  stmtList.push_back(std::move(assign));
  stmtList.push_back(std::move(whileStmt));
  stmtList.push_back(std::move(print_i));
  stmtList.push_back(std::move(read));

  auto StmtList = std::make_shared<StmtListNode>(std::move(stmtList));

  auto expected = std::make_shared<ProcedureNode>(
      std::make_shared<VariableNode>("main"), std::move(StmtList));

  REQUIRE(*proc == *expected);
}

TEST_CASE ("Node equality comparisons") {
  SECTION ("NumberNode") {
    auto n1 = make_shared<NumberNode>("1");
    auto n2 = make_shared<NumberNode>("1");
    auto n3 = make_shared<NumberNode>("2");

    REQUIRE(*n1 == *n2);
    REQUIRE(*n1 != *n3);
  }

  SECTION ("VariableNode") {
    auto v1 = make_shared<VariableNode>("i");
    auto v2 = make_shared<VariableNode>("i");
    auto v3 = make_shared<VariableNode>("j");

    REQUIRE(*v1 == *v2);
    REQUIRE(*v1 != *v3);
  }

  SECTION ("ReadNode") {
    auto r1 = make_shared<ReadNode>(make_shared<VariableNode>("i"));
    auto r2 = make_shared<ReadNode>(make_shared<VariableNode>("i"));
    auto r3 = make_shared<ReadNode>(make_shared<VariableNode>("j"));

    REQUIRE(*r1 == *r2);
    REQUIRE(*r1 != *r3);
  }

  SECTION ("PrintNode") {
    auto p1 = make_shared<ReadNode>(make_shared<VariableNode>("i"));
    auto p2 = make_shared<ReadNode>(make_shared<VariableNode>("i"));
    auto p3 = make_shared<ReadNode>(make_shared<VariableNode>("j"));

    REQUIRE(*p1 == *p2);
    REQUIRE(*p1 != *p3);
  }

  SECTION ("FactorNode") {
    auto f_var1 = make_shared<FactorNode>(make_shared<VariableNode>("i"));
    auto f_var2 = make_shared<FactorNode>(make_shared<VariableNode>("i"));
    auto f_var3 = make_shared<FactorNode>(make_shared<VariableNode>("j"));

    REQUIRE(*f_var1 == *f_var2);
    REQUIRE(*f_var1 != *f_var3);

    auto f_val1 = make_shared<FactorNode>(make_shared<NumberNode>("1"));
    auto f_val2 = make_shared<FactorNode>(make_shared<NumberNode>("1"));
    auto f_val3 = make_shared<FactorNode>(make_shared<NumberNode>("2"));

    REQUIRE(*f_val1 == *f_val2);
    REQUIRE(*f_val1 != *f_val3);

    // TODO: Test f_expr

    REQUIRE(*f_val1 != *f_var1);
  }

  SECTION ("StmtListNode") {
    auto r1 = make_shared<ReadNode>(make_shared<VariableNode>("i"));
    auto r2 = make_shared<ReadNode>(make_shared<VariableNode>("i"));
    auto r3 = make_shared<ReadNode>(make_shared<VariableNode>("j"));

    std::vector<shared_ptr<Node>> s1;
    std::vector<shared_ptr<Node>> s2;
    std::vector<shared_ptr<Node>> s3;

    s1.push_back(std::move(r1));
    s2.push_back(std::move(r2));
    s3.push_back(std::move(r3));

    auto sl1 = make_shared<StmtListNode>(std::move(s1));
    auto sl2 = make_shared<StmtListNode>(std::move(s2));
    auto sl3 = make_shared<StmtListNode>(std::move(s3));

    REQUIRE(*sl1 == *sl2);
    REQUIRE(*sl1 != *sl3);
  }

  SECTION ("ProcedureNode") {
    auto r1 = make_shared<ReadNode>(make_shared<VariableNode>("i"));
    auto r2 = make_shared<ReadNode>(make_shared<VariableNode>("i"));
    auto r3 = make_shared<ReadNode>(make_shared<VariableNode>("j"));

    std::vector<shared_ptr<Node>> s1;
    std::vector<shared_ptr<Node>> s2;
    std::vector<shared_ptr<Node>> s3;

    s1.push_back(std::move(r1));
    s2.push_back(std::move(r2));
    s3.push_back(std::move(r3));

    auto sl1 = make_shared<StmtListNode>(std::move(s1));
    auto sl2 = make_shared<StmtListNode>(std::move(s2));
    auto sl3 = make_shared<StmtListNode>(std::move(s3));

    auto p1 = make_shared<ProcedureNode>(make_shared<VariableNode>("main"),
                                         std::move(sl1));
    auto p2 = make_shared<ProcedureNode>(make_shared<VariableNode>("main"),
                                         std::move(sl2));
    auto p3 = make_shared<ProcedureNode>(make_shared<VariableNode>("main2"),
                                         std::move(sl2));
    auto p4 = make_shared<ProcedureNode>(make_shared<VariableNode>("main"),
                                         std::move(sl3));

    REQUIRE(*p1 == *p2);
    REQUIRE(*p1 != *p3);
    REQUIRE(*p1 != *p4);
  }

  // TODO: Test RelExprNode

  // TODO: Test WhileNode
};
