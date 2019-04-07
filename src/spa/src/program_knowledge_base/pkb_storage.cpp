#include "program_knowledge_base/pkb_storage.h"
#include <iostream>
#include "program_knowledge_base/pkb_exceptions.h"

namespace PKB {

PKBStorage::PKBStorage(){};
PKBStorage::~PKBStorage(){};

void PKBStorage::storeAST(const AST ast_node) { ast = ast_node; };

Line PKBStorage::storeLine(const StmtNode node) {
  const Line cur_line_num = getCurLineNumber();
  incrementCurLineNumber();
  std::visit(
      [this, cur_line_num](const auto &s) {
        line_node_map[cur_line_num] = s;
        node_line_map[s] = cur_line_num;
      },
      node);

  return cur_line_num;
}

Line PKBStorage::getCurLineNumber() { return std::to_string(num_lines); }

void PKBStorage::incrementCurLineNumber() { num_lines = num_lines + 1; }

Line PKBStorage::getLineFromNode(const StmtNode node) {
  return std::visit(
      [this](const auto &s) {
        if (node_line_map.find(s) != node_line_map.end()) {
          return node_line_map.at(s);
        } else {
          assert(false);
        }
      },
      node);
}

std::optional<std::shared_ptr<Node>> PKBStorage::getNodeFromLine(
    const Line line) {
  if (line_node_map.find(line) != line_node_map.end()) {
    return std::optional<std::shared_ptr<Node>>(line_node_map.at(line));
  } else {
    return std::nullopt;
  }
}

void PKBStorage::storeCFGEdge(const Line source, const Line dest) {
  // std::cout << source + " " + dest << std::endl;
  // add to adjacency list
  addToSetMap(line_previous_line_next_map, source, dest);
  addToSetMap(line_next_line_previous_map, dest, source);
}

void PKBStorage::storeCallsRelation(const ProcedureCaller procedure_caller,
                                    const ProcedureCallee procedure_callee) {
  if (procedure_caller.compare(procedure_callee) == 0) {
    throw PkbAstSemanticException("Found recursive call to procedure: '" +
                                  procedure_caller + "'.");
  }
  procedure_calls_procedure_set.insert(
      std::pair(procedure_caller, procedure_callee));
  addToSetMap(procedure_caller_procedure_callee_map, procedure_caller,
              procedure_callee);
  addToSetMap(procedure_callee_procedure_caller_map, procedure_callee,
              procedure_caller);
}

void PKBStorage::storeCallsRelationS(const ProcedureCaller procedure_caller,
                                     const ProcedureCallee procedure_callee) {
  if (procedure_caller.compare(procedure_callee) == 0) {
    throw PkbAstSemanticException("Found cyclic call between procedures.");
  }
  procedure_calls_procedure_set_t.insert(
      std::pair(procedure_caller, procedure_callee));
  addToSetMap(procedure_caller_procedure_callee_map_t, procedure_caller,
              procedure_callee);
  addToSetMap(procedure_callee_procedure_caller_map_t, procedure_callee,
              procedure_caller);
}

void PKBStorage::storeFollowsRelation(const LineBefore line_before,
                                      const LineAfter line_after) {
  follows_set.insert(std::pair<LineBefore, LineAfter>(line_before, line_after));
  line_before_line_after_map[line_before] = line_after;
  line_after_line_before_map[line_after] = line_before;
}

void PKBStorage::storeFollowsRelationS(const LineBefore line_before,
                                       const LineAfter line_after) {
  follows_set_t.insert(
      std::pair<LineBefore, LineAfter>(line_before, line_after));
  addToSetMap(line_before_line_after_map_t, line_before, line_after);
  addToSetMap(line_after_line_before_map_t, line_after, line_before);
}

void PKBStorage::storeParentRelation(const ParentLine parent_line,
                                     const ChildLine child_line) {
  parent_set.insert(std::pair<ParentLine, ChildLine>(parent_line, child_line));
  child_line_parent_line_map[child_line] = parent_line;
  addToSetMap(parent_line_child_line_map, parent_line, child_line);
}

void PKBStorage::storeParentRelationS(const ParentLine parent_line,
                                      const ChildLine child_line) {
  parent_set_t.insert(
      std::pair<ParentLine, ChildLine>(parent_line, child_line));
  addToSetMap(child_line_parent_line_map_t, child_line, parent_line);
  addToSetMap(parent_line_child_line_map_t, parent_line, child_line);
}

void PKBStorage::storeProcedureUsesVarRelation(const Procedure proc,
                                               const Variable var) {
  procedure_uses_var_set.insert(std::pair<Procedure, Variable>(proc, var));
  addToSetMap(procedure_uses_var_map, var, proc);
  addToSetMap(var_used_by_procedure_map, proc, var);
}

void PKBStorage::storeLineUsesVarRelation(const Line line, const Variable var) {
  line_uses_var_set.insert(std::pair<Line, Variable>(line, var));
  addToSetMap(line_uses_var_map, var, line);
  addToSetMap(var_used_by_line_map, line, var);
}

void PKBStorage::storeProcedureModifiesVarRelation(const Procedure proc,
                                                   const Variable var) {
  procedure_modifies_var_set.insert(std::pair<Procedure, Variable>(proc, var));
  addToSetMap(procedure_modifies_var_map, var, proc);
  addToSetMap(var_modified_by_procedure_map, proc, var);
}

void PKBStorage::storeLineModifiesVarRelation(const Line line,
                                              const Variable var) {
  line_modifies_var_set.insert(std::pair<Line, Variable>(line, var));
  addToSetMap(line_modifies_var_map, var, line);
  addToSetMap(var_modified_by_line_map, line, var);
}

void PKBStorage::storeVariable(const Variable var) { var_set.insert(var); }

void PKBStorage::storeAssign(const Line line) { assign_set.insert(line); }

void PKBStorage::storeStatement(const Line line) { statement_set.insert(line); }

void PKBStorage::storePrint(const Line line, const Variable var) {
  print_set.insert(line);
  line_print_var_map[line] = var;
}

void PKBStorage::storeRead(const Line line, const Variable var) {
  read_set.insert(line);
  line_read_var_map[line] = var;
}

void PKBStorage::storeWhile(const Line line) { while_set.insert(line); }

void PKBStorage::storeIf(const Line line) { if_set.insert(line); }

void PKBStorage::storeConstant(const Constant num) { constant_set.insert(num); }

void PKBStorage::storeProcedure(const Procedure proc) {
  if (procedure_set.find(proc) != procedure_set.end()) {
    throw PkbAstSemanticException(
        "Found multiple procedures with the same name: '" + proc + "'.");
  }

  procedure_set.insert(proc);
}

void PKBStorage::storeCall(const Line line, const Procedure proc) {
  if (procedure_set.find(proc) == procedure_set.end()) {
    // Throw an error if there's a call to a non existent procedure
    throw PkbAstSemanticException(
        "Found call statement to non-existing procedure: '" + proc + "'.");
  } else {
    call_set.insert(line);
    line_calls_procedure_set.insert(std::pair<Line, Procedure>(line, proc));
    line_calls_procedure_map[line] = proc;
  }
}

void PKBStorage::storeLineProcedureRelation(const Line line,
                                            const Procedure proc) {
  line_procedure_map[line] = proc;
}

// pattern

void PKBStorage::storeAssignPattern(const Variable var, const ExprStr expr_str,
                                    const Line line) {
  assign_pattern_line_var_set.insert(std::pair<Line, Variable>(line, var));
  addToSetMap(assign_pattern_var_line_map, var, line);

  addToSetMap(assign_pattern_var_line_expr_str_map, var,
              std::pair<Line, ExprStr>(line, expr_str));
  addToSetMap(assign_pattern_expr_str_line_var_map, expr_str,
              std::pair<Line, Variable>(line, var));
  // std::cout << var + " at line " + line + " maps to " + expr_str <<
  // std::endl;
  assign_pattern_var_expr_str_set.insert(
      std::pair<Variable, ExprStr>(var, expr_str));
  assign_pattern_line_expr_str_set.insert(
      std::pair<Line, ExprStr>(line, expr_str));
  assign_pattern_expr_str_set.insert(expr_str);
}

void PKBStorage::storeIfPattern(const Variable var, const Line line) {
  // std::cout << "if " + var + " at line " + line << std::endl;
  if_pattern_line_var_set.insert(std::pair<Line, Variable>(line, var));
  // addToSetMap(if_pattern_var_line_map, var, line);
  addToSetMap(if_pattern_line_control_variable_map, line, var);
  addToSetMap(if_pattern_control_variable_line_map, var, line);
}

void PKBStorage::storeWhilePattern(const Variable var, const Line line) {
  // std::cout << "while " + var + " at line " + line << std::endl;
  while_pattern_line_var_set.insert(std::pair<Line, Variable>(line, var));
  // addToSetMap(while_pattern_var_line_map, var, line);
  addToSetMap(while_pattern_line_control_variable_map, line, var);
  addToSetMap(while_pattern_control_variable_line_map, var, line);
}

Procedure PKBStorage::getProcedureFromLine(const Line line) {
  if (line_procedure_map.find(line) != line_procedure_map.end()) {
    return line_procedure_map.at(line);
  } else {
    assert(false);
  }
}

void PKBStorage::storeAssignLineModifiesVariable(const Line line,
                                                 const Variable var) {
  assign_line_modifies_variable_map[line] = var;
}

void PKBStorage::storeAssignLineUsesVariable(const Line line,
                                             const Variable var) {
  addToSetMap(assign_line_uses_variable_map, line, var);
}

// helper
void PKBStorage::addToSetMap(
    std::unordered_map<std::string, std::unordered_set<std::string>> &umap,
    const std::string index, std::string data) {
  if (umap.find(index) == umap.end()) {
    // create new set
    std::unordered_set<std::string> v;
    v.insert(data);
    umap[index] = v;
  } else {
    // retrieve vector and add element
    umap.at(index).insert(data);
  }
}

void PKBStorage::addToSetMap(
    std::unordered_map<std::string,
                       std::unordered_set<std::pair<std::string, std::string>,
                                          pair_hash>> &umap,
    const std::string index, std::pair<std::string, std::string> data) {
  if (umap.find(index) == umap.end()) {
    // create new vector
    std::unordered_set<std::pair<std::string, std::string>, pair_hash> v;
    v.insert(data);
    umap[index] = v;
  } else {
    // retrieve vector and add element
    umap.at(index).insert(data);
  }
}

}  // namespace PKB
