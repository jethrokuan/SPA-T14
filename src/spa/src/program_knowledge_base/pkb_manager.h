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

  std::vector<std::string> getUniqueVectorFromMap(
      const std::unordered_map<std::string, std::vector<std::string>> &,
      const std::string);
  std::vector<std::string> getUniqueVector(std::vector<std::string>);

 public:
  PKBManager(const std::shared_ptr<ProcedureNode> ast);
  ~PKBManager();

  // API exposed to Query Manager

  // is design entity exists
  bool isVariableExists(const Variable);
  bool isAssignExists(const Line);
  bool isStatementExists(const Line);
  bool isPrintExists(const Line);
  bool isReadExists(const Line);
  bool isWhileExists(const Line);
  bool isIfExists(const Line);
  bool isConstantExists(const Constant);
  bool isProcedureExists(const Procedure);

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

  // is relationship valid
  bool isLineFollowLine(const LineBefore, const LineAfter);
  bool isLineFollowLineS(const LineBefore, const LineAfter);
  bool isLineParentLine(const ParentLine, const ChildLine);
  bool isLineParentLineS(const ParentLine, const ChildLine);
  bool isProcedureModifiesVar(const Procedure, const Variable);
  bool isLineModifiesVar(const Line, const Variable);
  bool isProcedureUsesVar(const Procedure, const Variable);
  bool isLineUsesVar(const Line, const Variable);

  // get relationship mapping
  LineAfter getFollowingLine(const LineBefore);
  LineBefore getBeforeLine(const LineAfter);
  std::vector<LineAfter> getFollowingLineS(const LineBefore);
  std::vector<LineBefore> getBeforeLineS(const LineAfter);

  ParentLine getParentLine(const ChildLine);
  std::vector<ChildLine> getChildLine(const ParentLine);
  std::vector<ParentLine> getParentLineS(const ChildLine);
  std::vector<ChildLine> getChildLineS(const ParentLine);

  std::vector<Variable> getVarModifiedByProcedure(const Procedure);
  std::vector<Variable> getVarModifiedByLine(const Line);
  std::vector<Procedure> getProcedureModifiesVar(const Variable);
  std::vector<Line> getLineModifiesVar(const Variable);

  std::vector<Variable> getVarUsedByProcedure(const Procedure);
  std::vector<Variable> getVarUsedByLine(const Line);
  std::vector<Procedure> getProcedureUsesVar(const Variable);
  std::vector<Line> getLineUsesVar(const Variable);
};

}  // namespace PKB
