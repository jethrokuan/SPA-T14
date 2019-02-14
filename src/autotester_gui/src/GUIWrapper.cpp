#include "GUIWrapper.h"

#include <fstream>
#include <iostream>

// a default constructor
GUIWrapper::GUIWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  spa_manager = new SPAManager();
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  std::ifstream input(filename);

  try {
    spa_manager->loadSimpleSource(input);
    std::cout << "parsed " << filename << std::endl;
  } catch (const std::runtime_error e) {
    std::cout << e.what() << std::endl;
    std::cout << "Failed to parse " << filename << std::endl;
  }
}

// method to evaluating a query
void GUIWrapper::evaluate(std::string query, std::list<std::string>& results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...

  std::cout << "query=  " << query << std::endl;
  try {
    spa_manager->query(query);
    results.push_back(
        query +
        "\nQuery result: ... NONE (BUT STUB AUTOTESTER_GUI IS WORKING)");
  } catch (const std::runtime_error e) {
    std::cout << e.what() << std::endl;
    std::cout << "Failed to parse query " << query << std::endl;
    results.push_back(query + "\nQuery result: ... FAILURE: " + e.what());
  };

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}

GUIWrapper::~GUIWrapper() { delete spa_manager; }
