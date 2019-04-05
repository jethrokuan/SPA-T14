#pragma once
#include <string>

namespace PKB {
// using string for line numbers since it's easier to just handle a single type
// procedure and variables names also can't begin with a number anyway
// in particular helps with storing both line numbers and procedure names
// for parent relationships
using Line = std::string;
using ParentLine = std::string;    // Line
using ChildLine = std::string;     // Line
using NextLine = std::string;      // Line
using PreviousLine = std::string;  // Line
using ModifyLine = std::string;    // Line
using UsesLine = std::string;      // Line
using LineBefore = std::string;    // Line
using LineAfter = std::string;     // Line
using Variable = std::string;
using Procedure = std::string;
using ProcedureCaller = std::string;  // Procedure
using ProcedureCallee = std::string;  // Procedure
using Constant = std::string;
using Pattern = std::string;
using ExprStr = std::string;

const Line INVALID_LINE_NUMBER = "-1";
}  // namespace PKB
