#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "structs/node.h"

// https://stackoverflow.com/a/15161034/3826254
// How to make unordered set of pairs of integers in C++?
// http://ticki.github.io/blog/designing-a-good-non-cryptographic-hash-function/
// good hash function
struct pair_hash {
 public:
  inline std::size_t operator()(
      const std::pair<std::string, std::string> &v) const {
    std::hash<std::string> hash_str;
    size_t hash_val = 0;
    hash_val = hash_str(v.first) + 1;
    hash_val = hash_val ^ (hash_val >> 6);
    hash_val = hash_val * 31;
    hash_val = hash_val ^ (hash_val << 2);
    hash_val = hash_val + hash_str(v.second) + 1;
    hash_val = hash_val ^ (hash_val >> 7);
    hash_val = hash_val * 29;
    hash_val = hash_val ^ (hash_val << 1);

    return hash_val;
  }
};

// using string for line numbers since it's easier to just handle a single type
// procedure and variables names also can't begin with a number anyway
// in particular helps with storing both line numbers and procedure names
// for parent relationships
using LineNumber = std::string;
using VariableName = std::string;
using ProcedureName = std::string;
using Parent = std::string;  // ProcedureName or LineNumber

class PKB {
 private:
  std::shared_ptr<ProcedureNode> ast;
  // TODO deprecate this and use hash map
  std::vector<std::shared_ptr<Node>> lines;

  std::unordered_set<std::pair<LineNumber, LineNumber>, pair_hash> follows_set;
  std::unordered_map<LineNumber, std::vector<LineNumber>> follows_map;
  std::unordered_set<std::pair<Parent, LineNumber>, pair_hash> parent_set;
  std::unordered_map<Parent, LineNumber> parent_map;
  std::unordered_set<std::pair<LineNumber, VariableName>, pair_hash> uses_set;
  std::unordered_map<LineNumber, std::vector<VariableName>> uses_map;
  std::unordered_set<std::pair<LineNumber, VariableName>, pair_hash>
      modifies_set;
  std::unordered_map<LineNumber, std::vector<VariableName>> modifies_map;

  std::unordered_set<VariableName> variables_set;

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

  void setFollowsRelations(const std::shared_ptr<ProcedureNode> node);
  void setFollowsRelations(const std::shared_ptr<IfNode> node);
  void setFollowsRelations(const std::shared_ptr<WhileNode> node);
  void setFollowsRelationsIterator(const std::vector<StmtNode> stmt_lst);

  void setParentRelations(const std::shared_ptr<ProcedureNode> node);
  void setParentRelations(const std::shared_ptr<IfNode> node);
  void setParentRelations(const std::shared_ptr<WhileNode> node);
  void setParentRelationsIterator(const std::vector<StmtNode> stmt_lst,
                                  const std::shared_ptr<Node> parent_node);

  // omitted NumberNode since it does not seem to be necessary
  void setUsesRelations(const std::shared_ptr<ProcedureNode> node);
  void setUsesRelations(const std::shared_ptr<IfNode> node);
  void setUsesRelations(const std::shared_ptr<WhileNode> node);
  void setUsesRelations(const std::shared_ptr<PrintNode> node);
  void setUsesRelations(const std::shared_ptr<AssignNode> node);
  void setUsesRelations(const std::shared_ptr<ReadNode> node);
  void setUsesRelationsH(const std::shared_ptr<ExprNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<ExprPNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<TermNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<TermPNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<FactorNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<CondExprNode> node,
                         const std::shared_ptr<Node> parent_node);
  void setUsesRelationsH(const std::shared_ptr<RelExprNode> node,
                         const std::shared_ptr<Node> parent_node);
  // void setUsesRelationsH(const std::shared_ptr<RelFactorNode> node,
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
  LineNumber getLineNumberFromNode(std::vector<std::shared_ptr<Node>> ls,
                                   std::shared_ptr<Node> node);
  std::shared_ptr<Node> getNodeFromLineNumber(
      std::vector<std::shared_ptr<Node>> ls, int line_number);
  void addToVectorMap(
      std::unordered_map<std::string, std::vector<std::string>> umap,
      std::string index, std::string data);
  ProcedureName getNodeValue(std::shared_ptr<Node> node);

  // bool lineFollows(int a, int b); // currently testFollows make friend

 public:
  PKB(const std::shared_ptr<ProcedureNode> proc);
  ~PKB();

  // TODO deprecate temp testing methods
  // need to implement pkb-pql link first
  bool testFollows(LineNumber a, LineNumber b);
  bool testParent(Parent a, LineNumber b);
  bool testUses(LineNumber line, VariableName v);
  bool testModifies(LineNumber line, VariableName v);

  // TODO shift these to Query Manager???? or just make it as a friend class
  // would be public method in the QueryManager class
  std::vector<LineNumber> getFollows(LineNumber line);
  std::vector<LineNumber> getUses(LineNumber line);
};
