#pragma once

#include <optional>
#include <string>
#include <unordered_set>
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

  std::optional<std::unordered_set<std::string>> getSetFromMap(
      const std::unordered_map<std::string, std::unordered_set<std::string>> &,
      const std::string);

  void getNextLineSH(const Line, std::shared_ptr<std::unordered_set<Line>>);
  void getPreviousLineSH(const Line, std::shared_ptr<std::unordered_set<Line>>);

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
  std::unordered_set<Variable> getVariableSet();
  std::unordered_set<Line> getAssignSet();
  std::unordered_set<Line> getStatementSet();
  std::unordered_set<Line> getPrintSet();
  std::unordered_set<Line> getReadSet();
  std::unordered_set<Line> getWhileSet();
  std::unordered_set<Line> getIfSet();
  std::unordered_set<Constant> getConstantSet();
  std::unordered_set<Procedure> getProcedureSet();

  // is relationship set empty
  // TODO implement for calls and next
  bool isLineNextLineSetEmpty();
  bool isProcedureCallProcedureSetEmpty();
  bool isProcedureCallProcedureTSetEmpty();
  bool isLineFollowLineSetEmpty();
  bool isLineFollowLineSSetEmpty();
  bool isLineParentLineSetEmpty();
  bool isLineParentLineSSetEmpty();
  bool isLineUsesVarSetEmpty();
  bool isLineModifiesVarSetEmpty();
  bool isProcedureUsesVarSetEmpty();
  bool isProcedureModifiesVarSetEmpty();

  // is relationship valid
  bool isProcedureCallsProcedure(const ProcedureCaller, const ProcedureCallee);
  bool isLineFollowLine(const LineBefore, const LineAfter);
  bool isLineFollowLineS(const LineBefore, const LineAfter);
  bool isLineParentLine(const ParentLine, const ChildLine);
  bool isLineParentLineS(const ParentLine, const ChildLine);
  bool isProcedureModifiesVar(const Procedure, const Variable);
  bool isLineModifiesVar(const Line, const Variable);
  bool isProcedureUsesVar(const Procedure, const Variable);
  bool isLineUsesVar(const Line, const Variable);

  // get relationship mapping
  std::optional<std::unordered_set<ProcedureCallee>> getCalleeProcedures(
      const ProcedureCaller);
  std::optional<std::unordered_set<ProcedureCaller>> getCallerProcedures(
      const ProcedureCallee);
  std::optional<std::unordered_set<ProcedureCallee>> getCalleeProceduresS(
      const ProcedureCaller);
  std::optional<std::unordered_set<ProcedureCaller>> getCallerProceduresS(
      const ProcedureCallee);

  std::optional<LineAfter> getFollowingLine(const LineBefore);
  std::optional<LineBefore> getBeforeLine(const LineAfter);
  std::optional<std::unordered_set<LineAfter>> getFollowingLineS(
      const LineBefore);
  std::optional<std::unordered_set<LineBefore>> getBeforeLineS(const LineAfter);

  std::optional<ParentLine> getParentLine(const ChildLine);
  std::optional<std::unordered_set<ChildLine>> getChildLine(const ParentLine);
  std::optional<std::unordered_set<ParentLine>> getParentLineS(const ChildLine);
  std::optional<std::unordered_set<ChildLine>> getChildLineS(const ParentLine);

  std::optional<std::unordered_set<Variable>> getVarModifiedByProcedure(
      const Procedure);
  std::optional<std::unordered_set<Variable>> getVarModifiedByLine(const Line);
  std::optional<std::unordered_set<Procedure>> getProcedureModifiesVar(
      const Variable);
  std::optional<std::unordered_set<Line>> getLineModifiesVar(const Variable);

  std::optional<std::unordered_set<Variable>> getVarUsedByProcedure(
      const Procedure);
  std::optional<std::unordered_set<Variable>> getVarUsedByLine(const Line);
  std::optional<std::unordered_set<Procedure>> getProcedureUsesVar(
      const Variable);
  std::optional<std::unordered_set<Line>> getLineUsesVar(const Variable);

  // pattern
  std::optional<std::unordered_set<Line>> getCompleteMatchLinesWithVar(
      const Variable, const Pattern);
  std::optional<std::unordered_set<Line>> getPartialMatchLinesWithVar(
      const Variable, const Pattern);
  std::optional<std::unordered_set<Line>> getCompleteMatchLines(const Pattern);
  std::optional<std::unordered_set<Line>> getPartialMatchLines(const Pattern);
  std::optional<std::unordered_set<std::pair<Line, Variable>, pair_hash>>
  getCompleteMatchLinesAndVars(const Pattern);
  std::optional<std::unordered_set<std::pair<Line, Variable>, pair_hash>>
  getPartialMatchLinesAndVars(const Pattern);
  std::unordered_set<std::pair<Line, Variable>, pair_hash>
  getAllPatternLinesAndVars();
  bool isPatternExists(const Pattern);

  // next
  bool isLineNextLine(const PreviousLine, const NextLine);  // implemented
  bool isLineNextLineS(const PreviousLine, const NextLine);
  std::optional<std::unordered_set<PreviousLine>> getPreviousLine(
      const NextLine);  // implemented
  std::optional<std::unordered_set<NextLine>> getNextLine(
      const PreviousLine);  // implemented
  std::optional<std::unordered_set<PreviousLine>> getPreviousLineS(
      const NextLine);
  std::optional<std::unordered_set<NextLine>> getNextLineS(const PreviousLine);

  // misc helpers
  std::optional<std::unordered_set<Line>> getLineForAssignVar(const Variable);
};

}  // namespace PKB
