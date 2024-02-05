#ifndef HVRT_H
#define HVRT_H

#include "header/model/converter_common_models/vrt_model/vrt_model.h"
#include "header/block/integral_block.h"

class HVRT : public VRT_MODEL
{
    public:
        HVRT(STEPS& toolkit);
        ~HVRT();
        HVRT(const HVRT& model);
        virtual HVRT& operator=(const HVRT& model);
        void clear();
        void copy_from_const_model(const HVRT& model);
    public:
        void initialize_vrt();
    public:
        virtual void check_vrt_status();
    private:
        double vth_hvrt;
};

#endif // HVRT_H
