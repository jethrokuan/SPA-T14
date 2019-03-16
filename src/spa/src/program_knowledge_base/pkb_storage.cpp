#include "program_knowledge_base/pkb_storage.h"
#include <iostream>
#include "program_knowledge_base/pkb_exceptions.h"

namespace PKB {

PKBStorage::PKBStorage(){};
PKBStorage::~PKBStorage(){};

void PKBStorage::storeAST(const AST ast_node) { ast = ast_node; };

Line PKBStorage::storeLine(const std::shared_ptr<Node> node) {
  const Line cur_line_num = getCurLineNumber();
  incrementCurLineNumber();
  line_node_map[cur_line_num] = node;
  node_line_map[node] = cur_line_num;

  return cur_line_num;
}

Line PKBStorage::getCurLineNumber() { return std::to_string(num_lines); }

void PKBStorage::incrementCurLineNumber() { num_lines = num_lines + 1; }

Line PKBStorage::getLineFromNode(const std::shared_ptr<Node> node) {
  if (node_line_map.find(node) != node_line_map.end()) {
    return node_line_map.at(node);
  } else {
    assert(false);
  }
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
  // add to edge list
  line_previous_line_next_set.insert(std::pair(source, dest));
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

void PKBStorage::storeVariable(const Variable var) {
  if (procedure_set.find(var) != procedure_set.end()) {
    // Throw an error if there's a procedure with the same name as
    // the variable
    throw PkbAstSemanticException(
        "Found procedure and variable with the same name: '" + var + "'.");
  }
  var_set.insert(var);
}

void PKBStorage::storeAssign(const Line line, const Variable var) {
  assign_set.insert(line);
  assign_line_var_set.insert(std::pair<Line, Variable>(line, var));
  addToSetMap(assign_var_line_map, var, line);
}

void PKBStorage::storeStatement(const Line line) { statement_set.insert(line); }

void PKBStorage::storePrint(const Line line) { print_set.insert(line); }

void PKBStorage::storeRead(const Line line) { read_set.insert(line); }

void PKBStorage::storeWhile(const Line line) { while_set.insert(line); }

void PKBStorage::storeIf(const Line line) { if_set.insert(line); }

void PKBStorage::storeConstant(const Constant num) { constant_set.insert(num); }

void PKBStorage::storeProcedure(const Procedure proc) {
  if (var_set.find(proc) != var_set.end()) {
    // Throw an error if there's a procedure with the same name as
    // the variable
    throw PkbAstSemanticException(
        "Found procedure and variable with the same name: '" + proc + "'.");
  }
  procedure_set.insert(proc);
}

void PKBStorage::storeCall(const Line line, const Procedure proc) {
  call_set.insert(line);
  line_calls_procedure_set.insert(std::pair<Line, Procedure>(line, proc));
}

void PKBStorage::storeLineProcedureRelation(const Line line,
                                            const Procedure proc) {
  line_procedure_map[line] = proc;
}

// pattern

void PKBStorage::storePatternAssign(const Variable var, const ExprStr expr_str,
                                    const Line line) {
  addToSetMap(var_line_expr_str_map, var,
              std::pair<Line, ExprStr>(line, expr_str));
  addToSetMap(expr_str_line_var_map, expr_str,
              std::pair<Line, Variable>(line, var));
  // std::cout << var + " at line " + line + " maps to " + expr_str <<
  // std::endl;
  var_expr_str_set.insert(std::pair<Variable, ExprStr>(var, expr_str));
  line_expr_str_set.insert(std::pair<Line, ExprStr>(line, expr_str));
  expr_str_set.insert(expr_str);
}

Procedure PKBStorage::getProcedureFromLine(const Line line) {
  if (line_procedure_map.find(line) != line_procedure_map.end()) {
    return line_procedure_map.at(line);
  } else {
    assert(false);
  }
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
