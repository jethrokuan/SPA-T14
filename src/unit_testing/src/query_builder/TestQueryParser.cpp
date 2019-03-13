#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "query_builder/core/query_lexer.h"
#include "query_builder/core/query_parser.h"
#include "query_builder/pql/query.h"

using QE::DesignEntity;
using QE::Query;
using QE::QueryLexer;
using QE::QueryParser;

TEST_CASE ("Test Query Parser") {
  SECTION ("Test one assign one select query") {
    std::string input = "assign a;";
    QueryLexer lexer = QueryLexer(input);
    lexer.lex();
    QueryParser parser = QueryParser(lexer.Tokens);
    Query query = parser.parse();
    REQUIRE(*query.declarations ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("a").value())});
    REQUIRE(query.such_that->empty());
    REQUIRE(query.pattern->empty());
  }
}
