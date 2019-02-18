#pragma once
#include <string>

// using string for line numbers since it's easier to just handle a single type
// procedure and variables names also can't begin with a number anyway
// in particular helps with storing both line numbers and procedure names
// for parent relationships
using Line = std::string;
using ParentLine = std::string;  // Line
using ChildLine = std::string;   // Line
using LineBefore = std::string;  // Line
using LineAfter = std::string;   // Line
using Variable = std::string;
using Procedure = std::string;
using Constant = std::string;
