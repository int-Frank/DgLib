
#include <fstream>

#include "DgParser_INI.h"
#include "DgStringFunctions.h"

static char const g_commentChars[] = "#;";
static char const g_delim = '=';

namespace Dg
{
  class Parser_INI::PIMPL 
  {
  public:

    std::map<std::string, std::string> items;
  };

  Parser_INI::Parser_INI()
    : m_pimpl(new PIMPL())
  {

  }

  Parser_INI::~Parser_INI()
  {
    delete m_pimpl;
    m_pimpl = nullptr;
  }

  Parser_INI::Parser_INI(Parser_INI const & a_other)
    : m_pimpl(new PIMPL(*a_other.m_pimpl))
  {

  }

  Parser_INI & Parser_INI::operator=(Parser_INI const & a_other)
  {
    if (this != &a_other)
    {
      delete m_pimpl;
      m_pimpl = new PIMPL(*a_other.m_pimpl);
    }
    return *this;
  }

  static bool IsComment(std::string const & a_str)
  {
    std::string str(Trim(a_str));
    size_t strBegin = str.find_first_of(g_commentChars);
    return strBegin == 0;
  }

  static bool GetNameValue(std::string const & a_str,
                           std::string & a_name,
                           std::string & a_value)
  {
    std::string str(Trim(a_str));
    const size_t delimPos = str.find_first_of(g_delim);
    if (delimPos == std::string::npos)
    {
      a_name = str;
      a_value = "";
      return false;
    }

    a_name = str.substr(0, delimPos);
    a_value = str.substr(delimPos + 1, str.size() - 1);
    return (a_name != "") && (a_value != "");
  }

  std::map < std::string, std::string > const & Parser_INI::GetItems() const
  {
    return m_pimpl->items;
  }

  ErrorCode Parser_INI::Parse(std::string const & a_file)
  {
    m_pimpl->items.clear();

    std::ifstream file(a_file);
    if (!file.good())
    {
      return Err_FailedToOpenFile;
    }

    std::string line;
    while (std::getline(file, line))
    {
      line = Trim(line);
      if (IsComment(line))
      {
        continue;
      }

      std::string name, value;
      if (GetNameValue(line, name, value))
      {
        m_pimpl->items.insert(std::pair<std::string, std::string>(Trim(name), Trim(value)));
      }
    }
    return Err_None;
  }
}