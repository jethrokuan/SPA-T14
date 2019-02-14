#ifndef GUIWRAPPER_H
#define GUIWRAPPER_H

#include <iostream>
#include <list>
#include <string>

#include "spa_manager/spa_manager.h"

class GUIWrapper {
 private:
  SPAManager* spa_manager;

 public:
  // default constructor
  GUIWrapper();

  // destructor
  ~GUIWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
