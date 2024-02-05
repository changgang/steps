#ifndef LVRT_H
#define LVRT_H

#include "header/model/converter_common_models/vrt_model/vrt_relay_model.h"
#include "header/block/integral_block.h"

class LVRT : public VRT_RELAY_MODEL
{
    public:
        LVRT(STEPS& toolkit);
        ~LVRT();
        LVRT(const LVRT& model);
        virtual LVRT& operator=(const LVRT& model);
        void clear();
        void copy_from_const_model(const LVRT& model);
    public:
        void initialize_vrt();
    public:
        virtual void check_vrt_status();
    private:
        double vth_lvrt;
};

#endif // LVRT_H
