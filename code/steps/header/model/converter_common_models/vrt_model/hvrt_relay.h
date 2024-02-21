#ifndef HVRT_RELAY_H
#define HVRT_RELAY_H

#include "header/model/converter_common_models/vrt_model/vrt_relay_model.h"
#include "header/block/integral_block.h"

class HVRT_RELAY : public VRT_RELAY_MODEL
{
    public:
        HVRT_RELAY(STEPS& toolkit);
        ~HVRT_RELAY();
        HVRT_RELAY(const HVRT_RELAY& model);
        virtual HVRT_RELAY& operator=(const HVRT_RELAY& model);
        void clear();
        void copy_from_const_model(const HVRT_RELAY& model);
    public:
        void initialize_vrt();
    public:
        virtual void check_vrt_status();
    private:
        double vth_hvrt;
};

#endif // HVRT_RELAY_H
