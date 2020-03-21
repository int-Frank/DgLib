//@group Math

//! @file DgQueryCommon.h

#ifndef DGQUERYCOMMON_H
#define DGQUERYCOMMON_H

namespace Dg
{
  enum QueryCode
  {
    QC_Fail = 0,
    QC_Success,
    QC_Parallel,
    QC_Overlapping,
    QC_Intersecting,
    QC_NotIntersecting,
    QC_CompletelyInside,
    QC_CompletelyOutside
  };
}

#endif