#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_definitions.h"
#include "structs/node.h"
#include "utils/utils.h"

using namespace Utils;

namespace PKB {

class PKBStorage {
 private:
  // helpers
  void addToVectorMap(
      std::unordered_map<std::string, std::vector<std::string>> umap,
      std::string index, std::string data);

 public:
  PKBStorage();
  ~PKBStorage();
  // attributes
  std::shared_ptr<ProcedureNode> ast;
  // TODO deprecate this and use hash map
  std::vector<std::shared_ptr<Node>> lines;

  std::string getLineFromNode(const std::shared_ptr<Node> node);

  // follows
  std::unordered_set<std::pair<LineBefore, LineAfter>, pair_hash> follows_set;
  std::unordered_map<LineBefore, LineAfter> line_before_line_after_map;
  std::unordered_map<LineAfter, LineBefore> line_after_line_before_map;
  // std::unordered_map<LineBefore, std::vector<LineAfter>>
  //     line_before_line_after_map_s;
  // std::unordered_map<LineAfter, std::vector<LineBefore>>
  //     line_after_line_before_map_s;

  std::unordered_set<std::pair<ParentLine, ChildLine>, pair_hash> parent_set;
  std::unordered_set<std::pair<Procedure, Variable>, pair_hash>
      procedure_uses_var_set;
  std::unordered_set<std::pair<Line, Variable>, pair_hash> line_uses_var_set;
  std::unordered_set<std::pair<Procedure, Variable>, pair_hash>
      procedure_modifies_var_set;
  std::unordered_set<std::pair<Line, Variable>, pair_hash>
      line_modifies_var_set;

  std::unordered_map<ParentLine, Line> parent_line_child_line_map;
  std::unordered_map<ParentLine, std::vector<Line>>
      child_line_to_parent_line_map;
  std::unordered_map<Line, std::vector<Line>> line_parent_map;
  std::unordered_map<Line, std::vector<Variable>> uses_map;
  std::unordered_map<Line, std::vector<Variable>> modifies_map;

  std::unordered_set<Variable> var_set;
  std::unordered_set<Line> assign_set;
  std::unordered_set<Line> statement_set;
  std::unordered_set<Line> print_set;
  std::unordered_set<Line> read_set;
  std::unordered_set<Line> while_set;
  std::unordered_set<Line> if_set;
  std::unordered_set<Constant> constant_set;
  std::unordered_set<Procedure> procedure_set;

  std::vector<Variable> var_list;
  std::vector<Line> assign_list;
  std::vector<Line> statement_list;
  std::vector<Line> print_list;
  std::vector<Line> read_list;
  std::vector<Line> while_list;
  std::vector<Line> if_list;
  std::vector<Constant> constant_list;
  std::vector<Procedure> procedure_list;

  // setters
  void storeAST(const std::shared_ptr<ProcedureNode> proc);
  // TODO upon adding the hash function for line number mapping
  // change the parameters for storeLine to take in both
  // the node and the line number (instead of just the node)
  void storeLine(const std::shared_ptr<Node> node);
  void storeFollowsRelation(const LineBefore, const LineAfter);
  void storeFollowsRelationS(const LineBefore, const LineAfter);
  void storeParentRelation(const ParentLine, const ChildLine);
  void storeParentRelationS(const ParentLine, const ChildLine);
  void storeProcedureUsesVarRelation(const Procedure, const Variable);
  void storeLineUsesVarRelation(const Line, const Variable);
  void storeProcedureModifiesVarRelation(const Procedure, const Variable);
  void storeLineModifiesVarRelation(const Line, const Variable);

  void storeVariable(const Variable);
  void storeAssign(const Line);
  void storeStatement(const Line);
  void storePrint(const Line);
  void storeRead(const Line);
  void storeWhile(const Line);
  void storeIf(const Line);
  void storeConstant(const Constant);
  void storeProcedure(const Procedure);

  std::vector<Procedure> getProc() {
    std::cout << procedure_list.size() << std::endl;
    std::cout << procedure_list.size() << std::endl;
    std::cout << procedure_list.size() << std::endl;
    std::cout << procedure_list.size() << std::endl;
    std::cout << procedure_list.size() << std::endl;
    return procedure_list;
  }
};

}  // namespace PKB
