#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_definitions.h"
#include "program_knowledge_base/pkb_preprocessor.h"
#include "program_knowledge_base/pkb_storage.h"
#include "structs/node.h"
#include "utils/utils.h"

using namespace Utils;

namespace PKB {

class PKBManager {
 private:
  std::shared_ptr<PKBStorage> pkb_storage = std::make_shared<PKBStorage>();
  std::shared_ptr<ProcedureNode> ast;
  // TODO deprecate this and use hash map
  std::vector<std::shared_ptr<Node>> lines;

  std::unordered_set<std::pair<ParentLine, Line>, pair_hash> parent_set;
  std::unordered_map<ParentLine, Line> parent_map;
  std::unordered_set<std::pair<Line, Variable>, pair_hash> uses_set;
  std::unordered_map<Line, std::vector<Variable>> uses_map;
  std::unordered_set<std::pair<Line, Variable>, pair_hash> modifies_set;
  std::unordered_map<Line, std::vector<Variable>> modifies_map;

  std::unordered_set<Variable> variables_set;

  // TODO change function arguments
  // current implementations assume procedure node to be the root node
  // preprocessing functions
  void setLineNumbers(const std::shared_ptr<ProcedureNode> node);
  void setLineNumbers(const std::shared_ptr<IfNode> node);
  void setLineNumbers(const std::shared_ptr<WhileNode> node);
  void setLineNumbers(const std::shared_ptr<ReadNode> node);
  void setLineNumbers(const std::shared_ptr<PrintNode> node);
  void setLineNumbers(const std::shared_ptr<AssignNode> node);
  void setLineNumbersIterator(const std::vector<StmtNode> stmt_lst);

  // omitted NumberNode since it does not seem to be necessary
  void setUsesRelations(const std::shared_ptr<ProcedureNode> node);
  void setUsesRelations(const std::shared_ptr<IfNode> node);
  void setUsesRelations(const std::shared_ptr<WhileNode> node);
  void setUsesRelations(const std::shared_ptr<PrintNode> node);
  void setUsesRelations(const std::shared_ptr<AssignNode> node);
  void setUsesRelations(const std::shared_ptr<ReadNode> node);
  void setUsesRelationsH(Expr node, const std::shared_ptr<Node> parent_node);
  // void setUsesRelationsH(Factor node,
  //                        const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<CondExprNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<RelExprNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(std::shared_ptr<NumberNode>,
                         const std::shared_ptr<Node>);
  // void setUsesRelationsH(const std::shared_ptr<RelFactor> node,
  //                        const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<VariableNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsIterator(const std::vector<StmtNode> stmt_lst);

  void setModifiesRelations(const std::shared_ptr<ProcedureNode> node);
  void setModifiesRelations(const std::shared_ptr<IfNode> node);
  void setModifiesRelations(const std::shared_ptr<WhileNode> node);
  void setModifiesRelations(const std::shared_ptr<ReadNode> node);
  void setModifiesRelations(const std::shared_ptr<AssignNode> node);
  void setModifiesRelations(const std::shared_ptr<PrintNode> node);
  void setModifiesRelationsH(const std::shared_ptr<VariableNode> node,
                             const std::shared_ptr<Node> parent_node);
  void setModifiesRelationsIterator(const std::vector<StmtNode> stmt_lst);

  // utility functions
  Line getLineFromNode(std::vector<std::shared_ptr<Node>> ls,
                       std::shared_ptr<Node> node);
  Procedure getNodeValue(std::shared_ptr<Node> node);
  void addToVectorMap(
      std::unordered_map<std::string, std::vector<std::string>> umap,
      std::string index, std::string data);

  // bool lineFollows(int a, int b); // currently testFollows make friend

 public:
  PKBManager(const std::shared_ptr<ProcedureNode> ast);
  ~PKBManager();

  // TODO deprecate temp testing methods
  // need to implement pkb-pql link first
  bool testParent(ParentLine a, Line b);
  bool testUses(Line line, Variable v);
  bool testModifies(Line line, Variable v);

  std::vector<Line> getUses(Line line);

  // API exposed to Query Manager

  // get design entities
  std::vector<Variable> getVariableList();
  std::vector<Line> getAssignList();
  std::vector<Line> getStatementList();
  std::vector<Line> getPrintList();
  std::vector<Line> getReadList();
  std::vector<Line> getWhileList();
  std::vector<Line> getIfList();
  std::vector<Constant> getConstantList();
  std::vector<Procedure> getProcedureList();

  bool isVariableExists(Variable);
  bool isAssignExists(Line);
  bool isStatementExists(Line);
  bool isPrintExists(Line);
  bool isReadExists(Line);
  bool isWhileExists(Line);
  bool isIfExists(Line);
  bool isConstantExists(Constant);
  bool isProcedureExists(Procedure);

  // get mapping
  ParentLine getParentLine(const ChildLine);
  std::vector<ChildLine> getChildLine(const ParentLine);
  std::vector<ParentLine> getParentLineS(const ChildLine);
  std::vector<ChildLine> getChildLineS(const ParentLine);

  LineAfter getFollowingLine(const LineBefore);
  LineBefore getBeforeLine(const LineAfter);
  std::vector<LineAfter> getFollowingLineS(const LineBefore);
  std::vector<LineBefore> getBeforeLineS(const LineAfter);

  Procedure getProcedureModifiesVar(const Variable);
  Line getLineModifiesVar(const Variable);
  std::vector<Variable> getVarModifiedByProcedure(const Procedure);
  Variable getVarModifiedByLine(const Line);

  std::vector<Procedure> getProcedureUsesVar(const Variable);
  std::vector<Line> getLineUsesVar(const Variable);
  std::vector<Variable> getVarUsedByProcedure(const Procedure);
  std::vector<Variable> getVarUsedByLine(const Line);

  // is relationship valid
  bool isLineFollowLine(const LineBefore, const LineAfter);
  bool isLineParentLine(const ParentLine, const ChildLine);
  bool isProcedureModifiesVar(const Procedure, const Variable);
  bool isLineModifiesVar(const Line, const Variable);
  bool isProcedureUsesVar(const Procedure, const Variable);
  bool isLineUsesVar(const Line, const Variable);
};

}  // namespace PKB
