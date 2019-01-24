#ifndef __QUERY_EVALUATOR_H
#define __QUERY_EVALUATOR_H
#include <string>
#include <memory> // future use of smart pointers
#include "pql.h"

/*
 * The main API for the PQL Query Engine (QE)
 * 
*/

namespace QE {
    class QueryResult {
        // Stub class
    };

    class QueryEvaluator {
        public:
            // Give a string containing PQL, makes the appropriate calls to PKB
            // and retrieves the QueryResult
            std::unique_ptr<QueryResult> makePqlQuery(std::string&);

    };
}

#endif