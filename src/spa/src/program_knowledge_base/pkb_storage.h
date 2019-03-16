#pragma once
#include <iostream>

#include <cassert>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_definitions.h"
#include "structs/node.h"
#include "utils/utils.h"

using namespace Utils;

namespace PKB {

class PKBStorage {
 private:
  int num_lines = 1;
  std::unordered_map<Line, std::shared_ptr<Node>> line_node_map;
  std::unordered_map<std::shared_ptr<Node>, Line> node_line_map;
  std::unordered_map<std::shared_ptr<std::string>, std::string> test;
  // helper
  Line getCurLineNumber();
  void incrementCurLineNumber();
  std::unordered_map<Line, Procedure> line_procedure_map;
  void addToSetMap(
      std::unordered_map<std::string, std::unordered_set<std::string>> &umap,
      const std::string index, std::string data);
  void addToSetMap(
      std::unordered_map<Variable,
                         std::unordered_set<std::pair<std::string, std::string>,
                                            pair_hash>> &umap,
      const std::string index, std::pair<std::string, std::string> data);

 public:
  PKBStorage();
  ~PKBStorage();
  // attributes
  AST ast;
  // CFG
  std::unordered_set<std::pair<std::string, std::string>, pair_hash>
      line_previous_line_next_set;  // edge list
  std::unordered_map<std::string, std::unordered_set<std::string>>
      line_previous_line_next_map;  // adjacency list
  std::unordered_map<std::string, std::unordered_set<std::string>>
      line_next_line_previous_map;  // reversed adjacaency list

  Line getLineFromNode(const std::shared_ptr<Node> node);
  std::optional<std::shared_ptr<Node>> getNodeFromLine(const Line line);

  // calls
  std::unordered_set<std::pair<Line, Procedure>, pair_hash>
      line_calls_procedure_set;
  std::unordered_set<std::pair<ProcedureCaller, ProcedureCallee>, pair_hash>
      procedure_calls_procedure_set;
  std::unordered_set<std::pair<ProcedureCaller, ProcedureCallee>, pair_hash>
      procedure_calls_procedure_set_t;
  std::unordered_map<ProcedureCaller, std::unordered_set<ProcedureCallee>>
      procedure_caller_procedure_callee_map;
  std::unordered_map<ProcedureCallee, std::unordered_set<ProcedureCaller>>
      procedure_callee_procedure_caller_map;
  std::unordered_map<ProcedureCaller, std::unordered_set<ProcedureCallee>>
      procedure_caller_procedure_callee_map_t;
  std::unordered_map<ProcedureCallee, std::unordered_set<ProcedureCaller>>
      procedure_callee_procedure_caller_map_t;

  // follows
  std::unordered_set<std::pair<LineBefore, LineAfter>, pair_hash> follows_set;
  std::unordered_set<std::pair<LineBefore, LineAfter>, pair_hash> follows_set_t;
  std::unordered_map<LineBefore, LineAfter> line_before_line_after_map;
  std::unordered_map<LineAfter, LineBefore> line_after_line_before_map;
  std::unordered_map<LineBefore, std::unordered_set<LineAfter>>
      line_before_line_after_map_t;
  std::unordered_map<LineAfter, std::unordered_set<LineBefore>>
      line_after_line_before_map_t;

  // parent
  std::unordered_set<std::pair<ParentLine, ChildLine>, pair_hash> parent_set;
  std::unordered_set<std::pair<ParentLine, ChildLine>, pair_hash> parent_set_t;
  std::unordered_map<ChildLine, ParentLine> child_line_parent_line_map;
  std::unordered_map<ParentLine, std::unordered_set<ChildLine>>
      parent_line_child_line_map;
  std::unordered_map<ChildLine, std::unordered_set<ParentLine>>
      child_line_parent_line_map_t;
  std::unordered_map<ParentLine, std::unordered_set<ChildLine>>
      parent_line_child_line_map_t;

  // uses
  std::unordered_set<std::pair<Procedure, Variable>, pair_hash>
      procedure_uses_var_set;
  std::unordered_set<std::pair<Line, Variable>, pair_hash> line_uses_var_set;
  std::unordered_map<Procedure, std::unordered_set<Variable>>
      var_used_by_procedure_map;
  std::unordered_map<Line, std::unordered_set<Variable>> var_used_by_line_map;
  std::unordered_map<Variable, std::unordered_set<Procedure>>
      procedure_uses_var_map;
  std::unordered_map<Variable, std::unordered_set<Line>> line_uses_var_map;

  // modifies
  std::unordered_set<std::pair<Procedure, Variable>, pair_hash>
      procedure_modifies_var_set;
  std::unordered_set<std::pair<Line, Variable>, pair_hash>
      line_modifies_var_set;
  std::unordered_map<Procedure, std::unordered_set<Variable>>
      var_modified_by_procedure_map;
  std::unordered_map<Line, std::unordered_set<Variable>>
      var_modified_by_line_map;
  std::unordered_map<Variable, std::unordered_set<Procedure>>
      procedure_modifies_var_map;
  std::unordered_map<Variable, std::unordered_set<Line>> line_modifies_var_map;

  // design entities
  std::unordered_set<Variable> var_set;
  std::unordered_set<Line> assign_set;
  std::unordered_set<Line> statement_set;
  std::unordered_set<Line> print_set;
  std::unordered_set<Line> read_set;
  std::unordered_set<Line> while_set;
  std::unordered_set<Line> if_set;
  std::unordered_set<Constant> constant_set;
  std::unordered_set<Procedure> procedure_set;
  std::unordered_set<Line> call_set;

  // pattern
  std::unordered_map<Variable,
                     std::unordered_set<std::pair<Line, ExprStr>, pair_hash>>
      var_line_expr_str_map;
  std::unordered_map<ExprStr,
                     std::unordered_set<std::pair<Line, Variable>, pair_hash>>
      expr_str_line_var_map;
  std::unordered_set<std::pair<Variable, ExprStr>, pair_hash> var_expr_str_set;
  std::unordered_set<std::pair<Line, ExprStr>, pair_hash> line_expr_str_set;
  std::unordered_set<ExprStr> expr_str_set;

  // misc helpers
  // only assign, does not include read, so not all modifies fulfill this
  std::unordered_set<std::pair<Line, Variable>, pair_hash> assign_line_var_set;
  std::unordered_map<Variable, std::unordered_set<Line>> assign_var_line_map;

  // setters
  void storeAST(const AST);
  Line storeLine(const std::shared_ptr<Node> node);
  void storeCFGEdge(const Line, const Line);

  void storeCallsRelation(const ProcedureCaller, const ProcedureCallee);
  void storeCallsRelationS(const ProcedureCaller, const ProcedureCallee);
  void storeFollowsRelation(const LineBefore, const LineAfter);
  void storeFollowsRelationS(const LineBefore, const LineAfter);
  void storeParentRelation(const ParentLine, const ChildLine);
  void storeParentRelationS(const ParentLine, const ChildLine);
  void storeProcedureUsesVarRelation(const Procedure, const Variable);
  void storeLineUsesVarRelation(const Line, const Variable);
  void storeProcedureModifiesVarRelation(const Procedure, const Variable);
  void storeLineModifiesVarRelation(const Line, const Variable);

  void storeVariable(const Variable);
  void storeAssign(const Line, const Variable);
  void storeStatement(const Line);
  void storePrint(const Line);
  void storeRead(const Line);
  void storeWhile(const Line);
  void storeIf(const Line);
  void storeConstant(const Constant);
  void storeProcedure(const Procedure);
  void storeCall(const Line, const Procedure);

  void storePatternAssign(const Variable, const ExprStr, const Line);

  // helper
  void storeLineProcedureRelation(const Line, const Procedure);
  Procedure getProcedureFromLine(const Line);
};

}  // namespace PKB
