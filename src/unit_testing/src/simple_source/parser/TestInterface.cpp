#include "simple_parser/interface.h"
#include "structs/node.h"

#include "catch.hpp"

#include <memory>

using namespace Simple;
using std::make_shared;

TEST_CASE ("Test SIMPLE interface") {
  SECTION ("getAstFromFile") {
    auto ast =
        SimpleInterface::getAstFromFile("tests/simple_source/read/valid.txt");

    std::vector<StmtNode> stmtList;

    auto read_x = make_shared<ReadNode>(make_shared<VariableNode>("x"));

    stmtList.push_back(std::move(read_x));

    auto expected =
        make_shared<ProcedureNode>("main", make_shared<StmtListNode>(stmtList));

    REQUIRE(*ast == *expected);
  }

  SECTION ("parseExpression") {
    std::string expression = "1 + 2";
    Expr expr = SimpleInterface::parseExpression(expression);

    Expr expected = make_shared<BinOpNode>(make_shared<NumberNode>("1"),
                                           make_shared<NumberNode>("2"), "+");

    REQUIRE(
        std::visit([](auto& t, auto& o) { return *t == *o; }, expr, expected));
  }
}
