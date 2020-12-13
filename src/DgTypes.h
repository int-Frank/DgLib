//@group Misc

//! @file DgTypes.h
//!
//! @author Frank Hart
//! @date 3/07/2016

#ifndef DGTYPES_H
#define DGTYPES_H

#define IS_FLAGGED(x, y) ((x & (1 << y)) != 0)

namespace Dg
{
  struct Element
  {
    enum
    {
      //Axis
      x       = 0,
      y       = 1,
      z       = 2,
      w       = 3,

      //Dimensions
      width   = 0,
      height  = 1,
      depth   = 2,

      //2D box edge
      xmin    = 0,
      xmax    = 1,
      ymin    = 2,
      ymax    = 3
    };
  };
}

#endif