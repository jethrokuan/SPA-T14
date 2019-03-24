#include "query_executor/with/WithEvaluator.h"
#include <cassert>
#include <string>
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
  } else {
    return false;
  }
}

bool WithEvaluator::handleBothArgsQuoteIdent() {
  return (*argLeftAsQuoteIdent == *argRightAsQuoteIdent);
}

bool WithEvaluator::handleBothArgsNumber() {
  return (*argLeftAsNumber == *argRightAsNumber);
}