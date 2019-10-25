//! @file DgTypes.h
//!
//! @author Frank Hart
//! @date 3/07/2016

#ifndef DGTYPES_H
#define DGTYPES_H

#define IS_FLAGGED(x, y) ((x & (1 << y)) != 0)

namespace Dg
{
  //! @ingroup DgUtility_constants
  //! Error codes
  enum ErrorCode : int
  {
    Err_None = 0,
    Err_Unknown,
    Err_Failure,
    Err_Undefined,
    Err_OutOfBounds,
    Err_Duplicate,
    Err_FailedToOpenFile,
    Err_BadInput,
    Err_StreamNotOpen,
    Err_FailedToAllocMem,
    Err_Disallowed,
    Err_InsufficientSpace,
    Err_COUNT
  };
}

#endif