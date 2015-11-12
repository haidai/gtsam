/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file Signature.ccp
 * @author Ellon Paiva Mendes (LAAS-CNRS)
 **/

#include "Signature.h"
 
using namespace std;
using namespace wrap;

 /* ************************************************************************* */
size_t SignatureGroup::minArgs() const
{
  std::vector<size_t> sizes;

  BOOST_FOREACH(const Signature& sig, this->second) {
    sizes.push_back(sig.argList.size());
  }

  return *std::min_element(sizes.begin(),sizes.end());
}

/* ************************************************************************* */
size_t SignatureGroup::maxArgs() const
{
  std::vector<size_t> sizes;

  BOOST_FOREACH(const Signature& sig, this->second) {
    sizes.push_back(sig.argList.size());
  }

  return *std::max_element(sizes.begin(),sizes.end());
}

/* ************************************************************************* */
SignatureGroup SignatureGroup::FromNewSignature(const ArgumentList& argList, const ReturnValue& retValue)
{
  Signature sig(argList,retValue);

  SignatureGroup sigGroup;
  sigGroup.first = sig;
  sigGroup.second.push_back(sig);

  return sigGroup;
}


/* ************************************************************************* */
void SignatureGroupList::addSignature(const ArgumentList& argList, const ReturnValue& retValue)
{
  SignatureGroupList::iterator it;
  for(it = this->begin(); it != this->end(); ++it)
  {
  	// First check the return value
  	if(it->first.retValue == retValue)
  	{
	    if(argList.isSubsequenceOf(it->first.argList))
	    {
	      // 'argList' matches a existent group signature. Add 'argList' to that group.
	      it->second.push_back(Signature(argList,retValue));
	      return;
	    }
	    else if(it->first.argList.isSubsequenceOf(argList))
	    {
	      // 'argList' is a longer signature that globs an existing group signature.
	      // Change argument list of the main signature 'argList' and add 'argList' to the group
	      it->first.argList = argList;
	      it->second.push_back(Signature(argList,retValue));
	      return;
	    }
	}
  }

  // If we reach the end then the parameters compose a new signature, so we add them as a new group
  if(it == this->end())
  {
    this->push_back( SignatureGroup::FromNewSignature(argList,retValue) );
  } 

}
