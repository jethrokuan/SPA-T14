#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "query_builder/core/query_lexer.h"

using QE::QueryLexer;

TEST_CASE ("Test Query Lexer") {
  SECTION (
      "procedure p, q; Select p such that Calls (p, q) with  q.stmt# = "
      "\"Third\" such that "
      "Modifies (p, \"i\")") {
    std::string query =
        "procedure p, q; Select p such that Calls (p, q) with  q.stmt# = "
        "\"Third\" such that Modifies (p, \"i\")";

    QueryLexer lexer = QueryLexer(query);
    auto tokens = lexer.lex();
    auto expected = std::vector<std::string>(
        {"procedure", "p",    ",",     "q",    ";",        "Select", "p",
         "such",      "that", "Calls", "(",    "p",        ",",      "q",
         ")",         "with", "q",     ".",    "stmt#",    "=",      "\"",
         "Third",     "\"",   "such",  "that", "Modifies", "(",      "p",
         ",",         "\"",   "i",     "\"",   ")",        ""});

    REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                       end(expected),
                       [](const std::string l, const std::string o) {
                         return l.compare(o) == 0;
                       }));
  }

  SECTION ("assign a;  Select a pattern a (_, _\"x*y+z\"_)") {
    std::string query = "assign a;  Select a pattern a (_, _\"x*y+z\"_)";
    QueryLexer lexer = QueryLexer(query);
    auto tokens = lexer.lex();
    auto expected = std::vector<std::string>(
        {"assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",", "_",
         "\"", "x*y+z", "\"", "_", ")", ""});

    REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                       end(expected),
                       [](const std::string l, const std::string o) {
                         return l.compare(o) == 0;
                       }));
  }

  SECTION ("assign a;  Select a pattern a (_, _\"x*y+z\"_)") {
    std::string query = "assign a;  Select a pattern a (_, _\"x*y+z\"_)";
    QueryLexer lexer = QueryLexer(query);
    auto tokens = lexer.lex();
    auto expected = std::vector<std::string>(
        {"assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",", "_",
         "\"", "x*y+z", "\"", "_", ")", ""});

    REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                       end(expected),
                       [](const std::string l, const std::string o) {
                         return l.compare(o) == 0;
                       }));
  }
}
