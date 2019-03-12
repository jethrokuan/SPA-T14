#pragma once
#include "program_knowledge_base/pkb_storage.h"
#include "queue"
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
  void setCFG(const std::shared_ptr<IfNode> node,
              const Line = INVALID_LINE_NUMBER);
  void setCFG(const std::shared_ptr<WhileNode> node,
              const Line = INVALID_LINE_NUMBER);
  void setCFG(const std::shared_ptr<ReadNode>,
              const Line = INVALID_LINE_NUMBER);
  void setCFG(const std::shared_ptr<PrintNode>,
              const Line = INVALID_LINE_NUMBER);
  void setCFG(const std::shared_ptr<AssignNode>,
              const Line = INVALID_LINE_NUMBER);
  void setCFG(const std::shared_ptr<CallNode>,
              const Line = INVALID_LINE_NUMBER);
  void setCFGIterator(const std::vector<StmtNode> stmt_lst,
                      const Line = INVALID_LINE_NUMBER);

  void setDesignEntities(const std::shared_ptr<RootNode> node);
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
  void setUsesRelationsH(Expr node, const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<BinOpNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<CondExprNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<RelExprNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(std::shared_ptr<NumberNode>,
                         const std::shared_ptr<Node>);
  void setUsesRelationsH(const std::shared_ptr<VariableNode> node,
                         const std::shared_ptr<Node> parent_node);
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
                             const std::shared_ptr<Node> parent_node);
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
  void setPatternIterator(const std::vector<StmtNode> stmt_lst);

 public:
  PKBPreprocessor(const AST ast, std::shared_ptr<PKBStorage> pkb_storage);
  ~PKBPreprocessor();
};

}  // namespace PKB
