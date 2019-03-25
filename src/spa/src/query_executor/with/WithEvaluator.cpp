#include "query_executor/with/WithEvaluator.h"
#include <cassert>
#include <string>
#include <unordered_set>
#include <vector>

#include "query_executor/query_executor.h"

bool WithEvaluator::evaluate() {
  argLeftAsQuoteIdent = std::get_if<QuoteIdent>(&argLeft.attr);
  argLeftAsNumber = std::get_if<unsigned int>(&argLeft.attr);
  argLeftAsSynonym = std::get_if<Synonym>(&argLeft.attr);
  argLeftAsSynAttr = std::get_if<SynAttr>(&argLeft.attr);
  argRightAsQuoteIdent = std::get_if<QuoteIdent>(&argRight.attr);
  argRightAsNumber = std::get_if<unsigned int>(&argRight.attr);
  argRightAsSynonym = std::get_if<Synonym>(&argRight.attr);
  argRightAsSynAttr = std::get_if<SynAttr>(&argRight.attr);

  return dispatch();
}

bool WithEvaluator::dispatch() {
  if (argLeftAsQuoteIdent && argRightAsQuoteIdent) {
    return handleBothArgsQuoteIdent();
  } else if (argLeftAsNumber && argRightAsNumber) {
    return handleBothArgsNumber();
  } else if (argLeftAsSynonym && argRightAsSynonym) {
    return handleBothArgsSynonym();
  } else if (argLeftAsNumber && argRightAsSynonym) {
    return handleNumberSynonym(*argLeftAsNumber, *argRightAsSynonym);
  } else if (argRightAsNumber && argLeftAsSynonym) {
    return handleNumberSynonym(*argRightAsNumber, *argLeftAsSynonym);
  } else if (argLeftAsNumber && argRightAsSynAttr) {
    return handleNumberSynAttr(*argLeftAsNumber, *argRightAsSynAttr);
  } else if (argRightAsNumber && argLeftAsSynAttr) {
    return handleNumberSynAttr(*argRightAsNumber, *argLeftAsSynAttr);
  } else {
    return false;
  }
}

bool WithEvaluator::handleBothArgsQuoteIdent() {
  // with "asdf" = "asdf"
  return (*argLeftAsQuoteIdent == *argRightAsQuoteIdent);
}

bool WithEvaluator::handleBothArgsNumber() {
  // with 2 = 3
  return (*argLeftAsNumber == *argRightAsNumber);
}

bool WithEvaluator::handleBothArgsSynonym() {
  // with n1 = n2, where both n1, n2 are prog_line
  // prog_line n1, n2; Select <n1, n2> with n1 = n2
  // Only allow values of n1 and n2 where n1 = n2
  // Our simplified case: {(n', n') | n' in prog_line}
  auto all_prog_lines = QueryExecutor::getSelect(pkb, DesignEntity::PROG_LINE);
  if (all_prog_lines.empty()) return false;

  // [1,2,3,4] --> {(1,1), (2,2), (3,3), (4,4)}
  PairedConstraintSet pcs;
  for (const auto& prog_line : all_prog_lines) {
    pcs.insert({prog_line, prog_line});
  }
  qc.addToPairedVariableConstraints(argLeftAsSynonym->synonym,
                                    argRightAsSynonym->synonym, pcs);
  return true;
}

bool WithEvaluator::handleNumberSynonym(unsigned int num,
                                        QE::Synonym& synonym) {
  qc.addToSingleVariableConstraints(
      synonym.synonym, std::unordered_set<std::string>{std::to_string(num)});
  return true;
}

bool WithEvaluator::handleNumberSynAttr(unsigned int num,
                                        QE::SynAttr& synattr) {
  // General algorithm: get all design entities for this synonym
  // Then, filter by a generic operation that applies the Attr on the synonym
  // and checks it against the

  // For all Number <-> SynAttr constraints: we don't need to do any extra work
  // All the attribute applications are no-ops
  // We MUST have accessed constant.value / <stmttype>.stmt# - no work to do
  // Can just add constraint: (c) -> (10) for e.g.
  qc.addToSingleVariableConstraints(
      synattr.synonym.synonym,
      std::unordered_set<std::string>{std::to_string(num)});
  return true;
}