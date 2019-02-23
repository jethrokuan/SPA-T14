/*!
 * \file   parser.h
 * \brief  Parser for the SIMPLE language.
 *
 * Specifies the necessary methods required to parse the
 * SIMPLE language via gradient descent. Each node is encapsulated in a
 * shared_ptr, to ease memory management issues.
 *
 * \date   2019-02-05
 */

#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "simple_parser/pratt.h"
#include "simple_parser/token.h"
#include "structs/node.h"

//! Contains classes, methods and functions for the SIMPLE language
namespace Simple {
//! Parser for the SIMPLE language
/*!
 * The SIMPLE language is specified as the following CSG:
 *
 *     program: procedure
 *     procedure: 'procedure' proc_name {' stmtLst '}'
 *     stmtLst: stmt+
 *     stmt: read
 *       | print
 *       | while
 *       | if
 *       | assign
 *     read: 'read' var_name;
 *     print: 'print' var_name;
 *     while: 'while' '(' cond_expr ')' '{' stmtLst '}'
 *     if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
 *     assign: var_name '=' expr ';'
 *     cond_expr: rel_expr
 *       | '!' '(' cond_expr ')'
 *       | '(' cond_expr ')'  '&&' '(' cond_expr ')'
 *       | '(' cond_expr ')' '||' '(' cond_expr ')'
 *     rel_expr: rel_factor '>' rel_factor
 *       | rel_factor '>=' rel_factor
 *       | rel_factor '<' rel_factor
 *       | rel_factor '<=' rel_factor
 *       | rel_factor '==' rel_factor
 *       | rel_factor '!=' rel_factor
 *     rel_factor: var_name
 *       | const_value
 *       | expr
 *     expr: const_value | variable | bin_op
 *     bin_op: expr '+' expr | expr '-' expr | expr '*' expr
 *       | expr '/' expr | expr '%' expr
 *     factor: var_name | const_value | '(' expr ')'
 *
 */

using AST = std::shared_ptr<ProcedureNode>;

class Parser {
 private:
  //! Current index in `tokens`
  int current = 0;
  ExprParser exprParser;

  //! The vector of lexical tokens
  std::vector<Token*> tokens;

  //! Checks if token at the current index has TokenType t. If
  //! successful, consumes the token, advancing the index.
  bool match(TokenType t);

  //! Checks if token at the current index has value equal to string
  //! s. If successful, consumes the token, advancing the index.
  bool match(std::string s);

  //! Returns true if token at the current index is of TokenType t.
  bool check(TokenType type);

  //! Returns true if token at the current index has value string s.
  bool check(std::string s);

  //! Asserts the token at current index to be of stings s.
  //! If true, consumes the token. Else, throws an exception.
  bool expect(std::string s);

  //! Returns true if the parser is at the end of file.
  bool isAtEnd();

  //! Returns the current token
  Token* peek();

  //! Returns the previous token
  Token* previous();

  //! Returns the current token, and consumes it.
  Token* advance();

  //! Parses a Number expression.
  /*!
    This is const_value in the CSG.
  */
  std::shared_ptr<NumberNode> parseNumber();

  //! Parses a Variable expression.
  /*!
    This is var_name in the CSG.
  */
  std::shared_ptr<VariableNode> parseVariable();

  //! Parses a Procedure expression.
  /*!
    procedure: 'procedure' proc_name {' stmtLst '}'
  */
  std::shared_ptr<ProcedureNode> parseProcedure();

  //! Parses a Statement List.
  /*!
    stmtLst: stmt+
  */
  std::shared_ptr<StmtListNode> parseStmtList();

  //! Parses a Statement.
  /*!
    stmt: read | print | while | if | assign
  */
  std::optional<StmtNode> parseStatement();

  //! Parses a Factor expression.
  /*!
    factor: var_name | const_value | '(' expr ')'
   */
  Factor parseFactor();

  //! Parses a Rel Factor expression.
  /*!
    rel_factor: var_name | const_value | expr
   */
  RelFactor parseRelFactor();

  //! Parses a expr expression.
  /*!
    expr: term expr'
   */
  Expr parseExpr();

  //! Parses a rel_expr expression.
  /*!
    rel_expr: rel_factor '>' rel_factor
        | rel_factor '>=' rel_factor
        | rel_factor '<' rel_factor
        | rel_factor '<=' rel_factor
        | rel_factor '==' rel_factor
        | rel_factor '!=' rel_factor
   */
  std::shared_ptr<RelExprNode> parseRelExpr();

  //! Parses a assign expression.
  /*!
    assign: var_name '=' expr ';'
   */
  std::shared_ptr<AssignNode> parseAssign();

  //! Parses a read expression.
  /*!
    read: 'read' var_name;
   */
  std::shared_ptr<ReadNode> parseRead();

  //! Parses a print expression.
  /*!
    print: 'print' var_name;
   */
  std::shared_ptr<PrintNode> parsePrint();

  //! Parses a cond_expr expression.
  /*! cond_expr: rel_expr
    | '!' '(' cond_expr ')'
    | '(' cond_expr ')'  '&&' '(' cond_expr ')'
    | '(' cond_expr ')' '||' '(' cond_expr ')'
  */
  std::shared_ptr<CondExprNode> parseCondExpr();

  //! Parses a while expression.
  /*!
    while: 'while' '(' cond_expr ')' '{' stmtLst '}'
  */
  std::shared_ptr<WhileNode> parseWhile();

  //! Parses an if expression.
  /*!
    if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
  */
  std::shared_ptr<IfNode> parseIf();

 public:
  // ! Initializes the parser with the lexical tokens.
  explicit Parser(std::vector<Token*> t);

  // ! Parses the lexical tokens provided, returning the root node in the AST.
  AST parse();
};
}  // namespace Simple
