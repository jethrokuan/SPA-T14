#include "structs/node.h"

#include <iostream>

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overload(Ts...)->overload<Ts...>;

StmtListNode::StmtListNode(std::vector<StmtNode> stmtList)
    : StmtList(std::move(stmtList)){};
bool StmtListNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const StmtListNode*>(&other);
  return casted_other != 0 &&
         this->StmtList.size() == casted_other->StmtList.size() &&
         std::equal(begin(this->StmtList), end(this->StmtList),
                    begin(casted_other->StmtList), end(casted_other->StmtList),
                    [](const auto t, const auto o) {
                      return std::visit(overload{[](auto& tp, auto& op) {
                                          return *tp == *op;
                                        }},
                                        t, o);
                    });
};
std::string StmtListNode::to_str() {
  std::string acc = "(StmtListNode ";
  for (const auto& stmt : this->StmtList) {
    acc += std::visit([](auto& s) { return s->to_str(); }, stmt);
    acc += " ";
  }
  acc += ")";
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

PrintNode::PrintNode(std::shared_ptr<VariableNode> var) : Var(std::move(var)){};

bool PrintNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const PrintNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

std::string PrintNode::to_str() {
  return "(PrintNode " + this->Var->to_str() + ")";
}

ProcedureNode::ProcedureNode(const std::string name,
                             std::shared_ptr<StmtListNode> stmtList)
    : Name(name), StmtList(std::move(stmtList)){};
bool ProcedureNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ProcedureNode*>(&other);
  return casted_other != 0 && this->Name.compare(casted_other->Name) == 0 &&
         *this->StmtList == *casted_other->StmtList;
};

std::string ProcedureNode::to_str() {
  std::string acc = "(ProcedureNode " + this->Name + " ";
  acc += this->StmtList->to_str();
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
  // TODO(jethro): fix
  return "(CondExprNode)";
}

WhileNode::WhileNode(std::shared_ptr<CondExprNode> condExpr,
                     std::shared_ptr<StmtListNode> stmtList)
    : CondExpr(std::move(condExpr)), StmtList(std::move(stmtList)){};
bool WhileNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const WhileNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
         *this->StmtList == *casted_other->StmtList;
};
std::string WhileNode::to_str() {
  // TODO(jethro): fix
  std::string acc = "(WhileNode";
  acc += this->CondExpr->to_str();
  acc += this->StmtList->to_str();
  acc += ")";
  return acc;
}

IfNode::IfNode(std::shared_ptr<CondExprNode> condExpr,
               std::shared_ptr<StmtListNode> stmtListThen,
               std::shared_ptr<StmtListNode> stmtListElse)
    : CondExpr(std::move(condExpr)),
      StmtListThen(std::move(stmtListThen)),
      StmtListElse(std::move(stmtListElse)){};
bool IfNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const IfNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
         *this->StmtListThen == *casted_other->StmtListThen &&
         *this->StmtListElse == *casted_other->StmtListElse;
};

std::string IfNode::to_str() {
  // TODO(jethro): fix
  std::string acc = "(IfNode";
  acc += this->CondExpr->to_str();
  acc += this->StmtListThen->to_str();
  acc += this->StmtListElse->to_str();
  acc += ")";
  return acc;
}
