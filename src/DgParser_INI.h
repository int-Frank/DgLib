//! @file DgParser_INI.h
//!
//! @author Frank Hart
//! @date 4/10/2015
//!
//! Class declaration: Parser_INI

#ifndef DGPARSER_INI_H
#define DGPARSER_INI_H

#include <string>
#include <map>

#include "DgTypes.h"

namespace Dg
{
  //! @ingroup DgUtility
  //!
  //! @class Parser_INI
  //!
  //! Basic parser for .ini files. An ini file is in the format 
  //!
  //!   key=value
  //!
  //! Comments are preceeded by a one of '#;'
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  class Parser_INI
  {
  public:

    Parser_INI();
    ~Parser_INI();
    Parser_INI(Parser_INI const &);
    Parser_INI & operator=(Parser_INI const &);

    //! Loads and parses an ini file.
    //!
    //! @return Err_None on success.
    ErrorCode Parse(std::string const & fileName);

    //! Get the token/value items successfully extracted from the ini file.
    std::map<std::string, std::string> const & GetItems() const;

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };


}

#endif