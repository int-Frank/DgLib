//! @file DgQueryCommon.h

#ifndef DGQUERYCOMMON_H
#define DGQUERYCOMMON_H

namespace Dg
{
  //TODO Add prefix QC_
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