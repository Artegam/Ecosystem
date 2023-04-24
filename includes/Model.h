#ifndef MODEL_H
#define MODEL_H

#include <string>

using namespace std;

namespace Model {

  //interface
  ///Class Model -
  class GenericModel {
    // Operations
    public:
      GenericModel ();
      virtual string getRawData ();
  };

}

#endif
