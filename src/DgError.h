//@group Misc

//! @file DgERROR.h
//!
//! @author Frank Hart
//! @date 13/12/2020

#ifndef DGERROR_H
#define DGERROR_H

#include <stdint.h>

#define DG_ERROR_IF(cond, code)     do { if (cond)           { result = code; if (result != ::Dg::ErrorCode::None) goto epilogue; } while(0)
#define DG_ERROR_NULL(ptr, code)    do { if (ptr == nullptr) { result = code; if (result != ::Dg::ErrorCode::None) goto epilogue; } while(0)
#define DG_ERROR_SET(code)          do { result = code;                       if (result != ::Dg::ErrorCode::None) goto epilogue; } while(0)
#define DG_ERROR_CHECK(funcCall)    do { result = funcCall; if (result != ::Dg::ErrorCode::None) goto epilogue; } while(0)

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
  //! @ingroup DgUtility_constants
  //! Error codes
#undef ITEM
#define ITEM(x) x,
  enum class ErrorCode : uint32_t
  {
    ERRORCODES
    COUNT
  };
#undef ITEM

  char const * ErrorCodeToString(ErrorCode);
}

#endif