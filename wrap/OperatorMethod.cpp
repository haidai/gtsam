/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file OperatorMethod.cpp
 * @author Ellon Paiva Mendes (LAAS-CNRS)
 **/

#include "OperatorMethod.h"
#include "utilities.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace wrap;

/* ************************************************************************* */
void OperatorMethod::proxy_header(FileWriter& proxyFile) const {
  string upperName = matlabName();
  upperName[0] = toupper(upperName[0], locale());
  proxyFile.oss << "    function varargout = " << upperName << "(varargin)\n";
}

/* ************************************************************************* */
string OperatorMethod::wrapper_call(FileWriter& wrapperFile, Str cppClassName,
    Str matlabUniqueName, const ArgumentList& args) const {
  // check arguments
  // NOTE: for static functions, there is no object passed
  wrapperFile.oss << "  checkArguments(\"" << matlabUniqueName << "." << name_
      << "\",nargout,nargin," << args.size() << ");\n";

  // unwrap arguments, see Argument.cpp
  args.matlab_unwrap(wrapperFile, 0); // We start at 0 because there is no self object

  // call method and wrap result
  // example: out[0]=wrap<bool>(OperatorMethod(t));
  string expanded = cppClassName + "::" + name_;
  if (templateArgValue_)
    expanded += ("<" + templateArgValue_->qualifiedName("::") + ">");

  return expanded;
}

/* ************************************************************************* */
  void OperatorMethod::python_wrapper(FileWriter& wrapperFile, Str className) const
  {
    // operator <<
    if( operatorSymbol() == "<<" ) {
      // NOTE: we should use self_ns to especify the correct str. See https://mail.python.org/pipermail/cplusplus-sig/2005-February/008295.html
      wrapperFile.oss << "  .def(self_ns::str(self))\n";
      wrapperFile.oss << "  .def(repr(self))\n";
    } else {
      BOOST_FOREACH(const ArgumentList& argList, argLists_) {
        if(argList.empty()) {
          // The single argument should be the class itself
          wrapperFile.oss << "  .def(" << operatorSymbol() << "self)\n";
        } else if(argList.size() == 1){
          // At least one of the arguments should be the class.
          if( argList[0].type.name() == className)
            wrapperFile.oss << "  .def(self " << operatorSymbol() << " self)\n";
          else
            wrapperFile.oss << "  .def(self " << operatorSymbol() << " other<" << argList[0].type.name() << ">())\n";
        } else if(argList.size() == 2){
          if( argList[0].type.name() != className && argList[1].type.name() != className ) {
            std::stringstream ss; ss << "OperatorMethod::python_wrapper " << name_ << argList << " should have at least one argument of type " << className;
            throw std::runtime_error( ss.str() );
          }

          if( argList[0].type.name() == className && argList[1].type.name() == className )
            wrapperFile.oss << "  .def(self " << operatorSymbol() << " self)\n";
          else if (argList[0].type.name() == className)
            wrapperFile.oss << "  .def(self " << operatorSymbol() << " other<" << argList[1].type.name() << ">())\n";
          else
            wrapperFile.oss << "  .def(other<" << argList[0].type.name() << ">() " << operatorSymbol() << " self)\n";
        } else {
          std::stringstream ss; ss << "OperatorMethod::python_wrapper " << name_ << " has " << argList.size() << " arguments when it was expected 2 max";
          throw std::runtime_error( ss.str() );
        }
      }
    }

  }

std::string OperatorMethod::operatorSymbol() const
{
  // name_ format: operatorXX
  // indices:      0123456789
  return name_.substr(8);
}