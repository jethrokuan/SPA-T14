#include "structs/node.h"
#include <vector>
#include <unordered_set>

struct pair_hash {
  inline std::size_t operator()(const std::pair<int,int> & v) const {
    return v.first*31+v.second;
  }
};

class PKB {
 private:
  std::shared_ptr<ProcedureNode> ast;
  // int line_number = 1; // use this when doing hash map
  std::vector<std::shared_ptr<Node>> lines; // TODO deprecate this and use hash map
  std::unordered_set<std::pair<int, int>, pair_hash> parent_set;
  std::unordered_set<std::pair<int, int>, pair_hash> follows_set;
  // TODO update stubs
  void setLineNumbers(std::shared_ptr<ProcedureNode> node); // TODO error handling
  void setParentRelations(std::shared_ptr<ProcedureNode> node);
  void setFollowsRelations(std::shared_ptr<ProcedureNode> node);
  void setUsesRelatinos();
  void setModifiesRelations();
  void getVariablesUsed();
  int getNodeLineNumber(std::vector<std::shared_ptr<Node>> ls, std::shared_ptr<Node> node);

 public:
  PKB(std::shared_ptr<ProcedureNode> proc);
  ~PKB();

  // TODO deprecate temp testing methods
  // need to implement pkb-pql link first
  bool testFollows(int a, int b);
};