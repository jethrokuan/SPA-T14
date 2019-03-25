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
  } else if (argLeftAsQuoteIdent && argRightAsSynAttr) {
    return handleQuoteIdentSynAttr(*argLeftAsQuoteIdent, *argRightAsSynAttr);
  } else if (argRightAsQuoteIdent && argLeftAsSynAttr) {
    return handleQuoteIdentSynAttr(*argRightAsQuoteIdent, *argLeftAsSynAttr);
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

bool WithEvaluator::handleNumberSynonym(unsigned int num, Synonym& synonym) {
  qc.addToSingleVariableConstraints(synonym.synonym,
                                    SingleConstraintSet{std::to_string(num)});
  return true;
}

bool WithEvaluator::handleNumberSynAttr(unsigned int num, SynAttr& synAttr) {
  // For all Number <-> SynAttr constraints: we don't need to do any extra work
  // All the attribute applications are no-ops
  // We MUST have accessed constant.value / <stmttype>.stmt# - no work to do
  // Can just add constraint: (c) -> (10) for e.g.
  qc.addToSingleVariableConstraints(synAttr.synonym.synonym,
                                    SingleConstraintSet{std::to_string(num)});
  return true;
}

bool WithEvaluator::handleQuoteIdentSynAttr(QuoteIdent& quoteIdent,
                                            SynAttr& synAttr) {
  // General algorithm: get all design entities for this synonym
  // Then, filter by a generic operation that applies the Attr on the synonym
  // and checks it against the quoteIdent passed in
  auto found_declaration =
      Declaration::findDeclarationForSynonym(declarations, synAttr.synonym)
          .value();
  auto design_entity_type = found_declaration.getDesignEntity();
  // If we have a variable or procedure: no - op, just constrain directly
  if (design_entity_type == DesignEntity::PROCEDURE ||
      design_entity_type == DesignEntity::VARIABLE) {
    qc.addToSingleVariableConstraints(
        synAttr.synonym.synonym, SingleConstraintSet{quoteIdent.quote_ident});
  } else {
    auto all_des = QueryExecutor::getSelect(pkb, design_entity_type);
    // Filter out values that don't match call to .procName/.varName for
    // call/read/print
    SingleConstraintSet filtered_des;
    std::copy_if(all_des.begin(), all_des.end(),
                 std::inserter(filtered_des, filtered_des.end()),
                 [&, this](const std::string de) {
                   return this->applyAttrToDesignEntityValue(
                              design_entity_type, de) == quoteIdent.quote_ident;
                 });
    qc.addToSingleVariableConstraints(synAttr.synonym.synonym, filtered_des);
  }
  return true;
}

std::string WithEvaluator::applyAttrToDesignEntityValue(
    const DesignEntity& de_type, const std::string& de) {
  // This function does not check for the attribute name since there is no
  // ambiguity
  switch (de_type) {
    case DesignEntity::CALL:
      return pkb->getCallProcedureFromLine(de).value();
    case DesignEntity::READ:
      return pkb->getReadVariableFromLine(de).value();
    case DesignEntity::PRINT:
      return pkb->getPrintVariableFromLine(de).value();
    default:
      std::cerr
          << "Received unexpected design entity type to apply attrName to\n";
      assert(false);
  }
}