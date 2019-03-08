#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test multi-procedure parses") {
  SECTION ("x = 1") {
    auto ast =
        SimpleInterface::getAstFromFile("tests/simple_source/multiproc.txt");

    std::vector<StmtNode> stmt_list_1;
    std::vector<StmtNode> stmt_list_2;

    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    auto assign_1 = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                                            make_shared<NumberNode>("1"));
    stmt_list_1.push_back(std::move(assign_1));

    auto proc_main = make_shared<ProcedureNode>("main", stmt_list_1);

    auto assign_2 = make_shared<AssignNode>(make_shared<VariableNode>("y"),
                                            make_shared<NumberNode>("2"));
    stmt_list_2.push_back(std::move(assign_2));

    auto proc_bar = make_shared<ProcedureNode>("bar", stmt_list_2);

    proc_list.push_back(std::move(proc_main));
    proc_list.push_back(std::move(proc_bar));

    auto root = make_shared<RootNode>(std::move(proc_list));

    REQUIRE(ast->ProcList.size() == 2);
    REQUIRE(*ast == *root);
  }
}
