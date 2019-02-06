#include "structs/node.h"
#include <string>
#include <unordered_set>
#include <vector>

// https://stackoverflow.com/a/15161034/3826254
// How to make unordered set of pairs of integers in C++?
struct pair_hash {
  inline std::size_t operator()(const std::pair<int,int> & v) const {
    return v.first*31+v.second;
  }
};

class PKB {
 private:
  std::shared_ptr<ProcedureNode> ast;
  std::vector<std::shared_ptr<Node>> lines; // TODO deprecate this and use hash map
  std::unordered_set<std::pair<int, int>, pair_hash> follows_set;
  std::unordered_set<std::pair<int, int>, pair_hash> parent_set;
  // std::unordered_set<std::pair<int, int>, pair_hash> uses_set;
  // std::unordered_set<std::pair<int, int>, pair_hash> modifies_set;

  // TODO change function arguments
  // current implementations assume procedure node to be the root node
  void setLineNumbers(std::shared_ptr<ProcedureNode> node); // TODO error handling
  void setFollowsRelations(std::shared_ptr<ProcedureNode> node);
  void setParentRelations(std::shared_ptr<ProcedureNode> node);
  void setParentRelationsHelper(std::shared_ptr<Node> node, std::vector<int> parent);
  void setUsesRelatinos(std::shared_ptr<ProcedureNode> node);
  void setModifiesRelations(std::shared_ptr<ProcedureNode> node);
  int getNodeLineNumber(std::vector<std::shared_ptr<Node>> ls, std::shared_ptr<Node> node);

 public:
  PKB(std::shared_ptr<ProcedureNode> proc);
  ~PKB();

  // TODO deprecate temp testing methods
  // need to implement pkb-pql link first
  bool testFollows(int a, int b);
  bool testParent(int a, int b);
  bool testUses(int line, std::string v);
  bool testModifies(int line, std::string v);
};
