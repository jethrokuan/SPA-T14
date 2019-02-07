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
  inline std::size_t operator()(const std::pair<int, int> &v) const {
    std::hash<int> hash_int;
    size_t hash_val = 0;
    hash_val = hash_int(v.first) + 1;
    hash_val = hash_val ^ (hash_val >> 6);
    hash_val = hash_val * 31;
    hash_val = hash_val ^ (hash_val << 2);
    hash_val = hash_val + hash_int(v.second) + 1;
    hash_val = hash_val ^ (hash_val >> 7);
    hash_val = hash_val * 29;
    hash_val = hash_val ^ (hash_val << 1);

    return hash_val;
  }
  inline std::size_t operator()(const std::pair<int, std::string> &v) const {
    std::hash<int> hash_int;
    std::hash<std::string> hash_str;
    size_t hash_val = 0;
    hash_val = hash_int(v.first) + 1;
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

class PKB {
 private:
  std::shared_ptr<ProcedureNode> ast;
  std::vector<std::shared_ptr<Node>>
      lines;  // TODO deprecate this and use hash map
  // line_follows_line
  std::unordered_set<std::pair<int, int>, pair_hash> follows_set;
  // line_follow_lines
  std::unordered_map<int, std::vector<int>> follows_map;
  // line_parent_line
  std::unordered_set<std::pair<int, int>, pair_hash> parent_set;
  // line_parent_lines
  std::unordered_map<int, std::vector<int>> parent_map;
  // line_uses_var
  std::unordered_set<std::pair<int, std::string>, pair_hash> uses_set;
  // line_uses_vars
  std::unordered_map<int, std::vector<std::string>> uses_map;
  // line_modifies_var
  std::unordered_set<std::pair<int, int>, pair_hash> modifies_set;
  // line_modifies_vars
  std::unordered_map<int, std::vector<int>> modifies_map;

  // TODO change function arguments
  // current implementations assume procedure node to be the root node
  // preprocessing functions
  void setLineNumbers(
      std::shared_ptr<ProcedureNode> node);  // TODO error handling
  void setFollowsRelations(std::shared_ptr<ProcedureNode> node);
  void setParentRelations(std::shared_ptr<ProcedureNode> node);
  void setParentRelationsHelper(std::shared_ptr<Node> node,
                                std::vector<int> parent);
  void setUsesRelations(std::shared_ptr<ProcedureNode> node);
  void setUsesRelationsHelper(std::shared_ptr<Node> node, int line_number);
  void setModifiesRelations(std::shared_ptr<ProcedureNode> node);

  // utility functions
  int getLineNumberFromNode(std::vector<std::shared_ptr<Node>> ls,
                            std::shared_ptr<Node> node);
  std::shared_ptr<Node> getNodeFromLineNumber(
      std::vector<std::shared_ptr<Node>> ls, int line_number);

  // bool lineFollows(int a, int b); // currently testFollows make friend

 public:
  PKB(std::shared_ptr<ProcedureNode> proc);
  ~PKB();

  // TODO deprecate temp testing methods
  // need to implement pkb-pql link first
  bool testFollows(int a, int b);
  bool testParent(int a, int b);
  bool testUses(int line, std::string v);
  bool testModifies(int line, std::string v);

  // TODO shift these to Query Manager???? or just make it as a friend class
  // would be public method in the QueryManager class
  std::vector<int> getFollows(int line);
  std::vector<std::string> getUses(int line);
};
