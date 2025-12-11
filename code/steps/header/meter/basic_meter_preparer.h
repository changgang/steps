#ifndef BASIC_METER_PREPARER_H
#define BASIC_METER_PREPARER_H

#include "header/meter/meter.h"
#include "header/STEPS.h"

namespace basic_meter_preparer
{
    bool prepare_wt_generator_meter(STEPS& toolkit, METER& meter, const DEVICE_ID& device_id);
}
#endif // BASIC_METER_PREPARER_H
