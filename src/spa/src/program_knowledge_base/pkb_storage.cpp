#include "program_knowledge_base/pkb_storage.h"
#include <iostream>
#include "program_knowledge_base/pkb_exceptions.h"

namespace PKB {

PKBStorage::PKBStorage(){};
PKBStorage::~PKBStorage(){};

void PKBStorage::storeAST(const std::shared_ptr<ProcedureNode> proc) {
  ast = proc;
};

Line PKBStorage::storeLine(const std::shared_ptr<Node> node) {
  lines.push_back(node);
  return std::to_string(lines.size());
}

// TODO error handling
// TODO refactor to use hashmap instead
// would need to implement hash function of the node
// as well as a hash table
// return line number
// returns empty string if node does not exist
std::string PKBStorage::getLineFromNode(const std::shared_ptr<Node> node) {
  // loop through vector of nodes
  for (std::size_t i = 0; i < lines.size(); i++) {
    if (lines[i] == node) {
      return std::to_string(i + 1);  // +1 due to 0 based index
    }
  }
  return "";  // TODO error handling
}

void PKBStorage::storeFollowsRelation(const LineBefore line_before,
                                      const LineAfter line_after) {
  follows_set.insert(std::pair<LineBefore, LineAfter>(line_before, line_after));
  line_before_line_after_map[line_before] = line_after;
  line_after_line_before_map[line_after] = line_before;
}

void PKBStorage::storeFollowsRelationS(const LineBefore line_before,
                                       const LineAfter line_after) {
  follows_set_s.insert(
      std::pair<LineBefore, LineAfter>(line_before, line_after));
  addToVectorMap(line_before_line_after_map_s, line_before, line_after);
  addToVectorMap(line_after_line_before_map_s, line_after, line_before);
}

void PKBStorage::storeParentRelation(const ParentLine parent_line,
                                     const ChildLine child_line) {
  parent_set.insert(std::pair<ParentLine, ChildLine>(parent_line, child_line));
  child_line_parent_line_map[child_line] = parent_line;
  addToVectorMap(parent_line_child_line_map, parent_line, child_line);
}

void PKBStorage::storeParentRelationS(const ParentLine parent_line,
                                      const ChildLine child_line) {
  parent_set_s.insert(
      std::pair<ParentLine, ChildLine>(parent_line, child_line));
  addToVectorMap(child_line_parent_line_map_s, child_line, parent_line);
  addToVectorMap(parent_line_child_line_map_s, parent_line, child_line);
}

void PKBStorage::storeProcedureUsesVarRelation(const Procedure proc,
                                               const Variable var) {
  procedure_uses_var_set.insert(std::pair<Procedure, Variable>(proc, var));
  addToVectorMap(procedure_uses_var_map, var, proc);
  addToVectorMap(var_used_by_procedure_map, proc, var);
}

void PKBStorage::storeLineUsesVarRelation(const Line line, const Variable var) {
  line_uses_var_set.insert(std::pair<Line, Variable>(line, var));
  addToVectorMap(line_uses_var_map, var, line);
  addToVectorMap(var_used_by_line_map, line, var);
}

void PKBStorage::storeProcedureModifiesVarRelation(const Procedure proc,
                                                   const Variable var) {
  procedure_modifies_var_set.insert(std::pair<Procedure, Variable>(proc, var));
  addToVectorMap(procedure_modifies_var_map, var, proc);
  addToVectorMap(var_modified_by_procedure_map, proc, var);
}

void PKBStorage::storeLineModifiesVarRelation(const Line line,
                                              const Variable var) {
  line_modifies_var_set.insert(std::pair<Line, Variable>(line, var));
  addToVectorMap(line_modifies_var_map, var, line);
  addToVectorMap(var_modified_by_line_map, line, var);
}

void PKBStorage::storeVariable(const Variable var) {
  if (procedure_set.find(var) != procedure_set.end()) {
    // Throw an error if there's a procedure with the same name as
    // the variable
    throw PkbAstSemanticException(
        "Found procedure and variable with the same name: '" + var + "'.");
  }
  var_set.insert(var);
  var_list.push_back(var);
}

void PKBStorage::storeAssign(const Line line) {
  assign_set.insert(line);
  assign_list.push_back(line);
}

void PKBStorage::storeStatement(const Line line) {
  statement_set.insert(line);
  statement_list.push_back(line);
}

void PKBStorage::storePrint(const Line line) {
  print_set.insert(line);
  print_list.push_back(line);
}

void PKBStorage::storeRead(const Line line) {
  read_set.insert(line);
  read_list.push_back(line);
}

void PKBStorage::storeWhile(const Line line) {
  while_set.insert(line);
  while_list.push_back(line);
}

void PKBStorage::storeIf(const Line line) {
  if_set.insert(line);
  if_list.push_back(line);
}

void PKBStorage::storeConstant(const Constant num) {
  constant_set.insert(num);
  constant_list.push_back(num);
}

void PKBStorage::storeProcedure(const Procedure proc) {
  if (var_set.find(proc) != var_set.end()) {
    // Throw an error if there's a procedure with the same name as
    // the variable
    throw PkbAstSemanticException(
        "Found procedure and variable with the same name: '" + proc + "'.");
  }
  procedure_set.insert(proc);
  procedure_list.push_back(proc);
}

void PKBStorage::storeLineProcedureRelation(const Line line,
                                            const Procedure proc) {
  line_procedure_map[line] = proc;
}

// pattern

void PKBStorage::storePatternAssign(const Variable var, const ExprStr expr_str,
                                    const Line line) {
  addToVectorMap(var_expr_str_map, var,
                 std::pair<Line, ExprStr>(line, expr_str));
  // std::cout << var + " at line " + line + " maps to " + expr_str <<
  // std::endl;
  var_expr_str_set.insert(std::pair<Variable, ExprStr>(var, expr_str));
  line_expr_str_set.insert(std::pair<Line, ExprStr>(line, expr_str));
  expr_str_set.insert(expr_str);
}

Procedure PKBStorage::getProcedureFromLine(const Line line) {
  return line_procedure_map.at(line);
}

// helper
void PKBStorage::addToVectorMap(
    std::unordered_map<std::string, std::vector<std::string>> &umap,
    const std::string index, std::string data) {
  if (umap.find(index) == umap.end()) {
    // create new vector
    std::vector<std::string> v;
    v.push_back(data);
    umap[index] = v;
  } else {
    // retrieve vector and add element
    umap.at(index).push_back(data);
  }
}

void PKBStorage::addToVectorMap(
    std::unordered_map<std::string,
                       std::vector<std::pair<std::string, std::string>>> &umap,
    const std::string index, std::pair<std::string, std::string> data) {
  if (umap.find(index) == umap.end()) {
    // create new vector
    std::vector<std::pair<std::string, std::string>> v;
    v.push_back(data);
    umap[index] = v;
  } else {
    // retrieve vector and add element
    umap.at(index).push_back(data);
  }
}

}  // namespace PKB
