#ifndef DG_ASSERT_H
#define DG_ASSERT_H

//----------------------------------------------------------------------------------
// USAGE
//----------------------------------------------------------------------------------
// Simply call
//            DG_ASSERT(condition)
//                      or
//            DG_ASSERT(condition, message)
//
// Use Dg_Assert::SetHandler() to set you custom assert handler.
//----------------------------------------------------------------------------------

#ifdef DG_ASSERTS_ENABLED
#define DG_HALT() __debugbreak() 

#define DG_ASSERT(...) EXPAND(GET_MACRO(__VA_ARGS__, DG_ASSERT_2, DG_ASSERT_1)(__VA_ARGS__))

#define DG_ASSERT_2(cond, message)  \
do \
{ \
  if (!(cond)) \
  { \
    if (Dg_Assert::ReportFailure(#cond, __FILE__, __LINE__, message) == Dg_Assert::Halt) \
      DG_HALT(); \
  } \
} while(0)

#define EXPAND(x) x

#define DG_ASSERT_1(cond)  \
do \
{ \
  if (!(cond)) \
  { \
    if (Dg_Assert::ReportFailure(#cond, __FILE__, __LINE__, nullptr) == Dg_Assert::Halt) \
      DG_HALT(); \
  } \
} while(0)

#define GET_MACRO(_1,_2,NAME,...) NAME

#else  
#define DG_ASSERT(cond) \
do { (void)sizeof(cond); } while(0)
#endif 

namespace Dg_Assert
{
  enum FailBehavior  
  {  
    Halt,  
    Continue,  
  };  

  typedef FailBehavior (*Handler)(char const * condition,
    char const * file,  
    int line,
    char const * msg);  
  Handler GetHandler();  
  void SetHandler(Handler);  

  FailBehavior ReportFailure(char const * condition,
    char const * file,  
    int line,
    char const * msg);
}

#endif