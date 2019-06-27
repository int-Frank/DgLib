
#include "Dg_Assert.h"

namespace Dg_Assert
{
  FailBehavior DefaultReportFailure(char const * condition,
    char const * file,  
    int line,
    char const * msg)
  {
    return Halt;
  }

  Handler gHandler(DefaultReportFailure);

  FailBehavior ReportFailure(char const * a_condition,
    char const * a_file,  
    int a_line,
    char const * a_msg)
  {
    return gHandler(a_condition, a_msg, a_line, a_file);
  }

  Handler GetHandler()
  {
    return gHandler;
  }

  void SetHandler(Handler a_newHandler)
  {
    gHandler = a_newHandler;
  }
}