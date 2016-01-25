/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file OperatorMethod.h
 * @brief describes and generates code for methods
 * @author Ellon Paiva Mendes (LAAS-CNRS)
 **/

#pragma once

#include "MethodBase.h"

namespace wrap {

/// OperatorMethod class
class OperatorMethod: public MethodBase {

  bool is_const_;

public:

  typedef const std::string& Str;

  virtual bool isStatic() const {
    return false;
  }

  virtual bool isConst() const {
    return is_const_;
  }

  friend std::ostream& operator<<(std::ostream& os, const OperatorMethod& m) {
    for (size_t i = 0; i < m.nrOverloads(); i++)
      os << m.returnVals_[i] << " " << m.name_ << m.argLists_[i];
    return os;
  }

  // emit python wrapper
  void python_wrapper(FileWriter& wrapperFile, Str className) const;

private:

  // Emit method header
  void proxy_header(FileWriter& proxyFile) const;

  virtual std::string wrapper_call(FileWriter& wrapperFile, Str cppClassName,
      Str matlabUniqueName, const ArgumentList& args) const;

  std::string operatorSymbol() const;
};

} // \namespace wrap

