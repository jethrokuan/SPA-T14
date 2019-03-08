#include "structs/node.h"

#include <iostream>

RootNode::RootNode(std::vector<std::shared_ptr<ProcedureNode>> procList)
    : ProcList(std::move(procList)){};
bool RootNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const RootNode*>(&other);
  return casted_other != 0 &&
         std::equal(begin(this->ProcList), end(this->ProcList),
                    begin(casted_other->ProcList), end(casted_other->ProcList),
                    [](const auto t, const auto o) { return *t == *o; });
}
std::string RootNode::to_str() {
  std::string acc = "(RootNode ";
  for (const auto& proc : this->ProcList) {
    acc += proc->to_str();
    acc += " ";
  }
  return acc;
}

NumberNode::NumberNode(const std::string val) : Val(val){};
bool NumberNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const NumberNode*>(&other);
  return casted_other != 0 && this->Val.compare(casted_other->Val) == 0;
};
std::string NumberNode::to_str() { return "(NumberNode " + this->Val + ")"; }

VariableNode::VariableNode(const std::string name) : Name(name){};
bool VariableNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const VariableNode*>(&other);
  return casted_other != 0 && this->Name.compare(casted_other->Name) == 0;
};
std::string VariableNode::to_str() {
  return "(VariableNode " + this->Name + ")";
}

ReadNode::ReadNode(std::shared_ptr<VariableNode> var) : Var(std::move(var)){};
bool ReadNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ReadNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

std::string ReadNode::to_str() {
  return "(ReadNode " + this->Var->to_str() + ")";
}

CallNode::CallNode(std::string procName) : ProcName(std::move(procName)){};
bool CallNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const CallNode*>(&other);
  return casted_other != 0 &&
         this->ProcName.compare(casted_other->ProcName) == 0;
};

std::string CallNode::to_str() { return "(CallNode " + this->ProcName + ")"; }

PrintNode::PrintNode(std::shared_ptr<VariableNode> var) : Var(std::move(var)){};

bool PrintNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const PrintNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

std::string PrintNode::to_str() {
  return "(PrintNode " + this->Var->to_str() + ")";
}

ProcedureNode::ProcedureNode(const std::string name, ::StmtList stmtList)
    : Name(name), StmtList(std::move(stmtList)){};
bool ProcedureNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ProcedureNode*>(&other);
  return casted_other != 0 && this->Name.compare(casted_other->Name) == 0 &&
         std::equal(
             begin(this->StmtList), end(this->StmtList),
             begin(casted_other->StmtList), end(casted_other->StmtList),
             [](const auto& t, const auto& o) {
               return std::visit(
                   [](const auto& tp, const auto& op) { return *tp == *op; }, t,
                   o);
             });
};

std::string ProcedureNode::to_str() {
  std::string acc = "(ProcedureNode " + this->Name + " ";
  for (const auto& stmt : this->StmtList) {
    acc += std::visit([](const auto& s) { return s->to_str(); }, stmt);
  }
  acc += ")";
  return acc;
}

BinOpNode::BinOpNode(Expr left, Expr right, std::string op)
    : Left(std::move(left)), Right(std::move(right)), Op(op){};
bool BinOpNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const BinOpNode*>(&other);
  return casted_other != 0 &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->Left,
                    casted_other->Left) &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->Right,
                    casted_other->Right);
};
std::string BinOpNode::to_str() {
  std::string acc = "(BinOpNode " + this->Op + " ";
  acc += std::visit([](auto& s) { return s->to_str(); }, this->Left);
  acc + " ";
  acc += std::visit([](auto& s) { return s->to_str(); }, this->Right);
  acc += ")";
  return acc;
}

AssignNode::AssignNode(std::shared_ptr<VariableNode> var, Expr exp)
    : Var(std::move(var)), Exp(exp){};
bool AssignNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const AssignNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->Exp,
                    casted_other->Exp);
};
std::string AssignNode::to_str() {
  std::string acc = "(AssignNode ";
  acc += this->Var->to_str() + " ";
  acc += std::visit([](auto& s) { return s->to_str(); }, this->Exp);
  acc += ")";
  return acc;
}

RelExprNode::RelExprNode(RelFactor lhs, std::string op, RelFactor rhs)
    : LHS(lhs), Op(op), RHS(rhs){};
bool RelExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const RelExprNode*>(&other);
  return casted_other != 0 &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->LHS,
                    casted_other->LHS) &&
         std::visit([](auto& t, auto& o) { return *t == *o; }, this->RHS,
                    casted_other->RHS) &&
         this->Op.compare(casted_other->Op) == 0;
};
std::string RelExprNode::to_str() {
  std::string acc = "(RelExprNode " + this->Op;
  acc += std::visit([](auto& s) { return s->to_str(); }, this->LHS);
  acc += std::visit([](auto& s) { return s->to_str(); }, this->RHS);
  acc += ")";
  return acc;
}

CondExprNode::CondExprNode(std::shared_ptr<RelExprNode> relExpr)
    : RelExpr(std::move(relExpr)){};
CondExprNode::CondExprNode(std::shared_ptr<CondExprNode> condLHS)
    : CondLHS(std::move(condLHS)), Op("!"){};
CondExprNode::CondExprNode(std::shared_ptr<CondExprNode> condLHS,
                           std::string op,
                           std::shared_ptr<CondExprNode> condRHS)
    : CondLHS(std::move(condLHS)), Op(op), CondRHS(std::move(condRHS)){};
bool CondExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const CondExprNode*>(&other);
  return casted_other != 0 &&
         (this->RelExpr == casted_other->RelExpr ||
          *this->RelExpr == *casted_other->RelExpr) &&
         (this->CondLHS == casted_other->CondLHS ||
          *this->CondLHS == *casted_other->CondLHS) &&
         (this->CondRHS == casted_other->CondRHS ||
          *this->CondRHS == *casted_other->CondRHS) &&
         (this->Op.compare(casted_other->Op) == 0);
};
std::string CondExprNode::to_str() {
  std::string s;
  // just RelExpr:
  if (RelExpr != nullptr) {
    s = "(cond " + RelExpr->to_str() + ")";
  } else if (CondRHS == nullptr) {
    // ! (CondExpr)
    s = "(cond " + Op + " " + CondLHS->to_str() + ")";
  } else {
    // (CondExpr Op CondExpr)
    s = "(cond " + Op + " " + CondLHS->to_str() + " " + CondRHS->to_str() + ")";
  }
  return s;
}

WhileNode::WhileNode(std::shared_ptr<CondExprNode> condExpr,
                     ::StmtList stmtList)
    : CondExpr(std::move(condExpr)), StmtList(std::move(stmtList)){};
bool WhileNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const WhileNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
         std::equal(
             begin(this->StmtList), end(this->StmtList),
             begin(casted_other->StmtList), end(casted_other->StmtList),
             [](const auto& t, const auto& o) {
               return std::visit(
                   [](const auto& tp, const auto& op) { return *tp == *op; }, t,
                   o);
             });
};
std::string WhileNode::to_str() {
  std::string acc = "(WhileNode";
  acc += this->CondExpr->to_str();
  for (const auto& stmt : this->StmtList) {
    acc += std::visit([](const auto& s) { return s->to_str(); }, stmt);
  }
  acc += ")";
  return acc;
}

IfNode::IfNode(std::shared_ptr<CondExprNode> condExpr, StmtList stmtListThen,
               StmtList stmtListElse)
    : CondExpr(std::move(condExpr)),
      StmtListThen(std::move(stmtListThen)),
      StmtListElse(std::move(stmtListElse)){};
bool IfNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const IfNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
         std::equal(
             begin(this->StmtListThen), end(this->StmtListThen),
             begin(casted_other->StmtListThen), end(casted_other->StmtListThen),
             [](const auto& t, const auto& o) {
               return std::visit(
                   [](const auto& tp, const auto& op) { return *tp == *op; }, t,
                   o);
             }) &&
         std::equal(
             begin(this->StmtListElse), end(this->StmtListElse),
             begin(casted_other->StmtListElse), end(casted_other->StmtListElse),
             [](const auto& t, const auto& o) {
               return std::visit(
                   [](const auto& tp, const auto& op) { return *tp == *op; }, t,
                   o);
             });
  ;
};

std::string IfNode::to_str() {
  std::string acc = "(IfNode";
  acc += this->CondExpr->to_str();
  acc += "(";
  for (const auto& stmt : this->StmtListThen) {
    acc += std::visit([](const auto& s) { return s->to_str(); }, stmt);
  }
  acc += ")";
  acc += "(";
  for (const auto& stmt : this->StmtListElse) {
    acc += std::visit([](const auto& s) { return s->to_str(); }, stmt);
  }
  acc += ")";
  acc += ")";
  return acc;
}
