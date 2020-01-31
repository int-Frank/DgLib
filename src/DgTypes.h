//@group Misc

//! @file DgTypes.h
//!
//! @author Frank Hart
//! @date 3/07/2016

#ifndef DGTYPES_H
#define DGTYPES_H

#define IS_FLAGGED(x, y) ((x & (1 << y)) != 0)

#define ITEM()
#define ERRORCODES \
  ITEM(None) \
  ITEM(Unknown) \
  ITEM(Failure) \
  ITEM(Undefined) \
  ITEM(OutOfBounds) \
  ITEM(Duplicate) \
  ITEM(FailedToOpenFile) \
  ITEM(BadInput) \
  ITEM(StreamNotOpen) \
  ITEM(FailedToAllocMem) \
  ITEM(Disallowed) \
  ITEM(InsufficientSpace) \
  ITEM(NullObject) \
  ITEM(WriteError)

namespace Dg
{
  typedef int ErrorCodeType;
  //! @ingroup DgUtility_constants
  //! Error codes
#undef ITEM
#define ITEM(x) x,
  enum class ErrorCode : ErrorCodeType
  {
    ERRORCODES
    COUNT
  };
#undef ITEM

  char const * ErrorCodeToString(ErrorCode);
}

#endif