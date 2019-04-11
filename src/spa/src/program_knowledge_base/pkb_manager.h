#pragma once

#include <cassert>
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
  std::optional<std::string> getElemFromMap(
      const std::unordered_map<std::string, std::string> &, const std::string);

  bool isLineNextLineTH(const Line, const Line,
                        std::shared_ptr<std::unordered_set<Line>>);
  void getNextLineTH(const Line, std::shared_ptr<std::unordered_set<Line>>);
  void getPreviousLineTH(const Line, std::shared_ptr<std::unordered_set<Line>>);

  // helper for affects
  bool isLineAffectsLineH(const Line, const UsesLine, const Variable,
                          const bool, const ModifyLine,
                          std::shared_ptr<std::unordered_set<Line>>);
  bool isLineAffectsLineTH(
    const ModifyLine modify_line, const UsesLine target_line,
    std::shared_ptr<std::unordered_set<Line>> visited);
  void getAffectModifiesLineH(
      const Line cur_line, const Variable target_var,
      const ModifyLine source_line,
      std::shared_ptr<std::unordered_set<Line>> modifies_set);
  void getAffectModifiesLineH(
      const Line cur_line, const Variable target_var,
      const bool first_iteration, const ModifyLine source_line,
      std::shared_ptr<std::unordered_set<Line>> visited,
      std::shared_ptr<std::unordered_set<Line>> modifies_set);
  void getAffectModifiesLineTH(
      const UsesLine uses_line,
      std::shared_ptr<std::unordered_set<Line>> modifies_set,
      std::shared_ptr<std::unordered_set<Line>> visited);
  void getAffectUsesLineH(const Line cur_line, const Variable target_var,
                          const ModifyLine source_line,
                          std::shared_ptr<std::unordered_set<Line>> uses_set);
  void getAffectUsesLineH(const Line cur_line, const Variable target_var,
                          const bool first_iteration,
                          const ModifyLine source_line,
                          std::shared_ptr<std::unordered_set<Line>> visited,
                          std::shared_ptr<std::unordered_set<Line>> uses_set);
  void getAffectUsesLineTH(
      const ModifyLine modify_line,
      std::shared_ptr<std::unordered_set<Line>> uses_set,
      std::shared_ptr<std::unordered_set<Line>> visited);
  // cache
  std::unordered_map<ModifyLine, std::unordered_set<UsesLine>>
      modify_uses_affects_cache =
          std::unordered_map<ModifyLine, std::unordered_set<UsesLine>>();
  std::unordered_map<UsesLine, std::unordered_set<ModifyLine>>
      uses_modify_affects_cache =
          std::unordered_map<UsesLine, std::unordered_set<ModifyLine>>();

 public:
  PKBManager(const AST ast);
  ~PKBManager();

  // API exposed to Query Executor

  // is design entity set empty
  bool isVariableSetEmpty();
  bool isAssignSetEmpty();
  bool isPrintSetEmpty();
  bool isReadSetEmpty();
  bool isWhileSetEmpty();
  bool isIfSetEmpty();
  bool isConstantSetEmpty();
  bool isCallSetEmpty();

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
  bool isCallExists(const Line);

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
  std::unordered_set<Line> getCallSet();

  // is relationship set empty
  bool isLineNextLineMapEmpty();
  bool isProcedureCallProcedureSetEmpty();
  bool isProcedureCallProcedureTSetEmpty();
  bool isLineFollowLineSetEmpty();
  bool isLineFollowLineTSetEmpty();
  bool isLineParentLineSetEmpty();
  bool isLineParentLineTSetEmpty();
  bool isLineUsesVarSetEmpty();
  bool isLineModifiesVarSetEmpty();
  bool isProcedureUsesVarSetEmpty();
  bool isProcedureModifiesVarSetEmpty();

  // is relationship valid
  bool isProcedureCallsProcedure(const ProcedureCaller, const ProcedureCallee);
  bool isProcedureCallsProcedureT(const ProcedureCaller, const ProcedureCallee);
  bool isLineFollowLine(const LineBefore, const LineAfter);
  bool isLineFollowLineT(const LineBefore, const LineAfter);
  bool isLineParentLine(const ParentLine, const ChildLine);
  bool isLineParentLineT(const ParentLine, const ChildLine);
  bool isProcedureModifiesVar(const Procedure, const Variable);
  bool isLineModifiesVar(const Line, const Variable);
  bool isProcedureUsesVar(const Procedure, const Variable);
  bool isLineUsesVar(const Line, const Variable);

  // get relationship mapping
  std::optional<std::unordered_set<ProcedureCallee>> getCalleeProcedures(
      const ProcedureCaller);
  std::optional<std::unordered_set<ProcedureCaller>> getCallerProcedures(
      const ProcedureCallee);
  std::optional<std::unordered_set<ProcedureCallee>> getCalleeProceduresT(
      const ProcedureCaller);
  std::optional<std::unordered_set<ProcedureCaller>> getCallerProceduresT(
      const ProcedureCallee);

  std::optional<LineAfter> getFollowingLine(const LineBefore);
  std::optional<LineBefore> getBeforeLine(const LineAfter);
  std::optional<std::unordered_set<LineAfter>> getFollowingLineT(
      const LineBefore);
  std::optional<std::unordered_set<LineBefore>> getBeforeLineT(const LineAfter);

  std::optional<ParentLine> getParentLine(const ChildLine);
  std::optional<std::unordered_set<ChildLine>> getChildLine(const ParentLine);
  std::optional<std::unordered_set<ParentLine>> getParentLineT(const ChildLine);
  std::optional<std::unordered_set<ChildLine>> getChildLineT(const ParentLine);

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

  // assign pattern
  std::optional<std::unordered_set<Line>>
  getAssignPatternCompleteMatchLinesWithVar(const Variable, const Pattern);
  std::optional<std::unordered_set<Line>>
  getAssignPatternPartialMatchLinesWithVar(const Variable, const Pattern);
  std::optional<std::unordered_set<Line>> getAssignPatternCompleteMatchLines(
      const Pattern);
  std::optional<std::unordered_set<Line>> getAssignPatternPartialMatchLines(
      const Pattern);
  std::optional<std::unordered_set<std::pair<Line, Variable>, pair_hash>>
  getAssignPatternCompleteMatchLinesAndVars(const Pattern);
  std::optional<std::unordered_set<std::pair<Line, Variable>, pair_hash>>
  getAssignPatternPartialMatchLinesAndVars(const Pattern);
  std::unordered_set<std::pair<Line, Variable>, pair_hash>
  getAllAssignPatternLinesAndVars();
  bool isAssignPatternExists(const Pattern);

  // if pattern
  std::optional<std::unordered_set<Line>> getIfPatternLine(const Variable);
  std::optional<std::unordered_set<Variable>> getIfPatternVariable(const Line);
  std::unordered_set<std::pair<Line, Variable>, pair_hash>
  getAllIfPatternLinesAndVars();
  bool isIfPatternExists(const Line, const Variable);

  // while pattern
  std::optional<std::unordered_set<Line>> getWhilePatternLine(const Variable);
  std::optional<std::unordered_set<Variable>> getWhilePatternVariable(
      const Line);
  std::unordered_set<std::pair<Line, Variable>, pair_hash>
  getAllWhilePatternLinesAndVars();
  bool isWhilePatternExists(const Line, const Variable);

  // next
  bool isLineNextLine(const PreviousLine, const NextLine);
  bool isLineNextLineT(const PreviousLine, const NextLine);
  std::optional<std::unordered_set<PreviousLine>> getPreviousLine(
      const NextLine);
  std::optional<std::unordered_set<NextLine>> getNextLine(const PreviousLine);
  std::optional<std::unordered_set<PreviousLine>> getPreviousLineT(
      const NextLine);
  std::optional<std::unordered_set<NextLine>> getNextLineT(const PreviousLine);

  // affects
  bool isLineAffectsLine(const ModifyLine, const UsesLine);
  bool isLineAffectsLineT(const ModifyLine, const UsesLine);
  std::optional<std::unordered_set<ModifyLine>> getAffectModifiesLine(
      const UsesLine);
  std::optional<std::unordered_set<UsesLine>> getAffectUsesLine(
      const ModifyLine);
  std::optional<std::unordered_set<ModifyLine>> getAffectModifiesLineT(
      const UsesLine);
  std::optional<std::unordered_set<UsesLine>> getAffectUsesLineT(
      const ModifyLine);

  // CFG
  std::unordered_map<std::string, std::unordered_set<std::string>> getCFG();

  // misc helpers
  std::optional<std::unordered_set<Line>> getLineForAssignVar(const Variable);
  std::optional<Procedure> getProcedureFromLine(const Line);
  std::optional<Procedure> getCallProcedureFromLine(const Line);
  std::optional<Variable> getReadVariableFromLine(const Line);
  std::optional<Variable> getPrintVariableFromLine(const Line);
  std::optional<Variable> getModifyVariableFromAssignLine(const ModifyLine);
  std::optional<std::unordered_set<Variable>> getUsesVariableFromAssignLine(
      const UsesLine);
  bool isFromSameProcedure(const Line, const Line);
  bool isLineAffectsVariable(const Line, const Variable);
  // cache to be cleared after every query line
  void clearCache();
};

}  // namespace PKB
