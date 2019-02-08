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

using LineNumber = int;
using VariableName = std::string;
using ProcedureName = std::string;

class PKB {
 private:
  std::shared_ptr<ProcedureNode> ast;
  // TODO deprecate this and use hash map
  std::vector<std::shared_ptr<Node>> lines;

  std::unordered_set<std::pair<LineNumber, LineNumber>, pair_hash> follows_set;
  std::unordered_map<LineNumber, std::vector<LineNumber>> follows_map;
  std::unordered_set<std::pair<LineNumber, LineNumber>, pair_hash> parent_set;
  std::unordered_map<LineNumber, std::vector<LineNumber>> parent_map;
  std::unordered_set<std::pair<LineNumber, VariableName>, pair_hash> uses_set;
  std::unordered_map<LineNumber, std::vector<VariableName>> uses_map;
  std::unordered_set<std::pair<LineNumber, VariableName>, pair_hash>
      modifies_set;
  std::unordered_map<LineNumber, std::vector<VariableName>> modifies_map;

  std::unordered_set<VariableName> variables_set;

  // TODO change function arguments
  // current implementations assume procedure node to be the root node
  // preprocessing functions
  void setLineNumbers(std::shared_ptr<ProcedureNode> node);
  void setLineNumbers(std::shared_ptr<IfNode> node);
  void setLineNumbers(std::shared_ptr<WhileNode> node);
  void setLineNumbers(std::shared_ptr<ReadNode> node);
  void setLineNumbers(std::shared_ptr<PrintNode> node);
  void setLineNumbers(std::shared_ptr<AssignNode> node);
  void setLineNumbersIterator(std::vector<std::shared_ptr<Node>> stmt_lst);
  void setFollowsRelations(std::shared_ptr<Node> node);
  void setParentRelations(std::shared_ptr<Node> node);
  void setUsesRelations(std::shared_ptr<Node> node);
  void setUsesRelationsHelper(std::shared_ptr<Node> node,
                              LineNumber line_number);
  void setModifiesRelations(std::shared_ptr<Node> node);
  void setModifiesRelationsHelper(std::shared_ptr<Node> node,
                                  LineNumber line_number);

  // utility functions
  LineNumber getLineNumberFromNode(std::vector<std::shared_ptr<Node>> ls,
                                   std::shared_ptr<Node> node);
  std::shared_ptr<Node> getNodeFromLineNumber(
      std::vector<std::shared_ptr<Node>> ls, int line_number);
  void addToVectorMap(std::unordered_map<int, std::vector<int>> umap, int index,
                      int data);
  void addToVectorMap(std::unordered_map<int, std::vector<std::string>> umap,
                      int index, std::string data);

  // bool lineFollows(int a, int b); // currently testFollows make friend

 public:
  PKB(std::shared_ptr<ProcedureNode> proc);
  ~PKB();

  // TODO deprecate temp testing methods
  // need to implement pkb-pql link first
  bool testFollows(LineNumber a, LineNumber b);
  bool testParent(LineNumber a, LineNumber b);
  bool testUses(LineNumber line, VariableName v);
  bool testModifies(LineNumber line, VariableName v);

  // TODO shift these to Query Manager???? or just make it as a friend class
  // would be public method in the QueryManager class
  std::vector<int> getFollows(int line);
  std::vector<std::string> getUses(int line);
};
