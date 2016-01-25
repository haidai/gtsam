/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file Signature.h
 * @brief simple signature structs
 * @author Ellon Paiva Mendes (LAAS-CNRS)
 **/
#pragma once

#include "ReturnValue.h"
#include "Argument.h"

namespace wrap {

/** Simple signature struct.
 *
 */ 
struct Signature {
  ReturnValue retValue;
  ArgumentList argList;  

  Signature() {}

  Signature(const ArgumentList& argList, const ReturnValue& retValue) :
  	retValue(retValue), argList(argList) {}

  friend std::ostream& operator<<(std::ostream& os,
      const Signature& signature) {
      os << signature.retValue << signature.argList << std::endl;
    return os;
  }

};

/** SignatureGroup represents a set of signatures that may be identified by a main (longer) signature
 * 
 * For example, the signature 'void f(int x=0,int y=0)', is also valid for 
 * 'void f()' and 'void f(int)', so we can group everything under 
 * 'void f(int x=0,int y=0)'.
 * 
 * This structure was created to be is used to write proper 
 * BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS macro calls in the python wrapper.
 *
 * The wrapper interface should define all overloaded functions. So, for the 
 * example above the interface should define:
 *   void f();
 *   void f(int);
 *   void f(int,int);
 * 
 * The 'first' value is the main signature, and 'second' is a list of all 
 * signatures (including the main one).
 */
struct SignatureGroup: public std::pair<Signature, std::vector<Signature> > {

  const Signature& mainSignature() const { return this->first; }

  const std::vector<Signature>& signatureList() const { return this->second; }

  /// number of arguments of the shortest signature in the group
  size_t minArgs() const;
  /// number of arguments of the longest signature in the group
  size_t maxArgs() const;

  // Named constructor to initialize a new group from a single signature
  static SignatureGroup FromNewSignature(const ArgumentList& argList, const ReturnValue& retValue);

};

/**
 * Structure used to add a signature to the correct group.
 */
struct SignatureGroupList: public std::vector<SignatureGroup> {

  void addSignature(const ArgumentList& argList, const ReturnValue& retValue);

};

} // namespace wrap