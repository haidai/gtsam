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
    // name_ format: operatorXX
    // indices:      0123456789

    // operator <<
    if( name_.substr(8) == "<<" )
    {
      // NOTE: we should use self_ns to especify the correct str. See https://mail.python.org/pipermail/cplusplus-sig/2005-February/008295.html
      wrapperFile.oss << "  .def(self_ns::str(self))\n";
      wrapperFile.oss << "  .def(repr(self))\n";
    }

  }
