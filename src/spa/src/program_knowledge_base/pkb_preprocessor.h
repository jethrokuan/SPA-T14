#pragma once
#include "program_knowledge_base/pkb_storage.h"
#include "structs/node.h"
#include "utils/utils.h"

namespace PKB {

class PKBPreprocessor {
 private:
  std::shared_ptr<PKBStorage> storage;

  void setLineNumbers(const std::shared_ptr<RootNode> node);
  void setLineNumbers(const std::shared_ptr<ProcedureNode> node);
  void setLineNumbers(const StmtNode, const Procedure proc);
  void setLineNumbersIterator(const std::vector<StmtNode> stmt_lst,
                              const Procedure proc);

  void setCFG(const std::shared_ptr<RootNode> node);
  void setCFG(const std::shared_ptr<ProcedureNode> node);
  void setCFG(
      const std::shared_ptr<IfNode> node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>
          cache);
  void setCFG(
      const std::shared_ptr<WhileNode> node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>
          cache);
  void setCFG(
      const std::shared_ptr<ReadNode>,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>
          cache);
  void setCFG(
      const std::shared_ptr<PrintNode>,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>
          cache);
  void setCFG(
      const std::shared_ptr<AssignNode>,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>
          cache);
  void setCFG(
      const std::shared_ptr<CallNode>,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>
          cache);
  void setCFGIterator(
      const std::vector<StmtNode> stmt_lst,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>
          cache);

  std::unordered_set<Line> getTerminatingLines(
      const StmtNode node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>);
  std::unordered_set<Line> getTerminatingLines(
      const std::shared_ptr<IfNode> node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>);
  std::unordered_set<Line> getTerminatingLines(
      const std::shared_ptr<WhileNode> node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>);
  std::unordered_set<Line> getTerminatingLines(
      const std::shared_ptr<ReadNode> node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>);
  std::unordered_set<Line> getTerminatingLines(
      const std::shared_ptr<PrintNode> node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>);
  std::unordered_set<Line> getTerminatingLines(
      const std::shared_ptr<AssignNode> node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>);
  std::unordered_set<Line> getTerminatingLines(
      const std::shared_ptr<CallNode> node,
      std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>);

  void setDesignEntities(const std::shared_ptr<RootNode> node);
  void setDesignEntitiesProcListIterator(
      const std::vector<std::shared_ptr<ProcedureNode>> proc_lst);
  void setDesignEntities(const std::shared_ptr<ProcedureNode> node);
  void setDesignEntities(const std::shared_ptr<IfNode> node);
  void setDesignEntities(const std::shared_ptr<WhileNode> node);
  void setDesignEntities(const std::shared_ptr<ReadNode> node);
  void setDesignEntities(const std::shared_ptr<PrintNode> node);
  void setDesignEntities(const std::shared_ptr<AssignNode> node);
  void setDesignEntities(const std::shared_ptr<CallNode> node);
  void setDesignEntities(const Expr node);
  void setDesignEntities(const std::shared_ptr<BinOpNode> node);
  void setDesignEntities(const std::shared_ptr<CondExprNode> node);
  void setDesignEntities(const std::shared_ptr<RelExprNode> node);
  void setDesignEntities(const std::shared_ptr<NumberNode> node);
  void setDesignEntities(const std::shared_ptr<VariableNode> node);
  void setDesignEntitiesIterator(const std::vector<StmtNode> stmt_lst);

  void setCallsRelations(const std::shared_ptr<RootNode> node);
  void setCallsRelations(const std::shared_ptr<ProcedureNode> node);
  void setCallsRelations(const std::shared_ptr<IfNode> node);
  void setCallsRelations(const std::shared_ptr<WhileNode> node);
  void setCallsRelations(const std::shared_ptr<ReadNode> node);
  void setCallsRelations(const std::shared_ptr<PrintNode> node);
  void setCallsRelations(const std::shared_ptr<AssignNode> node);
  void setCallsRelations(const std::shared_ptr<CallNode> node);
  void setCallsRelationsIterator(const std::vector<StmtNode> stmt_lst);
  void setCallsIndirectRelations();
  void setCallsIndirectRelationsH(const ProcedureCaller proc_caller,
                                  const ProcedureCallee proc_callee);

  void setFollowsRelations(const std::shared_ptr<RootNode> node);
  void setFollowsRelations(const std::shared_ptr<ProcedureNode> node);
  void setFollowsRelations(const std::shared_ptr<IfNode> node);
  void setFollowsRelations(const std::shared_ptr<WhileNode> node);
  void setFollowsRelationsIterator(const std::vector<StmtNode> stmt_lst);

  void setParentRelations(const std::shared_ptr<RootNode> node);
  void setParentRelations(const std::shared_ptr<ProcedureNode> node);
  void setParentRelationsH(const std::shared_ptr<ProcedureNode> node,
                           const std::vector<ParentLine> parent_lines);
  void setParentRelationsH(const std::shared_ptr<IfNode> node,
                           std::vector<ParentLine> parent_lines);
  void setParentRelationsH(const std::shared_ptr<WhileNode> node,
                           std::vector<ParentLine> parent_lines);
  void setParentRelationsIterator(const std::vector<StmtNode> stmt_lst,
                                  const std::vector<ParentLine> parent_lines);

  void setUsesRelations(const std::shared_ptr<RootNode> node);
  void setUsesRelations(const std::shared_ptr<ProcedureNode> node);
  void setUsesRelations(const std::shared_ptr<IfNode> node);
  void setUsesRelations(const std::shared_ptr<WhileNode> node);
  void setUsesRelations(const std::shared_ptr<PrintNode> node);
  void setUsesRelations(const std::shared_ptr<AssignNode> node);
  void setUsesRelations(const std::shared_ptr<CallNode>);
  void setUsesRelations(const std::shared_ptr<ReadNode>);
  void setUsesRelationsH(Expr node, const StmtNode parent_node);
  void setUsesRelationsH(const std::shared_ptr<BinOpNode> node,
                         const StmtNode parent_node);
  void setUsesRelationsH(const std::shared_ptr<CondExprNode> node,
                         const StmtNode parent_node);
  void setUsesRelationsH(const std::shared_ptr<RelExprNode> node,
                         const StmtNode parent_node);
  void setUsesRelationsH(std::shared_ptr<NumberNode>, const StmtNode);
  void setUsesRelationsH(const std::shared_ptr<VariableNode> node,
                         const StmtNode parent_node);
  void setUsesRelationsIterator(const std::vector<StmtNode> stmt_lst);
  void setUsesIndirectRelations();

  void setModifiesRelations(const std::shared_ptr<RootNode> node);
  void setModifiesRelations(const std::shared_ptr<ProcedureNode> node);
  void setModifiesRelations(const std::shared_ptr<IfNode> node);
  void setModifiesRelations(const std::shared_ptr<WhileNode> node);
  void setModifiesRelations(const std::shared_ptr<ReadNode> node);
  void setModifiesRelations(const std::shared_ptr<AssignNode> node);
  void setModifiesRelations(const std::shared_ptr<CallNode> node);
  void setModifiesRelations(const std::shared_ptr<PrintNode> node);
  void setModifiesRelationsH(const std::shared_ptr<VariableNode> node,
                             const StmtNode parent_node);
  void setModifiesRelationsIterator(const std::vector<StmtNode> stmt_lst);
  void setModifiesIndirectRelations();

  void setPattern(const std::shared_ptr<RootNode> node);
  void setPattern(const std::shared_ptr<ProcedureNode> node);
  void setPattern(const std::shared_ptr<AssignNode> node);
  void setPattern(const std::shared_ptr<CallNode> node);
  void setPattern(const std::shared_ptr<IfNode> node);
  void setPattern(const std::shared_ptr<WhileNode> node);
  void setPattern(const std::shared_ptr<ReadNode>);
  void setPattern(const std::shared_ptr<PrintNode>);
  void setPatternH(const Expr node, const StmtNode parent_node);
  void setPatternH(const std::shared_ptr<BinOpNode> node,
                   const StmtNode parent_node);
  void setPatternH(const std::shared_ptr<CondExprNode> node,
                   const StmtNode parent_node);
  void setPatternH(const std::shared_ptr<RelExprNode> node,
                   const StmtNode parent_node);
  void setPatternH(const std::shared_ptr<NumberNode> node,
                   const StmtNode parent_node);
  void setPatternH(const std::shared_ptr<VariableNode> node,
                   const StmtNode parent_node);
  void setPatternIterator(const std::vector<StmtNode> stmt_lst);

  void setAssign(const std::shared_ptr<RootNode> node);
  void setAssign(const std::shared_ptr<ProcedureNode> node);
  void setAssign(const std::shared_ptr<IfNode> node);
  void setAssign(const std::shared_ptr<WhileNode> node);
  void setAssign(const std::shared_ptr<ReadNode> node);
  void setAssign(const std::shared_ptr<PrintNode> node);
  void setAssign(const std::shared_ptr<AssignNode> node);
  void setAssign(const std::shared_ptr<CallNode> node);
  void setAssign(const Expr node, const Line line_number);
  void setAssign(const std::shared_ptr<BinOpNode> node, const Line line_number);
  void setAssign(const std::shared_ptr<CondExprNode> node,
                 const Line line_number);
  void setAssign(const std::shared_ptr<RelExprNode> node,
                 const Line line_number);
  void setAssign(const std::shared_ptr<NumberNode> node, const Line);
  void setAssign(const std::shared_ptr<VariableNode> node,
                 const Line line_number);
  void setAssignIterator(const std::vector<StmtNode> stmt_lst);

  void setProcedureStartAndEnd(const std::shared_ptr<RootNode> node);
  void setProcedureEnd(const Line cur_line,
      std::shared_ptr<std::unordered_set<Line>> visited);

 public:
  PKBPreprocessor(const AST ast, std::shared_ptr<PKBStorage> pkb_storage);
  ~PKBPreprocessor();
};

}  // namespace PKB
