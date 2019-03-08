#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_definitions.h"
#include "program_knowledge_base/pkb_preprocessor.h"
#include "program_knowledge_base/pkb_storage.h"
#include "simple_parser/interface.h"
#include "structs/node.h"
#include "utils/utils.h"

using namespace Utils;
using namespace Simple;

namespace PKB {

class PKBManager {
 private:
  std::shared_ptr<PKBStorage> pkb_storage = std::make_shared<PKBStorage>();

  std::optional<std::vector<std::string>> getUniqueVectorFromMap(
      const std::unordered_map<std::string, std::vector<std::string>> &,
      const std::string);
  std::vector<std::string> getUniqueVector(std::vector<std::string>);

 public:
  PKBManager(const AST ast);
  ~PKBManager();

  // API exposed to Query Manager

  // is design entity set empty
  bool isVariableSetEmpty();
  bool isAssignSetEmpty();
  bool isStatementSetEmpty();
  bool isPrintSetEmpty();
  bool isReadSetEmpty();
  bool isWhileSetEmpty();
  bool isIfSetEmpty();
  bool isConstantSetEmpty();
  bool isProcedureSetEmpty();

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

  // is relationship set empty
  bool isLineFollowLineSetEmpty();
  bool isLineFollowLineSSetEmpty();
  bool isLineParentLineSetEmpty();
  bool isLineParentLineSSetEmpty();
  bool isLineUsesVarSetEmpty();
  bool isLineModifiesVarSetEmpty();
  bool isProcedureUsesVarSetEmpty();
  bool isProcedureModifiesVarSetEmpty();

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
  std::optional<LineAfter> getFollowingLine(const LineBefore);
  std::optional<LineBefore> getBeforeLine(const LineAfter);
  std::optional<std::vector<LineAfter>> getFollowingLineS(const LineBefore);
  std::optional<std::vector<LineBefore>> getBeforeLineS(const LineAfter);

  std::optional<ParentLine> getParentLine(const ChildLine);
  std::optional<std::vector<ChildLine>> getChildLine(const ParentLine);
  std::optional<std::vector<ParentLine>> getParentLineS(const ChildLine);
  std::optional<std::vector<ChildLine>> getChildLineS(const ParentLine);

  std::optional<std::vector<Variable>> getVarModifiedByProcedure(
      const Procedure);
  std::optional<std::vector<Variable>> getVarModifiedByLine(const Line);
  std::optional<std::vector<Procedure>> getProcedureModifiesVar(const Variable);
  std::optional<std::vector<Line>> getLineModifiesVar(const Variable);

  std::optional<std::vector<Variable>> getVarUsedByProcedure(const Procedure);
  std::optional<std::vector<Variable>> getVarUsedByLine(const Line);
  std::optional<std::vector<Procedure>> getProcedureUsesVar(const Variable);
  std::optional<std::vector<Line>> getLineUsesVar(const Variable);

  // pattern
  std::optional<std::vector<Line>> getCompleteMatchLinesWithVar(const Variable,
                                                                const Pattern);
  std::optional<std::vector<Line>> getPartialMatchLinesWithVar(const Variable,
                                                               const Pattern);
  std::optional<std::vector<Line>> getCompleteMatchLines(const Pattern);
  std::optional<std::vector<Line>> getPartialMatchLines(const Pattern);
  std::optional<std::vector<std::pair<Line, Variable>>>
  getCompleteMatchLinesAndVars(const Pattern);
  std::optional<std::vector<std::pair<Line, Variable>>>
  getPartialMatchLinesAndVars(const Pattern);
  std::vector<std::pair<Line, Variable>> getAllPatternLinesAndVars();
  bool isPatternExists(const Pattern);

  // next
  bool isLineNextLine(Line, Line);

  // misc helpers
  std::optional<std::vector<Line>> getLineForAssignVar(const Variable);
};

}  // namespace PKB
