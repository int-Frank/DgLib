//! @file DgTypes.h
//!
//! @author Frank Hart
//! @date 3/07/2016

#ifndef DGTYPES_H
#define DGTYPES_H

#define IS_FLAGGED(x, y) static_cast<bool>(x & (1 << y))

namespace Dg
{
  //! @ingroup DgUtility_constants
  //! Error codes
  enum ErrorCode
  {
    Err_None = 0,
    Err_Failure,
    Err_Undefined,
    Err_OutOfBounds,
    Err_Duplicate,
    Err_FailedToOpenFile,
    Err_BadInput
  };

  enum LogLevel
  {
    LL_Debug,
    LL_Info,
    LL_Warning,
    LL_Error,
    LL_Critical
  };
}

#endif