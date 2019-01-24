#include "query_preprocessor.h"
#include "pql.h"
#include <iostream>

using namespace QE;
std::unique_ptr<Query> QueryPreprocessor::getQuery(std::string& pql_query_string) {
    std::cout << pql_query_string << std::endl;
    return Query::makeStubQuery();
}