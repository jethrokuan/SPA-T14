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

    std::vector<StmtNode> stmt_list;
    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    auto read_x = make_shared<ReadNode>(make_shared<VariableNode>("x"));

    stmt_list.push_back(std::move(read_x));

    auto proc_main = make_shared<ProcedureNode>("main", stmt_list);
    proc_list.push_back(proc_main);
    auto root = make_shared<RootNode>(proc_list);

    REQUIRE(*ast == *root);
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
