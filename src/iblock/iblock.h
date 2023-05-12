#ifndef __IBLOCK_COMMON_IBLOCK_H_
#define __IBLOCK_COMMON_IBLOCK_H_

#include <omnetpp.h>
#include "features.h"

#if defined(IBLOCK_EXPORT)
#define IBLOCK_API OPP_DLLEXPORT
#elif defined(IBLOCK_IMPORT)
#define IBLOCK_API OPP_DLLIMPORT
#else
#define IBLOCK_API
#endif

#endif
